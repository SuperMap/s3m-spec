#pragma once

#include "Toolkit3D/UGJsonUtils.h"
#include "Base3D/UGModelNode.h"
#include "Base3D/UGModelPagedLodTree.h"
#include "FileParser/UGFileParseModel.h"

namespace UGC
{
class CACHEBUILDER3D_API UGS3MBPagedLodTree
{
public:
	UGS3MBPagedLodTree();
	UGS3MBPagedLodTree(UGModelNode* pModelNode);
	~UGS3MBPagedLodTree(void);

public:
	//! \brief 打开索引信息文件(.xml or .json)，得到树形结构（壳儿）
	UGbool ReadFromFile(const UGString& strFileName);

	//! \brief 解析s3mb根节点，得到树形结构
	UGbool LoadFromTile(const UGString& strSCPDir, const UGString& strRootTile);

	//! \brief 信息保存
	UGbool SaveToString(UGString& strContent) const;

	//! \brief 信息保存到文件
	UGbool SaveToFile(UGString& strFilePath) const;

	//! \brief 返回以后传给UGModelPagedLodTree就能查询啦~
	UGModelNode* GetModelNode();

private:
	//! \brief 加载一个切片的信息
	UGbool LoadPatchInfo(UGMarkup& markup, UGModelPagedPatch* pPatchParent, UGint nCurrentLODNum);

	//! \brief 添加一个Patch
	UGModelPagedPatch* AddOnePatch(UGint nCurrentLODNum,\
		const UGString& strTileName, UGdouble dbRangeMin, const UGBoundingBox& bbox);

	//! \brief 保存一个Patch的信息
	void SaveTileInfo(std::vector<UGModelPagedPatch*>& vecPatch, \
		UGdouble dbLodDis, UGJsonValue& jsonValue) const;

	//! \brief 构造json对象，准备保存
	void MakeJsonValue(UGJsonValue& jsonValue) const;

	//! \brief 获取根节点Patch
	std::vector<UGModelPagedPatch*> GetRootPatches() const;

private:
	//! \brief 模型对象
	UGModelNode* m_pModelNode;
	//! \brief 最后是否需要删除
	UGbool m_bModelNodeOwned;
};


}
