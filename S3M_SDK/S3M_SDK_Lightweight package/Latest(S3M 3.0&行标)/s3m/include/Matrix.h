#if !defined(S3MBMATRIX_H__2D0D4C17_EA53_489F_80E2_EFF9D6F6A23C__INCLUDED_)
#define S3MBMATRIX_H__2D0D4C17_EA53_489F_80E2_EFF9D6F6A23C__INCLUDED_

#pragma once
#include "stdafx.h"
#include "S3MBCommon.h"

namespace S3MB
{
	class Vector3d;
	class S3MB_API Matrix3d
	{
	public:
		Matrix3d();

		// 由值来构造三阶矩阵
		Matrix3d(
			double m00, double m01, double m02,
			double m10, double m11, double m12,
			double m20, double m21, double m22);

		// 3X3零矩阵
		// 所有元素为零
		static const Matrix3d ZERO;
		// 3X3 单位矩阵
		// 对角线元素为1，其他为0
		static const Matrix3d IDENTITY;

		// 重载的数组取值操作符。
		// iRow 数组索引[in]。
		double* operator[] (int iRow) const;

		// 重载的乘法运算符，求两个矩阵相乘结果。
		Matrix3d operator* (const Matrix3d& rkMatrix) const;

		// 获得矩阵某列三个数构成的向量。
		// iCol 列[in]。
		// return 向量。
		Vector3d GetColumn(int iCol) const;

		// 设置矩阵某列向量。
		// iCol 列[in]。
		// vec 向量[in]。
		void SetColumn(int iCol, const Vector3d& vec);
		
		// 由绕三个轴旋转角度构成旋转矩阵。
		// xAxis 绕x轴旋转的角度[in]。
		// yAxis 绕y轴旋转的角度[in]。
		// zAxis 绕z轴旋转的角度[in]。
		void FromAxes(const Vector3d& xAxis, const Vector3d& yAxis, const Vector3d& zAxis);

		// 计算对称矩阵的特征向量和特征值
		void EigenDecomposition(Matrix3d& matUnitary, Matrix3d& matDiagonal, double dTolerance = EP) const;

		// 求矩阵的转置矩阵。
		// return 矩阵的转置矩阵。
		Matrix3d Transpose() const;

	private:
		// Forbenius norm
		double FrobeniusNorm() const;

		// 非对角的Forbenius norm
		// 说明： 假设矩阵是对称的
		double OffDiagonalFrobeniusNorm() const;

		// 对称Schur Decomposition
		void SchurDecomposition(Matrix3d& matResult, double dTolerance = EP);
		
	protected:
		double m[3][3];
		// for faster access
		friend class Matrix4d;
	};

	// 四维矩阵
	class S3MB_API Matrix4d
	{
	public:
		Matrix4d();
		Matrix4d(
			double m00, double m01, double m02, double m03,
			double m10, double m11, double m12, double m13,
			double m20, double m21, double m22, double m23,
			double m30, double m31, double m32, double m33);
		// 4X4零矩阵
		// 所有元素为零
		static const Matrix4d ZERO;
		// 4X4 单位矩阵
		// 对角线元素为1，其他为0
		static const Matrix4d IDENTITY;

		// 重载*，矩阵与数值相乘。
		// scalar 数值[in]。
		Matrix4d operator*(double scalar);

		// 重载*，完成矩阵与矩阵相乘。
		// &m2 乘数矩阵[in]。
		Matrix4d operator * (const Matrix4d& m2) const;

		// 矩阵相乘。
		// &m2 另外一个4X4矩阵[in]。
		Matrix4d Concatenate(const Matrix4d& m2) const;

		// 重载[]，取出对应行。
		// iRow 行号，有效范围为零到三[in]。
		double* operator[] (unsigned int iRow);

		// 取出只读矩阵行数据。
		// iRow 行号，有效范围为零到三[in]。
		const double* const operator [] (unsigned int iRow) const;

		// 判断两个矩阵是否相等。
		// &m2 另外一个矩阵[in]。
		bool operator == (const Matrix4d& m2) const;

		// 判断两个矩阵是否不相等。
		// &m2 另外一个矩阵[in]。
		bool operator != (const Matrix4d& m2) const;

		// 计算矩阵的伴随矩阵。
		Matrix4d Adjoint() const;

		// 计算矩阵行列式值。
		double Determinant() const;

		// 计算机矩阵逆矩阵。
		// return 矩阵逆矩阵。
		Matrix4d Invert() const;

		// 计算4x4中任意3x3子矩阵行列式。
		// m 4x4矩阵[in]。
		// r0 第一行行号[in]。
		// r1 第二行行号[in]。
		// r2 第三行行号[in]。
		// c0 第一列列号[in]。
		// c1 第二列列号[in]。
		// c2 第三列列号[in]。
		double MINOR(const Matrix4d& m, const int r0, const int r1, const int r2,
			const int c0, const int c1, const int c2) const;

		// 设置矩阵平移部分, [3][0], [3][1], [3][2]的值。
		// v 平移向量[in]。
		void SetTrans(const Vector3d& v);

		// 获取矩阵平移部分, [3][0], [3][1], [3][2]的值。
		// v 平移向量[in]。
		Vector3d GetTrans() const;

		double m[4][4];

		// 获取矩阵的指针
		double* GetPointer();
	};
}

#endif