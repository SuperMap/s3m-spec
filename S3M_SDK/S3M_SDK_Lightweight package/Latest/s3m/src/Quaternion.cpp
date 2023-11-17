#include "Quaternion.h"

namespace S3MB
{
	const Quaternion Quaternion::IDENTITY(0.0, 0.0, 0.0, 1.0);
	const Quaternion Quaternion::ZERO(0.0, 0.0, 0.0, 0.0);
	const double Quaternion::msEpsilon = 1.0e-04;
}