#include "Material.h"
#include <assert.h>
#include <string>

namespace S3MB
{
	TextureMotionParameter::TextureMotionParameter()
		:m_OffsetMotion(0.0, 0.0, 0.0, 0.0, 0.0),
		m_TilingMotion(1.0, 1.0, 1.0, 1.0, 0.0)
	{
	}

	TextureMotionParameter::TextureMotionParameter(const TextureMotionParameter& other)
		:m_OffsetMotion(other.m_OffsetMotion),
		m_TilingMotion(other.m_TilingMotion)
	{
	}

	TextureMotionParameter& TextureMotionParameter::operator=(const TextureMotionParameter& other)
	{
		m_TilingMotion = other.m_TilingMotion;
		m_OffsetMotion = other.m_OffsetMotion;
		return *this;
	}

	TextureMotionParameter::~TextureMotionParameter()
	{
	}

	bool TextureMotionParameter::ToJson(JsonValue & json)
	{
		json.Add(TEXTUREMOTION_OFFSETSTARTU, m_OffsetMotion.m_fStartU);
		json.Add(TEXTUREMOTION_OFFSETSTARTV, m_OffsetMotion.m_fStartV);
		json.Add(TEXTUREMOTION_OFFSETSPEEDU, m_OffsetMotion.m_fSpeedU);
		json.Add(TEXTUREMOTION_OFFSETSPEEDV, m_OffsetMotion.m_fSpeedV);
		json.Add(TEXTUREMOTION_OFFSETPERIOD, m_OffsetMotion.m_fPeriod);
		json.Add(TEXTUREMOTION_TILINGSTARTU, m_TilingMotion.m_fStartU);
		json.Add(TEXTUREMOTION_TILINGSTARTV, m_TilingMotion.m_fStartV);
		json.Add(TEXTUREMOTION_TILINGSPEEDU, m_TilingMotion.m_fSpeedU);
		json.Add(TEXTUREMOTION_TILINGSPEEDV, m_TilingMotion.m_fSpeedV);
		json.Add(TEXTUREMOTION_TILINGPERIOD, m_TilingMotion.m_fPeriod);
		return true;
	}

	bool TextureMotionParameter::FromJson(JsonValue & json)
	{
		if (json.Contains(TEXTUREMOTION_OFFSETSTARTU))
		{
			json.GetValue(TEXTUREMOTION_OFFSETSTARTU, m_OffsetMotion.m_fStartU);
			json.GetValue(TEXTUREMOTION_OFFSETSTARTV, m_OffsetMotion.m_fStartV);
			json.GetValue(TEXTUREMOTION_TILINGSTARTU, m_TilingMotion.m_fStartU);
			json.GetValue(TEXTUREMOTION_TILINGSTARTV, m_TilingMotion.m_fStartV);
		}
		json.GetValue(TEXTUREMOTION_OFFSETSPEEDU, m_OffsetMotion.m_fSpeedU);
		json.GetValue(TEXTUREMOTION_OFFSETSPEEDV, m_OffsetMotion.m_fSpeedV);
		json.GetValue(TEXTUREMOTION_OFFSETPERIOD, m_OffsetMotion.m_fPeriod);
		json.GetValue(TEXTUREMOTION_TILINGSPEEDU, m_TilingMotion.m_fSpeedU);
		json.GetValue(TEXTUREMOTION_TILINGSPEEDV, m_TilingMotion.m_fSpeedV);
		json.GetValue(TEXTUREMOTION_TILINGPERIOD, m_TilingMotion.m_fPeriod);
		return true;
	}

	PBRParams::PBRParams() :
		m_vec3EmissiveFactor(0, 0, 0),
		m_EmissiveTextureMotion(),
		m_nEmissiveTextureIndex(-1),
		m_nEmissiveTextureCoordIndex(-1),
		m_nNormalTextureIndex(-1),
		m_nNormalTextureCoordIndex(-1),
		m_fNormalTextureScale(1.0),
		m_nOcclusionTextureIndex(-1),
		m_nOcclusionTextureCoordIndex(-1),
		m_fOcclusionTextureStrength(1.0),
		m_nMaskTextureIndex(-1),
		m_nMaskTextureCoordIndex(-1),
		m_nSemanticTextureIndex(-1),
		m_nSemanticTextureCoordIndex(-1)
	{
	}

	PBRMetallicRough::PBRMetallicRough():m_vec4BaseColor(1.0, 1.0, 1.0, 1.0)
	{	
		m_BaseColorTextureMotion = TextureMotionParameter();
		// 控制金属性强弱的因子
		m_fMetallicFactor = 0.0;
		// 控制粗糙性强弱的因子
		m_fRoughnessFactor = 0.0;
		// 物体基本颜色的纹理在纹理数组中的序号
		m_nBaseColorTextureIndex = -1;
		// 物体基本颜色的纹理坐标在纹理坐标数组中的序号
		 m_nBaseColorTextureCoordIndex = -1;
		// 金属度和粗糙性纹理在纹理数组中的序号(R通道存储金属度，G通道存储粗糙度)
		m_nMetallicRoughnessTextureIndex = -1;
		// 金属度和粗糙性纹理的纹理坐标在数组中的序号
		m_nMetallicRoughnessTextureCoordIndex = -1;
	}

	MaterialParamType PBRMetallicRough::GetType()
	{
		return MAT_PBRMetallicRough;
	}


	TextureUnitState::TextureUnitState() :
		m_TexModMatrix(Matrix4d::IDENTITY)
		, m_MinFilter(FO_LINEAR)
		, m_MaxFilter(FO_LINEAR)
	{
		m_AddressMode.u = TAM_WRAP;
		m_AddressMode.v = TAM_WRAP;
		m_AddressMode.w = TAM_WRAP;
	}

	Pass::Pass() :
		m_strName(U("")),
		m_CullMode(CULL_NONE),
		m_AlphaMode(PBRAM_OPAQUE),
		m_Ambient(ColorValue(1.0, 1.0, 1.0, 1.0)),
		m_Diffuse(ColorValue(1.0, 1.0, 1.0, 1.0)),
		m_Specular(ColorValue(0.0, 0.0, 0.0, 0.0)),
		m_Shininess(0),
		m_fAlphaCutoff(0.01f),
		m_pPRBMaterial(NULL),
		m_strExtensions(U(""))
	{			
	}

	Pass::~Pass()
	{
		TextureUnitStates::const_iterator i, iend;
		iend = m_pTextureUnitStates.end();
		for (i = m_pTextureUnitStates.begin(); i != iend; ++i)
		{
			delete* i;
		}
		m_pTextureUnitStates.clear();

		if (m_pPRBMaterial != NULL)
		{
			delete m_pPRBMaterial;
			m_pPRBMaterial = NULL;
		}
	}

	int Pass::GetPFFMode()
	{
		PolygonFrontFace mode;
		switch (m_CullMode)
		{
		case CULL_NONE:
			mode = PFF_NONE;
			break;
		case CULL_ANTICLOCKWISE:
			mode = PFF_CW;
			break;
		case CULL_CLOCKWISE:
			mode = PFF_CCW;
			break;
		}

		return mode;
	}

	void Pass::SetPFFMode(PolygonFrontFace mode)
	{
		switch (mode)
		{
		case PFF_NONE:
			m_CullMode = CULL_NONE;
			break;
		case PFF_CW:
			m_CullMode = CULL_ANTICLOCKWISE;
			break;
		case PFF_CCW:
			m_CullMode = CULL_CLOCKWISE;
			break;
		}
	}

	TextureUnitState* Pass::CreateTextureUnitState()
	{
		TextureUnitState* t = new TextureUnitState();
		m_pTextureUnitStates.push_back(t);
		if (t->m_strName.empty())
		{// 使用索引作用名称
			size_t idx = m_pTextureUnitStates.size() - 1;
			wstring strName;
			strName = to_wstring(idx);
			t->m_strName = strName;
		}
		return t;
	}

	unsigned short Pass::GetTextureUnitStatesSize(void) const
	{
		return static_cast<unsigned short>(m_pTextureUnitStates.size());
	}

	TextureUnitState* Pass::GetTextureUnitState(unsigned short index)
	{
		if (index >= m_pTextureUnitStates.size())
		{
			return NULL;
		}
		return m_pTextureUnitStates[index];
	}

	TextureUnitState* Pass::GetTextureUnitState(const std::wstring& name)
	{
		TextureUnitStates::iterator i = m_pTextureUnitStates.begin();
		TextureUnitStates::iterator iend = m_pTextureUnitStates.end();
		TextureUnitState* foundTextureUnit = NULL;

		while (i != iend)
		{
			if ((*i)->m_strTextureName == name)
			{
				foundTextureUnit = (*i);
				break;
			}
			++i;
		}

		return foundTextureUnit;
	}

	Technique::Technique()
	{
	}

	Technique::~Technique()
	{
		removeAllPasses();
	}

	Pass* Technique::CreatePass()
	{
		Pass* newPass = new Pass();
		mPasses.push_back(newPass);
		return newPass;
	}

	unsigned short Technique::getNumPasses(void) const
	{
		return mPasses.size();
	}

	Pass* Technique::getPass(unsigned short index)
	{
		return mPasses[index];
	}

	Pass* Technique::getPass(const wstring& name)
	{
		Passes::iterator i = mPasses.begin();
		Passes::iterator iend = mPasses.end();
		Pass* foundPass = NULL;

		while (i != iend)
		{
			if ((*i)->m_strName == name)
			{
				foundPass = (*i);
				break;
			}
			++i;
		}

		return foundPass;
	}

	void Technique::removeAllPasses(void)
	{
		Passes::iterator i, iend;
		iend = mPasses.end();
		for (i = mPasses.begin(); i != iend; ++i)
		{
			delete (*i);
		}
		mPasses.clear();
	}

	Material::Material()
	{
		m_strName = U("matdefault");
	}

	Material::~Material()
	{
		removeAllTechniques();
	}

	Technique* Material::CreateTechnique()
	{
		Technique* t = new Technique();
		mTechniques.push_back(t);
		return t;
	}

	int Material::getNumTechniques(void) const
	{
		return mTechniques.size();
	}

	Technique* Material::getTechnique(unsigned short index)
	{
		return mTechniques[index];
	}

	void Material::removeAllTechniques(void)
	{
		Techniques::iterator i, iend;
		iend = mTechniques.end();
		for (i = mTechniques.begin(); i != iend; ++i)
		{
			delete (*i);
		}
		mTechniques.clear();
	}
}

