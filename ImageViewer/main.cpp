#include "ImageViewer.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	ImageViewer w;
	a.setWindowIcon(QIcon(":/ImageViewer/Resources/picture.png"));
	w.show();
	return a.exec();
}
