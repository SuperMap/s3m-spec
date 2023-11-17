#if !defined(BOUNDINGBOX_H__7B747118_DB94_4FC7_9ED4_016DA765BD9A__INCLUDED_)
#define BOUNDINGBOX_H__7B747118_DB94_4FC7_9ED4_016DA765BD9A__INCLUDED_

#pragma once
#include "stdafx.h"
#include "Vector.h"
#include "Matrix.h"
#include <vector>

namespace S3MB
{
	class BoundingBox;
	class S3MB_API OrientedBoundingBox
	{
	public:

		struct HullVertice
		{
			std::vector<char*> data;
		};

		static std::vector<HullVertice> m_arrHullVertice;

		// 构造函数
		OrientedBoundingBox();

		OrientedBoundingBox(Vector3d vCenter, Matrix3d matHalfAxes);

		OrientedBoundingBox& operator = (const OrientedBoundingBox& boundingBox);

		// 得到包围盒的中心点
		Vector3d GetCenter() const;

		// 得到包围盒的变换矩阵
		Matrix3d GetHalfAxes() const;

		// 标志包围盒是否为空。
		bool IsNULL() const { return m_bNull; }

		// 将两个包围盒进行合并。
		// OrientedBoundingBox 要进行合并的包围盒[in]。
		void Merge(const OrientedBoundingBox& obb);

		// 获取方向包围盒
		void FromPoints(const std::vector<Vector3d>& vecPosition);

		// obb转BoundingBox
		void ToBoundingBox(BoundingBox& BBOX);

	private:
		// 中心点
		Vector3d m_vCenter;

		// 变换矩阵
		Matrix3d m_matHalfAxes;

		// 标志包围盒是否为空。
		bool m_bNull;

		// 是否计算投影
		bool m_bBoxAreaError;

		void UpdateBBox();

		std::vector<Vector3d> m_arrBox;

		Vector3d m_vecU;
		Vector3d m_vecV;
		Vector3d m_vecW;

		Vector3d m_posMin;
		Vector3d m_posMax;
		double m_dHalfDiagonalLine;
	};

	class S3MB_API BoundingBox
	{
	public:
		BoundingBox();
		BoundingBox(Vector3d vMin, Vector3d vMax);

		// 判断是否是合法实数。
		// dValue 实数[in]。
		// return true为合法，false为非法。
		bool IsRealNaN(double dValue);

		// 判断有效性
		// return 包围盒是否有效
		bool IsVaild();

		// 将两个包围盒进行合并。
		// BoundBox 要进行合并的包围盒[in]。
		void Merge(const BoundingBox& BoundBox);

		// 构成包围盒的最小向量。
		// 设置构成包围盒的最小向量。
		// vMin 构成包围盒的最小向量[in]。
		void SetMin(Vector3d vMin);

		// 设置构成包围盒的最大向量。
		// vMax 构成包围盒的最大向量[in]。
		void SetMax(Vector3d vMax);

		// 设置构成包围盒的最小向量。
		// x,y,z 构成包围盒的最小向量[in]。
		void SetMin(double x, double y, double z);

		// 设置构成包围盒的最大向量。
		// x,y,z 构成包围盒的最大向量[in]。
		void SetMax(double x, double y, double z);

		// 设置构成包围盒的最大、最小向量。
		// vMin 构成包围盒的最大最小向量[in]。
		// vMax 构成包围盒的最大向量[in]。
		void SetExtents(const Vector3d& vMin, const Vector3d& vMax);

		// 设置构成包围盒的最大、最小向量。
		// (minx,miny,minz) 构成包围盒的最大最小向量[in]。
		// (maxx,maxy,maxz) 构成包围盒的最大向量[in]。
		void SetExtents(double minx, double miny, double minz,
			double maxx, double maxy, double maxz);

		// 设置包围盒为空
		void SetNULL(void);

		// 判断包围盒是否为空
		bool IsNULL(void) const;

		//  得到构成包围盒的最小向量
		const Vector3d& GetMin(void) const;

		// 得到构成包围盒的最大向量
		const Vector3d& GetMax(void) const;

		// 得到构成包围盒的8个点。
		const Vector3d* GetCorners() const;

		// 得到包围盒的中心点。
		// 说明： 这个中心点是构成包围盒的最小向量和最大向量的平均值。
		Vector3d GetCenter() const;

		// 对包围盒进行矩阵变换
		// matrix 对包围盒进行变换的矩阵[in]。
		void Transform(Matrix4d& matrix);

		// 转成OBB包围盒
		void ToOrientedBoundingBox(OrientedBoundingBox& OBB);

	protected:
		// 根据最大最小向量更新包围盒的8个角点。
		void UpdateCorners(void);
	private:
		Vector3d m_vMin
			;
		// 构成包围盒的最大向量。
		Vector3d m_vMax;

		// 构成包围盒8个角点。
		Vector3d m_vCorner[8];

		// 标志包围盒是否为空。
		bool m_bNull;
	};
}

#endif