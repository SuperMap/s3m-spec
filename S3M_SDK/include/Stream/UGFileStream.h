/* 	\file UGFileStream.h
	\brief 文件流类
	\author Fox、艾国、曾志明
	\attention
	Copyright(c) 2000-2010 SuperMap Software Co., Ltd.<br>
	All Rights Reserved
	<b>更新说明：。</b>
	<em>1、第一条说明。</em>
	<em>2、第二条说明。</em>
	\sa UGStream UGMemoryStream。
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

#ifndef UGFILESTREAM_H
#define UGFILESTREAM_H
#include "UGStream.h"
#include "UGByteArray.h"

namespace UGC {

//! \brief 文件流类
/**	如果用UGStreamSave模式打开文件时，只能往文件中保存内容，不能在文件中读取数据，
如果用UGStreamLoad模式打开文件时，只能读取文件，只有用UGStreamLoadSave模式打开文件即可以读也可以写。
（1）、用只写模式打开
 UGFileStream fileStream;
 UGString strFileName  = "D:/Data/file/fileStreamLoad.txt";
 UGchar pch[10];
 for( i = 0; i < 10; i++)
 {
  pch[i] = 'a' + i;
 }
 if (fileStream.Open(strFileName, UGStreamSave)) //如果文件存在，文件里的内容将丢失
 {
  fileStream.Save(pch, 10);//正确
  fileStream.SetPosition(5, UGFromStart);
  fileStream.Load(pch, 4); //这样就会产生错误
  fileStream.Close();
 }
（2）、用只写模式打开
 UGFileStream fileStream;
 UGString strFileName  = "D:/Data/file/fileStreamLoad.txt";
 UGchar pch[10];
 for( i = 0; i < 10; i++)
 {
  pch[i] = 'a' + i;
 }
 if (fileStream.Open(strFileName, UGStreamLoad)) 
 {
  fileStream.Load(pch, 10);//正确
  fileStream.Save(pch, 4); //错误，这里不能把此内容写进文件里
  fileStream.Close();
 }
（3）、用读写模式打开
 UGFileStream fileStream;
 UGString strFileName  = "D:/Data/file/fileStreamLoad.txt";
 UGchar pch[10];
 for( i = 0; i < 10; i++)
 {
  pch[i] = 'a' + i;
 }
 if (fileStream.Open(strFileName, UGStreamLoadSave))
 {
  fileStream.Save(pch, 10); //把pch中的内容保存在文件里，如果原来文件存在，并且文件中有内容，会修改原来的内容
  fileStream.SetPosition(0, UGFromStart);
  fileStream.Load(pch, 10);//正确
  fileStream.Close();
 }
*/
#ifdef SYMBIAN60
class STREAM_APIC UGFileStream : public UGStream  
#else
class STREAM_API UGFileStream : public UGStream 
#endif
{
public:
	//!\brief 默认构造函数
	STREAM_APIF UGFileStream();
	//!\brief 默认析构函数
	virtual STREAM_APIF ~UGFileStream();	
	
	//! \brief 打开文件流。
	//! \param filename 指定文件名[in]。
	//! \param save_or_load 指定打开的方式，只读、只写或读写[in]。
	//! \param size 缓存的大小[in]。
	STREAM_APIF UGbool Open(const UGchar* filename, UGStreamDirection save_or_load, UGuint size=8192);			

	//! \brief 关闭文件流。
	virtual STREAM_APIF UGbool Close();
	
	//! \brief 设置文件指针的位置，如果超过文件大小，会导致文件增长。
	//! \param offset 偏移量[in]。
	//! \param whence 起始位置[in]。
	virtual STREAM_APIF UGbool SetPosition(UGlong offset,UGWhence whence=UGFromStart);	
	
	//! \brief 得到文件长度 add by aiguo
	virtual STREAM_APIF UGulong GetLength();

	// added by zengzm 2005-09-09
	//! \brief 判断是否已到文件尾
	virtual STREAM_APIF UGbool IsEOF() const;


	//! \brief 判断文件流是否处于打开状态。
	//! \return 打开状态返回true,关闭状态返回false。
	STREAM_APIF UGbool IsOpen() const;

	//! \brief operator <<
	STREAM_APIF UGStream& operator<<(const UGuchar& v){ UGStream::operator<<(v); return *this; }
	//! \brief operator <<
	STREAM_APIF UGStream& operator<<(const UGachar& v){ UGStream::operator<<(v); return *this; }
#if _MSC_VER > 1200
	STREAM_APIF UGStream& operator<<(const UGwchar& v){ UGStream::operator<<(v); return *this; }
#endif
	//! \brief operator <<
	STREAM_APIF UGStream& operator<<(const UGushort& v){ UGStream::operator<<(v); return *this; }
	//! \brief operator <<
	STREAM_APIF UGStream& operator<<(const UGshort& v){ UGStream::operator<<(v); return *this; }
	//! \brief operator <<
	STREAM_APIF UGStream& operator<<(const UGuint& v){ UGStream::operator<<(v); return *this; }
	//! \brief operator <<
	STREAM_APIF UGStream& operator<<(const UGint& v){ UGStream::operator<<(v); return *this; }
	//! \brief operator <<
	STREAM_APIF UGStream& operator<<(const UGfloat& v){ UGStream::operator<<(v); return *this; }
	//! \brief operator <<
	STREAM_APIF UGStream& operator<<(const UGdouble& v){ UGStream::operator<<(v); return *this; }
#ifdef UG_LONG
	//! \brief operator <<
	STREAM_APIF UGStream& operator<<(const UGlong& v){ UGStream::operator<<(v); return *this; }
	//! \brief operator <<
	STREAM_APIF UGStream& operator<<(const UGulong& v){ UGStream::operator<<(v); return *this; }
#endif
	
	//! \brief 保存 p 
	//! \param p 字节指针 [in]。
	//! \param n 个数[in]。
	STREAM_APIF UGStream& Save(const UGuchar* p,UGSizeT n){ UGStream::Save(p,n); return *this; }
	//! \brief 保存 p 
	//! \param p 字节指针 [in]。
	//! \param n 个数[in]。
	STREAM_APIF UGStream& Save(const UGachar* p,UGSizeT n){ UGStream::Save(p,n); return *this; }
//#if _MSC_VER > 1200
	STREAM_APIF UGStream& Save(const UGwchar* p,UGSizeT n){ UGStream::Save(p,n); return *this; }
//#endif
	//! \brief 保存 p 
	//! \param p 字节指针 [in]。
	//! \param n 个数[in]。
	STREAM_APIF UGStream& Save(const UGushort* p,UGSizeT n){ UGStream::Save(p,n); return *this; }
	
	//! \brief 保存 p 
	//! \param p 字节指针 [in]。
	//! \param n 个数[in]。
	STREAM_APIF UGStream& Save(const UGshort* p,UGSizeT n){ UGStream::Save(p,n); return *this; }
	//! \brief 保存 p 
	//! \param p 字节指针 [in]。
	//! \param n 个数[in]。
	STREAM_APIF UGStream& Save(const UGuint* p,UGSizeT n){ UGStream::Save(p,n); return *this; }
	//! \brief 保存 p 
	//! \param p 字节指针 [in]。
	//! \param n 个数[in]。
	STREAM_APIF UGStream& Save(const UGint* p,UGSizeT n){ UGStream::Save(p,n); return *this; }
	//! \brief 保存 p 
	//! \param p 字节指针 [in]。
	//! \param n 个数[in]。
	STREAM_APIF UGStream& Save(const UGfloat* p,UGSizeT n){ UGStream::Save(p,n); return *this; }
	//! \brief 保存 p 
	//! \param p 字节指针 [in]。
	//! \param n 个数[in]。
	STREAM_APIF UGStream& Save(const UGdouble* p,UGSizeT n){ UGStream::Save(p,n); return *this; }
#ifdef UG_LONG
	//! \brief 保存 p 
	//! \param p 字节指针 [in]。
	//! \param n 个数[in]。
	STREAM_APIF UGStream& Save(const UGlong* p,UGSizeT n){ UGStream::Save(p,n); return *this; }
	//! \brief 保存 p 
	//! \param p 字节指针 [in]。
	//! \param n 个数[in]。
	STREAM_APIF UGStream& Save(const UGulong* p,UGSizeT n){ UGStream::Save(p,n); return *this; }
#endif
	
	//! \brief  Load single items from stream
	STREAM_APIF UGStream& operator>>(UGuchar& v){ UGStream::operator>>(v); return *this; }
	//! \brief operator >>
	STREAM_APIF UGStream& operator>>(UGachar& v){ UGStream::operator>>(v); return *this; }
#if _MSC_VER > 1200
	STREAM_APIF UGStream& operator>>(UGwchar& v){ UGStream::operator>>(v); return *this; }
#endif
	//! \brief operator >>
	STREAM_APIF UGStream& operator>>(UGushort& v){ UGStream::operator>>(v); return *this; }
	//! \brief operator >>
	STREAM_APIF UGStream& operator>>(UGshort& v){ UGStream::operator>>(v); return *this; }
	//! \brief operator >>
	STREAM_APIF UGStream& operator>>(UGuint& v){ UGStream::operator>>(v); return *this; }
	//! \brief operator >>
	STREAM_APIF UGStream& operator>>(UGint& v){ UGStream::operator>>(v); return *this; }
	//! \brief operator >>
	STREAM_APIF UGStream& operator>>(UGfloat& v){ UGStream::operator>>(v); return *this; }
	//! \brief operator >>
	STREAM_APIF UGStream& operator>>(UGdouble& v){ UGStream::operator>>(v); return *this; }
#ifdef UG_LONG
	//! \brief operator >>
	STREAM_APIF UGStream& operator>>(UGlong& v){ UGStream::operator>>(v); return *this; }
	//! \brief operator >>
	STREAM_APIF UGStream& operator>>(UGulong& v){ UGStream::operator>>(v); return *this; }
#endif
	
	//! \brief 获取 p 指针 
	//! \param p 字节指针 [in]。
	//! \param n 个数[in]。
	STREAM_APIF UGStream& Load(UGuchar* p,UGSizeT n){ UGStream::Load(p,n); return *this; }
	//! \brief 获取 p 指针 
	//! \param p 字节指针 [in]。
	//! \param n 个数[in]。
	STREAM_APIF UGStream& Load(UGachar* p,UGSizeT n){ UGStream::Load(p,n); return *this; }
#if _MSC_VER > 1200
	STREAM_APIF UGStream& Load(UGwchar* p,UGSizeT n){ UGStream::Load(p,n); return *this; }
#endif
	//! \brief 获取 p 指针 
	//! \param p 字节指针 [in]。
	//! \param n 个数[in]。
	STREAM_APIF UGStream& Load(UGushort* p,UGSizeT n){ UGStream::Load(p,n); return *this; }
	//! \brief 获取 p 指针 
	//! \param p 字节指针 [in]。
	//! \param n 个数[in]。
	STREAM_APIF UGStream& Load(UGshort* p,UGSizeT n){ UGStream::Load(p,n); return *this; }
	//! \brief 获取 p 指针 
	//! \param p 字节指针 [in]。
	//! \param n 个数[in]。
	STREAM_APIF UGStream& Load(UGuint* p,UGSizeT n){ UGStream::Load(p,n); return *this; }
	//! \brief 获取 p 指针 
	//! \param p 字节指针 [in]。
	//! \param n 个数[in]。
	STREAM_APIF UGStream& Load(UGint* p,UGSizeT n){ UGStream::Load(p,n); return *this; }
	//! \brief 获取 p 指针 
	//! \param p 字节指针 [in]。
	//! \param n 个数[in]。
	STREAM_APIF UGStream& Load(UGfloat* p,UGSizeT n){ UGStream::Load(p,n); return *this; }
	//! \brief 获取 p 指针 
	//! \param p 字节指针 [in]。
	//! \param n 个数[in]。
	STREAM_APIF UGStream& Load(UGdouble* p,UGSizeT n){ UGStream::Load(p,n); return *this; }
#ifdef UG_LONG
	//! \brief 获取 p 指针 
	//! \param p 字节指针 [in]。
	//! \param n 个数[in]。
	STREAM_APIF UGStream& Load(UGlong* p,UGSizeT n){ UGStream::Load(p,n); return *this; }
	//! \brief 获取 p 指针 
	//! \param p 字节指针 [in]。
	//! \param n 个数[in]。
	STREAM_APIF UGStream& Load(UGulong* p,UGSizeT n){ UGStream::Load(p,n); return *this; }
#endif
	//! \brief 读文件到字节数组中，Add By tangzq in 2009-4-10
	//! \param byteArray [in]。
	//! \return 。
	//! \remarks 。
	//! \attention 。
	STREAM_APIF UGbool LoadByteArray(UGByteArray& byteArray);

	//! \brief 写字节数组到文件中，Add By tangzq in 2009-4-10
	//! \param byteArray [in]。
	//! \return 。
	//! \remarks 。
	//! \attention 。
	STREAM_APIF UGbool SaveByteArray(const UGByteArray& byteArray);

	STREAM_APIF UGStream& operator<<(const UGString& s);

	//! \brief 从流中读出字符串
	STREAM_APIF UGStream& operator>>(UGString& s);

	//! \brief 把UGVariant保存到流中
	STREAM_APIF UGStream& operator<<(const UGVariant& var);

	//! \brief 从流中读出UGVariant
	STREAM_APIF UGStream& operator>>(UGVariant& var);

private:
	UGint file;
protected:
	virtual STREAM_APIF UGSizeT WriteBuffer(UGSizeT count);
	virtual STREAM_APIF UGSizeT ReadBuffer(UGSizeT count);
	//{{Add by aiguo 主要是解决在UGStreamLoadSave模式下，Load和Save混乱的问题，这里主要是设置一些标志来控制Load和Save.
	virtual STREAM_APIF void SetSignLoadSave(UGint nSign);
	//}}
};

}

#endif


