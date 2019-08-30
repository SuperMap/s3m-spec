//////////////////////////////////////////////////////////////////////////
//                    _   _   ____   ____                    
//   Project         | | | | / ___) / ___)                   
//                   | | | || | __ | |                       
//                   | |_| || |_\ || \___                    
//                    \____/ \____| \ ___)     6.0           
//                                                           
//!  \file UGSharedPtr.h
//!  \brief Class 
//!  \details 文件详细信息。
//!  \author zhangshuai
//!  \attention 
//!   Copyright (c) 1996-2010 SuperMap Software Co., Ltd. <br> 
//!   All Rights Reserved.
//!  \version 6.0
//////////////////////////////////////////////////////////////////////////


#if !defined(AFX_UGSHAREDPTR_H__E5567E39_00CB_4E7D_9FF6_835268B2D58B__INCLUDED_)
#define AFX_UGSHAREDPTR_H__E5567E39_00CB_4E7D_9FF6_835268B2D58B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Base3D/UGPrerequisites3D.h"
namespace UGC
{
//! \brief 智能指针模版类
template<class T> 
class UGSharedPtr 
{
public:
	//! \brief 构造函数
	UGSharedPtr() : m_pRep(0), m_pUseCount(0)
	{
	}
	
	//! \brief 模型构造函数
	template< class Y>
	explicit UGSharedPtr(Y* rep) : m_pRep(rep), m_pUseCount(new UGuint(1))
	{
	}

	//! \拷贝构造函数
	UGSharedPtr(const UGSharedPtr& r)
		: m_pRep(0), m_pUseCount(0)
	{
		m_pRep = r.m_pRep;
		m_pUseCount = r.m_pUseCount; 
		if(m_pUseCount)
		{
			++(*m_pUseCount); 
		}
	}
	//! \重载=
	UGSharedPtr& operator=(const UGSharedPtr& r) 
	{
		if (m_pRep == r.m_pRep)
			return *this;

		UGSharedPtr<T> tmp(r);
		Swap(tmp);
		return *this;
	}

	
// 	//! \brief 模版拷贝构造
// 	template< class Y>
// 	UGSharedPtr(const UGSharedPtr<Y>& r)
// 		: m_pRep(0), m_pUseCount(0)
// 	{
// 		m_pRep = r.GetPointer();
// 		m_pUseCount = r.UseCountPointer();
// 		if(m_pUseCount)
// 		{
// 			++(*m_pUseCount);
// 		}
// 	}
// 
// 	//! \brief 重载=
// 	template< class Y>
// 	UGSharedPtr& operator=(const UGSharedPtr<Y>& r) 
// 	{
// 		if (m_pRep == r.m_pRep)
// 			return *this;
// 
// 		UGSharedPtr<T> tmp(r);
// 		Swap(tmp);
// 		return *this;
// 	}

	//! \brief 析构
	virtual ~UGSharedPtr() 
	{
		Release();
	}

	//! \brief 重载*
	inline T& operator*() const 
	{
		UGASSERT(m_pRep); 
		return *m_pRep;
	}
	//! \brief 重载->
	inline T* operator->() const
	{
		UGASSERT(m_pRep); 
		return m_pRep;
	}

	//! \brief 获取
	inline T* Get() const 
	{
		return m_pRep; 
	}

	
	void Bind(T* rep)
	{
		UGASSERT(!m_pRep && !m_pUseCount);
		m_pUseCount = new UGuint(1);
		m_pRep = rep;
	}

	//! \brief 是否是唯一
	inline UGbool Unique() const
	{ 
		UGASSERT(m_pUseCount);
		return *m_pUseCount == 1;
	}

	//! \brief 引用的次数
	inline UGuint UseCount() const 
	{
		UGASSERT(m_pUseCount);
		return *m_pUseCount;
	}

	//! \brief 引用计数指针
	inline UGuint* UseCountPointer() const 
	{ 
		return m_pUseCount;
	}

	//! \brief 引用对象指针
	inline T* GetPointer() const 
	{ 
		return m_pRep; 
	}

	//! \brief 引用对象是否为空
	inline UGbool IsNull() const 
	{ 
		return m_pRep == 0;
	}

	//! \释放当前引用的对象
	inline void SetNull()
	{ 
		if (m_pRep)
		{
			Release();
			m_pRep = 0;
			m_pUseCount = 0;
		}
	}

protected:

	//! \释放当前引用的对象
	inline void Release()
	{
		UGbool bDestroyThis = false;

		if (m_pUseCount)
		{
			if (--(*m_pUseCount) == 0) 
			{
				bDestroyThis = true;
			}
		}
		if (bDestroyThis)
		{
			Destroy();
		}
	}
	//! \从内存中删除当前对象
	virtual void Destroy()
	{
		if (m_pRep)
		{
			delete m_pRep;
			m_pRep = NULL;
		}
	
		if (m_pUseCount)
		{
			delete m_pUseCount;
			m_pUseCount = NULL;
		}		
	}

	virtual void Swap(UGSharedPtr<T> &other) 
	{
		std::swap(m_pRep, other.m_pRep);
		std::swap(m_pUseCount, other.m_pUseCount);
	}

protected:

	//! \brief 当引用的对象指针
	T* m_pRep;

	//! \brief 当前对象的引用计数指针
	UGuint* m_pUseCount;
};
}
#endif // !defined(AFX_UGSHAREDPTR_H__E5567E39_00CB_4E7D_9FF6_835268B2D58B__INCLUDED_)

