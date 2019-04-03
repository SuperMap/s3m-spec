//////////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)           
//                                                           
//!  \file OgdcColorTable.h
//!  \brief 分层设色颜色集 
//!  \attention 与OgdcColorset的结合,支持分层设色和计算光照颜色
//!   Copyright (c) 2007-2012 SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OGDCCOLORTABLE_H__B6F4BBA2_614E_4B02_83B3_FA02C425A6EC__INCLUDED_)
#define AFX_OGDCCOLORTABLE_H__B6F4BBA2_614E_4B02_83B3_FA02C425A6EC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ogdcdefs.h"
#include "OgdcString.h"
#include "OgdcArray.h"
#include "OgdcColorset.h"

namespace OGDC {
//!  \brief 颜色表类 
class BASE_API OgdcColorTable
{
public:
	//! \brief 默认构造函数
	OgdcColorTable();
	//! \brief 默认析构函数
	~OgdcColorTable(); //这个不应该用虚的析构函数
	//! \brief 拷贝构造函数
	//! \param 		colorTable 用来拷贝的颜色表
	OgdcColorTable(const OgdcColorTable& colorTable);
	//! \brief 重载操作符 =
	//! \param 		colorTable 用来拷贝的颜色表
	OgdcColorTable& operator=(const OgdcColorTable& colorTable);

public:
    
	//! \brief 	得到颜色数
	//! \return 	颜色数目
	OgdcInt GetSize() const;

	//! \brief 		设置颜色数
	//! \param 		nNewValue 颜色数
	void SetSize(OgdcInt nNewValue);

	//! \brief 		把颜色加入颜色集
	//! \param 		clr	要添加的颜色集
	//! \return 	返回结果颜色集大小
	OgdcLong AddColor(OgdcColor clr);

	//! \brief 		添加新的颜色(RGB模式)
	//! \param 		nRed 红颜色分量
	//! \param 		nGreen 绿颜色分量
	//! \param 		nBlue 蓝颜色分量
	//! \return 	返回结果颜色集大小
	OgdcLong Add(OgdcUchar nRed, OgdcUchar nGreen, OgdcUchar nBlue);

	//! \brief 		将m_Colorset导出到颜色集colorset中	
	//! \param 		colorset 目标颜色集
	//! \return 	导出成功，返回true；导出失败返回false
	//! \remark     若colorset不为空，则会将其清空再进行导出
	OgdcBool ToColorset(OgdcColorset& colorset) const;

	//! \brief 		导入颜色集colorset到m_Colorset中
	//! \param 		colorset 源颜色集
	//! \return 	导入成功，返回true；导入失败返回false
	OgdcBool FromColorset(const OgdcColorset &colorset);

	//! \brief 		根据索引获取颜色集中的颜色
	//! \param 		nIndex 颜色索引
	//! \return 	索引nIndex对应的颜色
	OgdcColor GetRGB(OgdcInt nIndex);

	//! \brief 		修改颜色集中的颜色
	//! \param 		nIndex 要修改的索引
	//! \param 		nNewValue 新的颜色值		
	void SetColor(OgdcInt nIndex, OgdcInt nNewValue);


	//! \brief 		根据高度取得颜色分量，可以应用于分层设色
	//! \param 		dZ 要获取的高度
	//! \param 		dR 存储红颜色分量
	//! \param 		dG 存储绿颜色分量
	//! \param 		gB 存储蓝颜色分量
	//! \return 	成功返回true，失败返回false.
	OgdcBool GetColor(OgdcDouble dZ,OgdcDouble& dR,OgdcDouble& dG,OgdcDouble& dB);
	
	//! \brief 		获取颜色集中的最小值
	//! \return 	返回颜色集中的颜色的最小值		
	OgdcDouble GetFloor()const;

	//! \brief 		设置颜色集中的最小值
	//! \param 		newValue		
	void SetFloor(OgdcDouble dNewValue);

	//! \brief 		获取颜色集中的最大值
	//! \return 	颜色集最大值		
	OgdcDouble GetCeiling()const;

	//! \brief 		设置颜色集中的最大值
	//! \param 		dNewValue 颜色集最大值		
	void SetCeiling(OgdcDouble dNewValue);

	//! \brief 		获得水平线高度
	//! \return 	返回水平线高度		
	OgdcDouble GetWaterLevel() const;

	//! \brief 		设置水平线高度
	//! \param 		dNewValue 水平线高度		
	void SetWaterLevel(OgdcDouble dNewValue);

	//! \brief 		颜色集颠倒排序	
	void SwapIndex();

	//! \brief 		红蓝颜色分量互换		
	void SwapRedBlue();

	//! \brief 		创建灰度渐变颜色集
	//! \return 	灰色渐变颜色集的大小
	//! \remarks    颜色数由当前m_Colorset的数目决定
	OgdcInt CreateGrey();

	//! \brief 		创建红色渐变颜色集
	//! \return 	红色渐变颜色集的大小
	//! \remarks	颜色数由当前m_Colorset的数目决定
	OgdcInt CreateRed();

	//! \brief 		创建绿色渐变颜色集
	//! \return 	绿色渐变颜色集的大小	
	//! \remarks	颜色数由当前m_Colorset的数目决定
	OgdcInt CreateGreen();

	//! \brief 		创建蓝色渐变颜色集
	//! \return 	渐变颜色集的大小		
	//! \remarks	颜色数由当前m_Colorset的数目决定
	OgdcInt CreateBlue();

	//! \brief 		创建地形渐变颜色集
	//! \return 	颜色集的大小
	//! \remarks 	颜色数由当前m_Colorset的数目决定。如果设置水线，那么要初始化两个Colorset	
	OgdcInt CreateDefault();
	
	
	//! \brief 		根据高度取得颜色，可以应用于分层设色
	//! \param 		dZValue	高度值
	//! \return 	获取的颜色
	OgdcColor GetColor(OgdcDouble dZValue);
	
	//! \brief 		生成渐变颜色集
	//! \param 		nCount				颜色数
	//! \param 		nStockGradientStyle 渐变类型
	//! \param 		bReverse			是否反向
	//! \return 	渐变颜色集大小，若返回0，则创建失败
	OgdcInt CreateStockGradient(OgdcInt nCount, OGDC_ENUM_STOCK_GRADIENT_SET_STYLE nStockGradientStyle, OgdcBool bReverse = FALSE);

	//! \brief 		创建渐变颜色集
	//! \param 		nCount		颜色数量	
	//! \param 		colorStart  开始和颜色值
	//! \param 		colorEnd	结束颜色值	
	//! \return 	渐变颜色集大小，若返回0，则创建失败
	OgdcInt CreateGradient(OgdcInt nCount, OgdcColor clrStart,OgdcColor clrEnd);

	//! \brief 		根据高度取得颜色
	//! \param 		dZValue 高度值
	//! \return 	取得的颜色值
	OgdcColor InterColor(OgdcDouble dZValue);
	
	//! \brief 		取得红颜色分量
	//! \param 		nIndex 颜色的索引
	//! \return 	红颜色分量值
	OgdcShort GetRed(OgdcInt nIndex);

	//! \brief 		设置红颜色分量
	//! \param 		nIndex 索引
	//! \param 		nNewValue 红颜色分量的新值 		
	void SetRed(OgdcInt nIndex,OgdcUchar nNewValue);

	//! \brief 		取得绿颜色分量
	//! \return 	绿颜色分量值
	//! \param 		nIndex 颜色的索引	
	OgdcShort GetGreen(OgdcInt nIndex);

	//! \brief 		设置绿颜色分量
	//! \param 		nIndex 索引
	//! \param 		nNewValue 绿颜色分量的新值 	
	void SetGreen(OgdcInt nIndex,OgdcUchar nNewValue);

	//! \brief 		取得蓝颜色分量
	//! \param 		nIndex 颜色的索引		
	//! \return 	蓝颜色分量值
	OgdcShort GetBlue(OgdcInt nIndex);

	//! \brief 		设置蓝颜色分量
	//! \param 		nIndex 索引
	//! \param 		nNewValue 蓝颜色分量的新值 		
	void SetBlue(OgdcInt nIndex,OgdcUchar nNewValue);

	//! \brief     设置是否使用水平线
	void SetUsingWaterLevel(OgdcBool isUsing);

	//! \brief     获取是否使用水平线
	OgdcBool GetUsingWaterLevel();

private:
	OgdcColor InterColor(OgdcColorset &colorset,OgdcDouble dUpperLimit,OgdcDouble dLowerLimit,OgdcDouble dValue) const;

public:
	//! \brief 	内部颜色集
	OgdcColorset m_Colorset;
	//! \brief 	内部颜色基准值
	OgdcColorset m_ColorsetWater;

	// {{add by Jingfei 2012-06-12 增加颜色对照表的支持
	OgdcBool m_bUseColorDictTable;		// 是否使用颜色对照表
	OgdcArray<OgdcDouble> m_arrValue;	// 高程值数组
	// }}end

protected:
	//! \brief 最大颜色值
	OgdcDouble m_dCeiling;
	//! \brief 最小颜色值
	OgdcDouble m_dFloor;
	//! \brief 水平线高度
	OgdcDouble m_dWaterLevel;
	//! \brief 是否为水平线高度
	OgdcBool m_bWaterLevel;
};

}


#endif // !defined(AFX_OGDCCOLORTABLE_H__B6F4BBA2_614E_4B02_83B3_FA02C425A6EC__INCLUDED_)

