#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#include "DialogI3SToS3MB.h"
#include <QRadioButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QStringListModel>
#include <QMessageBox>
#include "I3SParams.h"
#include "ProcessTools.h"
#include "ProcessThread.h"

using namespace S3MB;

const char* S3MVersion3 = "3.0";
const char* S3MVersion3_0_1 = "3.01";
const char* IsTrue = "true";
const char* IsFalse = "false";

DialogI3SToS3MB::DialogI3SToS3MB(QWidget *parent)
	: QDialog(parent)
{
	QVBoxLayout* vctLayout = new QVBoxLayout(this);

	m_srcEdit = new QLineEdit();
	m_srcEdit->setFixedSize(250, 20);
	m_desEdit = new QLineEdit();

    connect(m_srcEdit, SIGNAL(textEdited(QString)), this, SLOT(handleTextEditedEvent()));
    connect(m_desEdit, SIGNAL(textEdited(QString)), this, SLOT(handleTextEditedEvent()));

	QStringListModel* s3mVersionModel = new QStringListModel(this);
	s3mVersionModel->setStringList(QStringList()<< S3MVersion3 << S3MVersion3_0_1);
	m_s3mVersionBox = new QComboBox();
	m_s3mVersionBox->setModel(s3mVersionModel);

	QStringListModel* lonLatCoordModel = new QStringListModel(this);
	lonLatCoordModel->setStringList(QStringList() << IsTrue << IsFalse);
	m_lonLatCoordBox = new QComboBox();
	m_lonLatCoordBox->setModel(lonLatCoordModel);

    m_confirmButton = new QPushButton(buttonOK);
	m_confirmButton->setEnabled(false);
    QPushButton* cancelButton = new QPushButton(buttonCancle);
    connect(m_confirmButton, SIGNAL(clicked()), this, SLOT(handleConfirmButtonClickedEvent()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(handleCancelButtonClickedEvent()));


    QPushButton* srcFileButton = new QPushButton(waitText);
    srcFileButton->setFixedSize(25, 20);
    connect(srcFileButton, SIGNAL(clicked()), this, SLOT(handleSrcFileButtonClickedEvent()));
    QGridLayout* gridSrcLayout = new QGridLayout();
    gridSrcLayout->addWidget(m_srcEdit, 0,0);
    gridSrcLayout->addWidget(srcFileButton, 0,2);

    QPushButton* desFileButton = new QPushButton(waitText);
    desFileButton->setFixedSize(25, 20);
    connect(desFileButton, SIGNAL(clicked()), this, SLOT(handleDesFileButtonClickedEvent()));
    QGridLayout* gridDesLayout = new QGridLayout();
    gridDesLayout->addWidget(m_desEdit, 0,0);
    gridDesLayout->addWidget(desFileButton, 0,2);

	QFormLayout* formLayout = new QFormLayout();
	formLayout->setHorizontalSpacing(20);
	formLayout->setVerticalSpacing(10);
    formLayout->addRow(sourceSCP, gridSrcLayout);
    formLayout->addRow(outputDir, gridDesLayout);
    formLayout->addRow(s3mVersion, m_s3mVersionBox);
	formLayout->addRow(isLonLatCoord, m_lonLatCoordBox);

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

void DialogI3SToS3MB::handleTextEditedEvent()
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

void DialogI3SToS3MB::handleThreadFinishedEvent()
{
	if (m_progressDialog != nullptr)
	{
		m_progressDialog->close();
		m_progressDialog = nullptr;
	}
	
    QMessageBox::information(this, exeResult, isDone);
}

void DialogI3SToS3MB::handleConfirmButtonClickedEvent()
{
    std::wstring strSrcPath = StringUtil::UTF8_to_UNICODE(m_srcEdit->text().toStdString());
    std::wstring strDesPath = StringUtil::UTF8_to_UNICODE(m_desEdit->text().toStdString());

	float fS3MVersion = S3MB_VERSIONV3;
	QString strS3MVersion = m_s3mVersionBox->currentText();
    if (strS3MVersion == S3MVersion3)
	{
		fS3MVersion = S3MB_VERSIONV3;
	}
	else if (strS3MVersion == S3MVersion3_0_1)
	{
		fS3MVersion = S3MB_VERSIONV3_0_1;
	}

	bool bLonLatCoord = true;
	QString strLonLatCoord = m_lonLatCoordBox->currentText();
	if (strLonLatCoord == IsTrue)
	{
		bLonLatCoord = true;
	}
	else if (strLonLatCoord == IsFalse)
	{
		bLonLatCoord = false;
	}

	m_progressDialog = new QProgressDialog(this);
	m_progressDialog->setWindowModality(Qt::ApplicationModal);
	m_progressDialog->setMinimum(0);
	m_progressDialog->setMaximum(0);
    m_progressDialog->setWindowTitle(waiting);
	m_progressDialog->setCancelButtonText(nullptr);
	m_progressDialog->setWindowFlags(Qt::Window | Qt::WindowTitleHint);
	m_progressDialog->show();

	I3SParams* pParam = new I3SParams();
	pParam->SetInputPath(strSrcPath);
	pParam->SetOutputPath(strDesPath);
	pParam->SetS3MVersion(fS3MVersion);
	pParam->SetIsLonLatCoordinates(bLonLatCoord);
	
	ProcessThread* pThread = new ProcessThread(pParam);
    connect(pThread, SIGNAL(finished()), this, SLOT(handleThreadFinishedEvent()));
    connect(pThread, SIGNAL(finished()), pThread, SLOT(deleteLater()));
	pThread->start();
}

void DialogI3SToS3MB::handleCancelButtonClickedEvent()
{
	this->close();
}

void DialogI3SToS3MB::handleSrcFileButtonClickedEvent()
{
    QFileDialog* pFileSelectDialog = new QFileDialog(this);
    pFileSelectDialog->setWindowTitle(openFile);
    pFileSelectDialog->setNameFilter(tr("File(*.json)"));
    m_srcEdit->setText(pFileSelectDialog->getOpenFileName());
    handleTextEditedEvent();
}

void DialogI3SToS3MB::handleDesFileButtonClickedEvent()
{
    QFileDialog* pFileSelectDialog = new QFileDialog(this);
    pFileSelectDialog->setWindowTitle(openFile);
    pFileSelectDialog->setFileMode(QFileDialog::Directory);
    m_desEdit->setText(pFileSelectDialog->getExistingDirectory());
    handleTextEditedEvent();
}
