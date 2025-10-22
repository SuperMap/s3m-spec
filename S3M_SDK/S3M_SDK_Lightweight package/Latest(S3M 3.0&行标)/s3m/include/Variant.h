#if !defined(S3MBVARIANT_H__D06F5E58_0A61_4E0E_A1E6_530A2FECC070__INCLUDED_)
#define S3MBVARIANT_H__D06F5E58_0A61_4E0E_A1E6_530A2FECC070__INCLUDED_

#pragma once
#include "S3MBCommon.h"
#include <time.h>

namespace S3MB
{
	class S3MB_API Time
	{
	public:
		// ���캯��
		Time(double dValue);
		// ��ȡʱ��
		double GetTime() const;
		// ����һ����ʽ���ַ���,��S3MBTime����Ϣ����һ�������Ķ����ַ���
		// ˵���� 
		//! %d Day of month as decimal number (01 �C 31);
		//! %H Hour in 24-hour format (00 �C 23);
		//! %m Month as decimal number (01 �C 12)
		//! %M Minute as decimal number (00 �C 59)
		//! %S Second as decimal number (00 �C 59)
		//! %y Year without century, as decimal number (00 �C 99)
		//! %Y Year with century, as decimal number
		string Format(const char* pFormat) const;

		// ����ָ����ʽ�������ں�ʱ��
		// strDateTime ���ں�ʱ��ֵ
		// strFormat ���ں�ʱ��ĸ�ʽ
		// ����: strDateTimeΪ"1487-05-29 14:25:59", strFormatΪ"%Y-%m-%d %H:%M:%S",
		// ����ȡ��TimeΪ: 1487��05��29�� 14ʱ25��59��
		bool Parse(const std::string& strDateTime, const std::string& strFormat);

		// ��double��ֵת��Ϊtm�ṹ
		void GetDateTime(struct tm& tmDest) const;

	private:
		// ��ȡ��׼��tm�ṹ
		void GetStandardTm(struct tm& tmDest) const;
		// �ӵ�ǰλ�ü����һ������Number�ĳ���
		int GetNumberPos(const std::string& strDateTime, int nCurrentPos);
		// ��������ʱ����ת��Ϊdouble��ֵ
		// nYear ��
		// nMonth ��
		// nDay ��
		// nHour Сʱ
		// nMin ����
		// nSec ��
		void SetDateTime(int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec);

		double m_time;
	};

	class S3MB_API Variant
	{
	public:
		// Ĭ�Ϲ��캯��
		Variant();

		// ��������
		~Variant();

		// �������캯��
		Variant(const Variant& varSrc);

		// unsigned char���͹��캯��
		Variant(unsigned char bVal);
		// short���͹��캯��
		Variant(short sVal);
		// int���͹��캯��
		Variant(int iVal);
		// long���͹��캯��
		Variant(long lVal);
		// float���͹��캯��
		Variant(float fVal);
		// double���͹��캯��
		Variant(double dVal);
		// S3MBTime���͹��캯��
		Variant(const Time& tmVal);
		// unsigned char* ���͹��캯��
		// ˵���� �ڲ�Ϊ�ڴ濽�������ע���ͷ�pData;
		Variant(const unsigned char* pData, int nSize);
		// string ���͹��캯��
		Variant(const string& str);

		// �� varSrc ��ֵ���塣
		const Variant& operator=(const Variant& varSrc);
		// �� bVal ��ֵ����
		const Variant& operator=(unsigned char bVal);
		// �� sVal ��ֵ����
		const Variant& operator=(short sVal);
		// �� iVal ��ֵ����
		const Variant& operator=(int iVal);
		// �� lVal ��ֵ����
		const Variant& operator=(long lVal);
		// �� fVal ��ֵ����
		const Variant& operator=(float fVal);
		// �� dVal ��ֵ����
		const Variant& operator=(double dVal);
		// �� tmVal ��ֵ����
		const Variant& operator=(const Time& tmVal);
		// �� str ��ֵ����
		const Variant& operator=(const string& str);

		// �жϴ�С
		// ˵���� ���Ͳ����, ����Ϊ�����
		bool operator<(const Variant& varSrc) const;

		// �ж��Ƿ����
		// ˵���� ���Ͳ����, ����Ϊ�����
		bool operator==(const Variant& varSrc) const;

		// �ж��Ƿ����
		// ˵���� ���Ͳ����, ����Ϊ�����
		bool operator!=(const Variant& varSrc) const;

		// ���ñ���ֵΪbVal 
		void Set(unsigned char bVal);
		// ���ñ���ֵΪsVal 
		void Set(short sVal);
		// ���ñ���ֵΪnVal 
		void Set(int nVal);
		// ���ñ���ֵΪnVal 
		void Set(long lVal);
		// ���ñ���ֵΪdVal 
		void Set(float dVal);
		// ���ñ���ֵΪdVal 
		void Set(double dVal);
		// ���ñ���ֵΪtmVal 
		void Set(const Time& tmVal);
		// ���ñ���ֵΪpDataָ����ַ���
		void Set(const unsigned char* pData, unsigned int nSize);
		// ���ñ���ֵΪstr 
		void Set(const string& str);
		// ����Ϊ��
		void SetNull();

		// ���þ���(ֻ�Ը�����������)
		void SetPrecision(unsigned short p);

		// ���
		void Clear();

		// ��ȡ��������
		VarType GetType() const;

		// ��ȡ����ֵ
		VarValue& GetValue();

		// ��ȡ����ֵ
		const VarValue& GetValue() const;

		// ת��Ϊbool����
		bool ToBoolean() const;
		// ת��Ϊdouble����
		double ToDouble() const;
		// ת��Ϊint32 ����
		int ToInt() const;
		// ת��Ϊint64 ����
		long ToLong() const;
		// ת��ΪString����
		// ˵���� ����Time���ͣ�ת�������ݿ�ʶ��ĸ�ʽҲ���� %Y-%m-%d %H:%M:%S ��
		wstring ToString() const;
		// ת��ΪString����
		// ˵���� ����Time���ͣ�ת���ɱ��ص�ǰ��ʱ����,�����������ͺ�ToString()����һ����
		string ToStringLocal() const;

	public:
		// ��������
		VarType m_nType;

		// ����ֵ
		VarValue m_value;

	private:
		// ����
		unsigned short m_nPrecision;
		bool m_bUsePrec;
	};
}

#endif