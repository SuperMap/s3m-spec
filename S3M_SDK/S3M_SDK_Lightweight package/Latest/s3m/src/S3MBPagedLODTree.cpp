#include "S3MBPagedLODTree.h"
#include "S3MBReader.h"
#include "S3MBUtils.h"
#include "Shell.h"
#include <queue>

namespace S3MB
{
	S3MBTileInfo::S3MBTileInfo()
	{
		m_nLODNum = 0;
		m_dRangeValue = 0.0;
		m_bRootTile = false;
	}

	S3MBTileInfo::~S3MBTileInfo()
	{
		for (int i = 0; i < m_vecChildren.size(); i++)
		{
			delete m_vecChildren[i];
			m_vecChildren[i] = nullptr;
		}
		m_vecChildren.clear();
	}

	S3MBTileTree::S3MBTileTree()
	{
		m_pRootTileInfo = nullptr;
		m_nLODCount = 0;
		m_nTileCount = 0;
	}

	S3MBTileTree::~S3MBTileTree()
	{
		delete m_pRootTileInfo;
		m_pRootTileInfo = nullptr;
	}

	bool S3MBTileTree::LoadFromFile(const wstring& path)
	{
		if (!StringUtil::IsFileExist(path))
		{
			return false;
		}

		S3MBReader reader;
		reader.ReadFile(path);
		RenderOperationGroup* pGroup = reader.GetRenderOperationGroup();
		if (pGroup == nullptr)
		{
			return false;
		}
		// name
		m_strName = StringUtil::GetTitle(path);
		// info
		wstring strKey = StringUtil::GetName(path);
		wstring strDir = StringUtil::GetDir(path);
		map<wstring, S3MBTileInfo*> mapInfo;

		GetTileInfos(pGroup, pGroup, strDir, mapInfo);
		SetPagedLODNodeMap(pGroup, strDir, mapInfo);

		delete m_pRootTileInfo;
		m_pRootTileInfo = mapInfo[strKey];
		m_pRootTileInfo->m_bRootTile = true;
		
		// status
		int nLODNum = 0;
		int nTileCount = 1;
		queue<S3MBTileInfo*> queue;
		queue.push(m_pRootTileInfo);
		vector<S3MBTileInfo*> vecLODInfo;
		while (!queue.empty())
		{
			S3MBTileInfo* pInfo = queue.front();
			pInfo->m_nLODNum = nLODNum;
			// 逐层把Node全部加入数组
			vector<S3MBTileInfo*> vecChildInfo = pInfo->m_vecChildren;
			for (int i = 0; i < vecChildInfo.size(); i++)
			{
				vecLODInfo.push_back(vecChildInfo[i]);
			}
			queue.pop();

			if (queue.empty())
			{
				nLODNum++;
				// 逐层把Node加入队列
				for (int i = 0; i < vecLODInfo.size(); i++)
				{
					nTileCount++;
					queue.push(vecLODInfo[i]);
				}
				vecLODInfo.clear();
			}
		}

		m_nLODCount = nLODNum;
		m_nTileCount = nTileCount;
		return true;
	}

	void S3MBTileTree::GetTileInfos(RenderOperationGroup* pRootGroup, RenderOperationNode* pNode, const wstring& strDir, map<wstring, S3MBTileInfo*>& mapInfo)
	{
		if (pRootGroup == nullptr || pNode == nullptr)
		{
			return;
		}

		if (StringUtil::CompareNoCase(pNode->ClassName(), U("Group")))
		{
			RenderOperationGroup* pGroup = (RenderOperationGroup*)pNode;
			if (pGroup->GetSkeleton().empty())
			{
				return;
			}

			for (int i = 0; i < pGroup->GetNumChildren(); i++)
			{
				RenderOperationNode* pChildNode = pGroup->GetChild(i);
				GetTileInfos(pRootGroup, pChildNode, strDir, mapInfo);
			}
		}
		else if (StringUtil::CompareNoCase(pNode->ClassName(), U("PagedLOD")))
		{
			RenderOperationPagedLOD* pPagedLOD = (RenderOperationPagedLOD*)pNode;
			BoundingSphere sphere;
			for (int i = 0; i < pPagedLOD->GetNumChildren(); i++)
			{
				RenderOperationNode* pChildNode = pPagedLOD->GetChild(i);
				GetTileInfos(pRootGroup, pChildNode, strDir, mapInfo);
				sphere.ExpandBy(pChildNode->GetBoundingSphere());
			}

			if (pRootGroup->GetSkeleton().empty())
			{
				return;
			}

			pPagedLOD->SetBoundingSphere(sphere.GetCenter(), sphere.GetRadius());
			if (!pPagedLOD->GetFileNames().empty())
			{ 
				wstring strPath = StringUtil::GetAbsolutePath(strDir, pPagedLOD->GetFileNames()[0]);
				if (StringUtil::IsFileExist(strPath))
				{
					wstring strNewDir = StringUtil::GetDir(strPath);
					
					S3MBReader reader;
					reader.ReadFile(strPath);
					RenderOperationGroup* pChildGroup = reader.GetRenderOperationGroup();
					if (pChildGroup != nullptr)
					{
						pChildGroup->SetName(strPath);
						GetTileInfos(pChildGroup, pChildGroup, strNewDir, mapInfo);
						SetPagedLODNodeMap(pChildGroup, strNewDir, mapInfo);
					}
				}
			}
		}
		else if (StringUtil::CompareNoCase(pNode->ClassName(), U("Geode")))
		{
			RenderOperationGeode* pGeode = (RenderOperationGeode*)pNode;
			if (pGeode->GetNumSkeleton() == 0)
			{
				return;
			}

			pGeode->ComputerBoundingBox();
			BoundingSphere sphere(pGeode->GetBoundingBox());
			pGeode->SetBoundingSphere(sphere.GetCenter(), sphere.GetRadius());
		}
	}

	void S3MBTileTree::SetPagedLODNodeMap(RenderOperationGroup* pGroup, const wstring& strDir, map<wstring, S3MBTileInfo*>& mapInfo)
	{
		if (pGroup == nullptr)
		{
			return;
		}

		int nChildCount = pGroup->GetNumChildren();
		vector<wstring> vecChilds;

		bool bHasPagedLOD = false, bHasGeode = false;
		for (int nIndex = 0; nIndex < nChildCount; nIndex++)
		{
			RenderOperationNode* pChildNode = pGroup->GetChild(nIndex);
			if (pChildNode == nullptr)
			{
				continue;
			}

			wstring strName = pChildNode->ClassName();
			if (!StringUtil::CompareNoCase(strName, U("PagedLOD")) &&
				!StringUtil::CompareNoCase(strName, U("Geode")))
			{
				continue;
			}

			if (StringUtil::CompareNoCase(strName, U("PagedLOD")))
			{
				RenderOperationPagedLOD* pPagedLOD = (RenderOperationPagedLOD*)pChildNode;
				if (!pPagedLOD->GetFileNames().empty())
				{
					wstring strChild = pPagedLOD->GetFileNames()[0];
					vecChilds.push_back(strChild);
					bHasPagedLOD = true;
				}
			}
			else
			{
				bHasGeode = true;
			}
		}

		wstring strName = StringUtil::GetName(pGroup->GetName());
		map<wstring, S3MBTileInfo*>::iterator iter = mapInfo.find(strName);
		S3MBTileInfo* pTileInfo = nullptr;
		if (iter == mapInfo.end())
		{
			pTileInfo = new S3MBTileInfo();
			pTileInfo->m_strModelPath = strName;
			mapInfo[strName] = pTileInfo;
		}
		else
		{
			pTileInfo = mapInfo[strName];
		}

		if (!bHasPagedLOD && bHasGeode)
		{
			pTileInfo->m_strState = S3MB_PAGEDLODTREE_TILEINFO_STATE_ALLLEAF;
		}
		else if (bHasPagedLOD && !bHasGeode)
		{
			pTileInfo->m_strState = S3MB_PAGEDLODTREE_TILEINFO_STATE_NOLEAF;
		}
		else
		{
			pTileInfo->m_strState = S3MB_PAGEDLODTREE_TILEINFO_STATE_PARTLEAF;
		}

		if (nChildCount > 0)
		{
			RenderOperationNode* pChildNode = pGroup->GetChild(0);
			wstring strClassName = pChildNode->ClassName();
			if (StringUtil::CompareNoCase(strClassName, U("PagedLOD")))
			{
				RenderOperationPagedLOD* pPagedLOD = (RenderOperationPagedLOD*)pGroup->GetChild(0);
				if (!pPagedLOD->GetRanges().empty())
				{
					pTileInfo->m_dRangeValue = pPagedLOD->GetRanges()[0].second;
				}

				RangeMode rangeMode = pPagedLOD->GetRangeMode();
				switch (rangeMode)
				{
				case S3MB::Distance_From_EyePoint:
					pTileInfo->m_strRangeMode = S3MB_PAGEDLODTREE_TILEINFO_RANGEMODE_DISTANCE;
					break;
				case S3MB::Pixel_Size_OnScreen:
					pTileInfo->m_strRangeMode = S3MB_PAGEDLODTREE_TILEINFO_RANGEMODE_PIXELSIZE;
					break;
				case S3MB::Geometric_Error:
					pTileInfo->m_strRangeMode = S3MB_PAGEDLODTREE_TILEINFO_RANGEMODE_GEOMETRY_ERROR;
					break;
				default:
					break;
				}
			}
			else if (StringUtil::CompareNoCase(strClassName, U("Geode")))
			{
				pTileInfo->m_dRangeValue = 0.0;
				pTileInfo->m_strRangeMode = S3MB_PAGEDLODTREE_TILEINFO_RANGEMODE_GEOMETRY_ERROR;
			}
		}

		BoundingBox boundingBox;
		GetGroupBoundingBox(pGroup, boundingBox, mapInfo);
		pTileInfo->m_AABB = boundingBox;
		pTileInfo->m_OBB = pGroup->GetOrientedBoundingBox();

		unsigned int nChildNodeSize = vecChilds.size();
		for (unsigned int i = 0; i < nChildNodeSize; i++)
		{
			wstring strChildFile = strDir + U("\\") + vecChilds[i];
			if (!StringUtil::IsFileExist(strChildFile))
			{
				continue;
			}

			strChildFile = vecChilds[i];
			map<wstring, S3MBTileInfo*>::iterator iter = mapInfo.find(strChildFile);
			if (iter == mapInfo.end())
			{
				S3MBTileInfo* pChildTileInfo = new S3MBTileInfo();
				pChildTileInfo->m_strModelPath = strChildFile;
				pTileInfo->m_vecChildren.push_back(pChildTileInfo);
				mapInfo[strChildFile] = pChildTileInfo;
			}
			else
			{
				pTileInfo->m_vecChildren.push_back(mapInfo[strChildFile]);
			}
		}
	}

	void S3MBTileTree::GetGroupBoundingBox(RenderOperationGroup* pGroup, BoundingBox& boundingBox, map<wstring, S3MBTileInfo*>& mapInfo)
	{
		if (pGroup == nullptr)
		{
			return;
		}

		for (int i = 0; i < pGroup->GetNumChildren(); i++)
		{
			RenderOperationNode* pGroupChild = pGroup->GetChild(i);
			if (pGroupChild == nullptr)
			{
				continue;
			}

			if (StringUtil::CompareNoCase(pGroupChild->ClassName(), U("PagedLOD")))
			{
				RenderOperationPagedLOD* pPagedLOD = (RenderOperationPagedLOD*)pGroupChild;
				for (int j = 0; j < pPagedLOD->GetNumChildren(); j++)
				{
					RenderOperationNode* pLODChild = pPagedLOD->GetChild(j);
					if (StringUtil::CompareNoCase(pLODChild->ClassName(), U("Geode")))
					{
						RenderOperationGeode* pGeode = (RenderOperationGeode*)pLODChild;
						BoundingBox box = pGeode->GetBoundingBox();
						boundingBox.Merge(box);
					}
				}
			}
			else if (StringUtil::CompareNoCase(pGroupChild->ClassName(), U("Geode")))
			{
				RenderOperationGeode* pGeode = (RenderOperationGeode*)pGroupChild;
				BoundingBox box = pGeode->GetBoundingBox();
				boundingBox.Merge(box);
			}
		}
	}

	void S3MBTileTree::GetPagedLODBoundingBox(RenderOperationPagedLOD* pPagedLOD, BoundingBox& boundingBox, map<wstring, S3MBTileInfo*>& mapInfo)
	{
		if (pPagedLOD == nullptr)
		{
			return;
		}

		int nChildCount = pPagedLOD->GetNumChildren();
		S3MBTileInfo* pTileInfo = nullptr;
		if (!pPagedLOD->GetFileNames().empty())
		{
			wstring strName = StringUtil::GetTitle(pPagedLOD->GetFileNames()[0]);
			strName += U(".json");
			map<wstring, S3MBTileInfo*>::iterator iter = mapInfo.find(strName);
			if (iter == mapInfo.end())
			{
				pTileInfo = new S3MBTileInfo();
				pTileInfo->m_strModelPath = strName;
				mapInfo[strName] = pTileInfo;
			}
			else
			{
				pTileInfo = mapInfo[strName];
			}
		}

		BoundingBox box;
		for (int i = 0; i < nChildCount; i++)
		{
			RenderOperationNode* pChildNode = pPagedLOD->GetChild(i);
			if (StringUtil::CompareNoCase(pChildNode->ClassName(), U("PagedLOD")))
			{
				RenderOperationPagedLOD* pChildPagedLOD = (RenderOperationPagedLOD*)pChildNode;
				GetPagedLODBoundingBox(pChildPagedLOD, boundingBox, mapInfo);
			}
			else if (StringUtil::CompareNoCase(pChildNode->ClassName(), U("Geode")))
			{
				RenderOperationGeode* pGeode = (RenderOperationGeode*)pChildNode;
				BoundingBox tempBox = pGeode->GetBoundingBox();
				boundingBox.Merge(tempBox);
				box.Merge(tempBox);
			}
		}

		if (pTileInfo != nullptr)
		{
			pTileInfo->m_AABB = box;
			pTileInfo->m_OBB = pPagedLOD->GetOrientedBoundingBox();
		}
	}

	S3MBPagedLODTree::S3MBPagedLODTree()
	{
		m_pTileTree = nullptr;
		m_bOwnTileTree = false;

		m_bUseOBB = true;
	}

	S3MBPagedLODTree::~S3MBPagedLODTree()
	{
		if (m_bOwnTileTree)
		{
			delete m_pTileTree;
		}
		m_pTileTree = nullptr;
	}

	bool S3MBPagedLODTree::ReadFile(const wstring& path)
	{
		JsonValue jsonFile(objectValue);
		if (!jsonFile.LoadFromFile(path))
		{
			return false;
		}

		wstring dir = StringUtil::GetDir(path);
		return ReadTileTree(dir, jsonFile);
	}

	bool S3MBPagedLODTree::WriteFile(const wstring& path)
	{
		return WriteWholeFile(path);
	}

	void S3MBPagedLODTree::SetTileTree(S3MBTileTree* pTree, bool bOwn)
	{
		m_pTileTree = pTree;
		m_bOwnTileTree = bOwn;
	}

	const S3MBTileTree* S3MBPagedLODTree::GetTileTree() const
	{
		return m_pTileTree;
	}

	bool S3MBPagedLODTree::ReadTileTree(const wstring& dir, JsonValue& jsonFile)
	{
		JsonValue jsonTree;
		if (!jsonFile.GetValue(S3MB_LODTREEEXPORT, jsonTree))
		{
			return false;
		}

		if (m_bOwnTileTree)
		{
			delete m_pTileTree;
		}
		m_pTileTree = new S3MBTileTree();
		m_bOwnTileTree = true;

		jsonTree.GetValue(S3MB_PAGEDLODTREE_NAME, m_pTileTree->m_strName);

		JsonValue jsonTileInfo;
		if (jsonTree.GetValue(S3MB_PAGEDLODTREE_TILEINFO, jsonTileInfo))
		{
			ReadTileInfo(dir, jsonTileInfo, m_pTileTree->m_pRootTileInfo);
		}

		JsonValue jsonStatus;
		if (jsonTree.GetValue(S3MB_PAGEDLODTREE_STATUS, jsonStatus))
		{
			jsonStatus.GetValue(S3MB_PAGEDLODTREE_STATUS_LODCOUNT, m_pTileTree->m_nLODCount);
			jsonStatus.GetValue(S3MB_PAGEDLODTREE_STATUS_TILESCOUNT, m_pTileTree->m_nTileCount);
		}

		return true;
	}

	bool S3MBPagedLODTree::ReadNestedFile(const wstring& path, vector<S3MBTileInfo*>& vecInfo)
	{
		JsonValue jsonFile(objectValue);
		if (!jsonFile.LoadFromFile(path))
		{
			return false;
		}

		JsonValue jsonTree, jsonTileInfo;
		if (!jsonFile.GetValue(S3MB_LODTREEEXPORT, jsonTree) ||
			!jsonTree.GetValue(S3MB_PAGEDLODTREE_TILEINFO, jsonTileInfo))
		{
			return false;
		}

		JsonValue jsonChildren;
		if (jsonTileInfo.GetValue(S3MB_PAGEDLODTREE_TILEINFO_CHILDREN, jsonChildren))
		{
			wstring dir = StringUtil::GetDir(path);
			int nChildCount = jsonChildren.GetArraySize();
			for (int i = 0; i < nChildCount; i++)
			{
				JsonValue jsonOneChild(objectValue), jsonTileInfo;
				if (jsonChildren.GetAt(i, &jsonOneChild) &&
					jsonOneChild.GetValue(S3MB_PAGEDLODTREE_TILEINFO, jsonTileInfo))
				{
					S3MBTileInfo* pChildInfo = nullptr;
					ReadTileInfo(dir, jsonTileInfo, pChildInfo);
					vecInfo.push_back(pChildInfo);
				}
			}
		}

		return true;
	}

	bool S3MBPagedLODTree::ReadTileInfo(const wstring& dir, JsonValue& jsonInfo, S3MBTileInfo*& pInfo)
	{
		pInfo = new S3MBTileInfo();
		jsonInfo.GetValue(S3MB_PAGEDLODTREE_TILEINFO_LODNUM, pInfo->m_nLODNum);
		jsonInfo.GetValue(S3MB_PAGEDLODTREE_TILEINFO_MODELPATH, pInfo->m_strModelPath);
		jsonInfo.GetValue(S3MB_PAGEDLODTREE_TILEINFO_RANGEMODE, pInfo->m_strRangeMode);
		jsonInfo.GetValue(S3MB_PAGEDLODTREE_TILEINFO_RANGEVALUE, pInfo->m_dRangeValue);
		jsonInfo.GetValue(S3MB_PAGEDLODTREE_TILEINFO_STATE, pInfo->m_strState);
		jsonInfo.GetValue(S3MB_PAGEDLODTREE_TILEINFO_BBOX, pInfo->m_OBB);
		// 兼容低版本的数据
		jsonInfo.GetValue(S3MB_PAGEDLODTREE_TILEINFO_BBOX, pInfo->m_AABB);
		if (!pInfo->m_AABB.IsNULL())
		{
			m_bUseOBB = false;
		}

		if (jsonInfo.GetValue(S3MB_PAGEDLODTREE_TILEINFO_INDEXPATH, pInfo->m_strIndexFilePath))
		{
			wstring path = StringUtil::GetAbsolutePath(dir, pInfo->m_strIndexFilePath);
			ReadNestedFile(path, pInfo->m_vecChildren);
		}
		else
		{
			JsonValue jsonChildren;
			if (jsonInfo.GetValue(S3MB_PAGEDLODTREE_TILEINFO_CHILDREN, jsonChildren))
			{
				int nChildCount = jsonChildren.GetArraySize();
				for (int i = 0; i < nChildCount; i++)
				{
					JsonValue jsonOneChild(objectValue), jsonTileInfo;
					if (jsonChildren.GetAt(i, &jsonOneChild) &&
						jsonOneChild.GetValue(S3MB_PAGEDLODTREE_TILEINFO, jsonTileInfo))
					{
						S3MBTileInfo* pChildInfo = nullptr;
						ReadTileInfo(dir, jsonTileInfo, pChildInfo);
						pInfo->m_vecChildren.push_back(pChildInfo);
					}
				}
			}
		}

		return true;
	}

	bool S3MBPagedLODTree::WriteWholeFile(const wstring& path)
	{
		if (m_pTileTree == nullptr)
		{
			return false;
		}

		JsonValue jsonTree(objectValue);
		jsonTree.Add(S3MB_PAGEDLODTREE_NAME, m_pTileTree->m_strName);

		WriteTileInfo(m_pTileTree->m_pRootTileInfo, jsonTree);

		JsonValue jsonStatus(objectValue);
		jsonStatus.Add(S3MB_PAGEDLODTREE_STATUS_LODCOUNT, m_pTileTree->m_nLODCount);
		jsonStatus.Add(S3MB_PAGEDLODTREE_STATUS_TILESCOUNT, m_pTileTree->m_nTileCount);
		jsonTree.Add(S3MB_PAGEDLODTREE_STATUS, jsonStatus);

		JsonValue jsonFile(objectValue);
		jsonFile.Add(S3MB_LODTREEEXPORT, jsonTree);
		jsonFile.SaveToFile(path);

		return StringUtil::IsFileExist(path);
	}

	bool S3MBPagedLODTree::WriteTileInfo(S3MBTileInfo* pInfo, JsonValue& jsonTree)
	{
		if (pInfo == nullptr)
		{
			return false;
		}

		JsonValue jsonInfo(objectValue);
		jsonInfo.Add(S3MB_PAGEDLODTREE_TILEINFO_LODNUM, pInfo->m_nLODNum);
		jsonInfo.Add(S3MB_PAGEDLODTREE_TILEINFO_MODELPATH, pInfo->m_strModelPath);
		jsonInfo.Add(S3MB_PAGEDLODTREE_TILEINFO_RANGEMODE, pInfo->m_strRangeMode);
		jsonInfo.Add(S3MB_PAGEDLODTREE_TILEINFO_RANGEVALUE, pInfo->m_dRangeValue);
		if (!pInfo->m_strState.empty())
		{
			jsonInfo.Add(S3MB_PAGEDLODTREE_TILEINFO_STATE, pInfo->m_strState);
		}

		jsonInfo.Add(S3MB_PAGEDLODTREE_TILEINFO_BBOX, pInfo->m_OBB);

		JsonValue jsonChildren(arrayValue);
		for (int i = 0; i < pInfo->m_vecChildren.size(); i++)
		{
			S3MBTileInfo* pChildInfo = pInfo->m_vecChildren[i];
			if (pChildInfo == nullptr)
			{
				continue;
			}

			JsonValue jsonChildInfo(objectValue);
			WriteTileInfo(pChildInfo, jsonChildInfo);
			jsonChildren.Add(jsonChildInfo);
		}

		if (jsonChildren.GetArraySize() > 0)
		{
			jsonInfo.Add(S3MB_PAGEDLODTREE_TILEINFO_CHILDREN, jsonChildren);
		}

		jsonTree.Add(S3MB_PAGEDLODTREE_TILEINFO, jsonInfo);

		return true;
	}
}
