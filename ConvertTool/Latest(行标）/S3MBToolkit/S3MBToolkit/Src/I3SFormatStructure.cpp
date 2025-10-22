#include "I3SFormatStructure.h"

namespace S3MB
{
	I3SMetadata::I3SMetadata()
	{
		m_nNodeCount = 0;
	}

	I3SMetadata::~I3SMetadata()
	{
	}

	void I3SMetadata::SetNodeCount(unsigned int nCount)
	{
		m_nNodeCount = nCount;
	}

	unsigned int I3SMetadata::GetNodeCount() const
	{
		return m_nNodeCount;
	}

	///////////////////////////////////////////////////////////////////////////////////////////

	I3SAttStorageInfo::I3SAttStorageInfo()
	{
	}

	I3SAttStorageInfo::~I3SAttStorageInfo()
	{
	}

	void I3SAttStorageInfo::SetKey(const std::wstring& strKey)
	{
		m_strKey = strKey;
	}

	std::wstring I3SAttStorageInfo::GetKey() const
	{
		return m_strKey;
	}

	void I3SAttStorageInfo::SetName(const std::wstring& strName)
	{
		m_strName = strName;
	}

	std::wstring I3SAttStorageInfo::GetName() const
	{
		return m_strName;
	}

	///////////////////////////////////////////////////////////////////////////////////////////

	I3SDrawingInfo::I3SDrawingInfo()
	{
		m_eRendererType = RT_None;
		m_bHasMalColor = false;
		m_eColorMixMode = CMM_None;
		m_eSymbolType = ST_None;
	}

	I3SDrawingInfo::~I3SDrawingInfo()
	{
	}

	void I3SDrawingInfo::SetRendererType(I3SDrawingInfo::I3SRendererType eType)
	{
		m_eRendererType = eType;
	}

	I3SDrawingInfo::I3SRendererType I3SDrawingInfo::GetRendererType() const
	{
		return m_eRendererType;
	}

	void I3SDrawingInfo::SetFieldName(const std::wstring& strName)
	{
		m_strFieldName = strName;
	}

	std::wstring I3SDrawingInfo::GetFieldName() const
	{
		return m_strFieldName;
	}

	void I3SDrawingInfo::SetMaterialColor(const ColorValue& color)
	{
		m_malColor = color;
		m_bHasMalColor = true;
	}

	ColorValue I3SDrawingInfo::GetMaterialColor() const
	{
		return m_malColor;
	}

	bool I3SDrawingInfo::GetHasMaterialColor() const
	{
		return m_bHasMalColor;
	}

	void I3SDrawingInfo::SetColorMixMode(I3SDrawingInfo::I3SColorMixMode eMode)
	{
		m_eColorMixMode = eMode;
	}

	I3SDrawingInfo::I3SColorMixMode I3SDrawingInfo::GetColorMixMode() const
	{
		return m_eColorMixMode;
	}

	void I3SDrawingInfo::SetSymbolType(I3SDrawingInfo::I3SSymbol3DType eType)
	{
		m_eSymbolType = eType;
	}

	I3SDrawingInfo::I3SSymbol3DType I3SDrawingInfo::GetSymbolType() const
	{
		return m_eSymbolType;
	}

	void I3SDrawingInfo::SetFieldSymbols(const std::map<Variant, std::wstring>& mapValue)
	{
		m_mapFieldSymbol = mapValue;
	}

	std::map<Variant, std::wstring> I3SDrawingInfo::GetFieldSymbols() const
	{
		return m_mapFieldSymbol;
	}

	void I3SDrawingInfo::SetDefaultSymbolRef(const std::wstring& strRef)
	{
		m_strDefaultSymbolRef = strRef;
	}

	std::wstring I3SDrawingInfo::GetDefaultSymbolRef() const
	{
		return m_strDefaultSymbolRef;
	}

	void I3SDrawingInfo::SetSymbolPaths(const std::map<std::wstring, std::wstring>& mapPath)
	{
		m_mapSymbolPath = mapPath;
	}

	std::map<std::wstring, std::wstring> I3SDrawingInfo::GetSymbolPaths() const
	{
		return m_mapSymbolPath;
	}

	void I3SDrawingInfo::SetSymbolTransforms(const std::map<std::wstring, Matrix4d>& mapValue)
	{
		m_mapSymbolTransform = mapValue;
	}

	std::map<std::wstring, Matrix4d> I3SDrawingInfo::GetSymbolTransforms() const
	{
		return m_mapSymbolTransform;
	}

	///////////////////////////////////////////////////////////////////////////////////////////

	I3SAttributeInfo::I3SAttributeInfo()
	{
		m_nOriFieldType = FieldType::FT_UnKnown;
		m_bIsCoded = false;
	}

	I3SAttributeInfo::~I3SAttributeInfo()
	{
	}

	void I3SAttributeInfo::SetFieldName(const std::wstring& strName)
	{
		m_strFieldName = strName;
	}

	std::wstring I3SAttributeInfo::GetFieldName() const
	{
		return m_strFieldName;
	}

	void I3SAttributeInfo::SetOriginFieldType(FieldType nType)
	{
		m_nOriFieldType = nType;
	}

	FieldType I3SAttributeInfo::GetOriginFieldType() const
	{
		return m_nOriFieldType;
	}

	void I3SAttributeInfo::SetIsCoded(bool bIs)
	{
		m_bIsCoded = bIs;
	}

	bool I3SAttributeInfo::GetIsCoded() const
	{
		return m_bIsCoded;
	}

	void I3SAttributeInfo::SetCodeValues(const std::map<Variant, std::wstring>& mapValue)
	{
		m_mapCodeValue = mapValue;
	}

	std::map<Variant, std::wstring> I3SAttributeInfo::GetCodeValues() const
	{
		return m_mapCodeValue;
	}

	///////////////////////////////////////////////////////////////////////////////////////////

	I3SIndexInfo::I3SIndexInfo()
	{
		m_nPageNum = 0;
		m_nNodesPerPage = 64;
	}

	I3SIndexInfo::~I3SIndexInfo()
	{
	}

	void I3SIndexInfo::SetPageNum(unsigned int nNum)
	{
		m_nPageNum = nNum;
	}

	unsigned int I3SIndexInfo::GetPageNum() const
	{
		return m_nPageNum;
	}

	void I3SIndexInfo::SetNodesPerPage(unsigned int nNum)
	{
		m_nNodesPerPage = nNum;
	}

	unsigned int I3SIndexInfo::GetNodesPerPage() const
	{
		return m_nNodesPerPage;
	}

	void I3SIndexInfo::SetLodSelMetricType(const std::wstring& strType)
	{
		m_strLodSelMetricType = strType;
	}

	std::wstring I3SIndexInfo::GetLodSelMetricType() const
	{
		return m_strLodSelMetricType;
	}

	///////////////////////////////////////////////////////////////////////////////////////////

	I3SMaterialInfo::I3SMaterialInfo()
	{
		m_fShininess = 0.0;
		m_ambient = ColorValue(1.0, 1.0, 1.0, 1.0);
		m_diffuse = ColorValue(1.0, 1.0, 1.0, 1.0);
		m_specular = ColorValue(0.0, 0.0, 0.0, 0.0);
		m_nCullMode = CULL_NONE;
	}

	I3SMaterialInfo::~I3SMaterialInfo()
	{
	}

	void I3SMaterialInfo::SetName(const std::wstring& strName)
	{
		m_strName = strName;
	}

	std::wstring I3SMaterialInfo::GetName() const
	{
		return m_strName;
	}

	void I3SMaterialInfo::SetShininess(float fValue)
	{
		m_fShininess = fValue;
	}

	float I3SMaterialInfo::GetShininess() const
	{
		return m_fShininess;
	}

	void I3SMaterialInfo::SetAmbient(const ColorValue& color)
	{
		m_ambient = color;
	}

	ColorValue I3SMaterialInfo::GetAmbient() const
	{
		return m_ambient;
	}

	void I3SMaterialInfo::SetDiffuse(const ColorValue& color)
	{
		m_diffuse = color;
	}

	ColorValue I3SMaterialInfo::GetDiffuse() const
	{
		return m_diffuse;
	}

	void I3SMaterialInfo::SetSpecular(const ColorValue& color)
	{
		m_specular = color;
	}

	ColorValue I3SMaterialInfo::GetSpecular() const
	{
		return m_specular;
	}

	void I3SMaterialInfo::SetCullMode(CullingMode nMode)
	{
		m_nCullMode = nMode;
	}

	CullingMode I3SMaterialInfo::GetCullMode() const
	{
		return m_nCullMode;
	}

	///////////////////////////////////////////////////////////////////////////////////////////

	I3SNodeInfo::I3SNodeInfo()
	{
		m_dRangeValue = 0.0;
	}

	I3SNodeInfo::~I3SNodeInfo()
	{
	}

	void I3SNodeInfo::SetId(const std::wstring& strId)
	{
		m_strId = strId;
	}

	std::wstring I3SNodeInfo::GetId() const
	{
		return m_strId;
	}

	void I3SNodeInfo::SetObb(const OrientedBoundingBox& obb)
	{
		m_obb = obb;
	}

	OrientedBoundingBox I3SNodeInfo::GetObb() const
	{
		return m_obb;
	}

	void I3SNodeInfo::SetRangeValue(double dValue)
	{
		m_dRangeValue = dValue;
	}

	double I3SNodeInfo::GetRangeValue() const
	{
		return m_dRangeValue;
	}

	void I3SNodeInfo::SetGeometries(const std::vector<std::wstring>& vecGeo)
	{
		m_vecGeometry = vecGeo;
	}

	std::vector<std::wstring> I3SNodeInfo::GetGeometries() const
	{
		return m_vecGeometry;
	}

	void I3SNodeInfo::SetTextures(const std::vector<std::wstring>& vecTex)
	{
		m_vecTexture = vecTex;
	}

	std::vector<std::wstring> I3SNodeInfo::GetTextures() const
	{
		return m_vecTexture;
	}

	void I3SNodeInfo::SetFeatures(const std::vector<std::wstring>& vecFea)
	{
		m_vecFeature = vecFea;
	}

	std::vector<std::wstring> I3SNodeInfo::GetFeatures() const
	{
		return m_vecFeature;
	}

	void I3SNodeInfo::SetAttributes(const std::vector<std::wstring>& vecAtt)
	{
		m_vecAttribute = vecAtt;
	}

	std::vector<std::wstring> I3SNodeInfo::GetAttributes() const
	{
		return m_vecAttribute;
	}

	void I3SNodeInfo::SetSharedResource(const std::wstring& strRes)
	{
		m_strSharedRes = strRes;
	}

	std::wstring I3SNodeInfo::GetSharedResource() const
	{
		return m_strSharedRes;
	}

	void I3SNodeInfo::SetChildNodes(const std::vector<std::wstring>& vecNode)
	{
		m_vecChildNode = vecNode;
	}

	std::vector<std::wstring> I3SNodeInfo::GetChildNodes() const
	{
		return m_vecChildNode;
	}

	void I3SNodeInfo::SetVertexAttributeExtensionAndIdRelation(const std::vector<std::pair<std::wstring, int> >& vecRel)
	{
		m_vecVertexAttExtAndId = vecRel;
	}

	std::vector<std::pair<std::wstring, int> > I3SNodeInfo::GetVertexAttributeExtensionAndIdRelation() const
	{
		return m_vecVertexAttExtAndId;
	}

	///////////////////////////////////////////////////////////////////////////////////////////

	I3SLayerInfo::I3SLayerInfo()
	{
	}

	I3SLayerInfo::~I3SLayerInfo()
	{
	}

	void I3SLayerInfo::SetId(const std::wstring& strId)
	{
		m_strId = strId;
	}

	std::wstring I3SLayerInfo::GetId() const
	{
		return m_strId;
	}

	void I3SLayerInfo::SetType(const std::wstring& strType)
	{
		m_strType = strType;
	}

	std::wstring I3SLayerInfo::GetType() const
	{
		return m_strType;
	}

	void I3SLayerInfo::SetName(const std::wstring& strName)
	{
		m_strName = strName;
	}

	std::wstring I3SLayerInfo::GetName() const
	{
		return m_strName;
	}
}
