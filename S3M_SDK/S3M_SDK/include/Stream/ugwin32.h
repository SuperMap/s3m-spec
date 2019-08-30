/* 
	\file ugwin32.h
	\brief Win32平台的头文件定义
	\author zengzm
	\attention
	Copyright(c) 2000-2010 SuperMap Software Co., Ltd.<br>
	All Rights Reserved
	<b>更新说明：。</b>
	<em>1、第一条说明。</em>
	<em>2、第二条说明。</em>
*/

#ifndef _UGWIN32_H
#define _UGWIN32_H

// 2003-5-28-by-malq-base的线程类会使用
#ifndef _WIN32_WINNT		// Allow use of features specific to Windows NT 4 or later.
#define _WIN32_WINNT 0x0600	// Change this to the appropriate value to target Windows 2000 or later.
#endif	

//#undef LoadString
//#undef DeleteFile
//#undef GetCurrentTime

#pragma warning(disable: 4018)  // signed/unsigned mismatch
#pragma warning(disable: 4097)
#pragma warning(disable: 4100)
#pragma warning(disable: 4164)
#pragma warning(disable: 4244)
#pragma warning(disable: 4996)	// security warning about POSIX functions
// 以下几个警告太常见，还是放到ugdefs.h中
//#pragma warning(disable: 4251)
//#pragma warning(disable: 4275)
//#pragma warning(disable: 4786)
#pragma warning(disable: 4514)
#pragma warning(disable: 4663)
//#pragma warning(disable: 4710) // 为内联展开选定了给定函数，但编译器没有执行内联。

// 头文件包含 
#include <io.h>
//#include <wchar.h>
//#include <WinDef.h>
#ifdef _UGUNICODE
	//打开文件
	#define UGopen	_wopen
	#define UGfopen_s _wfopen_s  
	// 字符串长度
// 	#define UGstrlen wcslen
// 	#define UGstrcpy wcscpy
	#define UGaccess _waccess
	#define UGremove _wremove
	#define UGrename _wrename
	#define UGfopen _wfopen

	#define UG_findfirst _wfindfirst
	#define UG_finddata_t _wfinddata_t
	#define UG_findnext _wfindnext
	#define UG_findclose _findclose
	#define UG_stat	_wstat
	#define UG_stati64 _wstati64 
	#define UG_mkdir _wmkdir
	#define UG_rmdir _wrmdir
	#define UG_getcwd _wgetcwd
#else
	//打开文件
	#define UGopen	_open
	// 字符串长度
// 	#define UGstrlen strlen
// 	#define UGstrcpy strcpy
	#define UGaccess _access
	#define UGremove remove
	#define UGrename rename
	#define UGfopen fopen
	#define UGfopen_s fopen_s  
	#define UG_findfirst _findfirst
	#define UG_finddata_t _finddata_t
	#define UG_findnext _findnext
	#define UG_findclose _findclose
	#define UG_stat	_stat
	#define UG_stati64 _stati64 
	#define UG_mkdir _mkdir
	#define UG_rmdir _rmdir
	#define UG_getcwd _getcwd
#endif //_UNICODE

#define UGwrite	_write	// 写文件
#define UGread	_read	// 读文件
#define UGseek	_lseeki64	// 定位文件
#define UGclose	_close	// 关闭文件
#define UGchsize _chsize
#define UGtell _telli64	// 查询文件位置
#define UGsleep Sleep
#define UGfwrite fwrite
#define UGfclose	fclose

#if _MSC_VER> 1200
#define	UGfseek _fseeki64
#else
#define	UGfseek fseek
#endif

		
#define UGfread fread


//是否是binary文件
#define UGBINARY O_BINARY
//是否是read/write文件
#define UGREADWRITE (_S_IREAD|_S_IWRITE)

// 是否是目录
#define UGDIRECTORY _S_IFDIR

//路径sep
#define PATHSEP _U('\\')
//路径sep
#define PATHSEPSTRING _U("\\")
//路径listSep
#define PATHLISTSEP _U(';')
//路径listSep
#define PATHLISTSEPSTRING _U(";")
//是否是路径 sep
#define ISPATHSEP(c) ((c)==_U('/') || (c)==_U('\\'))
//换行符号
#define EOLSTRING _U("\r\n")


#define UGCALLBACK OGDCCALLBACK
#define UGSTDCALL __stdcall

namespace UGC {

// 变量定义
typedef void*         UGThreadID;
typedef unsigned int  UGHandle;

}

#endif

