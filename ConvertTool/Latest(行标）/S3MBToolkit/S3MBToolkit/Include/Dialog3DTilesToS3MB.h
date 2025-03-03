#pragma once
#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QButtonGroup>
#include <QProgressDialog>
#include <QTextCodec>
#include <QFileDialog>

class Dialog3DTilesToS3MB : public QDialog
{
	Q_OBJECT

public:
	Dialog3DTilesToS3MB(QWidget *parent = Q_NULLPTR);

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
	QComboBox* m_fileTypeBox;
	QComboBox* m_texCompBox;
	QComboBox* m_verCompBox;
	QPushButton* m_confirmButton;
	QProgressDialog* m_progressDialog;
};
