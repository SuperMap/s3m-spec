#if !defined(S3MBPAGEDLODTREE_H__5A40E348_CD07_4B4D_B8DB_7AA319481820__INCLUDED_)
#define S3MBPAGEDLODTREE_H__5A40E348_CD07_4B4D_B8DB_7AA319481820__INCLUDED_

#pragma once
#include "JsonValue.h"
#include "S3MBCommon.h"
#include "Variant.h"
#include "Shell.h"
#include <vector>

namespace S3MB
{
	class S3MB_API S3MBTileInfo
	{
	public:
		S3MBTileInfo();
		~S3MBTileInfo();

	public:
		// LOD层号
		int m_nLODNum;
		// 数据文件的路径
		wstring m_strModelPath;
		// 子索引文件的路径
		wstring m_strIndexFilePath;
		// LOD选取模式
		wstring m_strRangeMode;
		// LOD选取阈值因子
		double m_dRangeValue; 
		// 子节点Tile的信息
		vector<S3MBTileInfo*> m_vecChildren;
		// 子节点Tile的状态
		wstring m_strState;
		// 有向包围盒
		OrientedBoundingBox m_OBB;
		// 包围盒（兼容低版本的数据）
		BoundingBox m_AABB;
		// 是否是根Tile
		bool m_bRootTile;
	};

	class S3MB_API S3MBTileTree
	{
	public:
		S3MBTileTree();
		~S3MBTileTree();

		// 传入根Tile的文件路径
		bool LoadFromFile(const wstring& path);

	private:
		void GetTileInfos(RenderOperationGroup* pRootGroup, RenderOperationNode* pNode, const wstring& strDir, map<wstring, S3MBTileInfo*>& mapInfo);
		void SetPagedLODNodeMap(RenderOperationGroup* pGroup, const wstring& strDir, map<wstring, S3MBTileInfo*>& mapInfo);
		void GetGroupBoundingBox(RenderOperationGroup* pGroup, BoundingBox& boundingBox, map<wstring, S3MBTileInfo*>& mapInfo);
		void GetPagedLODBoundingBox(RenderOperationPagedLOD* pPagedLOD, BoundingBox& boundingBox, map<wstring, S3MBTileInfo*>& mapInfo);

	public:
		// 名称
		wstring m_strName;
		// RootTile的信息
		S3MBTileInfo* m_pRootTileInfo;
		// LOD层的总数
		int m_nLODCount;
		// Tile的总数
		int m_nTileCount;
	};

	class S3MB_API S3MBPagedLODTree
	{
	public:
		S3MBPagedLODTree();
		~S3MBPagedLODTree();

		// 读文件
		bool ReadFile(const wstring& path);

		// 写文件
		bool WriteFile(const wstring& path);

	public:
		// 设置/获取Tile的树结构
		void SetTileTree(S3MBTileTree* pTree, bool bOwn = false);
		const S3MBTileTree* GetTileTree() const;

	private:
		// 读取空间索引树结构
		bool ReadTileTree(const wstring& dir, JsonValue& jsonFile);
		// 读取嵌套着的文件
		bool ReadNestedFile(const wstring& path, vector<S3MBTileInfo*>& vecInfo);
		// 读取Tile信息
		bool ReadTileInfo(const wstring& dir, JsonValue& jsonInfo, S3MBTileInfo*& pInfo);

		// 空间索引是一整个文件的情况
		bool WriteWholeFile(const wstring& path);
		bool WriteTileInfo(S3MBTileInfo* pInfo, JsonValue& jsonTree);

	private:
		S3MBTileTree* m_pTileTree;
		bool m_bOwnTileTree;

		// 包围盒使用OBB还是AABB
		bool m_bUseOBB;
	};
}

#endif