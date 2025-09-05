#include "Variant.h"
#include "S3MBUtils.h"
#include <assert.h>
#include <math.h>
#include <string.h>
#ifdef WIN32
#include <windows.h>
#endif

namespace S3MB
{
	Time::Time(double dValue)
	{
		m_time = dValue;
	}

	double Time::GetTime() const
	{
		return m_time;
	}

	#define maxTimeBufferSize       128
	string Time::Format(const char* pFormat) const
	{
		char szBuffer[maxTimeBufferSize];
		struct tm tmDest;
		GetStandardTm(tmDest);

		bool bDealYear = false;
		if (tmDest.tm_year < 0)
		{
			tmDest.tm_year = -tmDest.tm_year;// 得到正数
			bDealYear = true;
		}

		if (!strftime(szBuffer, maxTimeBufferSize, pFormat, &tmDest))
		{
			szBuffer[0] = '\0';
		}

		string strResult(szBuffer);

		// 如果之前处理过,这里要还原
		if (bDealYear)
		{
			char* cOld = new char[4];
			char* cNew = new char[4];
			sprintf(cOld, "%04d", tmDest.tm_year + 1900);
			sprintf(cNew, "%04d", -tmDest.tm_year + 1900);
			string strOld(cOld), strNew(cNew);
			strResult.replace(strResult.find(strOld), strOld.size(), strNew); 
		}
		return strResult;
	}

	bool Time::Parse(const std::string& strDateTime, const std::string& strFormat)
	{
		if (strDateTime.empty() || strFormat.empty())
		{
			return false;
		}

		char* pCh = (char*)strFormat.c_str();
		int nPos = 0, nPosDateTime = 0;
		struct tm atime;
		atime.tm_year = 1899;
		atime.tm_mon = 12;
		atime.tm_mday = 30;
		atime.tm_hour = 0;
		atime.tm_min = 0;
		atime.tm_sec = 0;
		int nLen = 0;
		while (nPos < strFormat.size())
		{
			switch (*pCh)
			{
			case '%':
				break;
			case 'Y':
			{
				nLen = GetNumberPos(strDateTime, nPosDateTime);
				if (nLen > 0)
				{
					atime.tm_year = std::atoi(strDateTime.substr(nPosDateTime, nLen).c_str());
					nPosDateTime += nLen;
				}
				break;
			}
			case 'y':
			{
				nLen = GetNumberPos(strDateTime, nPosDateTime);
				if (nLen > 0)
				{
					atime.tm_year = std::atoi(strDateTime.substr(nPosDateTime, nLen).c_str());
					nPosDateTime += nLen;
				}
				break;
			}
			case 'm':
			{
				nLen = GetNumberPos(strDateTime, nPosDateTime);
				if (nLen > 0)
				{
					atime.tm_mon = std::atoi(strDateTime.substr(nPosDateTime, nLen).c_str());
					nPosDateTime += nLen;
				}
				break;
			}
			case 'd':
			{
				nLen = GetNumberPos(strDateTime, nPosDateTime);
				if (nLen > 0)
				{
					atime.tm_mday = std::atoi(strDateTime.substr(nPosDateTime, nLen).c_str());
					nPosDateTime += nLen;
				}
				break;
			}
			case 'X':
			{
				atime.tm_hour = std::atoi(strDateTime.substr(nPosDateTime, 2).c_str());
				nPosDateTime += 3;
				atime.tm_min = std::atoi(strDateTime.substr(nPosDateTime, 2).c_str());
				nPosDateTime += 3;
				atime.tm_sec = std::atoi(strDateTime.substr(nPosDateTime, 2).c_str());
				nPosDateTime += 2;
				break;
			}
			case 'B':
			{
				int nCount = 0;
				char* p = (char*)(strDateTime.c_str());
				if (nPosDateTime < strDateTime.size())
				{
					p += nPosDateTime;
					while ((*p >= 'A' && *p <= 'Z') ||
						(*p >= 'a' && *p <= 'z'))
					{
						nCount++;
						p++;
					}

					std::string strTemp = strDateTime.substr(nPosDateTime, nCount);

					if (strTemp == "January")
					{
						atime.tm_mon = 0;
					}
					else if (strTemp == "February")
					{
						atime.tm_mon = 1;
					}
					else if (strTemp == "March")
					{
						atime.tm_mon = 2;
					}
					else if (strTemp == "April")
					{
						atime.tm_mon = 3;
					}
					else if (strTemp == "May")
					{
						atime.tm_mon = 4;
					}
					else if (strTemp == "June")
					{
						atime.tm_mon = 5;
					}
					else if (strTemp == "July")
					{
						atime.tm_mon = 6;
					}
					else if (strTemp == "August")
					{
						atime.tm_mon = 7;
					}
					else if (strTemp == "September")
					{
						atime.tm_mon = 8;
					}
					else if (strTemp == "October")
					{
						atime.tm_mon = 9;
					}
					else if (strTemp == "November")
					{
						atime.tm_mon = 10;
					}
					else if (strTemp == "December")
					{
						atime.tm_mon = 11;
					}
					else
					{
						return false;
					}

					atime.tm_mon++;
					nPosDateTime += nCount;
				}
				break;
			}
			case 'b':
			{
				std::string strTemp = strDateTime.substr(nPosDateTime, 3);

				if (strTemp == "Jan")
				{
					atime.tm_mon = 0;
				}
				else if (strTemp == "Feb")
				{
					atime.tm_mon = 1;
				}
				else if (strTemp == "Mar")
				{
					atime.tm_mon = 2;
				}
				else if (strTemp == "Apr")
				{
					atime.tm_mon = 3;
				}
				else if (strTemp == "May")
				{
					atime.tm_mon = 4;
				}
				else if (strTemp == "Jun")
				{
					atime.tm_mon = 5;
				}
				else if (strTemp == "Jul")
				{
					atime.tm_mon = 6;
				}
				else if (strTemp == "Aug")
				{
					atime.tm_mon = 7;
				}
				else if (strTemp == "Sep")
				{
					atime.tm_mon = 8;
				}
				else if (strTemp == "Oct")
				{
					atime.tm_mon = 9;
				}
				else if (strTemp == "Nov")
				{
					atime.tm_mon = 10;
				}
				else if (strTemp == "Dec")
				{
					atime.tm_mon = 11;
				}
				else
				{
					return false;
				}
				nPosDateTime += 3;
				break;
			}
			case 'H':
			{
				nLen = GetNumberPos(strDateTime, nPosDateTime);
				if (nLen > 0)
				{
					atime.tm_hour = std::atoi(strDateTime.substr(nPosDateTime, nLen).c_str());
					nPosDateTime += nLen;
				}
				break;
			}
			case 'M':
			{
				nLen = GetNumberPos(strDateTime, nPosDateTime);
				if (nLen > 0)
				{
					atime.tm_min = std::atoi(strDateTime.substr(nPosDateTime, nLen).c_str());
					nPosDateTime += nLen;
				}
				break;
			}
			case 'S':
			{
				nLen = GetNumberPos(strDateTime, nPosDateTime);
				if (nLen > 0)
				{
					atime.tm_sec = std::atoi(strDateTime.substr(nPosDateTime, nLen).c_str());
					nPosDateTime += nLen;
				}
				break;
			}
			case '-':
			case ' ':
			case ':':
			case ',':
				nPosDateTime++;
				break;
			default:
				return false;
			}
			pCh++;
			nPos++;
		}

		SetDateTime(atime.tm_year, atime.tm_mon, atime.tm_mday, atime.tm_hour, atime.tm_min, atime.tm_sec);
		return true;
	}

	// 得到标准的tm结构
	void Time::GetStandardTm(struct tm& tmDest) const
	{
		GetDateTime(tmDest);
		// Convert afx internal tm to format expected by runtimes (_tcsftime, etc)
		tmDest.tm_year -= 1900;  // year is based on 1900
		tmDest.tm_mon -= 1;      // month of year is 0-based
		tmDest.tm_wday -= 1;     // day of week is 0-based
		tmDest.tm_yday -= 1;     // day of year is 0-based
	}

	int Time::GetNumberPos(const std::string& strDateTime, int nCurrentPos)
	{
		int nPos = 0;
		while (nCurrentPos < strDateTime.size() && '0' <= strDateTime.at(nCurrentPos) && strDateTime.at(nCurrentPos) <= '9')
		{
			nPos++;
			nCurrentPos++;
		}
		return nPos;
	}

	int g_nMonthDays[13] =
	{ 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 };
	#define DATA_MIN                (-657434L)  
	#define DATA_MAX               2958465L    
	#define SECOND_HALF             (1.0/172800.0)

	void Time::SetDateTime(int nYear, int nMonth, int nDay,
		int nHour, int nMin, int nSec)
	{
		if (nMonth < 1 || nYear > 9999 || nMonth > 12)
		{
			return;
		}

		bool bLeapYear = ((nYear & 3) == 0) && ((nYear % 100) != 0 || (nYear % 400) == 0);

		int nDaysInMonth = g_nMonthDays[nMonth] - g_nMonthDays[nMonth - 1] +
			((bLeapYear && nDay == 29 && nMonth == 2) ? 1 : 0);

		if (nHour > 23 || nMin > 59 || nDay < 1 || nDay > nDaysInMonth || nSec > 59)
		{
			return;
		}

		long nDate = 0;
		double dblTime = 0;

		nDate = nYear * 365L + nYear / 4 - nYear / 100 + nYear / 400 +
			g_nMonthDays[nMonth - 1] + nDay;

		if (nMonth <= 2 && bLeapYear)
			--nDate;

		nDate -= 693959L;

		dblTime = (((long)nHour * 3600L) +
			((long)nMin * 60L) +
			((long)nSec)) / 86400.0;

		m_time = (double)nDate + ((nDate >= 0) ? dblTime : -dblTime);
	}

	void Time::GetDateTime(struct tm& destTime) const
	{
		if (m_time > DATA_MAX || m_time < DATA_MIN)
		{
			destTime.tm_year = 1899;
			destTime.tm_mon = 12;
			destTime.tm_mday = 30;
			destTime.tm_hour = 0;
			destTime.tm_min = 0;
			destTime.tm_sec = 0;
			destTime.tm_isdst = 0;
			destTime.tm_wday = 0;
			destTime.tm_yday = 0;
			return;
		}
		long lDaysAbs = 0;    // Absolute Days
		long lSecWithinDay = 0;
		long lMntWithinDay = 0;
		long l4oo_Years = 0;
		long l4oo_Century = 0;
		long l4_Years = 0;
		long l4_Day = 0;
		long l4_Yr = 0;
		bool bHop4 = true;
		double date = m_time;

		date += ((m_time > 0.0) ? SECOND_HALF : -SECOND_HALF);

		lDaysAbs = (long)date + 693959L;

		date = fabs(date);
		lSecWithinDay = (long)((date - floor(date)) * 86400.);

		destTime.tm_wday = (int)((lDaysAbs - 1) % 7L) + 1;

		l4oo_Years = (long)(lDaysAbs / 146097L);

		lDaysAbs %= 146097L;

		l4oo_Century = (long)((lDaysAbs - 1) / 36524L);

		if (l4oo_Century != 0)
		{
			lDaysAbs = (lDaysAbs - 1) % 36524L;

			l4_Years = (long)((lDaysAbs + 1) / 1461L);

			if (l4_Years != 0)
				l4_Day = (long)((lDaysAbs + 1) % 1461L);
			else
			{
				bHop4 = false;
				l4_Day = (long)lDaysAbs;
			}
		}
		else
		{
			l4_Years = (long)(lDaysAbs / 1461L);
			l4_Day = (long)(lDaysAbs % 1461L);
		}

		if (!bHop4)
		{
			l4_Yr = l4_Day / 365;
			l4_Day %= 365;
		}
		else
		{
			l4_Yr = (l4_Day - 1) / 365;
			if (l4_Yr != 0)
			{
				l4_Day = (l4_Day - 1) % 365;
			}
		}

		destTime.tm_yday = (int)l4_Day + 1;
		destTime.tm_year = (int)(l4oo_Years * 400 + l4oo_Century * 100 + l4_Years * 4 + l4_Yr);

		if (l4_Yr == 0 && bHop4)
		{
			if (l4_Day == 59)
			{
				destTime.tm_mon = 2;
				destTime.tm_mday = 29;
				goto DoDataTime;
			}

			if (l4_Day >= 60)
			{
				l4_Day--;
			}
		}

		l4_Day++;

		for (destTime.tm_mon = (int)((l4_Day >> 5) + 1);
			l4_Day > g_nMonthDays[destTime.tm_mon]; destTime.tm_mon++);

		destTime.tm_mday = (int)(l4_Day - g_nMonthDays[destTime.tm_mon - 1]);

	DoDataTime:
		if (lSecWithinDay != 0)
		{
			destTime.tm_sec = (int)lSecWithinDay % 60L;
			lMntWithinDay = lSecWithinDay / 60L;
			destTime.tm_min = (int)lMntWithinDay % 60;
			destTime.tm_hour = (int)lMntWithinDay / 60;
		}
		else
		{
			destTime.tm_hour = destTime.tm_min = destTime.tm_sec = 0;
		}

		return;
	}

	Variant::Variant()
	{
		SetNull();
	}

	Variant::~Variant()
	{
		Clear();
	}

	Variant::Variant(const Variant& varSrc)
	{
		SetNull();
		*this = varSrc;
	}

	Variant::Variant(unsigned char bVal)
	{
		SetNull();
		Set(bVal);
	}

	Variant::Variant(short sVal)
	{
		SetNull();
		Set(sVal);
	}

	Variant::Variant(int iVal)
	{
		SetNull();
		Set(iVal);
	}

	Variant::Variant(long lVal)
	{
		SetNull();
		Set(lVal);
	}

	Variant::Variant(float fVal)
	{
		SetNull();
		Set(fVal);
	}

	Variant::Variant(double dVal)
	{
		SetNull();
		Set(dVal);
	}

	Variant::Variant(const Time& tmVal)
	{
		SetNull();
		Set(tmVal);
	}

	Variant::Variant(const unsigned char* pData, int nSize)
	{
		SetNull();
		Set(pData, nSize);
	}

	Variant::Variant(const string& str)
	{
		SetNull();
		Set(str);
	}

	const Variant& Variant::operator=(const Variant& varSrc)
	{
		if (this == &varSrc)
			return *this;
		Clear();
		switch (varSrc.m_nType) {
		case VT_Null:
			break;
		case VT_Byte:
		case VT_Boolean:
			m_value.bVal = varSrc.m_value.bVal;
			break;
		case VT_Short:
			m_value.sVal = varSrc.m_value.sVal;
			break;
		case VT_Integer:
			m_value.iVal = varSrc.m_value.iVal;
			break;
		case VT_Long:
			m_value.lVal = varSrc.m_value.lVal;
			break;
		case VT_Float:
			m_value.fVal = varSrc.m_value.fVal;
			break;
		case VT_Double:
			m_value.dVal = varSrc.m_value.dVal;
			break;
		case VT_Time:
		case VT_Date:
		case VT_TimeStamp:
			m_value.tmVal = varSrc.m_value.tmVal;
			break;
		case VT_Binary:
		{
			unsigned char* pOldData = NULL;
			unsigned int nSize = 0;
			unsigned char* pNewData = NULL;
			pOldData = (unsigned char*)varSrc.m_value.binVal.pVal;
			nSize = varSrc.m_value.binVal.nSize;

			if ((pOldData != NULL) && (nSize > 0))
			{
				if ((pNewData = (unsigned char*)malloc(sizeof(unsigned char) * nSize)) == NULL)
				{
					assert(false);
					return *this;
				}
				memcpy(pNewData, pOldData, nSize);
				m_value.binVal.pVal = pNewData;
				m_value.binVal.nSize = nSize;
			}
		}
		break;
		case VT_String:
		{
			string* pOldString = (string*)varSrc.m_value.binVal.pVal;
			if (pOldString)
			{
				string* pNewString = new string(*pOldString);
				m_value.binVal.pVal = pNewString;
			}
		}
		break;
		default:
			assert(false);
			break;
		}
		m_nType = varSrc.m_nType;
		return *this;
	}

	const Variant& Variant::operator=(unsigned char bVal)
	{
		Set(bVal);
		return *this;
	}

	const Variant& Variant::operator=(short sVal)
	{
		Set(sVal);
		return *this;
	}

	const Variant& Variant::operator=(int iVal)
	{
		Set(iVal);
		return *this;
	}

	const Variant& Variant::operator=(long lVal)
	{
		Set(lVal);
		return *this;
	}

	const Variant& Variant::operator=(float fVal)
	{
		Set(fVal);
		return *this;
	}

	const Variant& Variant::operator=(double dVal)
	{
		Set(dVal);
		return *this;
	}

	const Variant& Variant::operator=(const Time& tmVal)
	{
		Set(tmVal);
		return *this;
	}

	const Variant& Variant::operator=(const string& str)
	{
		Set(str);
		return *this;
	}

	bool Variant::operator<(const Variant& varSrc) const
	{
		//! 首先判断类型是否相等
		if (m_nType != varSrc.m_nType)
		{
			return false;
		}
		switch (m_nType) {
		case VT_Null:
			return true;
		case VT_Byte:
		case VT_Boolean:
			return m_value.bVal < varSrc.m_value.bVal;
		case VT_Short:
			return m_value.sVal < varSrc.m_value.sVal;
		case VT_Integer:
			return m_value.iVal < varSrc.m_value.iVal;
		case VT_Long:
			return m_value.lVal < varSrc.m_value.lVal;
		case VT_Float:
			return m_value.fVal < varSrc.m_value.fVal;
		case VT_Double:
			return m_value.dVal < varSrc.m_value.dVal;
		case VT_TimeStamp:
		case VT_Date:
		case VT_Time:
			return m_value.tmVal < varSrc.m_value.tmVal;
		case VT_Binary:
		{
			// 由stream形式改成结构体形式的了
			unsigned char* pData1 = NULL;
			unsigned int nSize1 = 0;
			unsigned char* pData2 = NULL;
			unsigned int nSize2 = 0;
			pData1 = (unsigned char*)m_value.binVal.pVal;
			nSize1 = m_value.binVal.nSize;
			pData2 = (unsigned char*)varSrc.m_value.binVal.pVal;
			nSize2 = varSrc.m_value.binVal.nSize;

			if ((pData1 != NULL) && (pData2 != NULL))
			{
				if (nSize1 >= nSize2) {
					return false;
				}

				return memcmp(pData1, pData2, nSize1) < 0;
			}
			return false;
		}
		case VT_String:
		{
			string* pStr1 = (string*)m_value.binVal.pVal;
			string* pStr2 = (string*)varSrc.m_value.binVal.pVal;
			if (pStr1 && pStr2)
			{
				return pStr1->compare(pStr2->c_str()) < 0;
			}
			return false;
		}
		default:
			assert(false);
			return false;
		}
	}

	bool Variant::operator==(const Variant& varSrc) const
	{
		if (this == &varSrc)
		{
			return true;
		}
		//! 首先判断类型是否相等
		if (m_nType != varSrc.m_nType)
		{
			return false;
		}
		switch (m_nType) {
		case VT_Null:
			return true;
		case VT_Byte:
			return m_value.bVal == varSrc.m_value.bVal;
		case VT_Short:
			return m_value.sVal == varSrc.m_value.sVal;
		case VT_Integer:
			return m_value.iVal == varSrc.m_value.iVal;
		case VT_Long:
			return m_value.lVal == varSrc.m_value.lVal;
		case VT_Float:
            return fabs(m_value.fVal - varSrc.m_value.fVal) < 1e-10;
		case VT_Double:
            return fabs(m_value.dVal - varSrc.m_value.dVal) < 1e-10;
		case VT_Time:
			// return EQUAL(m_value.tmVal , varSrc.m_value.tmVal);		
		case VT_Date:
		case VT_TimeStamp:
			return ToString().compare(varSrc.ToString()) == 0;
		case VT_Binary:
		{
			// 由stream形式改成结构体形式的了
			unsigned char* pData1 = NULL;
			unsigned int nSize1 = 0;
			unsigned char* pData2 = NULL;
			unsigned int nSize2 = 0;
			pData1 = (unsigned char*)m_value.binVal.pVal;
			nSize1 = m_value.binVal.nSize;
			pData2 = (unsigned char*)varSrc.m_value.binVal.pVal;
			nSize2 = varSrc.m_value.binVal.nSize;

			if ((pData1 != NULL) && (pData2 != NULL))
			{
				if (nSize1 != nSize2)
				{
					return false;
				}

				return memcmp(pData1, pData2, nSize2) == 0;
			}
			return false;
		}
		case VT_String:
		{
			string* pStr1 = (string*)m_value.binVal.pVal;
			string* pStr2 = (string*)varSrc.m_value.binVal.pVal;
			if (pStr1 && pStr2)
			{
				return pStr1->compare(pStr2->c_str()) == 0;
			}
			return false;
		}
		default:
			assert(false);
			return false;
		}
	}

	bool Variant::operator!=(const Variant& varSrc) const
	{
		return !(*this == varSrc);
	}

	void Variant::Set(unsigned char bVal)
	{
		Clear();
		m_nType = VT_Byte;
		m_value.bVal = bVal;
	}

	void Variant::Set(short sVal)
	{
		Clear();
		m_nType = VT_Short;
		m_value.sVal = sVal;
	}

	void Variant::Set(int iVal)
	{
		Clear();
		m_nType = VT_Integer;
		m_value.iVal = iVal;
	}

	void Variant::Set(long lVal)
	{
		Clear();
		m_nType = VT_Long;
		m_value.lVal = lVal;
	}

	void Variant::Set(float fVal)
	{
		Clear();
		m_nType = VT_Float;
		m_value.fVal = fVal;
	}

	void Variant::Set(double dVal)
	{
		Clear();
		m_nType = VT_Double;
		m_value.dVal = dVal;
	}

	void Variant::Set(const Time& tmVal)
	{
		Clear();
		m_nType = VT_TimeStamp;
		m_value.tmVal = tmVal.GetTime();
	}

	void Variant::Set(const unsigned char* pData, unsigned int nSize)
	{
		if (pData == (unsigned char*)m_value.binVal.pVal)
		{
			assert(false);
			return;
		}
		Clear();
		m_nType = VT_Binary;

		unsigned char* pNewData;

		if ((nSize > 0) || (pData != NULL))
		{
			if ((pNewData = (unsigned char*)malloc(sizeof(unsigned char) * nSize)) == NULL)
			{
				assert(false);
				return;
			}
			memcpy(pNewData, pData, nSize);
			m_value.binVal.pVal = pNewData;
			m_value.binVal.nSize = nSize;
		}
	}

	void Variant::Set(const string& str)
	{
		Clear();
		m_nType = VT_String;
		string* pString = new string(str);
		m_value.binVal.pVal = pString;
	}

	void Variant::SetNull()
	{
		m_nType = VT_Null;
		memset(&m_value, 0, sizeof(VarValue));

		m_nPrecision = 0;
		m_bUsePrec = false;
	}

	void Variant::SetPrecision(unsigned short p)
	{
		m_nPrecision = p;
		m_bUsePrec = true;
	}

	void Variant::Clear()
	{
		if (m_nType == VT_Binary)
		{
			if (m_value.binVal.pVal != NULL)
				free(m_value.binVal.pVal);
			m_value.binVal.pVal = NULL;
			m_value.binVal.nSize = 0;
		}
		if (m_nType == VT_String)
		{
			string* pString = (string*)m_value.binVal.pVal;
			delete pString;
			pString = NULL;
		}

		SetNull();
	}

	// 获取变体类型
	VarType Variant::GetType() const
	{
		return m_nType;
	}

	// 获取变体值
	VarValue& Variant::GetValue()
	{
		return m_value;
	}

	// 获取变体值
	const VarValue& Variant::GetValue() const
	{
		return m_value;
	}

	//! 转换为double类型
	double Variant::ToDouble() const
	{
		double dValue = 0;
		switch (m_nType)
		{
		case VT_Null:
			// Do Nothing 
			break;
		case VT_Byte:
		case VT_Boolean:
			dValue = m_value.bVal;
			break;
		case VT_Short:
			dValue = m_value.sVal;
			break;
		case VT_Integer:
			dValue = m_value.iVal;
			break;
		case VT_Long:
			dValue = (double)m_value.lVal;
			break;
		case VT_Float:
			dValue = m_value.fVal;
			break;
		case VT_Double:
			dValue = m_value.dVal;
			break;
		case VT_Time:
		case VT_TimeStamp:
		case VT_Date:
			dValue = m_value.tmVal;
			break;
		case VT_Binary:
			break;
		case VT_String:
			if (m_value.binVal.pVal)
			{
				string* pStr = (string*)m_value.binVal.pVal;
				dValue = atof(pStr->c_str());
			}
			break;
		default:
			assert(false);
			break;
		}
		return dValue;
	}

	//! 转换为int32 类型
	int Variant::ToInt() const
	{
		int nValue = -1;
		switch (m_nType)
		{
		case VT_Null:
			// Do Nothing 
			break;
		case VT_Byte:
		case VT_Boolean:
			nValue = m_value.bVal;
			break;
		case VT_Short:
			nValue = m_value.sVal;
			break;
		case VT_Integer:
			nValue = m_value.iVal;
			break;
		case VT_Long:
			nValue = (int)m_value.lVal;
			break;
		case VT_Float:
			nValue = ROUND(m_value.fVal);
			break;
		case VT_Double:
			nValue = ROUND(m_value.dVal);
			break;
		case VT_Date:
		case VT_TimeStamp:
		case VT_Time:
			nValue = (int)m_value.tmVal;
			break;
		case VT_Binary:
			break;
		case VT_String:
			if (m_value.binVal.pVal)
			{
				string* pStr = (string*)m_value.binVal.pVal;
				nValue = atoi(pStr->c_str());
			}
			break;
		default:
			assert(false);
			break;
		}
		return nValue;
	}

	//! 转换为int64 类型
	long Variant::ToLong() const
	{
		long nValue = 0;
		switch (m_nType)
		{
		case VT_Null:
			// Do Nothing 
			break;
		case VT_Byte:
		case VT_Boolean:
			nValue = m_value.bVal;
			break;
		case VT_Short:
			nValue = m_value.sVal;
			break;
		case VT_Integer:
			nValue = m_value.iVal;
			break;
		case VT_Long:
			nValue = m_value.lVal;
			break;
		case VT_Float:
			nValue = ROUND(m_value.fVal);
			break;
		case VT_Double:
			nValue = ROUND(m_value.dVal);
			break;
		case VT_Date:
		case VT_TimeStamp:
		case VT_Time:
			nValue = (long)m_value.tmVal;
			break;
		case VT_Binary:
			break;
		case VT_String:
			if (m_value.binVal.pVal)
			{
				string* pStr = (string*)m_value.binVal.pVal;
				nValue = atol(pStr->c_str());
			}
			break;
		default:
			assert(false);
			break;
		}
		return nValue;
	}

	//! 转换为String类型
	wstring Variant::ToString() const
	{
		wstring strValue;
		switch (m_nType)
		{
		case VT_Null:
			// Do Nothing 
			break;
		case VT_Byte:
		case VT_Boolean:
            strValue = StringUtil::Format(((wstring)U("%hd")).c_str(), m_value.bVal);
			break;
		case VT_Short:
            strValue = StringUtil::Format(((wstring)U("%hd")).c_str(), m_value.sVal);
			break;
		case VT_Integer:
            strValue = StringUtil::Format(((wstring)U("%d")).c_str(), m_value.iVal);
			break;
		case VT_Long:
			strValue = StringUtil::From(m_value.lVal);
			break;
		case VT_Float:
		{
			// %g: 不打印无效的0
			// %g精度太低，换成%lf，由后面的方法去多余的0
			char strFormat[256] = "%.1f";
			if (m_bUsePrec)
			{
				sprintf(strFormat, "%%.%df", m_nPrecision);
			}

			char str[256] = "";
			sprintf(str, strFormat, m_value.fVal);
            string temp_str(str);
            strValue = StringUtil::UTF8_to_UNICODE(temp_str);
            if (strValue.find(L'.') != -1)
			{
                StringUtil::TrimRight(strValue, L'0');
				if (StringUtil::CompareNoCase(StringUtil::Right(strValue, 1), U(".")))
				{
					strValue = strValue + U("0");
				}
			}
		}
			break;
		case VT_Double:
			// %g: 不打印无效的0
			// 注意: 经过测试,发现double时不能采用%g, %g输出的精度不够
			// 如 393482.2 输出的结果为 393482, 故而采用下面的方法去掉最后多余的0
			// (%lf-->%.16f), 精度问题, %lf只保留6位小数
			{
				char strFormat[256] = "%.16f";
				if (m_bUsePrec)
				{
					sprintf(strFormat, "%%.%df", m_nPrecision);
				}

				char str[256] = "";
				sprintf(str, strFormat, m_value.dVal);
                string temp_str(str);
                strValue = StringUtil::UTF8_to_UNICODE(temp_str);
				if (strValue.find('.') != -1)
				{
                    StringUtil::TrimRight(strValue, L'0');
					if (StringUtil::CompareNoCase(StringUtil::Right(strValue, 1), U(".")))
					{
						strValue = strValue + U("0");
					}
				}
			}
			break;
		case VT_Time:
			// 输出为一个规定格式的日期时间字符串 为数据库的指定格式
#if defined OS_ANDROID || defined IOS
		{
			Time t(m_value.tmVal);
			strValue.Format("%d:%d:%d", t.GetHour(), t.GetMinute(), t.GetSecond());
		}
#else
			strValue = StringUtil::UTF8_to_UNICODE(Time(m_value.tmVal).Format("%H:%M:%S"));
#endif
			break;
		case VT_Date:
			// 输出为一个规定格式的日期时间字符串 为数据库的指定格式
			//#if defined OS_ANDROID || defined IOS
			//	{
			//		S3MBTime t(m_value.tmVal);
			//		strValue.Format("%d-%02d-%02d", t.GetYear(), t.GetMonth(), t.GetDay());
			//	}
			//#else
			strValue = StringUtil::UTF8_to_UNICODE(Time(m_value.tmVal).Format("%Y-%m-%d"));
			//#endif
			break;
		case VT_TimeStamp:
			strValue = StringUtil::UTF8_to_UNICODE(Time(m_value.tmVal).Format("%Y-%m-%d %H:%M:%S"));
			break;
		case VT_Binary:
			break;
		case VT_String:
			if (m_value.binVal.pVal)
			{
				strValue = StringUtil::UTF8_to_UNICODE(*((string*)m_value.binVal.pVal));
			}
			break;
		default:
			assert(false);
			break;
		}
		return strValue;
	}

	string Variant::ToStringLocal() const
	{
		string strValue;
		switch (m_nType)
		{
		case VT_Date:
		case VT_TimeStamp:
		case VT_Time:
			strValue = Time(m_value.tmVal).Format("%x %X");
			break;
		case VT_Null:
		case VT_Byte:
		case VT_Boolean:
		case VT_Short:
		case VT_Integer:
		case VT_Long:
		case VT_Float:
		case VT_Double:
		case VT_Binary:
		case VT_String:
#ifdef WIN32
            strValue =StringUtil::UNICODE_to_UTF8(ToString());
#else

            strValue =StringUtil::UnicodeToANSI((ToString()));
#endif
			break;
		default:
			assert(false);
			break;
		}
		return strValue;
	}
}
