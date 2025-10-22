#if !defined(GLTFPARSER_H__A8CADE89_102A_4CB3_AAAD_A14663594035__INCLUDED_)
#define GLTFPARSER_H__A8CADE89_102A_4CB3_AAAD_A14663594035__INCLUDED_

#pragma once
#include "Common.h"
#include "GLTFContent.h"
#include "stdafx.h"

#include "document.h"
#include "istreamwrapper.h"
#include "prettywriter.h"
#include "stringbuffer.h"

#include <set>

namespace S3MB
{
	class STK_API GLTFParser
	{
	public:
		static GLTFTileInfos_1* ParseGLTFTileInfos_1(const std::wstring& strPath, unsigned char* pBuffer, AxisUpType axisUpType);
		static void* GetAttributeData(GLTFTileInfos_1* pTileInfos, std::wstring &strName, int & nDim);

		static GLTFTileInfos_2* ParseGLTFTileInfos_2(const std::wstring& strPath, unsigned char* pBuffer, unsigned int nBufferSize, std::wstring strImageDir, AxisUpType axisUpType);
		static void* GetAttributeData(GLTFTileInfos_2* pTileInfos, int index, int& nDim);

		// 解析glb文件
		static GLTFTileInfos_2* ParseGLBFile(const std::wstring& strPath);

		// gltf绘制类型转s3m绘制类型
		static OperationType ToDrawOperationType(unsigned int nType);

	private:
		// 读取文件
		static bool ReadGLTFFile(const std::wstring& strPath, rapidjson::Document& doc);
		static bool ReadGLBFile(const std::wstring& strPath, rapidjson::Document& doc, GLBChunk& chunk);
		static bool ParseGLBHeaderAndChunks(const std::wstring& strPath, GLBHeader& header, GLBChunk& chunk0, GLBChunk& chunk1);

#pragma region GLTF1.0
		static bool ParseScene(GLTFTileInfos_1* pTileInfos, rapidjson::Document& doc);
		static bool ParseNode(GLTFTileInfos_1* pTileInfos, rapidjson::Document& doc, const char* nodeName);
		static void ProcessGLTFNode(GLTFTileInfos_1* pTileInfos, rapidjson::Value& nodes, const char* nodeName, Matrix4d& matGeode, std::wstring parentName);
		static bool ParseMesh(GLTFTileInfos_1* pTileInfos, rapidjson::Document& doc);
		static bool ParseAccessors(GLTFTileInfos_1* pTileInfos, rapidjson::Document& doc);
		static bool ParseBufferViews(GLTFTileInfos_1* pTileInfos, rapidjson::Document& doc);
		static bool ParseMaterials(GLTFTileInfos_1* pTileInfos, rapidjson::Document& doc);
		static bool ParseTextures(GLTFTileInfos_1* pTileInfos, rapidjson::Document& doc);
		static bool ParseImages(GLTFTileInfos_1* pTileInfos, rapidjson::Document& doc);
		static bool ParseBuffers(GLTFTileInfos_1* pTileInfos, rapidjson::Document& doc);
		static bool ParseExtensions(GLTFTileInfos_1* pTileInfos, rapidjson::Document& doc);
#pragma endregion

#pragma region GLTF2.0
		static bool ParseNode(GLTFTileInfos_2* pTileInfos, rapidjson::Document& doc);
		static bool ParseNode_glb(GLTFTileInfos_2* pTileInfos, rapidjson::Document& doc);
		static void ProcessGLTFNode(GLTFTileInfos_2* pTileInfos, rapidjson::Value& value, int index, Matrix4d& matGeode, unsigned int& parentIndex);
		static void ProcessGLTFNode_glb(GLTFTileInfos_2* pTileInfos, rapidjson::Value& value, int index, Matrix4d& matGeode, int parentIndex, std::set<int>& setTotalIndex);
		static bool ParseMesh(GLTFTileInfos_2* pTileInfos, rapidjson::Document& doc);
		static bool ParseAccessors(GLTFTileInfos_2* pTileInfos, rapidjson::Document& doc);
		static bool ParseBufferViews(GLTFTileInfos_2* pTileInfos, rapidjson::Document& doc);
		static bool ParseMaterials(GLTFTileInfos_2* pTileInfos, rapidjson::Document& doc);
		static bool ParseTextures(GLTFTileInfos_2* pTileInfos, rapidjson::Document& doc);
		static bool ParseImages(GLTFTileInfos_2* pTileInfos, rapidjson::Document& doc);
		static bool ParseBuffers(GLTFTileInfos_2* pTileInfos, rapidjson::Document& doc);
		static bool ParseBuffers(GLTFTileInfos_2* pTileInfos, GLBChunk& chunk);
		static bool ParseExtensions(GLTFTileInfos_2* pTileInfos, rapidjson::Document& doc);

		static void DealGLTFFilePath(GLTFImage& gltfImage, std::wstring strFilePath);
#pragma endregion
	};
}

#endif