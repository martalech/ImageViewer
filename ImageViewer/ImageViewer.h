#pragma once
#include <QtWidgets/QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "ui_ImageViewer.h"

class ImageViewer : public QMainWindow
{
	Q_OBJECT

public:
	ImageViewer(QWidget *parent = Q_NULLPTR);
	void getImage();
	void saveImage();

	virtual void resizeEvent(QResizeEvent *event);

	void crop();
	void rotate();
	void flipHorizontally();
	void flipVertically();

	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);

	void checkChanges();
	void disableModifyingActions();
	void enableModifyingActions();
protected:
	virtual void closeEvent(QCloseEvent *event) override;
private:
	Ui::ImageViewerClass ui;
	QPixmap pixmap;
	QMovie *movie;
	QPixmap originalPixmap;
	QString path;
	QRubberBand *selection;
	QPoint point;
	bool click;
};

