#include "ImageViewer.h"

ImageViewer::ImageViewer(QWidget *parent)
	: QMainWindow(parent), click(false)
{	
	ui.setupUi(this);

	connect(ui.actionOpen_file, &QAction::triggered, this, &ImageViewer::getImage);
	connect(ui.actionCrop, &QAction::triggered, this, &ImageViewer::crop);
	connect(ui.actionSave_file, &QAction::triggered, this, &ImageViewer::saveImage);
	connect(ui.actionRotate_90_clockwise, &QAction::triggered, this, &ImageViewer::rotate);
	connect(ui.actionFlip_horizontally, &QAction::triggered, this, &ImageViewer::flipHorizontally);
	connect(ui.actionFlip_vertically, &QAction::triggered, this, &ImageViewer::flipVertically);
}

void ImageViewer::getImage()
{
	if (!pixmap.isNull())
		checkChanges();
	path = QFileDialog::getOpenFileName(this, "Choose an image", "C://", "Images (*.png *.jpg *.jpeg *.bmp *.gif)");
	if (path.isEmpty())
	{
		return;
	}
	pixmap = QPixmap(path);
	originalPixmap = pixmap;
	if (pixmap.isNull())
	{
		QMessageBox::information(this, "Error", "Unable to load image");
		return;
	}
	ui.labelImage->setSizePolicy(QSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored));
	ui.labelImage->setPixmap(pixmap.scaled(ui.labelImage->size(), 
			Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void ImageViewer::saveImage()
{
	QString filePath = QFileDialog::getSaveFileName(this, "Save image", path, "Images (*.png *.jpg *.jpeg *.bmp *.gif)");
	if (!filePath.isEmpty())
	{
		if (!pixmap.save(filePath))
		{
			QMessageBox::information(this, "Error", "Unable to save the image");
			return;
		}
	}
	else
	{
		return;
	}
}

void ImageViewer::resizeEvent(QResizeEvent *event)
{
	QWidget::resizeEvent(event);
	const QPixmap* labelPixmap = ui.labelImage->pixmap();
	if (labelPixmap != nullptr)
	{
		ui.labelImage->setPixmap(pixmap.scaled(ui.labelImage->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
	}
}

void ImageViewer::crop()
{
	click = !click;
}

void ImageViewer::rotate()
{
	QMatrix rotateMatrix;
	rotateMatrix.rotate(90);
	pixmap = pixmap.transformed(rotateMatrix);
	ui.labelImage->setPixmap(pixmap.scaled(ui.labelImage->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void ImageViewer::flipHorizontally()
{
	pixmap = pixmap.fromImage(pixmap.toImage().mirrored());
	QMatrix rotateMatrix;
	rotateMatrix.rotate(180);
	pixmap = pixmap.transformed(rotateMatrix);
	ui.labelImage->setPixmap(pixmap.scaled(ui.labelImage->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void ImageViewer::flipVertically()
{
	pixmap = pixmap.fromImage(pixmap.toImage().mirrored());
	ui.labelImage->setPixmap(pixmap.scaled(ui.labelImage->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void ImageViewer::mouseMoveEvent(QMouseEvent * event)
{
	if (click)
	{
		QPoint nextPoint = ui.labelImage->mapFromGlobal(event->globalPos());
		selection->setGeometry(QRect(point, nextPoint).normalized());
	}
	else
		QWidget::mouseMoveEvent(event);
}

void ImageViewer::mouseReleaseEvent(QMouseEvent * event)
{
	if (click)
	{
		const QPixmap* labelPixmap = ui.labelImage->pixmap();
		if (labelPixmap != nullptr)
		{
			int dx = (ui.labelImage->width() - labelPixmap->width()) >> 1;
			int dy = (ui.labelImage->height() - labelPixmap->height()) >> 1;
			double scale = (double)pixmap.width() / (double)labelPixmap->width();

			QRect cutRect = selection->frameGeometry().translated(-dx, -dy);
			QSize origSize = cutRect.size();
			cutRect.setTopLeft(cutRect.topLeft() * scale);
			cutRect.setSize(origSize * scale);

			pixmap = pixmap.copy(cutRect);
			ui.labelImage->setPixmap(pixmap.scaled(ui.labelImage->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
		}
		selection->hide();
		click = false;
	}
	else
		QWidget::mouseReleaseEvent(event);
}

void ImageViewer::mousePressEvent(QMouseEvent *event)
{
	if (click)
	{
		point = ui.labelImage->mapFromGlobal(event->globalPos());
		selection = new QRubberBand(QRubberBand::Rectangle, ui.labelImage);
		selection->setGeometry(QRect(point, point));
		selection->show();
	}
	else
		QWidget::mousePressEvent(event);
}

void ImageViewer::closeEvent(QCloseEvent *event)
{
	checkChanges();
	QMainWindow::closeEvent(event);
}

void ImageViewer::checkChanges()
{
	if (pixmap.toImage() != originalPixmap.toImage())
	{
		QMessageBox message;
		message.setText("The image has been modified.");
		message.setInformativeText("Do you want to save changes?");
		message.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
		message.setIcon(QMessageBox::Warning);
		int answer = message.exec();
		switch (answer)
		{
		case QMessageBox::Yes:
			saveImage();
			break;
		case QMessageBox::No:
			break;
		case QMessageBox::Cancel:
			return;
		}
	}
}

void ImageViewer::disableModifyingActions()
{
	ui.actionCrop->setEnabled(false);
	ui.actionFlip_horizontally->setEnabled(false);
	ui.actionFlip_vertically->setEnabled(false);
	ui.actionRotate_90_clockwise->setEnabled(false);
	ui.actionSave_file->setEnabled(false);
}

void ImageViewer::enableModifyingActions()
{
	ui.actionCrop->setEnabled(true);
	ui.actionFlip_horizontally->setEnabled(true);
	ui.actionFlip_vertically->setEnabled(true);
	ui.actionRotate_90_clockwise->setEnabled(true);
	ui.actionSave_file->setEnabled(true);
}

