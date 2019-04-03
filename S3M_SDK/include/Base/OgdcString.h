//////////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)               
//                                                           
//!  \file OgdcString.h
//!  \brief 字符串类  
//!  \details
//!  \attention 
//!   Copyright (c) 2007-2012 SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OGDCSTRING_H__2CA5E3F6_86E2_4EA7_A56F_CE716B8B1E05__INCLUDED_)
#define AFX_OGDCSTRING_H__2CA5E3F6_86E2_4EA7_A56F_CE716B8B1E05__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if defined(_WIN32) || defined(WIN32)
/* This needs to come before any includes for MSVC compiler */
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <string>
#include "OgdcArray.h"
#define WhiteBox_Ignore

#ifdef _UGUNICODE
#if !defined OS_ANDROID && !defined IOS
namespace icu_3_8
{
	class UnicodeString;
	class NumberFormat;
}
using namespace icu_3_8;
#else
namespace icu_50
{
	class UnicodeString;
	class NumberFormat;
}
using namespace icu_50;
#define UG_US_STACKBUF_SIZE (sizeof(void *)==4 ? 13 : 15)
#endif
#endif

namespace OGDC {
#pragma warning(disable:4267)
class OgdcMBString;
class OgdcUnicodeString;

//! \brief OGDCPCTSTR为了不与微软的LPCTSTR冲突，因为指针可能是4位或者8位，去掉L，加上OGDC前缀。
typedef const OgdcAchar* OGDCPCTSTR; 
typedef const OgdcWchar* OGDCPCWSTR;

typedef OgdcArray<OgdcUnicodeString> OgdcUnicodeStringArray; 
typedef OgdcArray<OgdcMBString> OgdcMBStringArray;

//! \brief 字符串字符集类
struct  OGDCCharset
{
public:
	enum Charset
	{   
		//! \brief ANSI编码
		ANSI = 0,
		//! \brief Default编码
		Default = 1,
		//! \brief Symbol编码
		Symbol = 2,
		//! \brief MAC编码
		MAC = 77,
		
		//! \brief ISO-2022-JP-2编码，建议做日文编码转换的时候都用ISO2022JP2编码
		//在做日文编码转换的时候发现 '\'用ShiftJIS转换出现问题，用ISO2022JP2就没有什么问题，
		//建议大家做日文编码转换的时候都用这个编码
		ISO2022JP2 = 127,
		//! \brief ShiftJIS编码
		ShiftJIS = 128,

		//! \brief Hangeul编码
		Hangeul = 129,
		//! \brief Johab编码
		Johab	= 130,
		
		//! \brief GB18030则是最新的国家标准,它涵盖了GB2312和GBK等很多内容, 因此,我们统一用GB18030来代表中国(大陆)的字符集
		GB18030 = 134, 
		//GB2312 是最核心的简体中文字符集, 而GBK是在GB2312上面的扩展,加入了很多日文和繁体字形
		//GB2312 = 134, 
		//! \brief Big5是台湾几个5大计算机公司搞出来的繁体字符集, 由于当时和中国(大陆)没有沟通和协商, 所以Big5和GB是不兼容的,
		//! 两者收录的字形有较大差别, 且编码有部分重叠, 因此计算机系统是无法自动识别 某个编码是 GB 还是 Big5的.
		//! 香港和澳门由于繁体字，因此也采用Big5；同时由于受葡萄牙语影响较大, 因此很多葡萄牙语也经常要和Big5混杂使用，
		//! 于是又在Big5上面进行了扩展, 最流行的为 BIG5-HKSCS:2001. 因此我们采用|BIG5-HKSCS:2001".
		ChineseBIG5 = 136, 
		//! \brief Greek编码
		Greek = 161,
		//! \brief Turkish编码
		Turkish = 162,
		//! \brief Vietnamese编码
		Vietnamese = 163,
		//! \brief Hebrew编码
		Hebrew = 177,
		//! \brief Arabic编码
		Arabic = 178,
		//! \brief Baltic编码
		Baltic = 186,
		//! \brief Russian编码
		Russian = 204,
		//! \brief Thai编码
		Thai = 222,
		//! \brief EastEurope编码
		EastEurope = 238,
		//! \brief 从严格意义上说, UTF8,UCS2,UCS4等都不是字符集,而只是Unicode的一种编码方式, 
		//! 但由于这几种使用较为频繁, 且计算机的处理上和其它字符集比较类似, 因此,都统一放在这里了.
		UTF8 = 250,
		//! \brief Windows默认的Unicode编码	这里默认是按照UTF16LE处理的
		UCS2LE = 251,
		//! \brief UCS2BE编码 这里默认是按照UTF16BE处理的
		UCS2BE = 252,
		//! \brief UCS4LE编码 这里默认是按照UTF32LE处理的
		UCS4LE = 253,
		//! \brief UCS4BE编码 这里默认是按照UTF32BE处理的
		UCS4BE = 254,
		//! \brief Unicode编码 这个编码由于需要添加头部，由于历史原因，在iconv中默认大端机器是UCS2BE 小端机器是UCS2LE
		Unicode = 132,
		//! \brief OEM编码
		OEM = 255,
		//上面的为Windows系统字符编码
		//! \brief Windows1252编码
		Windows1252 = 137,
		//! \brief Korean编码
		Korean = 131,
		//! \brief Cyrillic编码
		Cyrillic = 135,
        //! \brief xIA5编码
		xIA5 = 3,
		//! \brief xIA5German编码
		xIA5German = 4,
		//! \brief xIA5Swedish编码
		xIA5Swedish = 5,
		//! \brief xIA5Norwegian编码
		xIA5Norwegian = 6,
		//! \brief UTF7编码
		UTF7 = 7
		//! \brief UTF32编码
// 		UTF32 = 8,
// 		//! \brief UTF32BE编码
// 		UTF32BE = 9,
// 		//! \brief UTF32LE编码
// 		UTF32LE = 10,
// 		//! \brief UTF16编码
// 		UTF16BE = 11,
// 		UTF16LE = 12
//		UnicodeBE = 133,
	};                                                                                                                                                                      
};
//! \brief 字符串类
//! \attention OgdcMBString仅支持多字节编码的字符串。
class BASE_API OgdcMBString : public OGDCCharset 
{

public:
	
	//! \brief 默认构造函数。
	OgdcMBString();
    
#ifdef IOS
    //! \release function
    ~OgdcMBString();
#endif
    
	//! \brief 拷贝构造函数。
	//! \param str 用来拷贝的字符串(OgdcMBString类型)
	OgdcMBString(const OgdcMBString& str);	

	//! \brief 拷贝构造函数
	//! \param str 用来拷贝的字符串(std::string类型)
	//! \remarks 主要用于和Markup等进行匹配
	OgdcMBString(const std::string& str);	
	
	//! \brief 拷贝构造函数。	
	//! \param pstr 用来拷贝的字符串(OgdcAchar*类型)
	OgdcMBString(const OgdcAchar* pStr);
	
	//! \brief 拷贝构造函数。
	//! \param pstr 用来拷贝的字符串(OgdcAchar*类型)
	//! \param nSize 指定使用的字节长度
	OgdcMBString(const OgdcAchar* pStr, OgdcInt nSize);
	
	//! \brief 拷贝构造函数，将ch重复拷贝nRepeat次。
	//! \param ch 用来拷贝的字符串(OgdcAchar类型)
	//! \param nRepeat 重复拷贝次数,默认为1
	//! \remarks explicit用于防止隐形的转化
	//! \include Ogdcstring.cpp
	explicit OgdcMBString(OgdcAchar ch, OgdcInt nRepeat=1);
	
	//! \brief 追加字符串
	//! \param pch 要追加的字符串
	//! \param nSize 要追加的字节个数
	void Append(const OgdcAchar* pStr, OgdcInt nSize);

	//! \brief 将OgdcMBString转换为const char*。
	operator OGDCPCTSTR() const;

	//! \brief 将OgdcMBString转换成std::string
	operator std::string() const;

	//! \brief 获取C字符串
	const OgdcAchar * Cstr() const;

	//! \brief 获取C++字符串
	const std::string& Str() const;

	//! \brief 获取指定索引的字符。
	//! \param nIndex 指定的索引
	//! \return 返回指定索引的字符。
	OgdcAchar GetAt(OgdcInt nIndex) const;		
	
	//! \brief 设置指定索引的字符
	//! \param nIndex 指定的索引
	//! \param ch 要设置的字符
	void SetAt(OgdcInt nIndex, OgdcAchar ch);

	//! \brief 获取指定索引的字符。
	//! \param  nIndex 指定的索引
	//! \return 返回指定索引的字符
	OgdcAchar operator[](OgdcInt nIndex) const;
	
	//! \brief 设置指定索引的字符的引用，支持str[i]='a';
	//! \param nIndex 指定的索引
	//! \return 返回指定索引的字符的引用
	OgdcAchar& operator[](OgdcInt nIndex);

	//! \brief 保留指定字节个数的内存空间
	//! \param nCapacity 可容纳的字节个数
	//! \remarks 如果原有空间不够, 内存空间会增加。
	//! 如果原有空间比指定的大, 也不会释放空间。
	void SetCapacity(OgdcInt nCapacity);
	
	//! \brief 获取目前所有空间(包括已经使用的和保留的)所占用的字节个数
	OgdcInt GetCapacity() const; 

	//! \brief 获取字符串长度
	OgdcInt GetLength() const;	
	
	//! \brief 设置字符串长度
	//! \param nLength 字节个数
	//! \remarks 如果长度变小,会自动截断; 
	//!  如果长度变大,会填充0x00。
	void SetLength(OgdcInt nLength);
	

	//! \brief 把字符串内容转换为大写，不会修改非ASCII字符
	//! \return 返回自身的引用,支持连写
	OgdcMBString& MakeUpper();
	
	//! \brief 把字符串内容转换为小写，不会修改非ASCII字符
	//! \return 返回自身的引用,支持连写
	OgdcMBString& MakeLower();
	
	//! \brief 把字符串逆转
	//! \return 返回自身的引用,支持连写
	//! \remarks 支持中间带有中文（MBCS）的转换
	//! \include str_MakeReverse.cpp
	OgdcMBString& MakeReverse();
	
	//! \brief 取出字符串对象左边的指定长度的子字符串
	//! \param nCount 字节个数
	//! \return 返回左边指定长度的子字符串
	//! \include str_left.cpp
	OgdcMBString Left(OgdcInt nCount) const;
	
	//! \brief 取出字符串对象最右边的指定长度的子字符串
	//! \param nCount 字节个数
	//! \return 返回右边指定长度的子字符串
	//! \include str_right.cpp
	OgdcMBString Right(OgdcInt nCount) const;
	
	//! \brief 从字符串对象中取出从指定索引起的子字符串对象
	//! \param nFirst 指定的中间的索引序号
	//! \return 返回从中间指定的索引开始,到最后的子字符串
	OgdcMBString Mid(OgdcInt nFirst) const;
	
	//! \brief 从字符串对象中获取从指定索引起、指定长度的子字符串对象
	//! \param nFirst nFirst 索引值，大于等于零
	//! \param nCount 要取出的字节个数
	//! \include str_mid.cpp
	OgdcMBString Mid(OgdcInt nFirst,OgdcInt nCount) const;

	//! \brief 删除从指定索引起的指定长度的字符串
	//! \param nIndex 开始删除的索引值
	//! \param nCount 要删除的字节个数
	//! \return 返回删除字符串后的结果字符串对象的长度
	OgdcInt Delete(OgdcInt nIndex,OgdcInt nCount=1);
	
	//! \brief 在指定的索引之前插入字符串
	//! \param nIndex 指定的索引
	//! \param pstr 要插入的字符串
	//! \return 插入后,结果字符串的长度
	OgdcInt Insert(OgdcInt nIndex,const OgdcAchar * pStr);	
	
	//! \brief 在指定的索引之前插入若干数目的字符
	//! \param nIndex 指定的索引
	//! \param ch 指定的字符
	//! \param nCount 字符重复的次数
	//! \return 插入后,结果字符串的长度
	OgdcInt Insert(OgdcInt nIndex,OgdcAchar ch,OgdcInt nCount=1);
	
	//! \brief 删除字符串对象中等于指定字符的字符
	//! \param ch 要删除的字符
	//! \return 返回删除的字符个数
	OgdcInt Remove(OgdcAchar ch);		

	//! \brief 删除字符串对象左边的空格（\\t\\r\\n）
	//! \return 删除后,字符串对象的引用
	OgdcMBString& TrimLeft();
	
	//! \brief 删除字符串对象左边的指定字符
	//! \param ch 要删除的字符
	//! \return 删除后,字符串对象的引用
	OgdcMBString& TrimLeft(OgdcAchar ch);
	
	//! \brief 删除字符串对象左边属于指定字符集合的字符
	//! \param pstr 指定的字符串集合
	//! \return 删除后,字符串对象的引用,以支持连写
	//! \remarks 对于字符集合,是要删除左边的存在于字符集合中的每一个字符
	//! \include str_TrimLeft.cpp
	OgdcMBString& TrimLeft(const OgdcAchar* pStr);
	
	//! \brief 删除字符串对象右边的空格（\\t\\r\\n）
	//! \return 删除后,字符串对象的引用
	OgdcMBString& TrimRight();
	
	//! \brief 删除字符串对象右边的指定字符
	//! \param ch 指定的字符
	//! \return 删除后,字符串对象的引用
	OgdcMBString& TrimRight(OgdcAchar ch);
	
	//! \brief 删除字符串对象右边属于指定字符集合的字符
	//! \param pstr 指定的字符串集合
	//! \return 删除后,字符串对象的引用,以支持连写
	//! \remarks 对于字符集合,是要删除右边的存在于字符集合中的每一个字符
	//! \include str_TrimRight.cpp
	OgdcMBString& TrimRight(const OgdcAchar* pStr);

	//! \brief 删除字符串对象右边等于指定字符串的字符
	//! \param &str 指定的字符串
	//! \return 删除后,字符串对象的引用,以支持连写
	//! \include str_TrimRightString.cpp
	OgdcMBString& TrimRightString(const OgdcMBString &str);
	
	//! \brief 删除字符串对象左边等于指定字符串的字符
	//! \param &str 指定的字符串
	//! \return 删除后,字符串对象的引用,以支持连写
	//! \include str_TrimLeftString.cpp
	OgdcMBString& TrimLeftString(const OgdcMBString &str);
	
	//! \brief 判断字符串是否为空
	//! \return 为空,返回true;不为空,返回false
	OgdcBool IsEmpty() const;	
	
	//! \brief 清空字符串
	void Empty();

	//! \brief 比较两个字符串的大小
	//! \param lpsz 要比较的字符串指针
	//! \return 小于返回负数；等于返回0；大于返回正数
	//! \remarks 区分大小写，按照内部值依顺序比较
	OgdcInt Compare(const OgdcAchar* pStr) const;
	
	//! \brief 比较两个字符串的大小
	//! \param lpsz 要比较的字符串指针
	//! \return 小于返回负数；等于返回0；大于返回正数
	//! \remarks 不区分大小写，按照内部值依顺序比较
 	OgdcInt CompareNoCase(const OgdcAchar* pStr) const;

	//! \brief 比较两个字符串的大小
	//! \remarks 不区分大小写，按照内部值依顺序比较
	//! \param str 要比较的字符串
	//! \return 小于返回负数；等于返回0；大于返回正数。
	OgdcInt CompareNoCase(const OgdcMBString& str) const;
	
	//! \brief 从指定位置开始，查找指定的字符
	//!	\param ch 要查找的字符[in]。
	//!	\param nStart 查找起始位置，对应的字符会被查找[in]。
	//!	\return 返回找到的字符的索引值，从0算起。找不到返回-1
	OgdcInt Find(OgdcAchar ch,OgdcInt nStart=0) const;		

	//! \brief 从指定位置开始，查找完全相等的子字符串
	//!	\param lpszSub 要查找的子字符串[in]。
	//!	\param nStart 查找起始位置，对应的子字符串会被查找[in]。
	//!	\return 返回找到的子字符串的索引值，从0算起。找不到返回-1
	OgdcInt Find(const OgdcAchar* pStrSub, OgdcInt nStart=0) const;

	//! \brief 从后向前查找指定的字符
	//!	\param ch 要查找的字符[in]。
	//!	\param nStart 查找起始位置，对应的子字符会被查找[in] -1代表从末尾查找。
	//!	\return 返回找到的字符的索引值，从0算起;找不到返回-1
	OgdcInt ReverseFind(OgdcAchar ch,OgdcInt nStart=-1) const;	

	//! \brief 从后向前查找指定的字符串。
	//!	\param ch 要查找的字符串[in]。
	//!	\param nStart 查找起始位置，对应的子字符串会被查找[in] -1代表从末尾查找。
	//!	\return 返回找到的字符串的索引值，从0算起;找不到返回-1
	OgdcInt ReverseFind(const OgdcAchar* pStrSub,OgdcInt nStart=-1) const;
	
	//! \brief 查找第一个“既在字符串对象中，又在指定的字符集合中”的字符的位置。
	//!	\param lpszCharSet 指定的字符集合[in]。
	//!	\return 返回找到的字符的索引，从0算起。找不到返回-1
	OgdcInt FindOneOf(const OgdcAchar* pStrCharSet) const;	

	//! \brief 得到字符串对象中，从第0个字符起，到不符合指定字符集合中的字符为止的子字符串。
	//! \param lpszCharSet 指定的字符集合[in]。
	//!	\return 返回符合条件的子字符串。
	//!	include str_SpanIncluding.cpp
	OgdcMBString SpanIncluding(const OgdcAchar* pStrCharSet) const;		

	//! \brief 得到字符串对象中，从第0个字符起，到符合指定字符集合中的字符为止的子字符串。
	//! \param lpszCharSet 指定的字符集合[in]。
	//!	\return 返回符合条件的子字符串。
	//!	include str_SpanExcluding.cpp
	OgdcMBString SpanExcluding(const OgdcAchar* pStrCharSet) const;
		
	//! \brief 替换函数，把字符串对象中指定的字符替换为新的字符。
	//! \param chOld 指定要被替换的字符[in]。
	//!	\param chNew 新的字符[in]。
	//!	\return 返回替换的次数。
	//! \remarks 比较时区分大小写。
	OgdcInt Replace(OgdcAchar chOld,OgdcAchar chNew);

	//! \brief 替换函数，把字符串对象中指定的子字符串替换为新的字符串
	//! \param lpszOld 指定要被替换的子字符串[in]。
	//!	\param lpszNew 新的字符串[in]。
	//!	\return 返回替换的次数。
	//!	\remarks 比较时区分大小写。
	OgdcInt Replace(const OgdcAchar* pStrOld,const OgdcAchar* pStrNew);

	

	//! \brief 格式化函数，类似于sprintf。
	//!	\param fmt 用来表达格式的字符串[in]。
	//!	\param ... 可选参数[in]。
	//!	\remarks 不能将OgdcMBString作为参数传进该函数，而必须调用OgdcMBString的Cstr函数得到C字符串。
	//!			 不能将OgdcLong、OgdcUlong等64位整数作为参数传入该函数,而应该先强制转化为32位整数
	void Format(const OgdcAchar* pStr,...);
	/*
	//! \brief 格式化函数，类似于sprintf。
	//!	\param nID 用作表达格式的字符串的资源的ID[in]。
	//!	\param ... 可选参数[in]。
	//!	\remarks 不能将OgdcMBString作为参数传进该函数，而必须调用OgdcMBString的Cstr函数得到C字符串。
	//!			 不能将OgdcLong、OgdcUlong等64位整数作为参数传入该函数,而应该先强制转化为32位整数 
	
	//下面的函数不能要，因为他需要LoadString(nID);
		void Format(OgdcUint nID,...);*/

	//! \brief 字符串赋值函数。
	const OgdcMBString& operator=(const OgdcMBString& str);	
	
	//! \brief 用std::string来赋值函数。
	//! \param str std::string类型的字符串,主要用于和Markup等进行匹配
	const OgdcMBString& operator=(const std::string& str);	
	
	//! \brief 用C 字符串赋值函数。	
	//! \param pstr C字符串参数
	const OgdcMBString& operator=(const OgdcAchar* pStr);
	//! \brief 重载 +
	OgdcMBString operator+(const OgdcAchar ch) const;
	//! \brief 重载 +
	OgdcMBString operator+(const OgdcAchar* pStr) const;
	//! \brief 重载 +
	OgdcMBString operator+(const OgdcMBString& str) const;
	
	//! \brief 重载 +=
	const OgdcMBString& operator+=(const OgdcMBString& str);
	//! \brief 重载 +=
	const OgdcMBString& operator+=(const OgdcAchar ch);
	//! \brief 重载 +=
	const OgdcMBString& operator+=(const OgdcAchar* pStr);
	
	//! \brief 重载 ==
	bool operator==(const OgdcAchar* pStr) const;
	//! \brief 重载 ==
	bool operator==(const OgdcMBString& str) const;

	//! \brief 重载 !=
	bool operator!=(const OgdcAchar* pStr) const;
	//! \brief 重载 !=
	bool operator!=(const OgdcMBString& str) const;

	//! \brief 重载 <
	bool operator<(const OgdcMBString& str) const;

	//! \brief 重载 +
	friend BASE_API OgdcMBString operator+(OgdcAchar ch, const OgdcMBString &str);
	//! \brief 重载 +
	friend BASE_API OgdcMBString operator+(const OgdcAchar* pStr, const OgdcMBString &str);	

	
	//! \brief 把OgdcLong型的数据转化为OgdcMBString型字符串
	//! \param lVal 要转化的OgdcLong型数据
	//! \param base 进制,默认为10进制,还支持16进制;其它暂时不支持
	//! \return 返回转化后的字符串
	//! \remarks 当base为16进制时,转化后的结果前面会加0X,字母都为大写。
	//! 用这种方式间接支持long型数据的Format,即先转化为OgdcMBString,再用%s进行格式化。
	//! 采用静态函数，返回OgdcMBString的方式，以便于调用的代码连写。
	static OgdcMBString From(OgdcLong lVal, OgdcUint base=10);
	//! \brief 把OgdcUlong型的数据转化为OgdcMBString型字符串
	static OgdcMBString From(OgdcUlong lVal, OgdcUint base=10);
	//! \brief 把OgdcInt型的数据转化为OgdcMBString型字符串
	static OgdcMBString From(OgdcInt nVal, OgdcUint base=10);
	//! \brief 把OgdcUint型的数据转化为OgdcMBString型字符串
	static OgdcMBString From(OgdcUint nVal, OgdcUint base=10);
	//! \brief 考虑到OgdcArray.GetSize(),返回了OgdcSizeT,在Linux上该类型为unsigned long,特为此添加转换函数
	static OgdcMBString From(unsigned long nVal, OgdcUint base=10);	WhiteBox_Ignore

	//Convert string to a integer number, assuming given number base
	//! \brief 把字符串转换为OgdcInt型数值
	OgdcInt ToInt(OgdcUint base=10) const;

	//! \brief 把字符串转换为无符号OgdcUint型数值
	OgdcUint ToUInt(OgdcUint base=10) const;
	
	//! \brief 把字符串转换为OgdcLong型数值
	//! \param s 要转化的字符串
	//! \param base 进制,默认为10进制,还支持16进制;其它暂时不支持
	OgdcLong ToLong(OgdcUint base=10) const;

	//! \brief 把字符串转换为无符号OgdcUlong型数值
	//! \param s 要转化的字符串
	//! \param base 进制,默认为10进制,还支持16进制;其它暂时不支持
	OgdcUlong ToULong(OgdcUint base=10) const;
	
	//Convert string into real number
	//! \brief 把字符串转换为OgdcFloat型数值
	OgdcFloat ToFloat() const;

	//! \brief 把字符串转换为OgdcDouble型数值	
	OgdcDouble ToDouble() const;
	
	//! \brief 把字符串按照指定的分隔符分割为一个字符串数组。
	//! \param dest 生成的字符串数组[out]。
	//!	\param delims 指定的分隔符，默认为回车换行[in]。
	//!	\param bKeepEmptyString 是否保留分割后 空的字符串, 如字符串";"通过';'分割后,dest数组的size
	//! 在保留空字符串的时候为2, 不保留的时候为0; 
	//!	\return 分割后字符串数组的长度。
	OgdcInt Split(OgdcMBStringArray &strDests,const OgdcMBString& strDelims="\r\n", OgdcBool bKeepEmptyString=FALSE) const;
	
	//! \brief 设置字符串里面的字符的字符集	 
	//是不是OgdcInt 要变成OgdcMBString::Charset
	void SetCharset(OgdcInt nCharset);
	//! \brief 获取字符串里面的字符的字符集	 
	OgdcMBString::Charset GetCharset() const;

	//! \brief 替换函数，把字符串对象中指定的子字符串替换为新的字符串		
	//!	\remarks 比较时区分大小写。
	void Replace(OgdcInt startIndex, OgdcInt length, const OgdcAchar* pStrNew);
public:
	//! \brief 将当前String转换成std::string
	//! \param str 目标std::string
	//!  \return str的引用,内部不涉及到编码转换，简单的转换，这个和unicodestring的概念不太一致 
	std::string& ToStd(std::string& str) const;
	//! \brief 将std::string 转换成当前的String
	//! \param str 源std::string
	//!  \return 当前String的引用 这个和unicodestring的概念不太一致 
	OgdcMBString& FromStd(const std::string& str);


public:	
	//! \brief std的字符串
	std::string m_string; // 内部采用std的string实现
	//! \brief 字符集
	OgdcMBString::Charset m_nCharset;
};

#ifdef _UGUNICODE
const OgdcWchar g_strNull[]={_U('\r'),_U('\n'),_U('\t'),_U(' '),_U('\0')};
class BASE_API OgdcUnicodeString : public OGDCCharset 
{

public:
	//! \brief 默认构造函数
	OgdcUnicodeString();

	//! \breif 析构函数
	~OgdcUnicodeString();

	//! \brief 拷贝构造函数
	//! \param 用来拷贝的字符串OgdcUnicodeString
	OgdcUnicodeString(const OgdcUnicodeString& str);

// 	//! \brief 拷贝构造函数
// 	//! \param str 用来拷贝的字符串(std::string类型)
// 	//! \remarks 主要用于和Markup等进行匹配
// 	OgdcUnicodeString(const std::string& str);	

	//! \brief 拷贝构造函数。	
	//! \param pstr 用来拷贝的字符串(OgdcWchar*类型)
	OgdcUnicodeString(const OgdcWchar* pStr);

	//! \brief 拷贝构造函数。
	//! \param pstr 用来拷贝的字符串(OgdcWchar*类型)
	//! \param nSize 指定使用的字节长度
	OgdcUnicodeString(const OgdcWchar* pStr, OgdcInt nSize);

// 	//! \brief 拷贝构造函数。
// 	//! \param pstr 用来拷贝的字符串(OgdcAchar*类型)
// 	//! \param nSize 指定使用的字节长度
// 	OgdcUnicodeString(const OgdcAchar* pStr);
// 
// 	//! \brief 拷贝构造函数。
// 	//! \param pstr 用来拷贝的字符串(OgdcAchar*类型)
// 	//! \param nSize 指定使用的字节长度
// 	OgdcUnicodeString(const OgdcAchar* pStr, OgdcInt nSize);

	//! \brief 拷贝构造函数，将ch重复拷贝nRepeat次。
	//! \param ch 用来拷贝的字符串(OgdcWchar类型)
	//! \param nRepeat 重复拷贝次数,默认为1
	//! \remarks explicit用于防止隐形的转化
	//! \include Ogdcstring.cpp
	explicit OgdcUnicodeString(OgdcWchar ch, OgdcInt nRepeat=1);

	//! \brief 追加字符串
	//! \param pch 要追加的字符串
	//! \param nSize 要追加的字节个数
	void Append(const OgdcWchar* pStr, OgdcInt nSize);

	//! \brief 将OgdcUnicodeString转换为const char*。
	operator OGDCPCWSTR() const;

	//! \brief 将OgdcUnicodeString转换成std::string
	//operator std::string() const;

	//! \brief 将OgdcUnicodeString转换为UnicodeString
	operator UnicodeString() const;

	//! \brief 获取Wchar字符串
	const OgdcWchar * Cstr() const;

	//! \brief 获取指定索引的字符。
	//! \param nIndex 指定的索引
	//! \return 返回指定索引的字符。
	OgdcWchar GetAt(OgdcInt nIndex) const;		
	OgdcWchar32 GetWchar32At(OgdcInt nIndex) const;

	//! \brief 设置指定索引的字符
	//! \param nIndex 指定的索引
	//! \param ch 要设置的字符
	void SetAt(OgdcInt nIndex, OgdcWchar ch);

	//! \brief 获取指定索引的字符。
	//! \param  nIndex 指定的索引
	//! \return 返回指定索引的字符
	OgdcWchar operator[](OgdcInt nIndex) const;

	//! \brief 设置指定索引的字符的引用，支持str[i]='a';
	//! \param nIndex 指定的索引
	//! \return 返回指定索引的字符的引用
	OgdcWchar& operator[](OgdcInt nIndex);

	//! \brief 保留指定字节个数的内存空间
	//! \param nCapacity 可容纳的字节个数
	//! \remarks 如果原有空间不够, 内存空间会增加。
	//! 如果原有空间比指定的大, 也不会释放空间。
	void SetCapacity(OgdcInt nCapacity);

	//! \brief 获取目前所有空间(包括已经使用的和保留的)所占用的字节个数
	OgdcInt GetCapacity() const; 

	//! \brief 获取字符串长度
	OgdcInt GetLength() const;	

	//! \brief 获取字符串长度
	//! \breif 这里提供Char32长度，如果有大于2byte大小长度的字符，这个长度
	//  会和GetLength所获取的长度不等，
	OgdcInt GetWchar32Length() const;	

	//! \brief 设置字符串长度
	//! \param nLength 字节个数
	//! \remarks 如果长度变小,会自动截断; 
	//!  如果长度变大,会填充0x00。
	void SetLength(OgdcInt nLength);


	//! \brief 把字符串内容转换为大写，不会修改非ASCII字符
	//! \return 返回自身的引用,支持连写
	OgdcUnicodeString& MakeUpper();

	//! \brief 把字符串内容转换为小写，不会修改非ASCII字符
	//! \return 返回自身的引用,支持连写
	OgdcUnicodeString& MakeLower();

	//! \brief 把字符串逆转
	//! \return 返回自身的引用,支持连写
	//! \remarks 支持中间带有中文（MBCS）的转换
	//! \include str_MakeReverse.cpp
	OgdcUnicodeString& MakeReverse();

	//! \brief 取出字符串对象左边的指定长度的子字符串
	//! \param nCount 字节个数
	//! \return 返回左边指定长度的子字符串
	//! \include str_left.cpp
	OgdcUnicodeString Left(OgdcInt nCount) const;

	//! \brief 取出字符串对象最右边的指定长度的子字符串
	//! \param nCount 字节个数
	//! \return 返回右边指定长度的子字符串
	//! \include str_right.cpp
	OgdcUnicodeString Right(OgdcInt nCount) const;

	//! \brief 从字符串对象中取出从指定索引起的子字符串对象
	//! \param nFirst 指定的中间的索引序号
	//! \return 返回从中间指定的索引开始,到最后的子字符串
	OgdcUnicodeString Mid(OgdcInt nFirst) const;

	//! \brief 从字符串对象中获取从指定索引起、指定长度的子字符串对象
	//! \param nFirst nFirst 索引值，大于等于零
	//! \param nCount 要取出的字节个数
	//! \include str_mid.cpp
	OgdcUnicodeString Mid(OgdcInt nFirst,OgdcInt nCount) const;

	//! \brief 删除从指定索引起的指定长度的字符串
	//! \param nIndex 开始删除的索引值
	//! \param nCount 要删除的字节个数
	//! \return 返回删除字符串后的结果字符串对象的长度
	OgdcInt Delete(OgdcInt nIndex,OgdcInt nCount=1);

	//! \brief 在指定的索引之前插入字符串
	//! \param nIndex 指定的索引
	//! \param pstr 要插入的字符串
	//! \return 插入后,结果字符串的长度
	OgdcInt Insert(OgdcInt nIndex,const OgdcWchar * pStr);	

	//! \brief 在指定的索引之前插入若干数目的字符
	//! \param nIndex 指定的索引
	//! \param ch 指定的字符
	//! \param nCount 字符重复的次数
	//! \return 插入后,结果字符串的长度
	OgdcInt Insert(OgdcInt nIndex,OgdcWchar ch,OgdcInt nCount=1);
    
	//add by cuiwz
	//! \brief 删除字符串对象中等于指定字符的字符
	//! \param ch 要删除的字符
	//! \return 返回删除的字符个数
	OgdcInt Remove(OgdcWchar ch);

	//! \brief 删除字符串对象左边的空格（\\t\\r\\n）
	//! \return 删除后,字符串对象的引用
	OgdcUnicodeString& TrimLeft()
	{
		return TrimLeft(g_strNull);
	};
	
	//! \brief 删除字符串对象左边的指定字符
	//! \param ch 要删除的字符
	//! \return 删除后,字符串对象的引用
	OgdcUnicodeString& TrimLeft(OgdcWchar ch);
	
	//! \brief 删除字符串对象左边属于指定字符集合的字符
	//! \param pstr 指定的字符串集合
	//! \return 删除后,字符串对象的引用,以支持连写
	//! \remarks 对于字符集合,是要删除左边的存在于字符集合中的每一个字符
	//! \include str_TrimLeft.cpp
	OgdcUnicodeString& TrimLeft(const OgdcWchar* pStr);
	
	//! \brief 删除字符串对象右边的空格（\\t\\r\\n）
	//! \return 删除后,字符串对象的引用
	OgdcUnicodeString& TrimRight()
	{
		return TrimRight(g_strNull);
	};
	
	//! \brief 删除字符串对象右边的指定字符
	//! \param ch 指定的字符
	//! \return 删除后,字符串对象的引用
	OgdcUnicodeString& TrimRight(OgdcWchar ch);
	
	//! \brief 删除字符串对象右边属于指定字符集合的字符
	//! \param pstr 指定的字符串集合
	//! \return 删除后,字符串对象的引用,以支持连写
	//! \remarks 对于字符集合,是要删除右边的存在于字符集合中的每一个字符
	//! \include str_TrimRight.cpp
	OgdcUnicodeString& TrimRight(const OgdcWchar* pStr);

	//! \brief 删除字符串对象右边等于指定字符串的字符
	//! \param &str 指定的字符串
	//! \return 删除后,字符串对象的引用,以支持连写
	//! \include str_TrimRightString.cpp
	OgdcUnicodeString& TrimRightString(const OgdcUnicodeString &str);
	
	//! \brief 删除字符串对象左边等于指定字符串的字符
	//! \param &str 指定的字符串
	//! \return 删除后,字符串对象的引用,以支持连写
	//! \include str_TrimLeftString.cpp
	OgdcUnicodeString& TrimLeftString(const OgdcUnicodeString &str);
	
	//! \brief 判断字符串是否为空
	//! \return 为空,返回true;不为空,返回false
	OgdcBool IsEmpty() const;	
	
	//! \brief 清空字符串
	void Empty();

	//! \brief 比较两个字符串的大小
	//! \param lpsz 要比较的字符串指针
	//! \return 小于返回负数；等于返回0；大于返回正数
	//! \remarks 区分大小写，按照内部值依顺序比较
	OgdcInt Compare(const OgdcWchar* pStr) const;
	
	//! \brief 比较两个字符串的大小
	//! \param lpsz 要比较的字符串指针
	//! \return 小于返回负数；等于返回0；大于返回正数
	//! \remarks 不区分大小写，按照内部值依顺序比较
 	OgdcInt CompareNoCase(const OgdcWchar* pStr) const;

	//! \brief 比较两个字符串的大小
	//! \remarks 不区分大小写，按照内部值依顺序比较
	//! \param str 要比较的字符串
	//! \return 小于返回负数；等于返回0；大于返回正数。
	OgdcInt CompareNoCase(const OgdcUnicodeString& str) const;
	
	//! \brief 从指定位置开始，查找指定的字符
	//!	\param ch 要查找的字符[in]。
	//!	\param nStart 查找起始位置，对应的字符会被查找[in]。
	//!	\return 返回找到的字符的索引值，从0算起。找不到返回-1
	OgdcInt Find(OgdcWchar ch,OgdcInt nStart=0) const;		

	//! \brief 从指定位置开始，查找完全相等的子字符串
	//!	\param lpszSub 要查找的子字符串[in]。
	//!	\param nStart 查找起始位置，对应的子字符串会被查找[in]。
	//!	\return 返回找到的子字符串的索引值，从0算起。找不到返回-1
	OgdcInt Find(const OgdcWchar* pStrSub, OgdcInt nStart=0) const;

	//! \brief 从后向前查找指定的字符
	//!	\param ch 要查找的字符[in]。
	//!	\param nStart 查找起始位置，对应的子字符会被查找[in] -1代表从末尾查找。
	//!	\return 返回找到的字符的索引值，从0算起;找不到返回-1
	OgdcInt ReverseFind(OgdcWchar ch,OgdcInt nStart=-1) const;	

	//! \brief 从后向前查找指定的字符串。
	//!	\param ch 要查找的字符串[in]。
	//!	\param nStart 查找起始位置，对应的子字符串会被查找[in] -1代表从末尾查找。
	//!	\return 返回找到的字符串的索引值，从0算起;找不到返回-1
	OgdcInt ReverseFind(const OgdcWchar* pStrSub,OgdcInt nStart=-1) const;
	
	//! \brief 查找第一个“既在字符串对象中，又在指定的字符集合中”的字符的位置。
	//!	\param lpszCharSet 指定的字符集合[in]。
	//!	\return 返回找到的字符的索引，从0算起。找不到返回-1
	OgdcInt FindOneOf(const OgdcWchar* pStrCharSet) const;	

	//! \brief 得到字符串对象中，从第0个字符起，到不符合指定字符集合中的字符为止的子字符串。
	//! \param lpszCharSet 指定的字符集合[in]。
	//!	\return 返回符合条件的子字符串。
	//!	include str_SpanIncluding.cpp
	OgdcUnicodeString SpanIncluding(const OgdcWchar* pStrCharSet) const;		

	//! \brief 得到字符串对象中，从第0个字符起，到符合指定字符集合中的字符为止的子字符串。
	//! \param lpszCharSet 指定的字符集合[in]。
	//!	\return 返回符合条件的子字符串。
	//!	include str_SpanExcluding.cpp
	OgdcUnicodeString SpanExcluding(const OgdcWchar* pStrCharSet) const;
		
	//! \brief 替换函数，把字符串对象中指定的字符替换为新的字符。
	//! \param chOld 指定要被替换的字符[in]。
	//!	\param chNew 新的字符[in]。
	//!	\return 返回替换的次数。
	//! \remarks 比较时区分大小写。
	OgdcInt Replace(OgdcWchar chOld,OgdcWchar chNew);

	//! \brief 替换函数，把字符串对象中指定的子字符串替换为新的字符串
	//! \param lpszOld 指定要被替换的子字符串[in]。
	//!	\param lpszNew 新的字符串[in]。
	//!	\return 返回替换的次数。
	//!	\remarks 比较时区分大小写。
	OgdcInt Replace(const OgdcWchar* pStrOld,const OgdcWchar* pStrNew);

	

	//! \brief 格式化函数，类似于sprintf。
	//!	\param fmt 用来表达格式的字符串[in]。
	//!	\param ... 可选参数[in]。
	//!	\remarks 不能将OgdcUnicodeString作为参数传进该函数，而必须调用OgdcUnicodeString的Cstr函数得到C字符串。
	void Format(const OgdcWchar* pStr,...);
	/*
	//! \brief 格式化函数，类似于sprintf。
	//!	\param nID 用作表达格式的字符串的资源的ID[in]。
	//!	\param ... 可选参数[in]。
	//!	\remarks 不能将OgdcUnicodeString作为参数传进该函数，而必须调用OgdcUnicodeString的Cstr函数得到C字符串。
	//!			 不能将OgdcLong、OgdcUlong等64位整数作为参数传入该函数,而应该先强制转化为32位整数 
	
	//下面的函数不能要，因为他需要LoadString(nID);
		void Format(OgdcUint nID,...);*/

	//! \brief 字符串赋值函数。
	const OgdcUnicodeString& operator=(const OgdcUnicodeString& str);	
	
// 	//! \brief 用std::string来赋值函数。
// 	//! \param str std::string类型的字符串,主要用于和Markup等进行匹配
// 	const OgdcUnicodeString& operator=(const std::string& str);	
	
	//! \brief 用C 字符串赋值函数。	
	//! \param pstr C字符串参数
	const OgdcUnicodeString& operator=(const OgdcWchar* pStr);




	//add by jiangzb
	//! \brief 重载 +
	OgdcUnicodeString operator+(const OgdcWchar ch) const;
	//! \brief 重载 +
	OgdcUnicodeString operator+(const OgdcWchar* pStr) const;
	//! \brief 重载 +
	OgdcUnicodeString operator+(const OgdcUnicodeString& str) const;
	
	//! \brief 重载 +=
	const OgdcUnicodeString& operator+=(const OgdcUnicodeString& str);
	//! \brief 重载 +=
	const OgdcUnicodeString& operator+=(const OgdcWchar ch);
	//! \brief 重载 +=
	const OgdcUnicodeString& operator+=(const OgdcWchar* pStr);
	
	//! \brief 重载 ==
	bool operator==(const OgdcWchar* pStr) const;
	//! \brief 重载 ==
	bool operator==(const OgdcUnicodeString& str) const;

	//! \brief 重载 !=
	bool operator!=(const OgdcWchar* pStr) const;
	//! \brief 重载 !=
	bool operator!=(const OgdcUnicodeString& str) const;

	//! \brief 重载 <
	bool operator<(const OgdcUnicodeString& str) const;

	//! \brief 重载 +
	friend BASE_API OgdcUnicodeString operator+(OgdcWchar ch, const OgdcUnicodeString &str);
	//! \brief 重载 +
	friend BASE_API OgdcUnicodeString operator+(const OgdcWchar* pStr, const OgdcUnicodeString &str);	

	//! \brief 重载 ==
	friend BASE_API bool operator==(const OgdcWchar* pStr, const OgdcUnicodeString& str);
	
	//! \brief 把OgdcLong型的数据转化为OgdcUnicodeString型字符串
	//! \param lVal 要转化的OgdcLong型数据
	//! \param base 进制,默认为10进制,还支持16进制;其它暂时不支持
	//! \return 返回转化后的字符串
	//! \remarks 当base为16进制时,转化后的结果前面会加0X,字母都为大写。
	//! 用这种方式间接支持long型数据的Format,即先转化为OgdcUnicodeString,再用%s进行格式化。
	//! 采用静态函数，返回OgdcUnicodeString的方式，以便于调用的代码连写。
	static OgdcUnicodeString From(OgdcLong lVal, OgdcUint base=10);
	//! \brief 把OgdcUlong型的数据转化为OgdcUnicodeString型字符串
	static OgdcUnicodeString From(OgdcUlong lVal, OgdcUint base=10);
	//! \brief 把OgdcInt型的数据转化为OgdcUnicodeString型字符串
	static OgdcUnicodeString From(OgdcInt nVal, OgdcUint base=10);
	//! \brief 把OgdcUint型的数据转化为OgdcUnicodeString型字符串
	static OgdcUnicodeString From(OgdcUint nVal, OgdcUint base=10);
	//! \brief 考虑到OgdcArray.GetSize(),返回了OgdcSizeT,在Linux上该类型为unsigned long,特为此添加转换函数
	static OgdcUnicodeString From(unsigned long nVal, OgdcUint base=10);	WhiteBox_Ignore

	//Convert string to a integer number, assuming given number base
	//! \brief 把字符串转换为OgdcInt型数值
	OgdcInt ToInt(OgdcUint base=10) const;

	//! \brief 把字符串转换为无符号OgdcUint型数值
	OgdcUint ToUInt(OgdcUint base=10) const;
	
	//! \brief 把字符串转换为OgdcLong型数值
	//! \param s 要转化的字符串
	//! \param base 进制,默认为10进制,还支持16进制;其它暂时不支持
	OgdcLong ToLong(OgdcUint base=10) const;

	//! \brief 把字符串转换为无符号OgdcUlong型数值
	//! \param s 要转化的字符串
	//! \param base 进制,默认为10进制,还支持16进制;其它暂时不支持
	OgdcUlong ToULong(OgdcUint base=10) const;
	
	//Convert string into real number
	//! \brief 把字符串转换为OgdcFloat型数值
	OgdcFloat ToFloat() const;

	//! \brief 把字符串转换为OgdcDouble型数值	
	OgdcDouble ToDouble() const;
	
	//! \brief 把字符串按照指定的分隔符分割为一个字符串数组。
	//! \param dest 生成的字符串数组[out]。
	//!	\param delims 指定的分隔符，默认为回车换行[in]。
	//!	\param bKeepEmptyString 是否保留分割后 空的字符串, 如字符串";"通过';'分割后,dest数组的size
	//! 在保留空字符串的时候为2, 不保留的时候为0; 
	//!	\return 分割后字符串数组的长度。
	OgdcInt Split(OgdcUnicodeStringArray &strDests,const OgdcWchar* pStrDelims=_U("\r\n"), OgdcBool bKeepEmptyString=FALSE) const;
	
	//! \brief 设置字符串里面的字符的字符集	 
	//是不是OgdcInt 要变成OgdcUnicodeString::Charset
	void SetCharset(OgdcInt nCharset);
	//! \brief 获取字符串里面的字符的字符集	 
	OgdcMBString::Charset GetCharset() const;

	//! \brief 替换函数，把字符串对象中指定的子字符串替换为新的字符串		
	//!	\remarks 比较时区分大小写。
	void Replace(OgdcInt startIndex, OgdcInt length, const OgdcWchar* pStrNew);

	OgdcUnicodeString& FromUTF8(const OgdcAchar *pSrc, OgdcInt nSrcLength);
	OgdcMBString& ToUTF8(OgdcMBString& strResult) const;
public:
	//! \brief 将当前String转换成std::string
	//! \param str 目标std::string
	//! \param charset 目标std::string的编码
	//!  \return str的引用,
	std::string& ToStd(std::string& str,OGDCCharset::Charset charset = OGDCCharset::UTF8) const;

	//! \brief 转换成指定编码的MBString,编码为str的编码
	OgdcMBString& ToMBString(OgdcMBString& str) const
	{
		ToStd(str.m_string,str.m_nCharset);
		return str;
	}

	//! \brief 将std::string 转换成当前的String
	//! \param str 源std::string
	//! \param charset 源std::string 编码
	//!  \return 当前String的引用 
	OgdcUnicodeString& FromStd(const std::string& str,OGDCCharset::Charset charset = OGDCCharset::UTF8)
	{
		return FromMBString(str.c_str(),str.length(),charset);
	}

	//! \brief 从str转换成当前UnicodeString,str可以是任意编码
	OgdcUnicodeString& FromMBString(const OgdcMBString& str)
	{
		return  FromStd(str.m_string,str.m_nCharset);
	}
	OgdcUnicodeString& FromMBString(const OgdcAchar *pSrc, OgdcInt nSrcLength,OGDCCharset::Charset charset = OGDCCharset::Default);

public:
	static OgdcInt u_str32len(const OgdcWchar32* pStr)
	{
		const OgdcWchar32 *eos = pStr;
		while( *eos++ ) ;
		return( (OgdcInt)(eos - pStr - 1) );
	};

	static OgdcInt WcharToAchar(const OgdcWchar* pWchar, OgdcAchar *pAchar, OgdcInt nAcharLength);
	static OgdcInt AcharToWchar(const OgdcAchar* pAchar, OgdcWchar *pWchar, OgdcInt nWcharLength);
	static OgdcInt WcharToWchar32(const OgdcWchar* pWchar, OgdcWchar32* pWchar32, OgdcInt nWchar32Length);
	static OgdcInt Wchar32ToWchar( const OgdcWchar32* pWchar32, OgdcWchar* pWchar, OgdcInt nWcharLength );
	
	struct ShowUnicodeString	// 这个结构体很有意思，既能解决UnicodeString内容的显示问题，又能为UnicodeString初始分配内存
	{
		void		*pString;
		void		*pVftable;
#if !defined OS_ANDROID && !defined IOS
		OgdcInt		fLength;
		OgdcInt		fCapacity;     
		OgdcWchar	*fArray; 
		OgdcShort	fFlags;
		OgdcWchar	fStackBuffer[7];
 #else       
        union StackBufferOrFields {
            // fStackBuffer is used iff (fFlags&kUsingStackBuffer)
            // else fFields is used
            OgdcWchar fStackBuffer[8];  // buffer for short strings, together with fRestOfStackBuffer
            struct {
                OgdcWchar   *fArray;    // the Unicode data
                OgdcInt fCapacity;  // capacity of fArray (in UChars)
                OgdcInt fLength;    // number of characters in fArray if >127; else undefined
            } fFields;
        } fUnion;
		OgdcWchar	fRestOfStackBuffer[UG_US_STACKBUF_SIZE-8];
		OgdcAchar	fShortLength;
		OgdcByte	fFags;
#endif
	};
	
private:
	OgdcInt FindNotOneOf(const OgdcWchar* pStrCharSet) const;
    OgdcInt GetStrLength(const OgdcWchar* pStr) const ;
	inline OgdcWchar* UG_memchr(const OgdcWchar *s, OgdcWchar c, OgdcInt count) 
	{
		if(count>0) 
		{
			/* trivial search for a BMP code point */
			const OgdcWchar *limit=s+count;
			do 
			{
				if(*s==c) 
				{
					return (OgdcWchar *)s;
				}
			} while(++s!=limit);
			 return NULL;
		} 
		else 
		{
			return NULL; /* no string */
		}
	}

	inline OgdcInt UG_memcmp(const OgdcWchar *buf1, const OgdcWchar *buf2, OgdcInt count) 
	{
		if(count > 0) 
		{
			const OgdcWchar *limit = buf1 + count;
			OgdcInt result;

			while (buf1 < limit) {
				result = (OgdcInt)*buf1 - (OgdcInt)*buf2;
				if (result != 0) {
					return result;
				}
				buf1++;
				buf2++;
			}
		}
		return 0;
	}
	inline OgdcWchar* UG_memcpy(OgdcWchar  *dst,const OgdcWchar  *src, OgdcInt n) 
	{
		OgdcWchar *anchor = dst;            /* save a pointer to start of dst */
		while(n > 0 )  
		{
			*dst= *src;
			dst++;
			src++;
			--n;
		}

		return anchor;
	}
private:
	// UnicodeString采用icu的UnicodeString实现
	union   //定义匿名枚举,方便显示UnicodeString的真实内容,请勿操作struct ShowUnicodeString中的内容,结构体的具体内容需要与ICU的类成员同步更新  add by zhangkai!
	{		
		UnicodeString* m_pString;
		struct ShowUnicodeString m_innerUnicodeString;
	};
	//! \brief 字符集
public:
	OgdcMBString::Charset m_nCharset;

private:
	inline void InitCharset()
	{
		if (OGDC_ISBIGENDIAN)
		{
			m_nCharset = OgdcMBString::UCS2BE;
		}
		else
		{
			m_nCharset = OgdcMBString::UCS2LE;
		}
		//pChar = NULL;
	}
};
#endif

#ifdef _UGUNICODE
typedef OgdcUnicodeString OgdcString;
#else
typedef OgdcMBString OgdcString;
#endif //  _UNICODE

EXTERN_C BASE_API  void Unicode2MBString(const OgdcString& strUnicode, OgdcMBString& strMB);
#define UNICODE2MBSTRING(strU,strMB) Unicode2MBString(strU,strMB)

EXTERN_C BASE_API  void MBString2Unicode(const OgdcMBString& strMB, OgdcString& strUnicode);
#define MBSTRING2UNICODE(strMB,strU) MBString2Unicode(strMB,strU)

typedef OgdcArray<OgdcString> OgdcStringArray;
}

#endif // !defined(AFX_OGDCSTRING_H__2CA5E3F6_86E2_4EA7_A56F_CE716B8B1E05__INCLUDED_)

