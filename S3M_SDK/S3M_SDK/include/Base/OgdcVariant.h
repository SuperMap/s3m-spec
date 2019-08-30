//////////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)               
//                                                           
//!  \file OgdcVariant.h
//!  \brief 变体类
//!  \details  
//!  \attention 
//!   Copyright (c) 2007-2012 SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OGDCVARIANT_H__3DD52A73_5F64_43C1_9374_D128DF18117E__INCLUDED_)
#define AFX_OGDCVARIANT_H__3DD52A73_5F64_43C1_9374_D128DF18117E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Base/OgdcTime.h"

namespace OGDC{
//!  \brief 变体类
class BASE_API OgdcVariant  
{
public:
	//! \brief 支持的类型
	enum VarType
	{ 
		//! \brief 未定义的类型
		Null			=	0, 
		//! \brief 单字节类型
		Byte			=	1,
		//! \brief 双字节整型类型
		Short			=	2,
		//! \brief 四字节整型类型
		Integer			=	3,
		//! \brief 八字节整型类型
		Long			=	4,
		//! \brief 四字节浮点类型
		Float			=	5, 
		//! \brief 八字节浮点类型
		Double			=	6,
		//! \brief 时间类型
		Time			=	7,
		//! \brief 二进制类型
		Binary			=	8, 
		//! \brief 字符串
		String			=   9,
		//! \brief 日期类型
		Date			=  10,
		//! \brief 时间戳类型 
		TimeStamp		=  11,
		//! \brief 布尔类型
		Boolean			= 12
	};//为了与UGFieldInfo中的三种日期时间类型区分开，把原来的time类型扩展为time date timestamp，定义参考UGFieldInfo中对应的类型

	struct binaryVal{   
		//! \brief 存储 Binary 指针或者 String指针
		void* pVal;	      
		//! \brief 存储Binary 数据流的大小 以字节为单位。
		OgdcUint nSize;   
	};
	
	//! \brief 变体联合
	union VarValue
	{
		OgdcUchar bVal;
		OgdcShort sVal;
		OgdcInt	iVal;
		OgdcLong  lVal;
		OgdcFloat fVal;
		OgdcDouble dVal;
		OgdcDouble   tmVal; // OgdcTime的内部实现
		struct binaryVal binVal;
	};
	
public:
	//! \brief 默认构造函数
	OgdcVariant();

	//! \brief 析构函数
	~OgdcVariant();

	//! \brief 拷贝构造函数
	OgdcVariant(const OgdcVariant& varSrc);

	//! \brief OgdcUchar类型构造函数
	OgdcVariant(OgdcUchar bVal);
	//! \brief OgdcShort类型构造函数
	OgdcVariant(OgdcShort sVal);
	//! \brief OgdcInt类型构造函数
	OgdcVariant(OgdcInt iVal);
	//! \brief OgdcLong类型构造函数
	OgdcVariant(OgdcLong lVal);
	//! \brief OgdcFloat类型构造函数
	OgdcVariant(OgdcFloat fVal);
	//! \brief OgdcDouble类型构造函数
	OgdcVariant(OgdcDouble dVal);
	//! \brief OgdcTime类型构造函数
	OgdcVariant(const OgdcTime& tmVal);
	//! \brief OgdcUchar* 类型构造函数
	//! \remarks 内部为内存拷贝，外边注意释放pData;
	OgdcVariant(const OgdcUchar* pData,OgdcInt nSize);
	//! \brief OgdcString 类型构造函数
	OgdcVariant(const OgdcString& str);	

	//! \brief 将 varSrc 赋值变体。
	const OgdcVariant& operator=(const OgdcVariant& varSrc);
	//! \brief 将 bVal 赋值变体
	const OgdcVariant& operator=(OgdcUchar bVal);
	//! \brief 将 sVal 赋值变体
	const OgdcVariant& operator=(OgdcShort sVal);
	//! \brief 将 iVal 赋值变体
	const OgdcVariant& operator=(OgdcInt iVal);
	//! \brief 将 lVal 赋值变体
	const OgdcVariant& operator=(OgdcLong lVal);
	//! \brief 将 fVal 赋值变体
	const OgdcVariant& operator=(OgdcFloat fVal);
	//! \brief 将 dVal 赋值变体
	const OgdcVariant& operator=(OgdcDouble dVal);
	//! \brief 将 tmVal 赋值变体
	const OgdcVariant& operator=(const OgdcTime& tmVal);
	//! \brief 将 str 赋值变体
	const OgdcVariant& operator=(const OgdcString& str);

	//! \brief 设置变量值为bVal 
	void Set(OgdcUchar bVal);
    //! \brief 设置变量值为sVal 
	void Set(OgdcShort sVal);
	//! \brief 设置变量值为nVal 
	void Set(OgdcInt nVal);
	//! \brief 设置变量值为nVal 
	void Set(OgdcLong lVal);
	//! \brief 设置变量值为dVal 
	void Set(OgdcFloat dVal);
	//! \brief 设置变量值为dVal 
	void Set(OgdcDouble dVal);
	//! \brief 设置变量值为tmVal 
	void Set(const OgdcTime& tmVal);
	//! \brief 设置变量值为pData指向的字符串
	void Set(const OgdcUchar* pData,OgdcUint nSize);	
    //! \brief 设置变量值为str 
	void Set(const OgdcString& str);	
	//! \brief 设置为空
	void SetNull();
	
	//! \brief 判断是否相等
	//! \remarks 类型不相等, 则认为不相等(COleVariant的处理方式)
	OgdcBool operator==(const OgdcVariant& varSrc) const;

	//! \brief 判断是否不相等
	//! \remarks 类型不相等, 则认为不相等(COleVariant的处理方式)
	OgdcBool operator!=(const OgdcVariant& varSrc) const;

	//! \brief 判断大小
	//! \remarks 类型不相等, 则认为不相等(COleVariant的处理方式)
	OgdcBool operator<(const OgdcVariant& varSrc) const;

	//! \brief 清空
	void Clear();	

	//! \brief 获取变体类型
	VarType GetType() const 
	{
		return m_nType;
	}

	//! \brief 获取变体值
	VarValue& GetValue() 
	{
		return m_value;
	}
    
	//! \brief 获取变体值
	const VarValue& GetValue() const 
	{
		return m_value;
	}

	//! \brief 转换为double类型
	OgdcDouble ToDouble() const;
	//! \brief 转换为int32 类型
	OgdcInt ToInt() const;
	//! \brief 转换为int64 类型
	OgdcLong ToLong() const;
	//! \brief 转换为String类型
	//! \remarks 对于UGTime类型，转换成数据库识别的格式也就是 %Y-%m-%d %H:%M:%S 。
	OgdcString ToString() const;
	//! \brief 转换为String类型
	//! \remarks 对于UGTime类型，转换成本地当前的时间风格,对于其它类型和ToString()方法一样。
	OgdcString ToStringLocal() const;
	
public:		
	//! \brief 变体类型
	VarType m_nType;
	
	//! \brief 变体值
	VarValue m_value;	

};

}

#endif // !defined(AFX_OGDCVARIANT_H__3DD52A73_5F64_43C1_9374_D128DF18117E__INCLUDED_)

