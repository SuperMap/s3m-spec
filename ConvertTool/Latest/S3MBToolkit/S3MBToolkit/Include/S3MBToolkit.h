#pragma once

#ifdef WIN32
#include <QtWidgets/QMainWindow>
#else
#include <QMainWindow>
#endif
#include "ui_S3MBToolkit.h"

class S3MBToolkit : public QMainWindow
{
	Q_OBJECT

public:
	S3MBToolkit(QWidget *parent = Q_NULLPTR);

private slots:
	void handle3DTilesToS3MBClickedEvent();
	void handleS3MBTo3DTilesClickedEvent();
    void handleOSGBToS3MBClickedEvent();
    void handleOBJToS3MBClickedEvent();

private:
    Ui::S3MBToolkit ui;
};
