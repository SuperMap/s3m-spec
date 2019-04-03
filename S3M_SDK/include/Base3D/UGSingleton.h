//////////////////////////////////////////////////////////////////////////
//                    _   _   ____   ____                    
//   Project         | | | | / ___) / ___)                   
//                   | | | || | __ | |                       
//                   | |_| || |_\ || \___                    
//                    \____/ \____| \ ___)     6.0           
//                                                           
//!  \file headerfile.h
//!  \brief 单件类。
//!  \details 完成对单件类的封装。
//!  \author zhangshuai。
//!  \attention 
//!   Copyright (c) 1996-2010 SuperMap Software Co., Ltd. <br> 
//!   All Rights Reserved.
//!  \version 6.0
//////////////////////////////////////////////////////////////////////////


#if !defined(AFX_UGSINGLETON_H__E89D0670_EB32_4782_8964_7F60F1421BCF__INCLUDED_)
#define AFX_UGSINGLETON_H__E89D0670_EB32_4782_8964_7F60F1421BCF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Base3D/UGPrerequisites3D.h"
# pragma warning(disable:4661)
namespace UGC
{
template<typename T>
//! \brief 设计模式中的单件类。
class UGSingleton  
{ 
protected:
	//! \brief 静态保护成员，保证只初始化一次。
	static T* ms_pSingleton;
public:
	//! \brief 构造函数。
	UGSingleton()	
	{
		if(!ms_pSingleton)
		{
#if defined(_MSC_VER) && _MSC_VER < 1200

			UGint nOffSet = (UGint)(T*)1 - (UGint)(UGSingleton<T>*)(T*)1;
			ms_pSingleton = (T*)((UGint)this + nOffSet);
#else

			ms_pSingleton = static_cast<T*>(this);
#endif
		}
	}
	//! \brief 析构函数。
	~UGSingleton()
	{
		if(ms_pSingleton != NULL)
		{
			ms_pSingleton = NULL;
		}
	}
	//! \brief 得到静态成员。
	static T& GetSingleton()
	{
		UGASSERT(ms_pSingleton);	
		return(*ms_pSingleton);
		
	}
	//! \brief 得到静态成员指针。
	static T* GetSingletonPtr()
	{
		UGASSERT(ms_pSingleton);
		return ms_pSingleton;
	}
};
}
#endif // !defined(AFX_UGSINGLETON_H__E89D0670_EB32_4782_8964_7F60F1421BCF__INCLUDED_)

