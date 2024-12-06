#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#include "DialogS3MBTo3DTiles.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QMessageBox>
#include "ProcessTools.h"
#include "ProcessThread.h"

using namespace S3MB;

DialogS3MBTo3DTiles::DialogS3MBTo3DTiles(QWidget *parent)
	: QDialog(parent)
{
	QVBoxLayout* vctLayout = new QVBoxLayout(this);

	m_srcEdit = new QLineEdit();
	m_srcEdit->setFixedSize(250, 20);
	m_desEdit = new QLineEdit();
	m_threadEdit = new QLineEdit();
    connect(m_srcEdit, SIGNAL(textEdited(QString)), this, SLOT(handleTextEditedEvent()));
    connect(m_desEdit, SIGNAL(textEdited(QString)), this, SLOT(handleTextEditedEvent()));
    connect(m_threadEdit,SIGNAL(textEdited(QString)), this, SLOT(handleTextEditedEvent()));

    m_confirmButton = new QPushButton(buttonOK);
	m_confirmButton->setEnabled(false);
    QPushButton* cancelButton = new QPushButton(buttonCancle);
    connect(m_confirmButton, SIGNAL(clicked()), this, SLOT(handleConfirmButtonClickedEvent()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(handleCancelButtonClickedEvent()));


    QPushButton* srcFileButton = new QPushButton(waitText);
    srcFileButton->setFixedSize(30, 20);
    connect(srcFileButton, SIGNAL(clicked()), this, SLOT(handleSrcFileButtonClickedEvent()));
    QGridLayout* gridSrcLayout = new QGridLayout();
    gridSrcLayout->addWidget(m_srcEdit, 0,0);
    gridSrcLayout->addWidget(srcFileButton, 0,2);

    QPushButton* desFileButton = new QPushButton(waitText);
    desFileButton->setFixedSize(30, 20);
    connect(desFileButton, SIGNAL(clicked()), this, SLOT(handleDesFileButtonClickedEvent()));
    QGridLayout* gridDesLayout = new QGridLayout();
    gridDesLayout->addWidget(m_desEdit, 0,0);
    gridDesLayout->addWidget(desFileButton, 0,2);

	QFormLayout* formLayout = new QFormLayout();
	formLayout->setHorizontalSpacing(20);
	formLayout->setVerticalSpacing(10);
    formLayout->addRow(sourceSCP, gridSrcLayout);
    formLayout->addRow(outputDir, gridDesLayout);
    formLayout->addRow(threadCount, m_threadEdit);

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

void DialogS3MBTo3DTiles::handleTextEditedEvent()
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
    bool isEnabled = isEmpty && isExists && !m_threadEdit->text().isEmpty();
	m_confirmButton->setEnabled(isEnabled);
}

void DialogS3MBTo3DTiles::handleThreadFinishedEvent()
{
	if (m_progressDialog != nullptr)
	{
		m_progressDialog->close();
		m_progressDialog = nullptr;
	}
	
    QMessageBox::information(this, exeResult,isDone);
}

void DialogS3MBTo3DTiles::handleConfirmButtonClickedEvent()
{
    std::wstring strSrcPath = StringUtil::UTF8_to_UNICODE(m_srcEdit->text().toStdString());
    std::wstring strDesPath = StringUtil::UTF8_to_UNICODE(m_desEdit->text().toStdString());
	int nThreadNum = m_threadEdit->text().toInt();

	m_progressDialog = new QProgressDialog(this);
	m_progressDialog->setWindowModality(Qt::ApplicationModal);
	m_progressDialog->setMinimum(0);
	m_progressDialog->setMaximum(0);
    m_progressDialog->setWindowTitle(waiting);
	m_progressDialog->setCancelButtonText(nullptr);
	m_progressDialog->setWindowFlags(Qt::Window | Qt::WindowTitleHint);
	m_progressDialog->show();

	S3MBParams* pParams = new S3MBParams();
	pParams->SetSCPPath(strSrcPath);
	pParams->SetOutputDir(strDesPath);
	pParams->SetThreadNum(nThreadNum);

	ProcessThread* pThread = new ProcessThread(pParams);
    connect(pThread, SIGNAL(finished()), this, SLOT(handleThreadFinishedEvent()));
    connect(pThread, SIGNAL(finished()), pThread, SLOT(deleteLater()));
	pThread->start();
}

void DialogS3MBTo3DTiles::handleCancelButtonClickedEvent()
{
	this->close();
}

void DialogS3MBTo3DTiles::handleSrcFileButtonClickedEvent()
{
    QFileDialog* pFileSelectDialog = new QFileDialog(this);
    pFileSelectDialog->setWindowTitle(openFile);
    pFileSelectDialog->setNameFilter(tr("File(*.scp)"));
    pFileSelectDialog->setOption(QFileDialog::DontUseNativeDialog);
    m_srcEdit->setText(pFileSelectDialog->getOpenFileName());
    handleTextEditedEvent();
}

void DialogS3MBTo3DTiles::handleDesFileButtonClickedEvent()
{
    QFileDialog* pFileSelectDialog = new QFileDialog(this);
    pFileSelectDialog->setWindowTitle(openFile);
    pFileSelectDialog->setFileMode(QFileDialog::Directory);
    pFileSelectDialog->setOption(QFileDialog::DontUseNativeDialog);
    m_desEdit->setText(pFileSelectDialog->getExistingDirectory());
    handleTextEditedEvent();
}
