/*---------------------------------------------------------------------------
//                    _   _   ____   ____                    
//   Project         | | | | / ___) / ___)                   
//                   | | | || | __ | |                       
//                   | |_| || |_\ || \___                    
//                    \____/ \____| \ ___)     6.0           
//                                                           
//!  \file UGRenderOperationGroup.h
//!  \brief 封装的用于渲染得数据交换类文件
//!  \details 文件详细信息。
//!  \author yangyang
//!  \attention
//!   Copyright (c) 1996-2010 SuperMap Software Co., Ltd. <br> 
//!   All Rights Reserved.
//!  \version 6.0
---------------------------------------------------------------------------*/

#ifndef __RENDEROPERATIONGROUP_H__
#define __RENDEROPERATIONGROUP_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000	
#include "Base3D/UGPrerequisites3D.h"
#include "Base3D/UGVector3d.h"
#include "Base3D/UGMatrix4d.h"
#include "Base3D/UGBoundingSphere.h"
#include "Base3D/UGMaterial3D.h"
#include "Base3D/UGVertexDataPackageTemplate.h"

namespace UGC
{
	class UGVertexDeclaration;

	typedef UGVertexDataPackageTemplate<UGfloat, UGVertexDeclaration> UGVertexDataPackage;

	//! \brief double精度的顶点
	typedef UGVertexDataPackageTemplate<UGdouble, UGVertexDeclaration> UGVertexDataPackageExact;
// 	class UGVertexDataPackageExact: public UGVertexDataPackageTemplate<UGdouble, UGVertexDeclaration>
// 	{
// 	public:
// 		UGVertexDataPackageExact()
// 		{
// 			m_nVertexOptions |= VO_VERTEX_DOUBLE;
// 		}
// 	};

	//! \brief 描述VertexDataPacke顶点结构的元信息
	class BASE3D_API UGVertexDeclaration
	{
		template<typename T, class VertexDeclaration>
		friend class UGVertexDataPackageTemplate;
	public:	
		//! \brief 重载==
		UGbool operator== (const UGVertexDeclaration& other) const;

		//! \brief 重载!=
		UGbool operator!= (const UGVertexDeclaration& other) const;

		//! \brief 获取元信息对应的Hash值，用来分组
		UGint GetHashCode();
	protected:
		//! \brief 构造函数
		UGVertexDeclaration();
		UGVertexDeclaration(UGVertexDataPackage* pVertexDataPackage);
		UGVertexDeclaration(const UGVertexDeclaration& other);

		//! \brief 根据UGVertexDataPackage构造元定义
		void MakeWith(UGVertexDataPackage* pVertexDataPackage);

	private:
		//! \brief 顶点维数
		UGushort m_nVertexDimension;
		//! \brief 贴图坐标的维数
		UGArray<UGushort> m_arrTexDimensions;
		//! \brief 是否有法线坐标
		UGbool m_bHasNormal;
		//! \brief 是否有顶点颜色
		UGbool m_bHasColor;
		//! \brief 是否有副顶点颜色
		UGbool m_bHasSecondColor;
	};

	//! \brief 顶点索引数组
	struct BASE3D_API UGIndexPackage
	{
		//! \brief 索引数目
		UGuint m_nIndexesCount;
		//! \brief 索引数组
		UGushort *m_pIndexes;
		//! 是否使用无符整型数据类型
		VertexIndexType m_enIndexType;
		//! 是否使用索引
		UGbool m_bUseIndex;
		//! \brief 使用的UGPass的名称数组
		UGArray<UGString> m_strPassName;
		//! 数据绘制的方式
		OperationType m_OperationType;

		UGIndexPackage();
		UGIndexPackage(const UGIndexPackage& other);
		UGIndexPackage& operator = (const UGIndexPackage& other);
		~UGIndexPackage();
		void SetIndexNum(UGint nIndexNum);

		UGint GetDataSize();

		void Save(UGStream& stream,UGbool bAlign = false);

		UGbool Load(UGStream& stream,UGbool bAlign = false);

		//! \brief S3MB使用：对齐，且存储pass字符串
		void Save2(UGStream& stream);
		UGbool Load2(UGStream& stream);

		//! \brief 尝试将32位索引转换为16位索引
		void TransformTo16BitIndex();

		//! \brief 是否带属性
		UGbool HasAtt();

		//! \brief 获取顶点索引的个数
		UGint GetVIndexCount();

		//! \brief 获取三角形个数
		UGint GetTriangelCount();
	};

	struct BASE3D_API UGSubObjectInfo
	{
		UGuint m_nID;
		UGuint m_nVertexCount;
		UGBoundingBox m_Boundingbox;
		UGbool m_bVisible;
		std::map<int,int> m_mapSubIndex;
		std::vector<int> m_arrIndexStart;
		std::vector<int> m_arrOffset;
		// 记录文字的矩形框位置
		std::vector<UGRect2D> m_arrTextRect;

		UGSubObjectInfo()
			: m_nID(0)
			, m_bVisible(TRUE)
			, m_nVertexCount(0)
		{
		}

		UGint GetDataSize();

		void Save(UGStream& stream);

		UGbool Load(UGStream& stream);
	};


	class BASE3D_API UGRenderOperationGeometry
	{
	public:
		UGRenderOperationGeometry();
		UGRenderOperationGeometry(const UGRenderOperationGeometry& other);
		UGRenderOperationGeometry& operator = (const UGRenderOperationGeometry& other);
		virtual ~UGRenderOperationGeometry();

		void ComputerBoundingBox();

		//! \brief 模型顶点信息
		UGVertexDataPackage* m_pVertexDataPackage;

		//! \brief 索引信息
		UGArray<UGIndexPackage*> m_arrIndexPackage;

		//! \brief 材质名
		UGString m_strMaterialName;

		//! \brief 名字
		UGString m_strGeoName;

		//! \brief 包围盒
		UGBoundingBox m_BoundingBox;

		//! \brief 位置矩阵
		UGMatrix4d m_matWorldView;

		//! \brief 索引次数
		UGint m_nUseIndex;

		//! \brief 是否是一个实例批次
		UGbool m_bInstanceBatch;

		//! \brief 法线是否需要压缩
		UGbool m_bNormalDecode;
	};

	class UGRenderOperationGroup;
	class UGRenderOperationGeode;
	class BASE3D_API UGRenderOperationNode
	{
	public:
		struct TextureDataInfo
		{
			UGTextureData* m_pTextureData;

			UGbool m_bMipmap;

			UGint m_nLevel;

			TextureDataInfo()
			{
				m_pTextureData = NULL;
				m_bMipmap = FALSE;
				m_nLevel = 0;
			}

			TextureDataInfo(const TextureDataInfo& other)
			{
				m_pTextureData = NULL;
				*this = other;
			}

			TextureDataInfo& operator = (const TextureDataInfo& other)
			{
				if(m_pTextureData != NULL)
				{
					delete m_pTextureData;
					m_pTextureData = NULL;
				}

				if(other.m_pTextureData != NULL)
				{
					m_pTextureData = new UGTextureData(*other.m_pTextureData);
				}

				m_bMipmap = other.m_bMipmap;
				m_nLevel = other.m_nLevel;
				return *this;
			}

			~TextureDataInfo()
			{
				if(m_pTextureData != NULL)
				{
					delete m_pTextureData;
					m_pTextureData = NULL;
				}
			}
		};

		UGRenderOperationNode();
		virtual ~UGRenderOperationNode();

		//! \brief 如果是Group对象就返回this，否则NULL
		virtual UGRenderOperationGroup* AsGroup() {return NULL;};

		//! \brief 如果是Geode对象就返回this，否则NULL
		virtual UGRenderOperationGeode* AsGeode() { return NULL; };

		//! \brief 类名
		virtual UGString ClassName() { return _U("Node"); }

		//! \brief 名字
		void SetName(UGString strName) { m_strName = strName; };

		//! \brief 获得名字
		UGString GetName() { return m_strName; };

		//! \brief 获得包围球
		UGBoundingSphere GetBoudingSphere() { return m_BoundingSphere; };

		//! \brief 设置包围球
		void SetBoudingSphere(UGVector3d vecCenter, UGdouble dRadius) { 
			m_BoundingSphere.m_center = vecCenter; 
			m_BoundingSphere.m_radius = dRadius; };

		//! \brief 设置位置
		void SetPosition(UGMatrix4d mat) { m_matLocalView = mat; };

		//! \brief 获得位置
		UGMatrix4d GetPosition() { return m_matLocalView; };

		//! \brief 材质是否存在
		UGbool IsExistToMaterial3D(UGString strMaterialName);

		//! \brief 纹理是否存在
		UGbool IsExistToTexData(UGString strTextureName);

		//! \brief Node是否存在
		UGbool IsExistToGeometry(UGString strName);

		//! \brief 添加一个材质
		//! \param strMaterialName 材质名，可以和Material3D里的名字不对应[in]
		//! \param pMaterial3D 材质信息[in]
		//! \return 重复材质返回false[out]
		UGbool AddMaterial3D(UGString strMaterialName, UGMaterial3D* pMaterial3D);

		//! \brief 添加一个纹理信息
		//! \param strTextureName 纹理名[in]
		//! \param pTexData 纹理信息[in]
		//! \return 重复纹理返回false[out]
		UGbool AddTextureData(UGString strTextureName, TextureDataInfo* pTexData);

		//! \brief 添加一个Geometry信息
		//! \param strName Geometry名[in]
		//! \param pGeometry Geometry信息[in]
		//! \return 重复返回false[out]
		UGbool AddGeometry(UGString strName, UGRenderOperationGeometry* pGeometry);

		//! \brief 获得材质
		UGMaterial3D* GetMaterial3D(UGString strMaterialName);
		//! \brief 获得材质
		std::map<UGString, UGMaterial3D*>& GetMaterial3Ds();

		//! \brief 获得纹理
		TextureDataInfo* GetTextureData(UGString strTextureName);
		//! \brief 获得纹理
		std::map<UGString, TextureDataInfo*>& GetTextureData();

		//! \brief 获得Geometry
		UGRenderOperationGeometry* GetGeometry(UGString strName);

		//! \brief 获得Geometry
		std::map<UGString, UGRenderOperationGeometry*>& GetGeometry();

		//! \brief 移除所有材质
		void RemoveAllMaterial3D();

		//! \brief 移除所有纹理
		void RemoveAllTexData();

		//! \brief 移除所有Geometry
		void RemoveAllGeometry();

		//! \brief 清空所有材质
		void ClearMaterial3D() { m_mapMaterial3D.clear(); };

		//! \brief 清空所有纹理
		void ClearTexData() { m_mapTextureData.clear(); };

		//! \brief 清空所有Geometry
		void ClearGeometry() { m_mapGeometry.clear(); };

	protected:
		//! \brief 包围球
		UGBoundingSphere m_BoundingSphere;

		//! \brief 名字
		UGString m_strName;

		//! \brief 位置
		UGMatrix4d m_matLocalView;

		//! \brief 材质信息
		std::map<UGString, UGMaterial3D*> m_mapMaterial3D;

		//! \brief 纹理信息
		std::map<UGString, TextureDataInfo*> m_mapTextureData;

		//! \brief Node信息
		std::map<UGString, UGRenderOperationGeometry*> m_mapGeometry;
	};

	class BASE3D_API UGRenderOperationGroup : public UGRenderOperationNode
	{
	public:
		UGRenderOperationGroup();
		virtual ~UGRenderOperationGroup();

		//! \brief 如果是Group对象就返回this，否则NULL
		virtual UGRenderOperationGroup* AsGroup() {return this;};

		//! \brief 类名
		virtual UGString ClassName() { return _U("Group"); }

		//! \brief 设置父节点
		void SetParentNode(UGRenderOperationNode* pNode);

		//! \brief 获得父节点
		UGRenderOperationNode* GetParentNode();

		//! \brief 添加子节点
		void AddChild(UGRenderOperationNode* pNode);

		//! \brief 获得子节点数量
		UGint GetNumChildren();

		//! \brief 按索引获得子节点
		//! \param nIndex 索引[in]
		//! \return Node[out]
		UGRenderOperationNode* GetChild(UGint nIndex);

		//! \brief 移除所有节点
		void RemoveAllChildren();

	private:
		//! \brief 父节点
		UGRenderOperationNode* m_pParentNode;

		//! \brief 子节点
		std::vector<UGRenderOperationNode*> m_vecChildNode;
	};

	class BASE3D_API UGRenderOperationPagedLOD : public UGRenderOperationGroup
	{
	public:
		UGRenderOperationPagedLOD();
		virtual ~UGRenderOperationPagedLOD();

		//! \brief 如果是Group对象就返回this，否则NULL
		virtual UGRenderOperationGroup* AsGroup() {return this;};

		//! \brief 类名
		virtual UGString ClassName() { return _U("PagedLOD"); }

		//! \brief 获得范围
		std::vector<std::pair<UGfloat, UGfloat> >& GetRanges();

		//! \brief 获得子切片
		std::vector<UGString>& GetFileNames();

		//! \brief 获取子切片的包围球
		std::map<UGString, UGBoundingSphere>& GetLODSphere();

		//! \brief 切换范围模式
		void SetRangeMode(UGRangeMode nMode) { m_nRangeMode = nMode; };

		//! \brief 获得切换范围模式
		UGRangeMode GetRangeMode() { return m_nRangeMode; };

	private:
		//! \brief 切换范围
		std::vector<std::pair<UGfloat, UGfloat> > m_vecRangeList;

		//! \brief LOD切片名
		std::vector<UGString> m_vecFileName;

		//! \brief LOD切片包围盒
		std::map<UGString, UGBoundingSphere> m_mapBoundingSphere;

		//! \brief 切换范围模式
		UGRangeMode m_nRangeMode;
	};

	struct BASE3D_API UGGeodeInfo
	{
		UGString m_strTableName;
		UGuint m_nID;
	};

	class BASE3D_API UGRenderOperationGeode : public UGRenderOperationNode
	{
	public:
		UGRenderOperationGeode();
		virtual ~UGRenderOperationGeode();

		//! \brief 如果是Group对象就返回this，否则NULL
		virtual UGRenderOperationGeode* AsGeode() { return this;};

		//! \brief 类名
		virtual UGString ClassName() { return _U("Geode"); }

		//! \brief 添加Geometry
		//! \param pROGeoemtry Geometry[in]
		void AddRenderOperationGeoemtry(UGRenderOperationGeometry* pROGeoemtry);

		//! \brief 获得Geoemtry总数
		UGint GetNumRenderOperationGeometry();

		//! \brief 获得Geometry
		//! \param nIndex 索引[in]
		//! \return Geometry[out]
		UGRenderOperationGeometry* GetRenderOperationGeometry(UGint nIndex);

		//! \brief 替换Geometry
		//! \param 替换对象索引[in]
		//! \param 替换的Geometry[in]
		//! \param 是否删除原对象
		void ReplaceGeometry(UGint nIndex, UGRenderOperationGeometry* pGeometry, UGbool bRelease = FALSE);

		//! \brief 清理Geometry
		void Release();

		//! \brief 计算包围盒
		void ComputerBoundingBox();

		//! \brief 获得包围盒
		UGBoundingBox GetBoundingBox();

	private:
		//! \brief 包围盒
		UGBoundingBox m_BoundingBox;

		//! \brief Geoemtry数据
		std::vector<UGRenderOperationGeometry*> m_vecROGeometry;

	public:
		//! \brief 属性表
		UGGeodeInfo m_GeodeInfo;

		//! \brief ID
		UGString m_strUniqueID;
	};
}

#endif
