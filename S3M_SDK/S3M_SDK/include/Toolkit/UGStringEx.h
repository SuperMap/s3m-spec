/* 
	\file UGString.h
	\brief 字符串类
	\author 李同堂、曾志明
	\attention
	Copyright(c) 2000-2010 SuperMap Software Co., Ltd.<br>
	All Rights Reserved
	<b>更新说明：modified by zengzm 2007-3-12 UGString不要从std::string继承，而应该是包裹在内
	以避免绑定在 stl上；即UGString的用户应该依赖于UGString的接口，而不是实现（依赖倒置原理）。</b>
	<em>1、第一条说明。</em>
	<em>2、第二条说明。</em>
*/

#ifndef UGSTRINGEX_H
#define UGSTRINGEX_H

#include "Stream/UGStream.h"
#include "Stream/UGByteArray.h"
#include "ugres.h"
// by zengzm 注意: 这里应该避免把MFC的头文件Include进来(既便添加了WIN32宏),因为MFC把LoadString作为宏
// 在MBCS方式下会变为LoadStringA,导致编译不过.

namespace UGC {

//! \brief 字符串类
//!	\remarks 除非有特殊说明，UGString的接口尽可能和MFC的CString保持一致
//! \attention UGString仅支持多字节编码的字符串，宽字节编码的字符串类为UGStringW。
class TOOLKIT_API UGStringEx : public UGString
{
	friend class UGProgress;

public:
	//! \brief 默认构造函数。
	UGStringEx();

	  //!  \brief Toolkit
	UGStringEx(const UGString& str);

	  //!  \brief Toolkit
	const OgdcString& operator=(const UGString& str);	

	//! \brief 获取字符串对象的Hash值
	//! \remarks 一般用不上，仅供一些特殊类内部使用
	//! \return 返回构造的hash值,和具体的字符串的内容相关
	UGuint HashVal() const;
	
	//! \brief 根据资源ID装载字符串资源。
	//! \param nID 用来装载字符串资源的资源ID[in]。
	//!	\return 装载成功，返回true；失败返回false。
	UGbool LoadResString(UGuint nID);
	
	//! \brief 根据资源ID装载字符串资源。
	//! \param strKey 用来装载字符串资源的资源strKey[in]。
	//!	\return 装载成功，返回true；失败返回false。
	UGbool LoadResString(const UGString& strKey);	

	//! \brief 格式化函数，类似于sprintf。
	//!	\param nID 用作表达格式的字符串的资源的ID[in]。
	//!	\param ... 可选参数[in]。
	//!	\remarks 不能将UGString作为参数传进该函数，而必须调用UGString的Cstr函数得到C字符串。
	//!			 不能将UGlong、UGulong等64位整数作为参数传入该函数,而应该先强制转化为32位整数
	//void Format(UGuint nID,...);
				
//	//! \brief 把字符串保存到流中
//	friend TOOLKIT_API UGStream& operator<<(UGStream& store,const UGStringEx& s);
//		
//	//! \brief 从流中读出字符串
//	friend TOOLKIT_API UGStream& operator>>(UGStream& store,UGStringEx& s);

	// add by cuiwz 2008-5-28 支持 UGByteArray
	//! \brief 把字符串保存到流中
	friend TOOLKIT_API UGByteArray& operator<<(UGByteArray& store,const UGStringEx& s);
	
	//! \brief 从流中读出字符串
	friend TOOLKIT_API UGByteArray& operator>>(UGByteArray& store,UGStringEx& s);

	//! \brief 把自身转换成制定编码的字符串
	UGbool ChangeCharset(UGint eCharset);
private:
	// added by zengzm 2007-3-19 故意只定义不实现，避免拿UGString直接和NULL等进行比较
	// 因为在linux平台上，会导致程序崩溃
	bool operator==(int n) const;
// 
// #ifndef OS_ANDROID
// private:
	//void FormatV(const UGchar* fmt, va_list argList);
/*#endif*/
};

}

#endif

