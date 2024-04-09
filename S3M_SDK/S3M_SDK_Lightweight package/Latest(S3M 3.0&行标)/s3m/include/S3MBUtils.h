#if !defined(S3MBUTILS_H__40994DD6_DF74_45DE_85F9_145FABBE2A9A__INCLUDED_)
#define S3MBUTILS_H__40994DD6_DF74_45DE_85F9_145FABBE2A9A__INCLUDED_

#pragma once
#include "stdafx.h"
#include "S3MBCommon.h"
#include <vector>
#include <string>

namespace S3MB
{
	class S3MB_API Utils
	{
	public:
		static bool Zip(unsigned char* pvDestBuffer, unsigned int& dwDestLen,
			const unsigned char* pvSrcBuffer, unsigned int dwSrcLen, int nLevel = 8);

		static bool UnZip(unsigned char* pvDestBuffer, unsigned int& dwDestLen,
			const unsigned char* pvSrcBuffer, unsigned int dwSrcLen);

		static bool UnZip(unsigned char*& pvDestBuffer, unsigned long& dwDestLen,
			const unsigned char* pvSrcBuffer, unsigned long dwSrcLen);
	};

	class S3MB_API StringUtil
	{
	public:
		// 将字符串中小写字符转化为大写字符
		static wstring StrToUpper(wstring str);

		// 判断字符串是否相同
		static bool CompareNoCase(const wstring& strA, const wstring& strB);

		// 切割字符串
		static void Split(const wstring& s, vector<wstring>&arrStr, const wchar_t flag = ' ');
		static void Split(const string& s, vector<string>& arrStr, const char flag = ' ');

		// 去除左边指定字符串
		static wstring TrimLeft(wstring& str, wchar_t* pStr = NULL);

		// 去除右边指定字符串
		static wstring TrimRight(wstring& str, wchar_t* pStr = NULL);
		static wstring TrimRight(wstring& str, wchar_t ch);

		// unicode编码转化为utf8编码
		static std::string UNICODE_to_UTF8(const wstring& str);

		// utf8编码转化为unicode编码
		static std::wstring UTF8_to_UNICODE(const string& str);

		// ansi编码转化为utf8编码
		static std::string ANSI_to_UTF8(const string& str);

		// utf8编码转化为ansi编码
		static std::string UTF8_to_ANSI(const string& str);

		// 获取字符串长度
		static int GetStrLength(const wchar_t* pStr);
		// 格式化字符串
		static std::wstring Format(const wchar_t* pStr, ...);
		// 取出字符串对象最右边的指定长度的子字符串
		// nCount 字节个数
		// return 返回右边指定长度的子字符串
		static wstring Right(const wstring& str, int nCount);

		// 取出字符串对象最左边的指定长度的子字符串
		// nCount 字节个数
		// return 返回左边指定长度的子字符串
		static wstring Left(const wstring& str, int nCount);
		static wstring From(long lVal, unsigned int base = 10);
		static wstring From(unsigned long lVal, unsigned int base = 10);

		// 获取文件所在文件夹
		static wstring GetDir(wstring filePath);

		// 获取文件绝对路径
		static wstring GetAbsolutePath(wstring strSrcPath, wstring path);
		static wstring GetAbsolutePath(wstring relatPath);

		// 获取文件相对路径
		static std::wstring GetRelativePath(std::wstring strSrcPath, std::wstring strAbsPath);

		// 从文件全路径中得到文件名
		static std::wstring GetName(std::wstring strFilePath);

		// 从文件全路径中得到文件主题名(不带后缀名)
		static std::wstring GetTitle(const std::wstring& strFilePath);

		// 改变文件后缀
		static wstring ChangeExt(wstring path, wstring ext);
		// 获取文件后缀
		static wstring GetExt(wstring filePath);
		// 判断文件是否存在
		static bool IsFileExist(wstring path);
#ifndef WIN32
        static std::string wchar_to_char(const wchar_t* str);
        static std::wstring ANSIToUnicode(const string& str);
        static std::string UnicodeToANSI(const wstring& str);
        static int GetModuleFileName(void* Fun, char* &sFilePath);
#endif
	};

	class S3MB_API FileUtil
	{
	public:
		// 创建多级文件夹
		static bool MkDirEx(const std::wstring& strDir);
		// 创建文件夹
		static bool MkDir(const std::wstring& strDir);
	};
}

#endif
