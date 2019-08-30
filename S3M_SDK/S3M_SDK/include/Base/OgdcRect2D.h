//////////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)               
//                                                           
//!  \file OgdcRect2D.h
//!  \brief 2D矩形类 
//!  \attention 
//!   Copyright (c) 2007-2012 SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OGDCRECT2D_H__A3169E94_5510_4729_B37C_17CD30FC987B__INCLUDED_)
#define AFX_OGDCRECT2D_H__A3169E94_5510_4729_B37C_17CD30FC987B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OgdcPoint2D.h"

namespace OGDC {
//!  \brief 矩形对象类，存储了矩形的左上角点、右下角点的(x,y)坐标，坐标值为OgdcDouble类型。
class BASE_API OgdcRect2D  
{
public:
	//! \brief 矩形左边界
	OgdcDouble left;
	//! \brief 矩形上边界
	OgdcDouble top;
	//! \brief 矩形右边界。left<right
	OgdcDouble right;
	//! \brief 矩形下边界。bottom < top
	OgdcDouble bottom;

public:

	//! \brief 构造函数一个二维矩形对象
	OgdcRect2D();

	// fansc 加一个拷贝构造，发现在GCC编译器下，使用OpenMP时拷贝UGRect2D时会导致编译器内部出错
	//! \brief 拷贝构造函数
	OgdcRect2D(const OgdcRect2D& rc);

	//! \brief 		缺省析造函数
	~OgdcRect2D();
	
	//! \brief 		构造函数一个二维矩形对象
	//! \param 		nLeft	矩形左上角点的x轴坐标
	//! \param 		nTop	矩形左上角点的y轴坐标
	//! \param 		nRight	矩形右下角点的x轴坐标
	//! \param 		nBottom	矩形右下角点的y轴坐标
	//! \remarks 	请确保nLeft<nRight,nTop<nBottom
	OgdcRect2D(OgdcDouble dLeft,OgdcDouble dTop,OgdcDouble dRight,OgdcDouble dBottom);

	//! \brief 		构造函数一个二维矩形对象
	//! \param 		pnt	左下角坐标
	//! \param 		sz	矩形的大小
	OgdcRect2D(const OgdcPoint2D& pnt,const OgdcSize2D& sz);

	//! \brief 		构造函数一个二维矩形对象
	//! \param 		pntTL	左上角坐标
	//! \param 		pntBR   右下角坐标
	OgdcRect2D(const OgdcPoint2D& pntTL,const OgdcPoint2D& pntBR);	

	//! \brief 		重载 =
    OgdcRect2D& operator=(const OgdcRect2D& rc);

	//! \brief 		重载 ==
	friend BASE_API OgdcBool operator==(const OgdcRect2D& rcStart,const OgdcRect2D& rcEnd);

	//! \brief 		重载 !=
	friend BASE_API OgdcBool operator!=(const OgdcRect2D& rcStart,const OgdcRect2D& rcEnd);
	
	
	//! \brief 		设置矩形参数
	//! \param 		dLeft	矩形左上角点的x轴坐标
	//! \param 		dTop	矩形左上角点的y轴坐标
	//! \param 		dRight	矩形右下角点的x轴坐标
	//! \param 		dBottom	矩形右下角点的y轴坐标
	void SetRect(OgdcDouble dLeft,OgdcDouble dTop,OgdcDouble dRight,OgdcDouble dBottom);
	
	//! \brief 		设置矩形参数
	//! \param 		pntTL	左上角坐标
	//! \param 		pntBR	右下角坐标
	void SetRect(const OgdcPoint2D& pntTL,const OgdcPoint2D& pntBR);

	//! \brief 		将矩形置为空
	//! \remarks 	左上角、右下角点坐标都设为0
	void SetEmpty();

	//! \brief 		判断矩形是否为空或是否合法
	//! \return 	矩形为空返回true，矩形不为空返回false
	//! \remarks 	如果符合left=right,top=bottom规则，返回true，认为矩形为空。
	OgdcBool IsEmpty() const;

	//! \brief 		矩形是否为未初始化
	//! \return 	如果符合left==right==top==bottom==0规则，则返回true，否则，返回false
	OgdcBool IsNull() const;

	//! \brief 		矩形的宽度
	//! \return 	返回right-left的值(整型)
	OgdcDouble Width() const;

	//! \brief 		矩形的高度
	//! \return 	返回top-bottom的值(整型)
	OgdcDouble  Height() const;
	
	//! \brief 		获取矩形的左上角点
	OgdcPoint2D& TopLeft();

	//! \brief 		获取矩形的左上角点
	//! \remarks 	如果矩形对象为const类型，则将调用本接口
	const OgdcPoint2D& TopLeft() const;	

	
	//! \brief 		获取矩形右上角坐标点
	OgdcPoint2D TopRight() const;
	
	//! \brief 		获取矩形右下角坐标点
	OgdcPoint2D& BottomRight();	
	
	//! \brief 		获取矩形右下角坐标点
	//! \remarks 	如果矩形对象为const类型，则将调用本接口
	const OgdcPoint2D& BottomRight() const;

	//! \brief 		获取矩形左下角坐标点
	OgdcPoint2D BottomLeft() const;
	
	
	//! \brief 		获取矩形的中心点
	//! \remarks 	矩形对角线的交点
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
	OgdcBool Contains(const OgdcRect2D& rc) const;

	//! \brief 		判断当前矩形是否被矩形rc包含
	//! \param 		rc	矩形
	//! \return		若当前矩形被矩形rc包含，则返回ture，否则，返回false。
	//! \image html within.png
	OgdcBool Withins(const OgdcRect2D& rc) const;

	
	//! \brief 		判断当前矩形是否与矩形rc相交
	//! \param 		rc	矩形
	//! \returen	相交返回true，否则，返回false。
	OgdcBool IsIntersect(const OgdcRect2D& rc) const;
	
	//! \brief 		规范矩形，确保left<=right,bottom<=top	
	void Normalize();
	
	//! \brief 		对矩形进行偏移
	//! \param 		dX	横坐标偏移量
	//! \param 		dY	纵坐标偏移量
	void Offset(OgdcDouble dX,OgdcDouble dY);

	//! \brief 		对矩形进行偏移
	//! \param 		sz	横纵坐标偏移量
	void Offset(const OgdcSize2D& sz);

	//! \brief 		对矩形进行偏移
	//! \param 		pnt	横纵坐标偏移量
	void Offset(const OgdcPoint2D& pnt);
	
	//! \brief 		对矩形进行放大
	//! \param 		nMargin	横纵放大比例 	
	void Inflate(OgdcDouble dMargin);

	//! \brief 		对矩形进行放大
	//! \param 		dHormargin	横坐标放大比例
	//! \param 		dVermargin	总坐标放大比例	
	void Inflate(OgdcDouble dHorMargin,OgdcDouble dVerMargin);
	
	//! \brief 		对矩形进行放大
	//! \param 		nLeftMargin	左上角点沿x轴向左的偏移量
	//! \param 		nTopMargin	左上角点沿y轴向上的偏移量
	//! \param 		nRightMargin    右下角点沿x轴向右的偏移量
	//! \param 		nBottomMargin	右下角点沿y轴向下的偏移量
	void Inflate(OgdcDouble nLeftMargin,OgdcDouble nTopMargin,OgdcDouble nRightMargin,OgdcDouble nBottomMargin);
	
	//! \brief 		对矩形进行缩小
	//! \param 		dMargin	横纵缩小比例	
	void Deflate(OgdcDouble dMargin);
	
	//! \brief 		对矩形进行缩小
	//! \param 		dHorMargin	横坐标缩小比例
	//! \param 		dVerMargin	总坐标缩小比例
	void Deflate(OgdcDouble dHorMargin,OgdcDouble dVerMargin);

	
	//! \brief 		对矩形进行缩小
	//! \param 		dLeftMargin	左上角点沿x轴向右的偏移量
	//! \param 		dTopMargin	左上角点沿y轴向下的偏移量
	//! \param 		dRightMargin 右下角点沿x轴向左的偏移量	
	//! \param 		dBottomMargin 右下角点沿y轴向上的偏移量	
	void Deflate(OgdcDouble dLeftMargin,OgdcDouble dTopMargin,OgdcDouble dRightMargin,OgdcDouble dBottomMargin);
	
	//! \brief 		矩形合并
	//! \param 		pnt	合并的点
	//! \image html union.png
	void Union(const OgdcPoint2D& pnt);
	
	//! \brief 		矩形合并
	//! \param 		rc 用来与原矩形合并的矩形rc
	//! \image html union2.png
	void Union(const OgdcRect2D& rc);	

	//! \brief 矩形合并
	//! \remark 检查参数和自身合法性
	//! \return 是否成功
	OgdcBool UnionRect(const OgdcRect2D& rc);
	
	//! \brief 		矩形求交
	//! \param 		rc 用来与原矩形求交的矩形rc
	//! \param		当前矩形为与rc求交的结果矩形
	//! \image html Intersect2.png
	OgdcBool IntersectRect(const OgdcRect2D& rc);
	
	//! \brief 		矩形求交
	//! \param 		rc1 用来求交的矩形	
	//! \param 		rc2	用来求交的矩形
	//! \remarks	当前矩形为rc1，与rc2求交后的结构矩形。
	//! \image html Interset.png
	OgdcBool IntersectRect(const OgdcRect2D& rc1,const OgdcRect2D& rc2);
	
	//! \brief 		当前矩形与旋转之后的矩形求并
	//! \param 		pntOrg 中心点
	//! \param 		dbAngle 旋转角度
	//! \image html union3.png
	void UnionRotate(const OgdcPoint2D &pntOrg, OgdcDouble dbAngle);

	void Rotate(const OgdcPoint2D &pntOrg, OgdcDouble dbAngle);
	
	//! \brief 判断OgdcRect2D对象是否有效
	OgdcBool IsValid() const;
	
	void InflateRect(OgdcDouble x, OgdcDouble y);
	
};

}

#endif // !defined(AFX_OGDCRECT2D_H__A3169E94_5510_4729_B37C_17CD30FC987B__INCLUDED_)

