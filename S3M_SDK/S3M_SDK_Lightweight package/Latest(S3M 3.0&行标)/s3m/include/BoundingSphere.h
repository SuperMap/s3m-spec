#if !defined(BOUNDINGSPHERE_H__65D8FD01_BA25_482B_9F73_6754CC922D09__INCLUDED_)
#define BOUNDINGSPHERE_H__65D8FD01_BA25_482B_9F73_6754CC922D09__INCLUDED_

#pragma once
#include "stdafx.h"
#include "Vector.h"
#include "BoundingBox.h"

namespace S3MB
{
	class S3MB_API BoundingSphere
	{
	public:
		BoundingSphere();

		BoundingSphere(const BoundingBox& box);

		~BoundingSphere(void);

	public:
		// 判断包围球是否有效
		bool valid() const;

		// 获取包围盒
		BoundingBox GetBoundingBox() const;

		// 获取中心点
		Vector3d GetCenter() const;

		// 获取半径
		double GetRadius() const;

		// 给据给定的包围球来扩展，球心、半径都改变
		// 待扩展的球
		void ExpandBy(const BoundingSphere& bs);

	public:
		Vector3d m_center;
		double m_radius;
	};
}

#endif