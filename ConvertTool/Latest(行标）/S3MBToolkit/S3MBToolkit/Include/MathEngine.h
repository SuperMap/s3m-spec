#if !defined(MATHENGINE_H__FA79DC0B_BA21_42AE_9B4C_150DFF32012E__INCLUDED_)
#define MATHENGINE_H__FA79DC0B_BA21_42AE_9B4C_150DFF32012E__INCLUDED_

#pragma once
#include "Common.h"
#include "Vector.h"
#include "Matrix.h"

namespace S3MB
{
	//! \brief ��׼�������
	const Vector3d wgs84RadiiAxis(GLOBAL_RADIUS, GLOBAL_RADIUS, GLOBAL_RADIUS_MINOR);
	const Vector3d wgs84RadiiSquared(wgs84RadiiAxis.x * wgs84RadiiAxis.x, wgs84RadiiAxis.y * wgs84RadiiAxis.y, wgs84RadiiAxis.z * wgs84RadiiAxis.z);
	const Vector3d wgs84OneOverRadiiSquared(1.0 / wgs84RadiiSquared.x, 1.0 / wgs84RadiiSquared.y, 1.0 / wgs84RadiiSquared.z);

	//! \brief ƽ̨�������
	const Vector3d wgs84RadiiAxisPlatform(wgs84RadiiAxis.y, wgs84RadiiAxis.z, wgs84RadiiAxis.x);
	const Vector3d wgs84RadiiSquaredPlatform(wgs84RadiiAxisPlatform.x * wgs84RadiiAxisPlatform.x, wgs84RadiiAxisPlatform.y * wgs84RadiiAxisPlatform.y, wgs84RadiiAxisPlatform.z * wgs84RadiiAxisPlatform.z);

	//! \brief ��׼Բ�����
	const Vector3d wgs84SphericalRadiiAxis(GLOBAL_RADIUS, GLOBAL_RADIUS, GLOBAL_RADIUS);
	const Vector3d wgs84SphericalRadiiSquared(wgs84SphericalRadiiAxis.x * wgs84SphericalRadiiAxis.x, wgs84SphericalRadiiAxis.y * wgs84SphericalRadiiAxis.y, wgs84SphericalRadiiAxis.z * wgs84SphericalRadiiAxis.z);
	const Vector3d wgs84SphericalOneOverRadii(1.0 / wgs84SphericalRadiiAxis.x, 1.0 / wgs84SphericalRadiiAxis.y, 1.0 / wgs84SphericalRadiiAxis.z);
	const Vector3d wgs84SphericalOneOverRadiiSquared(1.0 / wgs84SphericalOneOverRadii.x, 1.0 / wgs84SphericalOneOverRadii.y, 1.0 / wgs84SphericalOneOverRadii.z);

	enum SceneType
	{
		ST_EARTH_SPHERICAL = 0,				//�򳡾�
		ST_EARTH_PROJECTION = 1,			//ƽ���ͶӰ����
		ST_NONEARTH = 2,					//ƽ��ľֲ����곡��
		ST_EARTH_ELLIPSOID_WGS84 = 3,		//WGS84�������곡��
	};

	//! \biref ȫ����״
	enum GlobalShape
	{
		//! \brief Բ��
		GS_SPHERE,
		//! \brief ����
		GS_ELLIPSOID
	};

	class STK_API MathEngine
	{
	public:
		//! \brief ��������ת��Ϊ�ѿ�������ϵ��Ĭ�ϵ���뾶Ϊ6378137 
		//! \param dLongitude �������꾭��[in]��
		//! \param dLatitude ��������ά��[in]��
		//! \param GLOBAL_RADIUS ����6378137[in]��
		//! \return �ѿ�������ϵ�����ꡣ
		//! \attention ��ΪԲ�����������
		static Vector3d SphericalToCartesian(double dLongitude, double dLatitude, double dRadius = GLOBAL_RADIUS);

		//! \brief ��������ת��Ϊ�ѿ�������ϵ��Ĭ�ϵ���뾶Ϊ6378137 
		//! \param sceneType ��������[in]��
		//! \param dLongitude �������꾭��[in]��
		//! \param dLatitude ��������ά��[in]��
		//! \param GLOBAL_RADIUS ����6378137[in]��
		//! \return �ѿ�������ϵ�����ꡣ
		//! \attention ��ΪԲ�����������
		static Vector3d SphericalToCartesian(SceneType sceneType, double dLongitude, double dLatitude, double dRadius = GLOBAL_RADIUS);

		//! \brief WGS84�����廻Ϊ�ѿ�������ϵ��Ĭ�ϵ���뾶Ϊ6378137 
		//! \param dLongitude �������꾭��[in]��
		//! \param dLatitude ��������ά��[in]��
		//! \param GLOBAL_RADIUS ����6378137[in]��
		//! \return �ѿ�������ϵ�����ꡣ
		//! \attention ��ΪԲ�����������
		static Vector3d WGS84ToCartesian(double dLongitude, double dLatitude, double dRadius = GLOBAL_RADIUS);

		//! \brief �ѿ�������ת��Ϊ�������꣨�������꣩��
		//! \param x �ѿ�������x[in]��
		//! \param y �ѿ�������y[in]��
		//! \param z �ѿ�������z[in]��
		//! \return ��������ϵ���ꡣ
		static Vector3d CartesianToWGS84(double x, double y, double z, GlobalShape gs = GS_ELLIPSOID);

		//! \brief �ѿ�������ת��Ϊ�������꣨�������꣩��
		//! \param x �ѿ�������x[in]��
		//! \param y �ѿ�������y[in]��
		//! \param z �ѿ�������z[in]��
		//! \return ��������ϵ���ꡣ
		static Vector3d CartesianToSphericalD(double x, double y, double z);
		//! \brief �ѿ�������ת��Ϊ�������꣨�������꣩��
		//! \param x �ѿ�������x[in]��
		//! \param y �ѿ�������y[in]��
		//! \param z �ѿ�������z[in]��
		//! \return ��������ϵ���ꡣ
		static Vector3d CartesianToSphericalD(double x, double y, double z, SceneType sceneType);

		//! \brief WGS84�����廻ΪCesium�µĵѿ�������ϵ��Ĭ�ϵ���뾶Ϊ6378137 
		//! \param dLongitude �������꾭��[in]��
		//! \param dLatitude ��������ά��[in]��
		//! \param GLOBAL_RADIUS ����6378137[in]��
		//! \return Cesium�µĵѿ�������ϵ�����ꡣ
		//! \attention ���ǵ���ΪԲ�����������
		static Vector3d WGS84ToCesiumCartesian(double dLongitude, double dLatitude, double dRadius = GLOBAL_RADIUS, GlobalShape gs = GS_ELLIPSOID);

		//! \brief ������ENU����ϵת��Ϊ���������ת������
		static Matrix4d EllipsoidalENUToWGS84(
			const Vector3d& vBaseLonLatPt,
			double dRadius = GLOBAL_RADIUS);

		//! \brief Բ����ENU����ϵת��Ϊ�������꣨�������꣩��
		//! \param vBasePt [in] ��׼�����ꡣ
		//! \param dX [in] �ѿ�������x��
		//! \param dY [in] �ѿ�������y��
		//! \param dZ [in] �ѿ�������z��
		//! \param dRadius [in] ��뾶��Ĭ��GLOBAL_RADIUS��
		//! \return ��������ϵ���ꡣ
		static Vector3d GlobalENUToSpherical(
			const Vector3d& vBasePt,
			double dX,
			double dY,
			double dZ,
			double dRadius = GLOBAL_RADIUS);

		//! \brief ��ȡ����Ĳ�������
		static Matrix4d GetEllipsoidWorldView(const Vector3d& vecPos);

		//! \brief �жϸ����͵�������
		//! \param fValue ����ĸ�����ֵ[in]��
		//! \return ��������1����������-1��0����0.0��
		static double Sign(double fValue);

		//! \brief ͨ�����Բ�ֵ����������š�from gluScaleImage
		//! \param components ��ɫ�ɷ�������RGBA��4
		//! \param widthin ����ͼ��Ŀ��
		//! \param heightin ����ͼ��ĸ߶�
		//! \param datain ����ͼ���ָ��
		//! \param widthout ���ͼ��Ŀ��
		//! \param heightout ���ͼ��ĸ߶�
		//! \param dataout ���ͼ���ָ��
		//! \param nsizedatain datain�Ĵ�С
		static void ScaleImageInternal(int components, int widthin, int heightin,
			const unsigned char* datain,
			int widthout, int heightout,
			unsigned char* dataout, const int nsizedatain = -1);

		//! \brief ͨ�����Բ�ֵ����������š�
		//! \param components ��ɫ�ɷ�������RGBA��4
		//! \param widthin ����ͼ��Ŀ��
		//! \param heightin ����ͼ��ĸ߶�
		//! \param datain ����ͼ���ָ��
		//! \param dataout ���ͼ���ָ��
		static void HalveImage(int components, unsigned int width, unsigned int height,
			const unsigned char *datain, unsigned char *dataout);

		//! \brief ��ͼ���������ݽ��ж��봦����ʱֻ��24λͼ����
		//! \param components ��ɫ�ɷ�������RGBA��4
		//! \param width ����ͼ��Ŀ��
		//! \param height ����ͼ��ĸ߶�
		//! \param datain ����ͼ���ָ��
		//! \param dataout ���ͼ���ָ��
		static bool ClipImageData(int components, int width, int height,
			const unsigned char *datain, const int ndatainsize, unsigned char *dataout);

	public:
		static SceneType m_enSceneType;
	};
}

#endif