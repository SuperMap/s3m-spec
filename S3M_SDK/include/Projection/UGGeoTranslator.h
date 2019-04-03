//=====================================================================================
// 项目名：			SuperMap Universal GIS class (UGC) Library 0.9
// 作  者：			张立立
// 单  位：			北京超图软件股份有限公司
// 最后修改时间：	以文件日期为准
//-------------------------------------------------------------------------------------
//版权声明:         版权所有 2000-2010 北京超图软件股份有限公司，保留所有权利 
//	                Copyright (c) 2000-2010 SuperMap Software Co., Ltd. 
//	                All rights reserved

// 重要声明：		1. 
//					2. 
//					
//-------------------------------------------------------------------------------------
// 文件名：			UGGeoTranslator.h 
// 类  名：			UGGeoTranslator
// 父  类：			
// 子  类：			

// 功能说明：		参照系变化器
// 调用说明：		1、调用SetGeoCoordSysSrc 设置源地理坐标系
//                  2、调用SetGeoCoordSysDes 设置目标地理坐标系 
//                  3、调用Translate 方法    进行参照系变换  

/*-------------------------------------版本更新---------------------------

$Revision: 1.3 $
$Log: UGGeoTranslator.h,v $
Revision 1.3  2009/11/05 09:34:23  siyh
修改头文件注释

Revision 1.2  2008/07/25 07:44:41  gaoff
高凤飞
修改注释中 公司的 中/英文名称

Revision 1.1  2008/04/29 07:02:40  lugw
*** empty log message ***

Revision 1.8  2008/04/07 01:52:39  dongfei
添加注释

Revision 1.7  2007/12/17 08:13:37  dongfei
添加注释

Revision 1.6  2007/11/15 10:52:53  dongfei
*** empty log message ***

Revision 1.5  2005/07/18 15:23:14  zhangli
增加文件说明和注释

$Author: siyh $
$Date: 2009/11/05 09:34:23 $

========================================================================*/

#if !defined(UGC_SMGEOTRANSLATOR_H__E2046C97_6C45_4741_8CDA_576CE89E1574__INCLUDED_)
#define UGC_SMGEOTRANSLATOR_H__E2046C97_6C45_4741_8CDA_576CE89E1574__INCLUDED_

#include "Stream/ugdefs.h"
#include "Projection/UGGeoCoordSys.h"
#include "Projection/UGTranslateParams.h"


namespace UGC{


class PROJECTION_API UGGeoTranslator  
{
public:
//	typedef enum tagGeoTransMethod
//	{
//		MTH_GEOCENTRIC_TRANSLATION = 9603, /* Geocentric Translation (3-par.)*/
//		MTH_MOLODENSKY             = 9604, /* Molodensky                     */
//		MTH_MOLODENSKY_ABRIDGED    = 9605, /* Abridged Molodensky            */
//		MTH_POSITION_VECTOR        = 9606, /* Position Vector (7-par.)       */
//		MTH_COORDINATE_FRAME       = 9607, /* Coordinate Frame (7-par.)      */
//		MTH_BURSA_WOLF             = (9607+33000), /* Bursa-Wolf             */
//	}	GeoTransMethod;
	//! \brief 默认构造函数
	UGGeoTranslator();

#ifdef SYMBIAN60
	~UGGeoTranslator();
#else
	virtual ~UGGeoTranslator();
#endif
	//! \brief 地理坐标转换
	//! \param dL 经度
	//! \param dB 纬度
	//! \param dH 高程
	//! \param nGeoTransMethod 转换方法
	//! \param bForward 是否正转换
	//! \return 是否转换成功
	UGbool Translate(UGdouble& dL, UGdouble& dB,UGdouble& dH,
				   EmGeoTransMethod nGeoTransMethod =MTH_GEOCENTRIC_TRANSLATION ,
				   UGbool bForward = true);
	//! \brief 地理坐标转换
	//! \param dL 经度
	//! \param dB 纬度	
	//! \param nGeoTransMethod 转换方法
	//! \param bForward 是否正转换
	//! \return 是否转换成功
	UGbool Translate(UGdouble& dL, UGdouble& dB,
				   EmGeoTransMethod nGeoTransMethod =MTH_GEOCENTRIC_TRANSLATION ,
				   UGbool bForward = true);

	//! \brief 设置源地理坐标系
	UGbool SetGeoCoordSysSrc(UGGeoCoordSys* pGeoCoordSysSrc);

	//! \brief 设置目标地理坐标系
	UGbool SetGeoCoordSysDes(UGGeoCoordSys* pGeoCoordSysDes);
	//! \brief 返回源地理坐标系
	UGGeoCoordSys* GetGeoCoordSysSrc();
	//! \brief  返回目标地理坐标西
	UGGeoCoordSys* GetGeoCoordSysDes();
	
	//! \brief 获取地理坐标变换参数
	UGGeoTransParams& GetGeoTransParams();
	//! \brief 设置地理坐标变换参数
	void SetGeoTransParams(const UGGeoTransParams& GeoTransParams);
	
protected:
	UGbool Schedule();

	UGGeoTransParams m_GeoTransParams;
//	UGGeoTransParams m_GeoTransParams_Inverse;
	UGGeoCoordSys* m_pGeoCoordSysSrc;
	UGGeoCoordSys* m_pGeoCoordSysDes;

	UGdouble m_dPrimaryMdnDif; // 目标坐标系的本初子午线 - 源坐标系的本初子午线
	UGdouble m_dUnitRatio1;     // 源坐标系的单位 / AU_DEGREE
	UGdouble m_dUnitRatio2;		// AU_DEGREE / 目标坐标系的单位

	UGint m_nStatus;// m_nSatus  =0 相同的地理坐标系,无需转换 
					// m_nStatus =1 参照系相同,只是本初子午线或坐标单位不同 
					// m_nStatus =2 参照系不同,需要使用数值方法转换
protected:
	UGbool GeoCentricTrans3(UGdouble& dL, UGdouble& dB,UGdouble& dH,const UGGeoTransParams& GeoTransParams, UGbool bForward = TRUE);
	UGbool Molodensky(UGdouble& dL, UGdouble& dB,UGdouble& dH,const UGGeoTransParams& GeoTransParams, UGbool bForward = TRUE);
	UGbool AbridgedMolodensky(UGdouble& dL, UGdouble& dB,UGdouble& dH,const UGGeoTransParams& GeoTransParams, UGbool bForward = TRUE);
	UGbool GeoCentricTrans7(UGdouble& dL, UGdouble& dB,UGdouble& dH,const UGGeoTransParams& GeoTransParams, UGbool bForward = TRUE);
	UGbool MolodenskyBadekas(UGdouble& dL, UGdouble& dB,UGdouble& dH,const UGGeoTransParams& GeoTransParams, UGbool bForward = TRUE);

	// LB 使用角度单位 H 使用米
	UGbool LBH2XYZ(UGdouble dL,UGdouble dB, UGdouble dH,UGPoint3D& pntXYZ, UGbool bForward = TRUE);
	
	UGbool XYZ2LBH(const UGPoint3D& pntXYZ,UGdouble& dL,UGdouble& dB,UGdouble& dH, UGbool bForward = TRUE );

	// 
	UGbool _LBH2XYZ(UGdouble dL,UGdouble dB, UGdouble dH,UGPoint3D& pntXYZ, UGbool bForward = TRUE);
	UGbool _XYZ2LBH(const UGPoint3D& pntXYZ,UGdouble& dL,UGdouble& dB,UGdouble& dH, UGbool bForward = TRUE );

	UGbool China_3D_7P(UGdouble& dL, UGdouble& dB,UGdouble& dH,const UGGeoTransParams& GeoTransParams, UGbool bForward = TRUE);
	UGbool China_2D_7P(UGdouble& dL, UGdouble& dB,UGdouble& dH,const UGGeoTransParams& GeoTransParams, UGbool bForward = TRUE);
	UGbool China_2D_4P(UGdouble& dX, UGdouble& dY,UGdouble& dH,const UGGeoTransParams& GeoTransParams, UGbool bForward = TRUE);

private:
	UGdouble m_dFlattenDes;		//目标参考系的扁率
	UGdouble m_dEPSDes2;		//目标参考系的第一偏心率的平方
	UGdouble m_dAxisDes;		//目标参考系的长轴半径
	UGdouble m_dAxis;			//源参考系的长轴半径
	UGdouble m_dEPS2;			//源参考系的第一偏心率的平方

	UGdouble m_dDeltaAxis;		//长轴半径之差
	UGdouble m_dFlatten;		//源参考系的扁率
	UGdouble m_dDeltaFlatten;	//扁率之差
	
	UGdouble GetW(UGdouble phi, UGbool bForward = TRUE);
	
	
};

}//namespace UGC


#endif // !defined(UGC_SMGEOTRANSLATOR_H__E2046C97_6C45_4741_8CDA_576CE89E1574__INCLUDED_)

