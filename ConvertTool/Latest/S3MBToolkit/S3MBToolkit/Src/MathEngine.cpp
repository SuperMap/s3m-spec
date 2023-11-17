#include "MathEngine.h"
#include "S3MBCommon.h"
#include "Utils.h"
#include <cmath>
#include <assert.h>

namespace S3MB
{
	SceneType MathEngine::m_enSceneType = ST_EARTH_ELLIPSOID_WGS84;
	const double wgs84CenterToleranceSquared = 0.1;
	const double EPSILON12 = 0.000000000001;

	Vector3d MathEngine::SphericalToCartesian(double dLongitude, double dLatitude, double dRadius)
	{
		return SphericalToCartesian(MathEngine::m_enSceneType, dLongitude, dLatitude, dRadius);
	}

	Vector3d MathEngine::SphericalToCartesian(SceneType sceneType, double dLongitude, double dLatitude, double dRadius)
	{
		switch (sceneType)
		{
		case ST_EARTH_SPHERICAL:
		{
			double dRadCosLat = dRadius * (double)cos(dLatitude);
			return Vector3d((double)(dRadCosLat*sin(dLongitude)),
				(double)(dRadius*sin(dLatitude)),
				(double)(dRadCosLat*cos(dLongitude)));
		}
		break;
		case ST_NONEARTH:
		{
			return Vector3d(dLongitude, dLatitude, dRadius);
		}
		break;
		case ST_EARTH_ELLIPSOID_WGS84:
		{
			return WGS84ToCartesian(dLongitude, dLatitude, dRadius);
		}
		break;
		default:
			return Vector3d(dLongitude, dLatitude, dRadius);
		}
	}

	Vector3d MathEngine::WGS84ToCartesian(double dLongitude, double dLatitude, double dRadius)
	{
		dRadius -= GLOBAL_RADIUS;
		double dCosLatitude = (double)cos(dLatitude);

		Vector3d vecScratchN, vecScratchK;
		vecScratchN.z = dCosLatitude * cos(dLongitude);
		vecScratchN.x = dCosLatitude * sin(dLongitude);
		vecScratchN.y = sin(dLatitude);

		vecScratchN.Normalize();

		vecScratchK = Vector3d(vecScratchN.x * wgs84RadiiSquaredPlatform.x, vecScratchN.y * wgs84RadiiSquaredPlatform.y, vecScratchN.z * wgs84RadiiSquaredPlatform.z);

		double dGamma = sqrt(vecScratchN.x * vecScratchK.x + vecScratchN.y * vecScratchK.y + vecScratchN.z * vecScratchK.z);
		vecScratchK = vecScratchK / dGamma;

		vecScratchN = vecScratchN * dRadius;

		return vecScratchK + vecScratchN;
	}

	Vector3d MathEngine::CartesianToWGS84(double x, double y, double z, GlobalShape gs)
	{
		Vector3d vOneOverRadii, vOneOverRadiiSquared;
		switch (gs)
		{
		case GS_SPHERE:
			vOneOverRadiiSquared = wgs84SphericalOneOverRadiiSquared;
			break;
		case GS_ELLIPSOID:
			vOneOverRadiiSquared = wgs84OneOverRadiiSquared;
			break;
		default:
			break;
		}

		Vector3d v3Position(x, y, z);

		double dLongitude(0.0), dLatitude(0.0), dHeight(0.0);
		if (gs == GS_ELLIPSOID)
		{
			double dX2 = x * x * vOneOverRadiiSquared.x;
			double dY2 = y * y * vOneOverRadiiSquared.y;
			double dZ2 = z * z * vOneOverRadiiSquared.z;

			double dSquaredNorm = dX2 + dY2 + dZ2;
			double dRatio = sqrt(1.0 / dSquaredNorm);

			if (dSquaredNorm < wgs84CenterToleranceSquared)
			{
				v3Position = v3Position * dRatio;
			}
			else
			{
				Vector3d v3Intersection(x * dRatio, y * dRatio, z * dRatio);
				Vector3d v3Gradient;
				v3Gradient.x = v3Intersection.x * vOneOverRadiiSquared.x * 2.0;
				v3Gradient.y = v3Intersection.y * vOneOverRadiiSquared.y * 2.0;
				v3Gradient.z = v3Intersection.z * vOneOverRadiiSquared.z * 2.0;

				double dLambda = (1.0 - dRatio) * Vector3d(x, y, z).Length() / (0.5 * v3Gradient.Length());
				double dCorrection = 0.0;

				double dFunc;
				double dXMultiplier, dYMultiplier, dZMultiplier;

				do
				{
					dLambda -= dCorrection;

					dXMultiplier = 1.0 / (1.0 + dLambda * vOneOverRadiiSquared.x);
					dYMultiplier = 1.0 / (1.0 + dLambda * vOneOverRadiiSquared.y);
					dZMultiplier = 1.0 / (1.0 + dLambda * vOneOverRadiiSquared.z);

					double dXMultiplier2 = dXMultiplier * dXMultiplier;
					double dYMultiplier2 = dYMultiplier * dYMultiplier;
					double dZMultiplier2 = dZMultiplier * dZMultiplier;

					double dXMultiplier3 = dXMultiplier2 * dXMultiplier;
					double dYMultiplier3 = dYMultiplier2 * dYMultiplier;
					double dZMultiplier3 = dZMultiplier2 * dZMultiplier;

					dFunc = dX2 * dXMultiplier2 + dY2 * dYMultiplier2 + dZ2 * dZMultiplier2 - 1.0;

					double dDenominator = dX2 * dXMultiplier3 * vOneOverRadiiSquared.x + dY2 * dYMultiplier3 * vOneOverRadiiSquared.y + dZ2 * dZMultiplier3 * vOneOverRadiiSquared.z;

					double dDerivative = -2.0 * dDenominator;

					dCorrection = dFunc / dDerivative;
				} while (ABS(dFunc) > EPSILON12);

				v3Position.x *= dXMultiplier; v3Position.y *= dYMultiplier; v3Position.z *= dZMultiplier;
			}

			Vector3d v3N(v3Position.x * vOneOverRadiiSquared.x, v3Position.y * vOneOverRadiiSquared.y, v3Position.z * vOneOverRadiiSquared.z);
			v3N.Normalize();

			Vector3d v3H(x - v3Position.x, y - v3Position.y, z - v3Position.z);

			dLongitude = atan2(v3N.y, v3N.x);
			dLatitude = asin(v3N.z);
			dHeight = Sign(v3H.x * x + v3H.y * y + v3H.z * z) * v3H.Length();
			dHeight += GLOBAL_RADIUS;
		}
		else
		{
			dHeight = v3Position.Length();
			if (!IS0(dHeight))
			{
				dLongitude = atan2(y, x);
				dLatitude = asin(z / dHeight);
			}
		}

		return Vector3d(dLongitude, dLatitude, dHeight);
	}

	Vector3d MathEngine::CartesianToSphericalD(double x, double y, double z)
	{
		return CartesianToSphericalD(x, y, z, MathEngine::m_enSceneType);
	}

	Vector3d MathEngine::CartesianToSphericalD(double x, double y, double z, SceneType sceneType)
	{
		switch (sceneType)
		{
		case ST_EARTH_SPHERICAL:
		{
			double rho = sqrt(x * x + y * y + z * z);
			if (IS0(rho))
			{
				return Vector3d();
			}
			double longitude = atan2(x, z);
			double latitude = asin(y / rho);

			return Vector3d(longitude, latitude, rho);
		}
		break;
		case ST_NONEARTH:
		{
			return Vector3d(x, y, z);
		}
		break;
		case ST_EARTH_ELLIPSOID_WGS84:
		{
			if (IS0(x) && IS0(y) && IS0(z))
			{
				return Vector3d();
			}
			return CartesianToWGS84(z, x, y, GS_ELLIPSOID);
		}
		break;
		default:
			return Vector3d(x, y, z);
		}
	}

	Vector3d MathEngine::WGS84ToCesiumCartesian(double dLongitude, double dLatitude, double dRadius /* = GLOBAL_RADIUS */, GlobalShape gs /* = GS_ELLIPSOID */)
	{
		dRadius -= GLOBAL_RADIUS;

		Vector3d vRadii;
		switch (gs)
		{
		case GS_SPHERE:
			vRadii = wgs84SphericalRadiiSquared;
			break;
		case GS_ELLIPSOID:
			vRadii = wgs84RadiiSquared;
			break;
		default:
			assert(false);
			break;
		}

		Vector3d n = Vector3d(cos(dLatitude) * cos(dLongitude), cos(dLatitude) * sin(dLongitude), sin(dLatitude));
		Vector3d k = Vector3d(vRadii.x * n.x, vRadii.y * n.y, vRadii.z * n.z);

		double gamma = sqrt(k.x * n.x + k.y * n.y + k.z * n.z);

		Vector3d rSurface = k / gamma;
		Vector3d vz = n * dRadius;

		return rSurface + vz;
	}

	Matrix4d MathEngine::EllipsoidalENUToWGS84(const Vector3d& vBaseLonLatPt, double dRadius)
	{
		Vector3d vBaseCarPt = WGS84ToCesiumCartesian(vBaseLonLatPt.x * DTOR, vBaseLonLatPt.y * DTOR, vBaseLonLatPt.z + dRadius);

		Vector3d vUp(vBaseCarPt.x * wgs84OneOverRadiiSquared.x, vBaseCarPt.y * wgs84OneOverRadiiSquared.y, vBaseCarPt.z * wgs84OneOverRadiiSquared.z);
		vUp.Normalize();

		Vector3d vEast(-vBaseCarPt.y, vBaseCarPt.x, 0.0);
		vEast.Normalize();

		Vector3d vNorth = VectorUtils::CrossProduct(vUp, vEast);
		vNorth.Normalize();

		return Matrix4d(
            vEast.x, vEast.y, vEast.z, 0.0,
            vNorth.x, vNorth.y, vNorth.z, 0.0,
            vUp.x, vUp.y, vUp.z, 0.0,
            vBaseCarPt.x, vBaseCarPt.y, vBaseCarPt.z, 1.0);
	}

	Vector3d MathEngine::GlobalENUToSpherical(const Vector3d& vBasePt, double dX, double dY, double dZ, double dRadius /* = GLOBAL_RADIUS */)
	{
		Matrix4d rotateMat = MatrixUtils::RotationXYZ(-vBasePt.y * DTOR, vBasePt.x * DTOR, 0.0);
		Vector3d vCartesian = SphericalToCartesian(ST_EARTH_SPHERICAL, vBasePt.x * DTOR, vBasePt.y * DTOR, vBasePt.z + dRadius);
		Matrix4d posMat = MatrixUtils::Translation(vCartesian.x, vCartesian.y, vCartesian.z);

		Vector3d vCar = Vector3d(dX, dY, dZ).MultiplyMatrix(rotateMat * posMat);
		Vector3d v = CartesianToSphericalD(vCar.x, vCar.y, vCar.z, ST_EARTH_SPHERICAL);
		return Vector3d(v.x * RTOD, v.y * RTOD, v.z - GLOBAL_RADIUS);
	}

	double MathEngine::Sign(double fValue)
	{
		if (fValue > 0.0)
			return 1.0;

		if (fValue < 0.0)
			return -1.0;

		return 0.0;
	}

	void MathEngine::ScaleImageInternal(int components, int widthin, int heightin,
		const unsigned char* datain,
		int widthout, int heightout,
		unsigned char* dataout, const int nsizedatain)
	{
		float x, lowx, highx, convx, halfconvx;
		float y, lowy, highy, convy, halfconvy;
		float xpercent, ypercent;
		float percent;
		/* Max components in a format is 4, so... */
		float totals[4];
		float area;
		int i, j, k, yint, xint, xindex, yindex;
		int temp;
		unsigned char* pData = (unsigned char*)datain;
		bool bDirty = false;

		if (widthin == widthout * 2 && heightin == heightout * 2) {
			HalveImage(components, widthin, heightin, pData, dataout);
			return;
		}

		if (components == 3)
		{
			pData = new unsigned char[widthin * heightin * 3];
			bDirty = true;
			if (!ClipImageData(components, widthin, heightin, datain, nsizedatain, pData))
			{
				delete[] pData;
				bDirty = false;
				pData = (unsigned char*)datain;
			}
		}

		convy = (float)heightin / heightout;
		convx = (float)widthin / widthout;
		halfconvy = convy / 2;
		halfconvx = convx / 2;
		for (i = 0; i < heightout; i++)
		{
			y = convy * (i + 0.5);
			if (heightin > heightout)
			{
				lowy = y - halfconvy;
				highy = halfconvy + y;
			}
			else
			{
				lowy = y - 0.5;
				highy = 0.5 + y;
			}
			for (j = 0; j < widthout; j++)
			{
				x = convx * (j + 0.5);
				if (widthin > widthout)
				{
					lowx = x - halfconvx;
					highx = halfconvx + x;
				}
				else
				{
					lowx = x - 0.5;
					highx = 0.5 + x;
				}

				/*
				** Ok, now apply box filter to box that goes from (lowx, lowy)
				** to (highx, highy) on input data into this pixel on output
				** data.
				*/
				area = 0.0;
				totals[0] = 0.0;
				totals[1] = 0.0;
				totals[2] = 0.0;
				totals[3] = 0.0;

				y = lowy;
				yint = floor(y);
				while (y < highy)
				{
					yindex = (heightin + yint) % heightin;
					if (highy < yint + 1)
					{
						ypercent = highy - y;
					}
					else
					{
						ypercent = yint - y + 1;
					}

					x = lowx;
					xint = floor(x);

					while (x < highx)
					{
						xindex = (widthin + xint) % widthin;
						if (highx < xint + 1)
						{
							xpercent = highx - x;
						}
						else
						{
							xpercent = xint - x + 1;
						}

						percent = ypercent * xpercent;
						area += percent;
						temp = components * (xindex + yindex * widthin);
						for (k = 0; k < components; k++)
						{
							totals[k] += pData[temp + k] * percent;
						}
						xint++;
						x = xint;
					}
					yint++;
					y = yint;
				}

				temp = components * (j + i * widthout);
				for (k = 0; k < components; k++)
				{
					/* totals[] should be rounded in the case of enlarging an RGB
					* ramp when the type is 332 or 4444
					*/
					dataout[temp + k] = (totals[k] + 0.5) / area;
				}
			}
		}

		if (bDirty)
		{
			delete[] pData;
		}
	}

	void MathEngine::HalveImage(int components, unsigned int width, unsigned int height,
		const unsigned char *datain, unsigned char *dataout)
	{
		int i, j, k;
		int nNewWidth, nNewHeight;
		int delta;
		unsigned char *s;
		const unsigned char *t;

		nNewWidth = width / 2;
		nNewHeight = height / 2;
		delta = components * width;
		t = datain;
		s = dataout;

		/* Piece o' cake! */
		for (i = 0; i < nNewHeight; i++)
		{
			for (j = 0; j < nNewWidth; j++)
			{
				for (k = 0; k < components; k++)
				{
					s[0] = (t[0] + t[components] + t[delta] + t[delta + components] + 2) / 4;
					t++;
					s++;
				}
				t += components;
			}
			t += delta;
		}
	}

	bool MathEngine::ClipImageData(int components, int width, int height,
		const unsigned char *datain, const int ndatainsize, unsigned char *dataout)
	{
		// 目前只有RGB格式需要对数据进行对其处理
		if (components != 3)
		{
			return false;
		}

		if (ndatainsize < 0 || ndatainsize <= components * width * height)
		{
			return false;
		}

		// 计算紧凑数据的宽度，获得宽度差
		int i, j;
		int widthData = WIDTHBYTES(width * IPF_RGB_BITS);
		int widthOffset = widthData - width * 3;
		if (widthOffset == 0)
		{
			return false;
		}

		for (i = 0; i < height; i++)
		{
			int nLineWidth = i * width * 3;
			int nLineWidthOld = i * widthData;
			for (j = 0; j < width; j++)
			{
				dataout[nLineWidth + 3 * j] = datain[nLineWidthOld + 3 * j];
				dataout[nLineWidth + 3 * j + 1] = datain[nLineWidthOld + 3 * j + 1];
				dataout[nLineWidth + 3 * j + 2] = datain[nLineWidthOld + 3 * j + 2];
			}
		}

		return true;
	}
}