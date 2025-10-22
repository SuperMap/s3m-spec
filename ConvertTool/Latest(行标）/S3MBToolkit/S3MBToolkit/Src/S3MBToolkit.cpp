#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#include "S3MBToolkit.h"
#include "ProcessTools.h"
#include "Dialog3DTilesToS3MB.h"
#include "DialogS3MBTo3DTiles.h"
#include "DialogOSGBToS3MB.h"
#include "DialogOBJToS3MB.h"
#include "DialogI3SToS3MB.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTextCodec>
#include "S3MBToolkit.h"
using namespace S3MB;
QTextCodec* codec1 = QTextCodec::codecForName("GBK");
#ifdef WIN32
QString t1 = "3DTiles转S3MB";
QString t2 = "S3MB转3DTiles";
QString t3 = "OSGB转S3MB";
QString t4 = "OBJ转S3MB";
QString t5 = "I3S转S3MB";
#else
QString t1 = codec1->toUnicode("3DTiles转S3MB");
QString t2 = codec1->toUnicode("S3MB转3DTiles");
QString t3 = codec1->toUnicode("OSGB转S3MB");
QString t4 = codec1->toUnicode("OBJ转S3MB");
QString t5 = codec1->toUnicode("I3S转S3MB");
#endif

S3MBToolkit::S3MBToolkit(QWidget *parent):
      QMainWindow(parent)
{
    ui.setupUi(this);

	QWidget* widget = new QWidget(this);
	QVBoxLayout* mainLayout = new QVBoxLayout();
	QHBoxLayout* horLayout = new QHBoxLayout();
	QVBoxLayout* vctLayout = new QVBoxLayout();
	horLayout->addSpacing(20);
	vctLayout->addSpacing(20);

	QLabel* label = new QLabel(this);
	QString title;
#ifdef WIN32
	title = "请选择工具:";
#else
	title = codec1->toUnicode("请选择工具:");
#endif
    label->setText(title);
	horLayout->addWidget(label);

	QPushButton* button3DTilesToS3MB = new QPushButton(this);
    button3DTilesToS3MB->setText(t1);
    connect(button3DTilesToS3MB, SIGNAL(clicked()), this, SLOT(handle3DTilesToS3MBClickedEvent()));
	vctLayout->addWidget(button3DTilesToS3MB);
	QPushButton* buttonS3MBTo3DTiles = new QPushButton(this);
    buttonS3MBTo3DTiles->setText(t2);
    connect(buttonS3MBTo3DTiles, SIGNAL(clicked()), this, SLOT(handleS3MBTo3DTilesClickedEvent()));
	vctLayout->addWidget(buttonS3MBTo3DTiles);

	QPushButton* buttonOSGBToS3MB = new QPushButton(this);
    buttonOSGBToS3MB->setText(t3);
    connect(buttonOSGBToS3MB, SIGNAL(clicked()), this, SLOT(handleOSGBToS3MBClickedEvent()));
	vctLayout->addWidget(buttonOSGBToS3MB);

	QPushButton* buttonOBJToS3MB = new QPushButton(this);
    buttonOBJToS3MB->setText(t4);
    connect(buttonOBJToS3MB, SIGNAL(clicked()), this, SLOT(handleOBJToS3MBClickedEvent()));
	vctLayout->addWidget(buttonOBJToS3MB);

	QPushButton* buttonI3SToS3MB = new QPushButton(this);
	buttonI3SToS3MB->setText(t5);
	connect(buttonI3SToS3MB, SIGNAL(clicked()), this, SLOT(handleI3SToS3MBClickedEvent()));
	vctLayout->addWidget(buttonI3SToS3MB);

	mainLayout->addLayout(horLayout);
	mainLayout->addLayout(vctLayout);
	mainLayout->addStretch();
	widget->setLayout(mainLayout);
	this->setCentralWidget(widget);
}

void S3MBToolkit::handle3DTilesToS3MBClickedEvent()
{
	Dialog3DTilesToS3MB dialog;
    dialog.setWindowTitle(t1);
	dialog.setFixedSize(526, 400);
	dialog.exec();
}

void S3MBToolkit::handleS3MBTo3DTilesClickedEvent()
{
	DialogS3MBTo3DTiles dialog;
    dialog.setWindowTitle(t2);
	dialog.setFixedSize(526, 400);
	dialog.exec();
}

void S3MBToolkit::handleOSGBToS3MBClickedEvent()
{
	DialogOSGBToS3MB dialog;
    dialog.setWindowTitle(t3);
	dialog.setFixedSize(526, 400);
	dialog.exec();
}

void S3MBToolkit::handleOBJToS3MBClickedEvent()
{
	DialogOBJToS3MB dialog;
    dialog.setWindowTitle(t4);
	dialog.setFixedSize(526, 400);
	dialog.exec();
}

void S3MBToolkit::handleI3SToS3MBClickedEvent()
{
	DialogI3SToS3MB dialog;
	dialog.setWindowTitle(t4);
	dialog.setFixedSize(526, 400);
	dialog.exec();
}