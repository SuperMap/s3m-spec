#include "ProcessThread.h"

ProcessThread::ProcessThread(void* pParam, QWidget *parent) : QThread()
{
	m_pParam = (ProcessParams*)pParam;
}

ProcessThread::~ProcessThread()
{
	delete (ProcessParams*)m_pParam;
	m_pParam = nullptr;
}

void ProcessThread::run()
{
	if (m_pParam == nullptr)
	{
		return;
	}

	ProcessTools tools;
	ProcessType nType = m_pParam->GetProcessType();
	if (nType == ProcessType::P_3DTilesToS3MB)
	{
		ThreeDTilesParams* p3DTileParam = (ThreeDTilesParams*)m_pParam;
		tools.ThreeDTilesToS3MB(*p3DTileParam);
	}
	else if (nType == ProcessType::P_S3MBTo3DTiles)
	{
		S3MBParams* pS3MBParam = (S3MBParams*)m_pParam;
		tools.S3MBTo3DTiles(*pS3MBParam);
	}
	else if (nType == ProcessType::P_OSGBToS3MB)
	{
		OSGBParams* pOSGBParam = (OSGBParams*)m_pParam;
		tools.OSGBToS3MB(*pOSGBParam);
	}
	else if (nType == ProcessType::P_OBJToS3MB)
	{
		OBJParams* pOBJParam = (OBJParams*)m_pParam;
		tools.OBJToS3MB(*pOBJParam);
	}
	finished();
}