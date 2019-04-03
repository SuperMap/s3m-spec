//////////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)                
//                                                           
//!  \file OgdcFeature.h
//!  \brief OgdcFeature 的接口定义。
//!  \details Ogdc 特征要素类。
//!  \author duxiaomin。
//!  \attention 
//!   Copyright(c) 2007-2012  SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////

#ifndef OGDCFEATURE_H
#define OGDCFEATURE_H

#include "Base/OgdcVariant.h"
#include "Element/OgdcFieldInfo.h"

#if !defined SYMBIAN60 && !defined OS_ANDROID
#include "Element/OgdcElement.h"
#endif

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace OGDC
{

class OgdcDatasetVector;

//! \brief Ogdc 特征要素类。
#ifdef SYMBIAN60
class ELEMENT_APIC OgdcFeature
#else
class ELEMENT_API OgdcFeature
#endif
{
public:
#ifdef SYMBIAN60
	struct ELEMENT_APIC OgdcFieldDefine 
#else
	struct ELEMENT_API OgdcFieldDefine
#endif
	{
		//! \brief 字段名称。
		OgdcString m_strName;
		
		//! \brief 字段类型。
		OgdcFieldInfo::FieldType m_nType;
		
		//! \brief 字段长度。
		OgdcInt m_nSize;

		//! \brief 是否是必填字段。
		OgdcBool m_bRequired;

		//! \brief 默认构造函数
		OgdcFieldDefine();
		//! \brief 重载=
		const OgdcFieldDefine& operator =(const OgdcFieldDefine& fieldDefine);
	};

public:
	//!  \brief 特征要素默认构造函数
	ELEMENT_APIF OgdcFeature();
	//!  \brief 特征要素默认析构函数
	virtual ELEMENT_APIF ~OgdcFeature();

public:
	//! \brief 获取几何对象ID。
	virtual ELEMENT_APIF OgdcInt GetID() const = 0;	
	
	//! \brief 设置几何对象ID。
	//! \param nID 几何对象ID[in]。
	virtual ELEMENT_APIF void SetID(OgdcInt nID) = 0;	

#if !defined SYMBIAN60 && !defined OS_ANDROID && !defined IOS
	//! \brief 获取几何对象。
	//! \return 是否取几何对象成功，成功返回获取到的几何对象，失败返回NULL。
	virtual ELEMENT_APIF OgdcElement* GetElement() = 0;	
	
	//! \brief 设置当前特征要素的几何对象。
	//! \param pElement 几何对象指针[in]。
	virtual ELEMENT_APIF void SetElement(OgdcElement* pElement) = 0;
#endif

	//! \brief 根据字段序号获取字段值。
	//! \param nIndex 字段序号[in]。
	//! \param varValue 字段值[out]。
	virtual ELEMENT_APIF OgdcBool GetValue(OgdcInt nIndex, OgdcVariant& varValue) = 0;

	//! \brief 根据字段名获取字段值。
	//! \param strFieldName 字段名[in]。
	//! \param varValue 字段值[out]。
	virtual ELEMENT_APIF OgdcBool GetValue(const OgdcString& strFieldName, OgdcVariant& varValue) = 0;	

	//! \brief 根据字段序号设置字段值。
	//! \param nIndex 字段序号[in]。
	//! \param varValue 字段值 [in]。
	//! \return 是否设置成功，成功返回TRUE，失败返回FALSE。
	virtual ELEMENT_APIF OgdcBool SetValue(OgdcInt nIndex, const OgdcVariant& varValue) = 0;    

	//! \brief 根据字段名设置字段值。
	//! \param strFieldName 字段名[in]。
	//! \param varValue 字段值 [in]。
	//! \return 是否设置成功，成功返回TRUE，失败返回FALSE。
	virtual ELEMENT_APIF OgdcBool SetValue(const OgdcString& strFieldName, const OgdcVariant& varValue) = 0;		

public:
	//! \brief 设置字段信息。
	//! \param fieldInfos 字段信息集合[in]。
	virtual ELEMENT_APIF void SetFieldInfos(const OgdcFieldInfos& fieldInfos);	
	//! \brief 获取字符集
	virtual ELEMENT_APIF OgdcString::Charset GetCharset() const;
	//! \brief 设置字符集
	virtual ELEMENT_APIF void SetCharset(OgdcString::Charset nCharset);

public:
	//! \brief 特征要素所在矢量数据集表中字段列表。
	OgdcArray<OgdcFieldDefine> m_fieldDefines;
};

}
#endif // !defined(OGDCFEATURE_H)

