#include "S3MBFeature.h"
#include <time.inl>
#include <time.h>
#include <iosfwd>
#include "Skeleton.h"
#include <assert.h>
#include <sstream>
S3mbTimeSpan::S3mbTimeSpan()
{
	m_timeSpan = 0;
}

S3mbTimeSpan::S3mbTimeSpan(double dTime)
{
	m_timeSpan = dTime;
}

S3mbTimeSpan::S3mbTimeSpan(int nDays, int nHours, int nMins, int nSecs)
{
	m_timeSpan = nDays + ((double)nHours) / 24 + ((double)nMins) / (24 * 60) + ((double)nSecs) / (24 * 60 * 60);
}

S3mbTimeSpan::S3mbTimeSpan(const S3mbTimeSpan& tmsSrc)
{
	m_timeSpan = tmsSrc.m_timeSpan;
}

const S3mbTimeSpan& S3mbTimeSpan::operator=(const S3mbTimeSpan& tmsSrc)
{
	if (this == &tmsSrc)
		return *this;
	m_timeSpan = tmsSrc.m_timeSpan;
	return *this;
}

int S3mbTimeSpan::GetDays() const
{
	return (int)m_timeSpan;
}

#define UG_DATETIME_HALFSECOND (1.0 / (2.0 * (60.0 * 60.0 * 24.0)))

int S3mbTimeSpan::GetTotalHours() const
{
	int nReturns = (int)(m_timeSpan * 24 + UG_DATETIME_HALFSECOND);
	return nReturns;
}

int S3mbTimeSpan::GetHours() const
{
	double dblTemp = 0;
	// Truncate days and scale up
	dblTemp = modf(m_timeSpan, &dblTemp);
	int lReturns = (int)((dblTemp + UG_DATETIME_HALFSECOND) * 24);
	if (lReturns >= 24)
	{
		lReturns -= 24;
	}
	return lReturns;
}

int S3mbTimeSpan::GetTotalMinutes() const
{
	int lReturns = (int)(m_timeSpan * 24 * 60 + UG_DATETIME_HALFSECOND);
	return lReturns;
}

int S3mbTimeSpan::GetMinutes() const
{
	double dblTemp = 0;
	// Truncate hours and scale up
	dblTemp = modf(m_timeSpan * 24, &dblTemp);
	int lReturns = (int)((dblTemp + UG_DATETIME_HALFSECOND) * 60);
	if (lReturns >= 60)
	{
		lReturns -= 60;
	}
	return lReturns;
}

int S3mbTimeSpan::GetTotalSeconds() const
{
	int lReturns = (int)(m_timeSpan * 24 * 60 * 60 + UG_DATETIME_HALFSECOND);
	return lReturns;
}

int S3mbTimeSpan::GetSeconds() const
{
	double dblTemp = 0;
	// Truncate minutes and scale up
	dblTemp = modf(m_timeSpan * 24 * 60, &dblTemp);
	int lReturns = (int)((dblTemp + UG_DATETIME_HALFSECOND) * 60);
	if (lReturns >= 60)
	{
		lReturns -= 60;
	}
	return lReturns;
}

double S3mbTimeSpan::GetTimeSpan() const
{
	return m_timeSpan;
}

S3mbTimeSpan S3mbTimeSpan::operator-(const S3mbTimeSpan &tms) const
{
	return S3mbTimeSpan(m_timeSpan - tms.m_timeSpan);
}

S3mbTimeSpan S3mbTimeSpan::operator+(const S3mbTimeSpan &tms) const
{
	return S3mbTimeSpan(m_timeSpan + tms.m_timeSpan);
}

const S3mbTimeSpan& S3mbTimeSpan::operator+=(const S3mbTimeSpan &tms)
{
	m_timeSpan += tms.m_timeSpan;
	return *this;
}

const S3mbTimeSpan& S3mbTimeSpan::operator-=(const S3mbTimeSpan &tms)
{
	m_timeSpan -= tms.m_timeSpan;
	return *this;
}

bool S3mbTimeSpan::operator==(const S3mbTimeSpan &tms) const
{
	//modified by cuiwz 2008-4-22 应该改成这样的形式
	return IS0(m_timeSpan - tms.m_timeSpan);
}

bool S3mbTimeSpan::operator!=(const S3mbTimeSpan &tms) const
{
	//modified by cuiwz 2008-4-22 应该改成这样的形式
	return !IS0(m_timeSpan - tms.m_timeSpan);
}

bool S3mbTimeSpan::operator<(const S3mbTimeSpan &tms) const
{
	return m_timeSpan < tms.m_timeSpan;
}

bool S3mbTimeSpan::operator>(const S3mbTimeSpan &tms) const
{
	return m_timeSpan > tms.m_timeSpan;
}

bool S3mbTimeSpan::operator<=(const S3mbTimeSpan &tms) const
{
	//modified by cuiwz 2008-4-22 应该改成这样的形式
	return ((m_timeSpan < tms.m_timeSpan) || IS0(m_timeSpan - tms.m_timeSpan));
}

bool S3mbTimeSpan::operator>=(const S3mbTimeSpan &tms) const
{
	//modified by cuiwz 2008-4-22 应该改成这样的形式
	return ((m_timeSpan > tms.m_timeSpan) || IS0(m_timeSpan - tms.m_timeSpan));
}


S3mbTime::S3mbTime()
{
	m_time = 0;
}

S3mbTime::S3mbTime(const S3mbTime& tmSrc)
{
	m_time = tmSrc.m_time;
}

//! \brief 通过time_t进行构造
S3mbTime::S3mbTime(time_t t)
{
	m_time = 0;
	*this = t;
}

S3mbTime S3mbTime::GetCurTime()
{
	return S3mbTime(::time(NULL));

}

S3mbTime S3mbTime::ConvertToRegionalTime(S3mbTime t, int bias)
{
	struct tm timeDest;
	//获取系统日常时间
	t.GetDateTime(timeDest);
	timeDest.tm_year = timeDest.tm_year - 1900;
	timeDest.tm_mon = timeDest.tm_mon - 1;

	time_t utcTime = ::mktime(&timeDest) + bias * 60;
	return S3mbTime(utcTime);
}

long S3mbTime::ConvertToLongTime(S3mbTime t)
{
	struct tm timeDest;
	//获取系统日常时间
	t.GetDateTime(timeDest);
	timeDest.tm_year = timeDest.tm_year - 1900;
	timeDest.tm_mon = timeDest.tm_mon - 1;

	time_t utcTime = ::mktime(&timeDest);
	return (long)utcTime;
}

string S3mbTime::ConvertToUGTimeFormat(string timeFormat)
{
	string newTimeFormat = timeFormat;
	StringHelper::TrimLeft(newTimeFormat);

	newTimeFormat.replace(newTimeFormat.find("YYYY"),sizeof("YYYY"), "%Y");
	newTimeFormat.replace(newTimeFormat.find("yyyy"), sizeof("yyyy"), "%Y");
	newTimeFormat.replace(newTimeFormat.find("YY"), sizeof("YY") ,"%y");
	newTimeFormat.replace(newTimeFormat.find("yy"), sizeof("yy"), "%y");

	newTimeFormat.replace(newTimeFormat.find("MM"), sizeof("MM") ,"%m");
	newTimeFormat.replace(newTimeFormat.find("DD"), sizeof("DD"), "%d");

	newTimeFormat.replace(newTimeFormat.find("hh"), sizeof("hh"), "%H");
	newTimeFormat.replace(newTimeFormat.find("mm"), sizeof("mm"), "%M");
	newTimeFormat.replace(newTimeFormat.find("ss"), sizeof("ss"), "%S");

	return newTimeFormat;
}

//! \brief 直接通过S3mbTime内部的double数值进行构造
S3mbTime::S3mbTime(double dValue)
{
	m_time = dValue;
}

S3mbTime::S3mbTime(int nYear, int nMonth, int nDay)
{
	m_time = 0;
	SetDateTime(nYear, nMonth, nDay, 0, 0, 0);
}

S3mbTime::S3mbTime(int nYear, int nMonth, int nDay,
	int nHour, int nMin, int nSec)
{
	m_time = 0;
	SetDateTime(nYear, nMonth, nDay, nHour, nMin, nSec);
}

const S3mbTime& S3mbTime::operator=(const S3mbTime& tmSrc)
{
	if (this == &tmSrc)
		return *this;
	m_time = tmSrc.m_time;
	return *this;
}

const S3mbTime& S3mbTime::operator=(time_t t)
{
	// Convert time_t to struct tm
	tm *ptm = localtime(&t);

	if (ptm != NULL)
	{
		SetDateTime(ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday,
			ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
	}
	return *this;
}

double S3mbTime::GetTime() const
{
	return m_time;
}

int S3mbTime::GetYear() const
{
	struct tm tmDest;
	GetDateTime(tmDest);
	return tmDest.tm_year;
}

int S3mbTime::GetMonth() const
{
	struct tm tmDest;
	GetDateTime(tmDest);
	return tmDest.tm_mon;
}

int S3mbTime::GetDay() const
{
	struct tm tmDest;
	GetDateTime(tmDest);
	return tmDest.tm_mday;
}

int S3mbTime::GetHour() const
{
	struct tm tmDest;
	GetDateTime(tmDest);
	return tmDest.tm_hour;
}

int S3mbTime::GetMinute() const
{
	struct tm tmDest;
	GetDateTime(tmDest);
	return tmDest.tm_min;
}

int S3mbTime::GetSecond() const
{
	struct tm tmDest;
	GetDateTime(tmDest);
	return tmDest.tm_sec;
}

int S3mbTime::GetDayOfWeek() const
{
	struct tm tmDest;
	GetDateTime(tmDest);
	return tmDest.tm_wday;
}

int S3mbTime::GetDayOfYear() const
{
	struct tm tmDest;
	GetDateTime(tmDest);
	return tmDest.tm_yday;
}

S3mbTimeSpan S3mbTime::operator-(const S3mbTime &tmTime) const
{
	return GetDoubleValue() - tmTime.GetDoubleValue();
}

S3mbTime S3mbTime::operator-(const S3mbTimeSpan &tms) const
{
	S3mbTime tmResult;
	tmResult.FromDoubleValue(GetDoubleValue() - tms.m_timeSpan);
	return tmResult;
}

S3mbTime S3mbTime::operator+(const S3mbTimeSpan &tms) const
{
	S3mbTime tmResult;
	tmResult.FromDoubleValue(GetDoubleValue() + tms.m_timeSpan);
	return tmResult;
}

const S3mbTime& S3mbTime::operator+=(const S3mbTimeSpan &tms)
{
	*this = this->operator+(tms);
	return *this;
}

const S3mbTime& S3mbTime::operator-=(const S3mbTimeSpan &tms)
{
	*this = *this - tms;
	return *this;
}

bool S3mbTime::operator==(const S3mbTime &tmTime) const
{
	return IS0(m_time - tmTime.m_time);
}

bool S3mbTime::operator!=(const S3mbTime &tmTime) const
{
	return !IS0(m_time - tmTime.m_time);
}

bool S3mbTime::operator< (const S3mbTime &tmTime) const
{
	return GetDoubleValue() < tmTime.GetDoubleValue();
}

bool S3mbTime::operator> (const S3mbTime &tmTime) const
{
	return GetDoubleValue() > tmTime.GetDoubleValue();
}

bool S3mbTime::operator<=(const S3mbTime &tmTime) const
{
	return (GetDoubleValue() < tmTime.GetDoubleValue()) || IS0(GetDoubleValue() - tmTime.GetDoubleValue());
}

bool S3mbTime::operator>=(const S3mbTime &tmTime) const
{
	return (GetDoubleValue() > tmTime.GetDoubleValue()) || IS0(GetDoubleValue() - tmTime.GetDoubleValue());
}

double S3mbTime::GetDoubleValue()  const
{
	// No problem if positive
	if (m_time >= 0)
		return m_time;

	// If negative, must convert since negative dates not continuous
	// (examples: -1.25 to -.75, -1.50 to -.50, -1.75 to -.25)
	double temp = ceil(m_time);
	return temp - (m_time - temp);
}

void S3mbTime::FromDoubleValue(double dValue)
{
	// No problem if positive
	if (dValue >= 0)
	{
		m_time = dValue;
		return;
	}

	// If negative, must convert since negative dates not continuous
	// (examples: -.75 to -1.25, -.50 to -1.50, -.25 to -1.75)
	double temp = floor(dValue); // dbl is now whole part
	//return temp + (temp - dbl);
	m_time = temp + (temp - dValue);
}

int g_nMonthDays[13] =
{ 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 };

void S3mbTime::SetDateTime(int nYear, int nMonth, int nDay,
	int nHour, int nMin, int nSec)
{
	// Validate year and month (ignore day of week and milliseconds)
	if (nYear > 9999 || nMonth < 1 || nMonth > 12)
		return;

	//  Check for leap year and set the number of days in the month
	bool bLeapYear = ((nYear & 3) == 0) &&
		((nYear % 100) != 0 || (nYear % 400) == 0);

	int nDaysInMonth =
		g_nMonthDays[nMonth] - g_nMonthDays[nMonth - 1] +
		((bLeapYear && nDay == 29 && nMonth == 2) ? 1 : 0);

	// Finish validating the date
	if (nDay < 1 || nDay > nDaysInMonth ||
		nHour > 23 || nMin > 59 ||
		nSec > 59)
	{
		return;
	}

	// Cache the date in days and time in fractional days
	long nDate = 0;	//	WhiteBox_Ignore
	double dblTime = 0;

	//It is a valid date; make Jan 1, 1AD be 1
	nDate = nYear * 365L + nYear / 4 - nYear / 100 + nYear / 400 +
		g_nMonthDays[nMonth - 1] + nDay;

	//  If leap year and it's before March, subtract 1:
	if (nMonth <= 2 && bLeapYear)
		--nDate;

	//  Offset so that 12/30/1899 is 0
	nDate -= 693959L;

	dblTime = (((long)nHour * 3600L) +  // hrs in seconds	//	WhiteBox_Ignore
		((long)nMin * 60L) +  // mins in seconds			//	WhiteBox_Ignore
		((long)nSec)) / 86400.0;							//	WhiteBox_Ignore

	m_time = (double)nDate + ((nDate >= 0) ? dblTime : -dblTime);

	return;
}

#define DATA_MIN                (-657434L)  
#define DATA_MAX               2958465L    
#define SECOND_HALF             (1.0/172800.0)

void S3mbTime::GetDateTime(struct tm& destTime) const
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
	long lDaysAbs = 0;    //Absolute Days
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

//! \brief 得到标准的tm结构
void S3mbTime::GetStandardTm(struct tm& tmDest) const
{
	GetDateTime(tmDest);
	// Convert afx internal tm to format expected by runtimes (_tcsftime, etc)
	tmDest.tm_year -= 1900;  // year is based on 1900
	tmDest.tm_mon -= 1;      // month of year is 0-based
	tmDest.tm_wday -= 1;     // day of week is 0-based
	tmDest.tm_yday -= 1;     // day of year is 0-based
}
#define maxTimeBufferSize       128
string S3mbTime::Format(const char *pFormat) const
{
	char szBuffer[maxTimeBufferSize];
	struct tm tmDest;
	GetStandardTm(tmDest);

	// addedd by zengzm 2007-4-18 VC8非常变态，在 strftime.c 文件 Line 783比VC6增加了一行代码
	// _VALIDATE_RETURN( ( timeptr->tm_year >=0 ), EINVAL, false)
	// 导致 如果小于1900的年代(标准的tm结构会减去1900), 在调用strftime时报错, 而这个限制看起来并没有什么意义
	// 为了支持VC8,我只好先处理一下,然后再还原回去
	bool bDealYear = false;
	if (tmDest.tm_year < 0)
	{
		tmDest.tm_year = -tmDest.tm_year;// 得到正数
		bDealYear = true;
	}
	// 就为这一个方法就不编译ogdcplatform了改一下
	if (!strftime(szBuffer, maxTimeBufferSize, pFormat, &tmDest))
	{
		szBuffer[0] = '\0';
	}

	string strResult(szBuffer);

	// 如果之前处理过,这里要还原
	if (bDealYear)
	{
		char* cOld=new char[4];
		char* cNew = new char[4];
		sprintf(cOld, "%04d", tmDest.tm_year + 1900);
		sprintf(cNew, "%04d", -tmDest.tm_year + 1900);
		string strOld(cOld), strNew(cNew);
		strResult.replace(strResult.find(strOld), strOld.size(),strNew);  //modified by cuiwz 因为 string 没有 operator UGPCTSTR() 方法
	}
	return strResult;
}

bool S3mbTime::ParseDateTime(string strDateTime, string strFormat)
{
	StringHelper::TrimLeft(strFormat);
	StringHelper::TrimRight(strFormat);
	StringHelper::TrimLeft(strDateTime);
	StringHelper::TrimRight(strDateTime);
	if (strFormat.length() <= 0)
	{
		return false;
	}

	char *pCh = (char *)strFormat.c_str();
	int nPos = 0, nPosDateTime = 0;
	struct tm atm;
	atm.tm_year = 1899;
	atm.tm_mon = 12;
	atm.tm_mday = 30;
	atm.tm_hour = 0;
	atm.tm_min = 0;
	atm.tm_sec = 0;
	int nLen = 0;
	while (nPos < strFormat.length())
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
				atm.tm_year = atoi(strDateTime.substr(nPosDateTime, nLen).c_str());
				nPosDateTime += nLen;
			}
			break;
		}
		case 'y':
		{
			nLen = GetNumberPos(strDateTime, nPosDateTime);
			if (nLen > 0)
			{
				atm.tm_year = atoi(strDateTime.substr(nPosDateTime, nLen).c_str());
				nPosDateTime += nLen;
			}
			break;
		}
		case 'm':
		{
			nLen = GetNumberPos(strDateTime, nPosDateTime);
			if (nLen > 0)
			{
				atm.tm_mon = atoi(strDateTime.substr(nPosDateTime, nLen).c_str()); // - 1;
				nPosDateTime += nLen;
			}


			break;
		}
		case 'd':
		{
			nLen = GetNumberPos(strDateTime, nPosDateTime);
			if (nLen > 0)
			{
				atm.tm_mday = atoi(strDateTime.substr(nPosDateTime, nLen).c_str()); // - 1;
				nPosDateTime += nLen;
			}


			break;
		}
		case 'X':
		{
			atm.tm_hour = atoi(strDateTime.substr(nPosDateTime, 2).c_str());
			nPosDateTime += 3;
			atm.tm_min = atoi(strDateTime.substr(nPosDateTime, 2).c_str());
			nPosDateTime += 3;
			atm.tm_sec = atoi(strDateTime.substr(nPosDateTime, 2).c_str());
			nPosDateTime += 2;
			break;
		}
		case 'B':
		{
			int nCount = 0;
			char *p = (char *)(strDateTime.c_str());
			if (nPosDateTime < strDateTime.length())
			{
				p += nPosDateTime;
				while ((*p >= 'A' && *p <= 'Z') ||
					(*p >= 'a' && *p <= 'z'))
				{
					nCount++;
					p++;
				}

				string strTemp = strDateTime.substr(nPosDateTime, nCount);

				if (StringHelper::CompareNoCase("January",strTemp))
					
				{
					atm.tm_mon = 0;
				}
				else if (StringHelper::CompareNoCase("February", strTemp))
				{
					atm.tm_mon = 1;
				}
				else if (StringHelper::CompareNoCase("March", strTemp))
				{
					atm.tm_mon = 2;
				}
				else if (StringHelper::CompareNoCase("April", strTemp))
				{
					atm.tm_mon = 3;
				}
				else if (StringHelper::CompareNoCase("May", strTemp))
				{
					atm.tm_mon = 4;
				}
				else if (StringHelper::CompareNoCase("June", strTemp))
				{
					atm.tm_mon = 5;
				}
				else if (StringHelper::CompareNoCase("July", strTemp))
				{
					atm.tm_mon = 6;
				}
				else if (StringHelper::CompareNoCase("August", strTemp))
				{
					atm.tm_mon = 7;
				}
				else if (StringHelper::CompareNoCase("September", strTemp))
				{
					atm.tm_mon = 8;
				}
				else if (StringHelper::CompareNoCase("October", strTemp))
				{
					atm.tm_mon = 9;
				}
				else if (StringHelper::CompareNoCase("November", strTemp))
				{
					atm.tm_mon = 10;
				}
				else if (StringHelper::CompareNoCase("December", strTemp))
				{
					atm.tm_mon = 11;
				}
				else
				{
					return false;
				}
				// added by zengzm 2007-4-5 
				atm.tm_mon++;
				nPosDateTime += nCount;

			}

			break;
		}
		case 'b':
		{
			string strTemp = strDateTime.substr(nPosDateTime, 3);

			if (StringHelper::CompareNoCase("Jan", strTemp))
			{
				atm.tm_mon = 0;
			}
			else if (StringHelper::CompareNoCase("Feb", strTemp))
			{
				atm.tm_mon = 1;
			}
			else if (StringHelper::CompareNoCase("Mar", strTemp))
			{
				atm.tm_mon = 2;
			}
			else if (StringHelper::CompareNoCase("Apr", strTemp))
			{
				atm.tm_mon = 3;
			}
			else if (StringHelper::CompareNoCase("May", strTemp))
			{
				atm.tm_mon = 4;
			}
			else if (StringHelper::CompareNoCase("Jun", strTemp))
			{
				atm.tm_mon = 5;
			}
			else if (StringHelper::CompareNoCase("Jul", strTemp))
			{
				atm.tm_mon = 6;
			}
			else if (StringHelper::CompareNoCase("Aug", strTemp))
			{
				atm.tm_mon = 7;
			}
			else if (StringHelper::CompareNoCase("Sep", strTemp))
			{
				atm.tm_mon = 8;
			}
			else if (StringHelper::CompareNoCase("Oct", strTemp))
			{
				atm.tm_mon = 9;
			}
			else if (StringHelper::CompareNoCase("Nov", strTemp))
			{
				atm.tm_mon = 10;
			}
			else if (StringHelper::CompareNoCase("Dec", strTemp))
			{
				atm.tm_mon = 11;
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
				atm.tm_hour = atoi(strDateTime.substr(nPosDateTime, nLen).c_str()); // - 1;
				nPosDateTime += nLen;
			}
			break;
		}
		case 'M':
		{
			nLen = GetNumberPos(strDateTime, nPosDateTime);
			if (nLen > 0)
			{
				atm.tm_min = atoi(strDateTime.substr(nPosDateTime, nLen).c_str()); // - 1;
				nPosDateTime += nLen;
			}
			break;
		}
		case 'S':
		{
			nLen = GetNumberPos(strDateTime, nPosDateTime);
			if (nLen > 0)
			{
				atm.tm_sec = atoi(strDateTime.substr(nPosDateTime, nLen).c_str()); // - 1;
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
	//m_time = mktime(&atm);
	//UGASSERT(m_time != -1);
	SetDateTime(atm.tm_year, atm.tm_mon, atm.tm_mday, atm.tm_hour, atm.tm_min, atm.tm_sec);
	return true;
	/*
	  {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"},

		{"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday",
				"Friday", "Saturday", },

		{"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug",
				"Sep", "Oct", "Nov", "Dec"},

		{"January", "February", "March", "April", "May", "June",
				"July", "August", "September", "October",
				"November", "December"},

		{"AM", "PM"}
	*/
}

unsigned int S3mbTime::GetMillisecond()
{
#ifdef WIN32	

	return ::GetTickCount();
#else

	timeval tv;
	//timezone在linux和solaris上都不认，传入NULL吧
	S3mbTime time = S3mbTime::GetCurTime();
	int nHour = time.GetHour();
	int nMinute = time.GetMinute();
	int nSecond = time.GetSecond();
	gettimeofday(&tv, NULL);
	int nResult = ((nHour * 60 + nMinute) * 60 + nSecond) * 1000 + (int)(tv.tv_usec / 1000);
	return nResult;

#endif //end if Win32
}

int S3mbTime::GetNumberPos(const string& strDateTime, int nCurrentPos)
{
	int nPos = 0;
	while (nCurrentPos < strDateTime.length() && '0' <= strDateTime.at(nCurrentPos) && strDateTime.at(nCurrentPos) <= '9')
	{
		nPos++;
		nCurrentPos++;
	}
	return nPos;
}

S3mbVariant::S3mbVariant()
{
	SetNull();
}

S3mbVariant::~S3mbVariant()
{
	Clear();
}

S3mbVariant::S3mbVariant(const S3mbVariant& varSrc)
{
	SetNull();
	*this = varSrc;
}

S3mbVariant::S3mbVariant(unsigned char bVal)
{
	SetNull();
	Set(bVal);
}

S3mbVariant::S3mbVariant(short sVal)
{
	SetNull();
	Set(sVal);
}

S3mbVariant::S3mbVariant(int iVal)
{
	SetNull();
	Set(iVal);
}

S3mbVariant::S3mbVariant(long lVal)
{
	SetNull();
	Set(lVal);
}

S3mbVariant::S3mbVariant(float fVal)
{
	SetNull();
	Set(fVal);
}

S3mbVariant::S3mbVariant(double dVal)
{
	SetNull();
	Set(dVal);
}

S3mbVariant::S3mbVariant(const S3mbTime& tmVal)
{
	SetNull();
	Set(tmVal);
}

S3mbVariant::S3mbVariant(const unsigned char* pData, int nSize)
{
	SetNull();
	Set(pData, nSize);
}

S3mbVariant::S3mbVariant(const string& str)
{
	SetNull();
	Set(str);
}

const S3mbVariant& S3mbVariant::operator=(const S3mbVariant& varSrc)
{
	if (this == &varSrc)
		return *this;
	Clear();
	switch (varSrc.m_nType) {
	case S3mbVariant::Null:
		break;
	case S3mbVariant::Byte:
	case S3mbVariant::Boolean:
		m_value.bVal = varSrc.m_value.bVal;
		break;
	case S3mbVariant::Short:
		m_value.sVal = varSrc.m_value.sVal;
		break;
	case S3mbVariant::Integer:
		m_value.iVal = varSrc.m_value.iVal;
		break;
	case S3mbVariant::Long:
		m_value.lVal = varSrc.m_value.lVal;
		break;
	case S3mbVariant::Float:
		m_value.fVal = varSrc.m_value.fVal;
		break;
	case S3mbVariant::Double:
		m_value.dVal = varSrc.m_value.dVal;
		break;
	case S3mbVariant::Time:
	case S3mbVariant::Date:
	case S3mbVariant::TimeStamp:
		m_value.tmVal = varSrc.m_value.tmVal;
		break;
	case S3mbVariant::Binary:
	{
		//modified by cuiwz 2008-4-22 由stream形式改成结构体形式的了
		unsigned char* pOldData = NULL;
		unsigned int nSize = 0;
		unsigned char* pNewData = NULL;
		pOldData = (unsigned char *)varSrc.m_value.binVal.pVal;
		nSize = varSrc.m_value.binVal.nSize;

		if ((pOldData != NULL) && (nSize > 0))
		{
			if ((pNewData = (unsigned char *)malloc(sizeof(unsigned char)*nSize)) == NULL)
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
	case S3mbVariant::String:
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

const S3mbVariant& S3mbVariant::operator=(unsigned char bVal)
{
	Set(bVal);
	return *this;
}

const S3mbVariant& S3mbVariant::operator=(short sVal)
{
	Set(sVal);
	return *this;
}

const S3mbVariant& S3mbVariant::operator=(int iVal)
{
	Set(iVal);
	return *this;
}

const S3mbVariant& S3mbVariant::operator=(long lVal)
{
	Set(lVal);
	return *this;
}

const S3mbVariant& S3mbVariant::operator=(float fVal)
{
	Set(fVal);
	return *this;
}

const S3mbVariant& S3mbVariant::operator=(double dVal)
{
	Set(dVal);
	return *this;
}

const S3mbVariant& S3mbVariant::operator=(const S3mbTime& tmVal)
{
	Set(tmVal);
	return *this;
}

const S3mbVariant& S3mbVariant::operator=(const string& str)
{
	Set(str);
	return *this;
}

bool S3mbVariant::operator<(const S3mbVariant& varSrc) const
{
	//! 首先判断类型是否相等
	if (m_nType != varSrc.m_nType)
	{
		return false;
	}
	switch (m_nType) {
	case S3mbVariant::Null:
		return true;
	case S3mbVariant::Byte:
	case S3mbVariant::Boolean:
		return m_value.bVal < varSrc.m_value.bVal;
	case S3mbVariant::Short:
		return m_value.sVal < varSrc.m_value.sVal;
	case S3mbVariant::Integer:
		return m_value.iVal < varSrc.m_value.iVal;
	case S3mbVariant::Long:
		return m_value.lVal < varSrc.m_value.lVal;
	case S3mbVariant::Float:
		return m_value.fVal < varSrc.m_value.fVal;
	case S3mbVariant::Double:
		return m_value.dVal < varSrc.m_value.dVal;
	case S3mbVariant::TimeStamp:
	case S3mbVariant::Date:
	case S3mbVariant::Time:
		return m_value.tmVal < varSrc.m_value.tmVal;
	case S3mbVariant::Binary:
	{
		//modified by cuiwz 2008-4-22 由stream形式改成结构体形式的了
		unsigned char* pData1 = NULL;
		unsigned int nSize1 = 0;
		unsigned char* pData2 = NULL;
		unsigned int nSize2 = 0;
		pData1 = (unsigned char *)m_value.binVal.pVal;
		nSize1 = m_value.binVal.nSize;
		pData2 = (unsigned char *)varSrc.m_value.binVal.pVal;
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
	case S3mbVariant::String:
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

bool S3mbVariant::operator==(const S3mbVariant& varSrc) const
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
	case S3mbVariant::Null:
		return true;
	case S3mbVariant::Byte:
		return m_value.bVal == varSrc.m_value.bVal;
	case S3mbVariant::Short:
		return m_value.sVal == varSrc.m_value.sVal;
	case S3mbVariant::Integer:
		return m_value.iVal == varSrc.m_value.iVal;
	case S3mbVariant::Long:
		return m_value.lVal == varSrc.m_value.lVal;
	case S3mbVariant::Float:
		return EQUAL((double)m_value.fVal, (double)varSrc.m_value.fVal);
	case S3mbVariant::Double:
		return EQUAL(m_value.dVal, varSrc.m_value.dVal);
	case S3mbVariant::Time:
		//return EQUAL(m_value.tmVal , varSrc.m_value.tmVal);		
	case S3mbVariant::Date://CHENZhao  增加了这两个类型，作出相应修改
	case S3mbVariant::TimeStamp:
		return ToString().compare(varSrc.ToString()) == 0;
	case S3mbVariant::Binary:
	{
		//modified by cuiwz 2008-4-22 由stream形式改成结构体形式的了
		unsigned char* pData1 = NULL;
		unsigned int nSize1 = 0;
		unsigned char* pData2 = NULL;
		unsigned int nSize2 = 0;
		pData1 = (unsigned char *)m_value.binVal.pVal;
		nSize1 = m_value.binVal.nSize;
		pData2 = (unsigned char *)varSrc.m_value.binVal.pVal;
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
	case S3mbVariant::String:
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

bool S3mbVariant::operator!=(const S3mbVariant& varSrc) const
{
	return !(*this == varSrc);
}

void S3mbVariant::SetNull()
{
	m_nType = Null;
	memset(&m_value, 0, sizeof(VarValue));
}

void S3mbVariant::Clear()
{
	if (m_nType == Binary)
	{
		if (m_value.binVal.pVal != NULL)
			free(m_value.binVal.pVal);
		m_value.binVal.pVal = NULL;
		m_value.binVal.nSize = 0;
	}
	if (m_nType == String)
	{
		string* pString = (string*)m_value.binVal.pVal;
		delete pString;
		pString = NULL;
	}

	SetNull();
}

void S3mbVariant::Set(unsigned char bVal)
{
	Clear();
	m_nType = S3mbVariant::Byte;
	m_value.bVal = bVal;
}

void S3mbVariant::Set(short sVal)
{
	Clear();
	m_nType = S3mbVariant::Short;
	m_value.sVal = sVal;
}

void S3mbVariant::Set(int iVal)
{
	Clear();
	m_nType = S3mbVariant::Integer;
	m_value.iVal = iVal;
}

void S3mbVariant::Set(long lVal)
{
	Clear();
	m_nType = S3mbVariant::Long;
	m_value.lVal = lVal;
}

void S3mbVariant::Set(float fVal)
{
	Clear();
	m_nType = S3mbVariant::Float;
	m_value.fVal = fVal;
}

void S3mbVariant::Set(double dVal)
{
	Clear();
	m_nType = S3mbVariant::Double;
	m_value.dVal = dVal;
}

void S3mbVariant::Set(const S3mbTime& tmVal)
{
	Clear();
	m_nType = S3mbVariant::TimeStamp;
	m_value.tmVal = tmVal.GetTime();
}

void S3mbVariant::Set(const unsigned char* pData, unsigned int nSize)
{
	if (pData == (unsigned char *)m_value.binVal.pVal)
	{
		assert(false);
		return;
	}
	Clear();
	m_nType = S3mbVariant::Binary;

	unsigned char *pNewData;

	if ((nSize > 0) || (pData != NULL))
	{
		if ((pNewData = (unsigned char *)malloc(sizeof(unsigned char)*nSize)) == NULL)
		{
			assert(false);
			return;
		}
		memcpy(pNewData, pData, nSize);
		m_value.binVal.pVal = pNewData;
		m_value.binVal.nSize = nSize;
	}


}

void S3mbVariant::Set(const string& str)
{
	Clear();
	m_nType = S3mbVariant::String;
	string* pString = new string(str);
	m_value.binVal.pVal = pString;
}

//! 转换为double类型
double S3mbVariant::ToDouble() const
{
	double dValue = 0;
	switch (m_nType)
	{
	case Null:
		// Do Nothing 
		break;
	case Byte:
	case Boolean:
		dValue = m_value.bVal;
		break;
	case Short:
		dValue = m_value.sVal;
		break;
	case Integer:
		dValue = m_value.iVal;
		break;
	case Long:
		dValue = (double)m_value.lVal;
		break;
	case Float:
		dValue = m_value.fVal;
		break;
	case Double:
		dValue = m_value.dVal;
		break;
	case Time:
	case TimeStamp:
	case Date:
		dValue = m_value.tmVal;
		break;
	case Binary:
		break;
	case String:
		if (m_value.binVal.pVal)
		{
			string *pStr = (string *)m_value.binVal.pVal;
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
int S3mbVariant::ToInt() const
{
	int nValue = -1;
	switch (m_nType)
	{
	case Null:
		// Do Nothing 
		break;
	case Byte:
	case Boolean:
		nValue = m_value.bVal;
		break;
	case Short:
		nValue = m_value.sVal;
		break;
	case Integer:
		nValue = m_value.iVal;
		break;
	case Long:
		nValue = (int)m_value.lVal;
		break;
	case Float:
		nValue = ROUND(m_value.fVal);
		break;
	case Double:
		nValue = ROUND(m_value.dVal);
		break;
	case Date:
	case TimeStamp:
	case Time:
		nValue = (int)m_value.tmVal;
		break;
	case Binary:
		break;
	case String:
		if (m_value.binVal.pVal)
		{
			string *pStr = (string *)m_value.binVal.pVal;
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
long S3mbVariant::ToLong() const
{
	long nValue = 0;
	switch (m_nType)
	{
	case Null:
		// Do Nothing 
		break;
	case Byte:
	case Boolean:
		nValue = m_value.bVal;
		break;
	case Short:
		nValue = m_value.sVal;
		break;
	case Integer:
		nValue = m_value.iVal;
		break;
	case Long:
		nValue = m_value.lVal;
		break;
	case Float:
		nValue = ROUND(m_value.fVal);
		break;
	case Double:
		nValue = ROUND(m_value.dVal);
		break;
	case Date:
	case TimeStamp:
	case Time:
		nValue = (long)m_value.tmVal;
		break;
	case Binary:
		break;
	case String:
		if (m_value.binVal.pVal)
		{
			string *pStr = (string *)m_value.binVal.pVal;
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
string S3mbVariant::ToString() const
{
	ostringstream ostr;
	string strValue;
	switch (m_nType)
	{
	case Null:
		// Do Nothing 
		break;
	case Byte:
	case Boolean:
		ostr << m_value.bVal;
		strValue = ostr.str();
		break;
	case Short:
		ostr << m_value.sVal;
		strValue = ostr.str();
		break;
	case Integer:
		ostr << m_value.iVal;
		strValue = ostr.str();
		break;
	case Long:
		ostr << m_value.lVal;
		strValue = ostr.str();
		break;
	case Float:
		// %g: 不打印无效的0
		//%g精度太低，换成%lf，由后面的方法去多余的0
		ostr << m_value.fVal;
		strValue = ostr.str();

		if (strValue.find('.') != -1)
		{
			StringHelper::TrimRight(strValue, '0');
			if (StringHelper::CompareNoCase(StringHelper::Right(strValue, 1), "."))
			{
				strValue = strValue + "0";
			}
		}
		break;
	case Double:
		// %g: 不打印无效的0
		// 注意: 经过测试,发现double时不能采用%g, %g输出的精度不够
		// 如 393482.2 输出的结果为 393482, 故而采用下面的方法去掉最后多余的0
		//modifed by lvzp (%lf-->%.16f), 精度问题, %lf只保留6位小数
		ostr << m_value.dVal;
		strValue = ostr.str();
		//因为发现出现如下问题，比如1.0，通过如下代码处理后变为1.，修改一下，好看一点。
		if (strValue.find('.') != -1)
		{
			StringHelper::TrimRight(strValue,'0');
			if (StringHelper::CompareNoCase(StringHelper::Right(strValue, 1), "."))
			{
				strValue = strValue + "0";
			}
		}
		break;
	case Time:
		// 输出为一个规定格式的日期时间字符串 为数据库的指定格式
#if defined OS_ANDROID || defined IOS
	{
		S3mbTime t(m_value.tmVal);
		ostr << t.GetHour() << ":" << t.GetMinute() << ":" << t.GetSecond();
		strValue = ostr.str();
	}
#else
		strValue = S3mbTime(m_value.tmVal).Format("%H:%M:%S");
#endif
		break;
	case Date:
		// 输出为一个规定格式的日期时间字符串 为数据库的指定格式
#if defined OS_ANDROID || defined IOS
	{
		S3mbTime t(m_value.tmVal);
		strValue.Format("%d-%02d-%02d", t.GetYear(), t.GetMonth(), t.GetDay());
	}
#else
		strValue = S3mbTime(m_value.tmVal).Format("%Y-%m-%d");
#endif
		break;
	case TimeStamp:
		// 输出为一个规定格式的日期时间字符串 为数据库的指定格式
#if defined OS_ANDROID || defined IOS
	{
		S3mbTime t(m_value.tmVal);
		strValue.Format("%d-%02d-%02d %02d:%02d:%02d), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());
	}
#else
		strValue = S3mbTime(m_value.tmVal).Format("%Y-%m-%d %H:%M:%S");
#endif
		break;
	case Binary:
		break;
	case String:
		if (m_value.binVal.pVal)
		{
			strValue = *((string *)m_value.binVal.pVal);
		}
		break;
	default:
		assert(false);
		break;
	}
	return strValue;
}

string S3mbVariant::ToStringLocal() const
{
	string strValue;
	switch (m_nType)
	{
	case Date:
	case TimeStamp:
	case Time:
		strValue = S3mbTime(m_value.tmVal).Format("%x %X");
		break;
	case Null:
	case Byte:
	case Boolean:
	case Short:
	case Integer:
	case Long:
	case Float:
	case Double:
	case Binary:
	case String:
		strValue = ToString();
		break;
	default:
		assert(false);
		break;


	}
	return strValue;
}