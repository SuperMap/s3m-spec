//////////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)               
//                                                           
//!  \file OgdcDict.h
//!  \brief 字典类 
//!  \details  内部采用STL的map实现
//!  \attention 
//!   Copyright (c) 2007-2012 SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OGDCDICT_H__EA6AD17F_452A_4C4B_9FA0_F79418967EB8__INCLUDED_)
#define AFX_OGDCDICT_H__EA6AD17F_452A_4C4B_9FA0_F79418967EB8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Base/ogdcdefs.h"

namespace OGDC {

#define OgdcLess std::less
#define OgdcGreater std::greater

//! \brief 字典类。
//! 类似于MFC中的CMap,内部采用stl的map实现。
template<typename Key, typename T, class Compare = std::less<Key> >
class OGDCEXPORT OgdcDict 
{
public:
	//! \brief 用来指示元素在字典中位置的值
	typedef typename std::map<Key,T,Compare>::const_iterator POSITION;
	
public:
	//! \brief 默认构造函数。
	OgdcDict()	
	{

	}

	//! \brief 得到元素个数。
	//! \return 返回元素个数。
	OgdcInt GetCount() const	
	{
		return (OgdcInt)m_dict.size();
	}

	//! \brief 判断是否为空。
	//! \return 空返回true;非空返回false。
	OgdcBool IsEmpty() const	
	{
		return m_dict.empty();
	}

	//! \brief 从另一个字典拷贝。
	//! \param src 用于拷贝的字典[in]。
	void Copy(const OgdcDict<Key,T,Compare>& src)	
	{
		*this=src;
	}

	//! \brief 查找指定rkey的value元素。
	//! \param rkey 要查找的key[in]。
	//! \param rValue 得到的元素[out]。
	//! \return 查找成功返回true;不成功返回false。
	OgdcBool Lookup(const Key& rkey,T& rValue) const	
	{
		POSITION it=m_dict.find(rkey);		
		if(it!=m_dict.end())
		{
			rValue = it->second;
			return TRUE;
		}
		return FALSE;
	}

	//! jifang 添加于08/18/2005.
	//! \brief 通过rkey直接查找到元素的指针。
	//! \param rkey 要查找的key值[in]。
	//! \return 找到则返回指向UGDict内部的元素的指针, 不是拷贝;若没查到, 则返回空指针。
	const T* Lookup(const Key &rkey) const	
	{
		POSITION it=m_dict.find(rkey);		
		if(it!=m_dict.end())
		{
			return (const T*)(&it->second);
		}
		return NULL;
	}	

	//! \brief 给指定的rkey设置指定的元素。
	//! \param rkey 指定的键值[in]。
	//! \param newValue 指定的元素[in]。
	void SetAt(const Key& rkey,const T& newValue)	
	{
		operator[](rkey)=newValue;
	}
#ifndef SYMBIAN60
	//有这个函数SYMBIAN下  调用OgdcDict，会编译不过
	//! \brief 通过指定的rkey得到对应元素的引用。
	//! \param rkey 指定的rkey值[in]。
	//! \return 返回对应元素的引用。
	const T& operator[](const Key& rkey) const	
	{
		return m_dict.operator [](rkey);
	}
#endif

	//! \brief 通过指定的rkey得到对应元素的引用。
	//! \param rkey 指定的key值[in]。
	//! \return 返回对应元素的引用。
	T& operator[](const Key& rkey) 
	{
		return m_dict.operator [](rkey);
	}

	//! \brief 移除rkey值对应的元素。
	//! \param rkey 要移除的key值[in]。
	//! \return 成功返回true;失败返回false。
	OgdcBool RemoveKey(const Key& rkey)	
	{
		return m_dict.erase(rkey)>0;
	}

	//! \brief 移除所有元素
	void RemoveAll()
	{
		m_dict.clear();
	}

	//! \brief 得到开始位置。
	//! \return 返回开始位置。
	//! \remarks 得到开始位置后,再通过GetNextAssoc函数就可以一个接着一个得到字典中所有的元素
	//! \attention 注意使用IsEOF函数判断不要超过字典的范围
	POSITION GetStartPosition() const	
	{
		return m_dict.begin();
	}

	//! \brief 判断传入的位置是否是字典尾。
	//! \param pos 要判断的位置[in]。
	//! \return 如果已到字典尾则发挥true;否则返回false。
	OgdcBool IsEOF(POSITION pos) const	
	{
		return pos==m_dict.end();
	}

	//! \brief 得到指定pos的rkey和元素,并把pos移动到下一个位置
	//! \param rNextPosition 指定的位置[in]。
	//! \param rKey 得到的key[out]。
	//! \param rValue 得到的元素[out]。
	void GetNextAssoc(POSITION& rNextPosition,Key& rKey,T& rValue) const	
	{
		OGDCASSERT(!IsEOF(rNextPosition));		
		rKey = rNextPosition->first;
		rValue = rNextPosition->second;
		++rNextPosition;
	}

private:
	std::map<Key,T,Compare> m_dict;
};

}

#endif // !defined(AFX_OGDCDICT_H__EA6AD17F_452A_4C4B_9FA0_F79418967EB8__INCLUDED_)

