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
    QTextCodec* codec = QTextCodec::codecForName("GBK");
};
