//////////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)               
//                                                           
//!  \file OgdcTime.h
//!  \brief 时间类，时间间隔类
//!  \details   
//!  \attention 
//!   Copyright (c) 2007-2012 SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OGDCTIME_H__F8775450_4889_4D7E_A042_7E61AF93876D__INCLUDED_)
#define AFX_OGDCTIME_H__F8775450_4889_4D7E_A042_7E61AF93876D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#ifdef WIN32
/* This needs to come before any includes for MSVC compiler */
#define _CRT_SECURE_NO_WARNINGS
//#include <Windows.h>
#endif


#include "OgdcString.h"

namespace OGDC {

//! \brief 时间间隔类
class BASE_API OgdcTimeSpan 
{
public:
	
	//! \brief 默认构造函数,时间间隔为0
	OgdcTimeSpan();
	//! \brief 指定时间间隔进行构造
	OgdcTimeSpan(OgdcInt nDays, OgdcInt nHours, OgdcInt nMins, OgdcInt nSecs);
	//! \brief 指定double时间进行构造
	OgdcTimeSpan(double dTime);
	
	//! \brief 拷贝构造函数
	OgdcTimeSpan(const OgdcTimeSpan& tmsSrc);
	//! \brief 拷贝赋值函数
	const OgdcTimeSpan& operator=(const OgdcTimeSpan& tmsSrc);
	
	//! \brief 获取日
	OgdcInt GetDays() const;
	//! \brief 获取小时,日转化为24小时
	OgdcInt GetTotalHours() const;
	//! \brief 获取小时,日不计算在内
	OgdcInt GetHours() const;
	//! \brief 获取分钟,日和时都转化为分
	OgdcInt GetTotalMinutes() const;
	//! \brief 获取分钟,日和时不计算在内
	OgdcInt GetMinutes() const;
	//! \brief 获取秒,日时分都转化为秒
	OgdcInt GetTotalSeconds() const;
	//! \brief 获取秒,日时分不计算在内
	OgdcInt GetSeconds() const;

	//! \brief 获取时间间隔
	double GetTimeSpan() const;
	
	//! \brief 重载操作符 -
	OgdcTimeSpan operator-(const OgdcTimeSpan &tms) const;
	//! \brief 重载操作符 +
	OgdcTimeSpan operator+(const OgdcTimeSpan &tms) const;
	//! \brief 重载操作符 +=
	const OgdcTimeSpan& operator+=(const OgdcTimeSpan &tms);
	//! \brief 重载操作符 -=
	const OgdcTimeSpan& operator-=(const OgdcTimeSpan &tms);
	//! \brief 重载操作符 ==
	OgdcBool operator==(const OgdcTimeSpan &tms) const;
	//! \brief 重载操作符 !=
	OgdcBool operator!=(const OgdcTimeSpan &tms) const;
	//! \brief 重载操作符 <
	OgdcBool operator<(const OgdcTimeSpan &tms) const;
	//! \brief 重载操作符 >
	OgdcBool operator>(const OgdcTimeSpan &tms) const;
	//! \brief 重载操作符 <=
	OgdcBool operator<=(const OgdcTimeSpan &tms) const;
	//! \brief 重载操作符 >=
	OgdcBool operator>=(const OgdcTimeSpan &tms) const;
		
private:
	//! \brief 时间间隔（天数）
	double m_timeSpan; 
	//! \brief 时间类
	friend class OgdcTime;
};

//! \brief 时间类
// 参照COleDateTime实现
// implemented as a floating-point value, measuring days from midnight, 
// 30 December 1899. So, midnight, 31 December 1899 is represented by 1.0. 
// Similarly, 6 AM, 1 January 1900 is represented by 2.25, 
// and midnight, 29 December 1899 is – 1.0. 
// However, 6 AM, 29 December 1899 is – 1.25.
// Now, OgdcTime can handle dates from 1 January 100 – 31 December 9999
class BASE_API OgdcTime  
{
public:
	

	//! \brief 获取以毫秒为计数的时间; 用来精确测算性能等; 
	static OgdcUint GetMillisecond();
	
	//! \brief 默认构造函数
	OgdcTime();

	//! \brief 拷贝构造函数
	OgdcTime(const OgdcTime& tmSrc);

	//! \brief 拷贝构造函数
	OgdcTime(time_t t);

	//! \brief 拷贝构造函数
	OgdcTime(double dValue);
	
	//! \brief 年月日构造函数
	OgdcTime(OgdcInt nYear,OgdcInt nMonth,OgdcInt nDay);
	
	//! \brief 年月日时分秒构造函数
	OgdcTime(OgdcInt nYear, OgdcInt nMonth, OgdcInt nDay, OgdcInt nHour, OgdcInt nMin, OgdcInt nSec);
	//! \brief 重载操作符=
	const OgdcTime& operator=(const OgdcTime& tmSrc);
	//! \brief 重载操作符=
	const OgdcTime& operator=(time_t t);

//	struct tm* GetGmtTm(struct tm* ptm = NULL) const;
//	struct tm* GetLocalTm(struct tm* ptm = NULL) const;

	//! \brief 获取时间
	double GetTime() const;

	//! \brief 获取年(公元)。
	OgdcInt GetYear() const;	
	//! \brief 获取一年中月数(1~12)
	//! \remarks 1月=1,2月=2,...,12月=12.
	OgdcInt GetMonth() const;
	//! \brief 获取每月中的日数(1~31)
	//! \remarks 1号=1,2号=2,...,31号=31.
	OgdcInt GetDay() const;         
	//! \brief 获取一天中的小时数(0~23)
	//! \remarks 0点=0，1点=1，...23点=23.
	OgdcInt GetHour() const;
	//! \brief 获取一小时内的分钟数(0~59)
	OgdcInt GetMinute() const;
	//! \brief 获取一分钟中的秒数(从0~59)
	OgdcInt GetSecond() const;
	//! \brief 获取一个星期的天数
	//! \remarks 周日=1,周一=2，周二=3，周三=4，周四=5；周五=6，周六=7
	OgdcInt GetDayOfWeek() const; 
	//! \brief 获取每年的日数(1~366)
	//! \remarks 1月1日为1,1月2日为2,...,12月31日为365或366
	OgdcInt GetDayOfYear() const;   
	//! \brief 重载操作符 -
	OgdcTimeSpan operator-(const OgdcTime &tmTime) const;
	//! \brief 重载操作符 -
	OgdcTime operator-(const OgdcTimeSpan &tms) const;
	//! \brief 重载操作符 +
	OgdcTime operator+(const OgdcTimeSpan &tms) const;
	//! \brief 重载操作符 +=
	const OgdcTime& operator+=(const OgdcTimeSpan &tms);
	//! \brief 重载操作符-=
	const OgdcTime& operator-=(const OgdcTimeSpan &tms);
	//! \brief 重载操作符 ==
	OgdcBool operator==(const OgdcTime &tmTime) const;
	//! \brief 重载操作符 !=
	OgdcBool operator!=(const OgdcTime &tmTime) const;
	//! \brief 重载操作符 <
	OgdcBool operator< (const OgdcTime &tmTime) const;
	//! \brief 重载操作符 >
	OgdcBool operator> (const OgdcTime &tmTime) const;
	//! \brief 重载操作符 <=
	OgdcBool operator<=(const OgdcTime &tmTime) const;
	//! \brief 重载操作符 >=
	OgdcBool operator>=(const OgdcTime &tmTime) const;

	//! \brief 传入一个格式化字符串,把OgdcTime的信息生成一个便于阅读的字符串
	//! \remarks 
	//! %d Day of month as decimal number (01 – 31);
	//! %H Hour in 24-hour format (00 – 23);
	//! %m Month as decimal number (01 – 12)
	//! %M Minute as decimal number (00 – 59)
	//! %S Second as decimal number (00 – 59)
	//! %y Year without century, as decimal number (00 – 99)
	//! %Y Year with century, as decimal number
	OgdcString Format(const OgdcChar *pFormat) const;

	// deleted by zengzm 2007-4-12 ToString以前采用ctime实现, 现在OgdcTime支持更广泛时间,就不能用ToString了.
	// 建议用Format("%Y-%m-%d %X")代替
	// 等同于 ctime(), MSDN上的说明如下, 例:Fri Apr 29 12:25:12 1994
	// Convert a time value to a string and adjust for local time zone settings.
	// OgdcString ToString() const;

	//! \brief 按照指定格式生成日期和时间
	//! \param strDateTime 日期和时间值
	//! \param strFormat 日期和时间的格式
	//! \remarks 例如: strFormat为"%Y-%m-%d %H:%M:%S", strDateTime为"1487-05-29 14:25:59", 
	//! 最后获取的OgdcTime为: 1487年05月29日 14时25分59秒
	//! \attention OgdcTime的ParseDateTime远没有MFC中COleDateTime的ParseDateTime强大, 
	//! COleDateTime的ParseDateTime中调用了Win32 API VarDateFromStr,其内部实现看不到,
	//! 实际上, OgdcC中也不用这么强大. 因此,请严格按照上述例子中的用法, 尤其注意多余的0也要写上
	OgdcBool ParseDateTime(OgdcString strDateTime,OgdcString strFormat);
	
	//! \brief 获取当前时间
	static OgdcTime GetCurTime();

	//!\brief 计算与当地时刻相差一定时间的地方时
	//!\param t 基础时间
    //!\param bias,与基础时间t的偏差，单位是分钟
	//!\param return返回新的地方时
	static OgdcTime ConvertToRegionalTime(OgdcTime t, OgdcInt bias);

	//!\brief 转换为Long型的时间
	//!\param t 时间
	//!\return 返回Long型的时间
	static OgdcLong ConvertToLongTime(OgdcTime t);

	//!\brief 将普通的日期格式转换成UGTime接受的转换格式,暂时不支持星期，毫秒，英文格式
	//!\timeFormat C#/Java日期格式，如“YYYY-MM-DD HH:mm:ss”
	//!\ return "%Y-%m-%d %H:%M:%S"
	static OgdcString ConvertToUGTimeFormat(OgdcString timeFormat);
public: 
	//! \brief 把年月日时分秒转化为内部的double数值
	//! \param nYear 年
	//! \param nMonth 月
	//! \param nDay 日
	//! \param nHour 小时
	//! \param nMin 分钟
	//! \param nSec 秒
	void SetDateTime(OgdcInt nYear, OgdcInt nMonth, OgdcInt nDay, OgdcInt nHour, OgdcInt nMin, OgdcInt nSec);
	//! \brief 把double数值转化为tm结构
	void GetDateTime(struct tm& tmDest) const;

	//! \brief 获取处理后的double值
	//! \remarks 等同于MFC中的_AfxDoubleFromDate
	OgdcDouble GetDoubleValue() const;
private:
	//! \brief 获取标准的tm结构
	void GetStandardTm(struct tm& tmDest) const;
	
	//! \brief 从处理后的double值中得到时间
	//! \remarks 等同于MFC中的_AfxDateFromDouble
	void FromDoubleValue(OgdcDouble dValue);
	//! \brief 从当前位置计算第一个不是Number的长度
	OgdcInt GetNumberPos(const OgdcString& strDateTime , OgdcInt nCurrentPos);

	//time_t m_time;
	double m_time;
};

}


#endif // !defined(AFX_OGDCTIME_H__F8775450_4889_4D7E_A042_7E61AF93876D__INCLUDED_)

