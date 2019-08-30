//========================================================================
// 项目名：			SuperMap Universal GIS class (UGC) Library 0.9
// 作  者：			张立立
// 单  位：			北京超图软件股份有限公司
// 最后修改时间：	以版本更新完成日期为准
//------------------------------------------------------------------------
//版权声明:         版权所有 2000-2010 北京超图软件股份有限公司，保留所有权利 
//	                Copyright (c) 2000-2010 SuperMap Software Co., Ltd. 
//	                All rights reserved

// 重要声明：		1. 
//					2. 
//					
//------------------------------------------------------------------------
// 文件名：			UGTranslateParams.h
// 类  名：			UGGeoTransParams
// 父  类：			
// 子  类：			

// 功能说明：		参照系变化使用的参数表
// 调用说明：		
/*-------------------------------------版本更新---------------------------


$Revision: 1.3 $
$Log: UGTranslateParams.h,v $
Revision 1.3  2009/04/03 01:20:26  liangshuang
头文件注释添加

Revision 1.2  2008/07/25 07:44:41  gaoff
高凤飞
修改注释中 公司的 中/英文名称

Revision 1.1  2008/04/29 07:02:41  lugw
*** empty log message ***

Revision 1.7  2007/12/17 08:13:38  dongfei
添加注释

Revision 1.6  2007/11/15 10:52:53  dongfei
*** empty log message ***

Revision 1.5  2005/07/18 15:24:04  zhangli
增加文件说明和注释

$Author: liangshuang $
$Date: 2009/04/03 01:20:26 $
========================================================================*/
#if !defined(UGC_SMTRANSLATEPARAMS_H__3173CAC9_B797_44C0_AC30_5F8888E57F0F__INCLUDED_)
#define UGC_SMTRANSLATEPARAMS_H__3173CAC9_B797_44C0_AC30_5F8888E57F0F__INCLUDED_


//// #include "SmImport.h"

#include "Stream/ugdefs.h"
#include "Projection/UGPjCon.h"



namespace UGC{


class PROJECTION_API UGGeoTransParams		//  GeoTransParamsDefault;
{
public:
	//! \brief 默认构造函数
	UGGeoTransParams();
	//! \brief 拷贝构造函数
	UGGeoTransParams(const UGGeoTransParams& GeoTransParams);

#ifdef SYMBIAN60
	~UGGeoTransParams();
#else
	virtual ~UGGeoTransParams();
#endif
	
	UGGeoTransParams& operator = (const UGGeoTransParams& GeoTransParams);
	UGbool  operator == (const UGGeoTransParams& GeoTransParams);
	UGbool  operator != (const UGGeoTransParams& GeoTransParams);
	UGGeoTransParams operator- ()const;
	
	//! \brief 根据转换方式获取地理坐标转换的参数个数 目前尚未实现	
	UGint		GetParamsCount(EmGeoTransMethod prjType);
	//! \brief 根据转换方式和序号获取参数名称 尚未实现
	UGString	GetParamName(UGint nIndex,EmGeoTransMethod prjType);
	//! \brief 根据转换方式和序号获取参数值 尚未实现
	UGdouble	GetParamValue(UGint nIndex,EmGeoTransMethod prjType);
	//! \brief 根据转换方式和序号设置参数值 尚未实现
	void		SetParamValue(UGint nIndex,UGdouble nValue,EmGeoTransMethod prjType);
	//! \brief 输出为XML字符串
	UGString	ToXML(UGint nVersion = 0);
	//! \brief 从XML字符串读取地理坐标转换参数
	UGbool		FromXML(const UGString& strPrjXml,UGint nVersion = 0);
public:
	//! \brief 0 X-axis translation ------ Geographic Transforms
	UGdouble m_dTranslateX;
	//! \brief 1 Y-axis translation
	UGdouble m_dTranslateY;
	//! \brief 2 Z-axis translation
	UGdouble m_dTranslateZ;
	//! \brief 3 X-axis rotation
	UGdouble m_dRotateX;
	//! \brief 4 Y-axis rotation
	UGdouble m_dRotateY;
	//! \brief 5 Z-axis rotation
	UGdouble m_dRotateZ;
	//! \brief 6 Scale difference
	UGdouble m_dScaleDifference;
	//! \brief 7 Rotation Origin X
	UGdouble m_dRotationOriginX;
	//! \brief 8 Rotation Origin Y
	UGdouble m_dRotationOriginY;
	//! \brief 9 Rotation Origin Z
	UGdouble m_dRotationOriginZ;
};


}//namespace UGC


#endif // !defined(UGC_SMTRANSLATEPARAMS_H__3173CAC9_B797_44C0_AC30_5F8888E57F0F__INCLUDED_)

