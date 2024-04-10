#pragma once
#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QButtonGroup>
#include <QProgressDialog>
#include <QTextCodec>
#include <QFileDialog>

class DialogOSGBToS3MB : public QDialog
{
	Q_OBJECT

public:
	DialogOSGBToS3MB(QWidget *parent = Q_NULLPTR);

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
	QPushButton* m_confirmButton;
	QButtonGroup* m_btnGroup;
	QProgressDialog* m_progressDialog;
    QTextCodec* codec = QTextCodec::codecForName("GBK");
};
