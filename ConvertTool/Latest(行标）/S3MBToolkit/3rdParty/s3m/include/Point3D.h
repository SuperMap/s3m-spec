#if !defined(POINT3D_H__1E4E26C1_2BB4_465E_A4B4_DD47ECC4B9F4__INCLUDED_)
#define POINT3D_H__1E4E26C1_2BB4_465E_A4B4_DD47ECC4B9F4__INCLUDED_

#pragma once
#include "stdafx.h"
#include <vector>

namespace S3MB
{
	class S3MB_API Point3D
	{
	public:
		//  	默认构造函数
		Point3D();
		//  	缺省析构函数
		~Point3D();
		//  带参数构造函数
		Point3D(double xx, double yy, double zz);

	public:
		// 		重载 = 
		Point3D& operator=(const Point3D& s);
		// 		重载 += 
		Point3D& operator+=(const Point3D& p);
		// 		重载 -= 
		Point3D& operator-=(const Point3D& p);
		// 		重载 *= 
		Point3D& operator*=(double c);
		// 		重载 /= 
		Point3D& operator/=(double c);
		// 		重载 - 负号
		Point3D operator-() const;
		// 		重载 == 
		friend S3MB_API bool operator==(const Point3D& p, const Point3D& q);
		// 		重载 !=
		friend S3MB_API bool operator!=(const Point3D& p, const Point3D& q);

	public:
		// 点的x轴坐标
		double x;
		// 点的y轴坐标
		double y;
		// 点的z轴坐标
		double z;
	};

	typedef vector<Point3D> Point3Ds;
}

#endif
