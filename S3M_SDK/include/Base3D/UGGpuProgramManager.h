//////////////////////////////////////////////////////////////////////////
//                    _   _   ____   ____                    
//   Project         | | | | / ___) / ___)                   
//                   | | | || | __ | |                       
//                   | |_| || |_\ || \___                    
//                    \____/ \____| \ ___)     6.1           
//                                                           
//!  \file headerfile.h
//!  \brief Gpu程序管理器类
//!  \details 完成对Gpu程序的管理
//!  \author sunyl。
//!  \attention 
//!   Copyright (c) 1996-2012 SuperMap Software Co., Ltd. <br> 
//!   All Rights Reserved.
//!  \version 6.1的
//////////////////////////////////////////////////////////////////////////


#ifndef __GPUPROGRAMMANAGER_H__
#define __GPUPROGRAMMANAGER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Base3D/UGPrerequisites3D.h"
#include "Base3D/UGSingleton.h"
#include "Base3D/UGGpuProgram.h"

namespace UGC
{
	class UGGpuProgram;

	class  BASE3D_API UGGpuProgramManager : public UGSingleton<UGGpuProgramManager>
	{
	public:
		UGGpuProgramManager();
		virtual ~UGGpuProgramManager();

		//! \brief 根据名称获取Gpu程序对象指针
		//! \param strName Gpu程序名称[in]。
		UGGpuProgram* GetGpuProgram(const UGString& strName);

		//! \brief 判断是否存在某一数据
		//! \param strFileName Gpu程序名称[in]。
		UGbool IsExists(const UGString& strName);

		//! \brief 删除指定名称的Gpu程序
		//! \param strName Gpu程序名称[in]。
		void Remove(const UGString& strName);

		//! \brief 删除所用数据
		void RemoveAll();

		//! \brief 创建一个Gpu程序
		//! \param strName Gpu程序名称[in]。
		//! \param strResourceGroup Gpu程序所属于的资源组[in]。
		//! \param strLanguage Gpu程序所用的语言，如“cg”[in]。
		UGGpuProgram* CreateGpuProgram(const UGString& strName, const UGString& strResourceGroup, const UGString& strLanguage);

	public:

		//! \brief 管理材质数据的字典
		//! \param UGString 标识材质的名称
		//! \param UGMaterial3DPtr　材质数据的智能指针
		UGDict<UGString,UGGpuProgram*> m_dictGpuProgram;
	private:

		
	public:
		static UGGpuProgramManager& GetSingleton();
		static UGGpuProgramManager* GetSingletonPtr();	
	};
}
#endif

