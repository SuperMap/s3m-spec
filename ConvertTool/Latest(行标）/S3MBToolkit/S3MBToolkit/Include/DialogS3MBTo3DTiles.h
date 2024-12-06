#pragma once
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QProgressDialog>
#include <QTextCodec>
#include <QFileDialog>

class DialogS3MBTo3DTiles : public QDialog
{
	Q_OBJECT

public:
	DialogS3MBTo3DTiles(QWidget *parent = Q_NULLPTR);

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
	QLineEdit* m_threadEdit;
	QPushButton* m_confirmButton;
	QProgressDialog* m_progressDialog;
#ifdef WIN32
	QString buttonOK = "确定";
	QString buttonCancle = "取消";
	QString waitText = "...";
	QString sourceSCP = "SCP文件:";
	QString outputDir = "目标路径:";
	QString threadCount = "线程数:";
	QString exeResult = "执行结果";
	QString isDone = "完成";
	QString waiting = "正在执行...";
	QString openFile = "打开";
#else
	QTextCodec* codec = QTextCodec::codecForName("GBK");
	QString buttonOK = codec->toUnicode("确定");
	QString buttonCancle = codec->toUnicode("取消");
	QString waitText = codec->toUnicode("...");
	QString sourceSCP = codec->toUnicode("SCP文件:");
	QString outputDir = codec->toUnicode("目标路径:");
	QString threadCount = codec->toUnicode("线程数:");
	QString exeResult = codec->toUnicode("执行结果");
	QString isDone = codec->toUnicode("完成:");
	QString waiting = codec->toUnicode("正在执行...");
	QString openFile = codec->toUnicode("打开");
#endif
};
