#ifndef UGTEXTCODEC_H
#define UGTEXTCODEC_H

#include "Stream/ugdefs.h"

namespace UGC {

//! \brief 当前的编码方式，由外部用户制定
static UGString::Charset g_charset;
static UGbool  g_bUseGlobal = FALSE;
//! \brief 字符集转换器, 内部采用iconv实现
class TOOLKIT_API UGTextCodec  
{
public:
	  //!  \brief Toolkit
	UGTextCodec();
	  //!  \brief Toolkit
	~UGTextCodec();
	  //!  \brief Toolkit
	UGTextCodec(const UGMBString& to, const UGMBString& from);
	  //!  \brief Toolkit
	UGTextCodec(UGString::Charset to, UGString::Charset from);

public:
	//!  \brief Toolkit
	static UGString GetCharsetName(UGString::Charset charset);
	//!  \brief Toolkit
	static UGString::Charset GetCharset(const UGString &strCharsetName);
#ifdef _UGUNICODE
	//!  \brief Toolkit
	static UGString::Charset GetCharset(const UGMBString &strCharsetName)
	{
		return GetCharset(UGString().FromMBString(strCharsetName.Cstr(), strCharsetName.GetLength()));
	}
#endif

	//! 外部用户用的Charset
	static UGString::Charset GetGlobalCharset();
	static UGString::Charset GetStrGlobalCharset(const UGString& strCharset);

	static void SetGlobalCharset(const UGString& strCharset,UGbool bUseGlobal = TRUE);
	static void SetGlobalCharset(UGString::Charset charset,UGbool bUseGlobal = TRUE);
	static void SetUseGlobalCharset(UGbool bUseGlobal = TRUE);
	static UGbool IsUseGlobalCharset();
	

public:
	// modified by jifang, 07/29/2005.
	//! \remarks 这个函数在用的时候, 是系统相关的, 要看系统所支持的编码之间的转换. 
	//! 而且直接指定字符串很容易导致打开失败, 如果能用下面的枚举参数的函数, 
	//! 就尽量用下面的重载函数, 实在不行的, 再直接用这个函数. 
	//! 总之一句话, 使用时要小心, 谨遵医嘱, 否则治死人不偿命!
	UGbool Open(const UGMBString& to, const UGMBString& from);
	
	  //!  \brief Toolkit
	UGbool Open(UGString::Charset to, UGString::Charset from);

	  //!  \brief Toolkit
	UGbool IsOpen();
	  //!  \brief Toolkit
	void Close();	

	//! 传入 pSource 和 nSize(按字节计)，
	//! 用target传出转换后的字符串
	//! remark 本函数适用于转换后的字符串属于MBCS（多字节编码）
	  //!  \brief Toolkit
	UGbool Convert(UGMBString& target,const UGMBString& source);
	  //!  \brief Toolkit
	UGbool Convert(UGMBString& target, const UGachar* pSource, UGint nSize);

	UGbool Convert(UGbyte *pDest, UGint nCapacity, UGint &nDestSize, const UGbyte* pSource, UGint nSize);

	// by zengzm 2007-11-21 这几个函数 有问题(static 对象), 先封起来
	  //!  \brief Toolkit
	//static UGTextCodec& UGTOMBCS(Charset charset);
	  //!  \brief Toolkit
	//static UGTextCodec& UGTOUCS(Charset charset);
	//!\remarks if bDirction == TRUE convert the Unicode to MBCS and vice versa
	  //!  \brief Toolkit
	//static UGTextCodec& GetDefault(UGbool bDirection = TRUE);

	// by zengzm 2007-11-21 经过测试,发现 TextCodec的Open 100万次的时间在2秒左右,
	// 效率还马马虎虎, 因此,暂时不进行特殊的优化; 以后谁发现这个是性能瓶颈,再做针对处理

	//! 得到某个字符集转换为 UCS2LE的 转换器
	//! \param charset 字符集类型
	//! \remarks 内部采用返回特定静态对象的方式, 提高效率
	//! \return 返回转换器
//	static UGTextCodec& ToUCS2LE(Charset charset);

	//! 得把UCS2LE 转换为 某个字符集的 转换器
	//! \param charset 字符集类型
	//! \remarks 内部采用返回特定静态对象的方式, 提高效率
	//! \return 返回转换器
//	static UGTextCodec& ToUCS2LE(Charset charset);

private:
	void* m_pHandle;
	// 经过测试,发现iconv应该支持多线程, 因此把buffer放到函数内部, 以便Textcodec也能支持多线程
	// UGMBString m_strBuffer;
	//add by cuiwz 发现在ubuntu8.0.X的版本发，iconv在相同编码转换的时候会崩溃，
	//加了一个环境变量用于控制相同编码转换。
	UGbool m_bCharsetEqual;
};

}

#endif



