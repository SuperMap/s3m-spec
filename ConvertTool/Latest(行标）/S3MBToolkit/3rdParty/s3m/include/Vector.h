#if !defined(S3MBVECTOR_H__DE49880B_94AC_4D46_9EA5_BC28C4905284__INCLUDED_)
#define S3MBVECTOR_H__DE49880B_94AC_4D46_9EA5_BC28C4905284__INCLUDED_

#pragma once
#include "stdafx.h"
#include "Matrix.h"

namespace S3MB
{
	class Matrix4d;
	// 三维向量
	class S3MB_API Vector3d
	{
	public:
		Vector3d();

		Vector3d(double fx, double fy, double fz);

		// 重载的取值运算符，取索引i处的分量
		double operator [] (const unsigned i) const;

		// 重载的取值运算符，取索引i处的分量的引用
		double& operator [] (const unsigned i);

		// 重载的加法运算符，求两个向量之和
		Vector3d operator + (const Vector3d& vec) const;

		// 重载的减法运算符，求两个向量之差
		Vector3d operator - (const Vector3d& vec) const;

		// 重载的乘法运算符，求乘以一个数的结果
		Vector3d operator * (const double fScale) const;

		// 重载的除法运算符，求除以一个数的结果
		Vector3d operator / (const double fScale) const;
		
		// 求两个向量的点积
		double DotProduct(const Vector3d& vec) const;

		// 求向量的长度
		double Length() const;

		// 求向量的长度的平方
		double Length2() const;

		// 取两个向量每个分类的最小值形成一个新的向量
		void MakeFloor(const Vector3d& cmp);

		// 取两个向量每个分类的最大值形成一个新的向量
		void MakeCeil(const Vector3d& cmp);

		// 该乘法支持D3D矩阵运算
		// 本系统可以直接使用
		Vector3d MultiplyMatrix(const Matrix4d& m) const;

		// 重载的取反运算符，对向量求反
		// 说明： 对向量求反就是对每个分量求反
		Vector3d operator - () const;

		void Normalize();

		double x, y, z;
	};

	// 四维向量
	class S3MB_API Vector4d
	{
	public:
		Vector4d();

		Vector4d(const double fx, const double fy, const double fz, const double fw);

		// 重载的取值运算符，取索引i处的分量
		double operator [] (const unsigned i) const;

		// 重载的取值运算符，取索引i处的分量的引用
		double& operator [] (const unsigned i);

		double x, y, z, w;
	};
}

#endif