#if !defined(SKELETON_H__78795D40_559B_470C_94B5_A3B47837A1D0__INCLUDED_)
#define SKELETON_H__78795D40_559B_470C_94B5_A3B47837A1D0__INCLUDED_

#pragma once
#include "S3MBCommon.h"
#include "Matrix.h"
#include "BoundingBox.h"
#include <iostream>
#include <vector>
#include <string.h>
#include "MemoryStream.h"

namespace S3MB
{

	struct InstanceInfo
	{
		InstanceInfo()
		{
			nCount = 0;
			pMatrixValues = nullptr;
		}
		~InstanceInfo()
		{
			if (pMatrixValues != nullptr)
			{
				delete pMatrixValues;
				pMatrixValues = nullptr;
			}
		}
		// 取值为16
		unsigned short nCount;
		float* pMatrixValues;
	};
	// 顶点包
	template<typename T>
	class VertexDataPackageTemplate
	{
	public:
		VertexDataPackageTemplate();
		~VertexDataPackageTemplate();
		void clear();
	public:
		// 数据绘制属性
		int m_nVertexOptions;
		// 数据压缩标记
		unsigned int m_nCompressOptions;
		// 是否有指定压缩项
		bool HasCompressOptions(VertexCompressOptions enOptions)
		{
			return (m_nCompressOptions & enOptions) == enOptions;
		}

		// 顶点个数
		unsigned int m_nVerticesCount;
		// 顶点维数
		unsigned short m_nVertexDimension;
		// 顶点偏移
		unsigned short m_nVertexStride;
		// 顶点数组
		T* m_pVertices;
		// 顶点各分量的最小值
		float m_minVerticesValue[4];
		// 顶点压缩归一化常量
		float m_fVertCompressConstant;

		// 法线个数
		unsigned int m_nNormalCount;
		// 法线维数
		unsigned short m_nNormalDimension;
		// 法线偏移
		unsigned short m_nNormalStride;
		// 法线数组
		float* m_pNormals;

		// 切线数组
		float* m_pTangents;
		// 切线数目
		unsigned int m_nTangentCount;
		//! 切线维数
		unsigned short m_nTangentDimension;
		// 切线数组偏移 
		unsigned short m_nTangentStride;

		// 顶点颜色个数
		unsigned int m_nVertexColorCount;
		// 顶点颜色偏移
		unsigned short m_nVertexColorStride;
		// 顶点颜色数组
		unsigned int* m_pVertexColor;

		// 贴图坐标数目
		unsigned int m_TexCoordCount[SMSCN_MAX_TEXTURE_COORD_SETS];

		// 贴图坐标的维数
		unsigned short m_nTexDimensions[SMSCN_MAX_TEXTURE_COORD_SETS];

		// 每组贴图坐标的Stride
		unsigned short m_TexCoordStride[SMSCN_MAX_TEXTURE_COORD_SETS];

		// 贴图坐标数据
		float* m_pTexCoords[SMSCN_MAX_TEXTURE_COORD_SETS];

		// 纹理坐标各分量的最小值
		float m_minTexCoordValue[SMSCN_MAX_TEXTURE_COORD_SETS][4];

		// 纹理坐标压缩归一化常量
		float m_texCoordCompressConstant[SMSCN_MAX_TEXTURE_COORD_SETS];

		std::vector<InstanceInfo*> m_vecInstanceInfo;

		// 属性数目
		unsigned int m_nVertexAttCount;
		// 属性语义信息和对应索引
		std::map<wstring,int> m_mapVertexAttributeDescript;
		// 属性数据数目的数组
		std::vector<unsigned int> m_vecVertexAttDataCount;
		// 属性数据维数的数组
		std::vector<unsigned short> m_vecVertexAttDataDimension;
		// 属性数据类型的数组
		std::vector<VertexAttributeType> m_vecVertexAttDataType;
		// 属性数据的数组
		std::vector<void*> m_vecVertexAttData;

		void SetVertexNum(unsigned int nVertexCount, unsigned short nVertexStride = 0)
		{
			m_nVerticesCount = nVertexCount;
			if (m_pVertices != NULL)
			{
				delete[] m_pVertices;
				m_pVertices = NULL;
			}
			if (nVertexCount == 0)
			{
				return;
			}
			m_pVertices = new T[nVertexCount * m_nVertexDimension];
			memset(m_pVertices, 0, sizeof(T) * nVertexCount * m_nVertexDimension);
			m_nVertexStride = nVertexStride;
		}
		void SetNormalNum(unsigned int nNormalCount, unsigned short nNormalStride = 0)
		{
			m_nNormalCount = nNormalCount;
			if (m_pNormals != NULL)
			{
				delete[] m_pNormals;
				m_pNormals = NULL;
			}
			m_nNormalStride = nNormalStride;
			if (nNormalCount == 0)
			{
				return;
			}

			m_pNormals = new float[nNormalCount * m_nNormalDimension];
			memset(m_pNormals, 0, sizeof(float) * nNormalCount * m_nNormalDimension);
		}
		void SetTangentNum(unsigned int nTangentCount, unsigned short nTangentStride = 0)
		{
			m_nTangentCount = nTangentCount;
			if (m_pTangents != NULL)
			{
				delete[] m_pTangents;
				m_pTangents = NULL;
			}
			m_nTangentStride = nTangentStride;
			if (nTangentCount == 0)
			{
				return;
			}

			m_pTangents = new float[nTangentCount * m_nTangentDimension];

			memset(m_pTangents, 0, sizeof(float) * nTangentCount * m_nTangentDimension);
		}
		void SetColorNum(unsigned int nColorCount, unsigned short nColorStride = 0)
		{
			m_nVertexColorCount = nColorCount;
			if (m_pVertexColor != NULL)
			{
				delete[] m_pVertexColor;
				m_pVertexColor = NULL;
			}
			if (nColorCount == 0)
			{
				return;
			}
			m_pVertexColor = new unsigned int[nColorCount];
			memset(m_pVertexColor, 0, sizeof(unsigned int) * nColorCount);
			m_nVertexColorStride = nColorStride;
		}
		bool SetTexCoordsNum(unsigned short nTexUnit, unsigned int nTexCoordCount, unsigned short nTexCoordStride = 0)
		{
			if (nTexUnit >= SMSCN_MAX_TEXTURE_COORD_SETS)
			{
				return false;
			}
			m_TexCoordCount[nTexUnit] = nTexCoordCount;

			if (m_pTexCoords[nTexUnit] != NULL)
			{
				delete[] m_pTexCoords[nTexUnit];
				m_pTexCoords[nTexUnit] = NULL;
			}
			if (nTexCoordCount == 0)
			{
				return true;
			}
			m_pTexCoords[nTexUnit] = new float[nTexCoordCount * m_nTexDimensions[nTexUnit]];
			memset(m_pTexCoords[nTexUnit], 0, sizeof(float) * nTexCoordCount * m_nTexDimensions[nTexUnit]);
			m_TexCoordStride[nTexUnit] = nTexCoordStride;
			return true;
		}

		bool SetAttDataNum(unsigned int nAttDataCount, \
			unsigned short nAttDataDimension, VertexAttributeType eAttDataType)
		{
			if (nAttDataCount == 0)
			{
				return true;
			}
			if (eAttDataType == AT_32BIT)
			{
				unsigned int* pAttData = new unsigned int[nAttDataCount * nAttDataDimension];
				memset(pAttData, 0, sizeof(unsigned int) * nAttDataCount * nAttDataDimension);
				m_vecVertexAttData.push_back(pAttData);
			}
			else if (eAttDataType == AT_FLOAT)
			{
				float* pAttData = new float[nAttDataCount * nAttDataDimension];
				memset(pAttData, 0, sizeof(float) * nAttDataCount * nAttDataDimension);
				m_vecVertexAttData.push_back(pAttData);
			}
			else if (eAttDataType == AT_DOUBLE)
			{
				double* pAttData = new double[nAttDataCount * nAttDataDimension];
				memset(pAttData, 0, sizeof(double) * nAttDataCount * nAttDataDimension);
				m_vecVertexAttData.push_back(pAttData);
			}
			return true;
		}
	};
	template<typename T>
	void VertexDataPackageTemplate<T>::clear()
	{
		if (m_pVertices != NULL)
		{
			delete[] m_pVertices;
			m_pVertices = NULL;
		}
		m_nVerticesCount = 0;

		if (m_pNormals != NULL)
		{
			delete[] m_pNormals;
			m_pNormals = NULL;
		}
		m_nNormalCount = 0;

		if (m_pTangents != NULL)
		{
			delete[] m_pTangents;
			m_pTangents = NULL;
		}
		m_nTangentCount = 0;

		if (m_pVertexColor != NULL)
		{
			delete[] m_pVertexColor;
			m_pVertexColor = NULL;
		}

		m_nVertexColorCount = 0;

		unsigned short i = 0;
		for (i = 0; i < SMSCN_MAX_TEXTURE_COORD_SETS; i++)
		{
			if (m_pTexCoords[i] != NULL)
			{
				delete[] m_pTexCoords[i];
				m_pTexCoords[i] = NULL;
			}
			m_TexCoordCount[i] = 0;
		}

		for (auto& iInfo : m_vecInstanceInfo)
		{
			if (iInfo != nullptr)
			{
				delete iInfo;
				iInfo = nullptr;
			}
		}

		m_nVertexAttCount = 0;
		m_vecVertexAttDataCount.clear();
		m_vecVertexAttDataDimension.clear();
		m_vecVertexAttDataType.clear();
		for (unsigned int i = 0; i < m_vecVertexAttData.size(); i++)
		{
			delete[] m_vecVertexAttData[i];
			m_vecVertexAttData[i] = NULL;
		}
		m_vecVertexAttData.clear();
	}
	template<typename T>
	VertexDataPackageTemplate<T>::VertexDataPackageTemplate()
	{
		m_nCompressOptions = 0;
		m_nVertexDimension = 3;
		m_nNormalDimension = 3;
		m_pVertices = NULL;
		m_nVerticesCount = 0;
		m_nVertexStride = 0;
		m_fVertCompressConstant = 0.0;
		m_pNormals = NULL;
		m_nNormalCount = 0;
		m_nNormalStride = 0;
		m_pVertexColor = NULL;
		m_nVertexColorCount = 0;
		m_nVertexColorStride = 0;
		m_pTangents = NULL;
		m_nTangentCount = 0;
		m_nTangentStride = 0;

		m_nVertexOptions = sizeof(T) == sizeof(double) ? VO_VERTEX_DOUBLE : 0;
		m_nVertexOptions = m_nVertexOptions | VO_NORMALS | VO_DIFFUSE_COLOURS | VO_TEXTURE_COORDS;

		unsigned short i = 0;
		for (i = 0; i < SMSCN_MAX_TEXTURE_COORD_SETS; i++)
		{
			m_pTexCoords[i] = NULL;
			m_TexCoordStride[i] = 0;
			m_TexCoordCount[i] = 0;
			m_nTexDimensions[i] = 2;
			m_texCoordCompressConstant[i] = -1.0;
			m_minTexCoordValue[i][0] = m_minTexCoordValue[i][1] = m_minTexCoordValue[i][2] = m_minTexCoordValue[i][3] = 0.0;
		}

		m_nVertexAttCount = 0;
		m_vecVertexAttDataCount.clear();
		m_vecVertexAttDataDimension.clear();
		m_vecVertexAttDataType.clear();
		for (unsigned int i = 0; i < m_vecVertexAttData.size(); i++)
		{
			delete[] m_vecVertexAttData[i];
			m_vecVertexAttData[i] = NULL;
		}
		m_vecVertexAttData.clear();
	}

	template<typename T>
	VertexDataPackageTemplate<T>::~VertexDataPackageTemplate()
	{
		clear();
	}

	// float精度的顶点
	typedef VertexDataPackageTemplate<float> VertexDataPackage;

	// double精度的顶点
	typedef VertexDataPackageTemplate<double> VertexDataPackageExact;

	// 索引包
	struct S3MB_API IndexPackage
	{
		IndexPackage();
		~IndexPackage();
		// 索引数目
		unsigned int m_nIndexesCount;

		// 索引数组
		unsigned short* m_pIndexes;

		// 是否使用无符整型数据类型
		VertexIndexType m_enIndexType;

		// 是否使用索引
		bool m_bUseIndex;

		// 使用的Pass的名称数组
		vector<wstring> m_strPassName;

		// 数据绘制的方式
		OperationType m_OperationType;

		// 加载索引信息
		bool Load(MemoryStream& stream);
		void Save(MemoryStream& stream);
		void SetIndexNum(int nIndexNum);
	};

	class S3MB_API IDInfo
	{
	public:
		IDInfo();
		IDInfo(const IDInfo& other);
		IDInfo& operator = (const IDInfo& other);
		~IDInfo();

		unsigned int m_nID;
		// 下述两个vector不同时有值
		std::vector<int> m_arrInstanceIndex;
		std::vector<std::pair<int, int> > m_arrVertexColorOffsetAndCount;
	};

	// 骨架类
	class S3MB_API Skeleton
	{
	public:
		Skeleton();
		~Skeleton();

		// 计算包围盒
		void ComputerBoundingBox();

		// 构建InstanceInfo
		bool SetInstanceInfoAndIDInfo(std::vector<Matrix4d>& vecMats, std::vector<unsigned int>& vecIDs);

		// Fill IDInfo
		void AddIDInfo(IDInfo* pIDInfo);

		// 通过IDInfo拆分索引包
		void SplitIndexPackageByIDInfo();

	public:
		// 名字
		wstring m_strGeoName;

		// 模型顶点信息
		VertexDataPackage* m_pVertexDataPackage;

		// 索引信息
		vector<IndexPackage*> m_arrIndexPackage;

		// 是否是一个实例批次
		bool m_bInstanceBatch;

		// 材质名
		wstring m_strMaterialName;

		// 位置矩阵
		Matrix4d m_matLocalView;

		// 包围盒
		BoundingBox m_BoundingBox;

		// 方向包围盒
		OrientedBoundingBox m_OBB;

		// ID信息
		std::vector<IDInfo*> m_arrIDInfo;
	};
}
#endif