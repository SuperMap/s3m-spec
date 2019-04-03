
/*!
    **************************************************************************************
     \file     UGFileStdio.h
    **************************************************************************************
     \author   艾国
     \brief    文本文件类                   
     \remarks                                                                          <br>
    ----------------------------------------------------------------------------------<br>
     Revision History :                                                               <br>  
     2005-04-28  艾国    Initial Version.                                           <br>
                                                                                      <br>   
    ----------------------------------------------------------------------------------<br>
     Copyright (c) 1996-2004 SuperMap GIS Technologies,Inc.                           <br>
    ----------------------------------------------------------------------------------<br>

    **************************************************************************************
*/


#if !defined(AFX_UGFILESTDIO_H__93B98C9C_498A_43C5_8D3D_F4D4D3FD5FC5__INCLUDED_)
#define AFX_UGFILESTDIO_H__93B98C9C_498A_43C5_8D3D_F4D4D3FD5FC5__INCLUDED_

#include "Stream/UGFileStream.h"

namespace UGC{

//!  \brief Toolkit
//const static unsigned char UDes_key[17] = "~@!#(*&)_OP%#_*!";	
//!  \brief Toolkit
//const static int UDes_key_length = 16;

class STREAM_API UGFileStdio  
{
public:
	
	//! \brief 文件换行符类型
	enum EnterType
	{
		Enter_Adapt = 1,   //当前的根据系统
		Enter_Windows = 2, //Windows上的换行符
		Enter_Unix = 3    //Linux和Unix上的换行符
		//Enter_Other = 4    //其他的系统，现在不知道，如WinCE等，以后可以扩展
	};


	UGFileStdio();
	~UGFileStdio();

public:

	//! \brief 		打开文本文件
	//! \return 	UGbool
	//! \param 		filename
	//! \param 		save_or_load
	//! \param 		size=8192
	//! \param      在Unicode版本中，按照指定编码读写文件，如果文件头为Unicode，则指定无效
	//! \remarks 
	UGbool Open(const UGchar* filename,UGStreamDirection save_or_load,UGuint size=8192,UGString::Charset charset = UGString::Default);

	//! \brief 是否在文件未
	UGbool IsEOF() const;
	//! \brief 设置操作系统换行符
	void SetCurEnterType(EnterType eEnterType);
	//! \brief 得到文件当前的位置
	UGulong GetPosition() const;
	//! \brief 设置文件的位置
	UGbool SetPosition(UGlong offset,UGWhence whence=UGFromStart);
	//! \brief 得到文件的长度
	UGlong GetLength();

	//! \brief 写一行文本到文件
	//! \remarks 写入一行数据时会自动添加行未的换行符
	UGbool WriteLine(const UGString& strSource);
    // add by cuiwz 2008-9-19 增加方法直接
	//! \brief 直接写入到文件中
    void Write(const UGString& strSource);
	
	// added by zengzm 2005.12.9 增加flush方法，把没有写入文件的数据写入
	//! \brief 把数据即时写入到文件中
	UGbool Flush();

	//! \brief 关闭文件
	UGbool Close();
	UGbool WriteLine(const UGachar *pSrc,UGint nLength);
	void Write(const UGachar *pSrc,UGint nLength);
	//! \brief  从文件中读取一行文本
	//! \remarks 读取一行数据时会自动去掉行未的换行符，这里自动识别行未的换行符，用户只管放心读
	UGint ReadLine(UGMBString& strSource);
#ifdef _UGUNICODE
	UGString::Charset GetCharset(){return m_eCharset;};
#else
	UGString::Charset GetCharset(){return UGString::Default;};
#endif
#ifdef _UGUNICODE
	UGint ReadLine(UGString& strSourceIn);
private:
	UGMBString::Charset m_eCharset;
	UGint m_nStartPos;
#endif
private:
	UGFileStream m_FileStream;  //文件流对象
	EnterType m_eEnterType;		//回车符类型,各个系统都不一样

// modified by cuiwz 2008-9-18 这些最好改为公有，这样其它文件可以使
};


} //namespace UGC
#endif // !defined(AFX_UGFILESTDIO_H__93B98C9C_498A_43C5_8D3D_F4D4D3FD5FC5__INCLUDED_)

