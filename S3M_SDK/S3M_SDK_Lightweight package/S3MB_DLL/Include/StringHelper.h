#pragma once
//#include <stringapiset.h>
#include <stdarg.h>
#include <algorithm>
#include <sstream>
#include <vector>
//#include <winnls.h>
#include "stdafx.h"
#include <string>
using namespace std;

#define U(x)  L ## x

namespace S3MB
{
	class S3MB_API StringHelper
	{
	public:
		//将字符串中小写字符转化为大写字符
		static wstring StrToUpper(wstring str);

		//判断字符串是否相同
		static bool CompareNoCase(const wstring& strA, const wstring& strB);

		//切割字符串
		static void Split(const wstring& s, vector<wstring>&arrStr, const wchar_t flag = ' ');
		static void Split(const string& s, vector<string>& arrStr, const char flag = ' ');

		//去除左边指定字符串
		static wstring TrimLeft(wstring& str, wchar_t* pStr = NULL);

		//去除右边指定字符串
		static wstring TrimRight(wstring& str, wchar_t* pStr = NULL);
		static wstring TrimRight(wstring& str, wchar_t ch);

		//unicode编码转化为utf8编码
		static std::string UNICODE_to_UTF8(const wstring& str);

		//utf8编码转化为unicode编码
		static std::wstring UTF8_to_UNICODE(const string& str);

		//ansi编码转化为utf8编码
		static std::string ANSI_to_UTF8(const string& str);

		//utf8编码转化为ansi编码
		static std::string UTF8_to_ANSI(const string& str);

		//获取字符串长度
		static int GetStrLength(const wchar_t* pStr);
		// 格式化字符串
		static std::wstring Format(const wchar_t* pStr, ...);
		//! \brief 取出字符串对象最右边的指定长度的子字符串
		//! \param nCount 字节个数
		//! \return 返回右边指定长度的子字符串
		//! \include str_right.cpp
		static wstring Right(const wstring& str, int nCount);

		//! \brief 取出字符串对象最左边的指定长度的子字符串
		//! \param nCount 字节个数
		//! \return 返回左边指定长度的子字符串
		//! \include str_right.cpp
		static wstring Left(const wstring& str, int nCount);
		static wstring From(long lVal, unsigned int base = 10);
		static wstring From(unsigned long lVal, unsigned int base = 10);

		//获取文件所在文件夹
		static wstring GetDir(wstring filePath);

		//获取文件绝对路径
		static wstring GetAbsolutePath(wstring strSrcPath, wstring path);
		static wstring GetAbsolutePath(wstring relatPath);

		//改变文件后缀
		static wstring ChangeExt(wstring path, wstring ext);
		//获取文件后缀
		static wstring GetExt(wstring filePath);
		//判断文件是否存在
		static bool IsFileExist(wstring path);

	};

	class S3MB_API FileType
	{
	public:
		enum EmType
		{
			//! \brief 未知数据格式。
			none = 0,
			//! \brief Tiff/BigTIFF/GeoTIFF文件。
			GTiff = 103,
			//! \brief bitmap文件。
			BMP = 121,
			//! \brief jpg文件。
			JPG = 122,
			//! \brief portal network graphic(PNG)。
			PNG = 123,
			//! \brief gif文件。
			GIF = 124,
			//! \brief tga文件。
			TGA = 125,
		};
		//! \brief 根据后缀名得到文件类型。
		//! \param strExtName [in] 带.的后缀名。
		//! \return 文件格式类型。
		static EmType ExtNameToFileType(const wstring& strExtName);

		//! \brief 根据文件类型得到后缀名。
		//! \param eFileType [in] 文件格式类型。
		//! \return 带.的后缀名。
		static wstring FileTypeToExtName(EmType eFileType);
	};
}
