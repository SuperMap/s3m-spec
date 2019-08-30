//////////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)               
//                                                           
//!  \file OgdcRectF.h
//!  \brief 矩形对象类 
//!  \details  
//!  \attention 一般用于屏幕坐标的高精度矩形对象
//!   Copyright (c) 2007-2012 SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OGDCRECTF_H__INCLUDED_)
#define AFX_OGDCRECTF_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OgdcPoint2D.h"

namespace OGDC {
//!  \brief 矩形对象类，存储了矩形的左上角点、右下角点的(x,y)坐标，坐标值为OgdcDouble型。
class BASE_API OgdcRectF  
{
public:
	//! \brief 矩形左上角点的x轴坐标
	OgdcDouble left;
	//! \brief 矩形左上角点的y轴坐标
	OgdcDouble top;
	//! \brief 矩形右下角点的x轴坐标
	OgdcDouble right;
	//! \brief 矩形右下角点的y轴坐标
	OgdcDouble bottom;		

public:

	//! \brief 		缺省构造函数
	OgdcRectF();	

	//! \brief 		缺省解析函数
	~OgdcRectF();	

	//! \brief 		构造函数一个二维矩形对象
	//! \param 		nLeft	矩形左上角点的x轴坐标
	//! \param 		nTop	矩形左上角点的y轴坐标
	//! \param 		nRight	矩形右下角点的x轴坐标
	//! \param 		nBottom	矩形右下角点的y轴坐标
	//! \remarks 	请确保nLeft<nRight,nTop<nBottom
	OgdcRectF(OgdcDouble nLeft,OgdcDouble nTop,OgdcDouble nRight,OgdcDouble nBottom);
	
	//! \brief 		带参构造函数
	//! \param 		pnt	左上角坐标
	//! \param 		sz	矩形的大小	
	OgdcRectF(const OgdcPoint2D& pnt,const OgdcSize2D& sz);

	//! \brief 		带参构造函数
	//! \param 		pntTL	左上角坐标点
	//! \param 		pntBR   右下角坐标点	
	OgdcRectF(const OgdcPoint2D& pntTL,const OgdcPoint2D& pntBR);	
 	
	//! \brief 		重载 =
    OgdcRectF& operator=(const OgdcRectF& rc);

	//! \brief 		重载 ==
	friend BASE_API OgdcBool operator==(const OgdcRectF& rcStart,const OgdcRectF& rcEnd);

	//! \brief 		重载 !=
	friend BASE_API OgdcBool operator!=(const OgdcRectF& rcStart,const OgdcRectF& rcEnd);
	
	//! \brief 		设置矩形参数
	//! \param 		nLeft	矩形左上角点的x轴坐标
	//! \param 		nTop	矩形左上角点的y轴坐标
	//! \param 		nRight	矩形右下角点的x轴坐标
	//! \param 		nBottom	矩形右下角点的y轴坐标
	void SetRect(OgdcDouble nLeft,OgdcDouble nTop,OgdcDouble nRight,OgdcDouble nBottom);

	//! \brief 		设置矩形参数
	//! \param 		pntTL	左上角坐标
	//! \param 		pntBR	右下角坐标	
	void SetRect(const OgdcPoint2D& pntTL,const OgdcPoint2D& pntBR);

	//! \brief 		将矩形置为空
	//! \remarks 	左上角、右下角点坐标都设为0
	void SetEmpty();

	//! \brief 		判断矩形是否为空或是否合法
	//! \return 	矩形为空返回true，矩形不为空返回false
	//! \remarks 	如果不符合left<right,top<bottom规则，返回true，认为矩形为空。
	OgdcBool IsEmpty() const;

	//! \brief 		矩形是否为未初始化
	//! \return 	如果符合left==right==top==bottom==0规则，则返回true，否则，返回false
	OgdcBool IsNull() const;

	//! \brief 		矩形的宽度
	//! \return 	返回right-left的值(整型)
	OgdcDouble Width() const;

	//! \brief 		矩形的高度
	//! \return 	返回bottom-top的值(整型)
	OgdcDouble  Height() const;

	//! \brief 		获取矩形的左上角点
	OgdcPoint2D& TopLeft();

	//! \brief 		获取矩形的左上角点
	//! \remarks 	如果矩形对象为const类型，则将调用本接口
	const OgdcPoint2D& TopLeft() const;	

	//! \brief 		获取矩形右下角坐标点
	OgdcPoint2D& BottomRight();		

	//! \brief 		获取矩形右下角坐标点
	//! \remarks 	如果矩形对象为const类型，则将调用本接口
	const OgdcPoint2D& BottomRight() const;
	
	//! \brief 		获取矩形的中心点
	OgdcPoint2D CenterPoint() const;
	
	//! \brief 		获取矩形的大小
	OgdcSize2D Size() const;

	//! \brief 		交换left与right的值
	void SwapLeftRight();

	//! \brief 		交换top与bottom的值	
	void SwapTopBottom();	

	//! \brief 		判断点是否在矩形中
	//! \param 		pnt	坐标点
	//! \return		若点在矩形中或在矩形边界上，则返回true，否则返回false。
	OgdcBool PtInRect(const OgdcPoint2D& pnt) const;

	//! \brief 		判断当前矩形是否包含矩形rc
	//! \param 		rc 矩形
	//! \return		若当前矩形包含矩形rc，则返回ture，否则，返回false。
	//! \image html Contains.png
	OgdcBool Contains(const OgdcRectF& rc) const;

	//! \brief 		判断当前矩形是否被矩形rc包含
	//! \param 		rc	矩形
	//! \return		若当前矩形被矩形rc包含，则返回ture，否则，返回false。
	//! \image html within.png
	OgdcBool Withins(const OgdcRectF& rc) const;

	//! \brief 		判断当前矩形是否与矩形rc相交
	//! \param 		rc	矩形
	//! \returen	相交返回true，否则，返回false。
	//! \image html Intersect2.png
	OgdcBool IsIntersect(const OgdcRectF& rc) const;


	//! \brief 		规范矩形，确保left<=right,top<=bottom	
	void Normalize();
	
	//! \brief 		对矩形进行偏移
	//! \param 		nX	横坐标偏移量
	//! \param 		nY	纵坐标偏移量	
	void Offset(OgdcDouble nX,OgdcDouble nY);
	
	//! \brief 		对矩形进行偏移
	//! \param 		sz	横纵坐标偏移量	
	void Offset(const OgdcSize2D& sz);
	
	//! \brief 		对矩形进行偏移
	//! \param 		pnt	横纵坐标偏移量	
	void Offset(const OgdcPoint2D& p);

	//! \brief 		对矩形进行放大
	//! \param 		nMargin	横纵放大比例 	
	void Inflate(OgdcDouble nMargin);

	//! \brief 		对矩形进行放大
	//! \param 		nHormargin	横坐标放大比例
	//! \param 		nVermargin	总坐标放大比例	
	void Inflate(OgdcDouble nHorMargin,OgdcDouble nVerMargin);

	//! \brief 		对矩形进行放大
	//! \param 		nLeftMargin	左上角点沿x轴向左的偏移量
	//! \param 		nTopMargin	左上角点沿y轴向上的偏移量
	//! \param 		nRightMargin    右下角点沿x轴向右的偏移量
	//! \param 		nBottomMargin	右下角点沿y轴向下的偏移量
	void Inflate(OgdcDouble nLeftMargin,OgdcDouble nTopMargin,OgdcDouble nRightMargin,OgdcDouble nBottomMargin);
	
	//! \brief 		对矩形进行缩小
	//! \param 		nMargin	横纵缩小比例	
	void Deflate(OgdcDouble nMargin);

	//! \brief 		对矩形进行缩小
	//! \param 		nHormargin	横坐标缩小比例
	//! \param 		nVermargin	总坐标缩小比例	
	void Deflate(OgdcDouble nHorMargin,OgdcDouble nVerMargin);

	//! \brief 		对矩形进行缩小
	//! \param 		nLeftMargin	左上角点沿x轴向右的偏移量
	//! \param 		nTopMargin	左上角点沿y轴向下的偏移量
	//! \param 		nRightMargin 右下角点沿x轴向左的偏移量	
	//! \param 		nBottomMargin 右下角点沿y轴向上的偏移量	
	void Deflate(OgdcDouble nLeftMargin,OgdcDouble nTopMargin,OgdcDouble nRightMargin,OgdcDouble nBottomMargin);

	//! \brief 		矩形合并
	//! \param 		pnt	合并的点
	//! \image html union.png
	void Union(const OgdcPoint2D& pnt);

	//! \brief 		矩形合并
	//! \param 		rc 用来与原矩形合并的矩形rc	
	//! \image html union2.png
	void Union(const OgdcRectF& rc);	

	//! \brief 		矩形求交
	//! \param 		rc 用来与原矩形求交的矩形rc
	//! \image html Intersect2.png
	void Intersection(const OgdcRectF& rc);

	//! \brief 判断OgdcRect2D对象是否有效
	OgdcBool IsValid() const;

};

}

#endif // !defined(AFX_OGDCRECTF_H__INCLUDED_)

