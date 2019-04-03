//!  \file UGFeature.h
//!  \brief UGFeature 的接口定义。
//!  \details UGC 特征要素类。
//!  \author ugc team
//!  \attention 
//!   Copyright (c) 1996-2008 SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0

#ifndef UGFEATURE_H
#define UGFEATURE_H

#include "Element/OgdcFeature.h"
#include "Geometry/UGGeometry.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace UGC
{

//##ModelId=482030510188
typedef OgdcFieldInfo UGFieldInfo;
//##ModelId=482030510196
typedef OgdcFieldInfos UGFieldInfos;
//##ModelId=483BB70803B0
typedef OgdcFeature::OgdcFieldDefine UGFieldDefine;

class UGDatasetVector;

//! \brief UGC 特征要素类。
//! \remarks 备注信息。
//##ModelId=48203050038A
class GEOMETRY_API UGFeature  
{
	friend class UGFeatureOgdc;
public:
	//##ModelId=482030500399
	UGFeature();
	//##ModelId=48203050039A
	virtual ~UGFeature();

	UGFeature(const UGFeature& feature);
	UGFeature& operator=(const UGFeature& feature);

public:
	//! \brief 获取几何对象ID。
	//##ModelId=4820305003A9
	virtual UGint GetID() const;	
	
	//! \brief 设置几何对象ID。
	//! \param nID 几何对象ID[in]。
	//##ModelId=4820305003AB
	virtual void SetID(UGint nID);	

	//! \brief 获取几何对象，外面不能释放。
	//! \return 是否取几何对象成功，成功返回几何对象，失败返回NULL。
	//##ModelId=4820305003B9
	virtual UGGeometry* GetGeometry();	
	
	//! \brief 设置当前特征要素的几何对象
	//! \param pElement 几何对象指针[in]。
	//##ModelId=4820305003BB
	virtual void SetGeometry(UGGeometry* pGeometry);

	//! \brief 获取内部的几何对象的Bounds，。
	//! \return 几何对象的Bounds。
	//! \remarks 如果内部几何对象为空则返回空UGRect2D。
	virtual UGRect2D GetBounds();

	//! \param bRelease设置为TRUE，m_pGeometry由Feature托管，内部负责释放，外面不能释放；设置为False外面负责释放pGeometry
	virtual void SetAutoReleaseGeometry(UGbool bRelease);

	//! \brief 根据字段序号获取字段值。
	//! \param nIndex 字段序号[in]。
	//##ModelId=4820305003C8
	virtual UGbool GetValue(UGint nIndex, UGVariant& varValue) const;

	//! \brief 根据字段名获取字段值。
	//! \param strFieldName 字段名[in]。
	//##ModelId=4820305003CC
	virtual UGbool GetValue(const UGString& strFieldName, UGVariant& varValue) const;	

	//! \brief 根据字段序号设置字段值。
	//! \param nIndex 字段序号[in]。
	//! \param varValue 字段值 [in]。
	//! \return 是否设置成功，成功返回TRUE，失败返回FALSE。
	//##ModelId=4820305003D8
	virtual UGbool SetValue(UGint nIndex, const UGVariant& varValue);    

	//! \brief 根据字段名设置字段值。
	//! \param strFieldName 字段名[in]。
	//! \param varValue 字段值 [in]。
	//! \return 是否设置成功，成功返回TRUE，失败返回FALSE。
	//##ModelId=4820305003DC
	virtual UGbool SetValue(const UGString& strFieldName, const UGVariant& varValue);
	
	//! \brief 已知字段为bool类型，根据字段序号设置字段值
	//! \param nFieldIndex 字段序号[in]。
	//! \param bValue 字段值 [in]。
	//! \return 是否设置成功，成功返回TRUE，失败返回FALSE。
	//##ModelId=482030510002
	virtual UGbool SetBool(UGint nFieldIndex, UGbool bValue = FALSE);
	
	//! \brief 已知字段为bool类型，根据字段名设置字段值
	//! \param strFieldName 字段名[in]。
	//! \param bValue 字段值 [in]。
	//! \return 是否设置成功，成功返回TRUE，失败返回FALSE。
	//##ModelId=482030510006
	virtual UGbool SetBool(const UGString& strFieldName, UGbool bValue = FALSE);

	//! \brief 已知字段为bool类型,根据字段序号获取字段值。
	//! \param nFieldIndex 字段序号[in]。
	//! \return 获取的字段值。
	//##ModelId=482030510012
	virtual UGbool GetBool(UGint nFieldIndex, UGbool& bValue);

	//! \brief 已知字段为bool类型,根据字段名获取字段值。
	//! \param strFieldName 字段名[in]。
	//! \return 获取的字段值。
	//##ModelId=48203051001F
	virtual UGbool GetBool(const UGString& strFieldName, UGbool& bValue);

	//! \brief 已知字段为byte类型，根据字段序号设置字段值
	//! \param nFieldIndex 字段序号[in]。
	//! \param bValue 字段值 [in]。
	//! \return 是否设置成功，成功返回TRUE，失败返回FALSE。
	//##ModelId=482030510022
	virtual UGbool SetByte(UGint nFieldIndex, UGbyte bValue = 0); 

	//! \brief 已知字段为byte类型，根据字段名设置字段值
	//! \param strFieldName 字段名[in]。
	//! \param bValue 字段值 [in]。
	//! \return 是否设置成功，成功返回TRUE，失败返回FALSE。
	//##ModelId=482030510026
	virtual UGbool SetByte(const UGString& strFieldName, UGbyte bValue = 0);
	
	//! \brief 已知字段为byte类型,根据字段序号获取字段值。
	//! \param nFieldIndex 字段序号[in]。	
	//! \return 获取的字段值。
	//##ModelId=482030510031
	virtual UGbool GetByte(UGint nFieldIndex, UGbyte& nValue);
	
	//! \brief 已知字段为byte类型,根据字段名获取字段值。
	//! \param strFieldName 字段名[in]。
	//! \return 获取的字段值。
	//##ModelId=482030510034
	virtual UGbool GetByte(const UGString& strFieldName, UGbyte& nValue);

	//! \brief 已知字段为Short类型，根据字段序号设置字段值
	//! \param nFieldIndex 字段序号[in]。
	//! \param nValue 字段值 [in]。
	//! \return 是否设置成功，成功返回TRUE，失败返回FALSE。
	//##ModelId=48203051003E
	virtual UGbool SetINT16(UGint nFieldIndex, UGshort nValue=0);

	//! \brief 已知字段为Short类型，根据字段名设置字段值
	//! \param strFieldName 字段名[in]。
	//! \param nValue 字段值 [in]。
	//! \return 是否设置成功，成功返回TRUE，失败返回FALSE。
	//##ModelId=482030510042
	virtual UGbool SetINT16(const UGString& strFieldName, UGshort nValue=0);

	//! \brief 已知字段为Short类型,根据字段序号获取字段值。
	//! \param nFieldIndex 字段序号[in]。
	//! \return 获取的字段值。
	//##ModelId=48203051004E
	virtual UGbool GetINT16(UGint nFieldIndex, UGshort& nValue);

	//! \brief 已知字段为Short类型,根据字段名获取字段值。
	//! \param strFieldName 字段名[in]。
	//! \return 获取的字段值。
	//##ModelId=482030510051
	virtual UGbool GetINT16(const UGString& strFieldName, UGshort& nValue);
	
	//! \brief 已知字段为int类型，根据字段序号设置字段值
	//! \param nFieldIndex 字段序号[in]。
	//! \param nValue 字段值 [in]。
	//! \return 是否设置成功，成功返回TRUE，失败返回FALSE。
	//##ModelId=482030510054
	virtual UGbool SetINT32(UGint nFieldIndex, UGint nValue=0);
	
	//! \brief 已知字段为int类型，根据字段名设置字段值
	//! \param strFieldName 字段名[in]。
	//! \param nValue 字段值 [in]。
	//! \return 是否设置成功，成功返回TRUE，失败返回FALSE。
	//##ModelId=48203051005F
	virtual UGbool SetINT32(const UGString& strFieldName, UGint nValue=0);

	//! \brief 已知字段为Int类型,根据字段序号获取字段值。
	//! \param nFieldIndex 字段序号[in]。
	//! \return 获取的字段值。
	//##ModelId=482030510063
	virtual UGbool GetINT32(UGint nFieldIndex, UGint& nValue);
	
	//! \brief 已知字段为Int类型,根据字段名获取字段值。
	//! \param strFieldName 字段名[in]。
	//! \return 获取的字段值。
	//##ModelId=48203051006D
	virtual UGbool GetINT32(const UGString& strFieldName, UGint& nValue);

	//! \brief 已知字段为Long类型，根据字段序号设置字段值
	//! \param nFieldIndex 字段序号[in]。
	//! \param lValue 字段值 [in]。
	//! \return 是否设置成功，成功返回TRUE，失败返回FALSE。
	//##ModelId=482030510070
	virtual UGbool SetINT64(UGint nFieldIndex, UGlong lValue=0);
	
	//! \brief 已知字段为Long类型，根据字段名设置字段值
	//! \param strFieldName 字段名[in]。
	//! \param lValue 字段值 [in]。
	//! \return 是否设置成功，成功返回TRUE，失败返回FALSE。
	//##ModelId=482030510074
	virtual UGbool SetINT64(const UGString& strFieldName, UGlong lValue=0);

	//! \brief 已知字段为Long类型,根据字段序号获取字段值。
	//! \param nFieldIndex 字段序号[in]。
	//! \return 获取的字段值。
	//##ModelId=48203051007E
	virtual UGbool GetINT64(UGint nFieldIndex, UGlong& lValue);
	
	//! \brief 已知字段为Long类型,根据字段名获取字段值。
	//! \param strFieldName 字段名[in]。
	//! \return 获取的字段值。
	//##ModelId=482030510081
	virtual UGbool GetINT64(const UGString& strFieldName, UGlong& lValue);

	//! \brief 已知字段为Float类型，根据字段序号设置字段值
	//! \param nFieldIndex 字段序号[in]。
	//! \param dbalue 字段值 [in]。
	//! \return 是否设置成功，成功返回TRUE，失败返回FALSE。
	//##ModelId=482030510084
	virtual UGbool SetFloat(UGint nFieldIndex, UGfloat dbalue=0);
	
	//! \brief 已知字段为Float类型，根据字段名设置字段值
	//! \param strFieldName 字段名[in]。
	//! \param dbValue 字段值 [in]。
	//! \return 是否设置成功，成功返回TRUE，失败返回FALSE。
	//##ModelId=48203051008E
	virtual UGbool SetFloat(const UGString& strFieldName, UGfloat dbValue=0);

	//! \brief 已知字段为Float类型,根据字段序号获取字段值。
	//! \param nFieldIndex 字段序号[in]。
	//! \return 获取的字段值。
	//##ModelId=482030510092
	virtual UGbool GetFloat(UGint nFieldIndex, UGfloat& dbValue);
	
	//! \brief 已知字段为Float类型,根据字段名获取字段值。
	//! \param strFieldName 字段名[in]。
	//! \return 获取的字段值。
	//##ModelId=482030510095
	virtual UGbool GetFloat(const UGString& strFieldName, UGfloat& dbValue);

	//! \brief 已知字段为Double类型，根据字段序号设置字段值
	//! \param nFieldIndex 字段序号[in]。
	//! \param dbValue 字段值 [in]。
	//! \return 是否设置成功，成功返回TRUE，失败返回FALSE。
	//##ModelId=48203051009E
	virtual UGbool SetDouble(UGint nFieldIndex, UGdouble dbValue=0);
	
	//! \brief 已知字段为Double类型，根据字段名设置字段值
	//! \param strFieldName 字段名[in]。
	//! \param dbValue 字段值 [in]。
	//! \return 是否设置成功，成功返回TRUE，失败返回FALSE。
	//##ModelId=4820305100A2
	virtual UGbool SetDouble(const UGString& strFieldName, UGdouble dbValue=0);

	//! \brief 已知字段为Double类型,根据字段序号获取字段值。
	//! \param nFieldIndex 字段序号[in]。
	//! \return 获取的字段值。
	//##ModelId=4820305100A6
	virtual UGbool GetDouble(UGint nFieldIndex, UGdouble& dbValue);
	
	//! \brief 已知字段为Double类型,根据字段名获取字段值。
	//! \param strFieldName 字段名[in]。
	//! \return 获取的字段值。
	//##ModelId=4820305100AC
	virtual UGbool GetDouble(const UGString& strFieldName, UGdouble& dbValue);

	//! \brief 已知字段为Time类型，根据字段序号设置字段值
	//! \param nFieldIndex 字段序号[in]。
	//! \param tmValue 字段值 [in]。
	//! \return 是否设置成功，成功返回TRUE，失败返回FALSE。
	//##ModelId=4820305100AF
	virtual UGbool SetTime(UGint nFieldIndex, UGTime tmValue);
	
	//! \brief 已知字段为Time类型，根据字段名设置字段值
	//! \param strFieldName 字段名[in]。
	//! \param tmValue 字段值 [in]。
	//! \return 是否设置成功，成功返回TRUE，失败返回FALSE。
	//##ModelId=4820305100B3
	virtual UGbool SetTime(const UGString& strFieldName, UGTime tmValue);

	//! \brief 已知字段为Time类型,根据字段序号获取字段值。
	//! \param nFieldIndex 字段序号[in]。
	//! \return 获取的字段值。
	//##ModelId=4820305100BB
	virtual UGbool GetTime(UGint nFieldIndex, UGTime& tmValue);
	
	//! \brief 已知字段为Time类型,根据字段名获取字段值。
	//! \param strFieldName 字段名[in]。
	//! \return 获取的字段值。
	//##ModelId=4820305100BE
	virtual UGbool GetTime(const UGString& strFieldName, UGTime& tmValue);

	//! \brief 已知字段为Date类型，根据字段序号设置字段值
	//! \param nFieldIndex 字段序号[in]。
	//! \param tmValue 字段值 [in]。
	//! \return 是否设置成功，成功返回TRUE，失败返回FALSE。
	//##ModelId=4820305100C1
	virtual UGbool SetDate(UGint nFieldIndex, UGTime tmValue);
	
	//! \brief 已知字段为Date类型，根据字段名设置字段值
	//! \param strFieldName 字段名[in]。
	//! \param tmValue 字段值 [in]。
	//! \return 是否设置成功，成功返回TRUE，失败返回FALSE。
	//##ModelId=4820305100C5
	virtual UGbool SetDate(const UGString& strFieldName, UGTime tmValue);

	//! \brief 已知字段为Date类型,根据字段序号获取字段值。
	//! \param nFieldIndex 字段序号[in]。
	//! \return 获取的字段值。
	//##ModelId=4820305100CD
	virtual UGbool GetDate(UGint nFieldIndex, UGTime& tmValue);
	
	//! \brief 已知字段为Date类型,根据字段名获取字段值。
	//! \param strFieldName 字段名[in]。
	//! \return 获取的字段值。
	//##ModelId=4820305100D0
	virtual UGbool GetDate(const UGString& strFieldName, UGTime& tmValue);

	//! \brief 已知字段为TimeStamp类型，根据字段序号设置字段值
	//! \param nFieldIndex 字段序号[in]。
	//! \param tmValue 字段值 [in]。
	//! \return 是否设置成功，成功返回TRUE，失败返回FALSE。
	//##ModelId=4820305100D3
	virtual UGbool SetTimeStamp(UGint nFieldIndex, UGTime tmValue);
	
	//! \brief 已知字段为TimeStamp类型，根据字段名设置字段值
	//! \param strFieldName 字段名[in]。
	//! \param tmValue 字段值 [in]。
	//! \return 是否设置成功，成功返回TRUE，失败返回FALSE。
	//##ModelId=4820305100D7
	virtual UGbool SetTimeStamp(const UGString& strFieldName, UGTime tmValue);

	//! \brief 已知字段为TimeStamp类型,根据字段序号获取字段值。
	//! \param nFieldIndex 字段序号[in]。
	//! \return 获取的字段值。
	//##ModelId=4820305100DD
	virtual UGbool GetTimeStamp(UGint nFieldIndex, UGTime& tmValue);
	
	//! \brief 已知字段为TimeStamp类型,根据字段名获取字段值。
	//! \param strFieldName 字段名[in]。
	//! \return 获取的字段值。
	//##ModelId=4820305100E0
	virtual UGbool GetTimeStamp(const UGString& strFieldName, UGTime& tmValue);

	//! \brief 已知字段为Binary类型，根据字段序号设置字段值
	//! \param nFieldIndex 字段序号[in]。
	//! \param pValue 字段值 [in]。
	//! \param nLength pValue的长度 [in]。
	//! \return 是否设置成功，成功返回TRUE，失败返回FALSE。
	//##ModelId=4820305100E3
	virtual UGbool SetBinary(UGint nFieldIndex, UGbyte *pValue, UGint nLength);
	
	//! \brief 已知字段为Binary类型，根据字段名设置字段值
	//! \param strFieldName 字段名[in]。
	//! \param pValue 字段值 [in]。
	//! \param nLength pValue的长度 [in]。
	//! \return 是否设置成功，成功返回TRUE，失败返回FALSE。
	//##ModelId=4820305100EA
	virtual UGbool SetBinary(const UGString& strFieldName, UGbyte *pValue, UGint nLength);

	//! \brief 已知字段为Binary类型,根据字段序号获取字段值。
	//! \param [in] nFieldIndex 字段序号。
	//! \param [out] nLength 二进制字段的长度。
	//! \return 获取的二进制字段指针。
	//##ModelId=4820305100EF
	virtual UGbool GetBinary(UGint nFieldIndex, UGbyte *&pValue, UGint& nLength);
	
	//! \brief 已知字段为Short类型,根据字段名获取字段值。
	//! \param strFieldName 字段名[in]。
	//! \param nLength 二进制字段的长度[out]。
	//! \return 获取的二进制字段指针。
	//##ModelId=4820305100F3
	virtual UGbool GetBinary(const UGString& strFieldName, UGbyte *&pValue, UGint& nLength);

	//! \brief 已知字段为String类型，根据字段序号设置字段值
	//! \param nFieldIndex 字段序号[in]。
	//! \param strValue 字段值 [in]。
	//! \return 是否设置成功，成功返回TRUE，失败返回FALSE。
	//##ModelId=4820305100FA
	virtual UGbool SetString(UGint nFieldIndex, UGString strValue);
	
	//! \brief 已知字段为String类型，根据字段名设置字段值
	//! \param strFieldName 字段名[in]。
	//! \param strValue 字段值 [in]。
	//! \return 是否设置成功，成功返回TRUE，失败返回FALSE。
	//##ModelId=4820305100FE
	virtual UGbool SetString(const UGString& strFieldName, UGString strValue);

	//! \brief 已知字段为String类型,根据字段序号获取字段值。
	//! \param nFieldIndex 字段序号[in]。
	//! \return 获取的字段值。
	//##ModelId=482030510102
	virtual UGbool GetString(UGint nFieldIndex, UGString& strValue);
	
	//! \brief 已知字段为String类型,根据字段名获取字段值。
	//! \param strFieldName 字段名[in]。
	//! \return 获取的字段值。
	//##ModelId=482030510105
	virtual UGbool GetString(const UGString& strFieldName, UGString& strValue);
	
	//! \brief 设置字段信息。
	//! \param fieldInfos 字段信息集合[in]。
	//##ModelId=482030510108
	virtual void SetFieldInfos(const UGFieldInfos& fieldInfos);	

	//! \brief 获取字段信息。
	//! \param fieldInfos 字段信息集合[in]。
	//##ModelId=483BB709011A
	virtual UGArray<UGFieldDefine>* GetFieldDefines();	

	//! \brief 释放数据。
	//##ModelId=48203051010B
	virtual void Release();

	//! \brief 是否自动释放Geometry内存。
	//##ModelId=48203051010B
	virtual UGbool IsAutoReleaseGeometry() const;

	//! \brief 得到字符集。
	//##ModelId=483BB709011C
	virtual UGString::Charset GetCharset() const;		
	
	//! \brief 设置字符集。
	//! \param nCharset [in]。
	//##ModelId=483BB709011E
	virtual void SetCharset(UGString::Charset nCharset);		
	
	//! \brief 由OgdcFeature构造UGFeature。
	//! \param pOFeature [in]。
	//##ModelId=483BB7090128
	void SetOgdcFeature(OgdcFeature* pOFeature);

	//! \brief 获取OgdcFeature指针。
	//##ModelId=483BB709012A
	OgdcFeature* GetOgdcFeature() const;

	// 判断Feature是否相似,即字段结构是否完全一致,包括字段顺序和字段名称大小写,字段长度信息
	UGbool IsLike(UGFeature *pFeature,UGbool bCheckGeometry=TRUE);

	//通过字段名取得字段索引
	UGint GetIndexByName(const UGString& strFieldName);

	//返回字段信息
//	virtual UGFieldInfos* GetFieldInfos();	

public:
	//! \brief 二进制数据。
	//##ModelId=48203051010D
	UGArray<UGbyte*> m_fieldValues;

	//! \brief Feature的标识号，在构造Feature或设置FieldDefines时自动生成,由该Feature的内存地址作为前32位,生成该Feature的系统时间作为后32位
	//! \remark 主要用于在从记录集获取Feature时,如果Feature的字段结构和记录集一致,直接通过字段索引来获取字段值,以提高GetFeature的性能.
	UGulong m_nIdentity;

protected:
    //! \brief 特征要素所在矢量数据集表中的ID。
	//##ModelId=48203051010E
	UGint m_nID;

	//! \brief 特征要素中的几何对象指针。
	//##ModelId=48203051010F
	UGGeometry* m_pGeometry;

	//! \brief SetAutoReleaseGeometry设置
	//! \brief 设置为TRUE，pGeometry由Feature托管，内部负责释放，外面不能释放；
	//! \brief 否则外面负责释放pGeometry
	UGbool m_bAutoReleaseGeometry;

	//! \brief 特征要素所在矢量数据集表中字段列表。
	//##ModelId=482030510119
	UGArray<UGFieldDefine> m_fieldDefines;

	//返回给给组件使用的字段信息
//	UGFieldInfos m_fieldInfos;

	//##ModelId=483BB709012C
	//! \brief 字符集
	OgdcString::Charset m_nCharset;

	//##ModelId=483BB7090139
	//! \brief OGDC对象
	OgdcFeature* m_pOFeature;

	UGMemoryStream m_stream;
};

}
#endif // !defined(UGFEATURE_H)

