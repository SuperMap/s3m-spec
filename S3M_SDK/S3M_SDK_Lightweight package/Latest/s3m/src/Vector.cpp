#include "Vector.h"
#include "S3MBCommon.h"
#include "Matrix.h"
#include <assert.h>
#include <math.h>

namespace S3MB
{
	Vector3d::Vector3d()
		:x(0), y(0), z(0)
	{
	}

	Vector3d::Vector3d(double fx, double fy, double fz)
		:x(fx), y(fy), z(fz)
	{
	}

	double Vector3d::operator[](const unsigned i) const
	{
		assert(i < 3);
		return *(&x + i);
	}

	double& Vector3d::operator[](const unsigned i)
	{
		assert(i < 3);
		return *(&x + i);
	}

	Vector3d Vector3d::operator+(const Vector3d& vec) const
	{
		Vector3d TempVec;
		TempVec.x = x + vec.x;
		TempVec.y = y + vec.y;
		TempVec.z = z + vec.z;
		return TempVec;
	}

	Vector3d Vector3d::operator-(const Vector3d& vec) const
	{
		Vector3d TempVec;
		TempVec.x = x - vec.x;
		TempVec.y = y - vec.y;
		TempVec.z = z - vec.z;
		return TempVec;
	}

	Vector3d Vector3d::operator * (const double fScale) const
	{
		Vector3d TempVec;
		TempVec.x = x * fScale;
		TempVec.y = y * fScale;
		TempVec.z = z * fScale;
		return TempVec;
	}

	Vector3d Vector3d::operator / (const double fScale) const
	{
		Vector3d vec;
		if (!IS0(fScale))
		{
			vec.x = x / fScale;
			vec.y = y / fScale;
			vec.z = z / fScale;
			return vec;

		}
		return *this;
	}

	double Vector3d::DotProduct(const Vector3d& vec) const
	{
		return x * vec.x + y * vec.y + z * vec.z;
	}
	double Vector3d::Length() const
	{
		return sqrt(x * x + y * y + z * z);
	}

	double Vector3d::Length2() const
	{
		return x * x + y * y + z * z;
	}

	void Vector3d::MakeFloor(const Vector3d& cmp)
	{
		if (cmp.x < x) x = cmp.x;
		if (cmp.y < y) y = cmp.y;
		if (cmp.z < z) z = cmp.z;
	}

	void Vector3d::MakeCeil(const Vector3d& cmp)
	{
		if (cmp.x > x) x = cmp.x;
		if (cmp.y > y) y = cmp.y;
		if (cmp.z > z) z = cmp.z;
	}

	Vector3d Vector3d::MultiplyMatrix(const Matrix4d& m) const
	{
		double w = 1;
		double m11 = 0, m12 = 0, m13 = 0, m14 = 0;

		m11 = x * m[0][0] + y * m[1][0] + z * m[2][0] + w * m[3][0];
		m12 = x * m[0][1] + y * m[1][1] + z * m[2][1] + w * m[3][1];
		m13 = x * m[0][2] + y * m[1][2] + z * m[2][2] + w * m[3][2];
		m14 = x * m[0][3] + y * m[1][3] + z * m[2][3] + w * m[3][3];

		return Vector3d(m11, m12, m13);
	}

	Vector3d Vector3d::operator-() const
	{
		Vector3d TempVec;
		TempVec.x = -x;
		TempVec.y = -y;
		TempVec.z = -z;
		return TempVec;
	}

	void Vector3d::Normalize()
	{
		double fLength = 0.0;
		fLength = sqrt(x * x + y * y + z * z);
		if (!IS0(fLength))
		{
			x /= fLength;
			y /= fLength;
			z /= fLength;
		}
	}

	Vector4d::Vector4d()
		:x(0), y(0), z(0), w(0)
	{
	}

	Vector4d::Vector4d(const double fx, const double fy, const double fz, const double fw)
		:x(fx), y(fy), z(fz), w(fw)
	{
	}

	double Vector4d::operator[](const unsigned i) const
	{
		assert(i < 4);
		return *(&x + i);
	}

	double& Vector4d::operator[](const unsigned i)
	{
		assert(i < 4);
		return *(&x + i);
	}
}