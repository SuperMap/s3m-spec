#pragma once
#include "Toolkit/UGMarkup.h"
#include "FileParser/UGFileParseModel.h"
#include "CacheBuilder3D/UGS3MBTags.h"

namespace UGC{

class CACHEBUILDER3D_API UGSCPLayerInfos
{
public:
	UGSCPLayerInfos(void);
	~UGSCPLayerInfos(void);
public:
	//! \brief strSCPFile是文件路径
	UGbool LoadFromIndexFile(const UGString& strAttFilePath);
	UGbool SaveToIndexFile(const UGString& strAttFilePath);

	//! \brief strSCPXML xml内容，不是路径
	UGbool Load(const UGString& strAttDataXML);
	UGbool Save(UGString& strAttDataXML);

	//! \brief 初始化信息
	void SetInfos(std::vector<UGLayerCacheInfo> &vecDatasetInfos);

	//! \brief 获取信息
	std::vector<UGLayerCacheInfo> &GetInfos();	

	static UGbool ReadLayerAttData(const UGString strSCVDFile, std::vector<UGLayerCacheInfo> &vecLayerInfos);

private:
	std::vector<UGLayerCacheInfo> m_vecLayerInfos;
};

}