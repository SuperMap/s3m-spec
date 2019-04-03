//========================================================================
// 项目名：			SuperMap Universal GIS class (UGC) Library 0.9
// 作  者：			张立立
// 单  位：			北京超图软件股份有限公司
// 最后修改时间：	以文件日期为准
//------------------------------------------------------------------------
//版权声明:         版权所有 2000-2010 北京超图软件股份有限公司，保留所有权利 
//	                Copyright (c) 2000-2010 SuperMap Software Co., Ltd. 
//	                All rights reserved

// 重要声明：		1. 
//					2. 
//					
//------------------------------------------------------------------------
// 文件名：			SmPrjParams.h
// 类  名：			UGPrjParams
// 父  类：			
// 子  类：			

// 功能说明：		投影参数定义
// 调用说明：		
/*-------------------------------------版本更新---------------------------
V 0.92
作    者：输入作者（或修改者）名字
完成日期：2002年10月20日
更新说明：

V 0.91
作    者：输入作者（或修改者）名字
完成日期：2001年7月20日
更新说明：

V 0.9 
原作者  ：张立立
完成日期：
========================================================================*/

#if !defined(UGC_SMPRJPARAMS_H__565B113E_EA70_4EC2_B1B5_9140AD14D73B__INCLUDED_)
#define UGC_SMPRJPARAMS_H__565B113E_EA70_4EC2_B1B5_9140AD14D73B__INCLUDED_


#include "Stream/ugdefs.h"

#include "Element/OgdcPrjParams.h"

#include "Projection/UGPjCon.h"

namespace UGC{

class PROJECTION_API UGPrjParams  
{
public:
	//! \brief 默认构造函数
	UGPrjParams();

#ifdef SYMBIAN60
	~UGPrjParams();
#else
	virtual ~UGPrjParams();
#endif

	UGPrjParams&  operator = (const UGPrjParams& PrjParams);
	UGbool  operator == (const UGPrjParams& PrjParams) const;
	UGbool  operator != (const UGPrjParams& PrjParams) const;
	
	/*
//	UGProjection GetProjection();
//	void		SetProjection(const UGProjection& prj);
	UGint		GetParamsCount(EmPrjObjectType prjType);
	UGString	GetParamName(UGint nIndex,EmPrjObjectType prjType);
	UGdouble		GetParamValue(UGint nIndex,EmPrjObjectType prjType);
	void		SetParamValue(UGint nIndex,UGdouble nValue,EmPrjObjectType prjType);
	*/
	//! \brief 输出为XML字符串
	UGString ToXML(UGint nVersion = 0) const;
	//! \brief 从XML字符串中读入
	UGbool FromXML(const UGString& strPrjXml, UGint nVersion = 0);

	//! \brief 输出为投影参数
	UGbool ToOgdcPrjParams(OgdcPrjParams& prjParams) const;
	//! \brief 从投影参数中读入
	UGbool FromOgdcPrjParams(const OgdcPrjParams& prjParams);
	UGString GetPrjParamString(UGbool blon_0 = true) const ;//for prj4 
public:
//	UGProjection m_Projection;
	//! \brief 0 False easting (X0)
	UGdouble m_dFalseEasting;			
	//! \brief 1 False northing (Y0)
	UGdouble m_dFalseNorthing;
	//! \brief 2 Central meridian, Longitude of origin (LON0)
	UGdouble m_dCentralMeridian;
	//! \brief 6 Central parallel, Latitude of origin	(LAT0)
	UGdouble m_dCentralParallel;
	//! \brief 3 Standard parallel 1, Latitude of 1st point (LAT1)
	UGdouble m_dStandardParallel1;
	//! \brief 4 Standard parallel 2, Latitude of 2nd point (LAT2)
	UGdouble m_dStandardParallel2;
	//! \brief 5 Scale factor
	UGdouble m_dScaleFactor;
	//! \brief 7 Azimuth
	UGdouble m_dAzimuth;
	//! \brief 8 Longitude of 1st point (LON1)
	UGdouble m_dFirstPointLongitude;
	//! \brief 9 Longitude of 2nd point  (LON2)
	UGdouble m_dSecondPointLongitude;
	//! \brief 10 XY_Plane_Rotation(Rectified Angle)
	UGdouble m_dRectifiedAngle;
};


}//namespace UGC



#endif // !defined(UGC_SMPRJPARAMS_H__565B113E_EA70_4EC2_B1B5_9140AD14D73B__INCLUDED_)

