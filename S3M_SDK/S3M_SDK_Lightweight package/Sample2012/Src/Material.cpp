#include "Material.h"
#include <assert.h>


PBRParams::PBRParams():
	m_vec3EmissiveFactor(0,0,0), m_vec4BaseColor(1.0, 1.0, 1.0, 1.0)
{
	m_AlphaMode = PBRAM_OPAQUE;
	m_fAlphaCutoff = 0.5;
}

PBRMetallicRough::PBRMetallicRough()
{
	m_fMetallicFactor = 0.0;
	m_fRoughnessFactor = 0.0;
}

PBRSpecularGlossy::PBRSpecularGlossy():
	m_vec4DiffuseFactor(1.0, 1.0, 1.0, 1.0), m_vec3SpecularFactor(1.0, 1.0, 1.0)
{
	m_fGlossinessFactor = 0.0;
}

TextureUnitState::TextureUnitState():
	m_TexModMatrix(Matrix4d::IDENTITY)
	, m_MinFilter(FO_LINEAR)
	, m_MaxFilter(FO_LINEAR)
{
	m_AddressMode.u = TAM_WRAP;
	m_AddressMode.v = TAM_WRAP;
	m_AddressMode.w = TAM_WRAP;
}


Pass::Pass(): m_strName("")
		, m_CullMode(CULL_NONE)
		, m_Ambient(ColorValue(1.0, 1.0, 1.0, 1.0))
		, m_Diffuse(ColorValue(1.0, 1.0, 1.0, 1.0))
		, m_Specular(ColorValue(0.0, 0.0, 0.0, 0.0))
		, m_Shininess(0)
		, m_TransparentSorting(false)
	{
		m_pPRBMaterial = NULL;
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
	TextureUnitState *t = new TextureUnitState();
	m_pTextureUnitStates.push_back(t);
	// if texture unit state name is empty then give it a default name based on its index
	if (t->m_strName.empty())
	{
		// its the last entry in the container so its index is size - 1
		unsigned int idx = m_pTextureUnitStates.size() - 1;
		string strName;
		strName = to_string(idx);
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

Technique::Technique(){}

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

Pass* Technique::getPass(const string& name)
{
	Passes::iterator i = mPasses.begin();
	Passes::iterator iend = mPasses.end();
	Pass* foundPass = NULL;

	// iterate through techniques to find a match
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

Material::Material()
{
	m_strName = "matdefault";
}

Technique* Material::CreateTechnique()
{
	Technique *t = new Technique();
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

