//////////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)                
//                                                           
//!  \file OgdcSystem.h
//!  \brief 系统工具类
//!  \details   
//!  \attention 
//!   Copyright (c) 2007-2012 SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OGDCSYSTEM_H__980E850B_7989_4E01_A079_7B18C9B132D5__INCLUDED_)
#define AFX_OGDCSYSTEM_H__980E850B_7989_4E01_A079_7B18C9B132D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Base/OgdcString.h"
namespace OGDC{
//!  \brief 系统工具类
class BASE_API OgdcSystem  
{
private:
	OgdcSystem();
    ~OgdcSystem();
public:

	//! \brief 通过动态库名称，加载动态库
	//! \param strLibName 动态库名称
	static void* LoadDll(const OgdcString& strLibName);

	//! \brief 通过库的句柄和函数名字,得到函数指针
	//! \param hModule 动态库句柄
	//! \param strProName 函数名称
	static void* GetProcAddress(void* hModule, const OgdcString& strProName);

	//! \brief 释放动态库
	//! \param hModule 动态库句柄
	static OgdcBool FreeDll(void* hModule);


	//! \brief 判断字符是否属于空ASCII字符, 包括：空格,\\t\\r\\n等
	static OgdcBool IsSpaceChar(OgdcAchar ch);

	//! \brief 判断字符是否属于空ASCII字符, 包括：空格,\\t\\r\\n等
	static OgdcBool IsSpaceChar(OgdcWchar wch);

	//! \brief 判断字符是否属于数字字符, 从 '0' 到 '9'
	static OgdcBool IsDigit(OgdcAchar ch);

	//! \brief 判断字符是否属于十六进制字符, 包括从 '0' 到 '9', 'A'到'F', 'a'到'f'
	static OgdcBool IsHexDigit(OgdcAchar ch);

	//! \brief 判断字符是否属于大写字符, 从 'A' 到 'Z'
	static OgdcBool IsBigLetter(OgdcAchar ch);

	//! \brief 判断字符是否属于小写字符, 从 'a' 到 'z'
	static OgdcBool IsLittleLetter(OgdcAchar ch);
	
	//! \brief 判断该字节是否是lead byte，即该字节和后续字节联合在一起构成一个双字节的字符（如中文、日文等）
	static OgdcBool IsLeadByte(OgdcAchar ch);
	
	//! \brief 判断一个UCS2的字符是否是英文字符
	//! \return 如果是中文或日文等字符，返回false；是英文字符，返回true
	static OgdcBool IsAscii(OgdcUshort ch);

#ifdef _UGUNICODE
	//! \brief 判断字符是否属于数字字符, 从 '0' 到 '9'
	static OgdcBool IsDigit(OgdcWchar32 ch);

	//! \brief 判断字符是否属于十六进制字符, 包括从 '0' 到 '9', 'A'到'F', 'a'到'f'
	static OgdcBool IsHexDigit(OgdcWchar32 ch);

	//! \brief 判断字符是否属于大写字符, 从 'A' 到 'Z'
	static OgdcBool IsBigLetter(OgdcWchar32 ch);

	//! \brief 判断字符是否属于小写字符, 从 'a' 到 'z'
	static OgdcBool IsLittleLetter(OgdcWchar32 ch);
#endif

	//! \brief 获取程序路径
	static OgdcString GetAppPath();
	//! \brief 获取模块路径
	static OgdcString GetModulePath(const OgdcString& strModultTitle);

	//! \brief 获取模块路径（带后缀名）
	static OgdcString GetModulePathWithExt(const OgdcString& strModultTitle);

	//! \brief 获取目录
	static OgdcString GetDir(const OgdcString& strFile);
	//! \brief 获取文件后缀名
	static OgdcString GetExtName(const OgdcString& strFile);
	//! \brief 获取文件后缀名
	static OgdcBool FindFileExtPaths(const OgdcString& strPath, const OgdcString& strExtName, 
								  OgdcArray<OgdcString>& strFileNames, OgdcBool bAddPath);

	//! \brief 判断当前dll加载方式是否需要修改系统路径，默认不需要修改，web三维应用时需要特殊处理
	static OgdcBool ms_bNeedResetCurrentDirectory;
	
	//!  \brief 获取对应字符的String 
	static OgdcString GetCharsetName(OGDCCharset::Charset charset);
	//!  \brief 获取String的对应的字符串
	static OGDCCharset::Charset GetCharset(const OgdcString &strCharsetName);
	//! \brief 得到字符集的名称
	//! param charset[in] 字符集
	//! return ICU中使用的字符集的名称
	//! remark 这个方法是在OgdcUnicodeString进行编码转换的时候使用，仅限OgdcUnicodeString的FromMBString和ToMBString中调用！
	//! IA5编码部分采用windows-1252进行处理，参见ICU官网,Converter Explorer
	static OgdcString GetCharsetNameICU(OGDCCharset::Charset charset);
	static OGDCCharset::Charset GetCurCharset();

#ifdef _UGUNICODE
	static OgdcInt GetModulePath(const OgdcChar* pModuleTitle, OgdcChar* pModulePath, OgdcInt nLength);
#endif
};

}

#endif // !defined(AFX_OGDCSYSTEM_H__980E850B_7989_4E01_A079_7B18C9B132D5__INCLUDED_)

