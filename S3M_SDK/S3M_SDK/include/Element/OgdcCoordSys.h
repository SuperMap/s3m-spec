// OgdcCoordSys.h: interface for the OgdcCoordSys class.
//
//////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)        
//                                                           
//!  \file OgdcCoordSys.h
//!  \brief 投影坐标系
//!  \details 
//!  \attention 
//!   Copyright(c) 2007-2012  SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OGDCCOORDSYS_H__B15D37E7_C1BB_45D9_AB5F_34B08BA3D5C4__INCLUDED_)
#define AFX_OGDCCOORDSYS_H__B15D37E7_C1BB_45D9_AB5F_34B08BA3D5C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OgdcPrjParams.h"
#include "Base/OgdcString.h"

namespace OGDC {

class ELEMENT_API OgdcCoordSys  
{
public:
	enum SpatialRefType
	{
		//! \brief 平面坐标系
		SPATIALREF_NONEARTH = 0,
		//! \brief 经纬坐标系
  		SPATIALREF_EARTH_LONGITUDE_LATITUDE = 1,
		//! \brief 投影坐标系
		SPATIALREF_EARTH_PROJECTION = 2,				
	};

	enum ProjectionType
	{
		//! \brief 非投影-- 
		PRJ_NONPROJECTION					= 43000,
		//! \brief Plate Carree   
		PRJ_PLATE_CARREE				    = 43001,
		//! \brief Equidistant Cylindrical 
		PRJ_EQUIDISTANT_CYLINDRICAL		    = 43002,
		//! \brief Miller Cylindrical 
		PRJ_MILLER_CYLINDRICAL			    = 43003, 
		//! \brief Mercator 
		PRJ_MERCATOR					    = 43004,
		//! \brief Gauss-Kruger
		PRJ_GAUSS_KRUGER				    = 43005,
		//! \brief Transverse Mercator ==43005  
		PRJ_TRANSVERSE_MERCATOR			    = 43006,
		//! \brief Albers
		PRJ_ALBERS                          = 43007,
		//! \brief Sinusoidal
		PRJ_SINUSOIDAL                      = 43008,
		//! \brief Mollweide
		PRJ_MOLLWEIDE                       = 43009,
		//! \brief Eckert VI 
		PRJ_ECKERT_VI                       = 43010,
		//! \brief Eckert V 
		PRJ_ECKERT_V                        = 43011,
		//! \brief Eckert IV
		PRJ_ECKERT_IV                       = 43012, 
		//! \brief Eckert III
		PRJ_ECKERT_III                      = 43013,
		//! \brief Eckert II
		PRJ_ECKERT_II                       = 43014,
		//! \brief Eckert I
		PRJ_ECKERT_I                        = 43015,                           
		//! \brief Gall Stereographic           
		PRJ_GALL_STEREOGRAPHIC              = 43016,      
		//! \brief Behrmann                     
		PRJ_BEHRMANN                        = 43017,      
		//! \brief Winkel I                     
		PRJ_WINKEL_I                        = 43018,      
		//! \brief Winkel II                    
		PRJ_WINKEL_II                       = 43019,      
		//! \brief Lambert Conformal Conic      
		PRJ_LAMBERT_CONFORMAL_CONIC         = 43020,      
		//! \brief Polyconic                    
		PRJ_POLYCONIC                       = 43021,      
		//! \brief Quartic Authalic             
		PRJ_QUARTIC_AUTHALIC                = 43022,      
		//! \brief Loximuthal                   
		PRJ_LOXIMUTHAL                      = 43023,      
		//! \brief Bonne                        
		PRJ_BONNE                           = 43024,      
		//! \brief Hotine                       
		PRJ_HOTINE                          = 43025,      
		//! \brief Stereographic                
		PRJ_STEREOGRAPHIC                   = 43026,      
		//! \brief Equidistant Conic            
		PRJ_EQUIDISTANT_CONIC               = 43027,      
		//! \brief Cassini                      
		PRJ_CASSINI                         = 43028,      
		//! \brief Van der Grinten I            
		PRJ_VAN_DER_GRINTEN_I               = 43029,      
		//! \brief Robinson                     
		PRJ_ROBINSON                        = 43030,      
		//! \brief Two-Point Equidistant        
		PRJ_TWO_POINT_EQUIDISTANT           = 43031,      
		//! \brief Equidistant Azimuthal        
		PRJ_EQUIDISTANT_AZIMUTHAL           = 43032,      
		//! \brief Lambert Azimuthal Equal Area 
		PRJ_LAMBERT_AZIMUTHAL_EQUAL_AREA    = 43033,      
		//! \brief Conformal Azimuthal 
		PRJ_CONFORMAL_AZIMUTHAL		        = 43034,      
		//! \brief ortho_graphic 正射 
		PRJ_ORTHO_GRAPHIC			        = 43035,      
		//! \brief Gnomonic 球心 
		PRJ_GNOMONIC				        = 43036,      
		//! \brief 中国全图方位投影 
		PRJ_CHINA_AZIMUTHAL			        = 43037,      
		//! \brief 桑逊投影——正弦曲线等积伪圆柱投影 
		PRJ_SANSON					        = 43040,	  
		//! \brief EqualArea Cylindrical        
		PRJ_EQUALAREA_CYLINDRICAL           = 43041,      
		//! \brief Hotine_azimuth_natorigin 
		PRJ_HOTINE_AZIMUTH_NATORIGIN        = 43042,
		//! \brief 斜轴墨卡托投影
		PRJ_OBLIQUE_MERCATOR				= 43043,
		//! \brief Hotine斜轴墨卡托投影
		PRJ_HOTINE_OBLIQUE_MERCATOR			= 43044,		
		//! \brief 正球墨卡托
		PRJ_SPHERE_MERCATOR					= 43045,		
		//! \brief 南半球彭纳投影
		PRJ_BONNE_SOUTH_ORIENTATED			= 43046,	
		PRJ_OBLIQUE_STEREOGRAPHIC           = 43047,     /* Oblique stereographic , Esri 称为 double stereographic*/ 
		PRJ_BAIDU_MERCATOR					= 43048     /* baidu Mercator*/ 
	};

public:
	//! \brief 默认构造函数
	OgdcCoordSys();
	//! \brief 默认析构函数
	virtual ~OgdcCoordSys();
	//! \brief 拷贝构造函数
	//! \param coordSys 用来拷贝的地理坐标系
	OgdcCoordSys(const OgdcCoordSys& coordSys);	
    //! \brief 重载 =
	//! \param coordSys 用来拷贝的地理坐标系
	OgdcCoordSys& operator=(const OgdcCoordSys& coordSys);
	//! \brief 重载 ==
	//! \param coordSys 用来比较的地理坐标系
	OgdcBool operator==(const OgdcCoordSys& coordSys);
	//! \brief 重载 !=
	//! \param coordSys 用来比较的地理坐标系
	OgdcBool operator!=(const OgdcCoordSys& coordSys);

public:
	//! \brief 得到坐标空间参考系类型。
	OgdcCoordSys::SpatialRefType GetSpatailRefType() const;	 

	//! \brief 设置坐标系为平面坐标系并设置坐标系单位
	//! \param nUnit 坐标系单位
	OgdcBool SetPlanar(OgdcInt nUnit);


	//! \brief 设置坐标系为地理坐标系并设置坐标系椭球体长轴半径，扁率和坐标系单位
	//! \param dPriAxis	椭球体长轴半径
	//! \param dFlatten	椭球体扁率
	//! \param nUnit 坐标系单位
	OgdcBool SetEarth(OgdcDouble dPriAxis, OgdcDouble dFlatten,	OgdcInt nUnit);
	
	//! \brief 设置坐标系为投影坐标系，并设置坐标系的投影方式，投影参数，椭球体长轴半径，椭球体扁率，坐标系单位
	//! \param  nType 投影方式
	//! \param  prjParams 投影参数对象	
	//! \param  dPriAxis 椭球体长轴半径
	//! \param  dFlatten 椭球体扁率	
	//! \param  dPrimeMeridian 本初子午线经度
	//! \param  nUnit 坐标系单位	
	//! \param  nDistUnit 坐标系量算单位
	OgdcBool SetCoordSys(ProjectionType nType,				
			const OgdcPrjParams& prjParams,								
			OgdcDouble dPriAxis,											
			OgdcDouble dFlatten,										
			OgdcDouble dPrimeMeridian,										
			OgdcInt nUnit,												
			OgdcInt nDistUnit);										


public:	
	//! \brief 投影坐标系的投影名称
	OgdcString m_strPrjName;	
	//! \brief 地理坐标系的名称
	OgdcString m_strEarthName;
	//! \brief 椭球体长轴半径（默认值：6378137）
	OgdcDouble m_dPriAxis;
	//! \brief 椭球体扁率（默认值：0.0033528107）
	OgdcDouble m_dFlatten;
	//! \brief 本初子午线经度（默认值：0.0）
	OgdcDouble m_dPrimeMeridian;
	//! \brief 坐标系单位（默认值：10000）
	OgdcInt m_nUnit;
	//! \brief 坐标系量算单位（默认值：10000）
	OgdcInt m_nDistUnit;
	//! \brief 投影方式（默认值：PRJ_NONPROJECTION）
	OgdcCoordSys::ProjectionType m_ProjectionType;
	//! \brief 投影参数对象
	OgdcPrjParams m_PrjParams;						

protected:
	//! \brief 坐标参考系类型（默认值：SPATIALREF_NONEARTH）
	OgdcCoordSys::SpatialRefType m_nRefType;		
};

}

#endif // !defined(AFX_OGDCCOORDSYS_H__B15D37E7_C1BB_45D9_AB5F_34B08BA3D5C4__INCLUDED_)

