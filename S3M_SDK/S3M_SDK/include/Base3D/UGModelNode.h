#pragma once

#include "Base3D/UGSharedPtr.h"
#include "Base3D/UGMesh.h"
#include "Base3D/UGRenderOperationGroup.h"

#include "Base3D/UGModelNodeEntities.h"
#include "Base3D/UGModelNodeShells.h"
#include "Base3D/UGModelEM.h"

namespace UGC
{
class UGModelPagedLOD;
class UGModelNode;

class BASE3D_API UGModelNode
{
	friend class UGEngineToolkit;
	friend class UGModelCheck3D;
	friend class UGModelProcess;
	friend class UGModelNodeTools;

public:
	//! \brief 构造函数
	UGModelNode();

	~UGModelNode();

public:
	UGModelNode& operator = (UGModelNode& other);

	//{{ MakeFrom 的几个方法都是对壳儿（名字）的操作 ////////////////////////////////
	//! \brief 通过已有模型构建，EM在外部指定
	//! \brief modelNode[in]
	//! \brief mapDes[in] 外部实体追加后的实体对照表
	void MakeFrom(UGModelNode& modelNode, \
		UGModelEMapPack& mapDes, UGbool bShellOnly=FALSE);

	//! \brief 通过 UGModelEMapPack 构建，全部加入精细层
	void MakeFrom(UGModelEMapPack& mapPackDes);

	//! \brief 
	void MakeFrom(UGVertexDataPackage* pVertexDataPackage,\
		UGArray<UGIndexPackage*> arrIndexPackage, \
		UGMatrix4d matGeode=UGMatrix4d::IDENTITY);

	//! \brief 
	void MakeFrom(UGVertexDataPackageExact* pVertexDataPackage,\
		UGArray<UGIndexPackage*> arrIndexPackage, \
		UGMatrix4d matGeode=UGMatrix4d::IDENTITY);

	//! \brief 构造模型对象。pSkeletonSrc 会被释放掉，外部不能再使用
	//! \brief 骨架上会默认新建一个材质对象
	void MakeFrom(UGModelSkeleton* pSkeletonSrc,UGMatrix4d matGeode=UGMatrix4d::IDENTITY);

	//! \brief 从pGeodeSrc 构造Geode
	void MakeFrom(UGModelGeode* pGeodeSrc, UGModelEMapPack& mapPackDes);

	//! \brief 通过 pModelNodeSrc的骨架ID构建
	void MakeFrom(UGModelNode* pModelNodeSrc,UGCSkeletonID& id);

	//! \brief 通过已有模型构建空架子：不包含Geode中的对象名
	void MakeShellFrom(UGModelNode& modelNode);

	//}} MakeFrom 的几个方法都是对壳儿（名字）的操作 ////////////////////////////////

	//! \brief 按Patch追加。对象合并使用
	void AppendByPatch(UGModelNode* pModelNode, UGbool bIgnoreLOD);

	UGbool IsEmpty(const UGint iLOD=-1);

	//! \brief 创建精细层Patch
	UGModelPagedPatch* CreatePatch();

	//! \brief 添加 精细层Patch
	void AddPatch(UGModelPagedPatch* pPatch);

	//! \brief 释放精细层Patch
	UGbool ReleasePagedPatch(UGModelPagedPatch *pPatch);

	//! \brief 清空数据
	void Clear(UGbool bRelease = TRUE);

	//! \brief 得到LOD
	UGModelPagedLOD* GetPagedLOD(UGint nLODNum);

	UGModelPagedLOD* AddPagedLOD();

	//! \brief 获得所有的距离范围
	std::vector<UGfloat>& GetLODRanges();

	//! \brief 获取LOD切换比例系数
	void GetLODScales(std::vector<UGfloat>& vecLODScales);

	//! \brief 最精细层
	void AddGeode(UGModelGeode* pGeode, UGint nPatchIndex);

	//! \brief 获取LOD层的Patch
	//! \param nLodNum LOD层级，-1指精细层，>0指LOD层，如果存在的话
	std::vector<UGModelPagedPatch*>& GetDataPatches(UGint nLodNum = -1);

	//! \brief 最精细层：没有ChildPatch的所有Patch
	//! \brief ModelNode允许某些层缺Patch
	void GetRefinedPatches(std::vector<UGModelPagedPatch*>& vecPatches);

	//! \brief 最精细层
	UGint GetDataPatcheCount() const;

	//! \brief 最精细层
	UGModelPagedPatch* GetDataPatche(UGint nIndex);

	//! \brief 最精细层
	std::vector<UGModelGeode *> GetGeodes();

	std::vector<UGModelPagedLOD*>& GetPagedLODs();

	UGint GetShellDataSize();

	UGbool SaveShells(UGStream& stream);

	UGbool LoadShells(UGStream& stream);

	//! \brief 包围盒
	void SetBoudingSphere(UGVector3d vecCenter, UGdouble dRadius );

	//! \brief 包围盒
	UGBoundingSphere GetBoudingSphere();

	UGBoundingSphere ComputeBoudingSphere();

	//! \brief 实时计算出准确的脚点
	UGBoundingBox ComputeBoudingBox();

	//! \brief 获取实体对象名称
	//! \brief nLOD=-2-->LOD+Geodes; nLOD=-1-->Geodes; nLOD=0-n -->LOD(i);
	void GetEntityHashCodes(const UGint nLOD,
		std::vector<UGlong>& vecHashCodes, UGbool bHash64, UGModelEntity::METype metype);

	//! \brief 获取实体对象名称
	//! \brief nLOD=-2-->LOD+Geodes; nLOD=-1-->Geodes; nLOD=0-n -->LOD(i);
	void GetEntityNames(UGint nLOD, std::vector<UGString>& vecNames, UGModelEntity::METype metype);

	//! \brief 慎用，小心野指针
	void Adjust();

	//! \brief 作用于各Geode的矩阵
	void ReMultiplyMatrix(const UGMatrix4d& m);

	//! \brief 返回指定LOD层的数据是否已经加载了，-1表示最精细层
	UGbool IsEntityLoaded(UGint nLOD);

	//! \brief 从ModelNode中提取指定骨架所在的patch
	UGModelPagedPatch* GetPatch(const UGCSkeletonID id);

	//! \brief 获取顶点和三角面个数
	UGbool GetObjectCount(const UGint nLOD, UGint& nVertex, UGint& nTriangle);

	//! \brief 供遍历骨架使用
	UGint GetSkeletonCount(UGint iLod);

	//! \brief iLod层的第iSkeleton个骨架对应的底层ID
	UGCSkeletonID MakeUGCSkeletonID(UGint iLod, UGint iSkeleton);
	//! \brief 根据UGC的骨架ID获取在当前LOD层是第几个骨架
	UGint MakeUGCSkeletonIndex(UGCSkeletonID ugc_id);

	//! \brief 获取根节点Patch
	std::vector<UGModelPagedPatch*> GetRootPatches() const;

	//! \brief 获取总的Tile个数
	UGint GetTileCount() const;

	//////////////////////////////////////////////////////////////////////////

	//! \brief 从加载UGModelEM填充vector
	//! \brief 仅精细层：nLOD=-1; 第n层：nLOD=n; 全部加载nLOD=-2
	UGbool LoadEntities(UGModelEM* pEM, UGint nLOD = -1,UGModelEntity::METype mtype = UGModelEntity::etUnknown);

	//! \brief 卸载实体数据
	//! \param nLOD [in] 卸载的数据LOD层级
	//! \param mtype [in] 卸载的数据类型
	void UnloadEntities(UGint nLOD, UGModelEntity::METype mtype = UGModelEntity::etUnknown);

	//! \brief 卸载实体对象
	void ReleaseEntity(const UGString strName, UGModelEntity::METype mtype);

	//! \brief 取实体
	UGModelMaterialPtr GetMaterial(const UGString strName);
	UGModelTexturePtr GetTexture(const UGString strName);
	UGModelSkeletonPtr GetSkeleton(const UGString strName);

	//! \brief 名字是否已经存在
	UGbool IsNameExisted(const UGString strName);

	//! \brief 替换实体名字，慎用。数据集追加时使用
	void ReplaceEntityName(std::map<UGString, UGString>& mapNames);

	//! \brief 用数据包更新模型
	UGbool Update(UGModelEMapPack &mapPack);

	//! \brief 实体名字更新，慎用。要求实体未加载，对象Update后，数据集刷新调用
	//! \brief pEMSrc有刚刚Update的信息
	UGbool UpdateShell(UGModelEM* pEMSrc);

	//! \brief 给一个可用的名字
	UGString GetUnoccupiedName(UGString strName);

	//! \brief 取出不重复的材质指针
	void GetMapMaterial3D(std::map<UGString, UGModelMaterialPtr>& mapMaterial3D);
	//! \brief 取出不重复的材质指针
	void GetMapTextureData(std::map<UGString, UGModelTexturePtr>& mapTexutreData);
	//! \brief 取出不重复的骨架指针
	void GetMapSkeleton(std::map<UGString, UGModelSkeletonPtr>& mapSkeleton);

	//! \brief 获取数据包
	void GetEMapPack(UGModelEMapPack &mapPack);

	//! \brief 取出特定LOD层的不重复的材质指针
	//! \param nLOD [in] LOD层号，-1为最精细层。
	void GetMapMaterial3DByLOD(UGint nLOD, std::map<UGString, UGModelMaterialPtr>& mapMaterial3D);
	//! \brief 取出特定LOD层的不重复的材质指针
	//! \param nLOD [in] LOD层号，-1为最精细层。
	void GetMapTextureDataByLOD(UGint nLOD, std::map<UGString, UGModelTexturePtr>& mapTexutreData);
	//! \brief 取出特定LOD层的不重复的骨架指针
	//! \param nLOD [in] LOD层号，-1为最精细层。
	void GetMapSkeletonByLOD(UGint nLOD, std::map<UGString, UGModelSkeletonPtr>& mapSkeleton);

	//! \brief 单独构建LOD，根据简化率对所有精细模型创建默认LOD
	//! \brief arrRatio[in]	各层简化率：简化为原来的多少，即剩下多少
	void CreateLODModel(UGlong pGraphics, UGArray<UGdouble>& arrRatio);

	//! \brief 根据简化率对所有精细模型创建默认LOD
	//! \brief arrRatio[in]		各层简化率：简化为原来的多少，即剩下多少
	//! \brief mapInstance[in]	数据集层面的对象实例化标记
	//! \brief mapInstanceLODEntity[in/out]	数据集层面实例化对象的各层中所有实体对象名称
	void CreateLODModel(UGlong pGraphics, UGArray<UGdouble>& arrRatio, \
		std::map<UGString, UGint>& mapInstance, \
		std::map<UGString, std::vector<std::vector<UGString> > > & mapInstanceLODEntity);

	//! \brief 移除LODs
	void RemoveLODModel();

	//! \brief 简化几何网格
	static void SimplificationGeoModel(UGVertexDataPackage* pVertexDataPackage, UGArray<UGIndexPackage*>& arrIndexPackage, UGdouble dPercent);

	//! \brief 从ModelNode中提取指定的骨架
	UGbool GetSkeleton(const UGCSkeletonID id, 
		UGModelSkeleton*& pSkeleton, UGMatrix4d& matGeode);

	//! \brief 移除指定的骨架
	UGbool RemoveSkeleton(UGCSkeletonID id);

	//! \brief 骨架的实例化删除
	//! \brief strSkeleton 骨架名称
	UGbool RemoveSkeleton(const UGString strSkeleton);

	//! \brief 获取父层骨架，依据命名规则[仅通过BuildLOD接口创建的数据适用]
	//! \brief strSkeleton[in]
	UGbool GetParentSkeleton(const UGString strSkeleton, 
		UGModelSkeletonPtr& pSkeletonP, UGMatrix4d& matGeodeP);

protected:

	void ReplaceSkeleton(UGModelSkeleton* pSkeleton, UGArray<UGModelSkeletonPtr>& arrSkeletons);

	//! \brief 构建父子关系：据Patch中的父子Index值
	void MakeTree();

private:
	void SortPagedLODs();
	static UGbool SortPagedLOD(const UGModelPagedLOD *p1,const UGModelPagedLOD *p2);

	void ReplaceSkeleton(UGModelPagedLOD* pPagedLOD, 
		UGModelSkeleton* pSkeleton, UGArray<UGModelSkeletonPtr>& arrSkeletons);
	void ReplaceSkeleton(UGModelPagedPatch* pPagedPatch,
		UGModelSkeleton* pSkeleton, UGArray<UGModelSkeletonPtr>& arrSkeletons);
	void ReplaceSkeleton(UGModelGeode* pGeode,
		UGModelSkeleton* pSkeleton, UGArray<UGModelSkeletonPtr>& arrSkeletons);

private:
	//! \brief 切换范围
	std::vector<UGfloat> m_vecDistanceList;

	//! \brief LOD 层
	std::vector<UGModelPagedLOD*> m_vecPagedLODs;

	//! \brief 叶子节点 ,精细层，所属LOD=NULL
	std::vector<UGModelPagedPatch *> m_vecDataPatches;

	//! \brief 包围球
	UGBoundingSphere m_BoundingSphere;

	//! \brief 最精细层的Entity是否Load过
	UGbool m_bEntityLoaded;
};

}
