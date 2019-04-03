#pragma once

#include "Base3D/UGSharedPtr.h"
#include "Base3D/UGMatrix4d.h"
#include "Base3D/UGTextureData.h"
#include "Base3D/UGMaterial3D.h"
#include "Base3D/UGRenderOperationGroup.h"
#include "Base3D/UGModelNodeEntities.h"
#include "Base3D/UGModelEntityPool.h"

namespace UGC
{
class UGModelEM;

//! \brief 自己控制，想
//! \brief 实体共用模式，管理器追加实体使用
enum EntityShared
{
	ES_None		= 0x00000000,
	ES_Skeleton		= 0x00000001,
	ES_Texture		= 0x00000002,
	ES_Material		= 0x00000004,
	ES_ALL			= 0x00000001|0x00000002|0x00000004,
};

class BASE3D_API UGModelEMapPack
{
public:
	UGModelEMapPack();

	UGModelEMapPack(UGModelEM* pEM);

	~UGModelEMapPack();

public:
	//! \brief 
	UGModelEMapPack& operator = (UGModelEMapPack& other);

	//! \brief 设置EM
	void SetEM(UGModelEM* pEM);

	//! \brief 获取EM
	UGModelEM* GetEM();

	//! \brief 创建新的实体；重名返回空
	UGModelSkeletonPtr CreateSkeleton(const UGString strName);
	UGModelMaterialPtr CreateMaterial(const UGString strName);
	UGModelTexturePtr CreateTexture(const UGString strName);

	//! \brief 添加
	UGModelSkeletonPtr AddSkeleton(UGModelSkeleton* pEntity);
	UGModelMaterialPtr AddMaterial(UGModelMaterial* pEntity);
	UGModelTexturePtr AddTexture(UGModelTexture* pEntity);
	void AddTexture(std::vector<UGModelTexture*> vecTexture);

	UGbool AddSkeleton(UGModelSkeletonPtr pEntity);
	UGbool AddMaterial(UGModelMaterialPtr pEntity);
	UGbool AddTexture(UGModelTexturePtr pEntity);

	//! \brief 添加空对象
	void AddEntityShell(const UGModelEntity::METype eType, const UGString strName);

	//! \brief 设置,从Em中Load实体使用,必须是智能指针
	UGModelSkeletonPtr SetSkeleton(UGModelSkeletonPtr pEntity);
	UGModelMaterialPtr SetMaterial(UGModelMaterialPtr pEntity);
	UGModelTexturePtr SetTexture(UGModelTexturePtr pEntity);

	//! \brief 加载实体
	//! \brief AddEntityShell 之后都是壳儿
	void LoadEntities(UGModelEMapPack& mapPack);

	//! \brief 获取
	UGModelSkeletonPtr GetSkeleton(const UGString strName, const UGbool bTagName=TRUE);
	UGModelMaterialPtr GetMaterial(const UGString strName, const UGbool bTagName=TRUE);
	UGModelTexturePtr GetTexture(const UGString strName, const UGbool bTagName=TRUE);	

	//! \brief 给一个可用的名字
	UGString GetUnoccupiedName(UGString strName);

	//! \brief 获得实体名称
	std::vector<UGString> GetEntityNames(UGModelEntity::METype metype);

	//! \brief 获取属性
	void GetAtt(const UGString strName, const UGModelEntity::METype eType, \
		std::vector<UGString>& vecAtt);

	//! \brief 返回实体名字
	UGString GetEntityMapName(const UGString strName, const UGModelEntity::METype eType);

	//! \brief 返回实体名字对照表
	void GetEntityMapName(std::map<UGString, UGString>& mapNames);

	//! \brief 包里数据同步，移除掉无用的对象
	void SynEntity();

	//! \brief 骨架删除
	//! \brief strSkeleton 骨架名称
	//! \brief 返回骨架的材质名
	UGString RemoveSkeleton(const UGString strSkeleton);

	//! \brief 从内存移除材质，返回该材质的纹理
	std::vector<UGString> RemoveMaterial(UGString strMaterialName);

	//! \brief 从内存移除纹理对象
	UGbool RemoveTexture(UGString strTextureName);

	//! \brief 清空
	void Clear();

	//! \brief 名字是否已经存在
	UGbool IsExisted(const UGString strName);

	//! \brief 用数据包更新模型;mapPackSrc 里面的对应关系已经是处理好的
	UGbool Update(UGModelEMapPack& mapPackSrc);

	//! \brief 获取所有实体的个数
	UGint GetEntityCount();

	//! \brief 替换实体名字，慎用。要求实体未加载，数据集追加时使用
	void ReplaceEntityName(std::map<UGString, UGString>& mapNames);

	//! \brief 实体名字更新，慎用。要求实体未加载，对象Update后，数据集刷新调用
	//! \brief pEMSrc有刚刚Update的信息
	UGbool UpdateShell(UGModelEM* pEMSrc);

	//! \brief 包内所有骨架的包围盒
	UGBoundingBox GetBBox();

public:
	UGModelSkeletonMap m_mapSkeleton;
	UGModelMaterialMap m_mapMaterial;
	UGModelTextureMap m_mapTexture;

private:
	UGModelEM* m_pEM;
};

class BASE3D_API UGModelEM
{
	friend class UGGeoModelEntitySkeleton;
	friend class UGGeoModelEntityMaterial3D;
	friend class UGGeoModelEntityTexture;
	friend class UGModelNode;
	friend class UGModelGeode;

public:
	UGModelEM(UGbool bHash64 = FALSE);
	~UGModelEM();

public:
	//! \brief 是否已经初始化过了
	UGbool HasInited();

	//! \brief 是否管理的是64位Hash
	UGbool IsHash64();

	//! \brief 初始化对象和属性信息
	void InitEntityInfo(std::map<UGString, std::vector<UGString> >& mapSkeletonAtt,
		std::map<UGString, std::vector<UGString> >& mapMaterialAtt,
		std::map<UGString, std::vector<UGString> >& mapTextureAtt, 
		std::map<UGString, std::vector<UGVector3d> >& mapBBox);

	//! \brief 清空
	void Clear();

	//! \brief 按模板创建新的实体，内部自动重命名
	UGModelSkeletonPtr CreateSkeletonFrom(UGModelSkeleton* pEntity, UGString strNameDes=_U(""));
	UGModelMaterialPtr CreateMaterial3DFrom(UGModelMaterial* pEntity, UGString strNameDes=_U(""));
	UGModelTexturePtr CreateTextureDataFrom(UGModelTexture* pEntity, UGString strNameDes=_U(""));

	//! \brief 添加修改的实体，最后要给存储
	UGbool UpdateEntity(UGModelSkeletonPtr pEntity);

	//! \brief 删除实体
	UGbool DeleteEntity(const UGString& strName, const UGModelEntity::METype eType);

	//! \brief 从内存获取实体
	UGModelSkeletonPtr GetLoadedSkeleton(UGString strName);
	UGModelMaterialPtr GetLoadedMaterial3D(UGString strName);
	UGModelTexturePtr GetLoadedTexture(UGString strName);
	
	std::map<UGString, UGModelSkeletonPtr>& GetLoadedSkeletonRef();
	std::map<UGString, UGModelMaterialPtr>& GetLoadedMaterialRef();
	std::map<UGString, UGModelTexturePtr>& GetLoadedTextureRef();

	//! \brief 获取实体名字
	void GetEntityNames(std::vector<UGString> &vecNames, UGModelEntity::METype eType);

	//! \brief 获取已加载的所有实体Hash
	void GetLoadedHash(std::vector<UGlong>& vecHash); 

	//! \brief 获取实体
	UGbool IsEntityLoaded(UGString strName);

	//! \brief 对strName取Hash，判断是否已经存在，返回一个未占用的
	//! \brief 外部调用（关键字不是谁都能用的）
	UGString GetUnoccupiedHash(UGString strName);

	//! \brief 对strName取Hash，判断是否已经存在
	//! \brief 包含命名规则关键字"_[R.H]_"
	UGString GetUnoccupiedHashInner(UGString strName);

	//! \brief 名字是否已存在
	//! \brief strEntityName[in]
	//! \brief bNameIsOrg[in] 传入的实体名是否是原始名字
	UGbool IsNameExisted(const UGString strEntityName);

	//! \brief 实体是否已存在，同IsNameExisted
	UGbool IsEntityExisted(UGString strEntityName);

	//! \brief 是否是可用的名字
	UGbool IsNameAvailable(UGString strName);

	//! \brief 清理掉UseCount=1(仅EntitiesManager自己使用)的对象	
	void Release();

	//! \brief 清理掉管理器中不再使用的数据
	void ReleaseUnuseEntity(std::vector<UGString> vecNames, UGModelEntity::METype metype);

	//! \brief 清理掉管理器中不再使用的数据
	void ReleaseUnuseEntity(UGModelEntity::METype metype);

	//! \brief 获取属性
	//! \brief strName[in] 传入对象的名字
	//! \brief eType[in]传入对象的类型
	void GetAtt(const UGString strName, const UGModelEntity::METype eType, \
		std::vector<UGString>& vecAtt);

	//! \brief 获取BBOX
	//! \brief strName[in] 传入对象的名字
	UGBoundingBox GetBBox(const UGString strName);

	//! \brief 获取对象及其属性对象名字包，包含自己
	void GetObjAttPack(const UGString strName,\
		const UGModelEntity::METype eType, std::vector<UGString>& vecAtt);

	//////////////////////////////////////////////////////////////////////////
	//! \brief 追加实体：实例化
	//! \brief mapPackResult ==> 是处理结果，外部 EntitySrcName : EntityDes
	//! \brief eh 指定共享模式；共享时出来的 EntityDes 有可能为空
	void AppendEntities(UGModelEMapPack& mapPackSrc,
		UGModelEMapPack& mapPackResult,const EntityShared eh);

	//! \brief 更新实体，不存在的自动创建
	//! \brief mapPackResult ==> 是处理结果，外部 EntitySrcName : EntityDes
	void UpdateEntities(UGModelEMapPack& mapPackSrc, UGModelEMapPack& mapPackResult);

	//! \brief 从骨架中提取所有实体的mapPack
	void GetEntityMap(UGModelSkeleton* pSkeleton, UGModelEMapPack& mapPack);
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	//! \brief 获取编辑过得对象
	void GetMapEdited(const EntityEditModel eModel, const UGModelEntity::METype eType, 
		std::vector<UGModelEntity*>& vecEntity);

	//! \brief 清空编辑列表
	void ClearEditMap(const EntityEditModel eModel);

	//! \brief 获取删除的对象
	void GetMapDeleted(const UGModelEntity::METype eType, std::vector<UGString>& vecEntity);

	//////////////////////////////////////////////////////////////////////////
protected:
	//! \brief 外部实体托管
	//! \return 重复返回false
	UGbool DelegateEntity(UGModelEntity* pEntity);

protected:
	//! \brief 骨架数据
	UGModelSkeletonPool m_SkeletonPool;
	
	//! \brief 材质数据
	UGModelMaterialPool m_MaterialPool;

	//! \brief 纹理数据
	UGModelTexturePool m_TexturePool;

	//! \brief 是否已经初始化
	UGbool m_bInited;

	//! \brief 是否64位Hash
	UGbool m_bHash64;
};

}
