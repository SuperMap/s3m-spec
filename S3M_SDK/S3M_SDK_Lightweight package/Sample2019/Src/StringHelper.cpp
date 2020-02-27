#include "StringHelper.h"
#include <assert.h>
#include <io.h>
#include "atlstr.h"
#include <windows.h>

std::string StringHelper::StrToUpper(string str)
{
	char* s = (char*)str.c_str();
	for (int i = 0; i < str.size(); i++)
	{
		if (s[i] >= 'a'&&s[i] <= 'z')
		{
			s[i] = s[i] - 0x20;
		}
	}
	string strTmp(s);
	return strTmp;
	/*string strTmp = str;
	transform(strTmp.begin(), strTmp.end(), strTmp.begin(), toupper);
	if (strTmp.size() == 0)
	{
		return str;
	}
	return strTmp;*/
}

bool StringHelper::CompareNoCase(const string &strA, const string &strB)
{
	if (strA == strB)
	{
		return true;
	}
	string str1 = StrToUpper(strA);
	string str2 = StrToUpper(strB);
	return (str1 == str2);
}

void StringHelper::Split(const string& s, vector<string>arrStr, const char flag /*= ' '*/)
{
	arrStr.clear();
	istringstream iss(s);
	string temp;
	while (getline(iss, temp, flag))
	{
		arrStr.push_back(temp);
	}
}

std::string StringHelper::TrimLeft(string &str, char* pStr /*= NULL*/)
{
	if (pStr == NULL)
	{
		char tmp[5] = { '\r','\t','\n','\0',' ' };
		pStr = tmp;
	}
	const char *pBuffer = str.c_str();
	int nLength = str.length();
	int nCount = 0;
	int i = 0;
	bool bFind;
	int j;
	for (i = 0; i < nLength; ++i)
	{
		bFind = false;
		j = 0;
		char wchTemp = pStr[j];
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
	str = str.substr(0, nCount);
	return str;
}

std::string StringHelper::TrimRight(string &str, char* pStr /*= NULL*/)
{
	if (pStr == NULL)
	{
		char tmp[5] = { '\r','\t','\n','\0',' ' };
		pStr = tmp;
	}
	const char* pBuffer = str.c_str();
	int nLength = str.length();
	int nCount = 0;
	int i = 0;
	int j;
	bool bFind;
	for (i = 0; i < nLength; ++i)
	{
		j = 0;
		bFind = false;
		char wchTemp = pStr[j];
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
	str = str.substr(nLength - nCount, nLength);
	return str;
}

std::string StringHelper::TrimRight(string &str, char ch)
{
	char *pBuffer = (char*)str.c_str();
	int nLength = str.length();
	// 	char *pBufferTerminal = pBuffer+nLength;
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
	str = str.substr(0, nLength - nCount);
	return str;
}

std::string StringHelper::ANSI_to_UTF8(const string& str)
{
	int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
	wchar_t* pwBuf = new wchar_t[nwLen + 1];
	memset(pwBuf,0, nwLen * 2 + 2);

	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, pwBuf, nwLen);

	int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, 0, NULL, NULL);
	char* pBuf = new char[nLen + 1];
	memset(pBuf,0, nLen + 1);

	::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, pBuf, nLen, NULL, NULL);
	std::string retStr(pBuf);

	delete[]pBuf;
	delete[]pwBuf;

	pBuf = NULL;
	pwBuf = NULL;

	return retStr;
}

std::string StringHelper::UTF8_to_ANSI(const string& str)
{
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
}

int StringHelper::GetStrLength(const char* pStr)
{
	const char *eos = pStr;

	while (*eos++);

	return((int)(eos - pStr - 1));
}

std::string StringHelper::Format(const char* pStr, ...)
{
	if (pStr && *pStr)
	{
		int len = 0;
		unsigned int nCapacity = GetStrLength(pStr);
		nCapacity = nCapacity + 512;

		va_list args;
		va_start(args, pStr);
		char* CStr = new char[nCapacity];
		vsprintf(CStr, pStr, args);
		va_end(args);
		string str(CStr);
		return str;
	}
}

std::string StringHelper::Right(const string& str, int nCount)
{
	if (nCount > str.length())
	{
		nCount = str.length();
	}
	if (nCount > -1)
	{
		string newStr((const char*)(str.c_str() + str.length() - nCount), nCount);
		return newStr;
	}
	else
	{
		return str;
	}
}

std::string StringHelper::Left(const string& str, int nCount)
{
	if (nCount > str.length())
	{
		nCount = str.length();
	}
	if (nCount > -1)
	{
		string newStr((const char*)str.c_str(), nCount);
		return newStr;
	}
	else
	{
		return str;
	}
}

std::string StringHelper::From(long lVal, unsigned int base /*= 10*/)
{
	bool bNegative = false; // 记录是否为负数
	if (base == 10 && lVal < 0)
	{
		bNegative = true;
		lVal = -lVal;
	}
	string strResult = From((unsigned long)lVal, base);
	if (bNegative)
	{
		strResult = string("-" + strResult); // 十进制下的负数,前面要加-
	}
	return strResult;
}

std::string StringHelper::From(unsigned long lVal, unsigned int base /*= 10*/)
{
	string strResult;
	do {
		unsigned int nNum = 0;
		nNum = (unsigned int)(lVal % base);
		lVal /= base;
		if (nNum > 9)
		{ // 十六进制
			strResult += char(nNum - 10 + L'A');
		}
		else
		{ // 十进制
			strResult += char(nNum + L'0');
		}
	} while (lVal > 0);
	// 最后反转结果
	reverse(strResult.begin(), strResult.end());
	if (base == 16)
	{
		strResult = "0X" + strResult; // 加0X,以便外边用户使用
	}
	return strResult;
}

std::string StringHelper::GetDir(string filePath)
{
	int index = -(int)filePath.rfind('/') < -(int)filePath.rfind('\\') ? filePath.rfind('/') : filePath.rfind('\\');
	return filePath.substr(0, index);
}

std::string StringHelper::GetAbsolutePath(string strSrcPath, string path)
{
	replace(strSrcPath.begin(), strSrcPath.end(), '\\', '/');
	replace(path.begin(), path.end(), '\\', '/');
	if (strSrcPath.at(strSrcPath.size() - 1) == '/')
	{
		strSrcPath = strSrcPath.substr(0, strSrcPath.size() - 1);
	}
	if (StringHelper::Left(path, 2) == "./")
	{
		return strSrcPath + '/' + path.substr(2, path.size() - 2);
	}
	while (StringHelper::Left(path, 3) == "../")
	{
		path = path.substr(3, path.size() - 3);
		strSrcPath = strSrcPath.substr(0, strSrcPath.find_last_of('/'));
	}
	return strSrcPath + '/' + path;
}

string StringHelper::GetAbsolutePath(string relatPath)
{
	//MAX_PATH = 260 
	char exeFullPath[260];
	memset(exeFullPath, 0, 260);
	GetModuleFileName(NULL, exeFullPath, 260);

	string filePath(exeFullPath);
	string fileDir = GetDir(filePath);
	return GetAbsolutePath(fileDir, relatPath);
}

std::string StringHelper::ChangeExt(string path, string ext)
{
	if (find(path.begin(), path.end(), '.') == path.end())
	{
		return path + ext;
	}
	else
	{
		return path.substr(0, path.find_last_of('.')) + ext;
	}
}

std::string StringHelper::GetExt(string filePath)
{
	int index = filePath.find_last_of('.');
	return filePath.substr(index, filePath.size() - index);
}

bool StringHelper::IsFileExist(string path)
{
	int result = _access(path.c_str(), 0);
	if (result == 0)
	{
		return true;
	}
	return false;
}

FileType::EmType FileType::ExtNameToFileType(const string& strExtName)
{
	string strExt = StringHelper::StrToUpper(strExtName);
	FileType::EmType nFileType = FileType::none;
	if (strExt == ".GIF")
	{
		nFileType = FileType::GIF;
	}
	else if (strExt == ".BMP")
	{
		nFileType = FileType::BMP;
	}
	else if (strExt == ".PNG")
	{
		nFileType = FileType::PNG;
	}
	else if (strExt == ".JPG")
	{
		nFileType = FileType::JPG;
	}
	else if (strExt == ".TIF" || strExt == ".TIFF")
	{
		nFileType = FileType::GTiff;
	}
	else if (strExt == ".TGA")
	{
		nFileType = FileType::TGA;
	}
	else
	{
		assert(false);
	}
	return nFileType;
}

std::string FileType::FileTypeToExtName(EmType eFileType)
{
	string strExt;
	switch (eFileType)
	{
	case GTiff:
		strExt = ".tif";
		break;
	case PNG:
		strExt = ".png";
		break;
	case GIF:
		strExt = ".gif";
		break;
	case JPG:
		strExt = ".jpg";
		break;
	case TGA:
		strExt = ".tga";
		break;
	default:
		assert(false);
		break;
	}
	return strExt;
}
