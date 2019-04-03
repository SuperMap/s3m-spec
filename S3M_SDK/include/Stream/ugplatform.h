/* 
	\file ugplatform.h
	\brief 不同平台特定实现的总入口
	\author zengzm
	\attention
	Copyright(c) 2000-2010 SuperMap Software Co., Ltd.<br>
	All Rights Reserved
	<b>更新说明：。</b>
	<em>1、第一条说明。</em>
	<em>2、第二条说明。</em>
*/

#ifndef _UGPLATFORM_H
#define _UGPLATFORM_H

#include "Stream/ugexports.h"
#include "Base/ogdcdefs.h"
using namespace OGDC;
#ifdef _UGUNICODE
	EXTERN_C STREAM_API OgdcChar* UG_strcpy( OgdcChar *strDestination, const OgdcChar *strSource );
	#define UGstrcpy	UG_strcpy	
	EXTERN_C STREAM_API OgdcChar* UG_strncpy( OgdcChar *strDestination, const OgdcChar *strSource, OgdcInt n);
	#define UGstrncpy	UG_strncpy
	EXTERN_C STREAM_API OgdcInt UG_strlen(const OgdcChar *string);
	#define UGstrlen	UG_strlen
	EXTERN_C STREAM_API OgdcChar* UG_strcat(OgdcChar  *dst, const OgdcChar *src);
	#define UGstrcat	UG_strcat 
	EXTERN_C STREAM_API OgdcInt UG_strcmp(OgdcChar  *s1, const OgdcChar *s2);
	#define UGstrcmp	UG_strcmp
	EXTERN_C STREAM_API OgdcInt UG_strncmp (const OgdcChar *ucs1, const OgdcChar *ucs2, OgdcInt n);
	#define UGstrncmp	UG_strncmp
	EXTERN_C STREAM_API OgdcInt UG_strncasecmp (const OgdcChar *s1, const OgdcChar *s2, OgdcInt n);
	#define UGstrnicmp UG_strncasecmp
	EXTERN_C STREAM_API OgdcChar* UG_strchr( const OgdcChar *string, OgdcChar c);
	#define UGstrchr	UG_strchr
	EXTERN_C STREAM_API OgdcChar* UG_strstr (const OgdcChar* s, const OgdcChar *substring);
	#define UGstrstr	UG_strstr
	EXTERN_C STREAM_API OgdcInt UG_strtol( const OgdcChar *nptr, OgdcChar **endptr, OgdcInt base );
	#define UGstrtol UG_strtol
#else
	#define UGstrcpy	strcpy
	#define UGstrncpy	strncpy
	#define UGstrlen	strlen
	#define UGstrcat	strcat 
	#define UGstrcmp	strcmp
	#define UGstrncmp	strncmp
	#define UGstrnicmp	strnicmp
	#define UGstrchr	strchr
	#define UGstrstr	strstr
	#define UGstrtol	strtol
#endif

extern STREAM_API OgdcInt UGsprintf( OgdcChar *buffer, OgdcInt nMaxSize, const OgdcChar *format, ...);
extern STREAM_API OgdcInt UGvfprintf(FILE* stream,const OgdcChar *format,va_list argptr);


#ifdef SYMBIAN60
#include "ugnowin.h"
#else
#ifdef WIN32
	#include "ugwin32.h"
#endif

#ifdef _WIN32_WCE
	#include "ugwince.h"
#endif

#ifndef WIN32
	#include "ugnowin.h"
#endif
#endif

#endif

