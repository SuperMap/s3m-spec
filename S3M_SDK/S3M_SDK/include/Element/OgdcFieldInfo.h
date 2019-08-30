//////////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)               
//                                                           
//!  \file OgdcFieldInfo.h
//!  \brief OgdcFieldInfo 的接口定义。
//!  \details Ogdc矢量数据集字段描述信息定义。
//!  \attention 
//!   Copyright(c) 2007-2012  SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////
#ifndef OGDCFIELDINFO_H
#define OGDCFIELDINFO_H

#include "Base/OgdcString.h"
#include "Base/OgdcArray.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace OGDC
{

//! \brief Ogdc矢量数据集字段描述信息。
class ELEMENT_API OgdcFieldInfo  
{
public:
	//! \brief 字段类型
	enum FieldType
	{		
		//! \brief 无效的字段类型。
		UnKnown	= 0,
		//! \brief 布尔值，单字节，TRUE,FALSE。
		Boolean = 1,
		//! \brief 无符号单字李，0-255。 
		Byte = 2,
		//! \brief 短整型，2字节。
		INT16 = 3,
		//! \brief 长整型，4字节。
		INT32 = 4,
		//! \brief 长整型，8字节。
		INT64 = 16, 
		//! \brief 单精度浮点型，4字节。
		Float = 6,
		//! \brief 双精度浮点型，8字节。
		Double = 7,
		//! \brief 日期型，年、月、日，不带时间。
		Date = 8,
		//! \brief 固定长度二进制型，需指定长度。
		Binary = 9,
		//! \brief 不定长字符串型。
		Text = 10,
		//! \brief 不定长二进制类型。
		LongBinary = 11,
		//! \brief 定长字符串型，需指定长度。
		Char = 18, 
		//! \brief 时间型，小时、分、秒，不带日期。
		Time = 22,
		//! \brief 时间戳型，年、月、日、小时、分、秒。
		TimeStamp = 23,
		//! \brief 宽字节不定长字符串类型。
		NText =127,
		//! \brief 几何数据类型。
		Geometry = 128,
		//用作Pg中的Jsonb字段类型
		JsonB =129
	};
	
	//! \brief 特殊系统字段标识。
	enum FieldSign  
	{
		signNone	= 0, 
		//! \brief 网络数据集的节点ID，默认是SmNodeID字段。
		signNodeID	= 1, 
		//! \brief 网络数据集起点字段, 默认是SmFNode。
		signFNode	= 2, 
		//! \brief 网络数据集终点字段, 默认是SmTNode。
		signTNode	= 3, 
		//! \brief 网络数据集边的ID字段。
		signEdgeID	= 4, 
		//! \brief 路线数据集的RouteID字段。
		signRouteID	= 5, 		
		//! \brief 对象ID字段。
		signID		= 11, 			
		//! \brief 几何对象字段。
		signGeometry= 12, 	
		//! \brief 用户自定义字段标识起始值。
		signStart	= 50 
	};

public:
	//! \brief Ogdc矢量数据集字段描述信息构造函数
	OgdcFieldInfo();
    //! \brief Ogdc矢量数据集字段描述信息构造函数
	OgdcFieldInfo(const OgdcFieldInfo& fieldInfo);
	
	OgdcFieldInfo(const OgdcString& strFieldName,const OgdcString& strForeignName,FieldType nType,OgdcInt nSize,
		const OgdcString& strFormat,const OgdcString& strDomain,OgdcBool bUpdateable,OgdcBool bRequired,
		const OgdcString& strDefault,FieldSign nSign);
    //! \brief Ogdc矢量数据集字段描述信息析构函数
	~OgdcFieldInfo();
public:
	//! \brief 重载 = 
	const OgdcFieldInfo& operator =(const OgdcFieldInfo& fieldInfo);
    //! \brief 重载 ==
	OgdcBool operator ==(const OgdcFieldInfo& fieldInfo);
    //! \brief 重载 != 
	OgdcBool operator !=(const OgdcFieldInfo& fieldInfo);
public:	
	//! \brief  判断字段是否可更新。
	//! \return 是否可以更新，可以返回TRUE，不可以返回FALSE。
	OgdcBool CanUpdate();	
	
	//! \brief 判断是否是系统字段
	//! \return 是否是系统字段，是返回TRUE，不是返回FALSE。
	OgdcBool IsSystemField();
	
	//! \brief 判断是否是几何对象字段
	//! \return 是否是几何对象字段，是返回TRUE，不是返回FALSE。
	OgdcBool IsGeoField();
	
	//! \brief 判断是否包含索引
	//! \return 是否包含索引，是返回TRUE，不是返回FALSE。
	OgdcBool IsContainIndex();
	//! \brief 判断字段是否为UUID
	OgdcBool IsUUIDField();

public:
	//! \brief 设置默认值
	void SetDefaultValue(const OgdcString& strDefaultValue);

	//! \brief 读取默认值
	//! \return 默认值，字符串格式
	OgdcString & GetDefaultValue();
	

public:	
	//! \brief 是否是系统UUID字段
	OgdcBool m_bUUID;

	//! \brief 是否是系统字段。
	OgdcBool m_bSystem;
	
	//! \brief 是否是必填字段。
	OgdcBool m_bRequired;
	
	//! \brief 字段长度是否允许零长度。
	OgdcBool m_bAllowZeroLength;

	//! \brief 刻度
	OgdcInt m_nScale;
	
	//! \brief 精度
	OgdcInt m_nPrecision;
	
	//! \brief 字段长度。
	OgdcInt m_nSize;
	
	//! \brief 字段属性。
	OgdcInt m_nAttributes;
	
	//! \brief 字段序号位置。
	OgdcInt m_nOrdinalPosition;
	//! \brief 比较序列
	OgdcInt m_nCollatingOrder;

	//! \brief 字段类型。
	FieldType m_nType;
	//! \brief 字段标识。
	FieldSign  m_nFieldSign;
	
	//! \brief 字段名称。
	OgdcString m_strName;
	
	//! \brief 字段的外键名。
	OgdcString m_strForeignName;
	
	//! \brief 源字段名
	OgdcString m_strSourceField;
	
	//! \brief 源表名
	OgdcString m_strSourceTable;
	
	//! \brief 验证规则
	OgdcString m_strValidationRule;
	
	//! \brief 验证条件
	OgdcString m_strValidationText;
	
private:
	//! \brief 字段默认值
	OgdcString m_strDefaultValue;
	
public:
	//! \brief 格式
	OgdcString m_strFormat;
	
	//! \brief 限制信息
	OgdcString m_strDomain;
	
	//! \brief 是否可编辑
	OgdcBool m_bUpdatable;
	
	//! \brief 索引表名称
    OgdcString m_strIndexTab;
};

//! \brief 矢量数据集字段描述信息集合类。
//! \remarks 矢量数据集字段描述信息的数组。
class ELEMENT_API OgdcFieldInfos :public OgdcArray<OgdcFieldInfo> 
{
public:
	//! \brief 默认构造函数
	OgdcFieldInfos();
	//! \brief 默认析构函数
	~OgdcFieldInfos();
public:
	//! \brief 向字段信集数组中添加字段。
	//! \param strName 字段名称[in]。
	//! \param nType 字段类型[in]。
	//! \param nSize 字段字节长度[in]。
	//! \param nAttributes 属性[in]。
	//! \param bRequired 是否必须赋值，默认为否[in]。
	//! \param bAllowZeroLength 字段长度是否可以为0，默认为可以[in]。
	//! \param strDefaultValue 默认值字符串，默认为空[in]。
	//! \param strIndexTabName 索引表名字符串，默认为空[in]。
	//! \return 是否添加成功，成功返回TRUE，不成功返回FALSE。
	//! \attention 字段名称的前两个字符不能使用“Sm”或者其任何大小写形式，
	//! 否则本函数拒绝加入并返回FALSE。
	OgdcBool AddField(
		const OgdcString& strName,
		OgdcFieldInfo::FieldType nType,
		OgdcInt lSize,
		OgdcInt lAttributes = 0,
		OgdcBool bRequired = FALSE,
		OgdcBool bAllowZeroLength = TRUE,
		OgdcString strDefaultValue = _U(""),
		OgdcString strIndexTabName = _U(""));	
	
	//! \brief 获取字段信集数组中指定标记的字段名称。。
	//! \param nFieldSign 字段标记[in]。
	//! \return 字段名称，如字段信集数组中是没有此字段，则返回空字符串。
	OgdcString GetSignField(OgdcFieldInfo::FieldSign nFieldSign);

	//! \brief 获取字段信集数组中指定标记的字段名称。
	//! \param strFieldName 字段名称[out]。
	//! \param nFieldSign 字段标记[in]。
	//! \return 字段信集数组中是否有此字段，有则返回TRUE，没有返回FALSE。
	OgdcBool GetSignField(OgdcString& strFieldName, OgdcFieldInfo::FieldSign nFieldSign);

	//! \brief 		调整字段名
	OgdcBool AdjustFieldNames();
};

}
#endif // !defined(OGDCFIELDINFO_H)

