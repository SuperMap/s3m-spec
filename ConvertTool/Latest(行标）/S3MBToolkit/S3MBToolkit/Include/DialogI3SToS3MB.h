#pragma once
#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QButtonGroup>
#include <QProgressDialog>
#include <QTextCodec>
#include <QFileDialog>

class DialogI3SToS3MB : public QDialog
{
	Q_OBJECT

public:
	DialogI3SToS3MB(QWidget *parent = Q_NULLPTR);

private slots:
	void handleTextEditedEvent();
	void handleConfirmButtonClickedEvent();
	void handleCancelButtonClickedEvent();
    void handleSrcFileButtonClickedEvent();
    void handleDesFileButtonClickedEvent();
	void handleThreadFinishedEvent();

private:
	QLineEdit* m_srcEdit;
	QLineEdit* m_desEdit;
	QComboBox* m_s3mVersionBox;
	QComboBox* m_lonLatCoordBox;
	QPushButton* m_confirmButton;
	QProgressDialog* m_progressDialog;

#ifdef WIN32
	QString buttonOK = "确定";
	QString buttonCancle = "取消";
	QString waitText = "...";
	QString sourceSCP = "源配置文件:";
	QString outputDir = "目标路径:";
	QString s3mVersion = "S3M版本号:";
	QString isLonLatCoord = "是否为经纬度坐标:";
	QString exeResult = "执行结果";
	QString isDone = "完成";
	QString waiting = "正在执行...";
	QString openFile = "打开";
#else
	QTextCodec* codec = QTextCodec::codecForName("GBK");
	QString buttonOK = codec->toUnicode("确定");
	QString buttonCancle = codec->toUnicode("取消");
	QString waitText = codec->toUnicode("...");
	QString sourceSCP = codec->toUnicode("源配置文件:");
	QString outputDir = codec->toUnicode("目标路径:");
	QString s3mVersion = codec->toUnicode("S3M版本号:");
	QString isLonLatCoord = codec->toUnicode("是否为经纬度坐标:");
	QString exeResult = codec->toUnicode("执行结果");
	QString isDone = codec->toUnicode("完成:");
	QString waiting = codec->toUnicode("正在执行...");
	QString openFile = codec->toUnicode("打开");
#endif
};
