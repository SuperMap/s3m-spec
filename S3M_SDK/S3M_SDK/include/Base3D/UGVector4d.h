//////////////////////////////////////////////////////////////////////////
//                    _   _   ____   ____                    
//   Project         | | | | / ___) / ___)                   
//                   | | | || | __ | |                       
//                   | |_| || |_\ || \___                    
//                    \____/ \____| \ ___)     6.0           
//                                                           
//!  \file headerfile.h
//!  \brief 三维4阶向量类的定义。
//!  \details 完成对三维4阶向量类定义。
//!  \author 。
//!  \attention 
//!   Copyright (c) 1996-2010 SuperMap Software Co., Ltd. <br> 
//!   All Rights Reserved.
//!  \version 6.0
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_UGVECTOR4R_H__92CEC4D7_6A04_475B_A0BB_2C457812FD95__INCLUDED_)
#define AFX_UGVECTOR4R_H__92CEC4D7_6A04_475B_A0BB_2C457812FD95__INCLUDED_

#include "Base3D/UGPrerequisites3D.h"
#include "Base3D/UGVector3d.h"

namespace UGC 
{
//! \brief 三维4阶向量类。
class BASE3D_API UGVector4d  
{
public:

#pragma warning(disable: 4201)   //使用了非标准扩展 : 无名称的结构/联合
	//! \brief 公用体，实现[]操作
    union 
	{
        struct 
		{
			//! \brief 三维4阶向量类成员。
            UGdouble x, y, z, w;
        };
        UGdouble val[4];
    };
#pragma warning(default: 4201)

public:
	//! \brief 构造函数
    UGVector4d();
	//! \brief 构造函数，由四个值构建
    UGVector4d( const UGdouble fX, const UGdouble fY, const UGdouble fZ, const UGdouble fW );
    //! \brief 构造函数,由数组构建
	UGVector4d( const UGdouble afCoordinate[4] );
    //! \brief 构造函数,由数组构建
	UGVector4d( const int afCoordinate[4] );
    //! \brief 构造函数,由数组构建
	UGVector4d( UGdouble* const r );
	//! \brief 构造函数,每个分量赋同样的值scaler
    UGVector4d( const UGdouble scaler );
	//! \brief 构造函数，由3×3的矩阵构造4×4的矩阵
    UGVector4d(const UGVector3d& rhs);
	//! \brief 拷贝构造函数
    UGVector4d( const UGVector4d& rkVector );
	//! \brief 重载的取值运算符，取索引i处的分量
	UGdouble operator [] ( const size_t i ) const;
	//! \brief 重载的取值运算符，取索引i处的分量的引用
	UGdouble& operator [] ( const size_t i );
	//! \brief 重载的赋值运算符，取索引i处的分量的引用
    UGVector4d& operator = ( const UGVector4d& rkVector );
	//! \brief 重载的赋值运算符，每个分量赋予同样的值fScalar
	UGVector4d& operator = ( const UGdouble fScalar);
	//! \brief 重载的等值判读运算符，判断两个向量是否相等
    bool operator == ( const UGVector4d& rkVector ) const;
	//! \brief 重载的不等值判读运算符，判断两个向量是否不相等
    bool operator != ( const UGVector4d& rkVector ) const;
	//! \brief 赋值函数，由3×3的矩阵构造4×4的矩阵
    UGVector4d& operator = (const UGVector3d& rhs);
	//! \brief 重载的加法运算符，求两个向量之和
    UGVector4d operator + ( const UGVector4d& rkVector ) const;
	//! \brief 重载的减法运算符，求两个向量之差
    UGVector4d operator - ( const UGVector4d& rkVector ) const;
	//! \brief 重载的乘法运算符，求乘以一个数的结果
    UGVector4d operator * ( const UGdouble fScalar ) const;
	//! \brief 重载的乘法运算符，求乘以一个向量的结果
    UGVector4d operator * ( const UGVector4d& rhs) const;
	//! \brief 重载的乘法运算符，求乘以一个矩阵的结果
	UGVector4d operator * (const UGMatrix4d& m);
	//! \brief 重载的除法运算符，求除以一个数的结果
    UGVector4d operator / ( const UGdouble fScalar ) const;
	//! \brief 重载的除法运算符，求除以一个向量的结果
    UGVector4d operator / ( const UGVector4d& rhs) const;
	//! \brief 重载的正算符，返回改向量
    const UGVector4d& operator + () const;
	//! \brief 重载的取反运算符，对向量求反
	//! \remarks 对向量求反就是对每个分量求反
    UGVector4d operator - () const;
	//! \brief 重载的乘法运算符，求向量乘以一个数的结果
    friend UGVector4d operator * ( const UGdouble fScalar, const UGVector4d& rkVector );
    //! \brief 重载的除法运算符，求向量除以一个数的结果
	friend UGVector4d operator / ( const UGdouble fScalar, const UGVector4d& rkVector );
	//! \brief 重载的加法运算符，求向量和一个数之和
	//! \remarks 对向量每个分量加上一个值
    friend UGVector4d operator + (const UGVector4d& lhs, const UGdouble rhs);
	//! \brief 重载的加法运算符，求向量和一个数之和
	//! \remarks 对向量每个分量加上一个值
    friend UGVector4d operator + (const UGdouble lhs, const UGVector4d& rhs);
	//! \brief 重载的减法运算符，求向量和一个数之差
	//! \remarks 对向量每个分量减去一个值
    friend UGVector4d operator - (const UGVector4d& lhs, UGdouble rhs);  
	//! \brief 重载的减法运算符，求向量和一个数之差
	//! \remarks 对向量每个分量减去一个值
	friend UGVector4d operator - (const UGdouble lhs, const UGVector4d& rhs);

	//! \brief 重载的+=运算符，求该向量+=一个向量的结果
    UGVector4d& operator += ( const UGVector4d& rkVector );

	//! \brief 重载的-=运算符，求该向量-=一个向量的结果
    UGVector4d& operator -= ( const UGVector4d& rkVector );

	//! \brief 重载的*=运算符，求该向量*=一个数的结果
    UGVector4d& operator *= ( const UGdouble fScalar );

	//! \brief 重载的+=运算符，求该向量+=一个数的结果
    UGVector4d& operator += ( const UGdouble fScalar );
	
	//! \brief 重载的-=运算符，求该向量-=一个数的结果
    UGVector4d& operator -= ( const UGdouble fScalar );

	//! \brief 重载的*=运算符，求该向量*=一个向量的结果
    UGVector4d& operator *= ( const UGVector4d& rkVector );

	//! \brief 重载的/=运算符，求该向量/=一个数的结果
    UGVector4d& operator /= ( const UGdouble fScalar );

	//! \brief 重载的/=运算符，求该向量/=一个向量的结果
    UGVector4d& operator /= ( const UGVector4d& rkVector );

	//! \brief 求两个向量的点积
    UGdouble dotProduct(const UGVector4d& vec) const;

	//! \brief 零向量 ( 0, 0, 0, 0 )
    static const UGVector4d ZERO;

};
}
#endif // !defined(AFX_UGVECTOR4D_H__92CEC4D7_6A04_475B_A0BB_2C457812FD95__INCLUDED_)

