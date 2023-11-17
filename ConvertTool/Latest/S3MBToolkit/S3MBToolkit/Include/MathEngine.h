#if !defined(MATHENGINE_H__FA79DC0B_BA21_42AE_9B4C_150DFF32012E__INCLUDED_)
#define MATHENGINE_H__FA79DC0B_BA21_42AE_9B4C_150DFF32012E__INCLUDED_

#pragma once
#include "Common.h"
#include "Vector.h"
#include "Matrix.h"

namespace S3MB
{
	//! \brief 标准椭球参数
	const Vector3d wgs84RadiiAxis(GLOBAL_RADIUS, GLOBAL_RADIUS, GLOBAL_RADIUS_MINOR);
	const Vector3d wgs84RadiiSquared(wgs84RadiiAxis.x * wgs84RadiiAxis.x, wgs84RadiiAxis.y * wgs84RadiiAxis.y, wgs84RadiiAxis.z * wgs84RadiiAxis.z);
	const Vector3d wgs84OneOverRadiiSquared(1.0 / wgs84RadiiSquared.x, 1.0 / wgs84RadiiSquared.y, 1.0 / wgs84RadiiSquared.z);

	//! \brief 平台椭球参数
	const Vector3d wgs84RadiiAxisPlatform(wgs84RadiiAxis.y, wgs84RadiiAxis.z, wgs84RadiiAxis.x);
	const Vector3d wgs84RadiiSquaredPlatform(wgs84RadiiAxisPlatform.x * wgs84RadiiAxisPlatform.x, wgs84RadiiAxisPlatform.y * wgs84RadiiAxisPlatform.y, wgs84RadiiAxisPlatform.z * wgs84RadiiAxisPlatform.z);

	//! \brief 标准圆球参数
	const Vector3d wgs84SphericalRadiiAxis(GLOBAL_RADIUS, GLOBAL_RADIUS, GLOBAL_RADIUS);
	const Vector3d wgs84SphericalRadiiSquared(wgs84SphericalRadiiAxis.x * wgs84SphericalRadiiAxis.x, wgs84SphericalRadiiAxis.y * wgs84SphericalRadiiAxis.y, wgs84SphericalRadiiAxis.z * wgs84SphericalRadiiAxis.z);
	const Vector3d wgs84SphericalOneOverRadii(1.0 / wgs84SphericalRadiiAxis.x, 1.0 / wgs84SphericalRadiiAxis.y, 1.0 / wgs84SphericalRadiiAxis.z);
	const Vector3d wgs84SphericalOneOverRadiiSquared(1.0 / wgs84SphericalOneOverRadii.x, 1.0 / wgs84SphericalOneOverRadii.y, 1.0 / wgs84SphericalOneOverRadii.z);

	enum SceneType
	{
		ST_EARTH_SPHERICAL = 0,				//球场景
		ST_EARTH_PROJECTION = 1,			//平面的投影场景
		ST_NONEARTH = 2,					//平面的局部坐标场景
		ST_EARTH_ELLIPSOID_WGS84 = 3,		//WGS84椭球坐标场景
	};

	//! \biref 全球形状
	enum GlobalShape
	{
		//! \brief 圆球
		GS_SPHERE,
		//! \brief 椭球
		GS_ELLIPSOID
	};

	class STK_API MathEngine
	{
	public:
		//! \brief 地理坐标转换为笛卡尔坐标系，默认地球半径为6378137 
		//! \param dLongitude 地理坐标经度[in]。
		//! \param dLatitude 地理坐标维度[in]。
		//! \param GLOBAL_RADIUS 常量6378137[in]。
		//! \return 笛卡尔坐标系的坐标。
		//! \attention 球为圆球而不是椭球。
		static Vector3d SphericalToCartesian(double dLongitude, double dLatitude, double dRadius = GLOBAL_RADIUS);

		//! \brief 地理坐标转换为笛卡尔坐标系，默认地球半径为6378137 
		//! \param sceneType 场景类型[in]。
		//! \param dLongitude 地理坐标经度[in]。
		//! \param dLatitude 地理坐标维度[in]。
		//! \param GLOBAL_RADIUS 常量6378137[in]。
		//! \return 笛卡尔坐标系的坐标。
		//! \attention 球为圆球而不是椭球。
		static Vector3d SphericalToCartesian(SceneType sceneType, double dLongitude, double dLatitude, double dRadius = GLOBAL_RADIUS);

		//! \brief WGS84椭球体换为笛卡尔坐标系，默认地球半径为6378137 
		//! \param dLongitude 地理坐标经度[in]。
		//! \param dLatitude 地理坐标维度[in]。
		//! \param GLOBAL_RADIUS 常量6378137[in]。
		//! \return 笛卡尔坐标系的坐标。
		//! \attention 球为圆球而不是椭球。
		static Vector3d WGS84ToCartesian(double dLongitude, double dLatitude, double dRadius = GLOBAL_RADIUS);

		//! \brief 笛卡尔坐标转换为球面坐标（地理坐标）。
		//! \param x 笛卡尔坐标x[in]。
		//! \param y 笛卡尔坐标y[in]。
		//! \param z 笛卡尔坐标z[in]。
		//! \return 球面坐标系坐标。
		static Vector3d CartesianToWGS84(double x, double y, double z, GlobalShape gs = GS_ELLIPSOID);

		//! \brief 笛卡尔坐标转换为球面坐标（地理坐标）。
		//! \param x 笛卡尔坐标x[in]。
		//! \param y 笛卡尔坐标y[in]。
		//! \param z 笛卡尔坐标z[in]。
		//! \return 球面坐标系坐标。
		static Vector3d CartesianToSphericalD(double x, double y, double z);
		//! \brief 笛卡尔坐标转换为球面坐标（地理坐标）。
		//! \param x 笛卡尔坐标x[in]。
		//! \param y 笛卡尔坐标y[in]。
		//! \param z 笛卡尔坐标z[in]。
		//! \return 球面坐标系坐标。
		static Vector3d CartesianToSphericalD(double x, double y, double z, SceneType sceneType);

		//! \brief WGS84椭球体换为Cesium下的笛卡尔坐标系，默认地球半径为6378137 
		//! \param dLongitude 地理坐标经度[in]。
		//! \param dLatitude 地理坐标维度[in]。
		//! \param GLOBAL_RADIUS 常量6378137[in]。
		//! \return Cesium下的笛卡尔坐标系的坐标。
		//! \attention 我们的球为圆球而不是椭球。
		static Vector3d WGS84ToCesiumCartesian(double dLongitude, double dLatitude, double dRadius = GLOBAL_RADIUS, GlobalShape gs = GS_ELLIPSOID);

		//! \brief 椭球体ENU坐标系转换为地理坐标的转换矩阵。
		static Matrix4d EllipsoidalENUToWGS84(
			const Vector3d& vBaseLonLatPt,
			double dRadius = GLOBAL_RADIUS);

		//! \brief 圆球体ENU坐标系转换为球面坐标（地理坐标）。
		//! \param vBasePt [in] 基准点坐标。
		//! \param dX [in] 笛卡尔坐标x。
		//! \param dY [in] 笛卡尔坐标y。
		//! \param dZ [in] 笛卡尔坐标z。
		//! \param dRadius [in] 球半径，默认GLOBAL_RADIUS。
		//! \return 地理坐标系坐标。
		static Vector3d GlobalENUToSpherical(
			const Vector3d& vBasePt,
			double dX,
			double dY,
			double dZ,
			double dRadius = GLOBAL_RADIUS);

		//! \brief 判断浮点型的正负。
		//! \param fValue 传入的浮点型值[in]。
		//! \return 正数返回1，负数返回-1，0返回0.0。
		static double Sign(double fValue);

		//! \brief 通过线性插值和箱过滤缩放。from gluScaleImage
		//! \param components 颜色成分数量，RGBA是4
		//! \param widthin 输入图像的宽度
		//! \param heightin 输入图像的高度
		//! \param datain 输入图像的指针
		//! \param widthout 输出图像的宽度
		//! \param heightout 输出图像的高度
		//! \param dataout 输出图像的指针
		//! \param nsizedatain datain的大小
		static void ScaleImageInternal(int components, int widthin, int heightin,
			const unsigned char* datain,
			int widthout, int heightout,
			unsigned char* dataout, const int nsizedatain = -1);

		//! \brief 通过线性插值和箱过滤缩放。
		//! \param components 颜色成分数量，RGBA是4
		//! \param widthin 输入图像的宽度
		//! \param heightin 输入图像的高度
		//! \param datain 输入图像的指针
		//! \param dataout 输出图像的指针
		static void HalveImage(int components, unsigned int width, unsigned int height,
			const unsigned char *datain, unsigned char *dataout);

		//! \brief 将图像数据内容进行对齐处理，暂时只对24位图像处理
		//! \param components 颜色成分数量，RGBA是4
		//! \param width 输入图像的宽度
		//! \param height 输入图像的高度
		//! \param datain 输入图像的指针
		//! \param dataout 输出图像的指针
		static bool ClipImageData(int components, int width, int height,
			const unsigned char *datain, const int ndatainsize, unsigned char *dataout);

	public:
		static SceneType m_enSceneType;
	};
}

#endif