//////////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)                
//                                                           
//!  \file OgdcColorset.h
//!  \brief 颜色集基类
//!  \attention 
//!   Copyright (c) 2007-2012 SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OGDCCOLORSET_H__6FC58E88_B3CD_4B04_BC40_DEC2E08042FD__INCLUDED_)
#define AFX_OGDCCOLORSET_H__6FC58E88_B3CD_4B04_BC40_DEC2E08042FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ogdcdefs.h"
#include "OgdcArray.h"

namespace OGDC{

#define MAXCOLORCOUNT 0x7fff

//! \brief 渐变颜色
typedef enum tagSM_ENUM_STOCK_GRADIENT_SET_STYLE
{
	//! \brief 黑->白
	gtBlackWhite,
	//! \brief 红->白
	gtRedWhite,	
	//! \brief 绿->白
	gtGreenWhite,
	//! \brief 蓝->白
	gtBlueWhite,
	//! \brief 黄->白
	gtYellowWhite,
	//! \brief 粉红->白
	gtPinkWhite,
	//! \brief 青->白
	gtCyanWhite,
	//! \brief 红->黑
	gtRedBlack,	
	//! \brief 绿->黑
	gtGreenBlack,
	//! \brief 蓝->黑
	gtBlueBlack,
	//! \brief 黄->黑
	gtYellowBlack,
	//! \brief 粉红->黑
	gtPinkBlack,
	//! \brief 青->黑
	gtCyanBlack,
	//! \brief 黄->红
	gtYellowRed,
	//! \brief 黄->绿
	gtYellowGreen,
	//! \brief 黄->蓝
	gtYellowBlue,
	//! \brief 绿->蓝
	gtGreenBlue,
	//! \brief 绿->黑
	gtGreenRed,	
	//! \brief 蓝->红
	gtBlueRed,
	//! \brief 青->红			
	gtPinkRed,
	//! \brief 青->蓝			
	gtPinkBlue,
	//! \brief 青->蓝			
	gtCyanBlue,
	//! \brief 青->绿			
	gtCyanGreen,
	//! \brief 彩虹
	gtRainbow,
	//! \brief 绿->桔黄->紫罗兰				
	gtGreenOrangeViolet,
	//! \brief 地形渐变,用于三维显示效果较好
	gtTerrain,
	//! \brief 光谱渐变			
	gtSpectrum				
}OGDC_ENUM_STOCK_GRADIENT_SET_STYLE;	

//!  \brief 颜色集基类
class BASE_API OgdcColorset:public OgdcArray<OgdcColor>
{
public:
	//!  \brief 颜色集基类默认构造函数
	OgdcColorset();
	//!  \brief 颜色集基类默认析构函数
	~OgdcColorset();  
	//!  \brief 颜色集基类拷贝构造函数
	OgdcColorset(const OgdcColorset& colorset);
	//!  \brief 重载=
	OgdcColorset& operator=(const OgdcColorset& colorset);
	//!  \brief 设置颜色集的颜色数
	void SetColorSize(OgdcInt nSize);

public:
	OgdcInt GetSize() const { return (OgdcInt)OgdcArray<OgdcColor>::GetSize(); };
	//! \brief 		指定颜色生成随机渐变色
	//! \param 		nCount  颜色数
	//! \param 		nIndex  颜色集类型
	//! \remarks 	如果返回0表示创建颜色集合不成功。	
	OgdcInt MakeStockRandom(OgdcInt nCount, OgdcInt nIndex,OgdcBool bReverse = FALSE);
	
	//! \brief 		生成随机渐变色
	//! \param 		nCount			颜色数
	//! \param 		btRedStart		Red分量起始值
	//! \param 		btRedEnd		Red分量终止值
	//! \param 		btGreenStart    Green分量起始值
	//! \param 		btGreenEnd		Green分量终止值
	//! \param 		btBlueStart		Blue分量起始值
	//! \param 		btBlueEnd		Blue分量终止值
	//! \param 		FALSE			是否灰度
	//! \remarks 	如果返回0表示创建颜色集合不成功。	
	OgdcInt MakeRandom(OgdcInt nCount, OgdcByte btRedStart,OgdcByte btRedEnd,
								OgdcByte btGreenStart, OgdcByte btGreenEnd,
								OgdcByte btBlueStart,OgdcByte btBlueEnd,
								OgdcBool bIsGray = FALSE);

	//! \brief 		指定颜色生成等距渐变色颜色集
	//! \param 		nCount					颜色数
	//! \param 		nStockGradientStyle     颜色集类型
	//! \param 		bReverse=FALSE			是否反向
	//! \remarks 	如果返回0表示创建颜色集合不成功。	
	OgdcInt MakeStockGradient(OgdcInt nCount, OGDC_ENUM_STOCK_GRADIENT_SET_STYLE nStockGradientStyle, OgdcBool bReverse=FALSE);
	
	//! \brief 		生成等距渐变色颜色集
	//! \param 		&arrayControlColors  
	//! \param 		&arrayIntervals
	//! \remarks 	计算多控制点非等距渐变颜色集，其中：arrayControlColors为控制点颜色集合,
	//!				arrayIntervals数组记录每两个控制点颜色之间的颜色数量，可以为0。因此，
	//!				arrayIntervals数组的大小必须等于arrayControlColors数组大小减去一，即：
	//!				arrayIntervals.GetSize()+1 == arrayControlColors.GetSize()。如果成功，
	//!				本函数返回生成的颜色集合大小，该大小应该等于arrayIntervals中各项之和。
	OgdcInt MakeGradient(OgdcColorset &arrayControlColors, OgdcArray<OgdcInt> &arrayIntervals);
	

	//! \brief 		计算多控制点等距渐变颜色集
	//! \param 		nCount				要生成的颜色集合中的颜色数量
	//! \param 		&arrayCotrolColors	控制点颜色集合
	//! \remarks 	如果返回0表示创建颜色集合不成功	
	OgdcInt MakeGradient(OgdcInt nCount, OgdcColorset &arrayCotrolColors);
		

	//! \brief 	    计算多控制点等距渐变颜色集	
	//! \param 		nCount		颜色数量	
	//! \param 		colorStart  开始颜色值
	//! \param 		colorEnd	结束颜色值
	//! \remarks 	如果返回0表示创建颜色集合不成功。	
	OgdcInt MakeGradient(OgdcInt nCount, OgdcColor clrStart, OgdcColor clrEnd);

	//! \brief	生成随机渐变色
	OgdcInt MakeRandom(OgdcInt nCount);

	//! \brief 		生成随机渐变色
	//! \param 		nCount				要生成的颜色集合中颜色总数
	//! \param 		&arrayCotrolColors	控制点颜色集合
	//! \remarks 	如果返回0表示创建颜色集合不成功	
	//added by zhengyl 2011-2-17
	OgdcInt MakeRandom(OgdcInt nTotalCount, OgdcColorset &arrayCotrolColors);

	
};

}

#endif // !defined(AFX_OGDCCOLORSET_H__6FC58E88_B3CD_4B04_BC40_DEC2E08042FD__INCLUDED_)

