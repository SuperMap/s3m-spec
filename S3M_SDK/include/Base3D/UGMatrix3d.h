//////////////////////////////////////////////////////////////////////////
//                    _   _   ____   ____                    
//   Project         | | | | / ___) / ___)                   
//                   | | | || | __ | |                       
//                   | |_| || |_\ || \___                    
//                    \____/ \____| \ ___)     6.0           
//                                                           
//!  \file headerfile.h
//!  \brief 三阶矩阵类。
//!  \details 对三阶矩阵的封装。
//!  \author 。
//!  \attention 
//!   Copyright (c) 1996-2010 SuperMap Software Co., Ltd. <br> 
//!   All Rights Reserved.
//!  \version 6.0
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_UGMATRIX3D_H__87211562_9080_4FF0_99EB_741D322A0BB3__INCLUDED_)
#define AFX_UGMATRIX3D_H__87211562_9080_4FF0_99EB_741D322A0BB3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Base3D/UGPrerequisites3D.h"

namespace UGC
{
//! \brief 三阶矩阵类。
class BASE3D_API UGMatrix3d  
{
public:
	//! \brief 构造函数
	UGMatrix3d ();
	//! \brief 由3×3的二维矩阵构造三阶矩阵
	UGMatrix3d (const UGdouble arr[3][3]);
	//! \brief 拷贝构造函数
	UGMatrix3d (const UGMatrix3d& rkMatrix);
	//! \brief 由值来构造三阶矩阵
	UGMatrix3d (UGdouble fEntry00, UGdouble fEntry01, UGdouble fEntry02,
		UGdouble fEntry10, UGdouble fEntry11, UGdouble fEntry12,
		UGdouble fEntry20, UGdouble fEntry21, UGdouble fEntry22);
	//! \brief 重载的数组取值操作符。
	//! \param iRow 数组索引[in]。
	UGdouble* operator[] (UGint iRow) const;	
	//! \brief 重载的取地址操作符。
	//! \return 矩阵数组的起始地形。
	operator UGdouble* ();	
	//! \brief 获得矩阵某列三个数构成的向量。
	//! \param iCol 列[in]。
	//! \return 向量。
	UGVector3d GetColumn (UGint iCol) const;	
	//! \brief 设置矩阵某列向量。
	//! \param iCol 列[in]。
	//! \param vec 向量[in]。
	void SetColumn(UGint iCol, const UGVector3d& vec);	
	//! \brief 由绕三个轴旋转角度构成旋转矩阵。
	//! \param xAxis 绕x轴旋转的角度[in]。
	//! \param yAxis 绕y轴旋转的角度[in]。
	//! \param zAxis 绕z轴旋转的角度[in]。
	void FromAxes(const UGVector3d& xAxis, const UGVector3d& yAxis, const UGVector3d& zAxis);	
	//! \brief 赋值函数。
	UGMatrix3d& operator= (const UGMatrix3d& rkMatrix);
	//! \brief 重载的等值判断运算符。
	UGbool operator== (const UGMatrix3d& rkMatrix) const;
	//! \brief 重载的不等判断运算符。
	UGbool operator!= (const UGMatrix3d& rkMatrix) const;
	//! \brief 重载的加法运算符，求两个矩阵相加结果。
	UGMatrix3d operator+ (const UGMatrix3d& rkMatrix) const;
	//! \brief 重载的减法运算符，求两个矩阵相减结果。
	UGMatrix3d operator- (const UGMatrix3d& rkMatrix) const;
	//! \brief 重载的乘法运算符，求两个矩阵相乘结果。
	UGMatrix3d operator* (const UGMatrix3d& rkMatrix) const;
	//! \brief 重载的相反运算符，求矩阵相反数。
	UGMatrix3d operator- () const;
	//! \brief 重载的乘法运算符，求矩阵和向量相乘结果。
	UGVector3d operator* (const UGVector3d& rkVector) const;

	//! \brief 重载的乘法运算符，求矩阵和数值相乘结果。
	UGMatrix3d operator* (UGdouble fScalar) const;
	//! \brief 重载的乘法运算符，求矩阵和数值相乘结果。
	friend UGMatrix3d operator* (UGdouble fScalar, const UGMatrix3d& rkMatrix);
	//! \brief 求矩阵的转置矩阵。
	//! \return 矩阵的转置矩阵。
	UGMatrix3d Transpose () const;
	//! \brief 求矩阵的逆矩阵。
	//! \param rkInverse 矩阵的逆矩阵[out]。
	//! \param fTolerance 容限[in]。
	//! \return 是否有逆矩阵存在。
	UGbool Inverse (UGMatrix3d& rkInverse, UGdouble fTolerance = 1e-06) const;
	//! \brief 求矩阵的逆矩阵。
	//! \param fTolerance 容限[in]。
	//! \return 矩阵的逆矩阵。
	//! \remarks 失败返回0矩阵ZERO。
	UGMatrix3d Inverse (UGdouble fTolerance = 1e-06) const;
	//! \brief 求矩阵的行列式的值。
	UGdouble Determinant () const;	
	/* 先注释掉，目前没用
	//! \brief singular value decomposition
	void SingularValueDecomposition (UGMatrix3d& rkL, UGVector3d& rkS,
		UGMatrix3d& rkR) const;
	void SingularValueComposition (const UGMatrix3d& rkL,
		const UGVector3d& rkS, const UGMatrix3d& rkR);
	
	//! \brief Gram-Schmidt orthonormalization (applied to columns of rotation matrix)
	void Orthonormalize ();
	
	//! \brief orthogonal Q, diagonal D, upper triangular U stored as (u01,u02,u12)
	void QDUDecomposition (UGMatrix3d& rkQ, UGVector3d& rkD,
		UGVector3d& rkU) const;
	*/
	
	//UGdouble SpectralNorm () const;
	//! \brief 旋转矩阵转换为绕某个轴旋转角度。
	//! \param rkAxis 旋转角度[out]。
	//! \param rfRadians 旋转轴[out]。
	//! \attention 矩阵必须是正交矩阵。
	void ToAxisAngle (UGVector3d& rkAxis, UGdouble& rfRadians) const;
	//! \brief 绕某个轴旋转角度转换为旋转矩阵。
	//! \param rkAxis 旋转角度[int]。
	//! \param rfRadians 旋转轴[int]。
	void FromAxisAngle (const UGVector3d& rkAxis, UGdouble fRadians);
	
	// The matrix must be orthonormal.  The decomposition is yaw*pitch*roll
	// where yaw is rotation about the Up vector, pitch is rotation about the
	// Right axis, and roll is rotation about the Direction axis.

	//! \brief 矩阵转换为绕XYZ的顺序旋转的欧拉角。
	//! \param rfYAngle 绕x轴旋转角度[out]。
	//! \param rfPAngle 绕Y轴旋转角度[out]。
	//! \param rfRAngle 绕z轴旋转角度[out]。
	UGbool ToEulerAnglesXYZ (UGdouble& rfYAngle, UGdouble& rfPAngle,
		UGdouble& rfRAngle) const;	
	//! \brief 矩阵转换为绕XZY的顺序旋转的欧拉角。
	//! \param rfYAngle 绕x轴旋转角度[out]。
	//! \param rfPAngle 绕z轴旋转角度[out]。
	//! \param rfRAngle 绕y轴旋转角度[out]。
	UGbool ToEulerAnglesXZY (UGdouble& rfYAngle, UGdouble& rfPAngle,
		UGdouble& rfRAngle) const;
	//! \brief 矩阵转换为绕YXZ的顺序旋转的欧拉角。
	//! \param rfYAngle 绕y轴旋转角度[out]。
	//! \param rfPAngle 绕x轴旋转角度[out]。
	//! \param rfRAngle 绕z轴旋转角度[out]。
	UGbool ToEulerAnglesYXZ (UGdouble& rfYAngle, UGdouble& rfPAngle,
		UGdouble& rfRAngle) const;
	//! \brief 矩阵转换为绕YZX的顺序旋转的欧拉角。
	//! \param rfYAngle 绕y轴旋转角度[out]。
	//! \param rfPAngle 绕z轴旋转角度[out]。
	//! \param rfRAngle 绕x轴旋转角度[out]。
	UGbool ToEulerAnglesYZX (UGdouble& rfYAngle, UGdouble& rfPAngle,
		UGdouble& rfRAngle) const;
	//! \brief 矩阵转换为绕XYZ的顺序旋转的欧拉角。
	//! \param rfYAngle 绕x轴旋转角度[out]。
	//! \param rfPAngle 绕Y轴旋转角度[out]。
	//! \param rfRAngle 绕z轴旋转角度[out]。
	UGbool ToEulerAnglesZXY (UGdouble& rfYAngle, UGdouble& rfPAngle,
		UGdouble& rfRAngle) const;
	//! \brief 矩阵转换为绕XYZ的顺序旋转的欧拉角。
	//! \param rfYAngle 绕x轴旋转角度[out]。
	//! \param rfPAngle 绕Y轴旋转角度[out]。
	//! \param rfRAngle 绕z轴旋转角度[out]。
	UGbool ToEulerAnglesZYX (UGdouble& rfYAngle, UGdouble& rfPAngle,
		UGdouble& rfRAngle) const;
	//! \brief 绕XYZ的顺序旋转的欧拉角转换为矩阵。
	//! \param rfYAngle 绕x轴旋转角度[int]。
	//! \param rfPAngle 绕Y轴旋转角度[int]。
	//! \param rfRAngle 绕z轴旋转角度[int]。
	void FromEulerAnglesXYZ (UGdouble fYAngle, UGdouble fPAngle, UGdouble fRAngle);
	//! \brief 绕XZY的顺序旋转的欧拉角转换为矩阵。
	//! \param rfYAngle 绕x轴旋转角度[int]。
	//! \param rfPAngle 绕z轴旋转角度[int]。
	//! \param rfRAngle 绕y轴旋转角度[int]。
	void FromEulerAnglesXZY (UGdouble fYAngle, UGdouble fPAngle, UGdouble fRAngle);
	//! \brief 绕YXZ的顺序旋转的欧拉角转换为矩阵。
	//! \param rfYAngle 绕y轴旋转角度[int]。
	//! \param rfPAngle 绕x轴旋转角度[int]。
	//! \param rfRAngle 绕z轴旋转角度[int]。
	void FromEulerAnglesYXZ (UGdouble fYAngle, UGdouble fPAngle, UGdouble fRAngle);
	//! \brief 绕YZX的顺序旋转的欧拉角转换为矩阵。
	//! \param rfYAngle 绕y轴旋转角度[int]。
	//! \param rfPAngle 绕z轴旋转角度[int]。
	//! \param rfRAngle 绕x轴旋转角度[int]。
	void FromEulerAnglesYZX (UGdouble fYAngle, UGdouble fPAngle, UGdouble fRAngle);
	//! \brief 绕XYZ的顺序旋转的欧拉角转换为矩阵。
	//! \param rfYAngle 绕x轴旋转角度[int]。
	//! \param rfPAngle 绕Y轴旋转角度[int]。
	//! \param rfRAngle 绕z轴旋转角度[int]。
	void FromEulerAnglesZXY (UGdouble fYAngle, UGdouble fPAngle, UGdouble fRAngle);
	//! \brief 绕XYZ的顺序旋转的欧拉角转换为矩阵。
	//! \param rfYAngle 绕x轴旋转角度[int]。
	//! \param rfPAngle 绕Y轴旋转角度[int]。
	//! \param rfRAngle 绕z轴旋转角度[int]。
	void FromEulerAnglesZYX (UGdouble fYAngle, UGdouble fPAngle, UGdouble fRAngle);
	/* 先注释掉，目前没用
	// eigensolver, matrix must be symmetric
	void EigenSolveSymmetric (UGdouble afEigenvalue[3],
		UGVector3d akEigenvector[3]) const;
	
	static void TensorProduct (const UGVector3d& rkU, const UGVector3d& rkV,
		UGMatrix3d& rkProduct);
	*/

	//!\brief orthogonal Q, diagonal D, upper triangular U stored as (u01,u02,u12)
	void QDUDecomposition (UGMatrix3d& kQ, UGVector3d& kD,
		UGVector3d& kU) const;

	//! \brief 阈值
	static const UGdouble EPSILON;

	//! \brief 零矩阵
	static const UGMatrix3d ZERO;

	//! \brief 单位矩阵
	static const UGMatrix3d IDENTITY;
	
protected:
    // support for eigensolver
	/* 先注释掉，目前没用
    void Tridiagonal (UGdouble afDiag[3], UGdouble afSubDiag[3]);
    UGbool QLAlgorithm (UGdouble afDiag[3], UGdouble afSubDiag[3]);
	
     support for singular value decomposition
    static const UGdouble ms_fSvdEpsilon;
    static const UGint ms_iSvdMaxIterations;
    static void Bidiagonalize (UGMatrix3d& kA, UGMatrix3d& kL,
        UGMatrix3d& kR);
    static void GolubKahanStep (UGMatrix3d& kA, UGMatrix3d& kL,
        UGMatrix3d& kR);
    // support for spectral norm
    static UGdouble MaxCubicRoot (UGdouble afCoeff[3]);
	*/
    UGdouble m[3][3];
    // for faster access
    friend class UGMatrix4d;
};

}

#endif // !defined(AFX_UGMATRIX3D_H__87211562_9080_4FF0_99EB_741D322A0BB3__INCLUDED_)

