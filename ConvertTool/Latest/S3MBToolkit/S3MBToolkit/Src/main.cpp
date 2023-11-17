#include "S3MBToolkit.h"
#include <QtWidgets/QApplication>
#include "ProcessTools.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	S3MBToolkit w;
	w.show();

	return a.exec();
}
