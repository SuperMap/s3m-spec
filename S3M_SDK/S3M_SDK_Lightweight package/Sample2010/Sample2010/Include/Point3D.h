#pragma once
#include <vector>
#ifndef IS0(x)
//! \brief  极小值定义
#define EP 1e-10
#define NEP -1e-10
#define IS0(x) (((x) < EP) && ((x) > NEP))
#endif // !IS0(x)

#ifndef EQUAL(X,Y)
#define EQUAL(X,Y) IS0(X-Y)
#endif // !EQUAL(X,Y)


using namespace std;
namespace S3MB
{

	class Point3D
	{
	public:
		//! \brief 点的x轴坐标
		double x;
		//! \brief 点的y轴坐标
		double y;
		//! \brief 点的z轴坐标
		double z;
	public:
		//! \brief  	默认构造函数
		Point3D()
			:x(0), y(0), z(0) {}
		//! \brief  	缺省析构函数
		~Point3D() {}

		//! \brief  带参数构造函数
		Point3D(double xx, double yy, double zz)
			:x(xx), y(yy), z(zz) {}

	public:
		//! \brief 		重载 = 
		Point3D& operator=(const Point3D& s);
		//! \brief 		重载 += 
		Point3D& operator+=(const Point3D& p);
		//! \brief 		重载 -= 
		Point3D& operator-=(const Point3D& p);
		//! \brief 		重载 *= 
		Point3D& operator*=(double c);
		//! \brief 		重载 /= 
		Point3D& operator/=(double c);
		//! \brief 		重载 - 负号
		Point3D operator-() const;
		//! \brief 		重载 == 
		friend  bool operator==(const Point3D& p, const Point3D& q);
		//! \brief 		重载 !=
		friend  bool operator!=(const Point3D& p, const Point3D& q);

	};

	typedef vector<Point3D> Point3Ds;

}