// UGInterNational.h: interface for the UGInterNational class.
//
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//  国际化宏的使用说明
//  在Toolkit 编译工程中定义USE_INTERNATIONALIZATION才能使用国际化函数。
// 一：说明
//     这些宏提供了各个国家和地区的数字，货币，时间的相应的字符串的表示方法。
//     并提供各个国家数字，货币，时间字符串表示和标准数示，货币，时间的相互转化。		 字
//     1.Locale 的表示方法 语言_国家
//       语言用的是 2字母 ISO-639 标准。
//       国家用的是 2字母 ISO-3166 标准。
//       则美国则表示为 "en_US";
//     2.Currency的表示方法
//       货币类型用的是 3字母 ISO-4217 标准。
//     3.TimeZone的表示方法
//       有两种表示方式
//       (1)GMT标准方式 如California 在GMT的8个小时前，则表示为 GMT-8:00
//		 (2) unix 系统中的 continent/city or ocean/cit。
//           则美国洛杉矶时间为"America/Los_Angeles"。
// 二：默认启动时 地区(locale) 货币(currency),时间区域(timezone)的查找顺序
//      第一步：到supermap.xml文件中查找相对应的配置信息 
//               <Locale></Locale>
//               <Currency></Currency> 
//               <TimeZone></TimeZone>
//      第二步：如果supermap中没有相应的信息，则默认情况下是本地的默认类型                          
// 三：例子
//   比如我们将货币 23 转化为美国的欧元表示方法
//     方法一：
//		 第一步，设置相应的地区，货币类型
//		 UGString strCurrency
//		 SetLocale("en_US");
//	     SetCurrency("EUR");
//		 第二步，将数字转化为货币字符串
//       CurrencyToString(23,strCurrency);
//     方法二：
//		 第一步，在supermap.xml设置相应的地区，货币类型
//		 UGString strCurrency
//       <Locale>en_US</Locale>
//       <Currency>EUR</Currency> 
//		 第二步，将数字转化为货币字符串
//       CurrencyToString(23,strCurrency);
//      
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_UGINTERN_H__A0DE3603_406A_4067_873B_E89DE17D39CB__INCLUDED_)
#define AFX_UGINTER_H__A0DE3603_406A_4067_873B_E89DE17D39CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Stream/ugdefs.h"
using namespace UGC;
namespace UGC
{
	
	//! \brief 设置当前的locale。
	//! \param strLocale [in] 要设置locale。
	//! \return 是否成功。
	//! \remarks 如果没有设置这个，当前的locale会是supermap.xml中的<Locale>，
	//           如果supermap.xml中没有描述，则是当前默认的locale.
	//! \attention 当你新设置一个locale的时候，同时currency，timezone会重置
	//             到当前系统的默认类型 。如果你的locale错误，则其他操作将按照原来
	//             的locale。
	extern TOOLKIT_API UGbool SetLocale(const UGString& strLocale);	
	//! \brief 返回当前的locale。。
	//! \return  locale。
	//! \remarks 。
	//! \attention 。
	extern TOOLKIT_API UGString GetLocale();


	//! \brief 设置当前的locale。
	//! \param strLocale [in] 要设置locale。
	//! \return 是否成功。
	//! \remarks 如果没有设置这个，当前的locale会是supermap.xml中的<Locale>，
	//           如果supermap.xml中没有描述，则是当前默认的locale.
	//! \attention 当你新设置一个locale的时候，同时currency，timezone会重置
	//             到当前系统的默认类型 。如果你的locale错误，则其他操作将按照原来
	//             的locale。
	extern TOOLKIT_API UGbool SetLocale(const UGString& strLocale);	
// 	//! \brief 返回当前的locale。。
// 	//! \return  locale。
// 	//! \remarks 。
// 	//! \attention 。
// 	extern TOOLKIT_API UGString GetCharset(UGString::Charset charset);
	
	//! \brief 设置当前的Currency。
	//! \param strCurrency [in] 要设置Currency。
	//! \return 是否成功。
	//! \remarks 如果没有设置这个，当前的Currency会是supermap.xml中的<Currency>，
	//!           如果supermap.xml中没有描述，则是当前默认的Currency.
	//! \attention 当你新设置一个locale的时候，同时currency会重置
	//!             到当前系统的默认类型 。
	extern TOOLKIT_API UGbool SetCurrency(const UGString& strCurrency);
	//! \brief 返回当前的Currency。
	//! \return  Currency
	//! \remarks 。
	//! \attention 。
	extern TOOLKIT_API UGString GetCurrency();

	//! \brief 设置当前的TimeZone。
	//! \param strTimeZone [in] 要设置TimeZone。
	//! \return 是否成功。
	//! \remarks 如果没有设置这个，当前的TimeZone会是supermap.xml中的<TimeZone>，
	//!           如果supermap.xml中没有描述，则是当前默认的TimeZone.
	//! \attention 当你新设置一个locale的时候，同时currency会重置
	//!             到当前系统的默认类型 。
	extern TOOLKIT_API UGbool SetTimeZone(const UGString& strTimeZone);
	//! \brief 返回当前的TimeZone。
	//! \return  TimeZone
	//! \remarks 。
	//! \attention 。
	extern TOOLKIT_API UGString GetTimeZone();

	//! \brief 设置设置当前string的编码。
	//! \param charset [in]。
	//! \return 。
	//! \remarks 。
	//! \attention 。
	extern TOOLKIT_API void SetCharset(UGMBString::Charset charset);	
	//! \brief 获取当前string的编码。
	//! \param  [in]。
	//! \return 。
	//! \remarks 。
	//! \attention 。
	extern TOOLKIT_API UGMBString::Charset GetCharset();	

    //! \brief 判断两个locale字符串的对象所表示的locale是否相等。
	//! \param localeSrc [in] 。
	//! \param localeTar [out]。
	//! \return 是否相等。
	//! \remarks 。
	//! \attention 。
	extern TOOLKIT_API UGbool EqualLocale(const UGString& localeSrc, const UGString& localeTar);	
	
	
	//! \brief 将dSource转化为本地化的字符串。
	//! \param dSource [in] 一个double值。
	//! \param strTarget [out] 本地化字符串。
	//! \return 转化是否成功。
	//! \remarks 如果没有定义USE_INTERNATIONALIZATION的宏，默认调用UGString::Format()函数。
	//! \attention 。
	extern TOOLKIT_API UGbool NumberToString(const UGdouble dSource,UGString& strTarget);
	//! \brief
 	extern TOOLKIT_API UGbool NumberToString(const UGint nSource,UGString& strTarget);
	//! \brief
	extern TOOLKIT_API UGbool NumberToString(const UGlong nSource,UGString& strTarget);
	//! \brief 将nSource转化为本地化的货币字符串。
	//! \param dSource [in] 一个double的货币值。
	//! \param strTarget [out] 本地化货币字符串。
	//! \return 转化是否成功。
	//! \remarks 如果没有定义USE_INTERNATIONALIZATION的宏，默认调用UGString::Format()函数。
	//! \attention 。
	extern TOOLKIT_API UGbool CurrencyToString(const UGdouble dSource,UGString& strTarget);
	//! \brief
	extern TOOLKIT_API UGbool CurrencyToString(const UGint nSource,UGString& strTarget);
	//! \brief
	extern TOOLKIT_API UGbool CurrencyToString(const UGlong nSource,UGString& strTarget);

	//! \brief 将nSource转化为本地化的百分号字符串。
	//! \param dSource [in] 一个double的百分数。
	//! \param strTarget [out] 本地化百分号字符串。
	//! \return 转化是否成功。
	//! \remarks 如果没有定义USE_INTERNATIONALIZATION的宏，默认调用UGString::Format()函数。
	//! \attention 
	extern TOOLKIT_API UGbool PercentToString(const UGdouble dSource,UGString& strTarget);
	//! \brief
	extern TOOLKIT_API UGbool PercentToString(const UGint nSource,UGString& strTarget);
	//! \brief
	extern TOOLKIT_API UGbool PercentToString(const UGlong nSource,UGString& strTarget);

	//! \brief 将本地化的字符串转化为一个strSource值。
	//! \param strSource [in] 本地化字符串。
	//! \param dTarget [out] 转化后的double数值。
	//! \return 转化是否成功。
	//! \remarks 如果没有定义USE_INTER的宏，默认调用UGString::ToXXXX()函数。
	//! \attention 。
	extern TOOLKIT_API UGbool StringToNumber(const UGString& strSource,UGdouble& dTarget);
	//! \brief
	extern TOOLKIT_API UGbool StringToNumber(const UGString& strSource,UGint& nTarget);
	//! \brief
	extern TOOLKIT_API UGbool StringToNumber(const UGString& strSource,UGlong& nTarget);
	//! \brief
	extern TOOLKIT_API UGdouble ugatof(const UGString& strSource);
	//! \brief
	extern TOOLKIT_API UGint ugatoi(const UGString& strSource);
	//! \brief
	extern TOOLKIT_API UGlong ugatol(const UGString& strSource);
	//! \brief 将本地化的货币字符串转化为一个strSource值。
	//! \param strSource [in] 本地化货币字符串。
	//! \param dTarget [out] 转化后的double数值。
	//! \return 转化是否成功。
	//! \remarks 如果没有定义USE_INTERNATIONALIZATION的宏，默认调用UGString::ToXXXX()函数。
	//! \attention 。
	extern TOOLKIT_API UGbool StringToCurrency(const UGString& strSource,UGdouble& dTarget);
	//! \brief
	extern TOOLKIT_API UGbool StringToCurrency(const UGString& strSource,UGint& nTarget);
	//! \brief
	extern TOOLKIT_API UGbool StringToCurrency(const UGString& strSource,UGlong& nTarget);
	
	
	//! \brief 将本地化的百分号字符串转化为一个strSource值。
	//! \param strSource [in] 本地化百分号字符串。
	//! \param dTarget [out] 转化后的double数值。
	//! \return 转化是否成功。
	//! \remarks 如果没有定义USE_INTERNATIONALIZATION的宏，默认调用UGString::ToXXXX()函数。
	//! \attention 。
	extern TOOLKIT_API UGbool StringToPercent(const UGString& strSource,UGdouble& dTarget);
	//! \brief
	extern TOOLKIT_API UGbool StringToPercent(const UGString& strSource,UGint& nTarget);
	//! \brief
	extern TOOLKIT_API UGbool StringToPercent(const UGString& strSource,UGlong& nTarget);

	//! \brief 将日期转化为本地化的字符串。
	//! \param tmSource [in] 一个日期值。
	//! \param strTarget [out] 本地化字符串。
	//! \param strFormat [in] 调用UGTime的格式。
	//! \return 转化是否成功。
	//! \remarks 如果没有定义USE_INTERNATIONALIZATION的宏，默认调用 UGTime::Format("%Y-%m-%d")函数。
	//! \attention 。
	extern TOOLKIT_API UGbool DateToString(const UGTime& tmSource,UGString &strTarget,const UGString &strFormat=_U("%Y-%m-%d"));
	//! \brief
	extern TOOLKIT_API UGbool DateToString(const UGdouble dSource,UGString &strTarget,const UGString &strFormat=_U("%Y-%m-%d"));

	//! \brief 将时间转化为本地化的字符串。
	//! \param tmSource [in] 一个时间值。
	//! \param strTarget [out] 本地化字符串。
	//! \param strFormat [in] 调用UGTime的格式。
	//! \return 转化是否成功。
	//! \remarks 如果没有定义USE_INTERNATIONALIZATION的宏，默认调用 UGTime::Format("%H:%M:%S")函数。
	//! \attention 。
	extern TOOLKIT_API UGbool TimeToString(const UGTime& tmSource,UGString &strTarget,const UGString &strFormat=_U("%H:%M:%S"));
	//! \brief
	extern TOOLKIT_API UGbool TimeToString(const UGdouble dSource,UGString &strTarget,const UGString &strFormat=_U("%H:%M:%S"));

	
	//! \brief 将日期时间转化为本地化的字符串。
	//! \param tmSource [in] 一个日期时间值。
	//! \param strTarget [out] 本地化字符串。
	//! \param strFormat [in] 调用UGTime的格式。
	//! \return 转化是否成功。
	//! \remarks 如果没有定义USE_INTERNATIONALIZATION的宏，默认调用 UGTime::Format("%Y-%m-%d %H:%M:%S")函数。
	extern TOOLKIT_API UGbool DateTimeToString(const UGTime& tmSource,UGString &strTarget,const UGString &strFormat=_U("%Y-%m-%d %H:%M:%S"));
	//! \brief
	extern TOOLKIT_API UGbool DateTimeToString(const UGdouble dSource,UGString &strTarget,const UGString &strFormat=_U("%Y-%m-%d %H:%M:%S"));

	
	//! \brief 将本地化的字符串转化为日期时间值。
	//! \param tmTarget [out] 一个日期时间值。
	//! \param strSource [in] 本地化字符串。
	//! \param strFormat [in] 调用UGTime的格式。
	//! \return 转化是否成功。
	//! \remarks 如果没有定义USE_INTERNATIONALIZATION的宏，默认调用 UGTime::ParseDateTime(strSource,"%Y-%m-%d");函数。
	extern TOOLKIT_API UGbool StringToDate(const UGString &strSource,UGTime& tmTarget,const UGString &strFormat=_U("%Y-%m-%d"));
	//! \brief
	extern TOOLKIT_API UGbool StringToDate(const UGString &strSource,UGdouble& dTarget,const UGString &strFormat=_U("%Y-%m-%d"));
	
	//! \brief 将本地化的字符串转化为时间值。
	//! \param tmTarget [out] 一个时间值。
	//! \param strSource [in] 本地化字符串。
	//! \param strFormat [in] 调用UGTime的格式。
	//! \return 转化是否成功。
	//! \remarks 如果没有定义USE_INTERNATIONALIZATION的宏，默认调用 UGTime::ParseDateTime(strSource,"%H:%M:%S");函数。
	extern TOOLKIT_API UGbool StringToTime(const UGString &strSource,UGTime& tmTarget,const UGString &strFormat=_U("%H:%M:%S"));
	//! \brief
	extern TOOLKIT_API UGbool StringToTime(const UGString &strSource,UGdouble& dTarget,const UGString &strFormat=_U("%H:%M:%S"));

	//! \brief 将本地化的字符串转化为日期时间值。
	//! \param tmTarget [out] 一个日期时间值。
	//! \param strSource [in] 本地化字符串。
	//! \param strFormat [in] 调用UGTime的格式。
	//! \return 转化是否成功。
	//! \remarks 如果没有定义USE_INTERNATIONALIZATION的宏，默认调用 UGTime::ParseDateTime(strSource,"%Y-%m-%d %H:%M:%S");函数。
	extern TOOLKIT_API UGbool StringToDateTime(const UGString &strSource,UGTime& tmTarget,const UGString &strFormat=_U("%Y-%m-%d %H:%M:%S"));
	//! \brief
	extern TOOLKIT_API UGbool StringToDateTime(const UGString &strSource,UGdouble& dTarget,const UGString &strFormat=_U("%Y-%m-%d %H:%M:%S"));


	//! \brief 将bool变量转换为TRUE, FALSE
	//! \param source [in] bool变量值。
	//! \param target [out] 目标字符串。
	//! \return 转化是否成功。
	extern TOOLKIT_API void BooleanToString(UGbool source, UGString& target);


	//! \brief 将字符串TRUE, FALSE，转换为bool变量
	//! \param source [in] TRUE or FALSE字符串。
	//! \param target [out] 目标bool变量。
	//! \return 转化是否成功。
	extern TOOLKIT_API void StringToBoolean(const UGString& source, UGbool& target);

} //end UGC
#endif // !defined(AFX_UGINTER_H__A0DE3603_406A_4067_873B_E89DE17D39CB__INCLUDED_)

