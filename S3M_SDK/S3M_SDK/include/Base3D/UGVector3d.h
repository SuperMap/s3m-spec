//////////////////////////////////////////////////////////////////////////
//                    _   _   ____   ____                    
//   Project         | | | | / ___) / ___)                   
//                   | | | || | __ | |                       
//                   | |_| || |_\ || \___                    
//                    \____/ \____| \ ___)     6.0           
//                                                           
//!  \file headerfile.h
//!  \brief 三维3阶向量类的定义。
//!  \details 完成对三维向量类的封装。
//!  \author 。
//!  \attention 
//!   Copyright (c) 1996-2010 SuperMap Software Co., Ltd. <br> 
//!   All Rights Reserved.
//!  \version 6.0
//////////////////////////////////////////////////////////////////////////


#if !defined(AFX_UGVECTOR3D_H__07E8BCDA_D3A9_431B_A917_E7A1D7351969__INCLUDED_)
#define AFX_UGVECTOR3D_H__07E8BCDA_D3A9_431B_A917_E7A1D7351969__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Base3D/UGPrerequisites3D.h"

namespace UGC 
{

//! \brief 三维向量类。
class BASE3D_API UGVector3d  
{
public:
	//! \brief 构造函数
	UGVector3d();

	//! \brief 构造函数
	UGVector3d(UGdouble fX, UGdouble fY, UGdouble fZ);

	//! \brief 析构函数
	virtual ~UGVector3d();	

	//! \brief 进行规格化
	UGVector3d& Normalize();

	//! \brief 赋值函数
	UGVector3d& operator = (const UGVector3d& vec);

	//! \brief 重载的等值判读运算符，判断两个向量是否相等
	UGbool operator == (const UGVector3d& vec) const;
	
	//! \brief 重载的不等值判读运算符，判断两个向量是否不相等
	UGbool operator != (const UGVector3d& vec) const;

	//! \brief 重载的大于判读运算符，判断该向量否大于一个向量
	//! \remarks 如果x，y，z都大于一个向量的x，y，z分量，则返回TRUE。
	UGbool operator > (const UGVector3d& vec) const;

	//! \brief 重载的小于判读运算符，判断该向量否小于一个向量
	//! \remarks 如果x，y，z都小于一个向量的x，y，z分量，则返回TRUE。
	UGbool operator < (const UGVector3d& vec)const;	
	
	//! \brief 重载的加法运算符，求两个向量之和
	UGVector3d operator + (const UGVector3d& vec) const;

	//! \brief 重载的减法运算符，求两个向量之差
	UGVector3d operator - (const UGVector3d& vec) const;
	
	//! \brief 重载的乘法运算符，求乘以一个数的结果
	UGVector3d operator * (const UGdouble fScale) const;

	//! \brief 重载的乘法运算符，一个数乘以向量的结果
	friend BASE3D_API UGVector3d operator * ( const UGdouble fScalar, const UGVector3d& rkVector );

	//! \brief 重载的除法运算符，求除以一个数的结果
	UGVector3d operator / (const UGdouble fScale) const;

	//! \brief 重载的乘法运算符，求乘以一个矩阵的结果
	UGVector3d operator * (const UGMatrix3d& m) const;	

	//! \brief 重载的取值运算符，取索引i处的分量
	UGdouble operator [] (const unsigned i) const;

	//! \brief 重载的取值运算符，取索引i处的分量的引用
	UGdouble& operator [] (const unsigned i);
	
	//! \brief 重载的取反运算符，对向量求反
	//! \remarks 对向量求反就是对每个分量求反
	UGVector3d operator - () const;

	//! \brief 重载的+=运算符，求该向量+=一个向量的结果
	UGVector3d& operator += (const UGVector3d& vec);
	
	//! \brief 重载的-=运算符，求该向量-=一个向量的结果
	UGVector3d& operator -= (const UGVector3d& vec);
	
	//! \brief 重载的*=运算符，求该向量*=一个数的结果
	UGVector3d& operator *= (const UGdouble fScale);
	
	//! \brief 重载的/=运算符，求该向量/=一个数的结果
	UGVector3d& operator /= (const UGdouble fScale);

	//! \brief 求两个向量的叉积
	UGVector3d CrossProduct(const UGVector3d &vec) const;

	//! \brief 求两个向量的点积
	UGdouble DotProduct(const UGVector3d& vec) const;
	
	//! \brief 取两个向量每个分类的最小值形成一个新的向量
	void MakeFloor( const UGVector3d& cmp );

	//! \brief 取两个向量每个分类的最大值形成一个新的向量
	void MakeCeil( const UGVector3d& cmp );
	
	//! \brief 求向量的长度
	UGdouble Length() const;

	//! \brief 求向量的长度的平方
	UGdouble Length2() const;

	//! \brief 判断向量的长度是否为0
	UGbool IsZeroLength(void) const;

	//! \brief 该乘法支持D3D矩阵运算
	//! \brief 本系统可以直接使用
	UGVector3d MultiplyMatrix(const UGMatrix4d& m) const;

	//! \brief 求向量乘以一个4*4的矩阵
	//! \param vecPos 向量[in][out]。
	//! \param m 4×4矩阵[in]。
	static void MultiplyMatrix(UGVector3d& vecPos, UGMatrix4d& m);	

	//! \brief 求向量乘以一个4*4的矩阵。
	//! \param m 4×4矩阵[in]。
	//! \param &w 用来补齐向量来做乘法[in][out]。
	void MultiplyMatrix(const UGMatrix4d& m, UGdouble &w);	
	
	/// \brief 将window坐标换成空间坐标
	/// \param viewport: x - x, y - y, z - width, w - height 视口
	/// \param projection 投影矩阵
	/// \param view 视图矩阵
	/// \param world 世界矩阵
	void UnProject(const UGVector4d &viewport, 
						   const UGMatrix4d &projection, 
						   const UGMatrix4d &view, 
						   const UGMatrix4d &world);
	//! \brief 自身缩放。每个分量乘以value
	void Scale(const UGdouble& value);

	//! \brief 判断向量是否无效
	UGbool IsRealNaN();

	UGQuaternion4d GetRotationTo(const UGVector3d& dest, 
		const UGVector3d& fallbackAxis = UGVector3d::ZERO) const;

	//! \brief 该乘法支持D3D矩阵运算
	//! \brief 本系统可以直接使用
	UGVector3d MultiplyMatrix(const UGMatrix3d& m) const;

	//! \brief 计算两个向量之间的夹角
	//! \param vecDest 目标向量，此向量不必是单位向量
	//! \return 两个向量的夹角，弧度制
	UGdouble AngleBetween(const UGVector3d& vecDest) const;

	//! \brief 计算与两个向量均垂直的向量,方向与a×b方向相同
	UGVector3d Perpendicular(const UGVector3d& vecDest) const;

	//! \brief 判断两个Vector是否相等，误差为EP
	UGbool IsEQ(const UGVector3d &vec);

	//! \brief 零向量
	static const UGVector3d ZERO;
	//! \brief x单位向量(1,0,0)
	static const UGVector3d UNIT_X;
	//! \brief y单位向量(0,1,0)
    static const UGVector3d UNIT_Y;
	//! \brief z单位向量(0,0,1)
    static const UGVector3d UNIT_Z;
	//! \brief -x单位向量(-1,0,0)
    static const UGVector3d NEGATIVE_UNIT_X;
	//! \brief -y单位向量(0,-1,0)
    static const UGVector3d NEGATIVE_UNIT_Y;
	//! \brief -z单位向量(0,0,-1)
    static const UGVector3d NEGATIVE_UNIT_Z;
	//! \brief 全单位向量(1,1,1)
    static const UGVector3d UNIT_SCALE;
public:
	//! \brief 向量的三个分量
	UGdouble x, y, z;
};
}
#endif // !defined(AFX_UGVECTOR3D_H__07E8BCDA_D3A9_431B_A917_E7A1D7351969__INCLUDED_)

