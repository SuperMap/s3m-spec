#pragma once

#include "Base3D/UGModelNodeEntities.h"
#include "Base3D/UGModelEM.h"

namespace UGC
{

//! \brief 一个指定的骨架
class BASE3D_API UGCSkeletonID
{
public:
	//! \brief 初始化值为非法值
	UGCSkeletonID();
	UGCSkeletonID(UGint nL,UGint nP, UGint nG, UGint nS);

	//! \brief 初始化值为非法值
	~UGCSkeletonID();

	UGCSkeletonID& operator = (const UGCSkeletonID& other);

	UGbool operator == (const UGCSkeletonID &id);

	void Reset();

	UGbool IsValid() const;

	//! \brief nLOD=-1则为精细层
	UGint m_nIdx_LOD;

	//! \brief >=0
	UGint m_nIdx_Patch;

	//! \brief >=0 
	UGint m_nIdx_Geode;

	//! \brief >=0 
	UGint m_nIdx_Skeleton;
};

class BASE3D_API UGModelGeode
{
	friend class UGModelNode;

public:
	UGModelGeode();

	//! \brief 完全拷贝.ModelNode operator = 使用
	UGModelGeode(UGModelGeode& geode);
	
	~UGModelGeode();

public:
	//! \brief 按geode构造，实体名与实体的对应关系在 mapPack 里
	void MakeFrom(UGModelGeode& geode, \
		UGModelEMapPack& mapPack, UGbool bShellOnly=FALSE);

	//! \brief 从mapPackSrc里的对象，取出名字A，去mapPackDes里找到实体ADes，加入Geode
	//! \brief ModelNode MakeFrom使用
	void MakeFrom(UGModelEMapPack& mapPackSrc, \
		UGModelEMapPack& mapPackDes, UGbool bShellOnly=FALSE);

	//! \brief 把mapPack里的骨架全部加入
	//! \brief 会从骨架中提取出材质名；只把用到的材质和纹理加入
	//! \brief CAD转模型对象使用
	void MakeFrom(UGModelEMapPack& mapPack);

	//! \brief 构造模型对象。pSkeletonSrc 会被释放掉，外部不能再使用
	//! \brief 会默认新建一个材质对象
	void MakeFrom(UGModelSkeleton* pSkeletonSrc, UGMatrix4d matGeode);

	//! \brief 从EM中取出骨架的材质和纹理，加入Geode
	UGbool AddSkeletonAttShell(UGModelEM* pEM, UGString strSkeleton);

	//! \brief 把mapPack里的骨架全部加入
	void AddMapPack(UGModelEMapPack& mapPack);
	
	//! \brief 移除Geometry;返回值具备实际意义！
	UGbool ReplaceSkeleton(UGModelSkeleton* pGeometry, UGModelSkeletonPtr pSkeletonNew);

	//! \brief 添加Geometry
	UGbool AddSkeleton(UGModelSkeletonPtr pModelSkeleton);
	UGbool AddMaterial(UGModelMaterialPtr pMaterial3D);
	UGbool AddTexture(UGModelTexturePtr pTexData);

	//! \brief 添加对象
	void AddEntity(UGModelEntity* pEntity);
	void AddTexture(std::vector<UGModelTexture*>& vecEntity);

	//! \brief 添加壳儿 Geometry
	void AddEntityShell(const UGModelEntity::METype eType, \
		const UGString strSkeleton);

	//! \brief 创建实体，不改名字，创建失败返回空
	UGModelSkeletonPtr CreateSkeleton(const UGString strName);
	UGModelMaterialPtr CreateMaterial(const UGString strName);
	UGModelTexturePtr CreateTexture(const UGString strName);

	//! \brief 获取数据包引用
	UGModelEMapPack& GetEMapPack();

	//! \brief 获得Entity总数
	UGint GetEntityNum(UGModelEntity::METype metype) const;

	//! \brief 获得实体名称
	std::vector<UGString> GetEntityNames(UGModelEntity::METype metype);

	UGModelSkeletonPtr GetSkeleton(const UGString strName);
	UGModelMaterialPtr GetMaterial(const UGString strName);
	UGModelTexturePtr GetTexture(const UGString strName);

	//! \brief 名字是否已经存在
	UGbool IsNameExisted(const UGString strName);

	//! \brief 用数据包更新模型
	UGbool Update(UGModelEMapPack &mapPack);

	//! \brief 实体名字更新，慎用。要求实体未加载，对象Update后，数据集刷新调用
	//! \brief pEMSrc有刚刚Update的信息
	UGbool UpdateShell(UGModelEM* pEMSrc);

	//! \brief 给一个可用的名字
	UGString GetUnoccupiedName(UGString strName);

	//! \brief 包内所有骨架的包围盒，包含骨架的矩阵和包的矩阵
	void ComputeBoundingBox();

	//! \brief 包内所有骨架的包围盒，包含骨架的矩阵和包的矩阵
	UGBoundingBox GetBoundingBox();

	//! \brief 设置包围盒,包内所有骨架的包围盒，包含骨架的矩阵和包的矩阵
	void SetBoundingBox(UGBoundingBox box);

	//! \brief 单个骨架的包围盒
	UGBoundingBox ComputeSkeletonBoundingBox(UGModelSkeletonPtr& pSkeleton);

	//! \brief 作用于各Geode的矩阵
	void ReMultiplyMatrix(const UGMatrix4d& m);

	//! \brief 设置位置
	void SetPosition(UGMatrix4d mat);

	//! \brief 获得位置
	const UGMatrix4d& GetPosition() const ;

	UGint GetShellDataSize();

	//! \brief 写入，不包含实体数据
	UGbool SaveShell(UGStream& stream);

	//! \brief 加载
	UGbool LoadShell(UGStream& stream);

	//! \brief 从加载EM填充vector
	UGbool LoadEntities(UGModelEM* pEntitiesManager, UGModelEntity::METype mtype = UGModelEntity::etUnknown);

	//! \brief 卸载实体数据
	//! \param mtype [in] 卸载的数据类型
	void UnloadEntities(UGModelEntity::METype mtype = UGModelEntity::etUnknown);

	//! \brief 卸载实体对象
	void ReleaseEntity(const UGString strName, UGModelEntity::METype mtype);

	//! \brief 从内存移除骨架对象及其包含的材质纹理（如果没人在用的话）
	//! \brief strSkeleton 骨架名称
	UGbool RemoveSkeleton(const UGString strSkeleton);

	//! \brief 清理
	void Clear();

	//! \brief 替换实体名字，慎用。数据集追加时使用
	void ReplaceEntityName(std::map<UGString, UGString>& mapNames);

private:
	//! \brief 加载实体
	void LoadEntities(UGModelEMapPack& mapPack);

public:
	//! \brief 名字
	UGString m_strName;
	//! \brief FBX 属性信息
	UGString m_strAttTableName;
	UGint m_nAttID;

private:
	//! \brief 位置矩阵
	UGMatrix4d m_matLocalView;

	//! \brief UGGeoModelPro 重新放置插入点时计算出的修正矩阵
	UGMatrix4d m_matSupplement;

	//! \brief 包围盒
	UGBoundingBox m_BoundingBox;

	//! \brief 数据包
	UGModelEMapPack m_mapPack;
};

class UGModelPagedLOD;
class UGModelNode;

class BASE3D_API UGModelPagedPatch
{
friend class UGModelPagedLOD;
friend class UGModelNode;
friend class UGModelNodeTools;
friend class UGFileParserOSGTile;
friend class UGFileParserS3MTile;
friend class UGFileParserS3MBTile;
friend class UGS3MBWriter;
friend class UGS3MBModelTools;
friend class UGTileStuff;

protected:
	UGModelPagedPatch();

	UGModelPagedPatch(UGModelPagedLOD* pPagedLOD, UGfloat range);

	~UGModelPagedPatch();

public:
		//! \brief 完全拷贝.ModelNode operator = 使用
	void MakeFrom(UGModelPagedLOD* pPagedLOD, UGModelPagedPatch* pPatch);

	//! \brief 按照 pPatch 构建一个新的给pPagedLOD
	void MakeFrom(UGModelPagedLOD* pPagedLOD, UGModelPagedPatch* pPatch, \
		UGModelEMapPack& mapPack, UGbool bShellOnly=FALSE);

	//! \brief 获取距离
	UGfloat GetLODDistance() const;
	void SetLODDistance(UGfloat dbLODDis);

	//! \brief 获取对象引用
	//! \brief pGeode 有可能被替换
	void AddGeode(UGModelGeode *& pGeode);

	//! \brief 释放Geode
	void ReleaseGeode(UGModelGeode * pGeode);

	//! \brief 获取个数
	 UGint GetGeodeCount() const;
	 
	 //! \brief 获取
	 UGModelGeode* GetGeode(UGint nIndex);

	std::vector<UGModelGeode *>& GetGeodes();

	//! \brief 包围球
	void SetBoudingSphere(UGVector3d vecCenter, UGdouble dRadius );

	//! \brief 包围盒
	void SetBoundingBox(const UGBoundingBox& bbox);

	//! \brief 包围球
	UGBoundingSphere GetBoudingSphere();

	//! \brief 包围盒
	UGBoundingBox GetBoundingBox();

	//! \brief 计算包围球
	void ComputeBoundingSphere();

	//! \brief 实时计算准确的脚点
	UGBoundingBox ComputeBoudingBox();

	//! \brief 作用于各Geode的矩阵
	void ReMultiplyMatrix(const UGMatrix4d& m);

	//! \brief 获取当前层的实体对象名称
	void GetEntityNames(std::vector<UGString>& vecNames, UGModelEntity::METype metype);

	void GetSkeletons(std::map<UGString, UGModelSkeletonPtr>& mapEntity);
	void GetMaterials(std::map<UGString, UGModelMaterialPtr>& mapEntity);
	void GetTextures(std::map<UGString, UGModelTexturePtr>& mapEntity);	

	UGModelSkeletonPtr GetSkeleton(const UGString strName);
	UGModelMaterialPtr GetMaterial(const UGString strName);
	UGModelTexturePtr GetTexture(const UGString strName);

	//! \brief 名字是否已经存在
	UGbool IsNameExisted(const UGString strName);

	//! \brief 用数据包更新模型
	UGbool Update(UGModelEMapPack &mapPack);

	//! \brief 实体名字更新，慎用。要求实体未加载，对象Update后，数据集刷新调用
	//! \brief pEMSrc有刚刚Update的信息
	UGbool UpdateShell(UGModelEM* pEMSrc);

	//! \brief 替换实体名字，慎用。数据集追加时使用
	void ReplaceEntityName(std::map<UGString, UGString>& mapNames);

	//! \brief 写入
	UGbool Save(UGStream& stream);

	//! \brief 加载
	UGbool Load(UGStream& stream);

	//! \brief 获取存储空间大小
	UGint GetShellDataSize();

	//! \brief 从加载EM填充vector
	UGbool LoadEntities(UGModelEM* pEM,UGModelEntity::METype mtype = UGModelEntity::etUnknown);

	//! \brief 卸载实体数据
	//! \param mtype [in] 卸载的数据类型
	void UnloadEntities(UGModelEntity::METype mtype = UGModelEntity::etUnknown);

	//! \brief 卸载实体对象
	void ReleaseEntity(const UGString strName, UGModelEntity::METype mtype);

	//! \brief 返回Entity是否Load过
	UGbool IsEntityLoaded();

	//! \brief 添加子节点，更精细层(LODNum-1)
	UGint AddChild(UGModelPagedPatch* pPatch);

	//! \brief 获取子节点
	UGModelPagedPatch* GetChild(UGint nIndex);

	//! \brief 获取所有子节点
	std::vector<UGModelPagedPatch*>& GetChildren();

	//! \brief 获取子节点个数
	UGint GetChildCount();

	//! \brief 父节点
	void SetParentPatch(UGModelPagedPatch* pPatch);

	//! \brief 父节点，更粗糙层(LODNum+1)
	UGModelPagedPatch* GetParentPatch();
	
	//! \brief 索引号
	UGint GetIndex();

	//! \brief 所属LOD
	void SetPagedLOD(UGModelPagedLOD* pPageLOD);

	//! \brief 所属LOD
	UGModelPagedLOD* GetPagedLOD();

	//! \brief 获得渲染使用的ID
	UGint GetRenderID();
	
	//! \brief 移除所有Geode指针，不删除指针内容
	void RemoveAllGeodes();

	//! \brief Patch对应的文件名，不存储
	UGString GetFileName();
	void SetFileName(UGString strName);

	//! \brief ChildPatch所在的文件名
	UGString GetChildTileName();

protected:
	//! \brief 移除所有子节点
	void RemoveChildren();

	//! \brief 移除指定子节点
	void RemoveChildren(UGModelPagedPatch* pChildPatch);

	//! \brief 索引号
	void SetIndex(UGint nIndex);

	//! \brief 构造父子节点关系，UGModelNode::LoadShells专用
	void LoadPatchTree(UGModelNode* pNode);

	//! \brief 慎用，仅Model的Adjust使用
	void ReplaceChild(UGModelPagedPatch* pPatch);

	//! \brief 慎用，仅Reset使用
	void ReplaceGeode(UGint nIndex, UGModelGeode* pGeode);

	//! \brief 骨架的实例化删除
	//! \brief strSkeleton 骨架名称
	UGbool RemoveSkeleton(const UGString strSkeleton);

	//! \brief 设置/获取父 索引号,ModelNode MakeShell使用
	void SetParentIndex(const UGint nPatchParentIndex);
	UGint GetParentIndex();
	//! \brief 设置/获取子 索引号,ModelNode MakeShell使用
	void SetChildrenIndex(const std::vector<UGint>& vecChildrenIndex);
	std::vector<UGint>& GetChildrenIndex();

private:
	//! \brief 距离范围
	UGfloat m_dbLODDis;

	//! \brief 所属LOD层
	UGModelPagedLOD * m_pPagedLOD;

	//! \brief 在当前LOD层Patches中的索引号
	UGint m_nIndex;

	//! \brief 父节点
	UGModelPagedPatch* m_pPatchParent;
	//! \brief 父节点Index，仅加载使用
	UGint m_nPatchParentIndex;

	//! \brief 子节点
	std::vector<UGModelPagedPatch*> m_vecChildren;
	//! \brief 子节点Index，仅加载使用
	std::vector<UGint> m_vecChildrenIndex;

	//! \brief 数据包 
	std::vector<UGModelGeode *> m_vecGeode;

	//! \brief 包围球
	UGBoundingSphere m_BoundingSphere;

	//! \brief 包围盒
	UGBoundingBox m_BoundingBox;

	//! \brief Entity是否Load过
	UGbool m_bEntityLoaded;

	//! \brief 渲染使用的ID
	UGint m_nRenderID;

	//! \brief Patch对应的文件名，不存储
	UGString m_strFileName;
};


class BASE3D_API UGModelPagedLOD
{
	friend class UGModelNode;
	friend class UGModelNodeLODBuilder;

protected:
	UGModelPagedLOD();

	UGModelPagedLOD(UGint nLOD);

	~UGModelPagedLOD();

public:
	//! \brief 完全拷贝.ModelNode operator = 使用
	void MakeFrom(UGModelPagedLOD *pPagedLOD);

	//! \brief 拷贝
	void MakeFrom(UGModelPagedLOD *pPagedLOD, \
		UGModelEMapPack& mapPack, UGbool bShellOnly=FALSE);

	//! \brief 切换范围模式
	void SetRangeMode(UGRangeMode nMode);

	//! \brief 获得切换范围模式
	UGRangeMode GetRangeMode();

	//! \brief 得到Patch，不存在则创建
	UGModelPagedPatch* CreatePagedPatch(UGfloat fDistance);

	//! \brief 添加一个已有的Patch，并设置好Index
	void AddPagedPatch(UGModelPagedPatch* pPagedPatch);

	//! \brief 释放
	UGbool ReleasePagedPatch(UGModelPagedPatch *pPatch);

	//! \brief 获取当前层的实体对象名称
	void GetEntityNames(std::vector<UGString>& vecNames,UGModelEntity::METype metype);

	void GetSkeletons(std::map<UGString, UGModelSkeletonPtr>& mapEntity);
	void GetMaterials(std::map<UGString, UGModelMaterialPtr>& mapEntity);
	void GetTextures(std::map<UGString, UGModelTexturePtr>& mapEntity);	

	UGModelSkeletonPtr GetSkeleton(const UGString strName);
	UGModelMaterialPtr GetMaterial(const UGString strName);
	UGModelTexturePtr GetTexture(const UGString strName);

	//! \brief 名字是否已经存在
	UGbool IsNameExisted(const UGString strName);

	//! \brief 用数据包更新模型
	UGbool Update(UGModelEMapPack &mapPack);

	//! \brief 实体名字更新，慎用。要求实体未加载，对象Update后，数据集刷新调用
	//! \brief pEMSrc有刚刚Update的信息
	UGbool UpdateShell(UGModelEM* pEMSrc);

	//! \brief 替换实体名字，慎用。数据集追加时使用
	void ReplaceEntityName(std::map<UGString, UGString>& mapNames);

	//! \brief 获得所有的距离范围
	void GetLODRanges(std::vector<UGfloat>& vecDis);

	//! \brief 得到Patches
	UGint GetPagedPatchCount();

	//! \brief 根据索引号得到Patch，不会创建新的Patch
	UGModelPagedPatch* GetPagedPatch(UGint nIndex);

	//! \brief 获取Patches
	std::vector<UGModelPagedPatch*>& GetPagedPatchesRef();

	//! \brief 获取距离
	UGfloat GetLODDistance(UGint nPatchIdex=-1) const;

	//! \brief 获取所有Patch的Geodes
	std::vector<UGModelGeode *> GetGeodes();

	//! \brief 获取存储空间大小
	UGint GetShellDataSize();

	//! \brief 写入
	UGbool Save(UGStream& stream);

	//! \brief 加载
	UGbool Load(UGStream& stream);

	//! \brief 从加载UGModelEM填充vector
	UGbool LoadEntities(UGModelEM* pEM, 
		UGModelEntity::METype mtype =UGModelEntity::etUnknown, UGint nPatchIndex = -1);

	//! \brief 卸载实体数据
	//! \param mtype [in] 卸载的数据类型
	void UnloadEntities(UGModelEntity::METype mtype = UGModelEntity::etUnknown);

	//! \brief 卸载实体对象
	void ReleaseEntity(const UGString strName, UGModelEntity::METype mtype);

	//! \brief 包围球
	void SetBoudingSphere(UGVector3d vecCenter, UGdouble dRadius );

	//! \brief 包围球
	UGBoundingSphere GetBoudingSphere();

	//! \brief 计算包围球
	void ComputeBoundingSphere();

	//! \brief 作用于各Geode的矩阵
	void ReMultiplyMatrix(const UGMatrix4d& m);

	//! \brief 获取层号
	UGint GetLODNum() const;

	//! \brief 返回指定LOD层的数据是否已经加载了，-1表示最精细层
	UGbool IsEntityLoaded(UGint nPatchIndex);

protected:
	void SetLODNum(UGint nLODNum);

private:
	//! \brief 切换范围模式
	UGRangeMode m_nRangeMode;

	//! \brief LOD 标识号
	UGint m_nLODNum;

	//! \brief 包围球
	UGBoundingSphere m_BoundingSphere;

	//! \brief 本层的PageLODs
	std::vector<UGModelPagedPatch*> m_vecPagedPatches;
};

}
