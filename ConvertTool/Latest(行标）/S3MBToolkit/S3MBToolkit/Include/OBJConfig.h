#if !defined(OBJCONFIG_H__53749146_ABED_4F4E_9E7A_69FC1CFB2C61__INCLUDED_)
#define OBJCONFIG_H__53749146_ABED_4F4E_9E7A_69FC1CFB2C61__INCLUDED_

#pragma once
#include "stdafx.h"
#include "BoundingBox.h"
#include "Common.h"
#include "Matrix.h"
#include "Point3D.h"

#include <string>

namespace S3MB
{
	class STK_API OBJConfig
	{
	public:
		OBJConfig();
		~OBJConfig();

		bool LoadFromFile(const std::wstring& strPath);

		const Point3D& GetPosition() const;

		bool GetHasSrs() const;

		const Matrix4d& GetModelTransform() const;

		int GetLodNum() const;

		int GetTileLength() const;

	private:
		// 插入点
		Point3D m_pntPostion;
		//判断是否有srs标签
		bool m_bHasSrs;
		//模型矩阵
		Matrix4d m_matModelTran;
		// LOD层数
		int m_nLodNum;
		// Tile长度
		int m_nTileLength;
	};
}

#endif