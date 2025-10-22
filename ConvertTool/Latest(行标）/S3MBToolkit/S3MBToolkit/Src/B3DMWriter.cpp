#include <QImage>
#include "B3DMWriter.h"
#include "ImageOperator.h"
#include "MathEngine.h"
#include "S3MBReader.h"
#include "S3MBUtils.h"
#include "Utils/Utils.h"

#include "document.h"
#include "istreamwrapper.h"
#include "prettywriter.h"
#include "stringbuffer.h"

#include <cstdio>

namespace S3MB
{
	const uint32_t HEADER_LENGTH = 12;
	const uint32_t CHUNK_HEADER_LENGTH = 8;
	const uint32_t B3DM_HEADER_LENGTH = 28;

	B3DMWriter::B3DMWriter()
	{
		m_bEllip = false;
		m_matViewSphere = Matrix4d::IDENTITY;
		m_matLocalView = Matrix4d::IDENTITY;
	}

	B3DMWriter::~B3DMWriter()
	{

	}

	void B3DMWriter::Write()
	{
		for (unsigned int i = 0; i < m_vecFilePath.size(); i++)
		{
			std::wstring strFilePath = m_vecFilePath[i];
			strFilePath = StringUtil::ChangeExt(strFilePath, m_strExt);
			strFilePath = StringUtil::GetRelativePath(m_strInput, strFilePath);
			std::wstring strOutput = StringUtil::GetAbsolutePath(m_strOutput, strFilePath);
			GroupToB3DM(m_vecFilePath[i], strOutput);
		}
	}

	void B3DMWriter::SetIsEllipsoid(bool bEllip)
	{
		m_bEllip = bEllip;
	}

	void B3DMWriter::SetViewSphere(const Matrix4d& mat)
	{
		m_matViewSphere = mat;
	}

	void B3DMWriter::SetLocalView(const Matrix4d& mat)
	{
		m_matLocalView = mat;
	}

	void B3DMWriter::SetOutput(const std::wstring& strOutput)
	{
		m_strOutput = strOutput;
	}

	void B3DMWriter::SetInput(const std::wstring& strInput)
	{
		m_strInput = strInput;
	}

	void B3DMWriter::SetArrInput(const std::vector<std::wstring>& vecInput)
	{
		m_vecFilePath = vecInput;
	}

	void B3DMWriter::GroupToB3DM(const std::wstring& strInputPath, const std::wstring& strOutputPath)
	{
		std::wstring strOutputDir = StringUtil::GetDir(strOutputPath);
		if (!StringUtil::IsFileExist(strOutputDir))
		{
			FileUtil::MkDirEx(strOutputDir);
		}
		std::wstring strFileName = StringUtil::GetName(strInputPath);
		std::wstring strFilePath = StringUtil::GetAbsolutePath(strOutputDir, strFileName);

		COLLADASaxFWL::Loader* loader = new COLLADASaxFWL::Loader();
		GLTF::Asset* asset = new GLTF::Asset();
		COLLADA2GLTF::Options* options = new COLLADA2GLTF::Options();

		asset->globalSampler->minFilter = GLTF::Constants::WebGL::LINEAR;

		options->inputPath = StringUtil::UNICODE_to_UTF8(strFilePath).c_str();
		options->outputPath = StringUtil::UNICODE_to_UTF8(strOutputPath).c_str();
		options->version = "2.0";
		options->binary = true;
		options->doubleSided = true;

		COLLADA2GLTF::Writer* writer = new COLLADA2GLTF::Writer(loader, asset, options, NULL);
		ConvertGroupToGLTFAsset(strInputPath, asset, writer);

		GLTF::Buffer* buffer = asset->packAccessors();
		// Create image bufferViews for binary glTF
		if (options->binary && options->embeddedTextures) {
			size_t imageBufferLength = 0;
			std::vector<GLTF::Image*> images = asset->getAllImages();
			for (GLTF::Image* image : images)
			{
				imageBufferLength += image->byteLength;
			}
			unsigned char* bufferData = buffer->data;
			bufferData = (unsigned char*)realloc(bufferData, buffer->byteLength + imageBufferLength);
			size_t byteOffset = buffer->byteLength;
			for (GLTF::Image* image : images)
			{
				GLTF::BufferView* bufferView = new GLTF::BufferView(byteOffset, image->byteLength, buffer);
				image->bufferView = bufferView;
				std::memcpy(bufferData + byteOffset, image->data, image->byteLength);
				byteOffset += image->byteLength;
			}
			buffer->data = bufferData;
			buffer->byteLength += imageBufferLength;
		}
		rapidjson::StringBuffer s;
		rapidjson::Writer<rapidjson::StringBuffer> jsonWriter(s);
		jsonWriter.StartObject();
		asset->writeJSON(&jsonWriter, options);
		jsonWriter.EndObject();
		// OSGB TO B3DM
		std::string jsonString = s.GetString();
		WriteB3DM(jsonString, buffer, options);
		clear(asset);
		//delete asset;
		delete writer;
		delete options;
	}

	bool B3DMWriter::ConvertGroupToGLTFAsset(const std::wstring& strFilePath, GLTF::Asset* pAsset, COLLADA2GLTF::Writer* pWriter)
	{
		if (!StringUtil::IsFileExist(strFilePath) || pAsset == NULL || pWriter == NULL)
		{
			return false;
		}

		S3MBReader reader;
		reader.ReadFile(strFilePath);
		RenderOperationGroup* pGroup = reader.GetRenderOperationGroup();
		if (pGroup == NULL)
		{
			return false;
		}

		m_mapGLTFMaterialCache.clear();
		m_mapGLTFImageCache.clear();
		m_mapModelNodeMaterials.clear();
		m_mapModelNodeTextures.clear();

		m_mapModelNodeMaterials = pGroup->GetMaterials();
		m_mapModelNodeTextures = pGroup->GetTextureData();

		pWriter->start();
		WriteGlobalAsset(pWriter, COLLADAFW::FileInfo::Z_UP);

		WriteVisualScene(pWriter, pGroup);

		pAsset->mergeAnimations();
		pAsset->removeUnusedNodes(pWriter->_options);

		reader.Clear();
		pGroup = nullptr;

		return true;
	}

	void B3DMWriter::ConvertGeodeToGLTFNode(RenderOperationGeode* pGeode, COLLADA2GLTF::Writer* pWriter, COLLADAFW::ObjectId id, std::vector<GLTF::Node*>* pVecNode)
	{
		GLTF::Node::TransformMatrix* pTransform = new GLTF::Node::TransformMatrix;
		MatrixToTransformMatrix(pGeode->GetPosition(), pTransform);

		GLTF::Node* pNode = new GLTF::Node();
		pNode->transform = pTransform;
		pVecNode->push_back(pNode);

		int nIndex = 0;
        int nNumSke = pGeode->GetNumSkeleton();

		Vector3d vecOffset(0, 0, 0);
		if (m_bEllip)
		{//坐标及ENU变换
			Vector3d vecMin;
			Vector3d vecMax;
            for (int iSke = 0; iSke < nNumSke; iSke++)
			{
                Skeleton* pSkeleton = pGeode->GetSkeleton(iSke);
				if (pSkeleton == nullptr)
				{
					continue;
				}

				VertexDataPackage* pVertexDataPackage = pSkeleton->m_pVertexDataPackage;
				unsigned int nVertexDim = pVertexDataPackage->m_nVertexDimension;
				unsigned int nVertexCount = pVertexDataPackage->m_nVerticesCount;
				for (size_t i = 0; i < nVertexCount * nVertexDim; i += nVertexDim)
				{
					Vector3d vecPos(pVertexDataPackage->m_pVertices[i], pVertexDataPackage->m_pVertices[i + 1], pVertexDataPackage->m_pVertices[i + 2]);
					PJConvert(vecPos.x, vecPos.y, vecPos.z, pGeode->GetPosition());
					pVertexDataPackage->m_pVertices[i] = vecPos.x;
					pVertexDataPackage->m_pVertices[i + 1] = vecPos.y;
					pVertexDataPackage->m_pVertices[i + 2] = vecPos.z;
                    if (i == 0 && iSke == 0)
					{
						vecMax = vecPos;
						vecMin = vecPos;
						continue;
					}
					vecMax.x = vecMax.x < vecPos.x ? vecPos.x : vecMax.x;
					vecMax.y = vecMax.y < vecPos.y ? vecPos.y : vecMax.y;
					vecMax.z = vecMax.z < vecPos.z ? vecPos.z : vecMax.z;
					vecMin.x = vecMin.x > vecPos.x ? vecPos.x : vecMin.x;
					vecMin.y = vecMin.y > vecPos.y ? vecPos.y : vecMin.y;
					vecMin.z = vecMin.z > vecPos.z ? vecPos.z : vecMin.z;
				}
			}
			vecOffset.x = (vecMax.x + vecMin.x) / 2;
			vecOffset.y = (vecMax.y + vecMin.y) / 2;
			vecOffset.z = (vecMax.z + vecMin.z) / 2;
			MatrixToTransformMatrix(MatrixUtils::Translation(vecOffset.x, vecOffset.y, vecOffset.z), pTransform);
		}
		unsigned int j = 0;
        for (int iSke = 0; iSke < nNumSke; iSke++)
		{
            Skeleton* pSkeleton = pGeode->GetSkeleton(iSke);
			if (pSkeleton == nullptr)
			{
				++j;
				continue;
			}

			std::wstring strMaterialName = pSkeleton->m_strMaterialName;
			COLLADAFW::UniqueId materialID;
			std::map<std::wstring, COLLADAFW::UniqueId> ::const_iterator materialCacheIt = m_mapGLTFMaterialCache.find(strMaterialName);
			if (materialCacheIt != m_mapGLTFMaterialCache.end())
			{
				materialID = materialCacheIt->second;
			}
			else
			{
				Material* pMaterial = m_mapModelNodeMaterials[strMaterialName];
				writeMaterialEffect(pWriter, pMaterial, id + j);
				materialID = COLLADAFW::UniqueId(COLLADAFW::COLLADA_TYPE::ClassId::MATERIAL, id + j, 1);
			}

			writeMesh(pWriter, pSkeleton, materialID, vecOffset, id + j);

			COLLADAFW::UniqueId meshId(COLLADAFW::COLLADA_TYPE::ClassId::MESH, id + j, 3);

			if (pNode->mesh != NULL)
			{
				GLTF::Node* meshNode = new GLTF::Node();
				meshNode->transform = pNode->transform;
				pVecNode->push_back(meshNode);
				meshNode->mesh = pWriter->_meshInstances[meshId];
			}
			else
			{
				pNode->mesh = pWriter->_meshInstances[meshId];
			}
			++j;
		}
	}

	void B3DMWriter::MatrixToTransformMatrix(const Matrix4d& mat, GLTF::Node::TransformMatrix* pTransfiom)
	{
		// 注意行列式区别
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				pTransfiom->matrix[i * 4 + j] = mat[i][j];
			}
		}
	}

	void B3DMWriter::PJConvert(double& x, double& y, double& z, Matrix4d mat)
	{
		if (m_bEllip)
		{
			return;
		}

		Vector3d vecPos(x, y, z);
		vecPos = vecPos.MultiplyMatrix(mat);
		vecPos = vecPos.MultiplyMatrix(m_matViewSphere);
		vecPos = MathEngine::CartesianToSphericalD(vecPos.x, vecPos.y, vecPos.z);
		vecPos.x *= RTOD;
		vecPos.y *= RTOD;
		vecPos.z -= GLOBAL_RADIUS;

		if (m_bEllip)
		{
			vecPos = MathEngine::SphericalToCartesian(vecPos.x*DTOR, vecPos.y*DTOR, vecPos.z + GLOBAL_RADIUS);
		}
		else
		{
			vecPos = MathEngine::WGS84ToCesiumCartesian(vecPos.x*DTOR, vecPos.y*DTOR, vecPos.z + GLOBAL_RADIUS);
		}

		vecPos = vecPos.MultiplyMatrix(m_matLocalView.Invert());
		x = vecPos.x;
		y = vecPos.y;
		z = vecPos.z;
	}

	bool B3DMWriter::WriteGlobalAsset(COLLADA2GLTF::Writer* pWriter, COLLADAFW::FileInfo::UpAxisType nType)
	{
		pWriter->_assetScale = 1;
		if (nType == COLLADAFW::FileInfo::X_UP)
		{
			pWriter->_rootNode = new GLTF::Node();
			pWriter->_rootNode->transform = new GLTF::Node::TransformMatrix(
				0, -1, 0, 0,
				1, 0, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1
			);
			pWriter->_rootNode->name = "X_UP";
		}
		else if (nType == COLLADAFW::FileInfo::Z_UP)
		{
			pWriter->_rootNode = new GLTF::Node();
			pWriter->_rootNode->transform = new GLTF::Node::TransformMatrix(
				1, 0, 0, 0,
				0, 0, 1, 0,
				0, -1, 0, 0,
				0, 0, 0, 1
			);
			pWriter->_rootNode->name = "Z_UP";
		}
		else if (nType == COLLADAFW::FileInfo::Y_UP)
		{
			pWriter->_rootNode = new GLTF::Node();
			pWriter->_rootNode->transform = new GLTF::Node::TransformMatrix(
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1
			);
			pWriter->_rootNode->name = "Y_UP";
		}
		return true;
	}

	bool B3DMWriter::WriteVisualScene(COLLADA2GLTF::Writer* pWriter, RenderOperationGroup* pGroup)
	{
		GLTF::Asset* pAsset = pWriter->_asset;
		GLTF::Scene* pScene = nullptr;
		if (pAsset->scene >= 0)
		{
			pScene = new GLTF::Scene();
			pAsset->scenes.push_back(pScene);
		}
		else
		{
			pScene = pAsset->getDefaultScene();
		}

		if (pWriter->_rootNode != nullptr)
		{
			pScene->nodes.push_back(pWriter->_rootNode);
			return WriteNodeToGroup(pWriter, &pWriter->_rootNode->children, pGroup);
		}
		return WriteNodeToGroup(pWriter, &pScene->nodes, pGroup);;
	}

	bool B3DMWriter::WriteNodeToGroup(COLLADA2GLTF::Writer* pWriter, std::vector<GLTF::Node*>* pVecNode, RenderOperationGroup* pGroup, COLLADAFW::ObjectId id)
	{
		for (int iNode = 0; iNode < pGroup->GetNumChildren(); iNode++)
		{
			RenderOperationNode* pNode = pGroup->GetChild(iNode);
			if (pNode == NULL)
			{
				assert(false);
				continue;
			}

			std::wstring strName = pNode->ClassName();
			if (StringUtil::CompareNoCase(strName, S3MB_SHELL_PAGEDLOD))
			{
				RenderOperationPagedLOD* pLOD = (RenderOperationPagedLOD*)pNode;
				for (int iChild = 0; iChild < pLOD->GetNumChildren(); iChild++)
				{
					RenderOperationNode* pChildNode = pLOD->GetChild(iChild);
					std::wstring strChildName = pChildNode->ClassName();
					if (StringUtil::CompareNoCase(strChildName, S3MB_SHELL_PAGEDLOD))
					{
						assert(false);
					}
					else if (StringUtil::CompareNoCase(strChildName, S3MB_SHELL_GEODE))
					{
						RenderOperationGeode* pChildGeode = (RenderOperationGeode*)pChildNode;
						ConvertGeodeToGLTFNode(pChildGeode, pWriter, id, pVecNode);
					}
				}
			}
			else if (StringUtil::CompareNoCase(strName, S3MB_SHELL_GEODE))
			{
				RenderOperationGeode* pGeode = (RenderOperationGeode*)pNode;
				ConvertGeodeToGLTFNode(pGeode, pWriter, id, pVecNode);
			}
		}
		return true;
	}

	void B3DMWriter::WriteB3DM(std::string strContent, GLTF::Buffer* pBuffer, COLLADA2GLTF::Options* pOptions)
	{
		// 输出文件
		std::string strOutput = pOptions->outputPath;
		std::wstring strPath = StringUtil::UTF8_to_UNICODE(strOutput);
		strPath = StringUtil::ChangeExt(strPath, U(".b3dm"));
		strOutput = StringUtil::UNICODE_to_UTF8(strPath);
		strOutput = StringUtil::UTF8_to_ANSI(strOutput);
		std::wstring strDir = StringUtil::GetDir(strPath);
		if (!StringUtil::IsFileExist(strDir))
		{
			FileUtil::MkDirEx(strDir);
		}

		FILE* file = fopen(strOutput.c_str(), "wb");
		if (file != NULL)
		{
			// 写头文件
			fwrite("b3dm", sizeof(unsigned char), 4, file);

			uint32_t jsonPadding = (4 - (strContent.length() & 3)) & 3;
			uint32_t binPadding = (4 - (pBuffer->byteLength & 3)) & 3;

			uint32_t binGltfLength = HEADER_LENGTH + (CHUNK_HEADER_LENGTH + strContent.length() + jsonPadding + pBuffer->byteLength + binPadding); // length
			if (pOptions->version != "1.0")
			{
				binGltfLength += CHUNK_HEADER_LENGTH;
			}
			// 写属性表
			uint32_t nTotalLength = 0;
			uint32_t nFeatureTableLength = 0;
			uint32_t nBatchTableLength = 0;

			rapidjson::StringBuffer fs;
			rapidjson::StringBuffer bs;
			if (m_BatchIDs.size() > 0)
			{
				rapidjson::Writer<rapidjson::StringBuffer> jsonFeatureTableWriter(fs);
				// FeatureTable
				jsonFeatureTableWriter.StartObject();
				jsonFeatureTableWriter.Key("BATCH_LENGTH");
				jsonFeatureTableWriter.Uint(m_BatchIDs.size());
				jsonFeatureTableWriter.EndObject();
				nFeatureTableLength = fs.GetSize();

				// BatchTable
				rapidjson::Writer<rapidjson::StringBuffer> jsonBatchTableWriter(bs);
				WriteBatchTable(&jsonBatchTableWriter);
				nBatchTableLength = bs.GetSize();
			}
			uint32_t jsonTablePadding = (4 - ((nFeatureTableLength + nBatchTableLength) & 3)) & 3;
			nTotalLength = B3DM_HEADER_LENGTH + binGltfLength + nFeatureTableLength + nBatchTableLength + jsonTablePadding;

			uint32_t header[6] = { 1, nTotalLength, nFeatureTableLength, 0, nBatchTableLength + jsonTablePadding, 0 };
			fwrite(header, sizeof(uint32_t), 6, file);

			// write featureTable and BatchTable
			fwrite(fs.GetString(), sizeof(char), nFeatureTableLength, file);
			fwrite(bs.GetString(), sizeof(char), nBatchTableLength, file);

			for (int i = 0; i < jsonTablePadding; i++) {
				fwrite(" ", sizeof(char), 1, file);
			}

			// write Binary glTF
			WriteBinaryGLTF(file, strContent, pBuffer, pOptions);

			fclose(file);
		}
	}

	void B3DMWriter::WriteBatchTable(void* pWriter)
	{
		rapidjson::Writer<rapidjson::StringBuffer>* pJsonWriter = (rapidjson::Writer<rapidjson::StringBuffer>*)pWriter;

		pJsonWriter->StartObject();
		pJsonWriter->Key("id");
		pJsonWriter->StartArray();
		std::map<unsigned int, unsigned int>::iterator it = m_propIDs.begin();
		for (; it != m_propIDs.end(); it++)
		{
			pJsonWriter->Uint(it->second);
		}
		pJsonWriter->EndArray();
		pJsonWriter->EndObject();
	}

	void B3DMWriter::WriteBinaryGLTF(FILE* file, std::string strContent, GLTF::Buffer* pBuffer, COLLADA2GLTF::Options* pOptions)
	{
		fwrite("glTF", sizeof(char), 4, file);

		uint32_t* writeHeader = new uint32_t[2];
		// version
		if (pOptions->version == "1.0") {
			writeHeader[0] = 1;
		}
		else {
			writeHeader[0] = 2;
		}

		uint32_t jsonPadding = (4 - (strContent.length() & 3)) & 3;
		uint32_t binPadding = (4 - (pBuffer->byteLength & 3)) & 3;

		writeHeader[1] = HEADER_LENGTH + (CHUNK_HEADER_LENGTH + strContent.length() + jsonPadding + pBuffer->byteLength + binPadding); // length
		if (pOptions->version != "1.0") {
			writeHeader[1] += CHUNK_HEADER_LENGTH;
		}
		fwrite(writeHeader, sizeof(uint32_t), 2, file); // GLB header

		writeHeader[0] = strContent.length() + jsonPadding; // 2.0 - chunkLength / 1.0 - contentLength
		if (pOptions->version == "1.0") {
			writeHeader[1] = 0; // 1.0 - contentFormat
		}
		else {
			writeHeader[1] = 0x4E4F534A; // 2.0 - chunkType JSON
		}
		fwrite(writeHeader, sizeof(uint32_t), 2, file);
		fwrite(strContent.c_str(), sizeof(char), strContent.length(), file);
		for (int i = 0; i < jsonPadding; i++) {
			fwrite(" ", sizeof(char), 1, file);
		}
		if (pOptions->binary)
		{
			if (pOptions->version != "1.0") {
				writeHeader[0] = pBuffer->byteLength + binPadding; // chunkLength
				writeHeader[1] = 0x004E4942; // chunkType BIN
				fwrite(writeHeader, sizeof(uint32_t), 2, file);
			}
			fwrite(pBuffer->data, sizeof(unsigned char), pBuffer->byteLength, file);
			for (int i = 0; i < binPadding; i++) {
				fwrite("\0", sizeof(char), 1, file);
			}
		}
		delete[] writeHeader;
	}

	bool B3DMWriter::writeMaterialEffect(COLLADA2GLTF::Writer* writer, Material* pMaterial, COLLADAFW::ObjectId id)
	{
		// 创建UniqueId
		COLLADAFW::UniqueId materialId(COLLADAFW::COLLADA_TYPE::ClassId::MATERIAL, id, 1);

		COLLADAFW::UniqueId effectId(COLLADAFW::COLLADA_TYPE::ClassId::EFFECT, id, 2);
		GLTF::MaterialPBR* material = new GLTF::MaterialPBR();
		std::wstring strMatName = pMaterial->m_strName;
		std::string tempName = StringUtil::UNICODE_to_UTF8(strMatName);

		material->stringId = tempName.c_str();
		material->name = tempName.c_str();

		Technique* pTech = pMaterial->getTechnique(0);
		Pass* pPass = NULL;
		if (pTech != NULL)
		{
			pPass = pTech->getPass(0);
		}
		bool bNeedRelease = false;

		if (pPass == NULL)
		{
			pPass = new Pass();
		}

		// ambient
		material->values->ambient = new float[4];
		packColladaColor(COLLADAFW::Color(pPass->m_Ambient.r, pPass->m_Ambient.g, pPass->m_Ambient.b, pPass->m_Ambient.a), material->values->ambient);

		// diffuse的纹理or颜色
		std::vector<std::wstring> vecTextureName;
		if (pMaterial->getNumTechniques() > 0)
		{
			Technique* pTech = pMaterial->getTechnique(0);
			if (pTech != nullptr && pTech->getNumPasses() > 0)
			{
				Pass* pPass = pTech->getPass(0);
				if (pPass != nullptr)
				{
					for (int t = 0; t < pPass->GetTextureUnitStatesSize(); t++)
					{
						TextureUnitState* pTexUnit = pPass->GetTextureUnitState(t);
						if (pTexUnit == NULL)
						{
							continue;
						}

						std::wstring strTextureName = pTexUnit->m_strTextureName;
						if (strTextureName.length() != 0)
						{
							vecTextureName.push_back(strTextureName);
						}
					}
				}
			}
		}
		material->metallicRoughness->metallicFactor = 0;
		if (vecTextureName.size() > 0)
		{
			TextureDataInfo* pTxtD = m_mapModelNodeTextures[vecTextureName[0]];
			GLTF::Texture* gtx = ConvertTexturetoImage(writer, pTxtD);
			material->metallicRoughness->baseColorTexture = new GLTF::MaterialPBR::Texture;
			material->metallicRoughness->baseColorTexture->texture = gtx;
            material->alphaMode = "OPAQUE";
			material->metallicRoughness->baseColorFactor = new float[4]{ 1,1,1,1 };
		}
		else
		{
			ColorValue materialColor;
			material->metallicRoughness->baseColorFactor = new float[4];
			packColladaColor(COLLADAFW::Color(pPass->m_Diffuse.r*materialColor.r, pPass->m_Diffuse.g*materialColor.g,
				pPass->m_Diffuse.b*materialColor.b, pPass->m_Diffuse.a*materialColor.a), material->metallicRoughness->baseColorFactor);
			if (pPass->m_Diffuse.a*materialColor.a < 1.0)
			{
				material->alphaMode = "BLEND";
			}
			else
			{
				material->alphaMode = "OPAQUE";
			}
		}

		// doubleside
		material->doubleSided = true;
		writer->_effectInstances[effectId] = material;
		writer->_materialEffects[materialId] = effectId;

		if (bNeedRelease&&pPass != nullptr)
		{
			delete pPass;
			pPass = nullptr;
		}
		return true;
	}

	void B3DMWriter::packColladaColor(COLLADAFW::Color color, float* packArray)
	{
		packArray[0] = (float)color.getRed();
		packArray[1] = (float)color.getGreen();
		packArray[2] = (float)color.getBlue();
		packArray[3] = (float)color.getAlpha();
	}

	GLTF::Texture* B3DMWriter::ConvertTexturetoImage(COLLADA2GLTF::Writer* pWriter, const TextureDataInfo* pTexture)
	{
        std::wstring tempPath = FileUtils::GetTemporaryPath();
		unsigned int nPixelSize = 4;
		std::wstring strExt = U(".png");
		if (pTexture->m_pTextureData->m_enFormat == PF_RGBA8 ||
			pTexture->m_pTextureData->m_enFormat == PF_RGBA32F)
		{
			nPixelSize = 4;
		}
		else
		{
			nPixelSize = 3;
			strExt = U(".jpg");
		}

#ifdef WIN32
		std::wstring fileName = tempPath + U("\\") + BaseUtils::CreateGuid() + strExt;
#else
		std::wstring fileName = tempPath + U("/") + BaseUtils::CreateGuid() + strExt;
#endif

		unsigned int nWidth = pTexture->m_pTextureData->m_nWidth;
		unsigned int nHeight = pTexture->m_pTextureData->m_nHeight;

		unsigned char* pBuffer = NULL;

		unsigned char* outBuffer = NULL;
        unsigned int nDecodeSize = ImageOperator::Decode(nPixelSize, nWidth, nHeight, &outBuffer, pTexture->m_pTextureData->m_pBuffer, pTexture->m_pTextureData->m_eCompressType);
		unsigned char* saveBuffer = nullptr;
		if (nPixelSize == 3)
		{
			saveBuffer = new unsigned char[nWidth*nHeight * 3];
			int i = 0, j = 0, k = 0;
			for (i = 0; i < nWidth*nHeight; i++, j += 3, k += 4)
			{
				saveBuffer[j] = outBuffer[k];    // R
				saveBuffer[j + 1] = outBuffer[k + 1];  //G
				saveBuffer[j + 2] = outBuffer[k + 2]; //B
			}
		}
		else
		{
			saveBuffer = outBuffer;
		}

		if (saveBuffer != nullptr)
		{
			if (nPixelSize == 4)
			{
				ImageFileType fileType = ImageFileType::IF_JPG;
				for (int i = 0; i < nWidth*nHeight; i++)
				{
					if (saveBuffer[4 * i + 3] < 255)
					{
						fileType = ImageFileType::IF_PNG;
						break;
					}
				}
				if (fileType == ImageFileType::IF_JPG)
				{
					strExt = U(".jpg");
					fileName = StringUtil::ChangeExt(fileName, strExt);
				}
			}

			std::string strName = StringUtil::UNICODE_to_UTF8(fileName);
			QImage::Format format = nPixelSize == 4 ? QImage::Format::Format_RGBA8888 : QImage::Format::Format_RGB888;
			QImage* pImg = new QImage(saveBuffer, nWidth, nHeight, format);
			pImg->mirrored().save(QString(strName.c_str()));
			delete pImg;
			pImg = nullptr;
		}

		std::string strName = StringUtil::UNICODE_to_UTF8(fileName);
		GLTF::Texture* tex = new GLTF::Texture;
		std::map<std::wstring, GLTF::Image*>::iterator imageCacheIt = m_mapGLTFImageCache.find(pTexture->m_strName);
		if (imageCacheIt != m_mapGLTFImageCache.end())
		{
			tex->source = imageCacheIt->second;
		}
		else
		{
			GLTF::Image* image = GLTF::Image::load(strName, false);
			m_mapGLTFImageCache[pTexture->m_strName] = image;
			tex->source = image;
		}
		remove(strName.c_str());

		tex->sampler = pWriter->_asset->globalSampler;

		delete outBuffer;
		outBuffer = NULL;

		if (nPixelSize == 3)
		{
			delete saveBuffer;
		}
		saveBuffer = NULL;

		return tex;
	}

    bool B3DMWriter::writeMesh(COLLADA2GLTF::Writer* writer, Skeleton* pSkeleton, COLLADAFW::UniqueId materialUniqueId, Vector3d vecOffset, COLLADAFW::ObjectId id)
	{
		if (pSkeleton == nullptr)
		{
			return false;
		}

		GLTF::Mesh* pMesh = new GLTF::Mesh();

		COLLADAFW::UniqueId meshId(COLLADAFW::COLLADA_TYPE::ClassId::MESH, id, 3);
		std::string name = StringUtil::UNICODE_to_UTF8(pSkeleton->m_strGeoName);
		pMesh->name = name;
		pMesh->stringId = name;

		GLTF::Primitive* pPrimitive = new GLTF::Primitive();
		//材质
		pPrimitive->material = writer->_effectInstances[writer->_materialEffects[materialUniqueId]];

		//Primitive索引表
		std::map<GLTF::Primitive*, std::vector<unsigned int> > positionMapping;
		//Primitive材质表
		std::map<int, std::set<GLTF::Primitive*> > primitiveMaterialMapping;
		//临时数据  需要释放！！！
		std::map<std::string, GLTF::Accessor*> mapTempAccessor;

		std::map<std::string, std::vector<float>> buildAttributes;
		std::map<std::string, unsigned int> attributeIndicesMapping;
		std::vector<unsigned int> skeletonIndeces;
		std::vector<unsigned int> gltfIndeces;

		unsigned int index = 0;
		unsigned int face = 0;
		unsigned int startFace = 0;
		unsigned int totalVertexCount = 0;
		unsigned int vertexCount = 0;
		unsigned int faceVertexCount = 0;

		VertexDataPackage* pVertexDataPackage = pSkeleton->m_pVertexDataPackage;

		int materialId = id;
		std::map<int, std::set<GLTF::Primitive*>>::iterator findPrimitiveSet = primitiveMaterialMapping.find(materialId);
		if (findPrimitiveSet != primitiveMaterialMapping.end())
		{
			findPrimitiveSet->second.insert(pPrimitive);
		}
		else
		{
			std::set<GLTF::Primitive*> primitiveSet;
			primitiveSet.insert(pPrimitive);
			primitiveMaterialMapping[materialId] = primitiveSet;
		}

		std::vector<unsigned int> mapping;
		bool shouldTriangulate = false;
		IndexPackage* pIndexPack = pSkeleton->m_arrIndexPackage[0];

		switch (pIndexPack->m_OperationType)
		{
		case OT_LINE_LIST:
			pPrimitive->mode = GLTF::Primitive::Mode::LINES;
			break;
		case OT_LINE_STRIP:
			pPrimitive->mode = GLTF::Primitive::Mode::LINE_STRIP;
			break;
		case OT_TRIANGLE_LIST:
			pPrimitive->mode = GLTF::Primitive::Mode::TRIANGLES;
			break;
		case OT_TRIANGLE_STRIP:
			pPrimitive->mode = GLTF::Primitive::Mode::TRIANGLE_STRIP;
			break;
		case OT_TRIANGLE_FAN:
			pPrimitive->mode = GLTF::Primitive::Mode::TRIANGLE_FAN;
			break;
		case OT_POINT_LIST:
			pPrimitive->mode = GLTF::Primitive::Mode::POINTS;
			break;
		}

		unsigned int count = pIndexPack->m_nIndexesCount;

        //骨架所有信息
		std::map<std::string, const void*> semanticIndices;

		// POSITION
		std::string semantic = "POSITION";
		buildAttributes[semantic] = std::vector<float>();
		unsigned int nVertexDim = pVertexDataPackage->m_nVertexDimension;
		float* pVertex = NULL;

		unsigned int nVertexCount = pVertexDataPackage->m_nVerticesCount;
		pVertex = new float[nVertexCount * 3];
		unsigned int pvIndex = 0;
		for (size_t i = 0; i < nVertexCount * nVertexDim; i += nVertexDim)
		{
			pVertex[pvIndex] = pVertexDataPackage->m_pVertices[i] - vecOffset.x;
			pVertex[pvIndex + 1] = pVertexDataPackage->m_pVertices[i + 1] - vecOffset.y;
			pVertex[pvIndex + 2] = pVertexDataPackage->m_pVertices[i + 2] - vecOffset.z;
			pvIndex += 3;
		}
		semanticIndices[semantic] = pVertex;

		mapTempAccessor[semantic] = new GLTF::Accessor(GLTF::Accessor::Type::VEC3, GLTF::Constants::WebGL::FLOAT,
			(unsigned char*)pVertex, nVertexCount, GLTF::Constants::WebGL::ARRAY_BUFFER);

		pPrimitive->attributes[semantic] = NULL;// pPosition;
		unsigned int nIndex = 0;
		//indeces
		if (pVertexDataPackage->m_nVerticesCount < 65536)
		{
			for (nIndex = 0; nIndex < pSkeleton->m_arrIndexPackage[0]->m_nIndexesCount; nIndex++)
			{
				skeletonIndeces.push_back(pSkeleton->m_arrIndexPackage[0]->m_pIndexes[nIndex]);
			}
		}
		else {
			for (nIndex = 0; nIndex < pSkeleton->m_arrIndexPackage[0]->m_nIndexesCount; nIndex++)
			{
				unsigned int * pValue = (unsigned int*)&pSkeleton->m_arrIndexPackage[0]->m_pIndexes[nIndex * 2];
				skeletonIndeces.push_back(*pValue);
			}
		}

		// Normal
		buildAttributes[semantic] = std::vector<float>();
		if (pVertexDataPackage->m_pNormals != NULL)
		{
			semantic = "NORMAL";
			unsigned int nSize = nVertexCount * 3;
			float* pNormal = new float[nSize];
			for (size_t i = 0; i < nSize; i++)
			{
				pNormal[i] = pVertexDataPackage->m_pNormals[i];
			}

			mapTempAccessor[semantic] = new GLTF::Accessor(GLTF::Accessor::Type::VEC3, GLTF::Constants::WebGL::FLOAT, (unsigned char*)pNormal, nVertexCount,
				GLTF::Constants::WebGL::ARRAY_BUFFER);
			pPrimitive->attributes[semantic] = NULL;
			semanticIndices[semantic] = pNormal;
		}
		//SecondColor
		buildAttributes[semantic] = std::vector<float>();
		if (!pSkeleton->m_arrIDInfo.empty())
		{
			semantic = "_BATCHID";
			unsigned int nSize = nVertexCount;
			unsigned int* pSecondVertexColor = new unsigned int[nSize];
			for (size_t i = 0; i < pSkeleton->m_arrIDInfo.size(); i++)
			{
				IDInfo* pIDInfo = pSkeleton->m_arrIDInfo[i];
				if (pIDInfo == nullptr)
				{
					continue;
				}

				for (size_t j = 0; j < pIDInfo->m_arrVertexColorOffsetAndCount.size(); j++)
				{
					std::pair<int, int> pair = pIDInfo->m_arrVertexColorOffsetAndCount[j];
					for (size_t k = 0; k < pair.second; k++)
					{
						pSecondVertexColor[pair.first + k] = pIDInfo->m_nID;
					}
				}
			}

			mapTempAccessor[semantic] = new GLTF::Accessor(GLTF::Accessor::Type::SCALAR, GLTF::Constants::WebGL::UNSIGNED_INT, (unsigned char*)pSecondVertexColor, nSize,
				GLTF::Constants::WebGL::ARRAY_BUFFER);

			pPrimitive->attributes[semantic] = NULL;
			semanticIndices[semantic] = pSecondVertexColor;
		}

		buildAttributes[semantic] = std::vector<float>();
		if (pVertexDataPackage->m_pVertexColor != NULL)
		{
			unsigned int nColorSize = pVertexDataPackage->m_nVertexColorCount;
			float* pColor = new float[nColorSize * 3];
			for (unsigned int i = 0; i < nColorSize; i++)
			{
				ColorValue color = ConvertUintColor2ColorValue3D(pVertexDataPackage->m_pVertexColor[i]);

				float r = color.r;
				float g = color.g;
				float b = color.b;
				float a = color.a;
				if (!(pPrimitive->material->hasTexture()))
				{
					if (r == 0 && g == 0 && b == 0)
					{
						r = 1;
						g = 1;
						b = 1;
						a = 1;
					}
					packColladaColor(COLLADAFW::Color(r, g, b, a), ((GLTF::MaterialPBR*)pPrimitive->material)->metallicRoughness->baseColorFactor);
					if (a < 1.0)
					{
						((GLTF::MaterialPBR*)pPrimitive->material)->alphaMode = "BLEND";
					}
				}
				pColor[3 * i] = r;
				pColor[3 * i + 1] = g;
				pColor[3 * i + 2] = b;
			}
			semantic = "COLOR_0";
			buildAttributes[semantic] = std::vector<float>();

			mapTempAccessor[semantic] = new GLTF::Accessor(GLTF::Accessor::Type::VEC3, GLTF::Constants::WebGL::FLOAT,
				(unsigned char*)pColor, nColorSize, GLTF::Constants::WebGL::ARRAY_BUFFER);
			pPrimitive->attributes[semantic] = (GLTF::Accessor*)NULL;
			semanticIndices[semantic] = pColor;
		}


		//TexCoords
		buildAttributes[semantic] = std::vector<float>();
		for (int t = 0; t < SMSCN_MAX_TEXTURE_COORD_SETS; t++)
		{
			if (pVertexDataPackage->m_pTexCoords[t] == 0)
			{
				break;
			}
			unsigned int nTexCoordDim = pVertexDataPackage->m_nTexDimensions[t];
			unsigned int nTexcoordCount = pVertexDataPackage->m_TexCoordCount[t];
			float* pTex2d = NULL;
			if (nTexCoordDim == 3)
			{
				semantic = "TEXCOORD_" + std::to_string(t);
				buildAttributes[semantic] = std::vector<float>();
				float* pTexCoord3d = pVertexDataPackage->m_pTexCoords[t];

				pTex2d = new float[nTexcoordCount * 2];
				unsigned int nSize = nTexcoordCount * nTexCoordDim;
				unsigned int nGIndex = 0;
				for (unsigned int jj = 0; jj < nSize; jj += nTexCoordDim)
				{
					float txDX = pTexCoord3d[jj];
					float txDY = pTexCoord3d[jj + 1];
					float txDZ = pTexCoord3d[jj + 2];

					float fXTran, fYTran, fScale;
					if (EQUAL(txDZ, 0.0) || EQUAL(txDZ, -99999.0) || txDZ < -99900.0 || EQUAL(txDZ, -9999.0) || txDZ < -9999.0)
					{
						pTex2d[nGIndex] = txDX;
						pTex2d[nGIndex + 1] = txDY;
						nGIndex += 2;
						continue;
					}

					GetMatrixFromPValue(txDZ, fXTran, fYTran, fScale);

					Matrix4d mat = MatrixUtils::Scaling(fScale, fScale, 1.0);
					mat.SetTrans(Vector3d(fXTran * fScale, fYTran * fScale, 0.0));

					Vector3d vecT(txDX, txDY, 1.0);
					vecT = vecT.MultiplyMatrix(mat);

					pTex2d[nGIndex] = vecT.x;
					pTex2d[nGIndex + 1] = vecT.y;

					nGIndex += 2;
				}
			}

			else if (nTexCoordDim == 2)
			{
				semantic = "TEXCOORD_" + std::to_string(t);
				buildAttributes[semantic] = std::vector<float>();
				float* pTexCoord3d = pVertexDataPackage->m_pTexCoords[t];

				pTex2d = new float[nTexcoordCount * 2];
				unsigned int nSize = nTexcoordCount * nTexCoordDim;
				unsigned int nGIndex = 0;
				for (unsigned int jj = 0; jj < nSize; jj += nTexCoordDim)
				{
					float txDX = pTexCoord3d[jj];
					float txDY = pTexCoord3d[jj + 1];

					pTex2d[nGIndex] = txDX;
					pTex2d[nGIndex + 1] = txDY;
					nGIndex += 2;
				}
			}
			mapTempAccessor[semantic] = new GLTF::Accessor(GLTF::Accessor::Type::VEC2, GLTF::Constants::WebGL::FLOAT, (unsigned char*)pTex2d, nTexcoordCount,
				GLTF::Constants::WebGL::ARRAY_BUFFER);
            pPrimitive->attributes[semantic] = NULL;
			semanticIndices[semantic] = pTex2d;
		}

		for (unsigned int j = 0; j < count; j++)
		{
			std::string attributeId;
			for (const auto& attr : mapTempAccessor)
			{
				semantic = attr.first;
				attributeId += BuildAttributeId2(attr.second, skeletonIndeces[j]);
			}

			std::map<std::string, unsigned int>::iterator search = attributeIndicesMapping.find(attributeId);
			if (search != attributeIndicesMapping.end())
			{
				gltfIndeces.push_back(search->second);
			}
			else
			{
				for (const auto& entry : buildAttributes)
				{
					semantic = entry.first;
					GLTF::Accessor* vertexData = mapTempAccessor[semantic];
					unsigned int numberOfComponents = vertexData->getNumberOfComponents();
					bool flipY = false;
					bool position = false;
					if (semantic.find("TEXCOORD") == 0)
					{
						numberOfComponents = 2;
						flipY = true;
					}
					unsigned int semanticIndex;

					unsigned short*  pUshort = (unsigned short*)(semanticIndices[semantic]);
					semanticIndex = skeletonIndeces[j];

					if (semantic == "POSITION")
					{
						position = true;
						mapping.push_back(semanticIndex);
					}

					unsigned int stride = numberOfComponents;
					for (unsigned int k = 0; k < numberOfComponents; k++)
					{
						float value = GetMeshVertexDataAtIndex2(vertexData, semanticIndex, k);
						if (flipY && k == 1)
						{
							value = 1 - value;
						}
						if (position)
						{
							value = value * writer->_assetScale;
						}
						buildAttributes[semantic].push_back(value);
					}
				}
				attributeIndicesMapping[attributeId] = index;
				gltfIndeces.push_back(index);
				index++;
			}
			totalVertexCount++;
			vertexCount++;
		}

		// Create attribute accessors
		for (const auto& entry : buildAttributes)
		{
			std::string semantic = entry.first;
			std::vector<float> attributeData = entry.second;
			GLTF::Accessor::Type type = GLTF::Accessor::Type::VEC3;
			if (semantic.find("TEXCOORD") == 0)
			{
				type = GLTF::Accessor::Type::VEC2;
			}
			else if (semantic.find("_BATCHID") == 0)
			{
				type = GLTF::Accessor::Type::SCALAR;
			}
			else if (semantic.find("NORMAL") == 0)
			{
				//continue;
				type = GLTF::Accessor::Type::VEC3;
			}
			GLTF::Accessor* accessor = new GLTF::Accessor(type, GLTF::Constants::WebGL::FLOAT, (unsigned char*)&attributeData[0], attributeData.size() / GLTF::Accessor::getNumberOfComponents(type), GLTF::Constants::WebGL::ARRAY_BUFFER);
			pPrimitive->attributes[semantic] = accessor;
		}

		positionMapping[pPrimitive] = mapping;

		GLTF::Accessor* indices = NULL;
		if (index < 65536)
		{
			std::vector<unsigned short> unsignedShortIndices(gltfIndeces.begin(), gltfIndeces.end());
			indices = new GLTF::Accessor(GLTF::Accessor::Type::SCALAR, GLTF::Constants::WebGL::UNSIGNED_SHORT, (unsigned char*)&unsignedShortIndices[0], unsignedShortIndices.size(), GLTF::Constants::WebGL::ELEMENT_ARRAY_BUFFER);
		}
		else
		{
			indices = new GLTF::Accessor(GLTF::Accessor::Type::SCALAR, GLTF::Constants::WebGL::UNSIGNED_INT, (unsigned char*)&gltfIndeces[0], gltfIndeces.size(), GLTF::Constants::WebGL::ELEMENT_ARRAY_BUFFER);
		}

		pPrimitive->indices = indices;
		pMesh->primitives.push_back(pPrimitive);

		writer->_meshMaterialPrimitiveMapping[meshId] = primitiveMaterialMapping;
		writer->_meshPositionMapping[meshId] = positionMapping;
		writer->_meshInstances[meshId] = pMesh;
		for (auto& entry : mapTempAccessor)
		{
			GLTF::Accessor* pAccessor = (GLTF::Accessor*)entry.second;
			if (pAccessor->bufferView != NULL)
			{
				if (pAccessor->bufferView->buffer != NULL)
				{
					if (pAccessor->bufferView->buffer->data != NULL)
					{
						delete pAccessor->bufferView->buffer->data;
						pAccessor->bufferView->buffer->data = NULL;
					}
					delete pAccessor->bufferView->buffer;
					pAccessor->bufferView->buffer = NULL;
				}
				delete pAccessor->bufferView;
				pAccessor->bufferView = NULL;
			}
			delete pAccessor;
			pAccessor = NULL;
		}
		mapTempAccessor.clear();
		for (auto& entry : semanticIndices)
		{
			if (entry.second != NULL)
				delete entry.second;
			entry.second = NULL;
		}
		semanticIndices.clear();

		return true;
	}

	float B3DMWriter::GetMeshVertexDataAtIndex2(const GLTF::Accessor* pAccerror, unsigned int index, int idx)
	{
		unsigned char* pBuffer = pAccerror->bufferView->buffer->data;

		if (pAccerror->type == GLTF::Accessor::Type::VEC3)
		{
			if (pAccerror->componentType == GLTF::Constants::WebGL::FLOAT)
			{
				float* pBuffer = (float*)pAccerror->bufferView->buffer->data;
				return pBuffer[index * 3 + idx];
			}
		}
		else if (pAccerror->type == GLTF::Accessor::Type::VEC2)
		{
			if (pAccerror->componentType == GLTF::Constants::WebGL::FLOAT)
			{
				float* pBuffer = (float*)pAccerror->bufferView->buffer->data;
				return pBuffer[index * 2 + idx];
			}
		}
		else if (pAccerror->type == GLTF::Accessor::Type::SCALAR)
		{
			if (pAccerror->componentType == GLTF::Constants::WebGL::FLOAT)
			{
				float* pBuffer = (float*)pAccerror->bufferView->buffer->data;
				return pBuffer[index + idx];
			}
			else if (pAccerror->componentType == GLTF::Constants::WebGL::UNSIGNED_INT)
			{
				unsigned int* pBuffer = (unsigned int*)pAccerror->bufferView->buffer->data;
				return pBuffer[index + idx];
			}
		}
	}

	void B3DMWriter::GetMatrixFromPValue(float nZ, float & XTran, float & YTran, float& scale)
	{
		float SHIFT_LEFT8 = 256.0;
		float SHIFT_RIGHT8 = 1.0 / 256.0;

		if (nZ <= 0.0)
		{
			return;
		}
		float nDel8 = floor(nZ * SHIFT_RIGHT8);
		float nDel16 = floor(nDel8 * SHIFT_RIGHT8);
		YTran = nZ - nDel8 * SHIFT_LEFT8;
		XTran = nDel8 - nDel16 * SHIFT_LEFT8;
		scale = 1.0 / pow(2.0, nDel16);
	}

	ColorValue B3DMWriter::ConvertUintColor2ColorValue3D(const unsigned int& color)
	{
		ColorValue verColor;
		verColor.a = ColorUtils::AlphaValue(color) / 255.0;
		verColor.r = ColorUtils::RedValue(color) / 255.0;
		verColor.g = ColorUtils::GreenValue(color) / 255.0;
		verColor.b = ColorUtils::BlueValue(color) / 255.0;
		return verColor;
	}

	std::string B3DMWriter::BuildAttributeId2(const GLTF::Accessor* pAccerror, unsigned int index)
	{
		std::string id;
		if (pAccerror->type == GLTF::Accessor::Type::VEC3)
		{
			if (pAccerror->componentType == GLTF::Constants::WebGL::FLOAT)
			{
				float* pBuffer = (float*)pAccerror->bufferView->buffer->data;

				id += std::to_string(pBuffer[index * 3]) + ":";
				id += std::to_string(pBuffer[index * 3 + 1]) + ":";
				id += std::to_string(pBuffer[index * 3 + 2]) + ":";

			}
		}
		else if (pAccerror->type == GLTF::Accessor::Type::VEC2)
		{
			if (pAccerror->componentType == GLTF::Constants::WebGL::FLOAT)
			{
				float* pBuffer = (float*)pAccerror->bufferView->buffer->data;

				id += std::to_string(pBuffer[index * 2]) + ":";
				id += std::to_string(pBuffer[index * 2 + 1]) + ":";
			}
		}
		else if (pAccerror->type == GLTF::Accessor::Type::SCALAR)
		{
			if (pAccerror->componentType == GLTF::Constants::WebGL::FLOAT)
			{
				float* pBuffer = (float*)pAccerror->bufferView->buffer->data;
				id += std::to_string(pBuffer[index]) + ":";
			}
			else if (pAccerror->componentType == GLTF::Constants::WebGL::UNSIGNED_INT)
			{
				unsigned int* pBuffer = (unsigned int*)pAccerror->bufferView->buffer->data;
				id += std::to_string(pBuffer[index]) + ":";
			}
		}
		return id;
	}

	void B3DMWriter::clear(GLTF::Asset* pAsset)
	{
		// 删除
		if (pAsset == NULL) return;

		if (pAsset->globalSampler != NULL)
		{
			delete pAsset->globalSampler;
			pAsset->globalSampler = NULL;
		}

		if (pAsset->metadata != NULL)
		{
			delete pAsset->metadata;
			pAsset->metadata = NULL;
		}

		std::vector<GLTF::Buffer*> buffers = getAssetAllBuffers(pAsset);
		for (GLTF::Buffer* pBuffer : buffers)
		{
			if (pBuffer != NULL)
			{
				if (pBuffer->data != NULL)
				{
					delete[] pBuffer->data;
					pBuffer->data = NULL;
				}
				delete pBuffer;
				pBuffer = NULL;
			}
		}
		GLTFObjectDeleterStdVector(getAssetAllBufferViews(pAsset));
		GLTFObjectDeleterStdVector(pAsset->getAllImages());
		GLTFObjectDeleterStdVector(pAsset->getAllTextures());
		GLTFObjectDeleterStdVector(pAsset->getAllShaders());
		GLTFObjectDeleterStdVector(pAsset->getAllPrograms());
		GLTFObjectDeleterStdVector(pAsset->getAllTechniques());
		GLTFObjectDeleterStdVector(pAsset->getAllMaterials());
		GLTFObjectDeleterStdVector(pAsset->getAllAccessors());
		GLTFObjectDeleterStdVector(pAsset->getAllPrimitives());
		GLTFObjectDeleterStdVector(pAsset->getAllMeshes());

		auto skins = pAsset->getAllSkins();
		auto nodes = pAsset->getAllNodes();

		GLTFObjectDeleterStdVector(std::move(skins));
		GLTFObjectDeleterStdVector(std::move(nodes));

		GLTFObjectDeleterStdVector(std::move(pAsset->animations));
		GLTFObjectDeleterStdVector(std::move(pAsset->scenes));
	}

	std::vector<GLTF::Buffer*> B3DMWriter::getAssetAllBuffers(GLTF::Asset* pAsset)
	{
		std::vector<GLTF::Buffer*> arrBufferViews;
		std::set<GLTF::Buffer*> arrUniqueBufferViews;
		if (pAsset != NULL)
		{
			for (GLTF::BufferView* pBufferView : getAssetAllBufferViews(pAsset))
			{
				GLTF::Buffer* pBuffer = pBufferView->buffer;
				if (pBuffer == NULL)
				{
					continue;
				}
				if (arrUniqueBufferViews.find(pBuffer) == arrUniqueBufferViews.end())
				{
					arrBufferViews.push_back(pBuffer);
					arrUniqueBufferViews.insert(pBuffer);
				}
			}
		}
		return arrBufferViews;
	}

	std::vector<GLTF::BufferView*> B3DMWriter::getAssetAllBufferViews(GLTF::Asset* pAsset)
	{
		std::vector<GLTF::BufferView*> arrBufferViews;
		std::set<GLTF::BufferView*> arrUniqueBufferViews;
		if (pAsset != NULL)
		{
			for (GLTF::Accessor* accessor : pAsset->getAllAccessors())
			{
				GLTF::BufferView* bufferView = accessor->bufferView;
				if (bufferView == NULL)
				{
					continue;;
				}
				if (arrUniqueBufferViews.find(bufferView) == arrUniqueBufferViews.end())
				{
					arrBufferViews.push_back(bufferView);
					arrUniqueBufferViews.insert(bufferView);
				}
			}
		}
		for (GLTF::Image*pImage : pAsset->getAllImages())
		{
			GLTF::BufferView* bufferView = pImage->bufferView;
			if (bufferView == NULL)
			{
				continue;;
			}
			if (arrUniqueBufferViews.find(bufferView) == arrUniqueBufferViews.end())
			{
				arrBufferViews.push_back(bufferView);
				arrUniqueBufferViews.insert(bufferView);
			}
		}

		return arrBufferViews;
	}

	void B3DMWriter::removeImage(GLTF::Image* pImage)
	{
		if (pImage->data != NULL)
		{
			delete[] pImage->data;
			pImage->data = NULL;
		}
	}

	void B3DMWriter::removeAccessor(GLTF::Accessor* pAccessor)
	{
		if (pAccessor->min != NULL)
		{
			delete[] pAccessor->min;
			pAccessor->min = NULL;
		}
		if (pAccessor->max != NULL)
		{
			delete[] pAccessor->max;
			pAccessor->max = NULL;
		}
	}

	void B3DMWriter::removeMaterial(GLTF::Material* pMaterial)
	{
		GLTF::Material::Values* values = pMaterial->values;
		if (values->ambient != NULL)
		{
			delete[] values->ambient;
			values->ambient = NULL;
		}
		if (values->diffuse != NULL)
		{
			delete[] values->diffuse;
			values->diffuse = NULL;
		}
		if (values->emission != NULL)
		{
			delete[] values->emission;
			values->emission = NULL;
		}
		if (values->specular != NULL)
		{
			delete[] values->specular;
			values->specular = NULL;
		}
		if (values->shininess != NULL)
		{
			delete[] values->shininess;
			values->shininess = NULL;
		}
		if (values->transparency != NULL)
		{
			delete[] values->transparency;
			values->transparency = NULL;
		}
		delete values;
		values = NULL;
	}

	template<typename T>
	void B3DMWriter::GLTFObjectDeleterStdVector(std::vector<T*>&&v)
	{
		for (T* t : v)
		{
			if (t != NULL)
			{
				if (t->typeName() == "material")
					removeMaterial((GLTF::Material*)t);
				if (t->typeName() == "image")
					removeImage((GLTF::Image*)t);
				if (t->typeName() == "accessor")
					removeAccessor((GLTF::Accessor*)t);
				t = NULL;
			}
		}
		v.clear();
	}
}