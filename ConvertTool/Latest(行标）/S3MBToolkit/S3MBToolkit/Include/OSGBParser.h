#if !defined(OSGBPARSER_H__3B188E58_BBBB_404E_908D_712DA3E5D78F__INCLUDED_)
#define OSGBPARSER_H__3B188E58_BBBB_404E_908D_712DA3E5D78F__INCLUDED_

#pragma once
#include "stdafx.h"
#include "Common.h"
#include "Point3D.h"
#include "Shell.h"

#include <osg/Drawable>
#include <osg/Geometry>
#include <osg/Node>
#include <osg/TexMat>
#include <string>

namespace S3MB
{
	class STK_API OSGBConfig
	{
	public:
		OSGBConfig();
		~OSGBConfig();

		bool LoadFromFile(const std::wstring& strPath);

		float GetVersion() const;

		const std::wstring& GetFileType() const;

		const Point3D& GetPosition() const;

		const std::vector<std::wstring>& GetFileNames() const;

		int GetEpsgCode() const;

	private:
		//! \brief 版本号
		float m_fVersion;
		//! \brief 文件类型
		std::wstring m_strFileType;
		//! \brief 位置
		Point3D m_pntPosition;
		//! \brief 根节点名称
		std::vector<std::wstring> m_vecFileName;
		//! \brief 坐标编码
		int m_nEpsgCode;
	};

	class STK_API OSGBParser
	{
	public:
		OSGBParser();
		~OSGBParser();

		//! \brief 解析OSGB数据
		bool ParseNodeFile(const std::wstring& strPath, const std::wstring& strOption);

		void GetGroup(RenderOperationGroup*& pGroup, bool bSetNull = true);

	private:
		osg::ref_ptr<osg::Node> ReadNodeFile(const wstring& strPath, const wstring& strOption);

		void ProcessNode(osg::Node* pNode, RenderOperationGroup* pGroup, RenderOperationGroup* pRootGroup,
			Matrix4d mat4, std::wstring strNameID, const std::wstring& strFilePath, std::vector<std::wstring>& vecLodName);
		void ProcessDrawable(osg::Drawable* pDrawable, RenderOperationGeode* pGeode, RenderOperationGroup* pRootGroup, Matrix4d mat4, const std::wstring& strFilePath);
        void ProcessGeometry(osg::Geometry* geometry, Skeleton* pROSke, RenderOperationGroup* pRootGroup, const std::wstring& strFilePath);

		bool ReadVertex(VertexDataPackage* pVertexDataPackage, osg::Geometry* pOsgGeometry);
        bool ReadColorArray(Skeleton* pROSke, osg::Geometry* pOsgGeometry, unsigned int nFileType = 0);
		bool ReadTextureCoords(VertexDataPackage* pVertexDataPackage, osg::Geometry* pOsgGeometry, bool& bInstanceBatch);
		bool ReadIndexPackage(std::vector<IndexPackage*>& vecIndexPackage, osg::Geometry* pOsgGeometry);
		bool ReadNormal(VertexDataPackage* pVertexDataPackage, osg::Geometry* pOsgGeometry);
        void ProcessMaterial(Skeleton* pROSke, osg::StateSet* state, RenderOperationGroup* pRootGroup, const std::wstring& strFilePath);
        bool ProcessDefaultMaterial(Skeleton* pROSke, RenderOperationGroup* pRootGroup);
		void ReadMaterial(osg::StateSet* state, Pass* pPass);
		void ReadTextures(osg::StateSet* state, const std::wstring& strFilePath, \
			std::vector<std::wstring> &vecTexutreExisted, std::vector<std::wstring>& vecTexName, \
			std::vector<TextureData *>& vecTextureData, std::vector<Matrix4d>& vecMatrix, \
			std::vector<bool>& vecHasMipMap, std::vector<int>& vecLevel, std::map<std::wstring, UVWAddressingMode>& mapTexAddressingMode);
		void ReadTextureMatrix(osg::StateSet* state, int i, Matrix4d& matResult);
		void ReadTexture(osg::Image* image, TextureData * osgTextureData, \
			std::wstring& strTexPath, bool& bMipmap, int &nLevel);

		bool ProcessUShortIndexPackage(osg::PrimitiveSet* pPrimitiveSet, IndexPackage*& pIndexPackage);
		bool ProcessUByteIndexPackage(osg::PrimitiveSet* pPrimitiveSet, IndexPackage*& pIndexPackage);
		bool ProcessUIntIndexPackage(osg::PrimitiveSet* pPrimitiveSet, IndexPackage*& pIndexPackage);
		bool ProcessArraysIndexPackage(osg::PrimitiveSet* pPrimitiveSet, IndexPackage*& pIndexPackage);
		bool ProcessArrayLengthsIndexPackage(osg::PrimitiveSet* pPrimitiveSet, IndexPackage*& pIndexPackage);

		int GetVertexCount(osg::Geometry* pOsgGeometry);
        void SetRoGeoMaterialName(Skeleton* pSkeleton, RenderOperationGroup* pRootGroup, std::wstring strMaterialName);
		std::wstring OperationTypeToString(OperationType nType);
		void Convert2Matrix(osg::TexMat* texMat, Matrix4d& matResult);
		STKPixelFormat GlPixelFormat2Format(GLenum glPixelFormat);
		TextureAddressingMode GetTextureWrapMode(osg::Texture::WrapMode osgMode);
		std::wstring MaterialHashFunc(Pass* pPass, const std::wstring& strFilePath);

		void Clear();

	private:
		std::map<std::wstring, Matrix4d> m_mapRootNodesMatrix;
		//! \brief 记录纹理对应的adressMode
		std::map<std::wstring, UVWAddressingMode> m_mapTexAddressingMode;
		RenderOperationGroup* m_pROGroup;
	};
}

#endif