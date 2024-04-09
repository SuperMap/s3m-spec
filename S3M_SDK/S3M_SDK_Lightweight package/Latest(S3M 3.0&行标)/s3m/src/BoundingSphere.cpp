#include "BoundingSphere.h"

namespace S3MB
{
	BoundingSphere::BoundingSphere()
		:m_center(0.0, 0.0, 0.0), m_radius(-1)
	{
	}

	BoundingSphere::BoundingSphere(const BoundingBox& box)
	{
		m_center = box.GetCenter();
		m_radius = (box.GetMax() - box.GetMin()).Length() / 2;
	}

	BoundingSphere::~BoundingSphere(void)
	{
	}

	bool BoundingSphere::valid() const
	{
		return m_radius > 0.0;
	}

	BoundingBox BoundingSphere::GetBoundingBox() const
	{
		Vector3d vMinVec(m_center.x - m_radius, m_center.y - m_radius, m_center.z - m_radius);
		Vector3d vMaxVec(m_center.x + m_radius, m_center.y + m_radius, m_center.z + m_radius);
		return BoundingBox(vMinVec, vMaxVec);
	}

	Vector3d BoundingSphere::GetCenter() const
	{
		return m_center;
	}

	double BoundingSphere::GetRadius() const
	{
		return m_radius;
	}

	void BoundingSphere::ExpandBy(const BoundingSphere& bs)
	{
		// 原球无效
		if (!valid())
		{
			m_center = bs.m_center;
			m_radius = bs.m_radius;
			return;
		}

		// 两包围球球心间的距离
		double d = (m_center - bs.m_center).Length();

		// bs在原球之内
		if (d + bs.m_radius <= m_radius)
		{
			return;
		}

		// bs包围原球 
		if (d + m_radius <= bs.m_radius)
		{
			m_center = bs.m_center;
			m_radius = bs.m_radius;
			return;
		}

		double new_radius = (m_radius + d + bs.m_radius) * 0.5;
		double ratio = (new_radius - m_radius) / d;

		m_center[0] += (bs.m_center[0] - m_center[0]) * ratio;
		m_center[1] += (bs.m_center[1] - m_center[1]) * ratio;
		m_center[2] += (bs.m_center[2] - m_center[2]) * ratio;

		m_radius = new_radius;
	}
}