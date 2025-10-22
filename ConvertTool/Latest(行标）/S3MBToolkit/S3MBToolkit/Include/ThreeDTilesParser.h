#if !defined(THREEDTILESPARSER_H__5D64F3B8_6A6C_4023_AB97_3F7733A44AB9__INCLUDED_)
#define THREEDTILESPARSER_H__5D64F3B8_6A6C_4023_AB97_3F7733A44AB9__INCLUDED_

#pragma once
#include "stdafx.h"
#include "BoundingSphere.h"
#include "Common.h"
#include "GLTFContent.h"
#include "Palette.h"
#include "Point3D.h"
#include "S3MBEnums.h"
#include "S3MBLayerInfos.h"
#include "Vector.h"

#include "document.h"
#include "istreamwrapper.h"
#include "prettywriter.h"
#include "stringbuffer.h"

#include <map>
#include <string>
#include <vector>

namespace S3MB
{
	struct BatchIDInfo
	{
		int m_nCount;
		int m_nOffset;
		BatchIDInfo()
		{
			m_nCount = 0;
			m_nOffset = 0;
		}
	};

	class STK_API ThreeDTilesParser
	{
	public:
		ThreeDTilesParser();
		~ThreeDTilesParser();

		void SetIsNEU(bool bNEU);
		bool GetIsNEU() const;

		void SetPosCenter(const Point3D& pos);
		const Point3D& GetPosCenter() const;

		const Point3D& GetPosExtract() const;

		const Matrix4d& GetGlobalMat() const;

		void SetGLTFNode(unsigned int i, GLTFTreeNode* pNode);
		const std::vector<GLTFTreeNode*>& GetGLTFNodes() const;

		void ResetFieldInfos();
		const S3MBFieldInfos& GetFieldInfos() const;

		void SetBatchIDNum(unsigned int nNum);
		unsigned int GetBatchIDNum() const;

		void AddFeature(Feature* pFeature);
		const std::vector<Feature*>& GetFeatures() const;
		void ClearFeatures();

		bool GetIsAddMode() const;

		bool GetIsDistanceChange() const;

		bool GetIsValidPosition() const;

		void SetTextureCompressType(TextureCompressType nType);
		TextureCompressType GetTextureCompressType() const;

		void SetBufferSize(unsigned int nSize);
		unsigned int GetBufferSize() const;

		const I3DMIDInfo& GetI3DMInfo() const;

		void AddAttributeIndexInfo(const std::wstring& strKey, GLTFAttributeIndexInfo& info);
		const std::map<std::wstring, std::vector<GLTFAttributeIndexInfo> >& GetAttributeIndexInfos() const;

		//! \brief ����Tileset
		bool ParseTileset(std::wstring strPath, GLTFTreeNode* pParentNode, int nLod, Matrix4d matParent);
		//! \brief ����ת������
		void ComputeTransMat();
		//! \brief ����ʵ������Ϣ
		bool ParseInstanceInfo(rapidjson::Document& doc, unsigned char* buffer);
		void ResetInstanceInfo();
		// ��GLTF�õ��������ݰ���������תԲ��任
		void ConvertPoint(GLTFTreeNode* pNode, float* pPosData, const unsigned int nVertexCount, const unsigned int nPosDim, const Matrix4d matLocal, const Point3D& pntCenter);

		void Clear();

		GLTFTileInfos_1* ParseGLTF_V1(unsigned char*& pBuffer, unsigned int jsonLength, std::wstring strOutputDir);
		GLTFTileInfos_2* ParseGLTF(GLTFTreeNode* pNode, unsigned char*& buffer, unsigned int jsonLength, std::wstring strOutDir);

	private:
		//! \brief ����root
		bool ParseRoot(rapidjson::Document& doc, std::wstring strDir, GLTFTreeNode* pParentNode, int nLod, Matrix4d matParent);
		bool ParseInnerRoot(rapidjson::Value& doc, std::wstring strDir, GLTFTreeNode* pParentNode, int nLod, Matrix4d matParent);
		bool ParseChildren(rapidjson::Value& value, std::wstring strDir, GLTFTreeNode* pParentNode, int nLod, Matrix4d matParent);

		//! \brief �ݹ����ת������
		void RecurComputerRelatMat(GLTFTreeNode* pNode);
		void ProcessTextureCompressType(GLTFImage gltfImage);
		void DealGLTFFilePath(GLTFImage& gltfImage, std::wstring strFilePath);
		Vector3d GetDecodeNormal(unsigned short normalX, unsigned short normalY);

	private:
		// �û�ѡ���3dtiles����������orԲ��
		bool m_bIsNEU;
		//! \brief ȫ���ڵ�
		std::vector<GLTFTreeNode*> m_vecGLTFNodes;
		//! \brief ���ڵ������
		int m_nRootCount;
		//�Ƿ��в��������
		bool m_bValidPosition;
		//! \brief ����������
		AxisUpType m_nAxisUpType;
		//! \brief ��γ���µĲ����
		Point3D m_posCenter;
		Point3D m_posExtract;
		//! \brief ���ݵİ�Χ��
		BoundingSphere m_boundingSphere;
		//! \brief ����
		std::map<std::wstring, int> m_mapFieldIndex;
		std::vector<S3MBFieldInfo> m_vecFieldInfo;
		S3MBFieldInfos m_fieldInfos;
		std::vector<Feature*> m_vecFeature;
		// �滻����׷��ģʽ
		bool m_bAddMode;
		//! \brief �ⲿjson����(�����)
		Matrix4d m_matGlobalMat;
		Matrix4d m_matENUWorldView;
		Matrix4d m_matSphereWorldView;
		// Normal or Batch
		bool m_bIsNormal;
		//! \brief lod����
		int m_nLodNum;
		//! \brief �Ƿ��ǵ���
		bool m_isPnts;
		std::wstring m_strExt;
		//�л�ģʽ
		bool m_bIsDistanceChange;
		//�ڵ����
		unsigned int m_nTotalNum;
		//! \brief �Ѵ����BatchID�ĸ���
		unsigned int m_nBatchIDNum;
		//����ѹ��ö��
		TextureCompressType m_textureCompressType;

		unsigned int m_nBufferSize;

		I3DMIDInfo m_i3dmInfo;

		std::map<std::wstring, std::vector<GLTFAttributeIndexInfo> > m_mapAttributeIndexInfos;
	};
}

#endif