//////////////////////////////////////////////////////////////////////////
//                    _   _   ____   ____                    
//   Project         | | | | / ___) / ___)                   
//                   | | | || | __ | |                       
//                   | |_| || |_\ || \___                    
//                    \____/ \____| \ ___)     6.0           
//                                                           
//!  \file UGMesh.h
//!  \brief Class 面结构体和网格对象类。
//!  \details 文件详细信息。
//!  \author malq zhangyl wangzhp。
//!  \attention 
//!   Copyright (c) 1996-2010 SuperMap Software Co., Ltd. <br> 
//!   All Rights Reserved.
//!  \version 6.0
//////////////////////////////////////////////////////////////////////////


#if !defined(AFX_UGMESH_H__E5567E39_00CB_4E7D_9FF6_835268B2D58B__INCLUDED_)
#define AFX_UGMESH_H__E5567E39_00CB_4E7D_9FF6_835268B2D58B__INCLUDED_

#include "Base3D/UGPrerequisites3D.h"
#include "Stream/UGFileStream.h"
#include "Base3D/UGBoundingSphere.h"

namespace UGC
{

//! \brief 网格对象。
class BASE3D_API UGMesh 
{

public:

	//! \brief 构造函数。
	UGMesh();
	
	//! \brief 带参数构造函数。
	//! \param strName 名称[in]。
	UGMesh(const UGString& strName);
	
	//! \brief 拷贝构造函数。
	UGMesh(const UGMesh& other);

	//! \brief 赋值函数。
	UGMesh& operator=(const UGMesh& other);

	//! \brief 析构函数。
	virtual ~UGMesh();	

	//! \brief 释放资源。
	void Release();	

	//! \brief 设置前面。
	//! \param enFrontFace 前面方式常量[in]。
	void SetFrontFace(UGint enFrontFace);	

	// 注意这个函数不改变计数，所以外面一定要改变计数
	
	//! \brief 建立几何数据。
	//! \param numVertice 顶点数目[in]。
	//! \param numNormal 向量数目[in]。
	//! \param numFace 面数目[in]。
	//! \param numTextCoord 纹理坐标数目[in]。
	void SetupGeometryData(UGint numVertice,UGint numNormal,UGint numFace,UGint numTextCoord );	

	//! \brief 网格伸缩。
	//! \param fScale 伸缩系数[in]。
	void Scale(UGfloat fScale);	
	
	//! \brief 调整顶点值。
	//! \param vecCenter 中心位置[in]。
	//! \param fScale 缩放系数[in]。
	void Unitize(const UGVector3d& vecCenter,UGfloat fScale);	
	
	//! \brief 计算顶点向量。
	void ComputerVertexNormals();
	
	//! \brief 从流文件中加载
	//! \param fStream 文件流[in]。
	UGbool Load(UGStream& fStream);

	//! \brief 存入流文件中
	//! \param fStream 文件流[in]。
	void Save(UGStream& fStream);

	//! \brief 获取数据存储时的大小
	//! \return 返回存储大小
	//! \remark 暂未考虑编码情况，与Save()一起维护
	//! \attention 字符串的长度要+4
	virtual UGint GetDataSize() const;

	//! \brief 射线与mesh相交。
	//! \param vecRayStart 射线起点[in]。
	//! \param vecRayEnd 射线的终点[in]。
	//! \return 返回相交点个数及相应参数。
	void PickingRayWithMesh(UGVector3d vecRayStart, UGVector3d vecRayEnd, UGArray<double>& arrT);

	//! \brief 获取多重纹理个数
	UGuint GetMultiTextureSet(){return m_nTexturesSet;}
	
	//! \brief 设置多重纹理个数
	void SetMultiTextureSet(UGuint nLevels);

	UGfloat* GetMultiTextureCoordsOf(UGuint nLevels);

	//! \breif 计算BoundingBox
	void CalculateBoundingBox(UGBoundingBox& boundingBox);

	//! \breif 将新模型转化为旧模型
	void Convert();

	//! \breif 旧模型转化为新模型
	void ConvertToSubMesh();

	//! \breif 计算Mesh的体积
	UGdouble GetMeshVol(const UGVector3d pnt);

public:

	//! \brief mesh的名称。
	UGString m_strName;
	
	//! \brief 材质ID。
	UGint  m_nMaterialID;

	//! \brief向量模式
	UGint m_enNormalMode;

	//! \brief 正面方向成员
	UGint m_enFrontFace;

	//! \brief 顶点数组
	UGfloat *m_pVertices;
	//! \brief 顶点数目
	UGuint m_nVerticesCount;
	//! \brief 顶点数组偏移
	UGushort m_nVertexStride;

	//! \brief 向量数组
	UGfloat* m_pNormals;
	//! \brief 向量数目
	UGint m_nNormalCount;
	//! \brief 向量数组偏移 
	UGushort m_nNormalStride; 

	//! \brief 纹理坐标数组
	UGfloat *m_pTexCoords;
	//! \brief 纹理坐标数目
	UGuint m_nTexCoordsCount;
	//! \brief 纹理坐标偏移
	UGushort m_nTexCoordStride;

	//! \brief 索引数组
	UGushort *m_pIndexes;
	
	//! \brief 索引数组的大小
	UGuint m_nIndexesCount;

	
	//! \brief 颜色数组
    UGuint *m_pDiffuseColor;
	//! \brief 颜色数组数目
	UGuint m_nDiffuseColorCount;
	//! \brief 颜色数组偏移
	UGushort m_nDiffuseStride;

	UGbool m_bUseSingleColor;

	//! \brief mesh的UGBoundingSphere
	UGBoundingSphere m_BoundingSphere;

	UGArray<std::pair<UGint/* m_nIndexOffset*/, UGint/* m_nSubMaterialID*/> > m_arrFaces;

	UGushort m_nIndexesStride;

	//! 是否使用无符整型数据类型
	VertexIndexType m_enIndexType;
private:

	//! \brief 多重纹理单元个数
	UGuint m_nTexturesSet;
};

}

#endif // !defined(AFX_UGMESH_H__E5567E39_00CB_4E7D_9FF6_835268B2D58B__INCLUDED_)

