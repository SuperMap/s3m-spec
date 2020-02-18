#include "Shell.h"

bool RenderOperationNode::IsExistToMaterial(string strMaterialName)
{
	if (m_mapMaterial.size() < 1)
	{
		return false;
	}

	if (m_mapMaterial.find(strMaterialName) != m_mapMaterial.end())
	{
		return true;
	}

	return false;
}

bool RenderOperationNode::IsExistToTexData(string strTextureName)
{
	if (m_mapTextureData.size() < 1)
	{
		return false;
	}

	if (m_mapTextureData.find(strTextureName) != m_mapTextureData.end())
	{
		return true;
	}

	return false;
}

bool RenderOperationNode::IsExistToGeometry(string strName)
{
	if (m_mapGeometry.size() < 1)
	{
		return false;
	}

	if (m_mapGeometry.find(strName) != m_mapGeometry.end())
	{
		return true;
	}

	return false;
}

bool RenderOperationNode::AddMaterial(string strMaterialName, Material* pMaterial)
{
	if (pMaterial == NULL)
	{
		return false;
	}

	if (IsExistToMaterial(strMaterialName))
	{
		return false;
	}

	m_mapMaterial[strMaterialName] = pMaterial;
	return true;
}

bool RenderOperationNode::AddTextureData(string strTextureName, TextureDataInfo* pTexData)
{
	if (pTexData == NULL || pTexData->m_pTextureData == NULL)
	{
		return false;
	}

	if (IsExistToTexData(strTextureName))
	{
		return false;
	}

	m_mapTextureData[strTextureName] = pTexData;
	return true;
}

bool RenderOperationNode::AddGeometry(string strName, Geometry* pGeometry)
{
	if (pGeometry == NULL)
	{
		return false;
	}

	if (IsExistToGeometry(strName))
	{
		return false;
	}

	m_mapGeometry[strName] = pGeometry;
	return true;
}

Material* RenderOperationNode::GetMaterial(string strMaterialName)
{
	std::map<string, Material*>::iterator itor = m_mapMaterial.find(strMaterialName);
	if (itor == m_mapMaterial.end())
	{
		return NULL;
	}
	else
	{
		return itor->second;
	}
}

std::map<std::string, Material*>& RenderOperationNode::GetMaterials()
{
	return m_mapMaterial;
}

TextureDataInfo* RenderOperationNode::GetTextureData(string strTextureName)
{
	std::map<string, TextureDataInfo*>::iterator itor = m_mapTextureData.find(strTextureName);
	if (itor == m_mapTextureData.end())
	{
		return NULL;
	}
	else
	{
		return itor->second;
	}
}

std::map<std::string, TextureDataInfo*>& RenderOperationNode::GetTextureData()
{
	return m_mapTextureData;
}

Geometry* RenderOperationNode::GetGeometry(string strName)
{
	std::map<string, Geometry*>::iterator itor = m_mapGeometry.find(strName);
	if (itor == m_mapGeometry.end())
	{
		return NULL;
	}
	else
	{
		return itor->second;
	}
}

std::map<std::string, Geometry*>& RenderOperationNode::GetGeometry()
{
	return m_mapGeometry;
}

void RenderOperationNode::RemoveAllMaterial()
{
	std::map<string, Material*>::iterator itorMaterial3D;
	for (itorMaterial3D = m_mapMaterial.begin(); itorMaterial3D != m_mapMaterial.end(); itorMaterial3D++)
	{
		delete itorMaterial3D->second;
		itorMaterial3D->second = NULL;
	}
	m_mapMaterial.clear();
}

void RenderOperationNode::RemoveAllTexData()
{
	std::map<string, TextureDataInfo*>::iterator itorTexData;
	for (itorTexData = m_mapTextureData.begin(); itorTexData != m_mapTextureData.end(); itorTexData++)
	{
		delete itorTexData->second;
		itorTexData->second = NULL;
	}
	m_mapTextureData.clear();
}

void RenderOperationNode::RemoveAllGeometry()
{
	std::map<string, Geometry*>::iterator itorGeometry;
	for (itorGeometry = m_mapGeometry.begin(); itorGeometry != m_mapGeometry.end(); itorGeometry++)
	{
		delete itorGeometry->second;
		itorGeometry->second = NULL;
	}
	m_mapGeometry.clear();
}

void RenderOperationNode::RemoveGeometry(string strName)
{
	std::map<string, Geometry*>::iterator itorGeometry = m_mapGeometry.find(strName);
	if (itorGeometry != m_mapGeometry.end())
	{
		if (itorGeometry->second != NULL)
		{
			delete itorGeometry->second;
			itorGeometry->second = NULL;
		}
		m_mapGeometry.erase(itorGeometry);
	}
}

void RenderOperationGroup::AddChild(RenderOperationNode* pNode)
{
	if (pNode == NULL)
	{
		return;
	}

	m_vecChildNode.push_back(pNode);
}

RenderOperationNode* RenderOperationGroup::GetChild(int nIndex)
{
	if (nIndex < m_vecChildNode.size())
	{
		return m_vecChildNode[nIndex];
	}
	else
	{
		return NULL;
	}
}

void RenderOperationGroup::RemoveAllChildren()
{
	for (int i = 0; i < m_vecChildNode.size(); i++)
	{
		delete m_vecChildNode[i];
		m_vecChildNode[i] = NULL;
	}
	m_vecChildNode.clear();
}

RenderOperationPagedLOD::~RenderOperationPagedLOD()
{
	m_vecRangeList.clear();
	m_vecFileName.clear();
	m_mapBoundingSphere.clear();
}

void RenderOperationGeode::AddGeometry(Geometry* pROGeoemtry)
{
	if (pROGeoemtry == NULL)
	{
		return;
	}

	m_vecROGeometry.push_back(pROGeoemtry);
}

Geometry* RenderOperationGeode::GetGeometry(int nIndex)
{
	if (nIndex < m_vecROGeometry.size())
	{
		return m_vecROGeometry[nIndex];
	}
	else
	{
		return NULL;
	}
}

void RenderOperationGeode::ReplaceGeometry(int nIndex, Geometry* pGeometry, bool bRelease /*= false*/)
{
	if (nIndex >= m_vecROGeometry.size())
	{
		return;
	}

	if (bRelease)
	{
		delete m_vecROGeometry[nIndex];
		m_vecROGeometry[nIndex] = NULL;
	}

	m_vecROGeometry[nIndex] = pGeometry;
}

void RenderOperationGeode::RemoveGeometry(string strName)
{
	std::vector<Geometry*>::iterator itoGeo = m_vecROGeometry.begin();
	for (; itoGeo != m_vecROGeometry.end();)
	{
		if ((*itoGeo)->m_strGeoName == strName)
		{
			itoGeo = m_vecROGeometry.erase(itoGeo);
		}
		else
			itoGeo++;
	}
}

void RenderOperationGeode::Release()
{
	m_vecROGeometry.clear();
	m_BoundingBox.SetNULL();
}

void RenderOperationGeode::ComputerBoundingBox()
{
	m_BoundingBox.SetNULL();

	int i = 0;
	for (i = 0; i < m_vecROGeometry.size(); i++)
	{
		Geometry* pROGeometry = m_vecROGeometry[i];
		if (pROGeometry == NULL)
		{
			continue;
		}

		if (!pROGeometry->m_BoundingBox.IsVaild())
		{
			pROGeometry->ComputerBoundingBox();
		}

		m_BoundingBox.Merge(pROGeometry->m_BoundingBox);
	}
}

void BoundingSphere::ExpandBy(const BoundingSphere& bs)
{
	//原球无效
	if (!valid())
	{
		m_center = bs.m_center;
		m_radius = bs.m_radius;
		return;
	}

	//两包围球球心间的距离
	double d = (m_center - bs.m_center).Length();

	//bs在原球之内
	if (d + bs.m_radius <= m_radius)
	{
		return;
	}

	//bs包围原球 
	if (d + m_radius <= bs.m_radius)
	{
		m_center = bs.m_center;
		m_radius = bs.m_radius;
		return;
	}

	double new_radius = (m_radius + d + bs.m_radius) * 0.5;
	double ratio = (new_radius - m_radius) / d;

	m_center[0] += (bs.m_center[0] - m_center[0]) * ratio;
	m_center[1] += (bs.m_center[1] - m_center[1]) * ratio;
	m_center[2] += (bs.m_center[2] - m_center[2]) * ratio;

	m_radius = new_radius;
}
