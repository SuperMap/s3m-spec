//////////////////////////////////////////////////////////////////////////
//                    _   _   ____   ____                    
//   Project         | | | | / ___) / ___)                   
//                   | | | || | __ | |                       
//                   | |_| || |_\ || \___                    
//                    \____/ \____| \ ___)     6.0           
//                                                           
//!  \file headerfile.h
//!  \brief 三维数学类。
//!  \details 完成对三维所用到的数学函数的封装。
//!  \author 。
//!  \attention 
//!   Copyright (c) 1996-2010 SuperMap Software Co., Ltd. <br> 
//!   All Rights Reserved.
//!  \version 6.0
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_UGMATHENGINE_H__90331450_3347_42D5_B0A0_41353F9437C8__INCLUDED_)
#define AFX_UGMATHENGINE_H__90331450_3347_42D5_B0A0_41353F9437C8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "Base3D/UGPrerequisites3D.h"
#include "Toolkit/UGPalette.h"
#include "Base3D/UGMesh.h"

//手动计算四个顶点的矩阵变换 2011.02.22 wangxinhe,目前只需要3个点 2012.04.01
#define MATRIX_MULTI(VERTICES,MATRIX,ARRAYX,ARRAYY,ARRAYZ) 	\
		VERTICES[0] =MATRIX[0][0] *ARRAYX[0] + MATRIX[1][0] *ARRAYY[0] + MATRIX[2][0] *ARRAYZ[0] +MATRIX[3][0]; \
		VERTICES[3] =MATRIX[0][0] *ARRAYX[1] + MATRIX[1][0] *ARRAYY[1] + MATRIX[2][0] *ARRAYZ[1] +MATRIX[3][0]; \
		VERTICES[6] =MATRIX[0][0] *ARRAYX[2] + MATRIX[1][0] *ARRAYY[2] + MATRIX[2][0] *ARRAYZ[2] +MATRIX[3][0]; \
		VERTICES[1] =MATRIX[0][1] *ARRAYX[0] + MATRIX[1][1] *ARRAYY[0] + MATRIX[2][1] *ARRAYZ[0] +MATRIX[3][1]; \
		VERTICES[4] =MATRIX[0][1] *ARRAYX[1] + MATRIX[1][1] *ARRAYY[1] + MATRIX[2][1] *ARRAYZ[1] +MATRIX[3][1]; \
		VERTICES[7] =MATRIX[0][1] *ARRAYX[2] + MATRIX[1][1] *ARRAYY[2] + MATRIX[2][1] *ARRAYZ[2] +MATRIX[3][1]; \
		VERTICES[2] =MATRIX[0][2] *ARRAYX[0] + MATRIX[1][2] *ARRAYY[0] + MATRIX[2][2] *ARRAYZ[0] +MATRIX[3][2]; \
		VERTICES[5] =MATRIX[0][2] *ARRAYX[1] + MATRIX[1][2] *ARRAYY[1] + MATRIX[2][2] *ARRAYZ[1] +MATRIX[3][2]; \
		VERTICES[8] =MATRIX[0][2] *ARRAYX[2] + MATRIX[1][2] *ARRAYY[2] + MATRIX[2][2] *ARRAYZ[2] +MATRIX[3][2];

//四个顶点直接赋值 2010.02.22 wangxinhe
#define MATRIX_ASSIGN(VERTICES,ARRAYX,ARRAYY,ARRAYZ)	\
												VERTICES[0] =ARRAYX[0]; \
												VERTICES[1] =ARRAYY[0]; \
												VERTICES[2] =ARRAYZ[0]; \
												VERTICES[3] =ARRAYX[1]; \
												VERTICES[4] =ARRAYY[1]; \
												VERTICES[5] =ARRAYZ[1]; \
												VERTICES[6] =ARRAYX[2]; \
												VERTICES[7] =ARRAYY[2]; \
												VERTICES[8] =ARRAYZ[2];

namespace UGC 
{
#if !defined (IOS) && !defined OS_ANDROID
typedef void (*MESHSCANPROC)(UGint X, UGint Y, UGuint pData);
#endif

typedef struct _TriangleInner
{
	UGVector3d m_InnerPoint;
	UGint			m_MeshID;
	UGint			m_TriangleID;
}TriangleInner;

// 射线与三角形的交点结构
typedef struct UGRayTriangleIntersection
{
	UGdouble dDistance; // 交点距射线起点的距离（与方向向量长度的比值）
	UGbool bSameDirection; // 射线方向与三角形法向量方向是否一致

	UGRayTriangleIntersection()
	{
		dDistance = 0.0;
		bSameDirection = FALSE;
	}
	inline UGbool operator < (const UGRayTriangleIntersection& X) const
	{
		if(this->dDistance < X.dDistance && !UGIS0(this->dDistance - X.dDistance))
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
}RayTriangleIntersection;

// 时区
struct UGTimeZoneInfo
{
	UGint m_bias;
	//夏令时，暂时不考虑
	//UGint m_daylightBais;
	UGTimeZoneInfo()
	{
		//m_daylightBais = 0;
		m_bias = 0;
	}		
	UGTimeZoneInfo(UGint bias)
	{
		m_bias = bias;
	}
};

//! \brief 标准坐标系
//      z
//      |
//     o|______y
//     /
//    / x
//! \brief 标准椭球参数
const UGVector3d wgs84RadiiAxis(GLOBAL_RADIUS, GLOBAL_RADIUS, GLOBAL_RADIUS_MINOR);
const UGVector3d wgs84RadiiSquared(wgs84RadiiAxis.x * wgs84RadiiAxis.x, wgs84RadiiAxis.y * wgs84RadiiAxis.y, wgs84RadiiAxis.z * wgs84RadiiAxis.z);
const UGVector3d wgs84OneOverRadii(1.0 / wgs84RadiiAxis.x, 1.0 / wgs84RadiiAxis.y, 1.0 / wgs84RadiiAxis.z);
const UGVector3d wgs84OneOverRadiiSquared(1.0 / wgs84RadiiSquared.x, 1.0 / wgs84RadiiSquared.y, 1.0 / wgs84RadiiSquared.z);

//! \brief 标准圆球参数
const UGVector3d wgs84SphericalRadiiAxis(GLOBAL_RADIUS, GLOBAL_RADIUS, GLOBAL_RADIUS);
const UGVector3d wgs84SphericalRadiiSquared(wgs84SphericalRadiiAxis.x * wgs84SphericalRadiiAxis.x, wgs84SphericalRadiiAxis.y * wgs84SphericalRadiiAxis.y, wgs84SphericalRadiiAxis.z * wgs84SphericalRadiiAxis.z);
const UGVector3d wgs84SphericalOneOverRadii(1.0 / wgs84SphericalRadiiAxis.x, 1.0 / wgs84SphericalRadiiAxis.y, 1.0 / wgs84SphericalRadiiAxis.z);
const UGVector3d wgs84SphericalOneOverRadiiSquared(1.0 / wgs84SphericalOneOverRadii.x, 1.0 / wgs84SphericalOneOverRadii.y, 1.0 / wgs84SphericalOneOverRadii.z);

//! \brief 平台三维场景使用的坐标系
//      y
//      |
//     o|______x
//     /
//    / z
//! \brief 平台椭球参数
const UGVector3d wgs84RadiiAxisPlatform(wgs84RadiiAxis.y, wgs84RadiiAxis.z, wgs84RadiiAxis.x);
const UGVector3d wgs84RadiiSquaredPlatform(wgs84RadiiAxisPlatform.x * wgs84RadiiAxisPlatform.x, wgs84RadiiAxisPlatform.y * wgs84RadiiAxisPlatform.y, wgs84RadiiAxisPlatform.z * wgs84RadiiAxisPlatform.z);
const UGVector3d wgs84OneOverRadiiPlatform(1.0 / wgs84RadiiAxisPlatform.x, 1.0 / wgs84RadiiAxisPlatform.y, 1.0 / wgs84RadiiAxisPlatform.z);
const UGVector3d wgs84OneOverRadiiSquaredPlatform(1.0 / wgs84RadiiSquaredPlatform.x, 1.0 / wgs84RadiiSquaredPlatform.y, 1.0 / wgs84RadiiSquaredPlatform.z);

//! \brief 标准圆球参数
const UGVector3d wgs84SphericalRadiiAxisPlatform(wgs84SphericalRadiiAxis);
const UGVector3d wgs84SphericalRadiiSquaredPlatform(wgs84SphericalRadiiSquared);
const UGVector3d wgs84SphericalOneOverRadiiPlatform(wgs84SphericalOneOverRadii);
const UGVector3d wgs84SphericalOneOverRadiiSquaredPlatform(wgs84SphericalOneOverRadiiSquared);

//! \brief 三维数学函数类。
class BASE3D_API UGMathEngine  
{
public:
	//! \brief 构造函数
	UGMathEngine();
	//! \brief 析构函数
	virtual ~UGMathEngine();
	//! \brief 将度转换为弧度
	//! \param degrees 以度为单位的值[in]。
	//! \return 返回以弧度表示的值。
	static UGdouble DegreesToRadians(UGdouble degrees);	
	//! \brief 将弧度转换为度
	//! \param dRadians 以弧度为单位的值[in]。
	//! \return 返回以度表示的值。
	static UGdouble RadiansToDegrees(UGdouble dRadians);
	//! \brief 地理坐标转换为笛卡尔坐标系，默认地球半径为6378137 
	//! \param dLongitude 地理坐标经度[in]。
	//! \param dLatitude 地理坐标维度[in]。
	//! \param GLOBAL_RADIUS 常量6378137[in]。
	//! \return 笛卡尔坐标系的坐标。
	//! \attention 我们的球为圆球而不是椭球。
	static UGVector3d SphericalToCartesian(UGdouble dLongitude, UGdouble dLatitude,UGdouble dRadius = GLOBAL_RADIUS);

	//! \brief 地理坐标转换为笛卡尔坐标系，默认地球半径为6378137 
	//! \param sceneType 场景类型[in]。
	//! \param dLongitude 地理坐标经度[in]。
	//! \param dLatitude 地理坐标维度[in]。
	//! \param GLOBAL_RADIUS 常量6378137[in]。
	//! \return 笛卡尔坐标系的坐标。
	//! \attention 我们的球为圆球而不是椭球。
	static UGVector3d SphericalToCartesian(SceneType sceneType, UGdouble dLongitude, UGdouble dLatitude,UGdouble dRadius = GLOBAL_RADIUS);

	//! \brief WGS84椭球体换为笛卡尔坐标系，默认地球半径为6378137 
	//! \param dLongitude 地理坐标经度[in]。
	//! \param dLatitude 地理坐标维度[in]。
	//! \param GLOBAL_RADIUS 常量6378137[in]。
	//! \return 笛卡尔坐标系的坐标。
	//! \attention 我们的球为圆球而不是椭球。
	static UGVector3d WGS84ToCartesian(UGdouble dLongitude, UGdouble dLatitude,UGdouble dRadius = GLOBAL_RADIUS);	

	//! \brief WGS84椭球体换为Cesium下的笛卡尔坐标系，默认地球半径为6378137 
	//! \param dLongitude 地理坐标经度[in]。
	//! \param dLatitude 地理坐标维度[in]。
	//! \param GLOBAL_RADIUS 常量6378137[in]。
	//! \return Cesium下的笛卡尔坐标系的坐标。
	//! \attention 我们的球为圆球而不是椭球。
	static UGVector3d WGS84ToCesiumCartesian(UGdouble dLongitude, UGdouble dLatitude,UGdouble dRadius = GLOBAL_RADIUS, GlobalShape gs = GS_ELLIPSOID);

	//! \brief 将度转换为弧度
	//! \param fDegree 以度为单位的值[in]。
	//! \return 返回以弧度表示的值。
	static UGdouble Degree2Radian(UGdouble fDegree);
	//! \brief 笛卡尔坐标转换为球面坐标（地理坐标）。
	//! \param x 笛卡尔坐标x[in]。
	//! \param y 笛卡尔坐标y[in]。
	//! \param z 笛卡尔坐标z[in]。
	//! \return 球面坐标系坐标。
	static UGVector3d CartesianToSphericalD(UGdouble x, UGdouble y, UGdouble z);
	//! \brief 笛卡尔坐标转换为球面坐标（地理坐标）。
	//! \param x 笛卡尔坐标x[in]。
	//! \param y 笛卡尔坐标y[in]。
	//! \param z 笛卡尔坐标z[in]。
	//! \return 球面坐标系坐标。
	static UGVector3d CartesianToSphericalD(UGdouble x, UGdouble y, UGdouble z, SceneType sceneType);	
	//! \brief 笛卡尔坐标转换为球面坐标（地理坐标）。
	//! \param x 笛卡尔坐标x[in]。
	//! \param y 笛卡尔坐标y[in]。
	//! \param z 笛卡尔坐标z[in]。
	//! \return 球面坐标系坐标。
	static UGVector3d CartesianToWGS84(UGdouble x, UGdouble y, UGdouble z, GlobalShape gs = GS_ELLIPSOID);	


	//! \brief Cesium坐标系的笛卡尔值转成平台圆球坐标系的笛卡尔值。
	//! \param vCesiumCartesianPt [in] Cesium坐标系的笛卡尔值。
	//! \return 平台圆球坐标系的笛卡尔值。
	static UGVector3d FromCesiumCartesian(const UGVector3d& vCesiumCartesianPt, GlobalShape gs = GS_ELLIPSOID);

	//! \brief 平台圆球坐标系的笛卡尔值转成Cesium坐标系的笛卡尔值。
	//! \param vCartesianPt [in] 平台圆球坐标系的笛卡尔值。
	//! \return Cesium坐标系的笛卡尔值。
	static UGVector3d ToCesiumCartesian(const UGVector3d& vCartesianPt, GlobalShape gs = GS_ELLIPSOID);


	//! \brief 地理坐标转换为椭球体ENU坐标系。
	//! \param vBasePt [in] 基准点坐标。
	//! \param dLongitude [in] 地理坐标经度。
	//! \param dLatitude [in] 地理坐标纬度。
	//! \param dHigh [in] 相对地面高度。
	//! \param dRadius [in] 球半径，默认GLOBAL_RADIUS。
	//! \return 椭球体ENU坐标系的坐标。
	static UGVector3d WGS84ToEllipsoidalENU(
		const UGVector3d& vBaseLonLatPt, 
		UGdouble dLongitude, 
		UGdouble dLatitude, 
		UGdouble dHigh,
		UGdouble dRadius = GLOBAL_RADIUS);

	//! \brief 地理坐标转换为椭球体ENU坐标的转换矩阵。
	static UGMatrix4d WGS84ToEllipsoidalENU(
		const UGVector3d& vBaseLonLatPt, 
		UGdouble dRadius = GLOBAL_RADIUS);

	//! \brief 椭球体ENU坐标系转换为地理坐标。
	//! \param vBasePt [in] 基准点坐标。
	//! \param dX [in] 笛卡尔坐标x。
	//! \param dY [in] 笛卡尔坐标y。
	//! \param dZ [in] 笛卡尔坐标z。
	//! \param dRadius [in] 球半径，默认GLOBAL_RADIUS。
	//! \return 地理坐标系坐标。
	static UGVector3d EllipsoidalENUToWGS84(
		const UGVector3d& vBaseLonLatPt, 
		UGdouble dX, 
		UGdouble dY, 
		UGdouble dZ,
		UGdouble dRadius = GLOBAL_RADIUS);

	//! \brief 椭球体ENU坐标系转换为地理坐标的转换矩阵。
	static UGMatrix4d EllipsoidalENUToWGS84(
		const UGVector3d& vBaseLonLatPt,
		UGdouble dRadius = GLOBAL_RADIUS);

	//! \brief 球面坐标（地理坐标）转换成伪平面直角坐标系。
	//! \param dLongitude [in] 地理坐标经度。
	//! \param dLatitude [in] 地理坐标纬度。
	//! \param dHigh [in] 相对地面高度。
	//! \param dFactor [in] 转换因子，默认GLOBAL_RADIUS。
	//! \return 伪平面直角坐标系的坐标。
	static UGVector3d SphericalToPseudoCartesian(
		UGdouble dLongitude, 
		UGdouble dLatitude, 
		UGdouble dHigh, 
		UGdouble dFactor = GLOBAL_RADIUS);

	//! \brief 伪平面直角坐标系转换成球面坐标（地理坐标）。
	//! \param dX [in] 笛卡尔坐标x。
	//! \param dY [in] 笛卡尔坐标y。
	//! \param dZ [in] 笛卡尔坐标z。
	//! \param dFactor [in] 转换因子，默认GLOBAL_RADIUS。
	//! \return 地理坐标系坐标。
	static UGVector3d PseudoCartesianToSpherical(
		UGdouble dX, 
		UGdouble dY, 
		UGdouble dZ, 
		UGdouble dFactor = GLOBAL_RADIUS);

	//! \brief 球面坐标（地理坐标）转换为圆球体ENU坐标系。
	//! \param vBasePt [in] 基准点坐标。
	//! \param dLongitude [in] 地理坐标经度。
	//! \param dLatitude [in] 地理坐标纬度。
	//! \param dHigh [in] 相对地面高度。
	//! \param dRadius [in] 球半径，默认GLOBAL_RADIUS。
	//! \return 圆球体ENU坐标系的坐标。
	static UGVector3d SphericalToGlobalENU(
		const UGVector3d& vBasePt,
		UGdouble dLongitude,
		UGdouble dLatitude, 
		UGdouble dHigh,
		UGdouble dRadius = GLOBAL_RADIUS);

	//! \brief 圆球体ENU坐标系转换为球面坐标（地理坐标）。
	//! \param vBasePt [in] 基准点坐标。
	//! \param dX [in] 笛卡尔坐标x。
	//! \param dY [in] 笛卡尔坐标y。
	//! \param dZ [in] 笛卡尔坐标z。
	//! \param dRadius [in] 球半径，默认GLOBAL_RADIUS。
	//! \return 地理坐标系坐标。
	static UGVector3d GlobalENUToSpherical(
		const UGVector3d& vBasePt, 
		UGdouble dX, 
		UGdouble dY, 
		UGdouble dZ, 
		UGdouble dRadius = GLOBAL_RADIUS);

	//! \brief 球面坐标（地理坐标）转换成以球心为原点的直角坐标系。
	//! \param dLongitude [in] 地理坐标经度。
	//! \param dLatitude [in] 地理坐标纬度。
	//! \param dHigh [in] 相对地面高度。
	//! \param dRadius [in] 球半径，默认GLOBAL_RADIUS。
	//! \return 以球心为原点的直角坐标系的坐标。
	static UGVector3d SphericalToGlobalCartesian(
		UGdouble dLongitude, 
		UGdouble dLatitude, 
		UGdouble dHigh, 
		UGdouble dRadius = GLOBAL_RADIUS);

	//! \brief 以球心为原点的直角坐标系转换成球面坐标（地理坐标）。
	//! \param dX [in] 笛卡尔坐标x。
	//! \param dY [in] 笛卡尔坐标y。
	//! \param dZ [in] 笛卡尔坐标z。
	//! \param dRadius [in] 球半径，默认GLOBAL_RADIUS。
	//! \return 地理坐标系坐标。
	static UGVector3d GlobalCartesianToSpherical(
		UGdouble dX, 
		UGdouble dY, 
		UGdouble dZ, 
		UGdouble dRadius = GLOBAL_RADIUS);


	//! \brief 四元数转换为欧拉角。
	//! \param q 四元数[in]。
	//! \return 欧拉角。
	static UGVector3d QuaternionToEuler(UGQuaternion4d q);	
	//! \brief 欧拉角转换为四元数。
	//! \param yaw 绕y轴旋转的角度[in]。
	//! \param pitch 绕x轴旋转角度[in]。
	//! \param roll 绕z轴旋转的角度[in]。
	//! \return 返回四元数。
	static UGQuaternion4d EulerToQuaternion(UGdouble yaw, UGdouble pitch, UGdouble roll);	

	//! \brief 计算两点的球面角度距离。
	//! \param lonA A点的经度，单位弧度[in]。
	//! \param latA A点的纬度，单位弧度[in]。
	//! \param lonB B点的经度，单位弧度[in]。
	//! \param latB B点的纬度，单位弧度[in]。
	//! \return A、B两点的球面角度距离。
	static UGdouble SphericalDistance( UGdouble lonA, UGdouble latA,UGdouble lonB,UGdouble latB);	
	//! \brief 计算两点的球面角度距离。
	//! \param lonA A点的经度，单位弧度[in]。
	//! \param latA A点的纬度，单位弧度[in]。
	//! \param lonB B点的经度，单位弧度[in]。
	//! \param latB B点的纬度，单位弧度[in]。
	//! \return A、B两点的球面角度距离。
	static UGdouble SphericalDistanceCos( UGdouble lonA, UGdouble latA,UGdouble lonB,UGdouble latB);	
	//! \brief 给一个顶点数组和索引数组计算三角型列表的向量。
	//! \param pVertex 顶点数组[in]。
	//! \param pIndice 构成三角形串的索引数组[in]。
	//! \param nVertexCount 顶点数组的大小[in]。
	//! \param nIndiceCount 索引数组的大小[in]。
	//! \return 各个顶点的向量。
	//! \attention 每个顶点计算出来的是光滑向量，即由周围所有的点来计算该点的向量。
	static UGdouble* ComputeVertexNormals(UGdouble* pVertex,UGushort* pIndice,UGint nVertexCount,UGint nIndiceCount);
	//! \brief 给一个顶点数组和索引数组计算三角型串的向量。
	//! \param pVertex 顶点数组[in]。
	//! \param pIndice 构成三角形串的索引数组[in]。
	//! \param nVertexCount 顶点数组的大小[in]。
	//! \param nIndiceCount 索引数组的大小[in]。
	//! \return 各个顶点的向量。
	//! \attention 每个顶点计算出来的是光滑向量，即由周围所有的点来计算该点的向量。
	static UGfloat* ComputeVertexNormals(UGfloat* pVertex,UGushort* pIndice,UGint nVertexCount,UGint nIndiceCount);
	static UGfloat* ComputeVertexNormals(UGfloat* pVertex,UGuint* pIndice,UGint nVertexCount,UGint nIndiceCount);
	//! \brief 给一个顶点数组数组计算三角型串的向量。
	//! \param pVertex 构成三角形串顶点数组,每三个点构成一个三角形，无索引[in]。
	//! \param nVertexCount 顶点数组的大小[in]。
	//! \return 各个顶点的向量。
	//! \attention 每个顶点计算出来的是光滑向量，即由周围所有的点来计算该点的向量。
	static UGdouble* ComputeVertexNormals(UGdouble* pVertex,UGint nVertexCount);
	//! \brief 给一个顶点数组数组计算三角型串的向量。
	//! \param pVertex 构成三角形串顶点数组,每三个点构成一个三角形，无索引[in]。
	//! \param nVertexCount 顶点数组的大小[in]。
	//! \return 各个顶点的向量。
	//! \attention 每个顶点计算出来的是光滑向量，即由周围所有的点来计算该点的向量。
	static UGfloat* ComputeVertexNormals(const UGfloat* pVertex,const UGuint nVertexCount);

	template<typename TVertex, typename TIndex>
	static UGfloat* ComputeVertexNormals(TVertex* pVertex, TIndex* pIndice,
		UGint nVertexCount,UGint nIndiceCount, UGbool bIndexNormal = FALSE)
	{
		UGVector3d vVector1, vVector2, vNormal, vPoly[3];
		UGint numOfFaces = nIndiceCount / 3;
		UGVector3d *pTempNormals1 = NULL;
		UGfloat* pNormals = NULL;
		try
		{
			pTempNormals1 = new UGVector3d [numOfFaces];
			UGint numOfVerts = nVertexCount, i = 0, j = 0;

			if (bIndexNormal)
			{
				// 每个索引都有一个向量
				pNormals = new UGfloat [nIndiceCount * 3];
			}
			else
			{
				// 每个顶点都有一个向量
				pNormals = new UGfloat [numOfVerts * 3];
			}

			for(i = 0; i < numOfFaces; i++)
			{		
				vPoly[0] = UGVector3d(pVertex[3*pIndice[i*3]],pVertex[3*pIndice[i*3]+1],pVertex[3*pIndice[i*3]+2]);
				vPoly[1] = UGVector3d(pVertex[3*pIndice[i*3+1]],pVertex[3*pIndice[i*3+1]+1],pVertex[3*pIndice[i*3+1]+2]);
				vPoly[2] = UGVector3d(pVertex[3*pIndice[i*3+2]],pVertex[3*pIndice[i*3+2]+1],pVertex[3*pIndice[i*3+2]+2]);

				vVector1 = vPoly[2] - vPoly[0];
				vVector2 = vPoly[2] - vPoly[1];
				vNormal = vVector1.CrossProduct(vVector2);
				UGdouble dNormalLength = vNormal.Length();
				vNormal.x /= dNormalLength;
				vNormal.y /= dNormalLength;
				vNormal.z /= dNormalLength;
				pTempNormals1[i] = vNormal;
			}

			UGVector3d vSum ,temp;
			UGint shared = 0;
			if (bIndexNormal)
			{
				for (i = 0; i < nIndiceCount; i++)
				{
					temp = pTempNormals1[i / 3];
					temp.Normalize();
					pNormals[3 * i + 0] = temp.x;
					pNormals[3 * i + 1] = temp.y;
					pNormals[3 * i + 2] = temp.z;
				}
			}
			else
			{
				for (i = 0; i < numOfVerts; i++)
				{
					for (j = 0; j < numOfFaces; j++)
					{										
						if (pIndice[j * 3 + 0] == i || 
							pIndice[j * 3 + 1] == i || 
							pIndice[j * 3 + 2] == i)
						{
							vSum += pTempNormals1[j];
							shared++;
						}
					}
					UGASSERT(!UGIS0(shared));
					temp = vSum / shared;
					temp.Normalize();
					pNormals[3 * i + 0] = temp.x;
					pNormals[3 * i + 1] = temp.y;
					pNormals[3 * i + 2] = temp.z;
					vSum = UGVector3d::ZERO;
					shared = 0;
				}
			}

			delete [] pTempNormals1;
		}
		catch (std::bad_alloc& )
		{
			if (NULL != pTempNormals1)
			{
				delete [] pTempNormals1;
				pTempNormals1 = NULL;
			}
		}
		return pNormals;
	}

	//! \brief 给一个顶点数组和索引数组计算三角型串的法线向量。
	//! \param pVertex 顶点数组[in]。
	//! \param pIndice 构成三角形串的索引数组[in]。
	//! \param nVertexCount 顶点数组的大小[in]。
	//! \param nIndiceCount 索引数组的大小[in]。
	//! \return 各个顶点的向量。
	//! \attention 根据顶点所在三角形各夹角权重求顶点法线。
	template<typename TVertex, typename TIndex>
	static UGfloat* ComputeVertexNormalsAngle(TVertex* pVertex,TIndex* pIndice,
		UGint nVertexCount,UGint nIndiceCount)
	{
		UGVector3d vVector1, vVector2, vNormal, vPoly[3];
		UGint numOfFaces=nIndiceCount/3;
		UGfloat* pNormals = NULL;
		UGint numOfVerts=nVertexCount;
		UGint i=0;
		UGint j=0;

		//每个顶点都有一个向量
		pNormals = new UGfloat [numOfVerts*3];
		std::map<UGint, UGVector3d> mapNormals;
		for(i=0; i < numOfFaces; i++)
		{		
			// 这样不行,好像有静态成员,所以 不行 memcpy(vPoly,pVertex+3*pIndice[i],9*sizeof(UGdouble));// 拷贝三个点到vPoly
			vPoly[0]=UGVector3d(pVertex[3*pIndice[i*3]],pVertex[3*pIndice[i*3]+1],pVertex[3*pIndice[i*3]+2]);
			vPoly[1]=UGVector3d(pVertex[3*pIndice[i*3+1]],pVertex[3*pIndice[i*3+1]+1],pVertex[3*pIndice[i*3+1]+2]);
			vPoly[2]=UGVector3d(pVertex[3*pIndice[i*3+2]],pVertex[3*pIndice[i*3+2]+1],pVertex[3*pIndice[i*3+2]+2]);

			vVector1 =  vPoly[0]-vPoly[2];
			vVector2 =	vPoly[2]-vPoly[1];
			vNormal=vVector1.CrossProduct(vVector2);
			vNormal.Normalize();
			vNormal = -vNormal;

			UGVector3d e0 = (vPoly[1]-vPoly[0]).Normalize();
			UGVector3d e1 = (vPoly[2]-vPoly[1]).Normalize();
			UGVector3d e2 = (vPoly[0]-vPoly[2]).Normalize();

			UGVector3d& v0Normal = mapNormals[pIndice[i*3]];
			UGVector3d eTemp = -e2;
			v0Normal += vNormal*UGMathEngine::AngleOfV1V2(e0,eTemp);

			UGVector3d& v1Normal = mapNormals[pIndice[i*3+1]];
			eTemp = -e0;
			v1Normal += vNormal*UGMathEngine::AngleOfV1V2(eTemp,e1);

			UGVector3d& v2Normal = mapNormals[pIndice[i*3+2]];
			eTemp = -e1;
			v2Normal += vNormal*UGMathEngine::AngleOfV1V2(eTemp,e2);
		}

		UGVector3d vSum =UGVector3d::ZERO ;
		UGVector3d vZero = vSum;
		int shared=0;
		UGVector3d temp;
		for (i = 0; i < numOfVerts; i++)
		{
			std::map<UGint, UGVector3d>::iterator ito = mapNormals.find(i);
			if (ito != mapNormals.end())
			{
				temp = ito->second;
				temp.Normalize();
			}
			else
			{
				temp = UGVector3d(1.0, 0.0, 0.0);
			}

			pNormals[3*i]=temp.x;
			pNormals[3*i+1]=temp.y;
			pNormals[3*i+2]=temp.z;
			vSum = vZero;
		}
		mapNormals.clear();
		return pNormals;
	}

	//! \brief 给一个顶点数组和索引数组计算三角型串的法线向量。
	//! \param pVertex 顶点数组[in]。
	//! \param pIndice 构成三角形串的索引数组[in]。
	//! \param nVertexCount 顶点数组的大小[in]。
	//! \param nIndiceCount 索引数组的大小[in]。
	//! \return 各个顶点的向量。
	//! \attention 根据Nelson Max的权重算法计算顶点法线。
	template<typename TVertex, typename TIndex>
	static UGfloat* ComputeVertexNormalsNelsonMax(TVertex* pVertex,TIndex* pIndice,
		UGint nVertexCount,UGint nIndiceCount)
	{
		UGVector3d vVector1, vVector2, vNormal, vPoly[3];
		UGint numOfFaces=nIndiceCount/3;
		UGfloat* pNormals = NULL;
		UGint numOfVerts=nVertexCount;
		UGint i=0;
		UGint j=0;

		//每个顶点都有一个向量
		pNormals = new UGfloat [numOfVerts*3];
		std::map<UGint, UGVector3d> mapNormals;
		for(i=0; i < numOfFaces; i++)
		{		
			// 这样不行,好像有静态成员,所以 不行 memcpy(vPoly,pVertex+3*pIndice[i],9*sizeof(UGdouble));// 拷贝三个点到vPoly
			vPoly[0]=UGVector3d(pVertex[3*pIndice[i*3]],pVertex[3*pIndice[i*3]+1],pVertex[3*pIndice[i*3]+2]);
			vPoly[1]=UGVector3d(pVertex[3*pIndice[i*3+1]],pVertex[3*pIndice[i*3+1]+1],pVertex[3*pIndice[i*3+1]+2]);
			vPoly[2]=UGVector3d(pVertex[3*pIndice[i*3+2]],pVertex[3*pIndice[i*3+2]+1],pVertex[3*pIndice[i*3+2]+2]);

			vVector1 =  vPoly[0]-vPoly[2];
			vVector2 =	vPoly[2]-vPoly[1];
			vNormal = -vVector1.CrossProduct(vVector2);

			UGdouble e0 = (vPoly[0]-vPoly[1]).Length2();
			UGdouble e1 = (vPoly[1]-vPoly[2]).Length2();
			UGdouble e2 = (vPoly[2]-vPoly[0]).Length2();

			UGVector3d& v0Normal = mapNormals[pIndice[i*3]];
			v0Normal += vNormal/(e0*e2);

			UGVector3d& v1Normal = mapNormals[pIndice[i*3+1]];
			v1Normal += vNormal/(e0*e1);

			UGVector3d& v2Normal = mapNormals[pIndice[i*3+2]];
			v2Normal += vNormal/(e1*e2);
		}

		UGVector3d vSum =UGVector3d::ZERO ;
		UGVector3d vZero = vSum;
		int shared=0;
		UGVector3d temp;
		for (i = 0; i < numOfVerts; i++)
		{
			std::map<UGint, UGVector3d>::iterator ito = mapNormals.find(i);
			if (ito != mapNormals.end())
			{
				temp = ito->second;
				temp.Normalize();
			}
			else
			{
				temp = UGVector3d(1.0, 0.0, 0.0);
			}

			pNormals[3*i]=temp.x;
			pNormals[3*i+1]=temp.y;
			pNormals[3*i+2]=temp.z;
			vSum = vZero;
		}
		mapNormals.clear();
		return pNormals;
	}
	
	//! \brief 给一个顶点数组数组计算三角型串的向量，作废的接口，好像有问题。
	//! \param pVertex 构成三角形串顶点数组,每三个点构成一个三角形，无索引[in]。
	//! \param nVertexCount 顶点数组的大小[in]。
	//! \param pNormal 各个顶点的向量[out]。
	//! \param bQuad [in]。
	static void ComputeFaceNormals(UGfloat* pVertex,UGint nVertexCount, UGfloat* pNormal, UGbool bQuad = FALSE);	
	//! \brief 由不共线的三个点计算过这三维点的面的法向了。
	//! \param vec1 顶点1[in]。
	//! \param vec2 顶点2[in]。
	//! \param vec3 顶点3[in]。
	//! \return 面的法向量。
	static UGVector3d ComputeFaceNormals(const UGVector3d& vec1,const UGVector3d& vec2,const UGVector3d& vec3);	
	
	//! \brief 曲度细分。
	//! \param arrPts3D [in/out] 顶点数组。
	//! \param arrIds [in/out] 索引数组。
	//! \param arrTexCoords [in/out] 纹理数据，当传入的数组为空时不计算，否则要求与顶点数组一一对应。
	//! \param opType [in] 索引类型，目前仅支持OT_TRIANGLE_LIST、OT_LINE_STRIP、OT_POINT_LIST。
	//! \param dGranularity [in] 细分粒度。顶点为经纬度坐标时，参数默认为PI/180.0度；顶点为平面坐标时，参数默认为1.0米。
	//! \param bLonLat [in] 顶点坐标是否是经纬度。
	static UGbool ComputeSubDivision(
		UGArray<UGPoint3D>& arrPts3D, 
		UGArray<UGuint>& arrIds,
		UGArray<UGArray<UGPoint3D> >& arrTexCoords,
		OperationType opType = OT_TRIANGLE_LIST, 
		UGdouble dGranularity = DTOR,
		UGbool bLonLat = TRUE);

	//! \brief 计算地平线剔除点。
	//! \param vDirectionToPoint [in] 视点位置。
	//! \param vecPositions [in] 地形块边点数组。
	//! \return 剔除点。
	static UGVector3d ComputeHorizonCullingPoint(const UGVector3d& vDirectionToPoint, const std::vector<UGVector3d>& vecPositions, GlobalShape gs = GS_ELLIPSOID);

	//! \brief 计算从向量vFrom到向量vTo的旋转矩阵。
	//! \param vTo [in] 目标向量。
	//! \param vFrom [in] 起始向量。
	//! \return 旋转矩阵。
	static UGMatrix3d CalcRotMatrix(const UGVector3d& vTo, const UGVector3d& vFrom);

	//! \brief 计算由向量vec1到向量vec2的欧拉角。
	//! \param vec1 向量1[in]。
	//! \param vec2 向量2[in]。
	//! \return 向量1到向量2的欧拉角。
	static UGVector3d GetVec1RotToVec2(const UGVector3d& vec1,const UGVector3d& vec2);	
	//! \brief 计算绕某个向量旋转一定角度所生成的欧拉角。
	//! \param fAngle 旋转角度[in]。
	//! \param vecAxies 旋转轴[in]。
	//! \return 欧拉角。
	static UGVector3d GetRotFormAngleAxies(UGdouble fAngle,UGVector3d& vecAxies);	
	//! \brief 判断浮点型的正负。
	//! \param fValue 传入的浮点型值[in]。
	//! \return 正数返回1，负数返回-1，0返回0.0。
	static UGdouble Sign (UGdouble fValue);	

	//! \brief 点斜式直线与矩形框的交点
	//! \param pnt1 二维点[in]。
	//! \param pnt1 直线斜率[in]。
	//! \param pnt1 矩形框[in]。
	//! \return 交点。
	static UGPoint2Ds IntersectionOfRectByPointslopeline(UGPoint2D& point, UGdouble dSlope, UGRect2D& rect);

	static UGdouble Compute2DAngleFromY(UGPoint2D pnt1);	
	//! \brief 判断是否是合法实数。
	//! \param dValue 实数[in]。
	//! \return true为合法，false为非法。
	static UGbool IsRealNaN(UGdouble dValue);
	//! \brief 计算顶点的包围盒。
	//! \param pVertex 顶点数组[in]。
	//! \param nVertexCount 顶点数目[in]。
	//! \param boundingBox 包围盒[out]。
	static void  ComputeBoundingBox(UGfloat* pVertex,UGint nVertexCount,UGBoundingBox& boundingBox);	
	//! \brief 对面进行三角化。
	//! \param pPoint 构成面的顶点数组[in]。
	//! \param nPoint 顶点数目[in]。
	//! \param nTri 三角形的数目[out]。
	//! \return 三角形的顶点数组。
	static void PartTriangulation(const UGArray<UGPoint3D>& arrPoint, UGArray<UGushort>& arrTriIndex, UGint& nTri);	
	//! \brief 射线与三角形相交。
	//! \param &rayOrg 射线起点[in]。
	//! \param rayDelta 射线方向（单位化）[in]。
	//! \param p0 三角形第一个点[in]。
	//! \param p1 三角形第二个点[in]。
	//! \param p2 三角形第三个点[in]。
	//! \param dTolerance 求交容限[in]。
	//! \return 返回相交参数t。
	//! \remarks rayIntersect = rayOrg + rayDelta * t。
	static UGbool RayTriangleIntersect(const UGVector3d &rayOrg, const UGVector3d& rayDelta, 
									   const UGVector3d& p0, const UGVector3d& p1, const UGVector3d& p2, 
									   UGdouble& t, UGdouble dTolerance = 0.0);

	//! \brief 射线与三角形相交。增强版，返回两个具体的空间位置状况
	//! \param &rayOrg 射线起点[in]。
	//! \param rayDelta 射线方向（单位化）[in]。
	//! \param p0 三角形第一个点[in]。
	//! \param p1 三角形第二个点[in]。
	//! \param p2 三角形第三个点[in]。
	//! \return 返回相交参数t。
	//! \return 如果不相交，返回是否是因为平行
	//! \return 如果相交，返回摄线方向与面法线方向是否同向
	//! \remarks rayIntersect = rayOrg + rayDelta * t。
	static UGbool RayTriangleIntersect(UGVector3d &rayOrg, UGVector3d& rayDelta, 
		UGVector3d& p0, UGVector3d& p1, UGVector3d& p2, 
		UGdouble& t, UGbool& bParallel, UGbool& bSameDirection);

	//! \brief  球坐标空间中的一段距离转到经纬度跨度。
	//! \param CartesianDist [in]。
	//! \param SphericalDist [in]。
	//! \return 。
	//! \remarks  半径是用地球半径。
	//! \attention 。
	static void CartesianDist2SphericalDist(const UGdouble& CartesianDist, UGdouble& SphericalDist);
	//! \brief 顶点数组与矩阵相乘。
	//! \param pDest 目的顶点数组[in]。
	//! \param pSrc 源顶点数组[in]。
	//! \param nCount 顶点个数[in]。
	//! \param matView 矩阵[in]。
	//! \return 。
	//! \remarks 。
	//! \attention 。
	static void VerticesMulitplyViewMatrix(UGfloat* pDest, UGfloat* pSrc,
										   UGint nCount, UGMatrix4d& matView);
	//! \brief 求两个向量的夹角。
	//! \param v1 向量1[in]。
	//! \param v2 向量2[in]。
	//! \return 两个向量夹角。
	static UGdouble AngleOfV1V2(UGVector3d& v1, UGVector3d& v2);	
	
	//static UGVector3d VerticalVectorOfV3(UGVector3d& v1, UGVector3d& v2, UGVector3d& v3);
	//! \brief  返回比a大的，并且是最接近a的2的次方的数。
	static UGuint NextP2 (UGuint a);

	//! \brief  计算2的多少次方刚好大于或等于a。
	static UGuint NextLog2(UGuint a);

	//! \brief 得到缓存的行列。
	//! \param nLevel 文件所在的层[in]。
	//! \param nRow 文件所在的行[in]。
	//! \param nCol 文件所在的列[in]。
	//! \param nCacheRow 文件在缓存目录的行[out]。
	//! \param nCacheCol 文件在缓存目录的列[out]。
	static void GetCacheRowCol(UGint nLevel, UGint nRow, UGint nCol, UGint& nCacheRow, UGint& nCacheCol);

	//! \brief 得到文件的缓存的相对路径。
	//! \param nLevel 文件所在的层[in]。
	//! \param nRow 文件所在的行[in]。
	//! \param nCol 文件所在的列[in]。
	//! \param nVersion 文件版本号[in]。
	//! \return 文件的相对路径（没有图片的扩展名）。
	static UGString GetCacheLocalPath(UGint nLevel, UGint nRow, UGint nCol, UGint nVersion);

	//! \brief 判断顶点是否逆时针走向。
	//! \param pPoint 顶点数组[in]。
	//! \param nPoint 顶点数组大小[in]。
	//! \return 是否逆时针走向。
	static UGint IsCounterClockwise(const UGPoint3D* pPoint, UGint nPoint);	
	//! \brief 线段与球相交。
	//! \param vecSphereCenter 球心[in]。
	//! \param dSphereRadius 球半径[in]。
	//! \param vecStart 线段起始点[in]。
	//! \param vecEnd 线段结束点[in]。
	//! \param t1 交点1[in]。
	//! \param t2 交点2[in]。
	//! \return 交点个数。
	//! \remarks 交点范围为0-1范围内有效。
	//! \attention 。
	static UGint LineSphereIntersection(UGVector3d vecSphereCenter,
							UGdouble dSphereRadius,
							UGVector3d vecStart,
							UGVector3d vecEnd, 
							UGfloat& t1, UGfloat& t2);

	//! \brief 射线与球相交。
	//! \param vecStart 射线起点[in]。
	//! \param vecDirection 射线方向in]。
	//! \param dSphereRadius 球半径[in]。
	//! \param vecIntersect 交点[in]。
	//! \return 二者相交返回TRUE, 否则返回FALSE。
	//! \remarks 球心在原点, 默认半径为地球半径。
	//! \attention 二者相交时vecIntersect返回交点位置，否则vecIntersect的值不改变
	static UGbool RayIntersectionWithSphere(const UGVector3d& vecStart, 
							const UGVector3d& vecDirection, 
							UGVector3d& vecIntersect,
							UGdouble dSphereRadius = GLOBAL_RADIUS);

	//! \brief 射线与球相交。
	//! \param vecStart 射线起点[in]。
	//! \param vecDirection 射线方向in]。
	//! \param dSphereRadius 球半径[in]。
	//! \param bDiscardInside 是否忽略顶点位于球内部的射线
	//! \return 二者相交返回TRUE和射线起点至交点的距离, 否则返回FALSE。
	//! \remarks 球心在原点, 默认半径为地球半径。
	//! \attention 返回距离为负表明交点在射线反方向。
	static std::pair<UGbool, UGdouble> RayIntersectionWithSphere(const UGVector3d& vecStart, 
		const UGVector3d& vecDirection,
		UGdouble dSphereRadius = GLOBAL_RADIUS,
		UGbool bDiscardInside = TRUE);

	
	//! \brief 计算rectRes是否和rectObj相交。
	//! \param rectRes 一般是较大的矩形框[in]。
	//! \param rectObj 被测试的矩形框[in]。
	//! \return 范围是否相交。
	//! \remarks 考虑是否变更线。
	//! \attention 。
	static UGbool IsIntersect3D(const UGRect2D& rectRes,const UGRect2D& rectObj);

	//! \brief 计算rectRes是否和pnt相交。
	//! \param rectRes [in]。
	//! \param pnt [in]。
	//! \return 范围是否相交。
	//! \remarks 考虑是否变更线。
	//! \attention 。
	static UGbool IsIntersect3D(const UGRect2D& rectRes, const UGPoint2D& pnt);

	//! \brief 判断两个球是否相交
	static UGbool IsIntersect3D(const UGBoundingSphere& bbSphere1,\
		const UGBoundingSphere& bbSphere2);

	//! \brief 由于存在计算误差，调整特殊角度的三角函数值
	//! \param dAngle [in]角度
	//! \param dCosVal[out]角度对应的余弦
	//! \param dSinVal[out]角度对应的正弦
	//! \remarks 。
	static void AdjSpecialTriFunValue(const UGdouble dAngle,UGdouble &dCosVal,UGdouble &dSinVal);
	
	//! \brief 计算得到新的插入点，将点对象向眼睛（相机）拉近一定的比例。
	//! \param ViewMatrix 视图矩阵[in]。
	//! \param vecInsertPos 插入的原始点[in]。
	//! \param nScale 眼睛到目标点的距离，新的目标点到原始目标点的距离，两者的比值[in]。
	//! \return 新的目标点。
	//! \remarks 。
	//! \attention 主要是用在点对象，文本对象，为了防止地标文字在左半球时陷到地里问题。
	static UGVector3d GetCloserInsertPnt(const UGMatrix4d& ViewMatrix, const UGVector3d& vecInsertPos, UGint nScale);

	//! \brief 射线与球面交点。
	//! \param vecStart 起始坐标[in]。
	//! \param vecEnd 终点坐标[in]。
	//! \param dRadius 球的半径[in]
	//! \param vecIntersect 交点[out]。
	//! \return 。
	//! \remarks 。
	//! \attention 球心坐标为(0,0,0)。
	static UGbool RayIntersectionWithSphere(UGVector3d& vecStart, UGVector3d& vecEnd,UGdouble dRadius, UGVector3d& vecIntersect);	
	
	//! \brief 图像数据采样，一般是从大到小的采样
	//! \param pSRC 源数据 外面分配空间
	//! \param pDEST 目标数据 外面分配空间
	//! \param nSrcWidth 源数据宽
	//! \param nSrcHeight 源数据高
	//! \param nDestWidth 目标数据宽
	//! \param nDestHeight 目标数据高
	static void  ScaleImage(UGuchar* pSRC, UGuchar* pDEST, UGint nSrcWidth, UGint nSrcHeight, 
		                    UGint nDestWidth, UGint nDestHeight, UGImageSamplerType nType = IMAGE3D_RESAMPLER_BILINEAR);

	//! \brief 对单个纹理进行缩放，建立多级纹理
	//! \param srcImageData 源数据
	//! \param desImageData 目标数据 
	//! \param nNewWidth 目标数据宽
	//! \param nNewHeight 目标数据高
	static void ScaleImage(const UGImageData& srcImageData,UGImageData& desImageData,UGint nNewWidth,UGint nNewHeight);


	//! \brief 由纬度和面片的尺寸求面片的行数。
	//! \param dLatitude 维度，单位弧度[in]。
	//! \param dTileSize 面片尺寸，三维弧度[in]。
	//! \return 面片所处的行数。
	static UGint GetRowFromLatitude(UGdouble dLatitude, UGdouble dTileSize, UGbool bStartLT = FALSE, TilingScheme eScheme = GeographicTilingScheme);

	//! \brief 由经度和面片的尺寸求面片的行数。
	//! \param dLongitude 经度，单位弧度[in]。
	//! \param dTileSize 面片尺寸，三维弧度[in]。
	//! \return 面片所处的列数。
	static UGint GetColFromLongitude(UGdouble dLongitude, UGdouble dTileSize, TilingScheme eScheme = GeographicTilingScheme);


	//! \brief 由纵坐标和面片的尺寸求面片的行数。
	//! \param dCoord 纵坐标，单位米[in]。
	//! \param dTileSize 面片尺寸，米[in]。
	//! \param dL0TileSize 0层的面片尺寸，米[in]。
	//! \return 面片所处的行数。
	static UGint GetRowFromCoord(UGdouble dCoord, UGdouble dTileSize, UGdouble dL0TileSize, UGbool bStartLT = FALSE, TilingScheme eScheme = GeographicTilingScheme);

	//! \brief 由横坐标和面片的尺寸求面片的列数。
	//! \param dCoord 横坐标，单位米[in]。
	//! \param dTileSize 面片尺寸，米[in]。
	//! \param dL0TileSize 0层的面片尺寸，米[in]。
	//! \return 面片所处的横数。
	static UGint GetColFromCoord(UGdouble dCoord, UGdouble dTileSize, UGdouble dL0TileSize, TilingScheme eScheme = GeographicTilingScheme);

 
	//! \brief 计算以左上角开始的Tile的经纬度Bounds.
	//! \param nRow Tile的行[in].
	//! \param nCol Tile的列[in].
	//! \param nLevel Tile的层[in].
	//! \param dL0TileSizeDeg 零层的经纬度跨度，默认为180[in].
	//! \param UGRect2D 返回经纬度Bounds[out].
	static UGRect2D CalcTileBounds(UGint nRow, UGint nCol, UGint nLevel, UGdouble dL0TileSizeDeg = 180.0, TilingScheme eScheme = GeographicTilingScheme);

	//! \brief 计算以左下角开始的Tile的经纬度Bounds.
	//! \param nRow Tile的行[in].
	//! \param nCol Tile的列[in].
	//! \param nLevel Tile的层[in].
	//! \param dL0TileSizeDeg 零层的经纬度跨度，默认为180[in].
	//! \param UGRect2D 返回经纬度Bounds[out].
	static UGRect2D CalcLeftBottomTileBounds(UGint nRow, UGint nCol, UGint nLevel, UGdouble dL0TileSizeDeg = 180.0, TilingScheme eScheme = GeographicTilingScheme);

	//! \brief　调整缓存Bounds的范围
	//! \param rcBounds 需要调整的范围[in].
	//! \param nLevel 调整的参考层[in].
	static UGRect2D CeilBounds(const UGRect2D& rcBounds, UGint nLevel, UGdouble dL0TileSizeDeg = 180.0, TilingScheme eScheme = GeographicTilingScheme);


	//! \brief 由经度计算墨卡托坐标值。
	static UGdouble CalcMercatorFromLongitude(UGdouble dLongitude);

	//! \brief 由维度计算墨卡托坐标值。
	static UGdouble CalcMercatorFromLatitude(UGdouble dLatitude);
	
	//! \brief 由墨卡托坐标值计算经度。
	static UGdouble CalcLongitudeFromMercator(UGdouble dMercator);

	//! \brief 由墨卡托坐标值计算维度。
	static UGdouble CalcLatitudeFromMercator(UGdouble dMercator);


	//! \brief 通过线性插值和箱过滤缩放。from gluScaleImage
	//! \param components 颜色成分数量，RGBA是4
	//! \param widthin 输入图像的宽度
	//! \param heightin 输入图像的高度
	//! \param datain 输入图像的指针
	//! \param widthout 输出图像的宽度
	//! \param heightout 输出图像的高度
	//! \param dataout 输出图像的指针
	//! \param nsizedatain datain的大小
	//! \param nsizedatain 临时补救措施：通过UGFileParseToolkit::GetImageData获取的内存流是按扫描行四字节对齐的,此时datain的实际大小大于components*w*h，需要调用ClipImageData进行紧缩
	static void ScaleImageInternal(UGint components, UGint widthin, UGint heightin,
		const UGuchar *datain,
		UGint widthout, UGint heightout,
		UGuchar* dataout, const UGint nsizedatain = -1);

	//! \brief 处理三维纹理，当然也可以处理二维纹理。
	//! \param components 颜色成分数量，RGBA是4
	//! \param widthin 输入图像的宽度
	//! \param heightin 输入图像的高度
	//! \param depthin 输入图像的深度
	//! \param datain 输入图像的指针
	//! \param widthout 输出图像的宽度
	//! \param heightout 输出图像的高度
	//! \param depthout 输出图像的深度
	//! \param dataout 输出图像的指针
	static void ScaleImageInternal(UGint components,
		UGint widthin, UGint heightin, UGint depthin, const UGuchar *datain,
		UGint widthout, UGint heightout, UGint depthout, UGuchar* dataout);

	//! \brief 处理三维浮点纹理，当然也可以处理二维纹理。（参考ogre）
	//! \param components 颜色成分数量，RGBA是4
	//! \param widthin 输入图像的宽度
	//! \param heightin 输入图像的高度
	//! \param depthin 输入图像的深度
	//! \param datain 输入图像的指针
	//! \param widthout 输出图像的宽度
	//! \param heightout 输出图像的高度
	//! \param depthout 输出图像的深度
	//! \param dataout 输出图像的指针
	static void ScaleImageInternal(UGint components,
		UGint widthin, UGint heightin, UGint depthin, const UGfloat *datain,
		UGint widthout, UGint heightout, UGint depthout, UGfloat* dataout);

	//! \brief 把图像数据生成Mipmap图像数据
	//! \param components [in] 原始图像位深，目前仅支持32位
	//! \param widthin	  [in] 原始图像宽度
	//! \param heightin   [in] 原始图像高度
	//! \param datain     [in] 原始图像数据
	//! \param widthout   [in/out] 生成图像的宽度
	//! \param heightout  [in/out] 生成图像的高度
	//! \param arrTextureSize [in/out] 原始MipMap图像数据每层的偏移
	//! \return MipMap生成结果
	static UGuchar* GenerateMipMap(UGint components, UGuint& widthin, UGuint& heightin,
		UGuchar *datain, UGuint& widthout, UGuint& heightout,UGArray<UGuint>& arrTextureOffset);

	//! \brief 把图像数据生成Mipmap图像数据
	//! \param pImageData [in] 原始图像数据
	//! \param pTotalImageData [out] 生成后的图像数据 
	static UGuchar* GenerateMipMap(UGint components, UGint& widthin, UGint& heightin,
		UGuchar *datain, UGint& widthout, UGint& heightout);

	//! \brief 通过一整张Mipmap图像数据得到Mipmap图像数组
	//! \param pMipMapImageData [in] 一整张Mipmap图像数据
	//! \param arrMipMapImageData [out] Mipmap图像数组 
	static UGuchar* ParseMipMap(UGint components, UGint widthin, UGint heightin,
		const UGuchar *datain,
		UGArray<UGuint>& arrTextureSize);

	//! \brief 计算8个顶点的BoundingBox表面到摄像机（坐标原点）的距离 by wangxinhe 2010.12.14
	//! \param vecBoundBox BoundBox的8个顶点
	//! \param BoundingBox 所在的地理坐标（经纬度，地面高度）
	//! \param viewMatrix 当前的视图矩阵
	//! \param BoundingBox 绕Z轴的旋转角度
	//! \param altitude 当前摄像机的高度
	//! \param tilt 摄像机的Tilt角度
	//! \return 得到的距离
	static double GetBoundingBoxDistanceToCamera(const UGVector3d *vecBoundBox,	const UGPoint3D &pntBoundBox,
												const UGMatrix4d &viewMatrix,UGdouble rotation,UGdouble altitude,
												UGdouble tilt);

	//! \brief 计算直线与平面的交点
	//! \param planeVector [IN] 平面法向量
	//! \param planePoint [IN] 平面上一点
	//! \param lineVector [IN] 直线的向量
	//! \param linePoint [IN] 直线上一点
	//! \return 返回交点
	static UGbool GetPlaneLineIntersectPoint(const UGVector3d &planeVector, const UGVector3d &planePoint,
		const UGVector3d &lineVector, const UGVector3d &linePoint,UGVector3d& vecIntersect);

	//! \brief 双线性采样
	//! \param pSRC [IN] 需要进行采样的原始点
	//! \param pDEST [IN] 期望的采样结果
	//! \param nOldWidth [IN] 原始数据的宽度
	//! \param nOldHeight [IN] 原始数据的高度
	//! \param nWidth [IN] 期望采样结果数据的宽度
	//! \param nHeight [IN] 期望采样结果数据的高度
	static void  TerrainSamplerBiLinear(const UGfloat* pSRC, UGfloat* pDEST, UGint nOldWidth, UGint nOldHeight, 
		UGint nWidth, UGint nHeight);

	//! \brief 将图像数据内容进行对齐处理，暂时只对24位图像处理
	//! \param components 颜色成分数量，RGBA是4
	//! \param width 输入图像的宽度
	//! \param height 输入图像的高度
	//! \param datain 输入图像的指针
	//! \param dataout 输出图像的指针
	static UGbool ClipImageData(UGint components, UGint width, UGint height, 
		const UGuchar *datain, const UGint ndatainsize, UGuchar *dataout);

	//! \brief 三角与平面求交
	//! \param planeNormalVector 平面法向量
	//!	\param PlanePoint 平面上的一个点
	//! \param triangleP1 三角形顶点1
	//! \param triangleP2 三角形顶点2
	//! \param triangleP3 三角形顶点3
	//! \return 三角边线与平面的交点(无相交或共面返回0个点，交在单个顶点返回1个点）
	static UGArray<UGVector3d> GetPlaneTriangleIntersectLine(UGVector3d& planeNormalVector, UGVector3d& planePoint, 
		UGVector3d& triangleP1, UGVector3d& triangleP2, UGVector3d& triangleP3);

	//! \brief 求三维点与平面的方向距离
	//! \param planeNormalVector 平面法向量
	//!	\param PlanePoint 平面上的一个点
	//! \param point 指定的待计算点
	//! \return 三维点与平面的方向距离，绝对值是距离，正数表示与法向量在面的同一侧
	static UGdouble GetPointDirectionDistanceToPlane(const UGVector3d& planeNormalVector, const UGVector3d& PlanePoint,
		const UGVector3d& point);

	//! \brief 平面与平面求交线
	//! \param planeVector1 平面1的方向向量[in]
	//! \param planePoint1	平面1中点坐标[in]
	//! \param planeVector2 平面2的方向向量[in]
	//! \param planePoint2	平面2中点坐标[in]
	//! \param lineVector		交线的方向向量[out]
	//! \param linePoint			交线上的点坐标[out]
	//! \return 获得交线返回TRUE，否则返回FALSE
	static UGbool GetPlanePlaneIntersectLine(UGVector3d &planeVector1,UGVector3d &planePoint1,
		UGVector3d &planeVector2,UGVector3d &planePoint2,UGVector3d& lineVector, UGVector3d& linePoint);

	//! \brief 三角形与三角形求交线
	//! \param triangleA1，triangleA2，triangleA3	三角形A的三个端点坐标[in]
	//! \param triangleB1，triangleB2，triangleB3	三角形B的三个端点坐标[in]
	//! \return 返回交线的两个端点坐标
	static UGArray<UGVector3d> GetTriangleTriangleIntersectPoints(UGVector3d &triangleA1, UGVector3d &triangleA2,
		UGVector3d &triangleA3, UGVector3d &triangleB1, UGVector3d &triangleB2, UGVector3d &triangleB3);

	//! \brief 判断点是否在Mesh内部
	//! \param point	待判断的点[in]
	//! \param mesh	用来判断的mesh[in]
	//! \return 在mesh内部返回TRUE，否则为FALSE
	static UGbool IsPointInMesh(UGVector3d &point, UGMesh &mesh);

	//! \brief 判断点是否在Mesh内部（支持凹的）不考虑坐标系的，即认为点和Mesh内顶点的坐标系一致
	//! \param point	待判断的点[in]
	//! \param mesh	用来判断的mesh[in]
	//! \return 在mesh内部返回1，外部返回-1，在Mesh表面返回0
	static UGint IsPointInMeshByLine(UGVector3d &point, UGMesh &mesh);

	//! \brief 判断三角形是否和BOX相交
	//! \param v0 v1 v2			三角形三个顶点[in]
	//! \param pMax pMin	BOX中最大值和最小值[in]
	//! \return 相交返回TRUE，否则为FALSE
	static UGbool IsTriangleBoxIntersect(UGVector3d &v0, UGVector3d &v1, UGVector3d &v2,
		UGVector3d &pMax, UGVector3d &pMin);

	//! \brief 判断平面是否和BOX相交
	//! \param planeVector	平面法向量[in]
	//! \param planePoint		平面内一点[in]
	//! \param pMax pMin	BOX中最大值和最小值[in]
	//! \return 相交0，在面的前面1，在面的后面-1
	static UGint IsPlaneBoxIntersect(UGVector3d &planeVector,UGVector3d &planePoint,
		UGVector3d &pMax, UGVector3d &pMin);

	//! \brief 将double值编码为两个float值
	//! \param dValue	待转化的double值[in]
	//! \param fHigh		存储double值的高位[in]
	//! \param fLow		存储double值的低位[in]
	static void DoubleToTwoFloats(UGdouble dValue, UGfloat &fHigh, UGfloat &fLow);

	//! \brief 点在平面上的投影点
	//! \param vector		平面法向量[in]
	//! \param point0	平面内一点[in]
	//! \param point1	平面外一点[in]
	//! \return 平面内的投影点
	static UGVector3d PointProjectPlane(UGVector3d &vector, const UGVector3d &point0,
		const UGVector3d &point1);

	//! \brief 点是否在三角形内部
	//! \param p0 p1 p2	三角形三个顶点[in]
	//! \param point			测试点[in]
	//! \return 在三角形内返回TRUE，否则为FALSE
	static UGbool IsPointInTriangle(UGVector3d &p0,UGVector3d &p1,UGVector3d &p2,
		UGVector3d &point);

	//! \brief 点到直线的投影点
	//! \param pntLine		直线上一点[in]
	//! \param vectorLine	直线的向量[in]
	//! \param point			测试点[in]
	//! \return 返回点在直线上的投影点
	static UGVector3d PointProjectLine(UGVector3d &pntLine,UGVector3d &vectorLine, UGVector3d &point);

	//! \brief 三点是否共线
	//! \param pntLine		直线上一点[in]
	//! \param p0 p1 p2	三角形三个顶点[in]
	//! \return 共线返回true
	static UGbool IsPointOnSameLine(UGVector3d &p0,UGVector3d &p1, UGVector3d &p2);

	//! \brief 点到三角面的距离
	//! \param vec	外部点[in]
	//! \param p1 p2 p3	三角面的顶点[in]
	//! \return 返回点到三角面的距离
	static UGdouble DistancePointToTriangle(UGVector3d &vec, UGVector3d &p1, UGVector3d &p2, UGVector3d &p3);

	//! \brief 两点弧段到三角面的距离
	//! \param vec1 vec2	弧段两个端点[in]
	//! \param p1 p2 p3	三角面的顶点[in]
	//! \return 返回两点弧段到三角面的距离
	static UGdouble DistanceSegmentToTriangle(UGVector3d &vec1,UGVector3d &vec2, UGVector3d &p1, UGVector3d &p2, UGVector3d &p3);

	//! \brief 直线与直线间的最短距离
	//! \param pntLine1			直线1上一点[in]
	//! \param vectorLine1	直线1的向量[in]
	//! \param pntLine2			直线2上一点[in]
	//! \param vectorLine2	直线2的向量[in]
	//! \return  直线与直线间的最短距离
	static UGdouble DistanceLineToLine(UGVector3d &pntLine1,UGVector3d &vectorLine1, UGVector3d &pntLine2,UGVector3d &vectorLine2);

	//! \brief 向量与向量的夹角(可以用来计算直线与直线的夹角)
	//! \param vector1	向量1[in]
	//! \param vector2	向量2[in]
	//! \return  向量与向量的夹角(返回值为角度)
	static UGdouble AngleVectorToVector(UGVector3d &vector1, UGVector3d &vector2);

	//! \brief 改变mesh内三角形的法向量，使其始终想外
	//! \param mesh	需要调整的mesh[in/out]
	static UGbool ChangeTriangleInMesh(UGMesh &mesh);

	//! \brief 判断两个BOX是否相交
	//! \param vMin1	 vMax1		第一个BOX的最大最小值[in]
	//! \param vMin2	 vMax2		第二个BOX的最大最小值[in]
	//! \return  相交返回true，否则为false
	static UGbool IsBoxIntersect(UGVector3d &vMin1, UGVector3d &vMax1, UGVector3d &vMin2, UGVector3d &vMax2);

	//! \brief 计算三角形到平面的方向距离（相交的距离为0）
	//! \param vector1	 vector2	vector3	三角形的三个顶点[in]
	//! \param pntPlane	 vectorPlane				平面的点和向量[in]
	//! \param nSide										三角形在平面的哪侧[out]
	//! \return  返回三角形到平面的距离(最近的顶点到平面的方向距离)
	static UGdouble GetTriangleDirectionDistanceToPlane(UGVector3d &vector1, UGVector3d &vector2, UGVector3d &vector3,
		UGVector3d &pntPlane,UGVector3d &vectorPlane, UGint &nSide);


	//! \brief 将一个数值限制到一定范围内. 
	//! \param val	初始值[in]
	//! \param minval		最小值[in]
	//! \param maxval		最大值[in]
	template <typename T>
	static T Clamp(T val, T minval, T maxval)
	{
		UGASSERT (minval < maxval);
		return UGMAX(UGMIN(val, maxval), minval);
	}

	//! \brief 取整数。
	//! \param dVal [in] 浮点数。
	//! \param dTol [in] 容差。
	static UGdouble Round(UGdouble dValue, UGdouble dTolerace = EP);

	//! \brief 从三角形索引数组中提取闭合环路
	//! \param ids [in] 三角形索引数组
	//! \param loops [out] 提取出的闭合环路
	//! \return 操作是否执行成功
	static UGbool ExtractLoops(const std::vector<UGuint>& ids, std::vector<std::vector<UGuint> >& loops);

	//! \brief 从边端点对应表中提取闭合回路
	//! \brief edges [in] 有向边数组
	//! \param loops [out] 提取出的闭合环路
	//! \return 操作是否执行成功
	static UGbool ExtractLoops(const std::vector<std::pair<UGuint, UGuint> >& edges, std::vector<std::vector<UGuint> >& loops);

	//! \brief 根据相机的位置，与方位角，得到视图矩阵
	//! \param vecPosition [in] 相机的位置
	//! \param orientation [in] 表示相机方向的四元数
	static UGMatrix4d MakeViewMatrix(const UGVector3d& vecPosition, const UGQuaternion4d& orientation);

	//!\brief 射线拾取 gluUnproject的实现
	//! \param logicalCoords[in] 逻辑坐标，即窗口坐标
	//! \param modelViewProMatrix[in] 模型视图投影矩阵
	//! \param viewport[in] 视口
	//! \praram vPoint[out] 世界空间中的坐标
	//! \return 成功返回TRUE，否则返回FALSE
	static UGbool UnProject(const UGVector3d& logicalCoords, const UGMatrix4d& modelViewProMatrix, const UGint viewport[4],	
		UGVector3d& vPoint);

	//!\brief 把空间坐标转化为窗口坐标
	//! \param worldCoords[in] 世界空间中的坐标
	//! \param modelViewProMatrix[in] 模型视图投影矩阵
	//! \param viewport[in] 视口
	//! \praram winCoords[out]窗口坐标
	//! \return 成功返回TRUE，否则返回FALSE
	static UGbool Project(const UGVector3d& worldCoords, const UGMatrix4d& modelViewProMatrix, const UGint viewport[4],
		UGVector3d& winCoords);

	//! \brief 将视图矩阵解析为 orientation / scale / position
	static void DecomposeViewMatrix(const UGMatrix4d& viewMatrix, UGVector3d& position, UGVector3d &scale, UGQuaternion4d& orientation);

	//! \brief 将世界矩阵分解为 插入点vPosition / 局部矩阵(mScaleMatrix * mRotateMatrix)
	static void DecomposeWorldMatrix(const UGMatrix4d& mWorldMatrix, UGVector3d& vPosition, UGMatrix4d& mLocalMatrix);

	//! \brief 将局部矩阵(mScaleMatrix * mRotateMatrix) / 插入点vPosition 组成世界矩阵
	static void ComposeWorldMatrix(UGMatrix4d& mWorldMatrix, const UGVector3d& vPosition, const UGMatrix4d& mLocalMatrix);

	static UGdouble GetTriangleArea(const UGVector3d v1, const UGVector3d v2, const UGVector3d v3);
	static UGdouble GetTriangulateVolume(const UGVector3d p1, const UGVector3d p2, const UGVector3d p3,
		UGdouble dBaseValue = 0);

	//! \brief 根据插入点及编移量计算新的插入点
	//! \param dLongitude 插入点的经度,单位是度[in]。
	//! \param dLatitude 　插入点的纬度，单位是度[in]。
	//! \param dAltitude 插入点的高度[in]。
	//! \param vecOffset 偏移量[in]。
	//! \return 新的经纬度坐标,单位是度
	static UGPoint3D CartesianToSphericalD(UGdouble dLongitude,UGdouble dLatitude,UGdouble dAltitude,UGVector3d vecOffset);

	//! \brief 按照传入参数计算透视投影矩阵。
	//! \param dAspect 宽高比[in]。
	//! \param dFov 角度[in]。
	//! \param dNearDist 近裁剪面[in]。
	//! \param dFarDist 远裁剪面[in]。
	//! \return 透视投影矩阵
	//! \remarks 此矩阵以D3D为准是左手系矩阵, 为行主序
	static UGMatrix4d ComputeProMatrix(UGdouble dAspect,	UGdouble dFov, UGdouble dNearDist, UGdouble dFarDist);

	//! \brief 按照传入参数计算正视投影矩阵。
	//! \param dLeft 左坐标[in]。
	//! \param dRight 右坐标[in]。
	//! \param dBottom 下坐标[in]。
	//! \param dTop 上坐标[in]。
	//! \param dNear近裁剪面[in]
	//! \prasm dFar 远裁剪面[in]
	//! \return 正视投影矩阵
	//! \remarks 此矩阵以D3D为准是左手系矩阵, 为行主序
	static UGMatrix4d ComputeOrthoMatrix(UGdouble dLeft, UGdouble dRight, UGdouble dBottom, UGdouble dTop, 
		UGdouble dNear, UGdouble dFar);

		/** 获取地图比例尺转化为字符串输出时的格式
		比例尺比较小，小数点后"0"比较多，格式化输出固定的位置会导致小比例尺损失精度。
		此函数计划小数点后的"0" 自动计算double应该输出多少位
		@param dScale比例尺
		*/
	static UGString GetMapScaleToStrFormat(UGdouble dScale);

	//! \brief 对一个多边形的进行三角形细化，会在内部插入点，输出插入后的所有点以及索引
	//! \param pPoint 点的数组，点的排列顺序为：外边界的点-》第一个内边界的点-》第二个内边界的点...[in]。
	//! \param 外边界点是逆时针排序，内边界点是顺时针排序。
	//! \param nPointCount 点的总数[in]。
	//! \param nSubCount 子多边形个数[in]。
	//! \param polyCounts 子多边形顶点个数的数组[in]。
	//! \param dMetersPerPoint 每隔多少米插入一个点[in]。
	//! \param arrAllPoint3D 输出所有点[out]。
	//! \param arrAllIntIndex 输出所有索引[out]。
	//! \param nOuterBorderPointCount 输出剖分后外部边界点的个数[out]。
	//! \param nInnerBorderPointCount 输出内部边界点个数组成的数组[out]。
	static UGbool TesselationPolygon(const UGPoint3D* pPoint, UGint nPointCount, UGint nSubCount, const UGint* polyCounts, UGdouble dMetersPerPoint, UGArray<UGPoint3D>& arrAllPoint3D, UGArray<UGuint>& arrAllIntIndex, UGint& nOuterBorderPointCount, UGArray<UGint>& arrInnerBorderPointCount);

	//! \brief 根据三个点为拐点进行平滑插值
	//! \param pt3D1 拐点的前一个点[in]
	//! \param pt3D2 拐点[in]
	//! \param pt3D3 拐点的后一个点[in]
	//! \param dLineWidth 转弯处的线宽[in]
	//! \param nInsertPointNum 插入点的个数[in]
	//! \param pt3DArray 插值后的点[out]
	//! \param arrPipeVT 插值后点的类型（位于拐弯什么位置）[out]
	//! \param dMinJointLength 线段上的点与转角的最小距离[out]
	//! \return 返回插值是否成功
	static UGbool CalculateCurveCoordinate(const UGVector3d &pt3D1, const UGVector3d &pt3D2,const UGVector3d &pt3D3, UGdouble dLineWidth, UGint nInsertPointNum,
		UGArray<UGVector3d>&pt3DArray, UGArray<PipeVertexType>& arrPipeVT, UGdouble &dMinLengthToCorner);

	//! \brief 计算实例化线型的截面法线方向
	//! \param vCurrentPos 当前顶点的位置[in]
	//! \param vPrevPos 当上一个顶点的位置[in]
	//! \param vNextPos 当下一个顶点的位置[in]
	//! \param vertexType 当前顶点的类型[in]
	//! \param bBegin 当前顶点是否是线段的起点[in]
	//! \return 法线方向
	static UGVector3d CalInstancePipeSectionNormal(const UGVector3d& vCurrentPos, const UGVector3d& vPrevPos, const UGVector3d& vNextPos, PipeVertexType vertexType, UGbool bBegin);

	//! \brief 计算实例化线型的截面旋转矩阵
	//! \param vSectionNormal 截面的法线方向[in]
	//! \param vUpDir 截面的YZ平面上的一个任意方向向量，与vSectionNormal不能重合，用来计算旋转矩阵的[in]
	//! \param vLocalYAxis 输出局部坐标Y轴在世界坐标中的方向[out]
	//! \param vLocalZAxis 输出局部坐标Z轴在世界坐标中的方向[out]
	//! \return 返回旋转矩阵
	static UGMatrix4d CalInstancePipeSectionRotMatrix(const UGVector3d& vSectionNormal, const UGVector3d& vUpDir, UGVector3d& vLocalYAxis, UGVector3d& vLocalZAxis);

	//! \brief 计算某一时间太阳位置。
	static UGVector3d GetSunPosition(const UGTime& dateTime, const UGTimeZoneInfo& timeZone, UGdouble sunRadius);

	//!\brief 获取本机时区
	//!\return 返回本机所采用的时区
	static UGTimeZoneInfo GetLocalTimeZone();
private:
	//! \brief 二维点的叉乘。
	//! \param U 点1[in]。
	//! \param V 点2[in]。
	//! \return 点1和点2的叉乘。
	static UGfloat Kross (const UGPoint2D& U, const UGPoint2D& V);	
	//! \brief 判断线段是否在圆锥之内。
	//! \param V0 线段的起始端点[in]。
	//! \param V1 线段的终止端点[in]。
	//! \param VM 线段V0-VM的终止端点[in]。
	//! \param VP 线段V0-VP的终止端点[in]。
	//! \param bClockwise 多边形是否顺时针[in]。
	//! \return 线段v0-v1是否包含与VM-V0-VP的圆锥内。
	static UGbool SegmentInCone(UGPoint2D& V0, UGPoint2D& V1, UGPoint2D& VM, UGPoint2D& VP, UGbool bClockwise);	
	//! \brief 判读多边形中的两个顶点是否是对角线。
	//! \param pPoint 多边形的顶点数组[in]。
	//! \param nPoint 顶点个数[in]。
	//! \param i0 顶点1的索引[in]。
	//! \param i2 顶点2的索引[in]。
	//! \param bClockwise 多边形是否顺时针[in]。
	//! \return 顶点1和顶点2是否构成多边形的对角线。
	static UGbool IsDiagonal(const UGPoint3D* pPoint, UGushort* pPointIndex, UGint nPoint,UGint i0, UGint i2, UGbool bClockwise);	

	//! \brief 判断三个点的缠绕情况。
	//! \param x1 顶点1的x坐标[in]。
	//! \param y1 顶点1的y坐标[in]。
	//! \param x2 顶点2的x坐标[in]。
	//! \param y2 顶点2的y坐标[in]。
	//! \param x3 顶点3的x坐标[in]。
	//! \param y3 顶点3的y坐标[in]。
	//! \return 返回1时为逆时针，－1时为顺时针，0为共线。
	static UGint counterclockwise(UGdouble x1, UGdouble y1, UGdouble x2, UGdouble y2, UGdouble x3, UGdouble y3);	
	
	//! \brief 最邻近采样
	static void  ImageSamplerNearestNeighbor(UGuchar* pSRC, UGuchar* pDEST, UGint nSrcWidth, UGint nSrcHeight, 
		                    UGint nDestWidth, UGint nDestHeight);
	//! \brief 双线性采样
	static void  ImageSamplerBiLinear(UGuchar* pSRC, UGuchar* pDEST, UGint nSrcWidth, UGint nSrcHeight, 
		                    UGint nDestWidth, UGint nDestHeight);
	
	//! \brief 立方卷积采样
	static void  ImageSamplerCubic(UGuchar* pSRC, UGuchar* pDEST, UGint nSrcWidth, UGint nSrcHeight, 
		                    UGint nDestWidth, UGint nDestHeight);

	//! \brief 通过线性插值和箱过滤缩放。from gluScaleImage
	//! \param components 颜色成分数量，RGBA是4
	//! \param widthin 输入图像的宽度
	//! \param heightin 输入图像的高度
	//! \param datain 输入图像的指针
	//! \param dataout 输出图像的指针
	static void HalveImage(UGint components, UGuint width, UGuint height,
		const UGuchar *datain, UGuchar *dataout);

	//! \brief 拓展到三维纹理
	static void HalveImage(UGint components, UGint width, UGint height, UGint depth,
		const UGuchar *datain, UGuchar *dataout);

	//! \brief 计算离散点的坐标范围
	//! \param pPoints 离散点数组
	//! \param nPointCount 离散点个数
	static UGRect2D ComputeBounds(const UGPoint3D* pPoints, UGint nPointCount);

	//! \brief 依据多边形范围大小，计算多边形的加密密度
	//! \param bounds 点的范围[in]
	//! \param dMetersPerPoint 每隔多少米插入一个点[in]
	//! \param nLongDensity 输出多边形的经向插值密度[out]
	//! \param nLatDensity 输出多边形的纬向插值密度[out]
	static void ComputeInsertPointDensity(UGRect2D bounds, UGdouble dMetersPerPoint, UGint& nLonDensity, UGint& nLatDensity);

	//! \brief 从多边形中取出内外边界的顶点，转化为Point2D放到数组中
	//! \param pGeoRegion3D 多边形[in]
	//! \param arrOuterBorderPoint2D 用来存储多边形外边界点的数组[out]
	//! \param arrInnerBorderPoint2Ds 用来存储多边形内边界点的数组[out]
	static void BuildBorderPoint3D(const UGPoint3D* pPoint, UGint nSubCount, const UGint* polyCounts, UGArray<UGPoint3D>& arrOuterBorderPoint3D, UGArray< UGArray<UGPoint3D> >& arrInnerBorderPoint3Ds);

	//! \brief 从点的数组中取出子数组的点
	//! \param pPoint 所有点的数组[in]
	//! \param subCounts 每个子组的点的个数[in]
	//! \param nSubIndex 要取的子组索引[in]
	//! \return 获取的子组的点
	static const UGPoint3D* GetSubPoints(const UGPoint3D* pPoint, const UGint* subCounts, UGint nSubIndex);

	//! \brief 在多边形的边界上，在经纬度方向上均匀的插入边界点
	//! \param nLonDensity，nLatDensity 经纬方向上插入点的个数 [in]
	//! \param dLongStep，dLatStep 多边形边上每隔多少经纬跨度插入一个点 [in]
	//! \param arrOriBorderPoint 原来的多边形边界点 [in]
	//! \param arrInsertedBorderPoint 输出插入过点多边形边界 [out]
	static void InsertPointInPolyBorder(UGint nLonDensity, UGint nLatDensity, UGdouble dLongStep, UGdouble dLatStep, const UGArray<UGPoint3D>& arrOriBorderPoint, UGArray<UGPoint3D>& arrInsertedBorderPoint);

	//! \brief 在线段的内部，在经纬度方向上均匀的插入点
	//! \param nLonDensity，nLatDensity 经纬方向上插入点的个数 [in]
	//! \param dLongStep，dLatStep 每隔多少经纬跨度插入一个点 [in]
	//! \param point1，point2 一条线段的起始终止顶点 [in]
	//! \param arrPoint 输出线段上插入的点 [out]
	static void InsertPointInSegment(UGint nLonDensity, UGint nLatDensity, UGdouble dLongStep, UGdouble dLatStep, const UGPoint3D &startPoint,const UGPoint3D &endPoint, UGArray<UGPoint3D>& arrPoint);

	//! \brief 在多边形的内部插入点
	//! \param pPoint3Ds 多边形的所有点[in]
	//! \param nPointCount 点的总数[in]
	//! \param nSubCount 子对象的个数[in]
	//! \param pPolyCount 每个子对象点的个数[in]
	//! \param pointDensity 插值密度，在每个方向上插入点的个数 [in]
	//! \param dLongStep，dLatStep 多边形边上每隔多少经纬跨度插入一个点 [in]
	//! \param rectBounds 多边形的坐标范围 [in]
	//! \param arrOuterBorderPoint3D 多边形外边界点数组 [in]
	//! \param arrInnerBorderPoint3Ds 多边形内边界点数组 [in]
	//! \param arrInnerPoint3D 输出的多边形内部点 [out]
	static void InsertInnerPointInPoly(const UGPoint3D* pPoint3Ds, UGint nPointCount, UGint nSubCount, const UGint* pPolyCount, UGint nLonDensity, UGint nLatDensity, UGdouble dLongStep, UGdouble dLatStep, UGRect2D rectBounds, const UGArray<UGPoint3D>& arrOuterBorderPoint3D, 
		const UGArray< UGArray<UGPoint3D> >& arrInnerBorderPoint3Ds, UGArray<UGPoint3D>& arrInnerPoint3D);

	//! \brief 对多边形边界点进行不加密的三角化
	//! \param pPoint3Ds 多边形的所有点[in]
	//! \param nPointCount 点的总数[in]
	//! \param nSubCount 子对象的个数[in]
	//! \param pPolyCount 每个子对象点的个数[in]
	//! \param arrIntIndex 输出三角化的索引[out]
	static UGbool Triangulate(const UGPoint3D* pPoint3Ds, UGint nPointCount, UGint nSubCount, const UGint* pPolyCount, UGArray<UGuint>& arrIntIndex);

	//! \brief 根据三角形三个点计算内部一点的Z值
	//! \param testPt 待测试点 [in]
	//! \param pPoints 点串 [in]
	//! \param arrIndex 构建好的索引 [in]
	//! \param firstPt，secondPt，ThirdPt  输出找到的三角形的三个顶点 [out]
	static void FindTrianglePtWithin(const UGPoint3D& testPt, const UGPoint3D* pPoints, const UGArray<UGuint>& arrIndex, UGPoint3D& firstPt, UGPoint3D& secondPt, UGPoint3D& ThirdPt);

	//! \brief 根据三角形三个点(经纬坐标)计算内部（xy经纬坐标已知）的一点的Z值
	//! \param firstPt 第一个点 [in]
	//! \param secondPt 第二个点 [in]
	//! \param ThirdPt 第三个点 [in]
	//! \param resultPt 传入的待计算点，输出此点带Z值 [in/out]
	static void ComputePoint3DInThreePoint(const UGPoint3D& firstPt, const UGPoint3D& secondPt, const UGPoint3D& ThirdPt, UGPoint3D& resultPt);

	//! \brief 找到一个位于多边形边界点内部的点（用来作为洞）
	//! \param arrBorderPoint2D 多边形边界点[in]
	//! \param nPointDensity 如果需要在内部遍历，每个方向遍历的次数[in]
	//! \param dLongStep，dLatStep 如果需要在内部遍历，每隔多少经纬跨度插入一个点[in]
	//! \param rectBounds 如果需要在内部遍历，遍历的坐标范围[in]
	//! \param innerPoint 输出找到的内部点[out]
	//! \return 是否成功找到内部点
	static UGbool FindInnerPointInBorder(const UGArray<UGPoint3D>& arrBorderPoint3D, UGint nLonDensity, UGint nLatDensity, UGdouble dLongStep, UGdouble dLatStep, UGRect2D rectBounds, UGPoint2D& innerPoint);

	//! \brief 为一个边界线段索引数组赋值，边界点必须按照 外边界->内边界的顺序排列，首尾点不重复
	//! \param pSegments 待赋值的边界索引数组，已经new好足够大的空间[out]
	//! \param nOuterBorderPointCount 多边形外边界的点数量[in]
	//! \param arrInnerBorderPointCounts 多边形内边界的顶点数量的数组[in]
	static void BuildSegmentList(UGint* pSegments, UGint nOuterBorderPointCount, const UGArray<UGint>& arrInnerBorderPointCounts);

	//! \brief 对二维点在指定边界、指定洞的条件下加密后进行约束Delaunay三角化
	//! \param pPoints 参与三角化的所有点[in]
	//! \param pHolePoints 洞点[in]
	//! \param pSegments 边界索引，每两个值代表一条线段的起止点的索引值，内边界外边界都可以[in]
	//! \param nPointCount 参与三角化的所有点的个数[in]
	//! \param nHoleCount 洞的个数[in]
	//! \param nSegmentCount 边界线段个数[in]
	//! \param arrAllIntIndex [out]
	//! \return 返回三角化是否成功
	static UGbool Tessellation(UGPoint2D* pPoints, UGPoint2D* pHolePoints, UGint* pSegments, UGint nPointCount, UGint nHoleCount, UGint nSegmentCount, UGArray<UGuint>& arrAllIntIndex);

	//! \brief 检查点数组里面有没有重复顶点
	//! \param pPoints 待检查的点数组[in]
	//! \param bCompareZ Z坐标是否参与比较[in]
	//! \param bStartEndSame 首尾顶点是否一样[in]
	//! \return 返回是否含有重复顶点
	static UGbool CheckRedundantPoint(const UGPoint3D* pPoints, UGint nPointCount, UGbool bCompareZ = TRUE, UGbool bStartEndSame = FALSE);
public:
	static SceneType m_msSceneType;

	static UGbool m_msIClient;


	//是否支持栅格地形
	static UGbool m_bSupportGridTerrain;
};
}
#endif // !defined(AFX_UGMATHENGINE_H__90331450_3347_42D5_B0A0_41353F9437C8__INCLUDED_)

