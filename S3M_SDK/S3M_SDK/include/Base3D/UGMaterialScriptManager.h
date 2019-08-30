//////////////////////////////////////////////////////////////////////////
//                    _   _   ____   ____                    
//   Project         | | | | / ___) / ___)                   
//                   | | | || | __ | |                       
//                   | |_| || |_\ || \___                    
//                    \____/ \____| \ ___)     6.1           
//                                                           
//!  \file headerfile.h
//!  \brief 材质管理器类
//!  \details 完成对材质的管理
//!  \author sunyl。
//!  \attention 
//!   Copyright (c) 1996-2012 SuperMap Software Co., Ltd. <br> 
//!   All Rights Reserved.
//!  \version 6.1的
//////////////////////////////////////////////////////////////////////////


#ifndef __MATERIAL3DMANAGER_H__
#define __MATERIAL3DMANAGER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Base3D/UGPrerequisites3D.h"
#include "Toolkit/UGSingleton.h"
#include "Base3D/UGMaterial3D.h"
#include "Base3D/UGScriptCompiler.h"
#include "Base3D/UGScriptTranslatorManager.h"

namespace UGC
{
	class UGMaterial3D;

	class  BASE3D_API UGMaterialScriptManager : public UGSingleton<UGMaterialScriptManager>
	{
	public:
		UGMaterialScriptManager();
		virtual ~UGMaterialScriptManager();

		//! \brief 根据名称获取材质对象的智能引用
		//! \param strFileName 材质名称[in]。
		UGMaterial3D* GetMaterial3D(const UGString& strName);

		//! \brief 当某个材质数据被释放时，通知管理器，同时清除记录
		//! \param pModelData 材质数据的指针[in]。
		//void NotifyMaterialManager(UGMaterial3D* pMaterial3D);

		//! \brief 判断是否存在某一数据
		//! \param strFileName 材质名称[in]。
		UGbool IsExists(const UGString& strName);

		//! \brief 删除指定名称的材质数据
		//! \param strFileName 材质名称[in]。
		void Remove(const UGString& strName);

		//! \brief 删除所用数据
		void RemoveAll();

		//! \brief 手动创建一个材质对象
		//! \param strName 材质名称[in]。
		//! \param strGroupName 材质所属于的组的名称[in]。
		UGMaterial3D* CreateManual(const UGString& strName, const UGString& strGroupName = _U(""));

		//! \brief 根据名称获取材质对象的智能引用
		//! \param strName 材质名称[in]。
		UGMaterial3D* GetByName(const UGString& strName);

		//! \brief 加载材质脚本文件
		//! \param strFileName 材质脚本文件路径[in]。
		//! \param strGroupName 材质所属于的组的名称[in]。
		UGMaterial3D* Load(const UGString& strFileName, const UGString& strGroupName = _U(""));

		void inline Lock()
		{
			m_mutex.lock();
		}
		void inline Unlock()
		{
			m_mutex.unlock();
		}

	public:

		//! \brief 管理材质数据的字典
		//! \param UGString 标识材质的名称
		//! \param UGMaterial3DPtr　材质数据的智能指针
		UGDict<UGString,UGMaterial3D*> m_dictMaterial3D;
	private:

		UGScriptCompiler *m_pScriptCompiler;

		UGMutex m_mutex;
	public:
		static UGMaterialScriptManager& GetSingleton();
		static UGMaterialScriptManager* GetSingletonPtr();	
	};
}
#endif

