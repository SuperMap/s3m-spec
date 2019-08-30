/*! \file	 UGErrorObj.h
 *  \brief	 错误对象定义文件
 *  \author	 ugc team
 *  \attention 
 *  Copyright(c) 2000-2010 SuperMap Software Co., Ltd.<br>
 *  All Rights Reserved
 *  \version $Id: UGErrorObj.h,v 1.8 2009/11/05 09:36:46 siyh Exp $
 */

#ifndef UGERROROBJ_H
#define UGERROROBJ_H

#include "Stream/ugdefs.h"
#include "Stream/ugplatform.h"
#include "UGStringEx.h"
//#include "UGLogFile.h"

namespace UGC {

typedef void (*ErrorInfoProc)(const UGchar* pchErrorInfo);

//! \brief 错误对象
class TOOLKIT_API UGErrorObj  
{
	friend class UGLogFile;
public:
	//! 错误信息
	  //!  \brief Toolkit
	struct ErrorInfo
	{
		//! \brief 错误发生所在的线程id
		UGint m_nThreadId;
		//! \brief 编号, 资源字符串id
		UGStringEx m_strID;
		//! \brief 描述
		UGStringEx m_strMessage; 
		//! \brief 文件名
		UGStringEx m_strCodeFile;
		//! \brief 行号
		UGint m_nCodeLine;
		//! \brief 默认构造函数
		ErrorInfo() {
			m_nThreadId = -1; 
			m_nCodeLine = -1;	//! 表示非法值
		}

	//! \brief 。
	  //!  \brief Toolkit
		ErrorInfo(UGuint nThreadId,const UGStringEx strID,UGString strMessage,
			UGString strCodeFile,UGint nCodeLine) 
		{
	  //!  \brief Toolkit
			m_nThreadId = nThreadId;
	  //!  \brief Toolkit
			m_strID = strID;
	  //!  \brief Toolkit
			m_strMessage = strMessage;
	  //!  \brief Toolkit
			m_strCodeFile = strCodeFile;
	  //!  \brief Toolkit
			m_nCodeLine = nCodeLine;
		}
	};

	//! \brief 。
	UGErrorObj();	
private:
	UGErrorObj(const UGErrorObj&);
	UGErrorObj& operator=(const UGErrorObj&);
	
public:	
	//! \brief 。
	~UGErrorObj();

	//! \brief 得到错误对象的唯一实例, 不要直接定义错误对象
	//! \remarks 要想得到错误信息, 必须先调用Startup 或打开日志文件
	static UGErrorObj& GetInstance(); 

	//! \brief 启动错误对象
	//! \remarks 错误对象必须先启动一下，才能使用; 内部使用LogFile实现
	//! \return 成功启动返回true,失败返回false
	UGbool Startup();

	//! \return 返回是否启动
	  
	UGbool IsStartup();

	//! \brief 设置错误处理的回调函数指针
	static void SetErrorInfoProc(ErrorInfoProc pErrorInfoFun);	

	//! \brief 得到错误对象个数
	  
	UGint GetCount() const;
	
	//! \brief 清空错误对象
	 
	void Clear();

	//! \brief 设置容量大小
	 
	void SetCapacity(UGint nCapacity);

	//! \brief 获取容量大小
	
	UGint GetCapacity() const;

	  //!  \brief Toolkit
	ErrorInfo GetAt(UGint nIndex) const;

	//! \brief 得到最后一条错误信息
	//! \param bBythread 是否只考虑当前线程,默认为考虑; 如果为true,则会取出在当前线程中的最后一条错误信息
	//!                  如果为false,则取出所有错误信息中的最后一条
	//! \return 返回最后一条错误信息
	ErrorInfo GetLast(UGbool bBythread=TRUE) const;

	//! \brief 。
	  //!  \brief Toolkit
	void Dump(UGString& strErrorInofs,UGbool bBythread=TRUE) const;

	//! \brief 。
	  //!  \brief Toolkit
	static void Format(const ErrorInfo& errorInfo,UGString& strErrorInfo,UGbool bBythread=TRUE);

private:
	//! \brief 添加错误信息
	void Add(const ErrorInfo& errorInfo);

	//! brief 添加错误信息
	void Add(UGuint nThreadID, UGStringEx strID, UGString m_strCodeFile, UGint m_nCodeLine);

private:

	// by zengzm 2007-10-11 错误对象内部采用日志文件来实现, 这些东东先注释起来
	//! 是否是循环的
//	UGbool m_bCycled;
	//! 下一个填充位置
//	UGint m_nPos;
	//! 存储错误信息
//	UGArray<ErrorInfo> m_ErrorObjs;	

	
	//! \brief 错误信息回调函数指针
	static ErrorInfoProc ms_pErrorInfoFun;
};

}

#endif


