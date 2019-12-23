#include "S3mbVariant.h"
#include <assert.h>
#include "StringHelper.h"
#include <math.h>

//! \brief 直接通过S3mbTime内部的double数值进行构造
S3mbTime::S3mbTime(double dValue)
{
	m_time = dValue;
}

double S3mbTime::GetTime() const
{
	return m_time;
}

#define maxTimeBufferSize       128
string S3mbTime::Format(const char *pFormat) const
{
	char szBuffer[maxTimeBufferSize];
	struct tm tmDest;
	GetStandardTm(tmDest);

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
		char* cOld = new char[4];
		char* cNew = new char[4];
		sprintf(cOld, "%04d", tmDest.tm_year + 1900);
		sprintf(cNew, "%04d", -tmDest.tm_year + 1900);
		string strOld(cOld), strNew(cNew);
		strResult.replace(strResult.find(strOld), strOld.size(), strNew);  //modified by cuiwz 因为 string 没有 operator UGPCTSTR() 方法
	}
	return strResult;
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

int g_nMonthDays[13] =
{ 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 };
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
		return abs(m_value.fVal - varSrc.m_value.fVal) < 1e-10;
	case S3mbVariant::Double:
		return abs(m_value.dVal - varSrc.m_value.dVal) < 1e-10;
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
	string strValue;
	switch (m_nType)
	{
	case Null:
		// Do Nothing 
		break;
	case Byte:
	case Boolean:
		strValue = StringHelper::Format("%hd", m_value.bVal);
		break;
	case Short:
		strValue = StringHelper::Format("%hd", m_value.sVal);
		break;
	case Integer:
		strValue = StringHelper::Format("%d", m_value.iVal);
		break;
	case Long:
		strValue = StringHelper::From(m_value.lVal);
		break;
	case Float:
		// %g: 不打印无效的0
		//%g精度太低，换成%lf，由后面的方法去多余的0
		strValue = StringHelper::Format("%lf", m_value.fVal);

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
		strValue = StringHelper::Format("%16f", m_value.dVal);
		//因为发现出现如下问题，比如1.0，通过如下代码处理后变为1.，修改一下，好看一点。
		if (strValue.find('.') != -1)
		{
			StringHelper::TrimRight(strValue, '0');
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
		strValue.Format("%d:%d:%d", t.GetHour(), t.GetMinute(), t.GetSecond());
	}
#else
		strValue = S3mbTime(m_value.tmVal).Format("%H:%M:%S");
#endif
		break;
	case Date:
		// 输出为一个规定格式的日期时间字符串 为数据库的指定格式
//#if defined OS_ANDROID || defined IOS
//	{
//		S3mbTime t(m_value.tmVal);
//		strValue.Format("%d-%02d-%02d", t.GetYear(), t.GetMonth(), t.GetDay());
//	}
//#else
		strValue = S3mbTime(m_value.tmVal).Format("%Y-%m-%d");
//#endif
		break;
	case TimeStamp:
//		// 输出为一个规定格式的日期时间字符串 为数据库的指定格式
//#if defined OS_ANDROID || defined IOS
//	{
//		S3mbTime t(m_value.tmVal);
//		strValue.Format("%d-%02d-%02d %02d:%02d:%02d), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());
//	}
//#else
		strValue = S3mbTime(m_value.tmVal).Format("%Y-%m-%d %H:%M:%S");
//#endif
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