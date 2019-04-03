#pragma once

#include "Base3D/UGModelNodeEntities.h"
#include "Base3D/UGSharedPtr.h"
#include "Base3D/UGMatrix4d.h"
#include "Base3D/UGTextureData.h"
#include "Base3D/UGMaterial3D.h"
#include "Base3D/UGRenderOperationGroup.h"
#include "Toolkit/UGLogFile.h"
#include "Toolkit/UGHashCode.h"

namespace UGC
{
//! \brief 发现hash重复时使用的后缀；为提取原始名字
#define MODELENTITYHASHRETAG	_U("_[R.H]_") 

//! \brief 编辑模式
enum EntityEditModel
{
	EEM_Add		= 1,
	EEM_Update	= 2,
	EEM_Delete	= 3
};

template<class TEntity, class TEntityPtr>
class BASE3D_API UGModelEntityMap : public std::map<UGString, TEntityPtr>
{
public:
	UGModelEntityMap()
	{

	}

	~UGModelEntityMap()
	{
		ReleaseEntities();
	}

	UGModelEntityMap& operator = (UGModelEntityMap& other)
	{
		typename std::map<UGString, TEntityPtr>::iterator it;
		for (it=other.begin(); it != other.end(); it++)
		{
			if(!it->second.IsNull())
			{
				TEntity* pEntity = new TEntity(*it->second.Get());
				(*this)[it->first] = TEntityPtr(pEntity);
			}
			else
			{
				(*this)[it->first] = TEntityPtr();
			}		
		}
		return *this;
	}

	void ReleaseEntities()
	{
		//clear自己就行了
		this->clear();
	}

	TEntityPtr CreateEntity(const UGString strName)
	{
		if(!GetEntity(strName).IsNull())
		{
			UGERROR2(-100, _U("Error UGModelEntityMap::CreateEntity"));
			UGASSERT(FALSE);
			return TEntityPtr();
		}
		TEntity* pEntity = new TEntity(strName);
		(*this)[pEntity->m_strName] = TEntityPtr(pEntity);
		return (*this)[pEntity->m_strName];
	}
	
	TEntityPtr GetEntity(const UGString strName, const UGbool bTagName=TRUE)
	{
		if(bTagName)
		{
			typename std::map<UGString, TEntityPtr>::iterator it = \
				this->find(strName);
			if(it != this->end())
			{
				return it->second;
			}
		}
		else
		{
			typename std::map<UGString, TEntityPtr>::iterator it = \
				this->begin();
			for (; it != this->end(); it++)
			{
				if(!it->second.IsNull() && 
					it->second->m_strName.CompareNoCase(strName) == 0)
				{
					return it->second;
				}
			}
		}
		return TEntityPtr();
	}

	TEntityPtr AddEntity(TEntity* pEntity)
	{
		if(pEntity == NULL)
		{
			UGERROR2(-100, _U("Error UGModelEntityMap::AddEntity"));
			UGASSERT(FALSE);
			return TEntityPtr();
		}
		UGString strName = pEntity->GetName();
		if(this->find(strName) != this->end())
		{
			UGASSERT(FALSE);
			UGERROR2(-100, _U("Error UGModelEntityMap::AddEntity"));
			return TEntityPtr();
		}	
		(*this)[strName] = TEntityPtr(pEntity);
		return (*this)[strName];
	}

	//添加引用
	UGbool AddEntity(TEntityPtr pEntity)
	{
		if(pEntity.IsNull())
		{
			UGERROR2(-100, _U("Error UGModelEntityMap::AddEntity"));
			UGASSERT(FALSE);
			return FALSE;
		}
		UGString strName = pEntity->GetName();
		(*this)[strName] = pEntity;
		return TRUE;
	}

	//模型Load时使用
	TEntityPtr SetEntity(TEntityPtr pEntity)
	{
		if(pEntity.IsNull())
		{
			UGERROR2(-100, _U("Error UGModelEntityMap::AddEntity"));
			UGASSERT(FALSE);
			return TEntityPtr();
		}
		UGString strName = pEntity->GetName();
		if(this->find(strName) == this->end())
		{
			UGASSERT(FALSE);
			UGERROR2(-100, _U("Error UGModelEntityMap::AddEntity"));
			return TEntityPtr();
		}	
		(*this)[strName] = pEntity;
		return (*this)[strName];
	}

	//! \brief 名字是否已经存在
	UGbool IsExisted(const UGString strName)
	{
		return this->find(strName) != this->end();
	}

	//! \brief 获取所有实体的个数
	UGint GetEntityCount()
	{
		return this->size();
	}

	//! \brief 添加空对象
	void AddEntityShell(const UGString strName)
	{
		if(!GetEntity(strName).IsNull())
		{
			//UGModelPagedPatch::AddGeode(UGModelGeode *& pGeode)中，可能存在 pGeode 中的对象与已有的共用情况，此时在UGModelGeode::AddMapPack(UGModelEMapPack& mapPack)中先加壳，再Load实体会掉进来（数据导入）
// 			UGERROR2(-100, _U("Error UGModelEMapPack::AddEntityShell"));
// 			UGASSERT(FALSE);
			return;
		}
		(*this)[strName] = TEntityPtr();
	}

	void LoadEntities(UGModelEntityMap& mapPack)
	{
		UGString strName;
		typename std::map<UGString, TEntityPtr>::iterator it = this->begin();
		for (; it != this->end(); it++)
		{
			strName = it->first;
			TEntityPtr pEntitySrc = mapPack.GetEntity(strName);
			if(it->second.IsNull() && !pEntitySrc.IsNull())
			{
				(*this)[strName] = pEntitySrc;
			}
		}
	}
	//仅释放内存(不占用智能指针)，但名字保留（渲染使用）
	void UnLoadEntities()
	{
		UGString strName;
		typename std::map<UGString, TEntityPtr>::iterator it = this->begin();
		for (; it != this->end(); it++)
		{
			strName = it->first;
			(*this)[strName] = TEntityPtr();
		}
	}

	//! \brief 返回实体名字对照表
	void GetEntityMapName(std::map<UGString, UGString>& mapNames)
	{
		UGString strDes;
		typename std::map<UGString, TEntityPtr>::iterator it = this->begin();
		for (; it != this->end(); it++)
		{	
			strDes = it->second.IsNull() ? it->first : it->second->m_strName;
			mapNames[it->first] = strDes;
		}
	}

	//! \brief 替换实体名字，慎用。要求实体未加载，数据集追加时使用
	void ReplaceEntityName(std::map<UGString, UGString>& mapNames)
	{
		//mapNames 是需要替换名字的对照表
		std::vector<UGString> vecAdd;
		std::map<UGString, UGString>::iterator itName;
		typename std::map<UGString, TEntityPtr>::iterator it;
		for (it = this->begin(); it != this->end();)
		{
			UGASSERT(it->second.IsNull());
			itName = mapNames.find(it->first);
			if(itName != mapNames.end())
			{
				vecAdd.push_back(itName->second);
				this->erase(it++);
			}
			else
			{
				++ it;
			}
		}
		for (UGint i = 0; i < vecAdd.size(); i++)
		{
			(*this)[vecAdd[i]] = TEntityPtr();
		}
	}

	std::vector<UGString> GetEntityNames()
	{
		std::vector<UGString> vecResult;
		typename std::map<UGString, TEntityPtr>::iterator it;
		for (it=this->begin(); it != this->end(); it++)
		{
			vecResult.push_back(it->first);
		}
		return vecResult;
	}

public:
};

typedef UGModelEntityMap<UGModelSkeleton, UGModelSkeletonPtr> UGModelSkeletonMap;
typedef UGModelEntityMap<UGModelMaterial, UGModelMaterialPtr> UGModelMaterialMap;
typedef UGModelEntityMap<UGModelTexture, UGModelTexturePtr> UGModelTextureMap;


template<class TEntity, class TEntityPtr>
class BASE3D_API UGModelEntityPool
{
public:
	UGModelEntityPool(UGbool bHash64 = FALSE)
	{
		m_bHash64 =  bHash64;
	}

	~UGModelEntityPool()
	{
		ReleaseEntities();
	}

public:
	//! \brief 初始化
	void InitDB(std::map<UGString, std::vector<UGString> >& mapEntityAtt,\
		std::map<UGString, std::vector<UGVector3d> >& mapBounds)
	{
		UGString strName;
		std::map<UGString, std::vector<UGString> >::iterator it;
		
		for (it=mapEntityAtt.begin(); it != mapEntityAtt.end(); it++)
		{
			strName = it->first;
			UGlong lHash = GetHash(strName);
			m_vecHash.push_back(lHash);
			
			m_mapEntityAtt[strName] = it->second;
			
			SetRename(strName);
		}

		m_mapBounds = mapBounds;
// 		std::map<UGString, std::vector<UGVector3d> >::iterator itBounds;
// 		for (itBounds=mapBounds.begin(); itBounds != mapBounds.end(); itBounds++)
// 		{
// 			strName = it->first;
// 			UGASSERT(it->second.size() == 2);
// 			m_mapBounds[strName] = it->second;
// 		}
	}

	//! \brief 清空
	void Clear()
	{
		m_vecHash.clear();
		m_mapEntityAtt.clear();
		m_mapBounds.clear();
		m_mapRename.clear();
		m_mapEntity.clear();
		if(m_vecEntityCreated.size() > 0 ||
			m_vecEntityUpdated.size() > 0 ||
			m_vecEntityDeleted.size() > 0)
		{
			UGERROR2(-100, _U("Entity UnSaved"));
			UGASSERT(FALSE);
		}
		m_vecEntityCreated.clear();
		m_vecEntityUpdated.clear();
		m_vecEntityDeleted.clear();		
	}

	//! \brief 模板创建
	template<class TModelEM>
	TEntityPtr CreateEntityFrom(TEntity* pEntity, \
		const UGString strNameBase, TModelEM* em)
	{
		UGString strNameDes = strNameBase.IsEmpty() ? \
			pEntity->m_strName : strNameBase;
		strNameDes = em->GetUnoccupiedHashInner(strNameDes);
		UGlong lHash = GetHash(strNameDes);
		m_vecHash.push_back(lHash);

		TEntity* pResult = new TEntity(strNameDes);
		*pResult = *pEntity;
		pResult->m_strName = strNameDes;

		m_mapEntity[strNameDes] = TEntityPtr(pResult);
		m_vecEntityCreated.push_back(strNameDes);

		SetRename(strNameDes);

		RefreshAtt(pResult);

		RefreshBBox(pResult);

		return m_mapEntity[strNameDes];
	}

	//! \brief 移除实体
	UGbool DeleteEntity(const UGString strName)
	{
		UGbool bFound = FALSE;
		typename std::map<UGString, TEntityPtr>::iterator it = m_mapEntity.find(strName);
		if(it != m_mapEntity.end())
		{
			UGASSERT(it->second.UseCount() == 1);
			m_mapEntity.erase(it);
		}

		UGlong lHash = GetHash(strName);
		std::vector<UGlong>::iterator itHash = 
			std::find(m_vecHash.begin(), m_vecHash.end(), lHash);
		UGASSERT(itHash != m_vecHash.end());
		if(itHash != m_vecHash.end())
		{
			m_vecHash.erase(itHash);
			bFound = TRUE;
		}
	
		//m_mapEntityAtt：一定有
		std::map<UGString, std::vector<UGString> >::iterator itAtt = m_mapEntityAtt.find(strName);
		UGASSERT(itAtt != m_mapEntityAtt.end() || m_bHash64);
		if(itAtt != m_mapEntityAtt.end())
		{
			m_mapEntityAtt.erase(itAtt);
			UGASSERT(bFound || m_bHash64);
		}

		//骨架才有
		std::map<UGString, std::vector<UGVector3d> >::iterator itBBox = m_mapBounds.find(strName);
		if(itBBox != m_mapBounds.end())
		{
			m_mapBounds.erase(itBBox);
		}

		std::map<UGString, UGString>::iterator itRename = \
			m_mapRename.find(GetBaseName(strName));
		if(itRename != m_mapRename.end())
		{
			m_mapRename.erase(itRename);
		}

		//m_vecEntityCreated
		UGbool bCreated = FALSE;
		std::vector<UGString>::iterator itCreated = 
			std::find(m_vecEntityCreated.begin(), m_vecEntityCreated.end(), strName);
		if(itCreated != m_vecEntityCreated.end())
		{
			m_vecEntityCreated.erase(itCreated);
			bCreated = TRUE;
		}
		else//只要不是新创建的，都要从数据库删除
		{
			m_vecEntityDeleted.push_back(strName);
		}

		//m_vecEntityUpdated
		std::vector<UGString>::iterator itUpdated = 
			std::find(m_vecEntityUpdated.begin(), m_vecEntityUpdated.end(), strName);
		if(itUpdated != m_vecEntityUpdated.end())
		{
			m_vecEntityUpdated.erase(itUpdated);
		}
		return bFound;
	}

	//! \brief 从内存获取实体
	TEntityPtr GetLoadedEntity(const UGString strName)
	{
		typename std::map<UGString, TEntityPtr>::iterator it = m_mapEntity.find(strName);
		if(it == m_mapEntity.end())
		{
			return TEntityPtr();
		}
		return it->second;
	}

	//! \brief 从内存获取实体
	std::map<UGString, TEntityPtr>& GetLoadedEntity()
	{
		return m_mapEntity;
	}

	//! \brief 名字是否已存在
	UGbool IsNameExisted(const UGString strEntityName)
	{
		std::map<UGString, std::vector<UGString> >::iterator iter = \
			m_mapEntityAtt.find(strEntityName);
		return iter != m_mapEntityAtt.end();
	};

	//! \brief 是否是可用的名字
	UGbool IsNameAvailable(const UGString strName)
	{
		UGlong lHash = GetHash(strName);
		return  IsNameAvailable(lHash);
	}

	//! \brief 是否是可用的名字(Hash)
	UGbool IsNameAvailable(const UGlong lHash)
	{
		std::vector<UGlong>::iterator iter = \
			std::find(m_vecHash.begin(), m_vecHash.end(), lHash);
		if(iter != m_vecHash.end())
		{
			return FALSE;
		}
		return TRUE;
	}

	//! \brief 是否已经被加载到内存
	UGbool IsEntityLoaded(const UGString strName)
	{
		return m_mapEntity.find(strName) != m_mapEntity.end();
	}
	
	//! \brief 清理内存：清理掉UseCount=1(仅自己使用)的对象	
	void ReleaseEntities()
	{
#ifdef _DEBUG
		//这都要释放了，实体对象的引用次数不为1的话，就有内存泄露
		typename std::map<UGString, TEntityPtr>::iterator it;
		for(it=m_mapEntity.begin(); it != m_mapEntity.end(); it++)
		{
			UGASSERT(it->second.UseCount() == 1);
		}
#endif
		m_mapEntity.clear();
	}

	//! \brief 清理掉管理器中不再使用的数据
	void ReleaseUnuseEntity(std::vector<UGString>& vecNames)
	{
		for (UGint i = 0; i < vecNames.size(); i++)
		{
			typename std::map<UGString, TEntityPtr>::iterator it = m_mapEntity.find(vecNames[i]);
			if (it != m_mapEntity.end() && it->second.UseCount() == 1)
			{
				m_mapEntity.erase(it);
			}
		}
	}

	//! \brief 清理掉管理器中不再使用的数据
	void ReleaseUnuseEntity()
	{
		typename std::map<UGString, TEntityPtr>::iterator it;
		for (it = m_mapEntity.begin(); it != m_mapEntity.end();)
		{
			if(it->second.UseCount() == 1)
			{
				m_mapEntity.erase(it ++);
			}
			else
			{
				++ it;
			}
		}
	}

	//! \brief 对象托管
	//! \return 名字重复返回false
	UGbool DelegateEntity(TEntity* pEntity)
	{
// 		UGASSERT(pEntity != NULL);
// 		if(IsNameExisted(pEntity->m_strName))
// 		{
// 			UGASSERT(FALSE);
// 			return FALSE;
// 		}
// 		UGASSERT(m_mapEntityAtt.size()==0 && m_mapRename.size()==0 &&
// 			m_vecHash.size()==0);

		m_mapEntity[pEntity->m_strName] = TEntityPtr(pEntity);

// 		UGint nHash = UGHashCode::FastStringToHashCode(pEntity->m_strName);
// 		m_vecHash.push_back(nHash);
// 		RefreshAtt(pEntity);

		return TRUE;
	}

	//! \brief 添加修改的实体，最后要给存储
	//! \brief 智能指针，放置外部释放
	UGbool UpdateEntity(TEntityPtr pEntity)
	{
		//如果实体都没有，最后你让我咋存
		UGASSERT(!pEntity.IsNull());
		//另外，池子里都没有，一定是外面搞错了
		if(!IsNameExisted(pEntity->m_strName))
		{
			UGASSERT(FALSE);
			return FALSE;
		}

		typename std::map<UGString, TEntityPtr>::iterator it = m_mapEntity.find(pEntity->m_strName);
		if(it != m_mapEntity.end())
		{
			UGASSERT(it->second.UseCount() == 1);
			m_mapEntity.erase(it);
		}

		m_mapEntity[pEntity->m_strName] = pEntity;

		std::vector<UGString>::iterator itFound = 
			std::find(m_vecEntityUpdated.begin(), m_vecEntityUpdated.end(), pEntity->m_strName);
		if(itFound == m_vecEntityUpdated.end())
		{
			//没找到再加入
			m_vecEntityUpdated.push_back(pEntity->m_strName);
		}

		//属性也可能被改了
		RefreshAtt(pEntity.Get());

		RefreshBBox(pEntity.Get());

		return TRUE;
	}

	//! \brief 同步Att
	void RefreshAtt(TEntity* pEntity)
	{
		m_mapEntityAtt[pEntity->m_strName] = pEntity->GetAtt();
	}

	void RefreshBBox(TEntity* pEntity)
	{
		if(pEntity->HasBoundingBox())
		{
			std::vector<UGVector3d> vecBox;
			UGBoundingBox bbox = pEntity->GetBoundingBox();
			vecBox.push_back(bbox.GetMin());
			vecBox.push_back(bbox.GetMax());
			m_mapBounds[pEntity->GetName()] = vecBox;
		}
	}

	std::vector<UGString> GetAtt(UGString strName)
	{
		std::map<UGString, std::vector<UGString> >::iterator it = 
			m_mapEntityAtt.find(strName);
		if(it == m_mapEntityAtt.end())
		{
			UGERROR2(-100, _U("Error GetAtt"));
			UGASSERT(FALSE);
			return std::vector<UGString>();
		}
		return it->second;
	}

	UGBoundingBox GetBBox(UGString strName)
	{
		std::map<UGString, std::vector<UGVector3d> >::iterator it = 
			m_mapBounds.find(strName);
		if(it == m_mapBounds.end())
		{
			UGERROR2(-100, _U("Error GetBBox"));
			UGASSERT(FALSE);
			return UGBoundingBox();
		}
		UGASSERT(it->second.size() == 2);
		return UGBoundingBox(it->second[0], it->second[1]);
	}

	//////////////////////////////////////////////////////////////////////////
	//! \brief 管理器追加实体：只负责往里面追加对象
	//! \brief mapEntityResult ==> srcName : DesEntity
	template<class TModelEM>
	void AppendEntities(std::map<UGString, TEntityPtr> &mapEntity,\
		std::map<UGString, TEntityPtr> &mapEntityResult, \
		TModelEM* emThis, UGbool bInstance)
	{
		typename std::map<UGString, TEntityPtr>::iterator it;
		UGString strNameSrc;		
		for (it = mapEntity.begin(); it != mapEntity.end(); it++)
		{
			TEntityPtr pEntityDesPtr;

			TEntityPtr pEntitySrc = it->second;			
			strNameSrc = it->first;
			
			//这个才是真正存在的
			UGString strNameSrcRe = GetRename(strNameSrc);

			if(pEntitySrc.IsNull())
			{
				if(!bInstance)//为空，你还不支持实例化？
				{
					UGASSERT(FALSE);
					UGERROR2(-100, _U("Error AppendEntities:EntitySrc NULL"));
					continue;
				}
				else//实例化
				{
					// 一定是已经存在
					std::map<UGString, std::vector<UGString> >::iterator iter = \
						m_mapEntityAtt.find(strNameSrcRe);
					if(iter != m_mapEntityAtt.end())
					{
						typename std::map<UGString, TEntityPtr>::iterator itEntity = \
							m_mapEntity.find(strNameSrcRe);
						if(itEntity != m_mapEntity.end())
						{
							pEntityDesPtr = itEntity->second;
						}
						else
						{
							//内存没有：给个壳儿；真正对应的是strNameSrcRe，so外面要处理一下
							pEntityDesPtr = TEntityPtr();
						}
					}
					else
					{
						UGASSERT(FALSE);
						UGERROR2(-100, _U("Error AppendEntities:EntitySrc NULL"));
					}
				}
			}
			else// ! pEntitySrc.IsNull()
			{
				if(strNameSrc.CompareNoCase(pEntitySrc->m_strName) != 0)
				{
					UGASSERT(FALSE);
					UGERROR2(-100, _U("Error AppendEntities:EntitySrc"));
				}

				if(!bInstance)
				{
					//非实例化的，让m_mapRename重复一下也没关系
					pEntityDesPtr = CreateEntityFrom(pEntitySrc.Get(), strNameSrc, emThis);
				}
				else   //实例化
				{
					//看看是否已经存在
					std::map<UGString, std::vector<UGString> >::iterator iter = \
						m_mapEntityAtt.find(strNameSrcRe);
					if(iter != m_mapEntityAtt.end())
					{
						typename std::map<UGString, TEntityPtr>::iterator itEntity = \
							m_mapEntity.find(strNameSrcRe);
						if(itEntity != m_mapEntity.end())
						{
							pEntityDesPtr = itEntity->second;
						}
						else
						{
							//内存没有，这不是传进来了相同的对象么，用呗
							TEntity* pEntityDes = new TEntity();	
							*pEntityDes = *(pEntitySrc.Get());
							pEntityDes->m_strName = strNameSrcRe;
							pEntityDesPtr = TEntityPtr(pEntityDes);
							//填进去
							m_mapEntity[strNameSrcRe] = pEntityDesPtr;
						}
					}
					else
					{
						pEntityDesPtr = CreateEntityFrom(pEntitySrc.Get(), strNameSrc, emThis);
					}
				}
			}

			mapEntityResult[strNameSrc] = pEntityDesPtr;
		}
	}

	//! \brief 管理器更新实体：存在的更新，不存在的追加
	//! \brief mapEntityResult ==> srcName : DesEntity
	template<class TModelEM>
	void UpdateEntities(std::map<UGString, TEntityPtr> &mapEntity,\
		std::map<UGString, TEntityPtr> &mapEntityResult, \
		TModelEM* emThis)
	{
		typename std::map<UGString, TEntityPtr>::iterator it;
		UGString strNameSrc, strNameDes;		
		for (it = mapEntity.begin(); it != mapEntity.end(); it++)
		{
			TEntityPtr pEntityDes;

			TEntityPtr pEntitySrc = it->second;			
			strNameSrc = it->first;
			
			if(!pEntitySrc.IsNull())
			{
				if(strNameSrc.CompareNoCase(pEntitySrc->m_strName) != 0)
				{
					UGASSERT(FALSE);
					UGERROR2(-100, _U("Error AppendEntities:EntitySrc"));
				}

				//这个才是真正存在的
				UGString strNameSrcRe = GetRename(strNameSrc);				
				if(!IsNameExisted(strNameSrcRe))
				{
					//不存在的
					pEntityDes = CreateEntityFrom(pEntitySrc.Get(), strNameSrc, emThis);
				}
				else//已经存在，需要更新的
				{
					UpdateEntity(pEntitySrc);
					pEntityDes = pEntitySrc;
				}
			}
			//即便pEntitySrc为空，也要留下个名字（投影转换时Update使用，材质不会加载）
			mapEntityResult[strNameSrc] = pEntityDes;
		}
	}
	//////////////////////////////////////////////////////////////////////////

	//! \brief 获取编辑过得对象
	void GetMapEdited(const EntityEditModel eModel, \
		std::vector<TEntity*>& vecEntity)
	{
		std::vector<UGString> vecEntityName;
		switch (eModel)
		{
		case EEM_Add:
			vecEntityName = m_vecEntityCreated;
			break;
		case EEM_Update:
			vecEntityName = m_vecEntityUpdated;
			break;
		default:
			UGASSERT(FALSE);
			break;
		}
		for (UGint i=0; i<vecEntityName.size(); i++)
		{
			vecEntity.push_back(GetLoadedEntity(vecEntityName[i]).Get());
		}
	}

	//! \brief 获取删除的对象
	void GetMapDeleted(std::vector<UGString>& vecEntity)
	{
		vecEntity.insert(vecEntity.end(), \
			m_vecEntityDeleted.begin(), m_vecEntityDeleted.end());
	}

	//! \brief 清空编辑列表
	void ClearEditMap(const EntityEditModel eModel)
	{
		switch (eModel)
		{
		case EEM_Add:
			m_vecEntityCreated.clear();
			break;
		case EEM_Update:
			m_vecEntityUpdated.clear();
			break;
		case EEM_Delete:
			m_vecEntityDeleted.clear();
			break;
		default:
			UGASSERT(FALSE);
			break;
		}
	}

	//! \brief 获取所有的Hash
	void GetLoadedHash(std::vector<UGlong>& vecHash)
	{
		typename std::map<UGString, TEntityPtr>::iterator it;
		for(it=m_mapEntity.begin(); it != m_mapEntity.end(); it++)
		{
			vecHash.push_back(GetHash(it->first));
		}		
	}

	//! \brief 获取实体名
	void GetNames(std::vector<UGString>& vecName)
	{
		std::map<UGString, std::vector<UGString> >::iterator it = \
			m_mapEntityAtt.begin();
		for (; it != m_mapEntityAtt.end(); it++)
		{
			vecName.push_back(it->first);
		}
	}

	//! \brief 获取追加后新的名字
	UGString GetRename(const UGString strName)
	{
		std::map<UGString, UGString>::iterator it = m_mapRename.find(strName);
		if(it != m_mapRename.end())
		{
			return it->second;
		}
		return strName;
	}

private:
	//! \brief 计算Hash值
	UGlong GetHash(UGString strName)
	{
		UGlong lHash =  m_bHash64 ? UGHashCode::StringToHashCode64(strName) :
			UGHashCode::FastStringToHashCode(strName);
		return lHash;
	}

	//! \brief 设置重命名前后的对照表
	void SetRename(const UGString strName)
	{
		UGString strBase = GetBaseName(strName);
		if(strBase.CompareNoCase(strName) != 0)
		{
			m_mapRename[strBase] = strName;//被重命名过，才留下
		}
	}

	//! \brief 获取原始名字
	UGString GetBaseName(const UGString strName)
	{
		UGString strBase = strName;
		UGint nPos = strName.ReverseFind(MODELENTITYHASHRETAG);
		if(nPos > 0)
		{
			strBase = strName.Left(nPos);
		}
		return strBase;
	}

private:
	//! \brief 池中已存在的实体Hash:包含未加载的和内存的
	std::vector<UGlong> m_vecHash;

	//! \brief 是否是 64位hash存储
	UGbool m_bHash64;

	//! \brief 池中已存在的实体名及其属性的对应关系：包含未加载的和内存的
	//! \brief 对于骨架==>骨架名：材质名
	//! \brief 对于材质==>材质名：纹理名
	//! \brief 对于纹理，这个应该为空
	std::map<UGString, std::vector<UGString> > m_mapEntityAtt;

	//! \brief 骨架的包围盒
	std::map<UGString, std::vector<UGVector3d> > m_mapBounds;
	
	//! \brief 内部重命名前后对照表
	std::map<UGString, UGString> m_mapRename;

	//! \brief 内存中的实体
	std::map<UGString, TEntityPtr> m_mapEntity;

	//! \brief 需要存储（即新Create出来的）实体的HashCodes
	std::vector<UGString> m_vecEntityCreated;

	//! \brief 更新过的（修改的）实体名
	std::vector<UGString> m_vecEntityUpdated;

	//! \brief 删除的实体名
	std::vector<UGString> m_vecEntityDeleted;
};

typedef UGModelEntityPool<UGModelSkeleton, UGModelSkeletonPtr> UGModelSkeletonPool;
typedef UGModelEntityPool<UGModelMaterial, UGModelMaterialPtr> UGModelMaterialPool;
typedef UGModelEntityPool<UGModelTexture, UGModelTexturePtr> UGModelTexturePool;

}
