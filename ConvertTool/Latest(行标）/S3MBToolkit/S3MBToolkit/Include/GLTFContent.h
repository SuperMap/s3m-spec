#if !defined(GLTFCONTENT_H__F841011E_24BA_4F84_BD19_8E2A762C5504__INCLUDED_)
#define GLTFCONTENT_H__F841011E_24BA_4F84_BD19_8E2A762C5504__INCLUDED_

#pragma once
#include "stdafx.h"
#include "BoundingSphere.h"
#include "Palette.h"
#include "Vector.h"

#include <map>
#include <vector>

namespace S3MB
{
	struct GLBHeader
	{
		unsigned int m_nMagic;
		unsigned int m_nVersion;
		unsigned int m_nLength;

		GLBHeader()
		{
			m_nMagic = 0;
			m_nVersion = 0;
			m_nLength = 0;
		}
	};

	struct GLBChunk
	{
		unsigned int m_nLength;
		unsigned int m_nType;
		unsigned char* m_pData;

		GLBChunk()
		{
			m_nLength = 0;
			m_nType = 0;
			m_pData = nullptr;
		}

		unsigned char* Give()
		{
			unsigned char* pData = m_pData;
			m_pData = nullptr;
			return pData;
		}

		~GLBChunk()
		{
			m_nLength = 0;
			m_nType = 0;
			delete[] m_pData;
			m_pData = nullptr;
		}
	};

	struct GLTFAttributes
	{
		int m_nBatchIds;
		int m_nPosIndex;
		int m_nNormalIndex;
		int m_nColorIndex;
		int m_nTex1Index;
		int m_nTex2Index;
		GLTFAttributes()
		{
			m_nBatchIds = -1;
			m_nPosIndex = -1;
			m_nNormalIndex = -1;
			m_nColorIndex = -1;
			m_nTex1Index = -1;
			m_nTex2Index = -1;
		}
	};

	struct GLTFDraco
	{
		int m_nPosAttributeIndex;
		int m_nNormalAttributeIndex;
		int m_nColorAttributeIndex;
		int m_nTex1AttributeIndex;
		int m_nTex2AttributeIndex;
		int m_nSecondColorIndex;
		int m_nBufferViewIndex;
		GLTFDraco()
		{
			m_nPosAttributeIndex = -1;
			m_nNormalAttributeIndex = -1;
			m_nColorAttributeIndex = -1;
			m_nTex1AttributeIndex = -1;
			m_nTex2AttributeIndex = -1;
			m_nSecondColorIndex = -1;
			m_nBufferViewIndex = -1;
		}
	};

	struct GLTFPrimitive
	{
		GLTFAttributes m_gltfAttributes;
		GLTFDraco m_gltfDraco;
		int m_nIndices;
		int m_nDrawType;
		int m_nMaterialIndex;
		GLTFPrimitive()
		{
			m_nIndices = -1;
			m_nDrawType = -1;
			m_nMaterialIndex = -1;
		}
	};

	struct GLTFMesh
	{
		//glTF 1.0 �汾
		std::vector<std::wstring> m_vecStrPos;
		std::vector<std::wstring> m_vecStrNormal;
		std::vector<std::wstring> m_vecStrMaterial;
		std::vector<std::wstring> m_vecStrColor;
		std::vector<std::wstring> m_vecStrTex;
		std::vector<std::wstring> m_vecStrIndices;
		std::vector<unsigned int> m_vecDrawType;

		//glTF 2.0�汾
		std::vector<GLTFPrimitive> m_vecGLTFPrimitive;

		//��չ
		std::map<std::wstring, std::wstring> m_mapExtensionsUsed;

		GLTFMesh()
		{
		}
	};

	struct GLTFAccessor
	{
		//glTF version 1.0
		std::wstring m_strBufferViewName;
		//glTF version 2.0
		int m_nBufferViewIndex;
		//----------------------
		unsigned int m_nByteOffset;
		unsigned int m_nCount;
		std::wstring m_strType;
		unsigned int m_nComponentType;

		Vector3d m_vMin;
		Vector3d m_vMax;

		GLTFAccessor()
		{
			m_nBufferViewIndex = -1;
			m_nByteOffset = 0;
			m_nCount = 0;
			m_nComponentType = -1;
		}
	};

	struct GLTFPBR
	{
		//! \brief ����Ļ�����ɫ����
		Vector4d m_vec4BaseColor;
		//! \brief ���������ɫ�����������������е����
		int m_nBaseColorTextureIndex;
		//! \brief ���������ɫ�������������������������е����
		int m_nBaseColorTextureCoordIndex;
		//! \brief �����Ⱥʹֲ������������������е����(Rͨ���洢�����ȣ�Gͨ���洢�ֲڶ�)
		int m_nMetallicRoughnessTextureIndex;
		//! \brief �����Ⱥʹֲ�����������������������е����
		int m_nMetallicRoughnessTextureCoordIndex;
		//! \brief ���ƽ�����ǿ��������
		float m_fMetallicFactor;
		//! \brief ���ƴֲ���ǿ��������
		float m_fRoughnessFactor;

		GLTFPBR()
		{
			m_fMetallicFactor = 0.0f;
			m_fRoughnessFactor = 0.0f;
			m_nBaseColorTextureIndex = m_nMetallicRoughnessTextureIndex = -1;
			m_vec4BaseColor = Vector4d(1, 1, 1, 1);
			m_fMetallicFactor = 1;
			m_fRoughnessFactor = 1;
		}
	};

	struct GLTFMaterial
	{
		//glTF version 1.0
		std::wstring m_strTextureName;
		//glTF version 2.0
		int m_nTextureIndex;
		//------------------------
		ColorValue m_colorV;

		GLTFPBR m_PBR;
		//! \brief �����Է���ǿ�ȵ�����
		Vector3d m_vec3EmissiveFactor;
		//! \brief �Է������������������е���� RGB����
		int m_nEmissiveTextureIndex;
		//! \brief �Է�������������������������������е����
		int m_nEmissiveTextureCoordIndex;
		//! \brief �������������������е���ţ��������İ�͹ϸ�� float��ʽRGB����
		int m_nNormalTextureIndex;
		//! \brief ��������������������������������е����
		int m_nNormalTextureCoordIndex;
		//! \brief �����������������
		float m_fNormalTextureScale;
		//! \brief �ڵ�ͼ�����������е���ţ�����������氼͹�ԶԹ��յ�Ӱ�죬�����϶���Ͱ� float�Ҷ�����
		int m_nOcclusionTextureIndex;
		//! \brief �ڵ�ͼ�������������������������е����
		int m_nOcclusionTextureCoordIndex;
		//! \brief �ڵ�ͼ����������
		float m_fOcclusionTextureStrength;
		//! \brief AlphaMode
		AlphaMode	m_AlphaMode;
		//! \brief ��alphaModeΪMaskʱ����ɫ���������ֵ��baseColor��Alphaֵ���бȽϾ����Ƿ���
		float	m_fAlphaCutoff;
		//! \brief �Ƿ�˫����Ⱦ
		bool m_bDoubleSided;

		float        m_fMetallicFactor;
		float        m_fRoughnessFactor;
		std::wstring       m_strName;

		GLTFMaterial()
		{
			m_nTextureIndex = -1;
			m_nNormalTextureIndex = -1;
			m_nOcclusionTextureIndex = -1;
			m_nEmissiveTextureIndex = -1;
			m_AlphaMode = PBRAM_BLEND;
			m_fOcclusionTextureStrength = 1;
			m_fNormalTextureScale = 1;
			m_fAlphaCutoff = 0;
			m_bDoubleSided = false;
			m_fMetallicFactor = 0.0f;
			m_fRoughnessFactor = 0.0f;
		}
	};

	struct GLTFSampler
	{
		int m_nWrapS;
		int m_nWrapT;

		GLTFSampler()
		{
			m_nWrapS = m_nWrapT = 10497;
		}
	};

	struct GLTFTexture
	{
		//glTF version 1.0
		std::wstring m_strImageName;
		//glTF version 2.0
		int m_nImageIndex;

		GLTFSampler m_GLTFSampler;

		GLTFTexture()
		{
			m_nImageIndex = -1;
		}
	};


	struct GLTFImage
	{
		//glTF version 1.0
		std::wstring m_strBufferViewName;
		//gltf version 2.0    ���������ң�m_nBufferViewIndex����ʹ��gltf���򣬽���-1 -2 - 3....-n
		int m_nBufferViewIndex;
		//--------------------------
		std::wstring m_strImageType;

		//�ȴ�����������·����֮���ת�ɾ���·��
		std::wstring m_strFile;

		//��������ļ���
		std::wstring m_strFileName;

		bool	 m_bCreatImage;

		GLTFImage()
		{
			m_nBufferViewIndex = -1;
			m_strImageType = U("jpg");
			m_bCreatImage = false;
		}
	};

	struct GLTFBufferView
	{
		//glTF version 1.0
		std::wstring m_strBufferName;
		//glTF version 2.0
		int m_nBufferIndex;
		//----------------------
		unsigned int   m_nByteOffset;
		unsigned int   m_nByteLength;
		unsigned int   m_nByteStride;
		int    m_nTarget;
		std::wstring m_strName;

		GLTFBufferView()
		{
			m_nBufferIndex = -1;
			m_nByteOffset = 0;
			m_nByteLength = 0;
			m_nByteStride = 0;
			m_nTarget = -1;
		}
	};

	struct GLTFBuffer
	{
		unsigned int m_nByteLength;
		std::wstring m_strFile;//��������ļ� 
		std::wstring m_strEncodeBuffer;
		std::wstring m_strEncodeMode;
		unsigned char* m_pBuffer;
		bool m_bManageBuffer;
		GLTFBuffer()
		{
			m_nByteLength = 0;
			m_pBuffer = nullptr;
			m_bManageBuffer = false;
		}
		~GLTFBuffer()
		{
			if (m_bManageBuffer && m_pBuffer != nullptr)
			{
				delete[] m_pBuffer;
				m_pBuffer = nullptr;
			}
		}
	};

	struct I3DMIDInfo
	{
		unsigned int m_nIdLen;
		std::vector<unsigned int> m_vecId;
		std::vector<Matrix4d> m_vecMat;
	};

	struct GLTFAttributeIndexInfo
	{
		//! \brief �����ļ�����
		std::wstring m_strAttributeFileName;
		//! \brief ID��Χ
		std::pair<int, int> m_pairIDRange;
	};

	struct GLTFTreeNode
	{
		std::wstring			m_strFile;
		std::wstring			m_strParentDir;
		BoundingSphere			m_boundSphere;
		int						m_nLod;
		GLTFTreeNode*			m_pParentNode;
		std::vector<GLTFTreeNode*>	m_vecChildNode;
		Matrix4d				m_RelatMat;
		Matrix4d				m_RootMat;
		double					m_dGeometryError;
		bool					m_bFileFind;

		std::pair<unsigned int, unsigned int> m_pairIDRange;

		GLTFTreeNode()
		{
			m_nLod = 0;
			m_pParentNode = nullptr;
			m_RelatMat = Matrix4d::IDENTITY;
			m_RootMat = Matrix4d::IDENTITY;
			m_dGeometryError = 0;
			m_bFileFind = false;
			m_pairIDRange = std::make_pair(0, 0);
		}

		~GLTFTreeNode()
		{
		}
	};

	struct GLTFTileInfos_1
	{
		std::map<std::wstring, Matrix4d> m_mapMeshTOLocalView;
		std::map<std::wstring, std::vector<std::wstring> > m_mapMeshSet;
		std::map<std::wstring, GLTFMesh> m_mapMeshs;
		std::map<std::wstring, GLTFAccessor> m_mapAccessors;
		std::map<std::wstring, GLTFBufferView> m_mapBufferViews;
		std::map<std::wstring, GLTFBuffer> m_mapBuffers;
		std::map<std::wstring, GLTFMaterial> m_mapMaterials;
		std::map<std::wstring, GLTFTexture> m_mapTextures;
		std::map<std::wstring, GLTFImage> m_mapImages;
		// ����������
		AxisUpType m_nAxisUpType = AxisUpType::Y_UP;
		Vector3d m_vCesuimRTC = Vector3d();
	};

	struct GLTFTileInfos_2
	{
		std::map<unsigned int, Matrix4d> m_mapMeshToLocalView;
		std::map<unsigned int, std::vector<unsigned int> > m_mapMeshSet;
		std::map<unsigned int, GLTFMesh> m_mapMeshs;
		std::map<unsigned int, GLTFAccessor> m_mapAccessors;
		std::map<unsigned int, GLTFBufferView> m_mapBufferViews;
		std::map<unsigned int, GLTFBuffer> m_mapBuffers;
		std::map<unsigned int, std::vector<GLTFMaterial> > m_mapMaterials;
		std::map<unsigned int, GLTFTexture> m_mapTextures;
		std::map<unsigned int, GLTFImage> m_mapImages;
		// ����������
		AxisUpType m_nAxisUpType;
		Vector3d m_vCesuimRTC = Vector3d();
	};
}

#endif