/*---------------------------------------------------------------------------
//                    _   _   ____   ____                    
//   Project         | | | | / ___) / ___)                   
//                   | | | || | __ | |                       
//                   | |_| || |_\ || \___                    
//                    \____/ \____| \ ___)     6.0           
//                                                           
//!  \file UGVertexDataPackageTemplate.h
//!  \brief 封装的用于渲染得数据交换类文件
//!  \details 文件详细信息。
//!  \author yangyang
//!  \attention
//!   Copyright (c) 1996-2010 SuperMap Software Co., Ltd. <br> 
//!   All Rights Reserved.
//!  \version 6.0
---------------------------------------------------------------------------*/

#ifndef __UGVERTEXDATAPACKAGETEMPLATE_H__
#define __UGVERTEXDATAPACKAGETEMPLATE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000	
#include "Base3D/UGPrerequisites3D.h"
#if (__GNUC__==4)
 #include <typeinfo>
#endif

namespace UGC
{

#define SIZE_PER_INSTANCE 17
#define SIZE_PER_DM_INSTANCE  29

//! \brief 不同结构之间的拷贝
template<class TVertexDataPackDes, class TVertexDataPackSrc>
void VertexDataPack_Copy(TVertexDataPackDes* pVertexDes, TVertexDataPackSrc* pVertexSrc)
{
	if(pVertexSrc->m_pVertices != NULL && pVertexSrc->m_nVerticesCount > 0)
	{
		pVertexDes->m_nVertexDimension = pVertexSrc->m_nVertexDimension;
		pVertexDes->SetVertexNum(pVertexSrc->m_nVerticesCount, pVertexSrc->m_nVertexStride);

		if(typeid(pVertexSrc->m_pVertices) == typeid(pVertexDes->m_pVertices))
		{
			UGint nSize = typeid(pVertexSrc->m_pVertices) == typeid(float) ? 4 : 8;
			memcpy(pVertexDes->m_pVertices, pVertexSrc->m_pVertices, 
				pVertexDes->m_nVerticesCount * pVertexDes->m_nVertexDimension * nSize);
		}
		else
		{
			UGint nValueCount = pVertexSrc->m_nVerticesCount * pVertexSrc->m_nVertexDimension;
			for (UGint i=0; i<nValueCount; i++)
			{
				*pVertexDes->m_pVertices ++ = *pVertexSrc->m_pVertices ++;
			}
			//danger! 位置移回去
			pVertexDes->m_pVertices -= nValueCount;
			pVertexSrc->m_pVertices -= nValueCount;
		}
	}
	else
	{
		if (pVertexDes->m_pVertices != NULL)
		{
			delete [] pVertexDes->m_pVertices;
			pVertexDes->m_pVertices = NULL;
		}
		pVertexDes->m_nVerticesCount = 0;
	}

	if(pVertexSrc->m_pNormals != NULL && pVertexSrc->m_nNormalCount > 0)
	{
		pVertexDes->m_nNormalDimension = pVertexSrc->m_nNormalDimension;
		pVertexDes->SetNormalNum(pVertexSrc->m_nNormalCount, pVertexSrc->m_nNormalStride);
		memcpy(pVertexDes->m_pNormals, pVertexSrc->m_pNormals, 
			pVertexDes->m_nNormalCount * pVertexDes->m_nNormalDimension * sizeof(UGfloat));
	}
	else
	{
		if (pVertexDes->m_pNormals != NULL)
		{
			delete [] pVertexDes->m_pNormals;
			pVertexDes->m_pNormals = NULL;
		}
		pVertexDes->m_nNormalCount = 0;
	}

	if(pVertexSrc->m_pVertexColor != NULL && pVertexSrc->m_nVertexColorCount > 0)
	{
		pVertexDes->SetColorNum(pVertexSrc->m_nVertexColorCount, pVertexSrc->m_nVertexColorStride);
		memcpy(pVertexDes->m_pVertexColor, pVertexSrc->m_pVertexColor, 
			pVertexDes->m_nVertexColorCount * sizeof(UGuint));
	}
	else
	{
		if (pVertexDes->m_pVertexColor != NULL)
		{
			delete [] pVertexDes->m_pVertexColor;
			pVertexDes->m_pVertexColor = NULL;
		}
		pVertexDes->m_nVertexColorCount = 0;
	}

	if(pVertexSrc->m_pSecondVertexColor != NULL && pVertexSrc->m_nSecondVertexColorCount > 0)
	{
		pVertexDes->SetSecondColorNum(pVertexSrc->m_nSecondVertexColorCount, pVertexSrc->m_nSecondVertexColorStride);
		memcpy(pVertexDes->m_pSecondVertexColor, pVertexSrc->m_pSecondVertexColor, 
			pVertexDes->m_nSecondVertexColorCount * sizeof(UGuint));
	}
	else
	{
		if(pVertexDes->m_pSecondVertexColor != NULL)
		{
			delete [] pVertexDes->m_pSecondVertexColor;
			pVertexDes->m_pSecondVertexColor = NULL;
		}
		pVertexDes->m_nSecondVertexColorCount = 0;
	}

	for(UGint i = 0; i < SMSCN_MAX_TEXTURE_COORD_SETS; i++)
	{
		if(pVertexSrc->m_pTexCoords[i] == NULL || pVertexSrc->m_TexCoordCount[i] < 1)
		{
			if (pVertexDes->m_pTexCoords[i] != NULL)
			{
				delete [] pVertexDes->m_pTexCoords[i];
				pVertexDes->m_pTexCoords[i] = NULL;
			}
			pVertexDes->m_TexCoordCount[i] = 0;
			continue;
		}

		pVertexDes->m_nTexDimensions[i] = pVertexSrc->m_nTexDimensions[i];
		pVertexDes->SetTexCoordsNum(i, pVertexSrc->m_TexCoordCount[i], pVertexSrc->m_TexCoordStride[i]);
		memcpy(pVertexDes->m_pTexCoords[i], pVertexSrc->m_pTexCoords[i], 
			pVertexDes->m_TexCoordCount[i] * pVertexDes->m_nTexDimensions[i] * sizeof(UGfloat));
	}
	pVertexDes->m_nVertexOptions = pVertexSrc->m_nVertexOptions;
 	pVertexDes->ResetVertexDoubleOptions();
}

enum VertexCompressOptions
{
	SVC_Vertex				=	1,				//顶点带压缩
	SVC_Normal				= 2,				//法线带压缩
	SVC_VertexColor		= 4,				//顶点颜色带压缩
	SVC_SecondColor		= 8,				//SecondColor带压缩
	SVC_TexutreCoord	= 16,				//纹理坐标带压缩
	SVC_TexutreCoordIsW = 32,  // 表示第一重纹理坐标存储顶点的W位
};

//! \brief 顶点数组
template<typename T, class VertexDeclaration>
class /*BASE3D_API */UGVertexDataPackageTemplate
{
	friend class UGVertexDeclaration;
public:
	//! \brief 数据绘制属性
	UGint m_nVertexOptions;

	//! \brief 数据压缩标记
	UGint m_nCompressOptions;

	//! \brief 顶点维数
	UGushort m_nVertexDimension;
	//! \brief 法线维数
	UGushort m_nNormalDimension;
	//! \brief 贴图坐标的维数
	UGushort m_nTexDimensions[SMSCN_MAX_TEXTURE_COORD_SETS];

	//! \brief 顶点数组
	T* m_pVertices;
	//! \brief 顶点数目
	UGuint m_nVerticesCount;
	//! \brief 顶点数组偏移
	UGushort m_nVertexStride;
	//! \brief 顶点各分量的最小值
	UGfloat m_minVerticesValue[4];
	//! \brief 顶点压缩归一化常量
	UGfloat m_fVertCompressConstant;


	//! \brief 向量数组
	UGfloat* m_pNormals;
	//! \brief 向量数目
	UGuint m_nNormalCount;
	//! \brief 向量数组偏移 
	UGushort m_nNormalStride; 

	//! \brief 颜色数组
	UGuint *m_pVertexColor;
	//! \brief 颜色数组大小
	UGuint m_nVertexColorCount;	
	//! \brief 颜色数组偏移
	UGushort m_nVertexColorStride;

	UGuint *m_pSecondVertexColor;
	UGuint m_nSecondVertexColorCount;	
	UGushort m_nSecondVertexColorStride;


	//! \brief 贴图坐标数据
	UGfloat* m_pTexCoords[SMSCN_MAX_TEXTURE_COORD_SETS];
	//! \brief 贴图坐标数目
	UGuint m_TexCoordCount[SMSCN_MAX_TEXTURE_COORD_SETS];
	//! \brief 每组贴图坐标的Stride
	UGushort m_TexCoordStride[SMSCN_MAX_TEXTURE_COORD_SETS];
	//! \brief 纹理坐标各分量的最小值
	UGfloat m_minTexCoordValue[SMSCN_MAX_TEXTURE_COORD_SETS][4];
	//! \brief 纹理坐标压缩归一化常量
	UGfloat m_texCoordCompressConstant[SMSCN_MAX_TEXTURE_COORD_SETS];

	UGVertexDataPackageTemplate()
	{
		Init();
	}

	UGVertexDataPackageTemplate(const UGVertexDataPackageTemplate& other)
	{
		Init();
		m_nCompressOptions = other.m_nCompressOptions;
		m_fVertCompressConstant = other.m_fVertCompressConstant;
		m_minVerticesValue[0] = other.m_minVerticesValue[0];
		m_minVerticesValue[1] = other.m_minVerticesValue[1];
		m_minVerticesValue[2] = other.m_minVerticesValue[2];
		m_minVerticesValue[3] = other.m_minVerticesValue[3];
		*this = other;
	}

	UGVertexDataPackageTemplate& operator = (const UGVertexDataPackageTemplate& other)
	{
		{
			if(other.m_pVertices != NULL && other.m_nVerticesCount > 0)
			{
				m_nVertexDimension = other.m_nVertexDimension;
				SetVertexNum(other.m_nVerticesCount, other.m_nVertexStride);
				if(m_nCompressOptions  & SVC_Vertex)
				{
					memcpy(m_pVertices, other.m_pVertices, m_nVerticesCount * m_nVertexDimension * sizeof(UGshort));
				}
				else
				{
					memcpy(m_pVertices, other.m_pVertices, m_nVerticesCount * m_nVertexDimension * sizeof(T));
				}
			}
			else
			{
				if (m_pVertices != NULL)
				{
					delete [] m_pVertices;
					m_pVertices = NULL;
				}
				m_nVerticesCount = 0;
			}

			if(other.m_pNormals != NULL && other.m_nNormalCount > 0)
			{
				m_nNormalDimension = other.m_nNormalDimension;
				SetNormalNum(other.m_nNormalCount, other.m_nNormalStride);
				memcpy(m_pNormals, other.m_pNormals, m_nNormalCount * m_nNormalDimension * sizeof(UGfloat));
			}
			else
			{
				if (m_pNormals != NULL)
				{
					delete [] m_pNormals;
					m_pNormals = NULL;
				}
				m_nNormalCount = 0;
			}

			if(other.m_pVertexColor != NULL && other.m_nVertexColorCount > 0)
			{
				SetColorNum(other.m_nVertexColorCount, other.m_nVertexColorStride);
				memcpy(m_pVertexColor, other.m_pVertexColor, m_nVertexColorCount * sizeof(UGuint));
			}
			else
			{
				if (m_pVertexColor != NULL)
				{
					delete [] m_pVertexColor;
					m_pVertexColor = NULL;
				}
				m_nVertexColorCount = 0;
			}

			if(other.m_pSecondVertexColor != NULL && other.m_nSecondVertexColorCount > 0)
			{
				SetSecondColorNum(other.m_nSecondVertexColorCount, other.m_nSecondVertexColorStride);
				memcpy(m_pSecondVertexColor, other.m_pSecondVertexColor, m_nSecondVertexColorCount * sizeof(UGuint));
			}
			else
			{
				if(m_pSecondVertexColor != NULL)
				{
					delete [] m_pSecondVertexColor;
					m_pSecondVertexColor = NULL;
				}
				m_nSecondVertexColorCount = 0;
			}

			for(UGint i = 0; i < SMSCN_MAX_TEXTURE_COORD_SETS; i++)
			{
				if(other.m_pTexCoords[i] == NULL || other.m_TexCoordCount[i] < 1)
				{
					if (m_pTexCoords[i] != NULL)
					{
						delete [] m_pTexCoords[i];
						m_pTexCoords[i] = NULL;
					}
					m_TexCoordCount[i] = 0;
					continue;
				}

				m_nTexDimensions[i] = other.m_nTexDimensions[i];
				SetTexCoordsNum(i, other.m_TexCoordCount[i], other.m_TexCoordStride[i]);
				memcpy(m_pTexCoords[i], other.m_pTexCoords[i], m_TexCoordCount[i] * m_nTexDimensions[i] * sizeof(UGfloat));
			}

			m_nVertexOptions = other.m_nVertexOptions;
			return *this;
		}
	}
		
	~UGVertexDataPackageTemplate()
	{
		Clear();
	}

	void Clear()
	{
		if (m_pVertices != NULL)
		{
			delete [] m_pVertices;
			m_pVertices = NULL;
		}
		m_nVerticesCount = 0;

		if (m_pNormals != NULL)
		{
			delete [] m_pNormals;
			m_pNormals = NULL;
		}
		m_nNormalCount = 0;

		if (m_pVertexColor != NULL)
		{
			delete [] m_pVertexColor;
			m_pVertexColor = NULL;
		}
		if(m_pSecondVertexColor != NULL)
		{
			delete [] m_pSecondVertexColor;
			m_pSecondVertexColor = NULL;
		}

		m_nVertexColorCount = 0;
		m_nSecondVertexColorCount = 0;

		UGushort i = 0;
		for (i = 0; i < SMSCN_MAX_TEXTURE_COORD_SETS; i++)
		{
			if (m_pTexCoords[i] != NULL)
			{
				delete [] m_pTexCoords[i];
				m_pTexCoords[i] = NULL;
			}
			m_TexCoordCount[i] = 0;
		}
	}

	//根据自己的顶点类型写上Option
	void ResetVertexDoubleOptions()
	{
		//先弄掉你
		m_nVertexOptions &= (~(VO_VERTEX_DOUBLE));

		if(typeid(T) == typeid(double))
		{
			m_nVertexOptions |= VO_VERTEX_DOUBLE;
		}
	}

	void SetVertexNum(UGuint nVertexCount, UGushort nVertexStride = 0)
	{
		m_nVerticesCount = nVertexCount;
		if(m_pVertices != NULL)
		{
			delete[] m_pVertices;
			m_pVertices =NULL;
		}
		m_pVertices = new T[nVertexCount * m_nVertexDimension];
		memset(m_pVertices, 0, sizeof(T) * nVertexCount * m_nVertexDimension);
		m_nVertexStride = nVertexStride;
	}
	void SetNormalNum(UGuint nNormalCount, UGushort nNormalStride = 0)
	{
		m_nNormalCount = nNormalCount;
		if(m_pNormals != NULL)
		{
			delete[] m_pNormals;
			m_pNormals =NULL;
		}
		m_nNormalStride = nNormalStride;
		if (nNormalCount == 0)
		{
			return;
		}

		m_pNormals = new UGfloat[nNormalCount * m_nNormalDimension];
		memset(m_pNormals, 0, sizeof(UGfloat) * nNormalCount * m_nNormalDimension);
	}
	void SetColorNum(UGuint nColorCount, UGushort nColorStride = 0)
	{
		m_nVertexColorCount = nColorCount;
		if(m_pVertexColor != NULL)
		{
			delete[] m_pVertexColor;
			m_pVertexColor =NULL;
		}
		m_pVertexColor = new UGuint[nColorCount];
		memset(m_pVertexColor, 0, sizeof(UGuint)*nColorCount);
		m_nVertexColorStride = nColorStride;
	}
	void SetSecondColorNum(UGuint nColorCount, UGushort nColorStride = 0)
	{
		m_nSecondVertexColorCount = nColorCount;


		if(m_pSecondVertexColor != NULL)
		{
			delete[] m_pSecondVertexColor;
			m_pSecondVertexColor =NULL;
		}
		m_pSecondVertexColor = new UGuint[nColorCount];
		memset(m_pSecondVertexColor, 0, sizeof(UGuint)*nColorCount);

		m_nSecondVertexColorStride = nColorStride;
	}
	UGbool SetTexCoordsNum(UGushort nTexUnit, UGuint nTexCoordCount, UGushort nTexCoordStride = 0)
	{
		if (nTexUnit >= SMSCN_MAX_TEXTURE_COORD_SETS)
		{
			return FALSE;
		}
		m_TexCoordCount[nTexUnit] = nTexCoordCount;

		if(m_pTexCoords[nTexUnit] != NULL)
		{
			delete[] m_pTexCoords[nTexUnit];
			m_pTexCoords[nTexUnit] =NULL;
		}
		m_pTexCoords[nTexUnit] = new UGfloat[nTexCoordCount * m_nTexDimensions[nTexUnit]];
		memset(m_pTexCoords[nTexUnit], 0, sizeof(UGfloat)*nTexCoordCount * m_nTexDimensions[nTexUnit]);
		m_TexCoordStride[nTexUnit] = nTexCoordStride;
		return TRUE;
	}

	UGint GetDataSize()
	{
		UGint nSize = sizeof(m_nVertexOptions);		
		nSize += sizeof(m_nVertexDimension);

		nSize += sizeof(m_nVerticesCount);
		// 顶点
		if (m_nVerticesCount > 0 && m_pVertices != NULL) 
		{
			nSize += sizeof(m_nVertexStride);
			nSize += sizeof(T) * m_nVerticesCount * m_nVertexDimension;	
		}

		nSize += sizeof(m_nNormalCount);
		//法向量
		if (m_nNormalCount > 0 && m_pNormals != NULL) 
		{
			nSize += sizeof(m_nNormalStride);
			nSize += sizeof(UGfloat) * m_nNormalCount * m_nNormalDimension;
		}

		nSize += sizeof(m_nVertexColorCount);
		//顶点颜色
		if (m_nVertexColorCount > 0 && m_pVertexColor != NULL) 
		{
			nSize += sizeof(m_nVertexColorStride);
			nSize += sizeof(UGuint) * m_nVertexColorCount;
		}

		nSize += sizeof(m_nSecondVertexColorCount);
		if (m_nSecondVertexColorCount > 0 && m_pSecondVertexColor != NULL) 
		{
			nSize += sizeof(m_nSecondVertexColorStride);
			nSize += sizeof(UGuint) * m_nSecondVertexColorCount;
		}

		UGint nTexCount = 0, i = 0;
		for(i = 0; i < SMSCN_MAX_TEXTURE_COORD_SETS; i++)
		{
			if(m_TexCoordCount[i] == 0)
			{
				break;
			}
			nTexCount++;
		}

		nSize += sizeof(nTexCount);
		for(i = 0; i < nTexCount; i++)
		{
			nSize += sizeof(m_nTexDimensions[i]);
			nSize += sizeof(m_TexCoordCount[i]);
			nSize += sizeof(m_TexCoordStride[i]);
			nSize += sizeof(UGfloat) * m_TexCoordCount[i] * m_nTexDimensions[i];
		}

		return nSize;
	}

	void Save(UGStream& stream,UGbool bAlign = false)
	{
		UGushort nDimension = m_nVertexDimension;
		if(m_nVertexDimension != m_nNormalDimension)
		{
			nDimension = m_nVertexDimension | (m_nNormalDimension << 8);
		}

		if(bAlign == false)
		{
			stream<<m_nVertexOptions;
			stream<<nDimension;

			stream<<m_nVerticesCount;
			// 顶点
			if(m_nVerticesCount > 0 && m_pVertices != NULL)
			{
				stream<<m_nVertexStride;
				stream.Save(m_pVertices, m_nVerticesCount * m_nVertexDimension);
			}

			stream<<m_nNormalCount;
			//法向量
			if (m_nNormalCount > 0 && m_pNormals != NULL) 
			{
				stream<<m_nNormalStride;
				stream.Save(m_pNormals, m_nNormalCount * m_nNormalDimension);
			}

			stream<<m_nVertexColorCount;
			//顶点颜色
			if (m_nVertexColorCount > 0 && m_pVertexColor != NULL) 
			{
				stream<<m_nVertexColorStride;
				stream.Save(m_pVertexColor, m_nVertexColorCount);
			}


			UGint nTexCount = 0, i = 0;
			for(i = 0; i < SMSCN_MAX_TEXTURE_COORD_SETS; i++)
			{
				if(m_TexCoordCount[i] == 0)
				{
					break;
				}
				nTexCount++;
			}

			stream<<nTexCount;
			for(i = 0; i < nTexCount; i++)
			{
				stream<<m_nTexDimensions[i];
				stream<<m_TexCoordCount[i];
				stream<<m_TexCoordStride[i];
				stream.Save(m_pTexCoords[i], m_TexCoordCount[i] * m_nTexDimensions[i]);
			}
		}
		else
		{
			UGuchar nByte = 0;
			stream<<m_nVertexOptions;

			stream<<nDimension;
			stream<<nByte;
			stream<<nByte;

			stream<<m_nVerticesCount;
			// 顶点
			if(m_nVerticesCount > 0 && m_pVertices != NULL)
			{
				stream<<m_nVertexStride;
				stream<<nByte;
				stream<<nByte;

				stream.Save(m_pVertices, m_nVerticesCount * m_nVertexDimension);
			}

			stream<<m_nNormalCount;
			//法向量
			if (m_nNormalCount > 0 && m_pNormals != NULL) 
			{
				stream<<m_nNormalStride;
				stream<<nByte;
				stream<<nByte;

				stream.Save(m_pNormals, m_nNormalCount * m_nNormalDimension);
			}

			stream<<m_nVertexColorCount;
			//顶点颜色
			if (m_nVertexColorCount > 0 && m_pVertexColor != NULL) 
			{
				stream<<m_nVertexColorStride;
				stream<<nByte;
				stream<<nByte;

				for(int j=0;j<m_nVertexColorCount;j++)
				{
					UGuint nColor = m_pVertexColor[j];
					UGfloat fR = UGREDVAL(nColor)/255.0;
					UGfloat fG = UGGREENVAL(nColor)/255.0;
					UGfloat fB = UGBLUEVAL(nColor)/255.0;
					UGfloat fA = UGALPHAVAL(nColor)/255.0;

					stream<<fR;
					stream<<fG;
					stream<<fB;
					stream<<fA;
				}
			}

			stream<<m_nSecondVertexColorCount;
			//顶点颜色
			if (m_nSecondVertexColorCount > 0 && m_pSecondVertexColor != NULL) 
			{
				for(UGint j=0;j<m_nSecondVertexColorCount;j++)
				{
					UGuint nColor = m_pSecondVertexColor[j];
					UGfloat fR = UGREDVAL(nColor)/255.0;
					UGfloat fG = UGGREENVAL(nColor)/255.0;
					UGfloat fB = UGBLUEVAL(nColor)/255.0;
					UGfloat fA = UGALPHAVAL(nColor)/255.0;

					stream<<fR;
					stream<<fG;
					stream<<fB;
					stream<<fA;				
				}
			}


			UGint nTexCount = 0, i = 0;
			for(i = 0; i < SMSCN_MAX_TEXTURE_COORD_SETS; i++)
			{
				if(m_TexCoordCount[i] == 0)
				{
					break;
				}
				nTexCount++;
			}

			stream<<nTexCount;
			for(i = 0; i < nTexCount; i++)
			{
				if(m_nTexDimensions[i] == SIZE_PER_INSTANCE)
				{
					UGushort nDimension = SIZE_PER_INSTANCE + 3;
					stream<<m_TexCoordCount[i];
					stream<<nDimension;
					stream<<m_TexCoordStride[i];

					UGfloat* pValue = m_pTexCoords[i];
					UGuint* pUint = (UGuint*)pValue;

					for (UGint kIndex =0;kIndex<m_TexCoordCount[i];kIndex++)
					{
						// 三个纹理坐标存储矩阵，一个纹理坐标存储选择ID，一个颜色存储选中时顶点颜色
						// 						stream<<pValue[kIndex*m_nTexDimensions[i]];
						// 						stream<<pValue[kIndex*m_nTexDimensions[i]+ 1];
						// 						stream<<pValue[kIndex*m_nTexDimensions[i]+ 2];
						// 						stream<<pValue[kIndex*m_nTexDimensions[i]+ 3];
						// 
						// 						stream<<pValue[kIndex*m_nTexDimensions[i]+ 4];
						// 						stream<<pValue[kIndex*m_nTexDimensions[i]+ 5];
						// 						stream<<pValue[kIndex*m_nTexDimensions[i]+ 6];
						// 						stream<<pValue[kIndex*m_nTexDimensions[i]+ 7];
						// 
						// 						stream<<pValue[kIndex*m_nTexDimensions[i]+ 8];
						// 						stream<<pValue[kIndex*m_nTexDimensions[i]+ 9];
						// 						stream<<pValue[kIndex*m_nTexDimensions[i]+ 10];
						// 						stream<<pValue[kIndex*m_nTexDimensions[i]+ 11];
						// 
						// 						stream<<pValue[kIndex*m_nTexDimensions[i]+ 12];
						// 						stream<<pValue[kIndex*m_nTexDimensions[i]+ 13];
						// 						stream<<pValue[kIndex*m_nTexDimensions[i]+ 14];
						// 						stream<<pValue[kIndex*m_nTexDimensions[i]+ 15];

						stream.Save(pValue+kIndex*SIZE_PER_INSTANCE,16);

						UGuint nSelectionID = pUint[SIZE_PER_INSTANCE * kIndex + 16];
						UGfloat fR = UGREDVAL(nSelectionID)/255.0;
						UGfloat fG = UGGREENVAL(nSelectionID)/255.0;
						UGfloat fB = UGBLUEVAL(nSelectionID)/255.0;
						UGfloat fA = UGALPHAVAL(nSelectionID)/255.0;
						stream<<fR;
						stream<<fG;
						stream<<fB;
						stream<<fA;				
					}

				}
				else if (m_nTexDimensions[i] == SIZE_PER_DM_INSTANCE)
				{
					UGushort nDimension = SIZE_PER_DM_INSTANCE + 6;
					stream<<m_TexCoordCount[i];
					stream<<nDimension;
					stream<<m_TexCoordStride[i];

					UGfloat* pValue = m_pTexCoords[i];
					UGuint* pUint = (UGuint*)pValue;

					for (UGint kIndex =0;kIndex<m_TexCoordCount[i];kIndex++)
					{
						// TextureCoord1(VET_FLOAT4): 第一个截面的矩阵第一行
						// TextureCoord2(VET_FLOAT4): 第一个截面的矩阵第二行
						// TextureCoord3(VET_FLOAT4): 第一个截面的矩阵第三行
						// TextureCoord4(VET_FLOAT4): 第二个截面的矩阵第一行
						// TextureCoord5(VET_FLOAT4): 第二个截面的矩阵第二行
						// TextureCoord6(VET_FLOAT4): 第二个截面的矩阵第三行
						// TextureCoord7(VET_FLOAT3): 前后截面的v方向纹理坐标
						// SecondColor(VET_COLOUR): 要素ID
						// SecondColor2(VET_COLOUR): 要素ID

						stream.Save(pValue+kIndex*SIZE_PER_DM_INSTANCE,27);

						UGuint nSelectionID = pUint[SIZE_PER_DM_INSTANCE * kIndex + 27];
						UGfloat fR = UGREDVAL(nSelectionID)/255.0;
						UGfloat fG = UGGREENVAL(nSelectionID)/255.0;
						UGfloat fB = UGBLUEVAL(nSelectionID)/255.0;
						UGfloat fA = UGALPHAVAL(nSelectionID)/255.0;
						stream<<fR;
						stream<<fG;
						stream<<fB;
						stream<<fA;	

						nSelectionID = pUint[SIZE_PER_DM_INSTANCE * kIndex + 28];
						fR = UGREDVAL(nSelectionID)/255.0;
						fG = UGGREENVAL(nSelectionID)/255.0;
						fB = UGBLUEVAL(nSelectionID)/255.0;
						fA = UGALPHAVAL(nSelectionID)/255.0;
						stream<<fR;
						stream<<fG;
						stream<<fB;
						stream<<fA;	

					}
				}
				else 
				{
					stream<<m_TexCoordCount[i];
					stream<<m_nTexDimensions[i];
					stream<<m_TexCoordStride[i];
					stream.Save(m_pTexCoords[i], m_TexCoordCount[i] * m_nTexDimensions[i]);
				}
			}
		}
	}

	UGbool Load(UGStream& stream,UGbool bAlign = false)
	{
		if(bAlign == false)
		{
			stream>>m_nVertexOptions;
			stream>>m_nVertexDimension;

			if(m_nVertexDimension > 4)
			{
				m_nNormalDimension = (m_nVertexDimension >> 8);
				m_nVertexDimension = 0x0F & m_nVertexDimension;
			}
			else
			{
				m_nNormalDimension = m_nVertexDimension;
			}

			UGuint nVerticesCount = 0;
			stream>>nVerticesCount;
			// 顶点
			if(nVerticesCount > 0)
			{
				m_nVerticesCount = nVerticesCount;
				stream>>m_nVertexStride;
				m_pVertices = new(std::nothrow) T[m_nVerticesCount * m_nVertexDimension];
				if(m_pVertices == NULL)
				{
					return FALSE;
				}
				stream.Load(m_pVertices, m_nVerticesCount * m_nVertexDimension);
			}

			UGuint nNormalCount = 0;
			stream>>nNormalCount;
			//法向量
			if (nNormalCount > 0) 
			{
				m_nNormalCount = nNormalCount;
				stream>>m_nNormalStride;
				m_pNormals = new(std::nothrow) UGfloat[m_nNormalCount * m_nNormalDimension];
				if(m_pNormals == NULL)
				{
					return FALSE;
				}
				stream.Load(m_pNormals,m_nNormalCount * m_nNormalDimension);
			}

			UGuint nColorCount = 0;
			stream>>nColorCount;
			//顶点颜色
			if (nColorCount > 0) 
			{
				m_nVertexColorCount = nColorCount;
				stream>>m_nVertexColorStride;
				m_pVertexColor = new(std::nothrow) UGuint[m_nVertexColorCount];
				if(m_pVertexColor == NULL)
				{
					return FALSE;
				}
				stream.Load(m_pVertexColor, m_nVertexColorCount);
			}

			UGint nTexCount = 0, i = 0;
			stream>>nTexCount;
			for(i = 0; i < nTexCount; i++)
			{
				stream>>m_nTexDimensions[i];
				stream>>m_TexCoordCount[i];
				stream>>m_TexCoordStride[i];
				m_pTexCoords[i] = new(std::nothrow) UGfloat[m_TexCoordCount[i] * m_nTexDimensions[i]];
				if(m_pTexCoords[i] == NULL)
				{
					return FALSE;
				}
				stream.Load(m_pTexCoords[i], m_TexCoordCount[i] * m_nTexDimensions[i]);
			}
		}
		else
		{
			UGuchar nByte = 0;
			stream>>m_nVertexOptions;

			stream>>m_nVertexDimension;
			stream>>nByte;
			stream>>nByte;

			if(m_nVertexDimension > 4)
			{
				m_nNormalDimension = (m_nVertexDimension >> 8);
				m_nVertexDimension = 0x0F & m_nVertexDimension;
			}
			else
			{
				m_nNormalDimension = m_nVertexDimension;
			}

			UGuint nVerticesCount = 0;
			stream>>nVerticesCount;
			// 顶点
			if(nVerticesCount > 0)
			{
				m_nVerticesCount = nVerticesCount;
				stream>>m_nVertexStride;
				stream>>nByte;
				stream>>nByte;

				m_pVertices = new(std::nothrow) T[m_nVerticesCount * m_nVertexDimension];
				if(m_pVertices == NULL)
				{
					return FALSE;
				}
				stream.Load(m_pVertices, m_nVerticesCount * m_nVertexDimension);
			}

			UGuint nNormalCount = 0;
			stream>>nNormalCount;
			//法向量
			if (nNormalCount > 0) 
			{
				m_nNormalCount = nNormalCount;
				stream>>m_nNormalStride;
				stream>>nByte;
				stream>>nByte;

				m_pNormals = new(std::nothrow) UGfloat[m_nNormalCount * m_nNormalDimension];
				if(m_pNormals == NULL)
				{
					return FALSE;
				}
				stream.Load(m_pNormals,m_nNormalCount * m_nNormalDimension);
			}

			UGuint nColorCount = 0;
			stream>>nColorCount;
			//顶点颜色
			if (nColorCount > 0) 
			{
				m_nVertexColorCount = nColorCount;
				stream>>m_nVertexColorStride;
				stream>>nByte;
				stream>>nByte;

				m_pVertexColor = new(std::nothrow) UGuint[m_nVertexColorCount];
				if(m_pVertexColor == NULL)
				{
					return FALSE;
				}

				for(int j=0;j<m_nVertexColorCount;j++)
				{
					UGfloat fR,fG,fB,fA;

					stream>>fR;
					stream>>fG;
					stream>>fB;
					stream>>fA;

					m_pVertexColor[j] = UGRGBA(fR*255,fG*255,fB*255,fA*255);
				}
			}

			UGuint nSecondVertexColorCount = 0;
			stream>>nSecondVertexColorCount;
			//顶点颜色
			if (nSecondVertexColorCount > 0) 
			{
				m_nSecondVertexColorCount = nSecondVertexColorCount;

				m_pSecondVertexColor = new(std::nothrow) UGuint[m_nSecondVertexColorCount];
				if(m_pSecondVertexColor == NULL)
				{
					return FALSE;
				}

				for(int j=0;j<m_nSecondVertexColorCount;j++)
				{
					UGfloat fR,fG,fB,fA;

					stream>>fR;
					stream>>fG;
					stream>>fB;
					stream>>fA;

					m_pSecondVertexColor[j] = UGRGBA(fR*255,fG*255,fB*255,fA*255);
				}
			}

			UGint nTexCount = 0, i = 0;
			stream>>nTexCount;
			for(i = 0; i < nTexCount; i++)
			{
				UGushort nDimension;

				stream>>m_TexCoordCount[i];
				stream>>nDimension;
				stream>>m_TexCoordStride[i];

				if (nDimension == SIZE_PER_INSTANCE + 3)
				{
					m_nTexDimensions[i] = SIZE_PER_INSTANCE;

					m_pTexCoords[i] = new(std::nothrow) UGfloat[m_TexCoordCount[i] * m_nTexDimensions[i]];
					if(m_pTexCoords[i] == NULL)
					{
						return FALSE;
					}
					
					UGfloat* pValue = m_pTexCoords[i];
					UGuint* pUint = (UGuint*)pValue;
					UGfloat fR,fG,fB,fA;
					for (UGint kIndex =0;kIndex<m_TexCoordCount[i];kIndex++)
					{
// 						stream>>pValue[kIndex*m_nTexDimensions[i]];
// 						stream>>pValue[kIndex*m_nTexDimensions[i]+ 1];
// 						stream>>pValue[kIndex*m_nTexDimensions[i]+ 2];
// 						stream>>pValue[kIndex*m_nTexDimensions[i]+ 3];
// 
// 						stream>>pValue[kIndex*m_nTexDimensions[i]+ 4];
// 						stream>>pValue[kIndex*m_nTexDimensions[i]+ 5];
// 						stream>>pValue[kIndex*m_nTexDimensions[i]+ 6];
// 						stream>>pValue[kIndex*m_nTexDimensions[i]+ 7];
// 
// 						stream>>pValue[kIndex*m_nTexDimensions[i]+ 8];
// 						stream>>pValue[kIndex*m_nTexDimensions[i]+ 9];
// 						stream>>pValue[kIndex*m_nTexDimensions[i]+ 10];
// 						stream>>pValue[kIndex*m_nTexDimensions[i]+ 11];
// 
// 						stream>>pValue[kIndex*m_nTexDimensions[i]+ 12];
// 						stream>>pValue[kIndex*m_nTexDimensions[i]+ 13];
// 						stream>>pValue[kIndex*m_nTexDimensions[i]+ 14];
// 						stream>>pValue[kIndex*m_nTexDimensions[i]+ 15];

						stream.Load(pValue+kIndex*SIZE_PER_INSTANCE,16);

						stream>>fR;
						stream>>fG;
						stream>>fB;
						stream>>fA;		

						UGuint nSelectionID = UGRGBA(fR*255,fG*255,fB*255,fA*255);		
						pUint[SIZE_PER_INSTANCE * kIndex + 16] = nSelectionID;
					}
				}
				else if (nDimension == SIZE_PER_DM_INSTANCE + 6)
				{
					m_nTexDimensions[i] = SIZE_PER_DM_INSTANCE;

					m_pTexCoords[i] = new(std::nothrow) UGfloat[m_TexCoordCount[i] * m_nTexDimensions[i]];
					if(m_pTexCoords[i] == NULL)
					{
						return FALSE;
					}

					UGfloat* pValue = m_pTexCoords[i];
					UGuint* pUint = (UGuint*)pValue;
					UGfloat fR,fG,fB,fA;

					if(m_pTexCoords[i] == NULL)
					{
						return FALSE;
					}
					for (UGint kIndex =0;kIndex<m_TexCoordCount[i];kIndex++)
					{
						stream.Load(pValue+kIndex*SIZE_PER_DM_INSTANCE,27);

						stream>>fR;
						stream>>fG;
						stream>>fB;
						stream>>fA;		

						UGuint nVertexColor = UGRGBA(fR*255,fG*255,fB*255,fA*255);		
						pUint[SIZE_PER_DM_INSTANCE * kIndex + 27] = nVertexColor;

						stream>>fR;
						stream>>fG;
						stream>>fB;
						stream>>fA;		

						UGuint nSelectionID = UGRGBA(fR*255,fG*255,fB*255,fA*255);		
						pUint[SIZE_PER_DM_INSTANCE * kIndex + 28] = nSelectionID;
					}
				}
				else
				{
					m_nTexDimensions[i] = nDimension;
					m_pTexCoords[i] = new(std::nothrow) UGfloat[m_TexCoordCount[i] * m_nTexDimensions[i]];
					if(m_pTexCoords[i] == NULL)
					{
						return FALSE;
					}
					stream.Load(m_pTexCoords[i], m_TexCoordCount[i] * m_nTexDimensions[i]);
				}
			}
		}

		return TRUE;
	}

	//! \brief 获取VertexDataPackage的顶点元信息
	VertexDeclaration GetVertexDeclaration()
	{
		return VertexDeclaration(this);
	}

	//! \brief 是否有指定压缩项
	UGbool HasCompressOptions(VertexCompressOptions enOptions)
	{
		return (m_nCompressOptions&enOptions) == enOptions;
	}

	//! \brief 数据是否带压缩
	UGbool IsCompressed()
	{
		return ( HasCompressOptions(SVC_Vertex) || HasCompressOptions(SVC_Normal) || 
			HasCompressOptions(SVC_VertexColor) || HasCompressOptions(SVC_SecondColor) ||
			HasCompressOptions(SVC_TexutreCoord) );
	}

private:
	void Init()
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
		m_pSecondVertexColor = NULL;
		m_nVertexColorCount = 0;
		m_nSecondVertexColorCount = 0;
		m_nVertexColorStride = 0;
		m_nSecondVertexColorStride = 0;

		m_nVertexOptions = sizeof(T)==sizeof(UGdouble) ? VO_VERTEX_DOUBLE : 0;
		m_nVertexOptions = m_nVertexOptions | VO_NORMALS | VO_DIFFUSE_COLOURS | VO_TEXTURE_COORDS;

		UGushort i = 0;
		for (i = 0; i < SMSCN_MAX_TEXTURE_COORD_SETS; i++)
		{
			m_pTexCoords[i] = NULL;
			m_TexCoordStride[i] = 0;
			m_TexCoordCount[i] = 0;
			m_nTexDimensions[i] = 2;
			m_texCoordCompressConstant[i] = -1.0;
			m_minTexCoordValue[i][0] = m_minTexCoordValue[i][1] = m_minTexCoordValue[i][2] = m_minTexCoordValue[i][3] = 0.0;
		}
	};
};

}

#endif
