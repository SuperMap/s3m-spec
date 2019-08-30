/*! \file	 UGAutoPtr.h
 *  \brief	 半智能指针
 *  \author	 ugc team
 *  \attention 
 *  Copyright(c) 2000-2010 SuperMap Software Co., Ltd.<br>
 *  All Rights Reserved
 *  \version $Id: UGAutoPtr.h,v 1.7 2009/11/05 09:36:46 siyh Exp $
 */

#if !defined(AFX_UGAUTOPTR_H__8844E5E9_172A_4931_913C_5B1322A01112__INCLUDED_)
#define AFX_UGAUTOPTR_H__8844E5E9_172A_4931_913C_5B1322A01112__INCLUDED_

#include "Stream/ugdefs.h"

namespace UGC {

//! \brief 半智能指针
template<typename T>
class UGEXPORT UGAutoPtr
{
private:
	//! \brief 禁止拷贝构造函数和赋值函数
	UGAutoPtr(const UGAutoPtr& autoPtr);
	//! \brief 。
	void operator=(const UGAutoPtr& autoPtr);

public:
	//! \brief 构造函数
	UGAutoPtr() {
		m_pData = NULL;
		m_bAutoDelete = FALSE;
	}

	//! \brief 析构函数
	~UGAutoPtr() {
		Release();
	}

public:
	//! \brief 设置数据指针
	//! \remarks 使用时必须根据实际情况绝对是否让智能指针删除内部保存的指针
	void SetData(const T* pData, UGbool bAutoDelete) {
		Release(); // 先Release一下，以支持循环使用
		m_pData = pData;
		m_bAutoDelete = bAutoDelete;
	}
	
	//! \brief 。
	const T* GetData() const
	{
	  //!  \brief Toolkit
		return m_pData;
	}
	
	//! \brief 自动转换函数
	//operator const T*() const {
	//	return m_pData;
	//}

	//const T* operator->() {
	//	return m_pData;
	//}

	//! \brief 。
	const T& operator[](UGint nIndex) {
		return m_pData[nIndex];
	}

	//! \brief 。
	void Release(){
	  //!  \brief Toolkit
		if (m_bAutoDelete) {
			T* pData = (T*)m_pData;
			delete []pData;
		}
	  //!  \brief Toolkit
		m_pData = NULL;
	  //!  \brief Toolkit
		m_bAutoDelete = FALSE;
	}
		
private:
	const T* m_pData;
	UGbool m_bAutoDelete;
};

}

#endif 


