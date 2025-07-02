#pragma once
#if !defined(S3MB_QUATERNION)
#define S3MB_QUATERNION
#include "Vector.h"

namespace S3MB
{
	class S3MB_API Quaternion
	{
	public:
		Quaternion()
			:x(0), y(0), z(0), w(1.0) {}

		Quaternion(double x, double y, double z, double w)
			:x(x), y(y), z(z), w(w) {}

		double x;
		double y;
		double z;
		double w;

		static const Quaternion ZERO;
		static const Quaternion IDENTITY;
		static const double msEpsilon;
	};
}

#endif