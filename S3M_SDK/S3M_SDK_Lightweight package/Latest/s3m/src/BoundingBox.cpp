#include "BoundingBox.h"
#include "S3MBCommon.h"
#ifndef WIN32
#include <cmath>
#endif

namespace S3MB
{
	OrientedBoundingBox::OrientedBoundingBox()
	{
		m_vCenter = Vector3d(0, 0, 0);
		m_matHalfAxes = Matrix3d::ZERO;
		m_bNull = true;
		m_bBoxAreaError = false;
		m_dHalfDiagonalLine = 0.0;
	}

	OrientedBoundingBox::OrientedBoundingBox(Vector3d vCenter, Matrix3d matHalfAxes)
	{
		m_vCenter = vCenter;
		m_matHalfAxes = matHalfAxes;
		m_bNull = false;
		m_bBoxAreaError = false;

		UpdateBBox();
	}

	S3MB::Vector3d OrientedBoundingBox::GetCenter() const
	{
		return m_vCenter;
	}

	S3MB::Matrix3d OrientedBoundingBox::GetHalfAxes() const
	{
		return m_matHalfAxes;
	}

	void OrientedBoundingBox::Merge(const OrientedBoundingBox& obb)
	{
		if (obb.IsNULL())
			return;
		else if (m_bNull)
			*this = obb;
		else
		{
			std::vector<Vector3d> vecCornerPt;
			vecCornerPt.insert(vecCornerPt.end(),m_arrBox.begin(),m_arrBox.end());

			vecCornerPt.insert(vecCornerPt.end(), obb.m_arrBox.begin(), obb.m_arrBox.end());

			FromPoints(vecCornerPt);
		}
	}

	void OrientedBoundingBox::FromPoints(const std::vector<Vector3d>& vecPosition)
	{
		if (vecPosition.empty())
		{
			m_vCenter = Vector3d();
			m_matHalfAxes = Matrix3d::ZERO;
			m_bNull = true;
			return;
		}

		int nSize = vecPosition.size();
		Vector3d vMeanPoint;
		for (int i = 0; i < nSize; i++)
		{
			vMeanPoint = vMeanPoint + vecPosition[i];
		}

		double dInvLength = 1.0 / nSize;
		vMeanPoint = vMeanPoint * dInvLength;

		double dExx = 0.0, dExy = 0.0, dExz = 0.0, dEyy = 0.0, dEyz = 0.0, dEzz = 0.0;
		Vector3d vPos;
		for (int i = 0; i < nSize; i++)
		{
			vPos = vecPosition[i] - vMeanPoint;
			dExx += vPos.x * vPos.x;
			dExy += vPos.x * vPos.y;
			dExz += vPos.x * vPos.z;
			dEyy += vPos.y * vPos.y;
			dEyz += vPos.y * vPos.z;
			dEzz += vPos.z * vPos.z;
		}

		dExx *= dInvLength;
		dExy *= dInvLength;
		dExz *= dInvLength;
		dEyy *= dInvLength;
		dEyz *= dInvLength;
		dEzz *= dInvLength;

		Matrix3d matCovariance(
			dExx, dExy, dExz,
			dExy, dEyy, dEyz,
			dExz, dEyz, dEzz);

		Matrix3d matUnitary, matDiagonal;
		matCovariance.EigenDecomposition(matUnitary, matDiagonal, 1e-20);

		Vector3d v1 = matUnitary.GetColumn(0);
		Vector3d v2 = matUnitary.GetColumn(1);
		Vector3d v3 = matUnitary.GetColumn(2);

		double dU1 = -DBLMAX;
		double dU2 = -DBLMAX;
		double dU3 = -DBLMAX;
		double dL1 = DBLMAX;
		double dL2 = DBLMAX;
		double dL3 = DBLMAX;

		for (int i = 0; i < nSize; i++)
		{
			vPos = vecPosition[i];
			dU1 = MAX(v1.DotProduct(vPos), dU1);
			dU2 = MAX(v2.DotProduct(vPos), dU2);
			dU3 = MAX(v3.DotProduct(vPos), dU3);

			dL1 = MIN(v1.DotProduct(vPos), dL1);
			dL2 = MIN(v2.DotProduct(vPos), dL2);
			dL3 = MIN(v3.DotProduct(vPos), dL3);
		}

		v1 = v1 * (0.5 * (dL1 + dU1));
		v2 = v2 * (0.5 * (dL2 + dU2));
		v3 = v3 * (0.5 * (dL3 + dU3));

		m_vCenter = v1 + v2 + v3;

		Vector3d vScale;
		vScale.x = dU1 - dL1;
		vScale.y = dU2 - dL2;
		vScale.z = dU3 - dL3;
		vScale = vScale * 0.5;

		Vector3d vColumn0;
		Vector3d vColumn1;
		Vector3d vColumn2;
		vColumn0 = matUnitary.GetColumn(0) * vScale.x;
		vColumn1 = matUnitary.GetColumn(1) * vScale.y;
		vColumn2 = matUnitary.GetColumn(2) * vScale.z;
		m_matHalfAxes.FromAxes(vColumn0, vColumn1, vColumn2);
		m_bNull = false;

		UpdateBBox();
	}

	void OrientedBoundingBox::ToBoundingBox(BoundingBox& BBOX)
	{
		UpdateBBox();

		const Vector3d& halfAxisX = m_matHalfAxes.GetColumn(0);
		const Vector3d& halfAxisY = m_matHalfAxes.GetColumn(1);
		const Vector3d& halfAxisZ = m_matHalfAxes.GetColumn(2);

		Vector3d extent;
		extent.x = std::abs(halfAxisX.x) + std::abs(halfAxisY.x) + std::abs(halfAxisZ.x);
		extent.y = std::abs(halfAxisX.y) + std::abs(halfAxisY.y) + std::abs(halfAxisZ.y);
		extent.z = std::abs(halfAxisX.z) + std::abs(halfAxisY.z) + std::abs(halfAxisZ.z);

		const Vector3d& min = m_vCenter - extent;
		const Vector3d& max = m_vCenter + extent;

		BBOX.SetExtents(min, max);
	}

	void OrientedBoundingBox::UpdateBBox()
	{
		Vector3d u = m_matHalfAxes.GetColumn(0);
		Vector3d u_n = -u;
		Vector3d v = m_matHalfAxes.GetColumn(1);
		Vector3d v_n = -v;
		Vector3d w = m_matHalfAxes.GetColumn(2);
		Vector3d w_n = -w;

		// check correction                                        w
		/*                                                         |
				   7-------6                                       |
				  / |    / |                                       /------u
				 /  |   /  |                                      /
				3---|---2  5                                     v
				| /     | /
				|/      |/
				0-------1
		*/


		Vector3d positionTmp = m_vCenter + v;// 0-3
		Vector3d positionTmp2 = positionTmp + w_n;// 0-1
		Vector3d positionTmp3 = positionTmp2 + u_n;// 0

		m_arrBox.clear();
		m_arrBox.push_back(positionTmp3);
		positionTmp3 = positionTmp2 + u;// 1
		m_arrBox.push_back(positionTmp3);

		positionTmp2 = positionTmp + w;// 2-3
		positionTmp3 = positionTmp2 + u;// 2
		m_arrBox.push_back(positionTmp3);
		positionTmp3 = positionTmp2 + u_n; // 3
		m_arrBox.push_back(positionTmp3);

		positionTmp = m_vCenter + v_n; // 4-7
		positionTmp2 = positionTmp + w_n; // 4-5
		positionTmp3 = positionTmp2 + u_n; // 4
		m_arrBox.push_back(positionTmp3);
		positionTmp3 = positionTmp2 + u; // 5
		m_arrBox.push_back(positionTmp3);
		positionTmp2 = positionTmp + w; // 6-7
		positionTmp3 = positionTmp2 + u; // 6
		m_arrBox.push_back(positionTmp3);
		positionTmp3 = positionTmp2 + u_n; // 7
		m_arrBox.push_back(positionTmp3);

		double uHalf = u.Length();
		double vHalf = v.Length();
		double wHalf = w.Length();

		m_posMin = Vector3d(-uHalf, -vHalf, -wHalf);
		m_posMax = Vector3d(uHalf, vHalf, wHalf);

		m_dHalfDiagonalLine = 0.5 * (m_posMax - m_posMin).Length();

		if (uHalf*vHalf*wHalf == 0)
		{
			m_bBoxAreaError = true;
		}

		u.Normalize();
		v.Normalize();
		w.Normalize();

		m_vecU = u;
		m_vecV = v;
		m_vecW = w;
	}

	OrientedBoundingBox& OrientedBoundingBox::operator=(const OrientedBoundingBox& boundingBox)
	{
		if (this == &boundingBox)
		{
			return *this;
		}

		m_vCenter = boundingBox.GetCenter();
		m_matHalfAxes = boundingBox.GetHalfAxes();
		m_bNull = boundingBox.IsNULL();
		UpdateBBox();

		return *this;
	}

	BoundingBox::BoundingBox()
	{
		SetMin(-0.5, -0.5, -0.5);
		SetMax(0.5, 0.5, 0.5);
		m_bNull = true;
	}

	BoundingBox::BoundingBox(Vector3d vMin, Vector3d vMax)
	{
		SetExtents(vMin, vMax);
	}

	bool BoundingBox::IsRealNaN(double dValue)
	{
		if (dValue > DBLMAX || dValue < -DBLMAX || IS0(dValue - DBLMAX) || IS0(dValue - DBLMIN))
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	bool BoundingBox::IsVaild()
	{
		return (IsRealNaN(m_vMax.x) && IsRealNaN(m_vMax.y) && IsRealNaN(m_vMax.z) && \
			IsRealNaN(m_vMin.x) && IsRealNaN(m_vMin.y) && IsRealNaN(m_vMin.z));
	}

	void BoundingBox::Merge(const BoundingBox& BoundBox)
	{
		if (BoundBox.m_bNull)
			return;
		else if (m_bNull)
			SetExtents(BoundBox.m_vMin, BoundBox.m_vMax);
		else
		{
			Vector3d vMin = m_vMin;
			Vector3d vMax = m_vMax;
			vMax.MakeCeil(BoundBox.m_vMax);
			vMin.MakeFloor(BoundBox.m_vMin);
			SetExtents(vMin, vMax);
		}
	}

	void BoundingBox::SetMin(Vector3d vMin)
	{
		m_bNull = false;
		m_vMin = vMin;
		UpdateCorners();
	}

	void BoundingBox::SetMin(double x, double y, double z)
	{
		m_bNull = false;
		m_vMin.x = x;
		m_vMin.y = y;
		m_vMin.z = z;
		UpdateCorners();
	}

	void BoundingBox::SetMax(Vector3d vMax)
	{
		m_bNull = false;
		m_vMax = vMax;
		UpdateCorners();
	}

	void BoundingBox::SetMax(double x, double y, double z)
	{
		m_bNull = false;
		m_vMax.x = x;
		m_vMax.y = y;
		m_vMax.z = z;
		UpdateCorners();
	}

	void BoundingBox::SetExtents(const Vector3d& vMin, const Vector3d& vMax)
	{
		m_bNull = false;
		m_vMin = vMin;
		m_vMax = vMax;
		UpdateCorners();
	}

	void BoundingBox::SetExtents(double minx, double miny, double minz, double maxx, double maxy, double maxz)
	{
		m_bNull = false;
		m_vMin.x = minx;
		m_vMin.y = miny;
		m_vMin.z = minz;

		m_vMax.x = maxx;
		m_vMax.y = maxy;
		m_vMax.z = maxz;
		UpdateCorners();
	}

	void BoundingBox::SetNULL(void)
	{ 
		m_bNull = true; 
	}

	bool BoundingBox::IsNULL(void) const
	{
		return m_bNull;
	}

	const Vector3d& BoundingBox::GetMin(void) const
	{
		return m_vMin;
	}

	const Vector3d& BoundingBox::GetMax(void) const
	{
		return m_vMax;
	}

	const Vector3d* BoundingBox::GetCorners() const
	{
		return (const Vector3d*)m_vCorner;
	}

	Vector3d BoundingBox::GetCenter() const
	{
		Vector3d center;
		center.x = (m_vMin.x + m_vMax.x) / 2.0;
		center.y = (m_vMin.y + m_vMax.y) / 2.0;
		center.z = (m_vMin.z + m_vMax.z) / 2.0;

		return center;
	}

	void BoundingBox::Transform(Matrix4d& matrix)
	{
		if (m_bNull)
			return;
		bool bFirst = true;
		Vector3d vMax, vMin, vTemp;
		int i = 0;
		for (i = 0; i < 8; i++)
		{
			vTemp = m_vCorner[i].MultiplyMatrix(matrix);
			if (bFirst || vTemp.x > vMax.x)
				vMax.x = vTemp.x;
			if (bFirst || vTemp.y > vMax.y)
				vMax.y = vTemp.y;
			if (bFirst || vTemp.z > vMax.z)
				vMax.z = vTemp.z;
			if (bFirst || vTemp.x < vMin.x)
				vMin.x = vTemp.x;
			if (bFirst || vTemp.y < vMin.y)
				vMin.y = vTemp.y;
			if (bFirst || vTemp.z < vMin.z)
				vMin.z = vTemp.z;
			bFirst = false;
		}
		SetExtents(vMin, vMax);
	}

	void BoundingBox::ToOrientedBoundingBox(OrientedBoundingBox& OBB)
	{
		if (m_bNull)
			return;

		std::vector<Vector3d> vecPosition;
		for (int i = 0; i < 8; i++)
		{
			vecPosition.push_back(m_vCorner[i]);
		}
		OBB.FromPoints(vecPosition);
	}

	void BoundingBox::UpdateCorners(void)
	{
		for (int pos = 0; pos < 8; ++pos)
		{
			m_vCorner[pos] = Vector3d(pos & 1 ? m_vMax.x : m_vMin.x, pos & 2 ? m_vMax.y : m_vMin.y, pos & 4 ? m_vMax.z : m_vMin.z);
		}
		m_bNull = false;
	}
}