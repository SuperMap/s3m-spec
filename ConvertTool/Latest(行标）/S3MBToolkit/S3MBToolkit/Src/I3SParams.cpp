#include "I3SParams.h"

namespace S3MB
{
	I3SParams::I3SParams()
	{
		m_fS3MVersion = S3MB_VERSIONV3;
		m_bLonLatCoord = true;
	}

	I3SParams::~I3SParams()
	{
	}

	ProcessType I3SParams::GetProcessType()
	{
		return ProcessType::P_I3SToS3MB;
	}

	void I3SParams::SetInputPath(const std::wstring& strPath)
	{
		m_strInputPath = strPath;
	}

	std::wstring I3SParams::GetInputPath() const
	{
		return m_strInputPath;
	}

	void I3SParams::SetOutputPath(const std::wstring& strPath)
	{
		m_strOutputPath = strPath;
	}

	std::wstring I3SParams::GetOutputPath() const
	{
		return m_strOutputPath;
	}

	void I3SParams::SetS3MVersion(float fVersion)
	{
		m_fS3MVersion = fVersion;
	}

	float I3SParams::GetS3MVersion() const
	{
		return m_fS3MVersion;
	}

	void I3SParams::SetDataName(const std::wstring& strName)
	{
		m_strDataName = strName;
	}

	std::wstring I3SParams::GetDataName() const
	{
		return m_strDataName;
	}

	void I3SParams::SetIsLonLatCoordinates(bool bIs)
	{
		m_bLonLatCoord = bIs;
	}

	bool I3SParams::GetIsLonLatCoordinates() const
	{
		return m_bLonLatCoord;
	}

	///////////////////////////////////////////////////////////////////////////////////////////

	I3SParseParams::I3SParseParams()
	{
		m_fI3SVersion = 0.0f;
		m_bHasFaceAttributes = false;
		m_nVexCompType = SV_Standard;
		m_nEpsgCode = 0;
		m_bHasEpsgCode = false;
		m_bHasDrawingInfo = false;
	}

	I3SParseParams::~I3SParseParams()
	{
	}

	void I3SParseParams::SetI3SVersion(float fVersion)
	{
		m_fI3SVersion = fVersion;
	}

	float I3SParseParams::GetI3SVersion() const
	{
		return m_fI3SVersion;
	}

	void I3SParseParams::SetRootNodes(const std::vector<std::pair<std::wstring, OrientedBoundingBox> >& vecNode)
	{
		m_vecRootNode = vecNode;
	}

	std::vector<std::pair<std::wstring, OrientedBoundingBox> > I3SParseParams::GetRootNodes() const
	{
		return m_vecRootNode;
	}

	void I3SParseParams::SetVertexAttributeAndIdRelation(const std::vector<std::pair<std::wstring, int> >& vecRel)
	{
		m_vecVertexAttAndId = vecRel;
	}

	std::vector<std::pair<std::wstring, int> > I3SParseParams::GetVertexAttributeAndIdRelation() const
	{
		return m_vecVertexAttAndId;
	}

	void I3SParseParams::SetVertexAttributeExtensionAndIdRelation(const std::vector<std::pair<std::wstring, int> >& vecRel)
	{
		m_vecVertexAttExtAndId = vecRel;
	}

	std::vector<std::pair<std::wstring, int> > I3SParseParams::GetVertexAttributeExtensionAndIdRelation() const
	{
		return m_vecVertexAttExtAndId;
	}

	void I3SParseParams::SetHasFaceAttributes(bool bHas)
	{
		m_bHasFaceAttributes = bHas;
	}

	bool I3SParseParams::GetHasFaceAttributes() const
	{
		return m_bHasFaceAttributes;
	}

	void I3SParseParams::SetTextureFormats(const std::map<std::wstring, std::wstring>& mapFormat)
	{
		m_mapTexFormat = mapFormat;
	}

	std::map<std::wstring, std::wstring> I3SParseParams::GetTextureFormats() const
	{
		return m_mapTexFormat;
	}

	void I3SParseParams::SetVertexCompressionType(S3MBVertexTag nType)
	{
		m_nVexCompType = nType;
	}

	S3MBVertexTag I3SParseParams::GetVertexCompressionType() const
	{
		return m_nVexCompType;
	}

	void I3SParseParams::SetEpsgCode(int nCode)
	{
		m_nEpsgCode = nCode;
		m_bHasEpsgCode = true;
	}

	int I3SParseParams::GetEpsgCode() const
	{
		return m_nEpsgCode;
	}

	bool I3SParseParams::GetHasEpsgCode() const
	{
		return m_bHasEpsgCode;
	}

	void I3SParseParams::SetLayerPath(const std::wstring& strPath)
	{
		m_strLayerPath = strPath;
	}

	std::wstring I3SParseParams::GetLayerPath() const
	{
		return m_strLayerPath;
	}

	void I3SParseParams::SetOutputPath(const std::wstring& strPath)
	{
		m_strOutputPath = strPath;
	}

	std::wstring I3SParseParams::GetOutputPath() const
	{
		return m_strOutputPath;
	}

	void I3SParseParams::SetDrawingInfo(const I3SDrawingInfo& info)
	{
		m_drawingInfo = info;
		m_bHasDrawingInfo = true;
	}

	I3SDrawingInfo I3SParseParams::GetDrawingInfo() const
	{
		return m_drawingInfo;
	}

	bool I3SParseParams::GetHasDrawingInfo() const
	{
		return m_bHasDrawingInfo;
	}

	void I3SParseParams::SetIndexInfo(const I3SIndexInfo& info)
	{
		m_indexInfo = info;
	}

	I3SIndexInfo I3SParseParams::GetIndexInfo() const
	{
		return m_indexInfo;
	}

	void I3SParseParams::SetAttributeInfos(const std::vector<I3SAttributeInfo>& vecInfo)
	{
		m_vecAttInfo = vecInfo;
	}

	std::vector<I3SAttributeInfo> I3SParseParams::GetAttributeInfos() const
	{
		return m_vecAttInfo;
	}

	void I3SParseParams::SetFieldInfos(const S3MBFieldInfos& infos)
	{
		m_fieldInfos = infos;
	}

	S3MBFieldInfos I3SParseParams::GetFieldInfos() const
	{
		return m_fieldInfos;
	}

	void I3SParseParams::SetAttStorageInfos(const std::vector<I3SAttStorageInfo>& vecInfo)
	{
		m_vecAttStorageInfo = vecInfo;
	}

	std::vector<I3SAttStorageInfo> I3SParseParams::GetAttStorageInfos() const
	{
		return m_vecAttStorageInfo;
	}
}
