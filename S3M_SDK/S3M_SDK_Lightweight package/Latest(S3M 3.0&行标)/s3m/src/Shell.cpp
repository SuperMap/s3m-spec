#include "Shell.h"
#include "S3MBUtils.h"

namespace S3MB
{
	RenderOperationNode::RenderOperationNode()
	{
	}

	RenderOperationNode::~RenderOperationNode()
	{
		RemoveAllMaterial();
		RemoveAllTexData();
		RemoveAllSkeleton();
	}

	RenderOperationGroup* RenderOperationNode::AsGroup()
	{
		return NULL;
	}

	RenderOperationGeode* RenderOperationNode::AsGeode()
	{
		return NULL;
	}

	wstring RenderOperationNode::ClassName()
	{
		return U("Node");
	}

	void RenderOperationNode::SetName(wstring strName)
	{
		m_strName = strName;
	}

	wstring RenderOperationNode::GetName()
	{
		return m_strName;
	}

	BoundingSphere RenderOperationNode::GetBoundingSphere()
	{
		return m_BoundingSphere;
	}

	void RenderOperationNode::SetBoundingSphere(Vector3d vecCenter, double dRadius) {
		m_BoundingSphere.m_center = vecCenter;
		m_BoundingSphere.m_radius = dRadius;
	}

	OrientedBoundingBox RenderOperationNode::GetOrientedBoundingBox()
	{ 
		return m_OBB; 
	}

	void RenderOperationNode::SetOrientedBoundingBox(OrientedBoundingBox orientedBoundingBox)
	{
		m_OBB = orientedBoundingBox;
	}

	BoundingBox RenderOperationNode::GetBoundingBox()
	{
		return m_BoundingBox;
	}

	void RenderOperationNode::SetBoundingBox(BoundingBox boundingBox)
	{
		m_BoundingBox = boundingBox;
	}

	void RenderOperationNode::SetPosition(Matrix4d mat)
	{
		m_matLocalView = mat;
	}

	Matrix4d RenderOperationNode::GetPosition()
	{
		return m_matLocalView;
	}

	bool RenderOperationNode::IsMaterialExist(wstring strMaterialName)
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

	bool RenderOperationNode::IsTexDataExist(wstring strTextureName)
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

	bool RenderOperationNode::IsSkeletonExist(wstring strName)
	{
		if (m_mapSkeleton.size() < 1)
		{
			return false;
		}

		if (m_mapSkeleton.find(strName) != m_mapSkeleton.end())
		{
			return true;
		}

		return false;
	}

	bool RenderOperationNode::AddMaterial(wstring strMaterialName, Material* pMaterial)
	{
		if (pMaterial == NULL)
		{
			return false;
		}

		if (IsMaterialExist(strMaterialName))
		{
			return false;
		}

		m_mapMaterial[strMaterialName] = pMaterial;
		return true;
	}

	bool RenderOperationNode::AddTextureData(wstring strTextureName, TextureDataInfo* pTexData)
	{
		if (pTexData == NULL || pTexData->m_pTextureData == NULL)
		{
			return false;
		}

		if (IsTexDataExist(strTextureName))
		{
			return false;
		}

		m_mapTextureData[strTextureName] = pTexData;
		return true;
	}

	bool RenderOperationNode::AddSkeleton(wstring strName, Skeleton* pSkeleton)
	{
		if (pSkeleton == NULL)
		{
			return false;
		}

		if (IsSkeletonExist(strName))
		{
			return false;
		}

		m_mapSkeleton[strName] = pSkeleton;
		return true;
	}

	Material* RenderOperationNode::GetMaterial(wstring strMaterialName)
	{
		std::map<wstring, Material*>::iterator itor = m_mapMaterial.find(strMaterialName);
		if (itor == m_mapMaterial.end())
		{
			return NULL;
		}
		else
		{
			return itor->second;
		}
	}

	std::map<std::wstring, Material*>& RenderOperationNode::GetMaterials()
	{
		return m_mapMaterial;
	}

	TextureDataInfo* RenderOperationNode::GetTextureData(wstring strTextureName)
	{
		std::map<wstring, TextureDataInfo*>::iterator itor = m_mapTextureData.find(strTextureName);
		if (itor == m_mapTextureData.end())
		{
			return NULL;
		}
		else
		{
			return itor->second;
		}
	}

	std::map<std::wstring, TextureDataInfo*>& RenderOperationNode::GetTextureData()
	{
		return m_mapTextureData;
	}

	Skeleton* RenderOperationNode::GetSkeleton(wstring strName)
	{
		std::map<wstring, Skeleton*>::iterator itor = m_mapSkeleton.find(strName);
		if (itor == m_mapSkeleton.end())
		{
			return NULL;
		}
		else
		{
			return itor->second;
		}
	}

	std::map<std::wstring, Skeleton*>& RenderOperationNode::GetSkeleton()
	{
		return m_mapSkeleton;
	}

	void RenderOperationNode::RemoveAllMaterial()
	{
		std::map<wstring, Material*>::iterator itorMaterial3D;
		for (itorMaterial3D = m_mapMaterial.begin(); itorMaterial3D != m_mapMaterial.end(); itorMaterial3D++)
		{
			delete itorMaterial3D->second;
			itorMaterial3D->second = NULL;
		}
		m_mapMaterial.clear();
	}

	void RenderOperationNode::RemoveAllTexData()
	{
		std::map<wstring, TextureDataInfo*>::iterator itorTexData;
		for (itorTexData = m_mapTextureData.begin(); itorTexData != m_mapTextureData.end(); itorTexData++)
		{
			delete itorTexData->second;
			itorTexData->second = NULL;
		}
		m_mapTextureData.clear();
	}

	void RenderOperationNode::RemoveAllSkeleton()
	{
		std::map<wstring, Skeleton*>::iterator itorSkeleton;
		for (itorSkeleton = m_mapSkeleton.begin(); itorSkeleton != m_mapSkeleton.end(); itorSkeleton++)
		{
			delete itorSkeleton->second;
			itorSkeleton->second = NULL;
		}
		m_mapSkeleton.clear();
	}

	void RenderOperationNode::RemoveSkeleton(wstring strName)
	{
		std::map<wstring, Skeleton*>::iterator itorSkeleton = m_mapSkeleton.find(strName);
		if (itorSkeleton != m_mapSkeleton.end())
		{
			if (itorSkeleton->second != NULL)
			{
				delete itorSkeleton->second;
				itorSkeleton->second = NULL;
			}
			m_mapSkeleton.erase(itorSkeleton);
		}
	}

	void RenderOperationNode::ClearMaterial3D()
	{
		m_mapMaterial.clear();
	}

	void RenderOperationNode::ClearTexData()
	{
		m_mapTextureData.clear();
	}

	void RenderOperationNode::ClearSkeleton()
	{
		m_mapSkeleton.clear();
	}

	RenderOperationGroup::RenderOperationGroup()
		:m_pParentNode(NULL)
	{
	}

	RenderOperationGroup::~RenderOperationGroup()
	{
		RemoveAllChildren();
		m_pParentNode = NULL;
	}

	RenderOperationGroup* RenderOperationGroup::AsGroup()
	{
		return this;
	}

	wstring RenderOperationGroup::ClassName()
	{
		return U("Group");
	}

	void RenderOperationGroup::SetParentNode(RenderOperationNode* pNode)
	{
		m_pParentNode = pNode;
	}

	RenderOperationNode* RenderOperationGroup::GetParentNode()
	{
		return m_pParentNode;
	}

	void RenderOperationGroup::AddChild(RenderOperationNode* pNode)
	{
		if (pNode == NULL)
		{
			return;
		}

		m_vecChildNode.push_back(pNode);
	}

	int RenderOperationGroup::GetNumChildren()
	{
		return m_vecChildNode.size();
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

	void RenderOperationGroup::ReComputeBoundingBox(bool bReCalSkeletonBBOX/* = true*/)
	{
		for (int i = 0; i < GetNumChildren(); i++)
		{
			if (StringUtil::CompareNoCase(GetChild(i)->ClassName(), U("Geode")))
			{
				RenderOperationGeode* pGeode = (RenderOperationGeode*)(GetChild(i));
			
				if (bReCalSkeletonBBOX)
				{// 计算所有Skeleton的包围盒
					for (i = 0; i < pGeode->GetNumSkeleton(); i++)
					{
						Skeleton* pSkeleton = pGeode->GetSkeleton(i);
						if (pSkeleton == NULL)
							continue;
						pSkeleton->ComputerBoundingBox();
					}
				}

				pGeode->ComputerBoundingBox();
				BoundingBox bboxObj = pGeode->GetBoundingBox();
                Matrix4d matPos = pGeode->GetPosition();
                bboxObj.Transform(matPos);
				BoundingSphere bSphere(bboxObj);
				pGeode->SetBoundingSphere(bSphere.GetCenter(), bSphere.GetRadius());
			}
			else if (StringUtil::CompareNoCase(GetChild(i)->ClassName(), U("PagedLOD")))
			{
				RenderOperationPagedLOD* pROPagedLOD = (RenderOperationPagedLOD*)GetChild(i);
				pROPagedLOD->ReComputeBoundingBox(bReCalSkeletonBBOX);
			}
		}
	}

	RenderOperationPagedLOD::RenderOperationPagedLOD()
		:m_nRangeMode(Pixel_Size_OnScreen)
	{
	}

	RenderOperationPagedLOD::~RenderOperationPagedLOD()
	{
		m_vecRangeList.clear();
		m_vecFileName.clear();
		m_mapBoundingSphere.clear();
	}

	RenderOperationGroup* RenderOperationPagedLOD::AsGroup()
	{
		return this;
	}

	wstring RenderOperationPagedLOD::ClassName()
	{
		return U("PagedLOD");
	}

	std::vector<std::pair<float, float> >& RenderOperationPagedLOD::GetRanges()
	{
		return m_vecRangeList;
	}

	std::vector<wstring>& RenderOperationPagedLOD::GetFileNames()
	{
		return m_vecFileName;
	}

	void RenderOperationPagedLOD::SetRangeMode(RangeMode nMode)
	{
		m_nRangeMode = nMode;
	}

	RangeMode RenderOperationPagedLOD::GetRangeMode()
	{
		return m_nRangeMode;
	}

	void RenderOperationPagedLOD::ReComputeBoundingBox(bool bReCalSkeletonBBOX/* = true*/)
	{
		BoundingBox mBox;
		OrientedBoundingBox mOBB;
		for (int i = 0; i < GetNumChildren(); i++)
		{
			if (StringUtil::CompareNoCase(GetChild(i)->ClassName(), U("Geode")))
			{
				RenderOperationGeode* pGeode = (RenderOperationGeode*)(GetChild(i));

				if (bReCalSkeletonBBOX)
				{// 计算所有Skeleton的包围盒
					for (i = 0; i < pGeode->GetNumSkeleton(); i++)
					{
						Skeleton* pSkeleton = pGeode->GetSkeleton(i);
						if (pSkeleton == NULL)
							continue;
						pSkeleton->ComputerBoundingBox();
					}
				}
				pGeode->ComputerBoundingBox();

				BoundingBox geodeBox = pGeode->GetBoundingBox();
                Matrix4d matPos = pGeode->GetPosition();
                geodeBox.Transform(matPos);
				OrientedBoundingBox obb;
				geodeBox.ToOrientedBoundingBox(obb);
				mOBB.Merge(obb);
				mBox.Merge(geodeBox);
				BoundingSphere geodeSphere(geodeBox);
				pGeode->SetBoundingSphere(geodeSphere.GetCenter(), geodeSphere.GetRadius());
			}
			else if (StringUtil::CompareNoCase(GetChild(i)->ClassName(), U("PagedLOD")))
			{
				// assert(false);
			}
		}
		m_BoundingBox = mBox;
		m_OBB = mOBB;
		BoundingSphere mSphere(mBox);
		SetBoundingSphere(mSphere.GetCenter(), mSphere.GetRadius());
	}

	void RenderOperationPagedLOD::SetNodes(const std::vector<Node*>& vecNode, bool bOwn /*= true*/)
	{
		if (bOwn)
		{
			for (int i = 0; i < m_vecNode.size(); i++)
			{
				delete m_vecNode[i];
				m_vecNode[i] = NULL;
			}
		}

		m_vecNode.clear();
		m_vecNode = vecNode;
		m_bOwnNode = bOwn;
	}

	void RenderOperationPagedLOD::SetSkins(const std::vector<Skin*>& vecSkin, bool bOwn /*= true*/)
	{
		if (bOwn)
		{
			for (int i = 0; i < m_vecSkin.size(); i++)
			{
				delete m_vecSkin[i];
				m_vecSkin[i] = NULL;
			}
		}

		m_vecSkin.clear();
		m_vecSkin = vecSkin;
		m_bOwnSkin = bOwn;
	}

	void RenderOperationPagedLOD::SetAnimationStates(const std::vector<Animation*>& vecAnimation, bool bOwn /*= true*/)
	{
		if (bOwn)
		{
			for (int i = 0; i < m_vecAnimationState.size(); i++)
			{
				delete m_vecAnimationState[i];
				m_vecAnimationState[i] = NULL;
			}
		}

		m_vecAnimationState.clear();
		m_vecAnimationState = vecAnimation;
		m_bOwnAnimationState = bOwn;
	}

	void RenderOperationPagedLOD::SetAnimationCurves(const std::vector<std::map<wstring, AnimationCurve*> >& vecAnimationCurve, bool bOwn /*= true*/)
	{
		if (bOwn)
		{
			for (int i = 0; i < m_vecAnimationCurve.size(); i++)
			{
				std::map<wstring, AnimationCurve*>& mapCurve = m_vecAnimationCurve[i];
				std::map<wstring, AnimationCurve*>::iterator itor;
				for (itor = mapCurve.begin(); itor != mapCurve.end(); itor++)
				{
					delete itor->second;
					itor->second = NULL;
				}
				mapCurve.clear();
			}
		}

		m_vecAnimationCurve.clear();
		m_vecAnimationCurve = vecAnimationCurve;
		m_bOwnAnimationCurve = bOwn;
	}

	RenderOperationGeode::RenderOperationGeode()
	{
	}

	RenderOperationGeode::~RenderOperationGeode()
	{
		Release();
	}

	RenderOperationGeode* RenderOperationGeode::AsGeode()
	{
		return this;
	}

	wstring RenderOperationGeode::ClassName()
	{
		return U("Geode");
	}

	void RenderOperationGeode::AddSkeleton(Skeleton* pROGeoemtry)
	{
		if (pROGeoemtry == NULL)
		{
			return;
		}

		m_vecSkeleton.push_back(pROGeoemtry);
	}

	int RenderOperationGeode::GetNumSkeleton()
	{
		return m_vecSkeleton.size();
	}

	Skeleton* RenderOperationGeode::GetSkeleton(int nIndex)
	{
		if (nIndex < m_vecSkeleton.size())
		{
			return m_vecSkeleton[nIndex];
		}
		else
		{
			return NULL;
		}
	}

	void RenderOperationGeode::ReplaceSkeleton(int nIndex, Skeleton* pSkeleton, bool bRelease /*= false*/)
	{
		if (nIndex >= m_vecSkeleton.size())
		{
			return;
		}

		if (bRelease)
		{
			delete m_vecSkeleton[nIndex];
			m_vecSkeleton[nIndex] = NULL;
		}

		m_vecSkeleton[nIndex] = pSkeleton;
	}

	void RenderOperationGeode::RemoveSkeleton(wstring strName)
	{
		std::vector<Skeleton*>::iterator itoGeo = m_vecSkeleton.begin();
		for (; itoGeo != m_vecSkeleton.end();)
		{
			if ((*itoGeo)->m_strGeoName == strName)
			{
				itoGeo = m_vecSkeleton.erase(itoGeo);
			}
			else
				itoGeo++;
		}
	}

	void RenderOperationGeode::Release()
	{
		m_vecSkeleton.clear();
		m_BoundingBox.SetNULL();
	}

	void RenderOperationGeode::ComputerBoundingBox()
	{
		m_BoundingBox.SetNULL();

		for (unsigned int i = 0; i < m_vecSkeleton.size(); i++)
		{
			Skeleton* pSkeleton = m_vecSkeleton[i];
			if (pSkeleton == NULL)
			{
				continue;
			}

			if (pSkeleton->m_BoundingBox.IsNULL())
			{
				pSkeleton->ComputerBoundingBox();
			}
			BoundingBox box = pSkeleton->m_BoundingBox;
			box.Transform(m_matLocalView);
			m_BoundingBox.Merge(box);
			OrientedBoundingBox obb;
			box.ToOrientedBoundingBox(obb);
			m_OBB.Merge(obb);
		}
		m_BoundingSphere.m_center = m_BoundingBox.GetCenter();
		m_BoundingSphere.m_radius = (m_BoundingBox.GetMax() - m_BoundingBox.GetMin()).Length() / 2;
	}
}