/* 
	\file UGStream.h
	\brief 流的虚基类，一般作为存取函数的引用型参数，不可实例化。
	实例化时，内存流采用UGMemoryStream，文件流采用UGFileStream。
	范例程序请参考UGMemoryStream和UGFileStream头文件。
	\author Fox、艾国、曾志明
	\attention
	Copyright(c) 2000-2010 SuperMap Software Co., Ltd.<br>
	All Rights Reserved
	<b>更新说明：。</b>
	<em>1、第一条说明。</em>
	<em>2、第二条说明。</em>
	\sa UGMemoryStream UGFileStream。
*/

/*	修改记录 1
修改人：曾志明
修改原因：增加对超过4G的流的管理
具体修改：把Position从int32改为int64
修改时间：2004-7-12

修改记录 2
修改人：  艾国
修改原因：增加对文件的双向的操作，也增加了一些接口，修改了原来一些bug，同时进行了性能的优化
具体修改：具体看头文件的注释
最后修改时间：2005-6-17
*/

#ifndef UGSTREAM_H
#define UGSTREAM_H

#include "ugdefs.h"

namespace UGC {

//! \brief 流的操作模式
enum UGStreamDirection
{
	UGStreamDead=0,
	//! \brief 只写
	UGStreamSave=1,
	//! \brief 只读
	UGStreamLoad=2,
	//! \brief 读写
	UGStreamLoadSave = 3  //add aiguo
};

//! \brief 流的状态
enum UGStreamStatus 
{
	//! \brief 状态良好
	UGStreamOK=0,
	//! \brief 读取时已到流的尾部
	UGStreamEnd=1,
	//! \brief 写入时流已经写满
	UGStreamFull=2,
	//! \brief 流不可写
	UGStreamNoWrite=3,
	//! \brief 流不可读
	UGStreamNoRead=4,
	//! \brief 内存分配错误
	UGStreamAlloc=5,
	//! \brief 流内部的数据格式错误
	UGStreamFormat=6,
	UGStreamFailure=7
};

//! \brief 流在设置位置（SetPosition）时的起始位置。
enum UGWhence 
{
	//! \brief 开始处。
	UGFromStart=0,
	//! \brief 当前位置。
	UGFromCurrent=1,
	//! \brief 结尾处。
	UGFromEnd=2
};

//! \brief 流的虚基类，一般作为存取函数的引用型参数，不可实例化。
//! \remarks 实例化时，内存流采用UGMemoryStream，文件流采用UGFileStream。
#ifdef SYMBIAN60
class STREAM_APIC UGStream 
#else
class STREAM_API UGStream
#endif
{
public:

	// 仅用于移动端调试用
	static void SetIsIn(bool bIn);
	static bool GetIsIn();

	//! \brief 默认构造函数。
	STREAM_APIF UGStream();	

	//! \brief 析构函数，特意设置为纯虚函数，因为UGStream本身不可实例化。
	virtual STREAM_APIF  ~UGStream();	
	
	//! \brief 关闭流。
	virtual STREAM_APIF UGbool Close();
	
	//! \brief 流的刷新，仅对文件流有效，即把缓存中的内容写入到文件中。
	virtual STREAM_APIF UGbool Flush();  	

	//! \brief 得到流全部空间的大小，文件流时等同于内部缓存区的大小。
	virtual STREAM_APIF UGSizeT GetSpace() const;	

	//! \brief 设置流的空间大小，内存流时只能在空间由流所有时使用，文件流时则改变缓存区大小。
	//! \param space 设置的空间大小值[in]。
	//! \attention 对内存流，在内存空间不由流所有时调用，会发生Assert错误。
	STREAM_APIF void SetSpace(UGSizeT space);  		

	//! \brief 得到当前流的状态。
	STREAM_APIF UGStreamStatus  GetStatus() const;  	

	//! \brief 判断流是否已到结尾，主要供文件流使用
	virtual STREAM_APIF UGbool IsEOF() const;  

	//! \brief 设置流已经发生了某种错误。
	//! \param err 设置的错误类型[in]。
	//! \remarks 一般不需要使用。
	STREAM_APIF void  SetError(UGStreamStatus err);  

	//! \brief 得到流是只读、只写或者读写。
	STREAM_APIF UGStreamDirection GetDirection() const;  	
	
	//! \brief 得到当前指针的位置，以byte为单位。
	virtual STREAM_APIF UGulong GetPosition() const;  
	
	//! \brief 设置当前指针的位置。
	//!  \remarks 对文件流，如果要设置的位置超过原来文件大小，会导致文件长度增加，
	//! 对内存流，如果要设置的位置超过内存空间大小，且内存由流所有，会导致内存空间重新分配，
    //!	内存空间不为流所有，则流会处于Full状态
	//! \param offset 偏移量[in]。
	//! \param whence 偏移量的起始位置[in]。
	virtual STREAM_APIF UGbool SetPosition(UGlong offset,UGWhence whence=UGFromStart);	
	
	//{{ Add by aiguo
	//! \brief 对于文件流，得到文件长度；对于内存流，得到流中实际有效数据的长度（小于等于内存空间）。
	virtual STREAM_APIF UGulong GetLength();;	
	//}}
	
	// 按引擎的需求，重新开放该接口 by jiangzb 2011-11-7
	// deleted by zengzm 2006-3-9 改变是否交换字节顺序的东东太危险，还是内部自己判断，暂时封存
	 //STREAM_APIF void SwapBytes(UGbool s){ swap=s; }
	 STREAM_APIF void SetSwapBytes(UGbool s) {swap = s; }

	//! \brief 得到是否需要进行字节顺序的交换。
	//! \remarks 我们的数据都采用LittleEndian方式存储，在BigEndian的CPU中需要进行字节顺序交换。
	STREAM_APIF UGbool SwapBytes() const; 
#ifdef SYMBIAN60
	//! \brief operator <<
	STREAM_APIF UGStream& operator<<(const UGuchar& v);
	//! \brief operator <<
	STREAM_APIF UGStream& operator<<(const UGachar& v);
	#if _MSC_VER > 1200
	STREAM_APIF UGStream& operator<<(const UGwchar& v){ *this<<(UGshort&)v; ; return *this; }
	#endif
	//! \brief operator <<
	STREAM_APIF UGStream& operator<<(const UGushort& v);
	//! \brief operator <<
	STREAM_APIF UGStream& operator<<(const UGshort& v);
	//! \brief operator <<
	STREAM_APIF UGStream& operator<<(const UGuint& v);
	//! \brief operator <<
	STREAM_APIF UGStream& operator<<(const UGint& v);
	//! \brief operator <<
	STREAM_APIF UGStream& operator<<(const UGfloat& v);
	//! \brief operator <<
	STREAM_APIF UGStream& operator<<(const UGdouble& v);
#ifdef UG_LONG
	//! \brief operator <<
	STREAM_APIF UGStream& operator<<(const UGlong& v);
	//! \brief operator <<
	STREAM_APIF UGStream& operator<<(const UGulong& v);
#endif
#else
	//! \brief operator <<
	virtual STREAM_APIF UGStream& operator<<(const UGuchar& v);
	//! \brief operator <<
	virtual STREAM_APIF UGStream& operator<<(const UGachar& v);
	#if _MSC_VER > 1200
	virtual STREAM_APIF UGStream& operator<<(const UGwchar& v){ *this<<(UGshort)v; return *this; }
	#endif
	//! \brief operator <<
	virtual STREAM_APIF UGStream& operator<<(const UGushort& v);
	//! \brief operator <<
	virtual STREAM_APIF UGStream& operator<<(const UGshort& v);
	//! \brief operator <<
	virtual STREAM_APIF UGStream& operator<<(const UGuint& v);
	//! \brief operator <<
	virtual STREAM_APIF UGStream& operator<<(const UGint& v);
	//! \brief operator <<
	virtual STREAM_APIF UGStream& operator<<(const UGfloat& v);
	//! \brief operator <<
	virtual STREAM_APIF UGStream& operator<<(const UGdouble& v);
#ifdef UG_LONG
	//! \brief operator <<
	virtual STREAM_APIF UGStream& operator<<(const UGlong& v);
	//! \brief operator <<
	virtual STREAM_APIF UGStream& operator<<(const UGulong& v);
#endif
#endif

	//! \brief 保存 p 
	//! \param p 字节指针 [in]。
	//! \param n 个数[in]。
	virtual STREAM_APIF UGStream& Save(const UGuchar* p,UGSizeT n);	
	//! \brief 保存 p 
	//! \param p 字节指针 [in]。
	//! \param n 个数[in]。
	virtual STREAM_APIF UGStream& Save(const UGachar* p,UGSizeT n);
//	#if _MSC_VER > 1200
	virtual STREAM_APIF UGStream& Save(const UGwchar* p,UGSizeT n){ Save((UGshort*)p, n); return *this; }
//	#endif
	//! \brief 保存 p 
	//! \param p 字节指针 [in]。
	//! \param n 个数[in]。
	virtual STREAM_APIF UGStream& Save(const UGushort* p,UGSizeT n);
	//! \brief 保存 p 
	//! \param p 字节指针 [in]。
	//! \param n 个数[in]。
	virtual STREAM_APIF UGStream& Save(const UGshort* p,UGSizeT n);
	//! \brief 保存 p 
	//! \param p 字节指针 [in]。
	//! \param n 个数[in]。
	virtual STREAM_APIF UGStream& Save(const UGuint* p,UGSizeT n);
	//! \brief 保存 p 
	//! \param p 字节指针 [in]。
	//! \param n 个数[in]。
	virtual STREAM_APIF UGStream& Save(const UGint* p,UGSizeT n);
	//! \brief 保存 p 
	//! \param p 字节指针 [in]。
	//! \param n 个数[in]。
	virtual STREAM_APIF UGStream& Save(const UGfloat* p,UGSizeT n);
	//! \brief 保存 p 
	//! \param p 字节指针 [in]。
	//! \param n 个数[in]。
	virtual STREAM_APIF UGStream& Save(const UGdouble* p,UGSizeT n);
#ifdef UG_LONG
	//! \brief 保存 p 
	//! \param p 字节指针 [in]。
	//! \param n 个数[in]。
	virtual STREAM_APIF UGStream& Save(const UGlong* p,UGSizeT n);
	//! \brief 保存 p 
	//! \param p 字节指针 [in]。
	//! \param n 个数[in]。
	virtual STREAM_APIF UGStream& Save(const UGulong* p,UGSizeT n);
#endif

#ifdef SYMBIAN60
	//! \brief operator >>
	STREAM_APIF UGStream& operator>>(UGuchar& v);
	//! \brief operator >>
	STREAM_APIF UGStream& operator>>(UGachar& v);
#if _MSC_VER > 1200
	STREAM_APIF UGStream& operator>>(UGwchar& v){ *this>>(UGshort)v; }
#endif
	//! \brief operator >>
	STREAM_APIF UGStream& operator>>(UGushort& v);
	//! \brief operator >>
	STREAM_APIF UGStream& operator>>(UGshort& v);
	//! \brief operator >>
	STREAM_APIF UGStream& operator>>(UGuint& v);
	//! \brief operator >>
	STREAM_APIF UGStream& operator>>(UGint& v);
	//! \brief operator >>
	STREAM_APIF UGStream& operator>>(UGfloat& v);
	//! \brief operator >>
	STREAM_APIF UGStream& operator>>(UGdouble& v);
#ifdef UG_LONG
	//! \brief operator >>
	STREAM_APIF UGStream& operator>>(UGlong& v);
	//! \brief operator >>
	STREAM_APIF UGStream& operator>>(UGulong& v);
#endif
#else
	//! \brief operator >>
	virtual STREAM_APIF UGStream& operator>>(UGuchar& v);
	//! \brief operator >>
	virtual STREAM_APIF UGStream& operator>>(UGachar& v);
	#if _MSC_VER > 1200
	virtual STREAM_APIF UGStream& operator>>(UGwchar& v){ *this>>(UGshort&)v; return *this; }
	#endif
	//! \brief operator >>
	virtual STREAM_APIF UGStream& operator>>(UGushort& v);
	//! \brief operator >>
	virtual STREAM_APIF UGStream& operator>>(UGshort& v);
	//! \brief operator >>
	virtual STREAM_APIF UGStream& operator>>(UGuint& v);
	//! \brief operator >>
	virtual STREAM_APIF UGStream& operator>>(UGint& v);
	//! \brief operator >>
	virtual STREAM_APIF UGStream& operator>>(UGfloat& v);
	//! \brief operator >>
	virtual STREAM_APIF UGStream& operator>>(UGdouble& v);
#ifdef UG_LONG
	//! \brief operator >>
	virtual STREAM_APIF UGStream& operator>>(UGlong& v);
	//! \brief operator >>
	virtual STREAM_APIF UGStream& operator>>(UGulong& v);
#endif
#endif
	
	//! \brief 获取 p 指针 
	//! \param p 字节指针 [in]。
	//! \param n 个数[in]。
	virtual STREAM_APIF UGStream& Load(UGuchar* p,UGSizeT n);
	//! \brief 获取 p 指针 
	//! \param p 字节指针 [in]。
	//! \param n 个数[in]。
	virtual STREAM_APIF UGStream& Load(UGachar* p,UGSizeT n);
	#if _MSC_VER > 1200
	virtual STREAM_APIF UGStream& Load(UGwchar* p,UGSizeT n){ Load((UGshort*)p, n); return *this; }
	#endif
	//! \brief 获取 p 指针 
	//! \param p 字节指针 [in]。
	//! \param n 个数[in]。
	virtual STREAM_APIF UGStream& Load(UGushort* p,UGSizeT n);
	//! \brief 获取 p 指针 
	//! \param p 字节指针 [in]。
	//! \param n 个数[in]。
	virtual STREAM_APIF UGStream& Load(UGshort* p,UGSizeT n);
	//! \brief 获取 p 指针 
	//! \param p 字节指针 [in]。
	//! \param n 个数[in]。
	virtual STREAM_APIF UGStream& Load(UGuint* p,UGSizeT n);
	//! \brief 获取 p 指针 
	//! \param p 字节指针 [in]。
	//! \param n 个数[in]。
	virtual STREAM_APIF UGStream& Load(UGint* p,UGSizeT n);
	//! \brief 获取 p 指针 
	//! \param p 字节指针 [in]。
	//! \param n 个数[in]。
	virtual STREAM_APIF UGStream& Load(UGfloat* p,UGSizeT n);
	//! \brief 获取 p 指针 
	//! \param p 字节指针 [in]。
	//! \param n 个数[in]。
	virtual STREAM_APIF UGStream& Load(UGdouble* p,UGSizeT n);
#ifdef UG_LONG
	//! \brief 获取 p 指针 
	//! \param p 字节指针 [in]。
	//! \param n 个数[in]。
	virtual STREAM_APIF UGStream& Load(UGlong* p,UGSizeT n);
	//! \brief 获取 p 指针 
	//! \param p 字节指针 [in]。
	//! \param n 个数[in]。
	virtual STREAM_APIF UGStream& Load(UGulong* p,UGSizeT n);
#endif


	//! \brief 把字符串保存到流中
	STREAM_APIF UGStream& operator<<(const UGString& s);
		
	//! \brief 从流中读出字符串
	STREAM_APIF UGStream& operator>>(UGString& s);

	//! \brief 把UGVariant保存到流中
	STREAM_APIF UGStream& operator<<(const UGVariant& var);
	
	//! \brief 从流中读出UGVariant
	STREAM_APIF UGStream& operator>>(UGVariant& var);

	STREAM_APIF void SetCharset(UGString::Charset eCharset)
	{
		m_eCharset = eCharset;
	}
	STREAM_APIF UGString::Charset GetCharset()
	{
		return m_eCharset;
	}

private:
	// modified by jiangzb 2011-11-7 改回成员变量，Postgre引擎的需求（小端环境下按行操作数据库的二进制是大端的）
	// modified by zengzm 2007-3-27 改为静态变量,只需要判断一次,提高运行效率
	// static UGbool swap;
	UGbool swap;

	// 考虑到Stream中要操作字符串，就需要考虑字符串的编码问题，增加该变量来控制对Stream
	// 对于多字节版本来说，只在读取UGString的时候有效，会把该UGString的字符集设计为m_eCharset，写入的时候不考虑该变量
	// 对于Unicode版本来说，在写入UGString的时候会先把字符集转换成m_eCharset，再写入，读取时，也会从m_eCharset转换成Unicode
	UGString::Charset m_eCharset;
protected:
	UGuchar           *begptr;
	UGuchar           *endptr;
	UGuchar           *wrptr;
	UGuchar           *rdptr;
	// UGuint			 pos;
	UGulong			 pos;
	
	//文件的实际长度,主要用在内存文件中
	// modified by zengzm 2007-3-28 改为无符号型
	//UGlong m_lActualPos; //Add by aiguo
	UGulong m_lActualPos;
	
	UGStreamDirection  dir;
	UGStreamStatus     code;
	UGbool             owns;
	
	//Add by aiguo
	STREAM_APIF enum UGSignLoadSave 
	{
		UGSignStreamLoad =0,
		UGSignStreamSave =1
	};
	//{{//Add by aiguo,在这种UGStreamLoadSave模式下Load和Save和标识
	UGSignLoadSave     SignLoadSave; 
	//}}
private:
	STREAM_APIF UGStream(const UGStream&);
	STREAM_APIF UGStream &operator=(const UGStream&);
	
protected:
	// modified by zengzm 2007-3-9 这个函数被子类调用，外面不直接使用
	STREAM_APIF UGbool Open(UGStreamDirection save_or_load, UGSizeT size=8192,  UGuchar* data=NULL);  	
	
	virtual STREAM_APIF UGSizeT WriteBuffer(UGSizeT count);
	virtual STREAM_APIF UGSizeT ReadBuffer(UGSizeT count);
	
	
	//{{Add by aiguo 主要是解决在UGStreamLoadSave模式下，Load和Save混乱的问题，这里主要是设置一些标志来控制Load和Save.
	virtual STREAM_APIF void SetSignLoadSave(UGint nSign);
	//}}
};


}

#endif



