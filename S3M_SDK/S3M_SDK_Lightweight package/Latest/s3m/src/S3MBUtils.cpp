#include "S3MBUtils.h"
#include "zlib/include/zlib.h"
#include <assert.h>
#ifdef WIN32
#include <atlstr.h>
#include <codecvt>
#include <io.h>
#else
#include <dlfcn.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#endif
#include <fstream>
#include <regex>
#include <stdarg.h>
#include <vector>
#include <locale>
#include <string.h>
#include <iostream>
#include <regex>
#include <sstream>

namespace S3MB
{
	bool Utils::Zip(unsigned char* pvDestBuffer, unsigned int& dwDestLen, const unsigned char* pvSrcBuffer, unsigned int dwSrcLen, int nLevel /*= 8*/)
	{
		unsigned long ulDestLen = dwDestLen;
		bool bResult = false;
		int nResult = compress2(pvDestBuffer, &ulDestLen, pvSrcBuffer, dwSrcLen, nLevel);
		dwDestLen = (unsigned int)ulDestLen;

		// 防错处理
		switch (nResult)
		{
		case Z_OK:
			bResult = true;
			break;
		case Z_MEM_ERROR:
			wcout << U("内存分配不足") << endl;
			break;
		case Z_BUF_ERROR:
			wcout << U("输出缓存没有足够的空间") << endl;
			break;
		case Z_STREAM_ERROR:
			wcout << U("压缩文件参数非法") << endl;
			break;
		case Z_DATA_ERROR:
			wcout << U("数据错误") << endl;
			break;
		default:
			// 不可识别的错误，
			wcout << U("不可识别的错误！") << endl;
			break;
		}
		return bResult;
	}

	bool Utils::UnZip(unsigned char* pvDestBuffer, unsigned int& dwDestLen,
		const unsigned char* pvSrcBuffer, unsigned int dwSrcLen)
	{
		unsigned long ulDestLen = dwDestLen;
		bool bResult = false;
		int nResult = uncompress(pvDestBuffer, &ulDestLen, pvSrcBuffer, dwSrcLen);
		dwDestLen = (unsigned int)ulDestLen;

		// 防错处理
		switch (nResult)
		{
		case Z_OK:
			bResult = true;
			break;
		case Z_MEM_ERROR:
			wcout << "内存分配不足" << endl;
			break;
		case Z_BUF_ERROR:
			wcout << "输出缓存没有足够的空间" << endl;
			break;
		case Z_STREAM_ERROR:
			wcout << "压缩文件参数非法" << endl;
			break;
		case Z_DATA_ERROR:	
			wcout << "数据错误" << endl;
			break;
		default:
			// 不可识别的错误，
			wcout << "不可识别的错误！" << endl;
			break;
		}
		return bResult;
	}

	bool Utils::UnZip(unsigned char*& pvDestBuffer, unsigned long& dwDestLen,
		const unsigned char* pvSrcBuffer, unsigned long dwSrcLen)
	{
		bool bResult = false;
		while (!bResult)
		{
			int nResult = uncompress(pvDestBuffer, &dwDestLen, pvSrcBuffer, dwSrcLen);// dwDestLen的大小会改变
			// 防错处理
			switch (nResult)
			{
			case Z_OK:
				bResult = true;
				break;
			case Z_MEM_ERROR:
				cout << "内存分配不足" << endl;
				break;
			case Z_BUF_ERROR:
				// cout << "输出缓存没有足够的空间" << endl;
				break;
			case Z_STREAM_ERROR:
				cout << "压缩文件参数非法" << endl;
				break;
			case Z_DATA_ERROR:
				cout << "数据错误" << endl;
				break;
			default:
				// 不可识别的错误，
				cout << "不可识别的错误！" << endl;
				break;
			}

			if (!bResult)// 如果解压失败.
			{
				if (nResult != Z_BUF_ERROR)
				{
					cout << "解压失败！正在重试..." << endl;
				}
				delete[] pvDestBuffer;
				dwDestLen = dwDestLen * 4;
				pvDestBuffer = new(std::nothrow) unsigned char[dwDestLen];
				continue;
			}
		}
		return bResult;
	}

	std::wstring StringUtil::StrToUpper(wstring str)
	{
		wstring strTmp = str;
		transform(strTmp.begin(), strTmp.end(), strTmp.begin(), towupper);
		if (strTmp.size() == 0)
		{
			return str;
		}
		return strTmp;
	}

	bool StringUtil::CompareNoCase(const wstring& strA, const wstring& strB)
	{
		if (strA == strB)
		{
			return true;
		}
#ifdef WIN32
		wstring str1 = StrToUpper(strA);
		wstring str2 = StrToUpper(strB);
        return (str1 == str2);
#else
        return (strA == strB);
#endif
	}

	void StringUtil::Split(const wstring& s, vector<wstring>&arrStr, const wchar_t flag /*= ' '*/)
	{
		std::wstringstream ss;
		ss.str(s);
		std::wstring item;
		while (std::getline(ss, item, flag))
		{
			arrStr.push_back(item);
		}
	}

	void StringUtil::Split(const string& s, vector<string>& arrStr, const char flag /*= ' '*/)
	{
		std::stringstream ss;
		ss.str(s);
		std::string item;
		while (std::getline(ss, item, flag))
		{
			arrStr.push_back(item);
		}
	}

	std::wstring StringUtil::TrimLeft(wstring& str, wchar_t* pStr /*= NULL*/)
	{
		if (pStr == NULL)
		{
			wchar_t tmp[5] = { L'\r',L'\t',L'\n',L'\0',L' ' };
			pStr = tmp;
		}
		const wchar_t* pBuffer = str.c_str();
		int nLength = str.length();
		int nCount = 0;
		int i = 0;
		bool bFind;
		int j;
		for (i = 0; i < nLength; ++i)
		{
			bFind = false;
			j = 0;
			wchar_t wchTemp = pStr[j];
			while (wchTemp != 0)
			{
				if (wchTemp == pBuffer[i])
				{
					++nCount;
					bFind = true;
					break;
				}
				wchTemp = pStr[++j];
			}

			if (!bFind)
			{
				break;
			}
		}
#ifdef WIN32
        str = str.substr(0, nCount);
#else
        str = str.erase(nCount, str.size());
#endif
		return str;
	}

	std::wstring StringUtil::TrimRight(wstring& str, wchar_t* pStr /*= NULL*/)
	{
		if (pStr == NULL)
		{
			wchar_t tmp[5] = { '\r','\t','\n','\0',' ' };
			pStr = tmp;
		}
		const wchar_t* pBuffer = str.c_str();
		int nLength = str.length();
		int nCount = 0;
		int i = 0;
		int j;
		bool bFind;
		for (i = 0; i < nLength; ++i)
		{
			j = 0;
			bFind = false;
			wchar_t wchTemp = pStr[j];
			while (wchTemp != 0)
			{
				if (wchTemp == pBuffer[nLength - i - 1])
				{
					++nCount;
					bFind = true;
					break;
				}
				wchTemp = pStr[++j];
			}

			if (!bFind)
			{
				break;
			}
		}
#ifdef WIN32
        str = str.substr(nLength - nCount, nLength);
#else
        str = str.erase(0, nLength - nCount -1);
        str = str.erase(nLength, str.size());
#endif
		return str;
	}
	std::wstring StringUtil::TrimRight(wstring& str, wchar_t ch)
	{
		wchar_t* pBuffer = (wchar_t*)str.c_str();
		int nLength = str.length();
		int nCount = 0;
		int i = 0;
		bool bFind;
		for (i = nLength - 1; i >= 0; --i)
		{
			bFind = false;
			if (ch == pBuffer[i])
			{
				++nCount;
				bFind = true;
			}

			if (!bFind)
			{
				break;
			}
		}
#ifdef WIN32
        str = str.substr(0, nLength - nCount);
#else
        str = str.erase(nLength - nCount, str.size());
#endif
		return str;
	}

	std::string StringUtil::UNICODE_to_UTF8(const wstring& str)
	{
#ifdef WIN32
		std::wstring_convert<std::codecvt_utf8<wchar_t>>strCnv;
		return strCnv.to_bytes(str);
#else
		return wchar_to_char(str.c_str());
#endif
	}

	std::wstring StringUtil::UTF8_to_UNICODE(const string& str)
	{
#ifdef WIN32
		std::wstring_convert<std::codecvt_utf8<wchar_t>>strCnv;
		return strCnv.from_bytes(str);
#else
		return U(str);
#endif
	}

	std::string StringUtil::ANSI_to_UTF8(const string& str)
	{
#ifdef WIN32
		int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
		wchar_t* pwBuf = new wchar_t[nwLen + 1];
		memset(pwBuf, 0, nwLen * 2 + 2);

		::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, pwBuf, nwLen);

		int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, 0, NULL, NULL);
		char* pBuf = new char[nLen + 1];
		memset(pBuf, 0, nLen + 1);

		::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, pBuf, nLen, NULL, NULL);
		std::string retStr(pBuf);

		delete[]pBuf;
		delete[]pwBuf;

		pBuf = NULL;
		pwBuf = NULL;

		return retStr;
#else
        return UNICODE_to_UTF8(ANSIToUnicode(str));
#endif
	}

	std::string StringUtil::UTF8_to_ANSI(const string& str)
	{
#ifdef WIN32
		int nwLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
		wchar_t* pwBuf = new wchar_t[nwLen + 1];
		memset(pwBuf, 0, nwLen * 2 + 2);

		MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), pwBuf, nwLen);

		int nLen = WideCharToMultiByte(CP_ACP, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

		char* pBuf = new char[nLen + 1];
		memset(pBuf, 0, nLen + 1);

		WideCharToMultiByte(CP_ACP, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

		string retStr = pBuf;
		delete[]pBuf;
		delete[]pwBuf;

		pBuf = NULL;
		pwBuf = NULL;

		return retStr;
#else
        return UnicodeToANSI(UTF8_to_UNICODE(str));
#endif
	}

#ifndef WIN32
    std::wstring StringUtil::ANSIToUnicode(const string& str)
    {
        std::wstring ret;
        std::mbstate_t state = {};
        const char* temp_str = str.c_str();
        size_t len = std::mbsrtowcs(nullptr, &temp_str, 0, &state);
        if(static_cast<size_t>(-1) != len){
            std::unique_ptr<wchar_t[]> buff(new wchar_t[len+1]);
            len = std::mbsrtowcs(buff.get(), &temp_str, len, &state);
            if(static_cast<size_t>(-1) != len){
                ret.assign(buff.get(), len);
            }
        }
        return ret;
    }

    std::string StringUtil::UnicodeToANSI(const wstring &str)
    {
        std::string ret;
        std::mbstate_t state = {};
        const wchar_t* temp_str = str.c_str();
        size_t len = std::wcsrtombs(nullptr, &temp_str, 0, &state);
        if(static_cast<size_t>(-1) != len){
            std::unique_ptr<char[]> buff(new char[len+1]);
            len = std::wcsrtombs(buff.get(), &temp_str, len, &state);
            if(static_cast<size_t>(-1) != len){
                ret.assign(buff.get(), len);
            }
        }
        return ret;
    }

    std::string StringUtil::wchar_to_char(const wchar_t* str)
    {
        int len = 0;
        int ret = 0;

        len = wcslen(str);
        char result[len];

        ret = wcstombs(result, str, len*sizeof(wchar_t));
        std::string charResult(result);
        return charResult;
    }

    int StringUtil::GetModuleFileName(void* Fun, char* &sFilePath)
    {
        int ret = -1;
        Dl_info dl_info;
        if(dladdr(Fun, &dl_info))
        {
            ret = 0;
            sFilePath = strdup(dl_info.dli_fname);
            char* pName = strrchr(sFilePath, '/');
            *pName = '\0';
        }
        return ret;
    }
#endif

	int StringUtil::GetStrLength(const wchar_t* pStr)
	{
		const wchar_t* eos = pStr;

		while (*eos++);

		return((int)(eos - pStr - 1));
	}

	std::wstring StringUtil::Format(const wchar_t* pStr, ...)
	{
		if (pStr && *pStr)
		{
			int len = 0;
			unsigned int nCapacity = GetStrLength(pStr);
			nCapacity = nCapacity + 512;

			va_list args;
			va_start(args, pStr);
			wchar_t* CStr = new wchar_t[nCapacity];
#ifdef WIN32
			wvsprintf(CStr, pStr, args);
#else
			string strFormat = StringUtil::wchar_to_char(pStr);
            char* temp = new char[nCapacity*2];
            vsprintf(temp, strFormat.c_str(), args);
            string temp_char(temp);
            CStr = (wchar_t*)(string_to_wstring(temp_char).c_str());
#endif
			va_end(args);
			wstring str(CStr);
			return str;
		}
		return L"";
	}

	std::wstring StringUtil::Right(const wstring& str, int nCount)
	{
		if (nCount > str.length())
		{
			nCount = str.length();
		}
		if (nCount > -1)
		{
			wstring newStr((const wchar_t*)(str.c_str() + str.length() - nCount), nCount);
			return newStr;
		}
		else
		{
			return str;
		}
	}

	std::wstring StringUtil::Left(const wstring& str, int nCount)
	{
		if (nCount > str.length())
		{
			nCount = str.length();
		}
		if (nCount > -1)
		{
			wstring newStr((const wchar_t*)str.c_str(), nCount);
			return newStr;
		}
		else
		{
			return str;
		}
	}

	std::wstring StringUtil::From(long lVal, unsigned int base /*= 10*/)
	{
		bool bNegative = false; // 记录是否为负数
		if (base == 10 && lVal < 0)
		{
			bNegative = true;
			lVal = -lVal;
		}
		wstring strResult = From((unsigned long)lVal, base);
		if (bNegative)
		{
			strResult = wstring(U("-") + strResult); // 十进制下的负数,前面要加-
		}
		return strResult;
	}

	std::wstring StringUtil::From(unsigned long lVal, unsigned int base /*= 10*/)
	{
		wstring strResult;
		do {
			unsigned int nNum = 0;
			nNum = (unsigned int)(lVal % base);
			lVal /= base;
			if (nNum > 9)
			{ // 十六进制
				strResult += wchar_t(nNum - 10 + L'A');
			}
			else
			{ // 十进制
				strResult += wchar_t(nNum + L'0');
			}
		} while (lVal > 0);
		// 最后反转结果
		reverse(strResult.begin(), strResult.end());
		if (base == 16)
		{
			strResult = U("0X") + strResult; // 加0X,以便使用
		}
		return strResult;
	}

	std::wstring StringUtil::GetDir(wstring filePath)
	{
		int n = 0, i = 0;
		if (!filePath.empty())
		{
			i = 0;
#ifdef WIN32
			if (isalpha((unsigned char)filePath[0]) && filePath[1] == ':')
			{
				i = 2;
				n = i;
			}
#endif

			int nLen = filePath.size();
			while (i < nLen)
			{
				if (ISPATHSEP(filePath[i]))
				{
					n = i;
				}
				i++;
			}
			if (n != 0 || ISPATHSEP(filePath[0]))
			{
				// 剔除没有路径信息的情况
				return std::wstring(filePath.c_str(), n + 1);
			}
		}
		return U("");
	}

	std::wstring StringUtil::GetAbsolutePath(wstring strSrcPath, wstring path)
	{
#ifdef WIN32
		replace(strSrcPath.begin(), strSrcPath.end(), U('\\'), U('/'));
		replace(path.begin(), path.end(), U('\\'), U('/'));
#endif
        if (strSrcPath.at(strSrcPath.size() - 1) == L'/')
		{
#ifdef WIN32
            strSrcPath = strSrcPath.substr(0, strSrcPath.size() - 1);
#else
            strSrcPath = strSrcPath.erase(strSrcPath.size() - 1, strSrcPath.size());
#endif
		}
		if (StringUtil::Left(path, 2) == U("./"))
		{
#ifdef WIN32
            return strSrcPath + U("/") + path.substr(2, path.size() - 2);
#else
            return strSrcPath + path.erase(0, 1);
#endif
		}
        else if (path.size() >= 2 && path[1] == L':')
		{
			return path;
		}
        else if (path[0] == L'/')
		{
			return path;
		}
		while (StringUtil::Left(path, 3) == U("../"))
		{
#ifdef WIN32
            path = path.substr(3, path.size() - 3);
            strSrcPath = strSrcPath.substr(0, strSrcPath.find_last_of(U("/")));
#else
            path = path.erase(0, 2);
            strSrcPath = strSrcPath.erase(strSrcPath.find_last_of(U("/"), strSrcPath.size()));
#endif
		}
        return strSrcPath + U("/") + path;
	}

	wstring StringUtil::GetAbsolutePath(wstring relatPath)
	{
		// MAX_PATH = 260 
		wchar_t exeFullPath[260];
		memset(exeFullPath, 0, 260);
#ifdef WIN32
        GetModuleFileName(NULL, exeFullPath, 260);
        wstring filePath(exeFullPath);
        wstring fileDir = GetDir(filePath);
        return GetAbsolutePath(fileDir, relatPath);
#else
        char* exePath;
        void* fun = (void*)"main";
        GetModuleFileName(fun, exePath);

        string temp_exePath(exePath);
        wstring filePath = string_to_wstring(temp_exePath);
        return GetAbsolutePath(filePath, relatPath);
#endif
	}

	std::wstring StringUtil::GetRelativePath(std::wstring strSrcPath, std::wstring strAbsPath)
	{
		if (strSrcPath.length() == 0)
		{
			return strAbsPath;
		}

#ifdef WIN32
		replace(strSrcPath.begin(), strSrcPath.end(), U('\\'), U('/'));
		replace(strAbsPath.begin(), strAbsPath.end(), U('\\'), U('/'));
#endif

		int nFlagSrc = strSrcPath.find(U(":"));
		int nFlagAbs = strAbsPath.find(U(":"));
		if (nFlagSrc < strSrcPath.size() && nFlagAbs < strAbsPath.size())
		{
			std::vector<std::wstring> vecSrcPath, vecAbsPath;
#ifndef WIN32
			const wchar_t* wstrSymbol = U(":").c_str();
			Split(strSrcPath, vecSrcPath, *wstrSymbol);
			Split(strAbsPath, vecAbsPath, *wstrSymbol);
#else
			Split(strSrcPath, vecSrcPath, U(':'));
			Split(strAbsPath, vecAbsPath, U(':'));
#endif


			if (CompareNoCase(vecSrcPath[0], vecAbsPath[0]))
			{
				strSrcPath = vecSrcPath[1];
				strAbsPath = vecAbsPath[1];
			}
		}

		// src = /a/b/c/, abs = /a/b/c/,     => relative=./
		if (strSrcPath == strAbsPath)
		{
			return U("./");
		}
		// src = /a/b/c/, abs = /a/b/c/d/e/, => relative=./d/e/
		else if (strAbsPath.find(strSrcPath) < strAbsPath.size())
		{
			size_t size = strSrcPath.size();
			return U("./") + strAbsPath.substr(size);
		}
		// src = /a/b/c/d/e/, abs = /a/b/c/, => relative=../../
		else if (strSrcPath.find(strAbsPath) < strSrcPath.size())
		{
			size_t size = strAbsPath.size();
			std::wstring strSub = strSrcPath.substr(size);
			std::wstring strSep = U("/");
			std::wstring strRelativePath;
			size_t subSize = strSub.size();
			size_t sepSize = strSep.size();
			size_t pos = 0, index = 0;
			while ((index = strSub.find(strSep, pos)) < subSize)
			{
				strRelativePath += U("../");
				pos = index + sepSize;
			}
			return strRelativePath;
		}
		// src = /a/b/c/d/e/, abs = /f/g/, => relative=../../../../../f/g/
		else
		{
			size_t index = 0;
			for (; index < strSrcPath.size(); index++)
			{
				if (strSrcPath[index] != strAbsPath[index])
				{
					break;
				}
			}

			std::wstring strSame = strSrcPath.substr(0, index);
			size_t pos = strSame.rfind(U("/"));
			if (pos < strSame.size())
			{
				std::wstring strSrcRest = strSrcPath.substr(pos + 1);
				std::wstring strAbsRest = strAbsPath.substr(pos + 1);

				std::wstring strSep = U("/");
				std::wstring strRelativePath;
				size_t restSize = strSrcRest.size();
				size_t sepSize = strSep.size();
				size_t pos = 0;
				while ((index = strSrcRest.find(strSep, pos)) < restSize)
				{
					strRelativePath += U("../");
					pos = index + sepSize;
				}
				strRelativePath += strAbsRest;
				return strRelativePath;
			}
			else
			{
				return strAbsPath;
			}
		}
	}

	std::wstring StringUtil::GetName(std::wstring strFilePath)
	{
		int f = 0, n = 0;
		if (strFilePath.length() != 0)
		{
#ifdef WIN32
			if (isalpha((unsigned char)strFilePath[0]) && strFilePath[1] == ':')
			{
				n = 2;
			}
#endif
			f = n;
			int size = strFilePath.size();
			while (n < size)
			{
				if (ISPATHSEP(strFilePath[n]))
				{
					f = n + 1;
				}
				n++;
			}
			return strFilePath.substr(f, n-f);
		}
		return U("");
	}

	std::wstring StringUtil::GetTitle(const std::wstring& strFilePath)
	{
		int f = 0, e = 0, b = 0, i = 0;
		if (strFilePath.length() != 0)
		{
			i = 0;
#ifdef WIN32
			if (isalpha((unsigned char)strFilePath[0]) && strFilePath[1] == ':')
			{
				i = 2;
			}
#endif
			f = i;
			int nLen = strFilePath.length();
			while (i < nLen)
			{
				if (ISPATHSEP(strFilePath[i]))
				{
					f = i + 1;
				}
				i++;
			}
			b = f;
			if (b >= nLen)
			{
				return U("");
			}

			if (strFilePath[b] == '.')
			{
				b++;
			}
			e = i;
			while (b < i)
			{
				if (strFilePath[--i] == '.')
				{
					e = i;
					break;
				}
			}
			return strFilePath.substr(f, e - f);
		}
		return U("");
	}

	std::wstring StringUtil::ChangeExt(wstring path, wstring ext)
	{
		if (find(path.begin(), path.end(), L'.') == path.end())
		{
			return path + ext;
		}
		else
		{
#ifdef WIN32
            return path.substr(0, path.find_last_of(U("."))) + ext;
#else
            return path.erase(path.find_last_of(U("."), path.size())) + ext;
#endif
		}
	}

	std::wstring StringUtil::GetExt(wstring filePath)
	{
#ifdef WIN32
        int index = filePath.find_last_of(L'.');
        return filePath.substr(index, filePath.size() - index);
#else
        int index = filePath.find_last_of(U("."));
        wstring tempPath = filePath.erase(0, index);
        return tempPath;
#endif
	}

	bool StringUtil::IsFileExist(wstring path)
	{
#ifdef WIN32
        int result = _waccess(path.c_str(), 0);
#else
        string tempPath = UnicodeToANSI(path);
        int result = access(tempPath.c_str(), 0);
#endif
		if (result == 0)
		{
			return true;
		}
		return false;
	}

	bool FileUtil::MkDirEx(const std::wstring& strDir)
	{

		std::wstring strTempDir = strDir;
		std::vector<std::wstring> vecStr;
#ifndef WIN32
		const wchar_t* tempStr = U("/").c_str();
		StringUtil::Split(strTempDir, vecStr, *tempStr);
		bool bAbsolutePath = false;
		if (strDir.length() != 0 && strDir[0] == L'/')
		{
			bAbsolutePath = true;
		}
		if (bAbsolutePath && vecStr.size() >= 1)
		{
			vecStr[0] = (U("/") + vecStr[0]);
		}
#else
		replace(strTempDir.begin(), strTempDir.end(), U('\\'), U('/'));
		StringUtil::Split(strTempDir, vecStr, U('/'));
#endif

		strTempDir = U("");
		bool bResult = true;
		for (int i = 0; i < vecStr.size(); i++)
		{
			strTempDir += vecStr[i];
			strTempDir += U("/");
			bResult = MkDir(strTempDir);
		}
		return bResult;
	}

	bool FileUtil::MkDir(const std::wstring& strDir)
	{
#ifdef WIN32
		return _wmkdir(strDir.c_str()) == 0;
#else
		std::string strTemp = StringUtil::UNICODE_to_UTF8(strDir);
		return mkdir(strTemp.c_str(), 0777) == 0;
#endif
	}
}
