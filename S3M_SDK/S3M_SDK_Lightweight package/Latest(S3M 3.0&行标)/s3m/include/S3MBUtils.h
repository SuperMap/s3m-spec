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
		// ���ַ�����Сд�ַ�ת��Ϊ��д�ַ�
		static wstring StrToUpper(wstring str);

		// �ж��ַ����Ƿ���ͬ
		static bool CompareNoCase(const wstring& strA, const wstring& strB);

		// �и��ַ���
		static void Split(const wstring& s, vector<wstring>&arrStr, const wchar_t flag = ' ');
		static void Split(const string& s, vector<string>& arrStr, const char flag = ' ');

		// ȥ�����ָ���ַ���
		static wstring TrimLeft(wstring& str, wchar_t* pStr = NULL);
		static std::wstring TrimLeftString(std::wstring& strOri, const std::wstring& strDelete);

		// ȥ���ұ�ָ���ַ���
		static wstring TrimRight(wstring& str, wchar_t* pStr = NULL);
		static wstring TrimRight(wstring& str, wchar_t ch);
		static std::wstring TrimRightString(std::wstring& strOri, const std::wstring& strDelete);

		// �滻�ַ���
		// strOri Դ�ַ���
		// strOld ��Ҫ�滻�����ַ���
		// strNew �����滻���ַ���
		static int Replace(std::wstring& strOri, const std::wstring& strOld, const std::wstring& strNew);

		// unicode����ת��Ϊutf8����
		static std::string UNICODE_to_UTF8(const wstring& str);

		// utf8����ת��Ϊunicode����
		static std::wstring UTF8_to_UNICODE(const string& str);

		// ansi����ת��Ϊutf8����
		static std::string ANSI_to_UTF8(const string& str);

		// utf8����ת��Ϊansi����
		static std::string UTF8_to_ANSI(const string& str);

		// unicode����תΪansi����
		static std::string UnicodeToANSI(const wstring& str);
		// ansi����תΪunicode����
		static std::wstring ANSIToUnicode(const string& str);

		// ��ȡ�ַ�������
		static int GetStrLength(const wchar_t* pStr);
		// ��ʽ���ַ���
		static std::wstring Format(const wchar_t* pStr, ...);
		// ȡ���ַ����������ұߵ�ָ�����ȵ����ַ���
		// nCount �ֽڸ���
		// return �����ұ�ָ�����ȵ����ַ���
		static wstring Right(const wstring& str, int nCount);

		// ȡ���ַ�����������ߵ�ָ�����ȵ����ַ���
		// nCount �ֽڸ���
		// return �������ָ�����ȵ����ַ���
		static wstring Left(const wstring& str, int nCount);
		static wstring From(long lVal, unsigned int base = 10);
		static wstring From(unsigned long lVal, unsigned int base = 10);

		// ��ȡ�ļ������ļ���
		static wstring GetDir(wstring filePath);

		// ��ȡ�ļ�����·��
		static wstring GetAbsolutePath(wstring strSrcPath, wstring path);
		static wstring GetAbsolutePath(wstring relatPath);

		// ��ȡ�ļ����·��
		static std::wstring GetRelativePath(std::wstring strSrcPath, std::wstring strAbsPath);

		// ���ļ�ȫ·���еõ��ļ���
		static std::wstring GetName(std::wstring strFilePath);

		// ���ļ�ȫ·���еõ��ļ�������(������׺��)
		static std::wstring GetTitle(const std::wstring& strFilePath);

		// �ı��ļ���׺
		static wstring ChangeExt(wstring path, wstring ext);
		// ��ȡ�ļ���׺
		static wstring GetExt(wstring filePath);
		// �ж��ļ��Ƿ����
		static bool IsFileExist(wstring path);
#ifndef WIN32
        static std::string wchar_to_char(const wchar_t* str);
        static int GetModuleFileName(void* Fun, char* &sFilePath);
#endif
	};

	class S3MB_API FileUtil
	{
	public:
		// �����༶�ļ���
		static bool MkDirEx(const std::wstring& strDir);
		// �����ļ���
		static bool MkDir(const std::wstring& strDir);
	};
}

#endif
