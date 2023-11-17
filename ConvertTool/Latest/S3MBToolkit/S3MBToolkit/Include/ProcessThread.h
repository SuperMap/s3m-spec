#pragma once
#include <QThread>
#include "ProcessTools.h"

using namespace S3MB;

class ProcessThread : public QThread
{
	Q_OBJECT

public:
	ProcessThread(void* pParam, QWidget *parent = Q_NULLPTR);
	~ProcessThread();

protected:
	void run();

signals:
	void finished();
	
private:
	ProcessParams* m_pParam;
};
