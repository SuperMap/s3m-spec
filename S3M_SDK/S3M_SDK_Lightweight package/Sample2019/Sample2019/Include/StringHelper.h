#pragma once
#include <string>
//#include <stringapiset.h>
#include <stdarg.h>
#include <algorithm>
#include <sstream>
#include <vector>
//#include <winnls.h>
using namespace std;

namespace S3MB
{
	class StringHelper
	{
	public:
		//将字符串中小写字符转化为大写字符
		static string StrToUpper(string str);

		//判断字符串是否相同
		static bool CompareNoCase(const string& strA, const string& strB);

		//切割字符串
		static void Split(const string& s, vector<string>arrStr, const char flag = ' ');

		//去除左边指定字符串
		static string TrimLeft(string& str, char* pStr = NULL);

		//去除右边指定字符串
		static string TrimRight(string& str, char* pStr = NULL);
		static string TrimRight(string& str, char ch);

		//ansi编码转化为utf8编码
		static std::string ANSI_to_UTF8(const string& str);

		//utf8编码转化为ansi编码
		static std::string UTF8_to_ANSI(const string& str);

		//获取字符串长度
		static int GetStrLength(const char* pStr);

		// 格式化字符串
		static std::string Format(const char* pStr, ...);

		//! \brief 取出字符串对象最右边的指定长度的子字符串
		//! \param nCount 字节个数
		//! \return 返回右边指定长度的子字符串
		//! \include str_right.cpp
		static string Right(const string& str, int nCount);

		//! \brief 取出字符串对象最左边的指定长度的子字符串
		//! \param nCount 字节个数
		//! \return 返回左边指定长度的子字符串
		//! \include str_right.cpp
		static string Left(const string& str, int nCount);
		static string From(long lVal, unsigned int base = 10);
		static string From(unsigned long lVal, unsigned int base = 10);

		//获取文件所在文件夹
		static string GetDir(string filePath);

		//获取文件绝对路径
		static string GetAbsolutePath(string strSrcPath, string path);
		static string GetAbsolutePath(string relatPath);

		//改变文件后缀
		static string ChangeExt(string path, string ext);
		//获取文件后缀
		static string GetExt(string filePath);
		//判断文件是否存在
		static bool IsFileExist(string path);

	};

	class FileType
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
		static EmType ExtNameToFileType(const string& strExtName);

		//! \brief 根据文件类型得到后缀名。
		//! \param eFileType [in] 文件格式类型。
		//! \return 带.的后缀名。
		static string FileTypeToExtName(EmType eFileType);
	};
}
