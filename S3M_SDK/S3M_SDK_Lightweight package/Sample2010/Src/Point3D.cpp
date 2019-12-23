#include "Point3D.h"
bool operator==(const Point3D& p, const Point3D& q)
{
	return EQUAL(p.x, q.x) && EQUAL(p.y, q.y) && EQUAL(p.z, q.z);
}

bool operator!=(const Point3D& p, const Point3D& q)
{
	return !EQUAL(p.x, q.x) || !EQUAL(p.y, q.y) || !EQUAL(p.z, q.z);
}

Point3D& Point3D::operator=(const Point3D& s)
{
	if (this == &s)
		return *this;
	x = s.x;
	y = s.y;
	z = s.z;
	return *this;
}

Point3D& Point3D::operator+=(const Point3D& p)
{
	x += p.x;
	y += p.y;
	z += p.z;
	return *this;
}

Point3D& Point3D::operator-=(const Point3D& p)
{
	x -= p.x;
	y -= p.y;
	z -= p.z;
	return *this;
}

Point3D& Point3D::operator*=(double c)
{
	x *= c;
	y *= c;
	z *= c;
	return *this;
}

Point3D& Point3D::operator/=(double c)
{
	if (!IS0(c))
	{
		x /= c;
		y /= c;
		z /= c;
	}

	return *this;
}

Point3D Point3D::operator-() const
{
	return Point3D(-x, -y, -z);
}