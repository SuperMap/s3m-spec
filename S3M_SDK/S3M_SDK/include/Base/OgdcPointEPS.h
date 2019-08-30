//////////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)                
//                                                           
//!  \file OgdcPointEPS.h
//!  \brief 清华山维 CPoint3D 类型
//!  \details  
//!  \attention 
//!   Copyright (c) 2007-2012 SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OGDCPOINTEPS_H_)
#define AFX_OGDCPOINTEPS_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Base/OgdcString.h"
#include "Base/OgdcPoint2D.h"
#include "Base/OgdcPoint3D.h"
#include "Base/OgdcArray.h"

namespace OGDC {
//空间点
typedef enum PointInfoEnum
{
	e_Survey_Point    = 0x0001,	  //实测的或计算的
	e_Height_Mark     = 0x0002,	  //高程注记否
	e_Enter_Dtm       = 0x0004,   //参加建模否
	e_New_Point       = 0x0008,   //新产生的点或旧点
	e_Turn_Point      = 0x0010,   //转向点或一般点
	e_Break_Point     = 0x0020,   //断点
	e_LType_Line      = 0x0040,   //直线
	e_LType_Curve     = 0x0080,   //曲线
	e_LType_Arc       = 0x00c0,   //圆弧
	e_Flag_Point1     = 0x0100,   //标志1, 为方便内部计算而设置的标志位，不存数据库，使用后应清空
	e_Flag_Point2     = 0x0200,   //标志2
	e_Flag_Point3     = 0x0400,   //标志3
	e_Flag_Point4     = 0x0800,   //标志4
	e_Attach_Point    = 0x1000,   //依附点
	e_Formicate_Point = 0x2000,   //群集点
	e_Smooth_Point    = 0x4000,   //平滑点
	e_Framework_Point = 0x8000,   //特征点
	e_Unknown_Point   = 0xf000,   //未知点
}PointEPSType;

//!  \brief 三维坐标点，OgdcDouble型，一般用来描述地理坐标的三维点。
class BASE_API OgdcPointEPS
{
public:
	//! \brief 点的x轴坐标
	OgdcDouble x;
	//! \brief 点的y轴坐标
	OgdcDouble y;
	//! \brief 点的z轴坐标
	OgdcDouble z;
	//! \brief 点的类型
	OgdcInt nType;
	//! \brief 点的名称
	OgdcString strName;
public:
	//! \brief  	默认构造函数
	OgdcPointEPS();
	//! \brief  	缺省析构函数
	~OgdcPointEPS();

	//! \brief  带参数构造函数
	OgdcPointEPS(OgdcDouble xx,OgdcDouble yy);
	OgdcPointEPS(OgdcDouble xx,OgdcDouble yy,OgdcDouble zz);
	OgdcPointEPS(OgdcDouble xx,OgdcDouble yy,OgdcDouble zz, OgdcInt type);

public:	
	//! \brief 		重载 = 
	OgdcPointEPS& operator=(const OgdcPointEPS& s);
	//! \brief 		重载 - 负号
	OgdcPointEPS operator-() const;
	//! \brief 		重载 == 
	friend BASE_API OgdcBool operator==(const OgdcPointEPS& p,const OgdcPointEPS& q);
	//! \brief 		重载 !=
	friend BASE_API OgdcBool operator!=(const OgdcPointEPS& p,const OgdcPointEPS& q);			
	//获得点类型信息
	//进口参数: eInfo(点类型信息枚举)
	//返回值: TRUE表示是该类型的点，FALSE表示点不是该类型的点
	OgdcBool GetInfo(PointInfoEnum eInfo) const
	{
		OgdcBool state = nType & eInfo ;
		return state;
	}
	//设置点类型信息
	//进口参数: bFlag(TRUE表示设置点是该类型的点，FALSE表示设置点不是该类型的点),eInfo(点类型信息枚举)
	void SetInfo(PointInfoEnum eInfo, OgdcBool bFlag)
	{
		if(bFlag)
			nType |= eInfo;
		else 
			nType &= ~eInfo;
	}

	OgdcPoint2D ToPoint2D();

	OgdcPoint3D ToPoint3D();
};

typedef OgdcArray<OgdcPointEPS> OgdcPointEPSs;
}


#endif // !defined(AFX_OGDCPOINTEPS_H_)

