#if !defined(B3DMWRITER_H__C98B3A73_106D_4002_B416_8465B8604F1F__INCLUDED_)
#define B3DMWRITER_H__C98B3A73_106D_4002_B416_8465B8604F1F__INCLUDED_

#pragma once
#include "stdafx.h"
#include "Material.h"
#include "Matrix.h"
#include "Shell.h"
#include "Texture.h"

#include "COLLADA2GLTFWriter.h"
#include "COLLADA2GLTFExtrasHandler.h"
#include "COLLADASaxFWLLoader.h"
#include "GLTFAsset.h"

#include <string>
#include <vector>

namespace S3MB
{
	class STK_API B3DMWriter
	{
	public:
		B3DMWriter();
		~B3DMWriter();

		void Write();

	public:
		void SetIsEllipsoid(bool bEllip);
		void SetViewSphere(const Matrix4d& mat);
		void SetLocalView(const Matrix4d& mat);
		void SetOutput(const std::wstring& strOutput);
		void SetInput(const std::wstring& strInput);
		void SetArrInput(const std::vector<std::wstring>& vecInput);

		void GroupToB3DM(const std::wstring& strInputPath, const std::wstring& strOutputPath);

	private:
		bool ConvertGroupToGLTFAsset(const std::wstring& strFilePath, GLTF::Asset* pAsset, COLLADA2GLTF::Writer* pWriter);

		void ConvertGeodeToGLTFNode(RenderOperationGeode* pGeode, COLLADA2GLTF::Writer* pWriter, COLLADAFW::ObjectId id, std::vector<GLTF::Node*>* pVecNode);

		void MatrixToTransformMatrix(const Matrix4d& mat, GLTF::Node::TransformMatrix* pTransfiom);

		void PJConvert(double& x, double& y, double& z, Matrix4d mat = Matrix4d::IDENTITY);

		bool WriteGlobalAsset(COLLADA2GLTF::Writer* pWriter, COLLADAFW::FileInfo::UpAxisType nType);

		bool WriteVisualScene(COLLADA2GLTF::Writer* pWriter, RenderOperationGroup* pGroup);

		bool WriteNodeToGroup(COLLADA2GLTF::Writer* pWriter, std::vector<GLTF::Node*>* pVecNode, RenderOperationGroup* pGroup, COLLADAFW::ObjectId id = 0);

	private:
		void WriteB3DM(std::string strContent, GLTF::Buffer* pBuffer, COLLADA2GLTF::Options* pOptions);

		void WriteBatchTable(void* pWriter);

		void WriteBinaryGLTF(FILE* file, std::string strContent, GLTF::Buffer* pBuffer, COLLADA2GLTF::Options* pOptions);

		bool writeMaterialEffect(COLLADA2GLTF::Writer* writer, Material* pMaterial, COLLADAFW::ObjectId id);

		static void packColladaColor(COLLADAFW::Color color, float* packArray);

        //转换为GLTF纹理
		GLTF::Texture* ConvertTexturetoImage(COLLADA2GLTF::Writer* pWriter, const TextureDataInfo* pTexture);

        //转换为GLTMesh
        bool writeMesh(COLLADA2GLTF::Writer* writer, Skeleton* pSkeleton, COLLADAFW::UniqueId materialUniqueId, Vector3d vecOffset, COLLADAFW::ObjectId id = 0);

	private:
		static float GetMeshVertexDataAtIndex2(const GLTF::Accessor* pAccerror, unsigned int index, int idx);

		//纹理坐标转换
		static void GetMatrixFromPValue(float nZ, float & XTran, float & YTran, float& scale);

		static ColorValue ConvertUintColor2ColorValue3D(const unsigned int& color);

		static std::string BuildAttributeId2(const GLTF::Accessor* pAccerror, unsigned int index);

	private:

		void clear(GLTF::Asset* pAsset);

		template<typename T>
		void GLTFObjectDeleterStdVector(std::vector<T*>&&v);

		std::vector<GLTF::Buffer*> getAssetAllBuffers(GLTF::Asset* pAsset);

		std::vector<GLTF::BufferView*> getAssetAllBufferViews(GLTF::Asset* pAsset);

		void removeImage(GLTF::Image* pImage);

		void removeAccessor(GLTF::Accessor* pAccessor);

		void removeMaterial(GLTF::Material* pMaterial);

	private:

		std::map<std::wstring, COLLADAFW::UniqueId> m_mapGLTFMaterialCache;
		std::map<std::wstring, GLTF::Image*> m_mapGLTFImageCache;
		std::map<std::wstring, TextureDataInfo*>m_mapModelNodeTextures;
		std::map<std::wstring, Material*>m_mapModelNodeMaterials;
	private:
		std::wstring m_strExt;
		std::vector<std::wstring> m_vecFilePath;
		std::wstring m_strInput;
		std::wstring m_strOutput;
		bool m_bEllip;
		Matrix4d m_matViewSphere;
		Matrix4d m_matLocalView;

		std::map<unsigned int, unsigned int> m_BatchIDs;
		std::map<unsigned int, unsigned int> m_propIDs;
	};
}

#endif