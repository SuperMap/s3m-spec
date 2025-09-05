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
		// 构造函数
		Time(double dValue);
		// 获取时间
		double GetTime() const;
		// 传入一个格式化字符串,把S3MBTime的信息生成一个便于阅读的字符串
		// 说明： 
		//! %d Day of month as decimal number (01 – 31);
		//! %H Hour in 24-hour format (00 – 23);
		//! %m Month as decimal number (01 – 12)
		//! %M Minute as decimal number (00 – 59)
		//! %S Second as decimal number (00 – 59)
		//! %y Year without century, as decimal number (00 – 99)
		//! %Y Year with century, as decimal number
		string Format(const char* pFormat) const;

		// 按照指定格式生成日期和时间
		// strDateTime 日期和时间值
		// strFormat 日期和时间的格式
		// 例如: strDateTime为"1487-05-29 14:25:59", strFormat为"%Y-%m-%d %H:%M:%S",
		// 最后获取的Time为: 1487年05月29日 14时25分59秒
		bool Parse(const std::string& strDateTime, const std::string& strFormat);

		// 把double数值转化为tm结构
		void GetDateTime(struct tm& tmDest) const;

	private:
		// 获取标准的tm结构
		void GetStandardTm(struct tm& tmDest) const;
		// 从当前位置计算第一个不是Number的长度
		int GetNumberPos(const std::string& strDateTime, int nCurrentPos);
		// 把年月日时分秒转化为double数值
		// nYear 年
		// nMonth 月
		// nDay 日
		// nHour 小时
		// nMin 分钟
		// nSec 秒
		void SetDateTime(int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec);

		double m_time;
	};

	class S3MB_API Variant
	{
	public:
		// 默认构造函数
		Variant();

		// 析构函数
		~Variant();

		// 拷贝构造函数
		Variant(const Variant& varSrc);

		// unsigned char类型构造函数
		Variant(unsigned char bVal);
		// short类型构造函数
		Variant(short sVal);
		// int类型构造函数
		Variant(int iVal);
		// long类型构造函数
		Variant(long lVal);
		// float类型构造函数
		Variant(float fVal);
		// double类型构造函数
		Variant(double dVal);
		// S3MBTime类型构造函数
		Variant(const Time& tmVal);
		// unsigned char* 类型构造函数
		// 说明： 内部为内存拷贝，外边注意释放pData;
		Variant(const unsigned char* pData, int nSize);
		// string 类型构造函数
		Variant(const string& str);

		// 将 varSrc 赋值变体。
		const Variant& operator=(const Variant& varSrc);
		// 将 bVal 赋值变体
		const Variant& operator=(unsigned char bVal);
		// 将 sVal 赋值变体
		const Variant& operator=(short sVal);
		// 将 iVal 赋值变体
		const Variant& operator=(int iVal);
		// 将 lVal 赋值变体
		const Variant& operator=(long lVal);
		// 将 fVal 赋值变体
		const Variant& operator=(float fVal);
		// 将 dVal 赋值变体
		const Variant& operator=(double dVal);
		// 将 tmVal 赋值变体
		const Variant& operator=(const Time& tmVal);
		// 将 str 赋值变体
		const Variant& operator=(const string& str);

		// 判断大小
		// 说明： 类型不相等, 则认为不相等
		bool operator<(const Variant& varSrc) const;

		// 判断是否相等
		// 说明： 类型不相等, 则认为不相等
		bool operator==(const Variant& varSrc) const;

		// 判断是否不相等
		// 说明： 类型不相等, 则认为不相等
		bool operator!=(const Variant& varSrc) const;

		// 设置变量值为bVal 
		void Set(unsigned char bVal);
		// 设置变量值为sVal 
		void Set(short sVal);
		// 设置变量值为nVal 
		void Set(int nVal);
		// 设置变量值为nVal 
		void Set(long lVal);
		// 设置变量值为dVal 
		void Set(float dVal);
		// 设置变量值为dVal 
		void Set(double dVal);
		// 设置变量值为tmVal 
		void Set(const Time& tmVal);
		// 设置变量值为pData指向的字符串
		void Set(const unsigned char* pData, unsigned int nSize);
		// 设置变量值为str 
		void Set(const string& str);
		// 设置为空
		void SetNull();

		// 设置精度(只对浮点型起作用)
		void SetPrecision(unsigned short p);

		// 清空
		void Clear();

		// 获取变体类型
		VarType GetType() const;

		// 获取变体值
		VarValue& GetValue();

		// 获取变体值
		const VarValue& GetValue() const;

		// 转换为double类型
		double ToDouble() const;
		// 转换为int32 类型
		int ToInt() const;
		// 转换为int64 类型
		long ToLong() const;
		// 转换为String类型
		// 说明： 对于Time类型，转换成数据库识别的格式也就是 %Y-%m-%d %H:%M:%S 。
		wstring ToString() const;
		// 转换为String类型
		// 说明： 对于Time类型，转换成本地当前的时间风格,对于其它类型和ToString()方法一样。
		string ToStringLocal() const;

	public:
		// 变体类型
		VarType m_nType;

		// 变体值
		VarValue m_value;

	private:
		// 精度
		unsigned short m_nPrecision;
		bool m_bUsePrec;
	};
}

#endif