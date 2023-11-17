#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#include "DialogOBJToS3MB.h"
#include <QRadioButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QStringListModel>
#include <QMessageBox>
#include "ProcessTools.h"
#include "ProcessThread.h"

using namespace S3MB;

DialogOBJToS3MB::DialogOBJToS3MB(QWidget *parent)
	: QDialog(parent)
{
	QVBoxLayout* vctLayout = new QVBoxLayout(this);

    m_srcEdit = new QLineEdit();
    m_srcEdit->setFixedSize(250, 25);
    m_desEdit = new QLineEdit();
    m_desEdit->setFixedSize(250, 25);
    connect(m_srcEdit, SIGNAL(textEdited(QString)), this, SLOT(handleTextEditedEvent()));
    connect(m_desEdit, SIGNAL(textEdited(QString)), this, SLOT(handleTextEditedEvent()));

    m_confirmButton = new QPushButton(codec->toUnicode("确定"));
	m_confirmButton->setEnabled(false);
    QPushButton* cancelButton = new QPushButton(codec->toUnicode("取消"));
    connect(m_confirmButton, SIGNAL(clicked()), this, SLOT(handleConfirmButtonClickedEvent()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(handleCancelButtonClickedEvent()));

    QPushButton* srcFileButton = new QPushButton(codec->toUnicode("..."));
    srcFileButton->setFixedSize(30, 20);
    connect(srcFileButton, SIGNAL(clicked()), this, SLOT(handleSrcFileButtonClickedEvent()));
    QGridLayout* gridSrcLayout = new QGridLayout();
    gridSrcLayout->addWidget(m_srcEdit, 0,0);
    gridSrcLayout->addWidget(srcFileButton, 0,2);

    QPushButton* desFileButton = new QPushButton(codec->toUnicode("..."));
    desFileButton->setFixedSize(30, 20);
    connect(desFileButton, SIGNAL(clicked()), this, SLOT(handleDesFileButtonClickedEvent()));
    QGridLayout* gridDesLayout = new QGridLayout();
    gridDesLayout->addWidget(m_desEdit, 0,0);
    gridDesLayout->addWidget(desFileButton, 0,2);

    QFormLayout* formLayout = new QFormLayout();
    formLayout->setHorizontalSpacing(30);
    formLayout->setVerticalSpacing(10);
    formLayout->addRow(codec->toUnicode("源配置文件:"), gridSrcLayout);
    formLayout->addRow(codec->toUnicode("目标路径:"), gridDesLayout);

	QHBoxLayout* bottomLayout = new QHBoxLayout();
	bottomLayout->addStretch();
	bottomLayout->addWidget(m_confirmButton);
	bottomLayout->addWidget(cancelButton);

	vctLayout->setMargin(15);
	vctLayout->setSpacing(15);
	vctLayout->addLayout(formLayout);
	vctLayout->addLayout(bottomLayout);
	vctLayout->setSizeConstraint(QLayout::SetFixedSize);
}

void DialogOBJToS3MB::handleTextEditedEvent()
{
    QString strSrcPath = m_srcEdit->text();
    QString strDesPath = m_desEdit->text();
    bool isEmpty = !strSrcPath.isEmpty() && !strSrcPath.isEmpty();
    bool isExists = false;
    QFileInfo srcFile(strSrcPath);
    QFileInfo desFile(strDesPath);
    if (srcFile.isDir() || !srcFile.exists() || !desFile.isDir() || !desFile.exists())
    {
        isExists = false;
    }
    else
    {
        isExists = true;
    }
    bool isEnabled = isEmpty && isExists;
	m_confirmButton->setEnabled(isEnabled);
}

void DialogOBJToS3MB::handleThreadFinishedEvent()
{
	if (m_progressDialog != nullptr)
	{
		m_progressDialog->close();
		m_progressDialog = nullptr;
	}
	
    QMessageBox::information(this, codec->toUnicode("执行结果"), codec->toUnicode("完成!"));
}

void DialogOBJToS3MB::handleConfirmButtonClickedEvent()
{
	std::wstring strSrcPath = m_srcEdit->text().toStdWString();
	std::wstring strDesPath = m_desEdit->text().toStdWString();

	m_progressDialog = new QProgressDialog(this);
	m_progressDialog->setWindowModality(Qt::ApplicationModal);
	m_progressDialog->setMinimum(0);
	m_progressDialog->setMaximum(0);
    m_progressDialog->setWindowTitle(codec->toUnicode("正在执行..."));
	m_progressDialog->setCancelButtonText(nullptr);
	m_progressDialog->setWindowFlags(Qt::Window | Qt::WindowTitleHint);
	m_progressDialog->show();

	OBJParams* pParam = new OBJParams();
	pParam->SetInputFilePath(strSrcPath);
	pParam->SetOutputDir(strDesPath);
	
	ProcessThread* pThread = new ProcessThread(pParam);
    connect(pThread, SIGNAL(finished()), this, SLOT(handleThreadFinishedEvent()));
    connect(pThread, SIGNAL(finished()), pThread, SLOT(deleteLater()));
	pThread->start();
}

void DialogOBJToS3MB::handleCancelButtonClickedEvent()
{
	this->close();
}

void DialogOBJToS3MB::handleSrcFileButtonClickedEvent()
{
    QFileDialog* pFileSelectDialog = new QFileDialog(this);
    pFileSelectDialog->setWindowTitle(codec->toUnicode("打开"));
    pFileSelectDialog->setNameFilter(tr("File(*.json)"));
    m_srcEdit->setText(pFileSelectDialog->getOpenFileName());
    handleTextEditedEvent();
}

void DialogOBJToS3MB::handleDesFileButtonClickedEvent()
{
    QFileDialog* pFileSelectDialog = new QFileDialog(this);
    pFileSelectDialog->setWindowTitle(codec->toUnicode("打开"));
    pFileSelectDialog->setFileMode(QFileDialog::Directory);
    m_desEdit->setText(pFileSelectDialog->getExistingDirectory());
    handleTextEditedEvent();
}
