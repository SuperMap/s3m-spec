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
		//glTF 1.0 版本
		std::vector<std::wstring> m_vecStrPos;
		std::vector<std::wstring> m_vecStrNormal;
		std::vector<std::wstring> m_vecStrMaterial;
		std::vector<std::wstring> m_vecStrColor;
		std::vector<std::wstring> m_vecStrTex;
		std::vector<std::wstring> m_vecStrIndices;
		std::vector<unsigned int> m_vecDrawType;

		//glTF 2.0版本
		std::vector<GLTFPrimitive> m_vecGLTFPrimitive;

		//扩展
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
		//! \brief 物体的基本颜色因子
		Vector4d m_vec4BaseColor;
		//! \brief 物体基本颜色的纹理在纹理数组中的序号
		int m_nBaseColorTextureIndex;
		//! \brief 物体基本颜色的纹理坐标在纹理坐标数组中的序号
		int m_nBaseColorTextureCoordIndex;
		//! \brief 金属度和粗糙性纹理在纹理数组中的序号(R通道存储金属度，G通道存储粗糙度)
		int m_nMetallicRoughnessTextureIndex;
		//! \brief 金属度和粗糙性纹理的纹理坐标在数组中的序号
		int m_nMetallicRoughnessTextureCoordIndex;
		//! \brief 控制金属性强弱的因子
		float m_fMetallicFactor;
		//! \brief 控制粗糙性强弱的因子
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
		//! \brief 控制自发光强度的因子
		Vector3d m_vec3EmissiveFactor;
		//! \brief 自发光纹理在纹理数组中的序号 RGB纹理
		int m_nEmissiveTextureIndex;
		//! \brief 自发光纹理的纹理坐标在纹理坐标数组中的序号
		int m_nEmissiveTextureCoordIndex;
		//! \brief 法线纹理在纹理数组中的序号，物体表面的凹凸细节 float格式RGB纹理
		int m_nNormalTextureIndex;
		//! \brief 法线纹理的纹理坐标在纹理坐标数组中的序号
		int m_nNormalTextureCoordIndex;
		//! \brief 法线纹理的缩放因子
		float m_fNormalTextureScale;
		//! \brief 遮挡图在纹理数组中的序号，用于物体表面凹凸性对光照的影响，比如缝隙处就暗 float灰度纹理
		int m_nOcclusionTextureIndex;
		//! \brief 遮挡图的纹理坐标在纹理坐标数组中的序号
		int m_nOcclusionTextureCoordIndex;
		//! \brief 遮挡图的缩放因子
		float m_fOcclusionTextureStrength;
		//! \brief AlphaMode
		AlphaMode	m_AlphaMode;
		//! \brief 当alphaMode为Mask时，着色器根据这个值和baseColor的Alpha值进行比较决定是否丢弃
		float	m_fAlphaCutoff;
		//! \brief 是否双面渲染
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
		//gltf version 2.0    如果纹理外挂，m_nBufferViewIndex不在使用gltf规则，将从-1 -2 - 3....-n
		int m_nBufferViewIndex;
		//--------------------------
		std::wstring m_strImageType;

		//先存纹理外挂相对路径，之后会转成绝对路径
		std::wstring m_strFile;

		//纹理外挂文件名
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
		std::wstring m_strFile;//可能外挂文件 
		std::wstring m_strEncodeBuffer;
		std::wstring m_strEncodeMode;
		unsigned char* m_pBuffer;
		bool m_bManageBuffer;
		GLTFBuffer()
		{
			m_nByteLength = 0;
			m_pBuffer = NULL;
			m_bManageBuffer = false;
		}
		~GLTFBuffer()
		{
			if (m_bManageBuffer && m_pBuffer != NULL)
			{
				delete m_pBuffer;
				m_pBuffer = NULL;
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
		//! \brief 属性文件名称
		std::wstring m_strAttributeFileName;
		//! \brief ID范围
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
			m_pParentNode = NULL;
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
}

#endif