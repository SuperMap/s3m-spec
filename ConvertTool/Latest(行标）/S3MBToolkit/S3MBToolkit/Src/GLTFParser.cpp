#include "GLTFParser.h"
#include "s3m/include/MemoryStream.h"
#include "S3MBUtils.h"
#include "Utils/Utils.h"

#include <assert.h>
#include <fstream>

#define IDENTIFY_GLB (0x46546C67)
#define IDENTIFY_JSON (0x4E4F534A)
#define IDENTIFY_BIN (0x004E4942)
#define GLB_VERSIONV2 2
#define HEADER_SIZE 12

namespace S3MB
{
	GLTFTileInfos_1* GLTFParser::ParseGLTFTileInfos_1(const std::wstring& strPath, unsigned char* pBuffer, AxisUpType axisUpType)
	{
		rapidjson::Document doc;
		if (!ReadGLTFFile(strPath, doc))
		{
			return nullptr;
		}

		GLTFTileInfos_1* pTileInfos = new GLTFTileInfos_1();
		pTileInfos->m_nAxisUpType = axisUpType;

		// Scene
		ParseScene(pTileInfos, doc);

		// Mesh
		ParseMesh(pTileInfos, doc);

		// Accessors
		ParseAccessors(pTileInfos, doc);

		// BufferViews
		ParseBufferViews(pTileInfos, doc);

		// Materials
		ParseMaterials(pTileInfos, doc);

		// Textures
		ParseTextures(pTileInfos, doc);

		// Images
		ParseImages(pTileInfos, doc);

		// Buffer
		ParseBuffers(pTileInfos, doc);

		// externsions
		ParseExtensions(pTileInfos, doc);

		if (pTileInfos->m_mapBuffers.size() > 0)
		{
			pTileInfos->m_mapBuffers.begin()->second.m_pBuffer = pBuffer;
		}

		return pTileInfos;
	}

	void * GLTFParser::GetAttributeData(GLTFTileInfos_1* pTileInfos, std::wstring & strName, int & nDim)
	{
		if (strName.length() == 0)
		{
			return nullptr;
		}

		GLTFAccessor & gltfAccessor = pTileInfos->m_mapAccessors[strName];
		unsigned int nCount = gltfAccessor.m_nCount;
		std::wstring strDataType = gltfAccessor.m_strType;
		unsigned int nComponentType = gltfAccessor.m_nComponentType;
		unsigned int nByteOffset = gltfAccessor.m_nByteOffset;
		unsigned int nByteWidth = 4;
		unsigned int nConWidth = 0;
		if (StringUtil::CompareNoCase(strDataType, U("SCALAR")))
		{
			nConWidth = 1;
			nDim = 1;
		}
		else if (StringUtil::CompareNoCase(strDataType, U("VEC3")))
		{
			nConWidth = 3;
			nDim = 3;
		}
		else if (StringUtil::CompareNoCase(strDataType, U("VEC2")))
		{
			nConWidth = 2;
			nDim = 2;
		}
		else if (StringUtil::CompareNoCase(strDataType, U("VEC4")))
		{
			nConWidth = 4;
			nDim = 2;
		}

		if (nComponentType == 5120 || nComponentType == 5121)
		{
			nByteWidth = 1;
		}
		else if (nComponentType == 5122 || nComponentType == 5123)
		{
			nByteWidth = 2;
		}
		else if (nComponentType == 5126)
		{
			nByteWidth = 4;
		}

		if (gltfAccessor.m_strBufferViewName.length() == 0 || nConWidth == 0)
		{
			return nullptr;
		}


		GLTFBufferView & gltfBufferView = pTileInfos->m_mapBufferViews[gltfAccessor.m_strBufferViewName];

		// 获取buffer
		unsigned int nBufferSize = nCount * nConWidth * nByteWidth;
		unsigned char* pBuffer = new unsigned char[nBufferSize];
		memcpy(pBuffer, pTileInfos->m_mapBuffers[gltfBufferView.m_strBufferName].m_pBuffer + gltfBufferView.m_nByteOffset + nByteOffset, nBufferSize);
		return pBuffer;
	}

	GLTFTileInfos_2* GLTFParser::ParseGLTFTileInfos_2(const std::wstring& strPath, unsigned char* pBuffer, unsigned int nBufferSize, std::wstring strImageDir, AxisUpType axisUpType)
	{
		rapidjson::Document doc;
		if (!ReadGLTFFile(strPath, doc))
		{
			return nullptr;
		}

		GLTFTileInfos_2* pTileInfos = new GLTFTileInfos_2();
		pTileInfos->m_nAxisUpType = axisUpType;

		// Node
		ParseNode(pTileInfos, doc);

		// Mesh
		ParseMesh(pTileInfos, doc);

		// Accessors
		ParseAccessors(pTileInfos, doc);

		// BufferViews
		ParseBufferViews(pTileInfos, doc);

		// Materials
		ParseMaterials(pTileInfos, doc);

		// Textures
		ParseTextures(pTileInfos, doc);

		// Images
		ParseImages(pTileInfos, doc);

		// Buffer 
		ParseBuffers(pTileInfos, doc);

		// externsions
		ParseExtensions(pTileInfos, doc);

		//处理纹理外挂路径，如果非纹理外挂，会判断返回
		for (int i = 0; i < pTileInfos->m_mapImages.size(); i++)
		{
			DealGLTFFilePath(pTileInfos->m_mapImages[i], strImageDir);
		}

		std::wstring strKey = U("data:application/octet-stream;base64,");
		for (int i = 0; i < pTileInfos->m_mapBuffers.size(); i++)
		{
			std::wstring strFile = pTileInfos->m_mapBuffers[i].m_strFile;
			if (strFile == U(""))
			{
				if (pTileInfos->m_mapBuffers[i].m_nByteLength > nBufferSize)
				{
					delete pTileInfos;
					pTileInfos = nullptr;
					return nullptr;
				}

				unsigned char* pTempBuffer = pBuffer + 8;
				pTileInfos->m_mapBuffers[i].m_pBuffer = pTempBuffer;
				pTileInfos->m_mapBuffers[i].m_bManageBuffer = false;
			}
			else if (strFile.find(strKey) < strFile.length()) //从第一个位置匹配
			{
				strFile = strFile.substr(strKey.length());
				pTileInfos->m_mapBuffers[i].m_strEncodeBuffer = strFile;
				pTileInfos->m_mapBuffers[i].m_strFile = U("");
				pTileInfos->m_mapBuffers[i].m_strEncodeMode = U("base64");

				//做Base64解压			
				std::wstring strDecode = BaseUtils::Base64Decode(pTileInfos->m_mapBuffers[i].m_strEncodeBuffer, pTileInfos->m_mapBuffers[i].m_strEncodeBuffer.length());
				std::string strBuffer = StringUtil::UNICODE_to_UTF8(strDecode);
				const char * bufferOut = strBuffer.c_str();

				unsigned char* pTempBuffer = new unsigned char[strBuffer.size()];
				memcpy(pTempBuffer, bufferOut, strBuffer.size());

				pTileInfos->m_mapBuffers[i].m_pBuffer = pTempBuffer;
				pTileInfos->m_mapBuffers[i].m_bManageBuffer = true;
			}
			else
			{
				std::wstring strBufferFile = strImageDir + pTileInfos->m_mapBuffers[i].m_strFile;
				std::string strBuffer = StringUtil::UNICODE_to_UTF8(strBufferFile);

				unsigned int size = pTileInfos->m_mapBuffers[i].m_nByteLength;
				unsigned char* extendBuffer = new unsigned char[size];
				FILE* bufferFile = fopen(strBuffer.c_str(), "rb");

				unsigned int bytesRead = fread(extendBuffer, sizeof(unsigned char), size, bufferFile);
				fclose(bufferFile);

				pTileInfos->m_mapBuffers[i].m_pBuffer = extendBuffer;
				pTileInfos->m_mapBuffers[i].m_bManageBuffer = true;
			}
		}

		return pTileInfos;
	}

	void* GLTFParser::GetAttributeData(GLTFTileInfos_2* pTileInfos, int index, int & nDim)
	{
		if (index == -1)
		{
			return nullptr;
		}

		const GLTFAccessor& gltfAccessor = pTileInfos->m_mapAccessors[index];
		unsigned int nCount = gltfAccessor.m_nCount;
		std::wstring strDataType = gltfAccessor.m_strType;
		unsigned int nComponentType = gltfAccessor.m_nComponentType;
		unsigned int nByteOffset = gltfAccessor.m_nByteOffset;
		unsigned int nByteWidth = 4;
		unsigned int nConWidth = 0;
		if (StringUtil::CompareNoCase(strDataType, U("SCALAR")))
		{
			nConWidth = 1;
			nDim = 1;
		}
		else if (StringUtil::CompareNoCase(strDataType, U("VEC3")))
		{
			nConWidth = 3;
			nDim = 3;
		}
		else if (StringUtil::CompareNoCase(strDataType, U("VEC2")))
		{
			nConWidth = 2;
			nDim = 2;
		}
		else if (StringUtil::CompareNoCase(strDataType, U("VEC4")))
		{
			nConWidth = 4;
			nDim = 4;
		}

		if (nComponentType == 5120 || nComponentType == 5121)
		{
			nByteWidth = 1;
		}
		else if (nComponentType == 5122 || nComponentType == 5123)
		{
			nByteWidth = 2;
		}
		else if (nComponentType == 5126 || nComponentType == 5125)
		{
			nByteWidth = 4;
		}

		if (gltfAccessor.m_nBufferViewIndex == -1 || nConWidth == 0)
		{
			return nullptr;
		}

		const GLTFBufferView& gltfBufferView = pTileInfos->m_mapBufferViews[gltfAccessor.m_nBufferViewIndex];

		// 获取buffer
		unsigned int nBufferSize = nCount * nConWidth * nByteWidth;
		unsigned char* pBuffer = new unsigned char[nBufferSize];
		const GLTFBuffer& gltfBuffer = pTileInfos->m_mapBuffers[gltfBufferView.m_nBufferIndex];

		if (gltfBufferView.m_nByteStride == 0)
		{
			memcpy(pBuffer, gltfBuffer.m_pBuffer + gltfBufferView.m_nByteOffset + nByteOffset, nBufferSize);
		}
		else if (gltfBufferView.m_nByteStride != 0)
		{
			for (int i = 0; i < nCount; i++)
			{
				memcpy(pBuffer + i * nConWidth * nByteWidth, gltfBuffer.m_pBuffer + i * gltfBufferView.m_nByteStride + gltfBufferView.m_nByteOffset + nByteOffset, nConWidth * nByteWidth);
			}
		}

		return pBuffer;
	}

	GLTFTileInfos_2* GLTFParser::ParseGLBFile(const std::wstring& strPath)
	{
		rapidjson::Document doc;
		GLBChunk chunk;
		if (!ReadGLBFile(strPath, doc, chunk))
		{
			return nullptr;
		}

		GLTFTileInfos_2* pTileInfos = new GLTFTileInfos_2();
		// Node
		ParseNode_glb(pTileInfos, doc);
		// Mesh
		ParseMesh(pTileInfos, doc);
		// Accessors
		ParseAccessors(pTileInfos, doc);
		// BufferViews
		ParseBufferViews(pTileInfos, doc);
		// Materials
		ParseMaterials(pTileInfos, doc);
		// Textures
		ParseTextures(pTileInfos, doc);
		// Images
		ParseImages(pTileInfos, doc);
		// Buffer 
		ParseBuffers(pTileInfos, chunk);
		return pTileInfos;
	}

	OperationType GLTFParser::ToDrawOperationType(unsigned int nType)
	{
		switch (nType)
		{
		case 0: return OT_POINT_LIST;
		case 1: return OT_LINE_LIST;
		case 3: return OT_LINE_STRIP;
		case 4: return OT_TRIANGLE_LIST;
		case 5: return OT_TRIANGLE_STRIP;
		case 6: return OT_TRIANGLE_FAN;
		default: return OT_TRIANGLE_LIST;
		}
	}

	bool GLTFParser::ReadGLTFFile(const std::wstring& strPath, rapidjson::Document& doc)
	{
		std::string path = StringUtil::UnicodeToANSI(strPath);
		std::ifstream ifs(path.c_str());
		rapidjson::IStreamWrapper isw(ifs);

		doc.ParseStream(isw);
		ifs.close();
		if (doc.HasParseError())
		{
			//rapidjson::ParseErrorCode code = doc.GetParseError();
			assert(false);
			return false;
		}

		return true;
	}

	bool GLTFParser::ReadGLBFile(const std::wstring& strPath, rapidjson::Document& doc, GLBChunk& chunk)
	{
		std::wstring strExt = StringUtil::GetExt(strPath);
		GLBHeader header;
		GLBChunk chunk0;
		if (!ParseGLBHeaderAndChunks(strPath, header, chunk0, chunk))
		{
			return false;
		}

		std::string content = std::string(reinterpret_cast<const char*>(chunk0.m_pData), chunk0.m_nLength);
		if (doc.Parse<0>(content.c_str()).HasParseError())
		{
			//rapidjson::ParseErrorCode code = doc.GetParseError();
			assert(false);
			return false;
		}

		return true;
	}

	bool GLTFParser::ParseGLBHeaderAndChunks(const std::wstring& strPath, GLBHeader& header, GLBChunk& chunk0, GLBChunk& chunk1)
	{
		unsigned char* pBuffer = nullptr;
		unsigned long long nBufferSize = 0;
		if (!FileUtils::LoadBuffer(strPath, pBuffer, nBufferSize))
		{
			delete[] pBuffer;
			pBuffer = nullptr;
			return false;
		}

		MemoryStream stream;
		stream.Init(pBuffer, nBufferSize, true);

		stream >> header.m_nMagic;
		if (header.m_nMagic != IDENTIFY_GLB)
		{
			assert(false);
			return false;
		}

		stream >> header.m_nVersion;
		stream >> header.m_nLength;

		if (header.m_nVersion == GLB_VERSIONV2 && header.m_nLength > HEADER_SIZE)
		{
			//chunk 0
			stream >> chunk0.m_nLength;
			stream >> chunk0.m_nType;
			if (chunk0.m_nLength > 0 && chunk0.m_nType == IDENTIFY_JSON)
			{
				delete[] chunk0.m_pData;
				chunk0.m_pData = new unsigned char[chunk0.m_nLength];
				stream.Load(chunk0.m_pData, chunk0.m_nLength);
			}
		}

		// 这里的8表示chunk0 length + type的8个字节
		if (header.m_nVersion == GLB_VERSIONV2 && header.m_nLength > (HEADER_SIZE + chunk0.m_nLength + 8))
		{
			//chunk1
			stream >> chunk1.m_nLength;
			stream >> chunk1.m_nType;
			if (chunk1.m_nLength > 0 && chunk1.m_nType == IDENTIFY_BIN)
			{
				delete[] chunk1.m_pData;
				chunk1.m_pData = new unsigned char[chunk1.m_nLength];
				stream.Load(chunk1.m_pData, chunk1.m_nLength);
			}
		}

		return true;
	}

	bool GLTFParser::ParseScene(GLTFTileInfos_1* pTileInfos, rapidjson::Document& doc)
	{
		rapidjson::Value& scene = doc["scene"];
		if (scene.Empty())
			return false;

		rapidjson::Value& scenes = doc["scenes"];
		if (scenes.Empty())
			return false;

		rapidjson::Value& nodes = scenes[scene.GetString()]["nodes"];
		if (nodes.Empty() && !nodes.IsArray())
			return false;

		for (unsigned int i = 0; i < nodes.Size(); i++)
		{
			const char* nodeName = nodes[i].GetString();
			ParseNode(pTileInfos, doc, nodeName);
		}

		return true;
	}

	bool GLTFParser::ParseNode(GLTFTileInfos_1* pTileInfos, rapidjson::Document& doc, const char* nodeName)
	{
		rapidjson::Value& nodes = doc["nodes"];
		if (nodes.Empty())
			return false;

		Matrix4d matGeode = Matrix4d::IDENTITY;
		ProcessGLTFNode(pTileInfos, nodes, nodeName, matGeode, U("Root"));

		return true;
	}

	void GLTFParser::ProcessGLTFNode(GLTFTileInfos_1* pTileInfos, rapidjson::Value& nodes, const char* nodeName, Matrix4d& matGeode, std::wstring parentName)
	{
		rapidjson::Value& value = nodes[nodeName];

		rapidjson::Value name;
		if (value.HasMember("name"))
		{
			name = value["name"];
		}
		rapidjson::Value matrix;
		if (value.HasMember("matrix"))
		{
			matrix = value["matrix"];
		}
		rapidjson::Value meshs;
		if (value.HasMember("meshes"))
		{
			meshs = value["meshes"];
		}

		std::wstring strName;
		if (name.IsString())
		{
			std::string strTemp(name.GetString());
			strName = StringUtil::UTF8_to_UNICODE(strTemp);
		}

		Matrix4d tempMat = matGeode;
		if (matrix.IsArray())
		{
			Matrix4d mat = Matrix4d::IDENTITY;
			if (matrix.Size() == 16)
			{
				for (int r = 0; r < 4; r++)
				{
					mat[r][0] = matrix[r * 4 + 0].GetDouble();
					mat[r][1] = matrix[r * 4 + 1].GetDouble();
					mat[r][2] = matrix[r * 4 + 2].GetDouble();
					mat[r][3] = matrix[r * 4 + 3].GetDouble();
				}
			}

			int nNameLength = strName.length();
			if (strName.find(U("X_UP")) < nNameLength ||
				strName.find(U("Y_UP")) < nNameLength ||
				strName.find(U("Z_UP")) < nNameLength)
			{
				if (strName.find(U("Z_UP")) < nNameLength)
				{
					pTileInfos->m_nAxisUpType = AxisUpType::Z_UP;
				}
				else if (strName.find(U("Y_UP")) < nNameLength)
				{
					pTileInfos->m_nAxisUpType = AxisUpType::Y_UP;
				}
				else
				{
					pTileInfos->m_nAxisUpType = AxisUpType::X_UP;
				}
				mat = Matrix4d::IDENTITY;
			}

			tempMat = mat * tempMat;
		}

		if (!meshs.IsNull())
		{
			for (unsigned int n = 0; n < meshs.Size(); n++)
			{
				char* meshName = (char*)meshs[n].GetString();

				std::wstring strMeshName = StringUtil::UTF8_to_UNICODE(std::string(meshName));

				Matrix4d matR = Matrix4d::IDENTITY;
				if (pTileInfos->m_nAxisUpType == Y_UP)
				{
					matR = Matrix4d(
						1, 0, 0, 0,
						0, 0, 1, 0,
						0, -1, 0, 0,
						0, 0, 0, 1);
				}
				pTileInfos->m_mapMeshTOLocalView[strMeshName] = matR * tempMat;

				std::map<std::wstring, std::vector<std::wstring> >::iterator it = pTileInfos->m_mapMeshSet.find(parentName);
				if (it == pTileInfos->m_mapMeshSet.end())
				{
					pTileInfos->m_mapMeshSet[parentName] = std::vector<std::wstring>();
				}

				pTileInfos->m_mapMeshSet[parentName].push_back(strMeshName);
			}
		}

		rapidjson::Value children;
		if (value.HasMember("children"))
		{
			children = value["children"];
		}

		if (children.IsArray())
		{
			// 父节点名字
			std::wstring strNodeName = StringUtil::UTF8_to_UNICODE(std::string(nodeName));

			for (unsigned int n = 0; n < children.Size(); n++)
			{
				char* childNodeName = (char*)children[n].GetString();

				ProcessGLTFNode(pTileInfos, nodes, childNodeName, tempMat, strNodeName);
			}
		}
	}

	bool GLTFParser::ParseMesh(GLTFTileInfos_1* pTileInfos, rapidjson::Document& doc)
	{
		if (!doc.HasMember("meshes"))
		{
			return false;
		}

		rapidjson::Value& meshs = doc["meshes"];

		rapidjson::Value::ConstMemberIterator it = meshs.MemberBegin();
		while (it != meshs.MemberEnd())
		{
			// 创建骨架	
			rapidjson::Value key;
			rapidjson::Value value;

			rapidjson::Document::AllocatorType allocator;

			key.CopyFrom(it->name, allocator);
			value.CopyFrom(it->value, allocator);

			std::wstring strMeshName;
			if (key.IsString())
			{
				std::string strTemp(key.GetString());
				strMeshName = StringUtil::UTF8_to_UNICODE(strTemp);
			}

			if (!value.HasMember("primitives"))
			{
				it++;
				continue;
			}

			rapidjson::Value& primitives = value["primitives"];

			if (primitives.IsArray())
			{
				GLTFMesh mesh;
				for (unsigned int j = 0; j < primitives.Size(); j++)
				{
					// 顶点
					rapidjson::Value& primitive = primitives[j];
					if (primitive.HasMember("attributes"))
					{
						rapidjson::Value& attribute = primitive["attributes"];

						// 顶点
						if (attribute.HasMember("POSITION"))
						{
							std::string strTemp(attribute["POSITION"].GetString());
							std::wstring strPos = StringUtil::UTF8_to_UNICODE(strTemp);
							mesh.m_vecStrPos.push_back(strPos);
						}
						// 法线
						if (attribute.HasMember("NORMAL"))
						{
							std::string strTemp(attribute["NORMAL"].GetString());
							std::wstring strNormal = StringUtil::UTF8_to_UNICODE(strTemp);
							mesh.m_vecStrNormal.push_back(strNormal);
						}
						//顶点颜色
						if (attribute.HasMember("COLOR_0"))
						{
							std::string strTemp(attribute["COLOR_0"].GetString());
							std::wstring strColor = StringUtil::UTF8_to_UNICODE(strTemp);
							mesh.m_vecStrColor.push_back(strColor);
						}
						// 纹理(一重纹理)
						if (attribute.HasMember("TEXCOORD_0"))
						{
							std::string strTemp(attribute["TEXCOORD_0"].GetString());
							std::wstring strTex = StringUtil::UTF8_to_UNICODE(strTemp);
							mesh.m_vecStrTex.push_back(strTex);
						}
					}

					// 索引
					if (primitive.HasMember("indices"))
					{
						std::string strTemp(primitive["indices"].GetString());
						std::wstring strIndices = StringUtil::UTF8_to_UNICODE(strTemp);
						mesh.m_vecStrIndices.push_back(strIndices);
					}

					// 绘制方式
					if (primitive.HasMember("mode"))
					{
						mesh.m_vecDrawType.push_back(primitive["mode"].GetUint());
					}

					// 纹理和材质
					if (primitive.HasMember("material"))
					{
						std::string strTemp(primitive["material"].GetString());
						std::wstring strMaterial = StringUtil::UTF8_to_UNICODE(strTemp);
						mesh.m_vecStrMaterial.push_back(strMaterial);
					}
				}
				pTileInfos->m_mapMeshs[strMeshName] = mesh;
			}

			it++;
		}

		return true;
	}

	bool GLTFParser::ParseAccessors(GLTFTileInfos_1* pTileInfos, rapidjson::Document& doc)
	{
		rapidjson::Value& accessors = doc["accessors"];
		if (accessors.Empty())
		{
			return false;
		}

		rapidjson::Value::ConstMemberIterator it = accessors.MemberBegin();
		while (it != accessors.MemberEnd())
		{
			rapidjson::Value key;
			rapidjson::Value accessor;

			rapidjson::Document::AllocatorType allocatorType;

			key.CopyFrom(it->name, allocatorType);
			accessor.CopyFrom(it->value, allocatorType);

			std::wstring strAccessorName;
			if (key.IsString())
			{
				std::string strTemp(key.GetString());
				strAccessorName = StringUtil::UTF8_to_UNICODE(strTemp);
			}

			std::string strTempBufferView(accessor["bufferView"].GetString());
			std::wstring strBufferViewName = StringUtil::UTF8_to_UNICODE(strTempBufferView);
			unsigned int nByteOffset = accessor["byteOffset"].GetUint();
			unsigned int nCount = accessor["count"].GetUint();
			unsigned int nComponentType = accessor["componentType"].GetUint();
			std::string strTempType(accessor["type"].GetString());
			std::wstring strDataType = StringUtil::UTF8_to_UNICODE(strTempType);

			GLTFAccessor gltfAccessor;
			gltfAccessor.m_strBufferViewName = strBufferViewName;
			gltfAccessor.m_nByteOffset = nByteOffset;
			gltfAccessor.m_nCount = nCount;
			gltfAccessor.m_strType = strDataType;
			gltfAccessor.m_nComponentType = nComponentType;
			pTileInfos->m_mapAccessors[strAccessorName] = gltfAccessor;

			it++;
		}

		return true;
	}

	bool GLTFParser::ParseBufferViews(GLTFTileInfos_1* pTileInfos, rapidjson::Document& doc)
	{
		rapidjson::Value& bufferViews = doc["bufferViews"];
		if (bufferViews.Empty())
		{
			return false;
		}

		rapidjson::Value::ConstMemberIterator it = bufferViews.MemberBegin();
		while (it != bufferViews.MemberEnd())
		{
			rapidjson::Value key;
			rapidjson::Value bufferView;

			rapidjson::Document::AllocatorType allocatorType;

			key.CopyFrom(it->name, allocatorType);
			bufferView.CopyFrom(it->value, allocatorType);

			std::wstring strBufferViewName;
			if (key.IsString())
			{
				std::string strTemp(key.GetString());
				strBufferViewName = StringUtil::UTF8_to_UNICODE(strTemp);
			}

			std::string strBuffer(bufferView["buffer"].GetString());
			std::wstring strBufferName = StringUtil::UTF8_to_UNICODE(strBuffer);

			unsigned int nByteOffset = bufferView["byteOffset"].GetUint();
			unsigned int nByteStride = 0;

			if (bufferView.HasMember("byteStride"))
			{
				nByteStride = bufferView["byteStride"].GetUint();
			}
			unsigned int nByteLength = bufferView["byteLength"].GetUint();

			GLTFBufferView gltfBufferView;
			gltfBufferView.m_strBufferName = strBufferName;
			gltfBufferView.m_nByteOffset = nByteOffset;
			gltfBufferView.m_nByteStride = nByteStride;
			gltfBufferView.m_nByteLength = nByteLength;

			pTileInfos->m_mapBufferViews[strBufferViewName] = gltfBufferView;

			it++;
		}

		return true;
	}

	bool GLTFParser::ParseMaterials(GLTFTileInfos_1* pTileInfos, rapidjson::Document& doc)
	{
		if (doc.HasMember("materials"))
		{
			rapidjson::Value& materials = doc["materials"];

			rapidjson::Value::ConstMemberIterator it = materials.MemberBegin();
			while (it != materials.MemberEnd())
			{
				rapidjson::Value key;
				rapidjson::Value material;

				rapidjson::Document::AllocatorType allocatorType;

				key.CopyFrom(it->name, allocatorType);
				material.CopyFrom(it->value, allocatorType);

				std::wstring strMaterialName;
				if (key.IsString())
				{
					std::string strTemp(key.GetString());
					strMaterialName = StringUtil::UTF8_to_UNICODE(strTemp);
				}

				rapidjson::Value& values = material["values"];

				rapidjson::Value& ambient = values["ambient"];
				rapidjson::Value& diffuse = values["diffuse"];
				rapidjson::Value& emission = values["emission"];
				rapidjson::Value& specular = values["specular"];
				rapidjson::Value& diffuseTex = values["diffuseTex"];

				std::wstring strTexName;
				if (ambient.IsString())
				{
					std::string strTemp(ambient.GetString());
					strTexName = StringUtil::UTF8_to_UNICODE(strTemp);
				}
				else if (diffuse.IsString())
				{
					std::string strTemp(diffuse.GetString());
					strTexName = StringUtil::UTF8_to_UNICODE(strTemp);
				}
				else if (emission.IsString())
				{
					std::string strTemp(emission.GetString());
					strTexName = StringUtil::UTF8_to_UNICODE(strTemp);
				}
				else if (specular.IsString())
				{
					std::string strTemp(specular.GetString());
					strTexName = StringUtil::UTF8_to_UNICODE(strTemp);
				}
				else if (diffuseTex.IsString())
				{
					std::string strTemp(diffuseTex.GetString());
					strTexName = StringUtil::UTF8_to_UNICODE(strTemp);
				}

				if (strTexName.length() != 0)
				{
					GLTFMaterial gltfMaterial;
					gltfMaterial.m_strTextureName = strTexName;

					pTileInfos->m_mapMaterials[strMaterialName] = gltfMaterial;
				}

				it++;
			}
		}

		return true;
	}

	bool GLTFParser::ParseTextures(GLTFTileInfos_1* pTileInfos, rapidjson::Document& doc)
	{
		if (doc.HasMember("textures"))
		{
			rapidjson::Value& textures = doc["textures"];

			rapidjson::Value::ConstMemberIterator it = textures.MemberBegin();
			while (it != textures.MemberEnd())
			{
				rapidjson::Value key;
				rapidjson::Value texture;

				rapidjson::Document::AllocatorType allocatorType;

				key.CopyFrom(it->name, allocatorType);
				texture.CopyFrom(it->value, allocatorType);

				std::wstring strTextureName;
				if (key.IsString())
				{
					std::string strTemp(key.GetString());
					strTextureName = StringUtil::UTF8_to_UNICODE(strTemp);
				}

				if (texture.HasMember("source"))
				{
					std::string strTemp(texture["source"].GetString());
					std::wstring strImageName = StringUtil::UTF8_to_UNICODE(strTemp);

					GLTFTexture gltfTexture;
					gltfTexture.m_strImageName = strImageName;
					pTileInfos->m_mapTextures[strTextureName] = gltfTexture;
				}

				it++;
			}
		}

		return true;
	}

	bool GLTFParser::ParseImages(GLTFTileInfos_1* pTileInfos, rapidjson::Document& doc)
	{
		if (doc.HasMember("images"))
		{
			rapidjson::Value& images = doc["images"];
			if (images.Empty())
			{
				return false;
			}

			rapidjson::Value::ConstMemberIterator it = images.MemberBegin();
			while (it != images.MemberEnd())
			{
				rapidjson::Value key;
				rapidjson::Value image;

				rapidjson::Document::AllocatorType allocatorType;

				key.CopyFrom(it->name, allocatorType);
				image.CopyFrom(it->value, allocatorType);

				std::wstring strImageName;
				if (key.IsString())
				{
					std::string strTemp(key.GetString());
					strImageName = StringUtil::UTF8_to_UNICODE(strTemp);
				}

				rapidjson::Value& extensions = image["extensions"];

				if (!extensions.Empty())
				{
					rapidjson::Value& v = extensions["KHR_binary_glTF"];
					if (!v.Empty())
					{
						if (v.HasMember("bufferView"))
						{
							std::string strBufferView(v["bufferView"].GetString());
							std::wstring strBufferViewName = StringUtil::UTF8_to_UNICODE(strBufferView);
							GLTFImage gltfImage;
							gltfImage.m_strBufferViewName = strBufferViewName;
							if (v.HasMember("mimeType"))
							{
								std::string strMimeType(v["mimeType"].GetString());
								std::wstring strImageType = StringUtil::UTF8_to_UNICODE(strMimeType);
								gltfImage.m_strImageType = strImageType;
							}

							pTileInfos->m_mapImages[strImageName] = gltfImage;
						}
					}
				}

				it++;
			}
		}
		return true;
	}

	bool GLTFParser::ParseBuffers(GLTFTileInfos_1* pTileInfos, rapidjson::Document& doc)
	{
		rapidjson::Value& buffers = doc["buffers"];
		if (buffers.Empty())
		{
			return false;
		}

		rapidjson::Value::ConstMemberIterator it = buffers.MemberBegin();
		while (it != buffers.MemberEnd())
		{
			rapidjson::Value key;
			rapidjson::Value buffer;

			rapidjson::Document::AllocatorType allocatorType;

			key.CopyFrom(it->name, allocatorType);
			buffer.CopyFrom(it->value, allocatorType);

			std::wstring strBufferName;
			if (key.IsString())
			{
				std::string strTemp(key.GetString());
				strBufferName = StringUtil::UTF8_to_UNICODE(strTemp);
			}

			unsigned int nByteLength = buffer["byteLength"].GetUint();

			GLTFBuffer gltfBuffer;
			gltfBuffer.m_nByteLength = nByteLength;

			pTileInfos->m_mapBuffers[strBufferName] = gltfBuffer;

			it++;
		}

		return true;
	}

	bool GLTFParser::ParseExtensions(GLTFTileInfos_1* pTileInfos, rapidjson::Document& doc)
	{
		rapidjson::Value& extensions = doc["extensions"];
		if (extensions.Empty())
		{
			return false;
		}

		rapidjson::Value& cesium_rtc = extensions["CESIUM_RTC"];
		if (cesium_rtc.Empty())
		{
			return false;
		}

		rapidjson::Value& jCenter = cesium_rtc["center"];
		if (jCenter.Empty())
		{
			return false;
		}

		if (jCenter.IsArray())
		{
			pTileInfos->m_vCesuimRTC.x = jCenter[0].GetDouble();
			pTileInfos->m_vCesuimRTC.y = jCenter[1].GetDouble();
			pTileInfos->m_vCesuimRTC.z = jCenter[2].GetDouble();
		}

		return true;
	}

	bool GLTFParser::ParseNode(GLTFTileInfos_2* pTileInfos, rapidjson::Document & doc)
	{
		rapidjson::Value& scenes = doc["scenes"];
		rapidjson::Value& nodes = scenes[0]["nodes"];
		unsigned int nNodeIdx = 0;
		for (int i = 0; i < nodes.Size(); i++)
		{
			Matrix4d matGeode = Matrix4d::IDENTITY;
			int k = nodes[i].GetInt();
			ProcessGLTFNode(pTileInfos, doc["nodes"], k, matGeode, ++nNodeIdx);
		}
		return true;
	}

	bool GLTFParser::ParseNode_glb(GLTFTileInfos_2* pTileInfos, rapidjson::Document & doc)
	{
		rapidjson::Value& scenes = doc["scenes"];
		rapidjson::Value& nodes = scenes[0]["nodes"];
		Matrix4d matGeode = Matrix4d::IDENTITY;
		std::set<int> setTotalIndex;
		for (int i = 0; i < nodes.Size(); i++)
		{
			ProcessGLTFNode_glb(pTileInfos, doc["nodes"], nodes[i].GetInt(), matGeode, -1, setTotalIndex);
		}
		return true;
	}

	void GLTFParser::ProcessGLTFNode(GLTFTileInfos_2* pTileInfos, rapidjson::Value& value, int index, Matrix4d& matGeode, unsigned int& parentIndex)
	{
		if (index == -1)
		{
			return;
		}

		rapidjson::Value& node = value[index];
		rapidjson::Value name;
		if (node.HasMember("name"))
		{
			name = node["name"];
		}
		rapidjson::Value  matrix;
		if (node.HasMember("matrix"))
		{
			matrix = node["matrix"];
		}

		rapidjson::Value rotation;
		if (node.HasMember("rotation"))
		{
			rotation = node["rotation"];
		}

		rapidjson::Value scale;
		if (node.HasMember("scale"))
		{
			scale = node["scale"];
		}

		rapidjson::Value translation;
		if (node.HasMember("translation"))
		{
			translation = node["translation"];
		}

		rapidjson::Value  mesh;
		if (node.HasMember("mesh"))
		{
			mesh = node["mesh"];
		}

		std::string strName;
		if (name.IsString())
		{
			strName = std::string(name.GetString());
		}

		Matrix4d tempMat = matGeode;
		if (matrix.IsArray())
		{
			Matrix4d mat = Matrix4d::IDENTITY;
			if (matrix.Size() == 16)
			{
				for (int r = 0; r < 4; r++)
				{
					mat[r][0] = matrix[r * 4 + 0].GetDouble();
					mat[r][1] = matrix[r * 4 + 1].GetDouble();
					mat[r][2] = matrix[r * 4 + 2].GetDouble();
					mat[r][3] = matrix[r * 4 + 3].GetDouble();
				}
			}

			if (strName == "X_UP" ||
				strName == "Y_UP" ||
				strName == "Z_UP")
			{
				if (strName == "Z_UP")
				{
					pTileInfos->m_nAxisUpType = AxisUpType::Z_UP;
				}
				else if (strName == "Y_UP")
				{
					pTileInfos->m_nAxisUpType = AxisUpType::Y_UP;
				}
				else
				{
					pTileInfos->m_nAxisUpType = AxisUpType::X_UP;
				}
				mat = Matrix4d::IDENTITY;
			}

			tempMat = mat * tempMat;
		}
		else
		{
			Matrix4d rotMat = Matrix4d::IDENTITY;
			Matrix4d scaleMat = Matrix4d::IDENTITY;
			Matrix4d transMat = Matrix4d::IDENTITY;
			if (scale.IsArray())
			{
				double x = scale[0].GetDouble();
				double y = scale[1].GetDouble();
				double z = scale[2].GetDouble();
				scaleMat = MatrixUtils::Scaling(scale[0].GetDouble(), scale[1].GetDouble(), scale[2].GetDouble());
			}
			if (rotation.IsArray())
			{
				Vector4d quat(rotation[0].GetDouble(), rotation[1].GetDouble(), rotation[2].GetDouble(), rotation[3].GetDouble());
				rotMat = MatrixUtils::QuaternionToRotationMatrix(quat);
			}
			if (translation.IsArray())
			{
				double x = translation[0].GetDouble();
				double y = translation[1].GetDouble();
				double z = translation[2].GetDouble();
				transMat = MatrixUtils::Translation(x, y, z);
			}
			Matrix4d mat = scaleMat * rotMat * transMat;
			tempMat = mat * tempMat;
		}

		if (!mesh.IsNull())
		{
			unsigned int meshIndex = mesh.GetUint();
			Matrix4d matR = Matrix4d::IDENTITY;
			if (pTileInfos->m_nAxisUpType == Y_UP)
			{
				matR = Matrix4d(
					1, 0, 0, 0,
					0, 0, 1, 0,
					0, -1, 0, 0,
					0, 0, 0, 1);
			}
			pTileInfos->m_mapMeshToLocalView[meshIndex] = tempMat * matR;

			std::map<unsigned int, std::vector<unsigned int> >& mapMeshSet = pTileInfos->m_mapMeshSet;
			std::map<unsigned int, std::vector<unsigned int> >::iterator it = mapMeshSet.find(parentIndex);
			if (it == mapMeshSet.end())
			{
				mapMeshSet[parentIndex] = std::vector<unsigned int>();
			}
			mapMeshSet[parentIndex].push_back(meshIndex);
		}

		rapidjson::Value children;
		if (node.HasMember("children"))
		{
			children = node["children"];
		}

		if (children.IsArray())
		{
			for (unsigned int i = 0; i < children.Size(); i++)
			{
				ProcessGLTFNode(pTileInfos, value, children[i].GetInt(), tempMat, ++parentIndex);
			}
		}
	}

	void GLTFParser::ProcessGLTFNode_glb(GLTFTileInfos_2* pTileInfos, rapidjson::Value& value, int index, Matrix4d& matGeode, int parentIndex, std::set<int>& setTotalIndex)
	{
		if (index == -1)
		{
			return;
		}

		if (index == parentIndex && !value[index].HasMember("children"))
		{
			if (setTotalIndex.find(index) != setTotalIndex.end())
			{
				return;
			}
		}

		rapidjson::Value& node = value[index];
		rapidjson::Value name;
		if (node.HasMember("name"))
		{
			name = node["name"];
		}
		rapidjson::Value  matrix;
		if (node.HasMember("matrix"))
		{
			matrix = node["matrix"];
		}

		rapidjson::Value rotation;
		if (node.HasMember("rotation"))
		{
			rotation = node["rotation"];
		}

		rapidjson::Value scale;
		if (node.HasMember("scale"))
		{
			scale = node["scale"];
		}

		rapidjson::Value translation;
		if (node.HasMember("translation"))
		{
			translation = node["translation"];
		}

		rapidjson::Value  mesh;
		if (node.HasMember("mesh"))
		{
			mesh = node["mesh"];
		}

		std::string strName;
		if (name.IsString())
		{
			strName = std::string(name.GetString());
		}

		Matrix4d tempMat = matGeode;
		if (matrix.IsArray())
		{
			Matrix4d mat = Matrix4d::IDENTITY;
			if (matrix.Size() == 16)
			{
				for (int r = 0; r < 4; r++)
				{
					mat[r][0] = matrix[r * 4 + 0].GetDouble();
					mat[r][1] = matrix[r * 4 + 1].GetDouble();
					mat[r][2] = matrix[r * 4 + 2].GetDouble();
					mat[r][3] = matrix[r * 4 + 3].GetDouble();
				}
			}

			tempMat = mat * tempMat;
		}
		else
		{
			Matrix4d rotMat = Matrix4d::IDENTITY;
			Matrix4d scaleMat = Matrix4d::IDENTITY;
			Matrix4d transMat = Matrix4d::IDENTITY;
			if (scale.IsArray())
			{
				double x = scale[0].GetDouble();
				double y = scale[1].GetDouble();
				double z = scale[2].GetDouble();
				scaleMat = MatrixUtils::Scaling(scale[0].GetDouble(), scale[1].GetDouble(), scale[2].GetDouble());
			}
			if (rotation.IsArray())
			{
				Vector4d quat(rotation[0].GetDouble(), rotation[1].GetDouble(), rotation[2].GetDouble(), rotation[3].GetDouble());
				rotMat = MatrixUtils::QuaternionToRotationMatrix(quat);
			}
			if (translation.IsArray())
			{
				double x = translation[0].GetDouble();
				double y = translation[1].GetDouble();
				double z = translation[2].GetDouble();
				transMat = MatrixUtils::Translation(x, y, z);
			}
			Matrix4d mat = scaleMat * rotMat * transMat;
			tempMat = mat * tempMat;
		}

		if (!mesh.IsNull())
		{
			unsigned int meshIndex = mesh.GetUint();
			Matrix4d matR(
				1, 0, 0, 0,
				0, 0, 1, 0,
				0, -1, 0, 0,
				0, 0, 0, 1);
			pTileInfos->m_mapMeshToLocalView[index] = tempMat * matR;

			std::map<unsigned int, std::vector<unsigned int> >& mapMeshSet = pTileInfos->m_mapMeshSet;
			std::map<unsigned int, std::vector<unsigned int> >::iterator it = mapMeshSet.find(meshIndex);
			if (it == mapMeshSet.end())
			{
				mapMeshSet[meshIndex] = std::vector<unsigned int>();
			}
			mapMeshSet[meshIndex].push_back(index);
			setTotalIndex.insert(index);
		}

		rapidjson::Value children;
		if (node.HasMember("children"))
		{
			children = node["children"];
		}

		if (children.IsArray())
		{
			for (unsigned int i = 0; i < children.Size(); i++)
			{
				ProcessGLTFNode_glb(pTileInfos, value, children[i].GetInt(), tempMat, index, setTotalIndex);
			}
		}
	}

	bool GLTFParser::ParseMesh(GLTFTileInfos_2* pTileInfos, rapidjson::Document& doc)
	{
		if (!doc.HasMember("meshes"))
		{
			return false;
		}

		rapidjson::Value& meshs = doc["meshes"];
		// 创建骨架
		for (unsigned int i = 0; i < meshs.Size(); i++)
		{
			rapidjson::Value& mesh = meshs[i];
			if (!mesh.HasMember("primitives"))
			{
				continue;
			}

			rapidjson::Value& primitives = mesh["primitives"];
			if (primitives.IsArray())
			{
				GLTFMesh mesh;
				for (unsigned int j = 0; j < primitives.Size(); j++)
				{
					// 顶点
					rapidjson::Value& primitive = primitives[j];
					GLTFPrimitive gltfPrimitive;

					if (primitive.HasMember("attributes"))
					{
						rapidjson::Value& attribute = primitive["attributes"];
						GLTFAttributes gltfAttributes;
						// 顶点
						if (attribute.HasMember("POSITION"))
						{
							gltfAttributes.m_nPosIndex = attribute["POSITION"].GetInt();
						}
						// 法线
						if (attribute.HasMember("NORMAL"))
						{
							gltfAttributes.m_nNormalIndex = attribute["NORMAL"].GetInt();
						}
						//顶点颜色
						if (attribute.HasMember("COLOR_0"))
						{
							gltfAttributes.m_nColorIndex = attribute["COLOR_0"].GetInt();
						}
						// 纹理(一重纹理)
						if (attribute.HasMember("TEXCOORD_0"))
						{
							gltfAttributes.m_nTex1Index = attribute["TEXCOORD_0"].GetInt();
						}
						if (attribute.HasMember("TEXCOORD_1"))
						{
							gltfAttributes.m_nTex2Index = attribute["TEXCOORD_1"].GetInt();
						}
						if (attribute.HasMember("_BATCHID"))
						{
							gltfAttributes.m_nBatchIds = attribute["_BATCHID"].GetInt();
						}
						gltfPrimitive.m_gltfAttributes = gltfAttributes;
					}

					// 索引
					if (primitive.HasMember("indices"))
					{
						gltfPrimitive.m_nIndices = primitive["indices"].GetInt();
					}

					// 绘制方式
					if (primitive.HasMember("mode"))
					{
						gltfPrimitive.m_nDrawType = primitive["mode"].GetUint();
					}

					// 纹理和材质
					if (primitive.HasMember("material"))
					{
						gltfPrimitive.m_nMaterialIndex = primitive["material"].GetUint();
					}

					if (primitive.HasMember("extensions"))
					{
						rapidjson::Value& extensions = primitive["extensions"];
						if (extensions.HasMember("KHR_draco_mesh_compression"))
						{
							GLTFDraco gltfDraco;
							rapidjson::Value& draco = extensions["KHR_draco_mesh_compression"];
							if (draco.HasMember("attributes"))
							{
								rapidjson::Value& attribute = draco["attributes"];

								// 顶点
								if (attribute.HasMember("POSITION"))
								{
									gltfDraco.m_nPosAttributeIndex = attribute["POSITION"].GetInt();
								}
								// 法线
								if (attribute.HasMember("NORMAL"))
								{
									gltfDraco.m_nNormalAttributeIndex = attribute["NORMAL"].GetInt();
								}
								//顶点颜色
								if (attribute.HasMember("COLOR_0"))
								{
									gltfDraco.m_nColorAttributeIndex = attribute["COLOR_0"].GetInt();
								}
								// 纹理(一重纹理)
								if (attribute.HasMember("TEXCOORD_0"))
								{
									gltfDraco.m_nTex1AttributeIndex = attribute["TEXCOORD_0"].GetInt();
								}
								if (attribute.HasMember("TEXCOORD_1"))
								{
									gltfDraco.m_nTex2AttributeIndex = attribute["TEXCOORD_1"].GetInt();
								}
								if (attribute.HasMember("_BATCHID"))
								{
									gltfDraco.m_nSecondColorIndex = attribute["_BATCHID"].GetInt();
								}
							}

							if (draco.HasMember("bufferView"))
							{
								rapidjson::Value& bufferrView = draco["bufferView"];
								gltfDraco.m_nBufferViewIndex = bufferrView.GetInt();
							}
							gltfPrimitive.m_gltfDraco = gltfDraco;
						}
					}

					mesh.m_vecGLTFPrimitive.push_back(gltfPrimitive);
				}
				pTileInfos->m_mapMeshs[i] = mesh;
			}
		}
		return true;
	}

	bool GLTFParser::ParseAccessors(GLTFTileInfos_2* pTileInfos, rapidjson::Document& doc)
	{
		rapidjson::Value& accessors = doc["accessors"];
		for (unsigned int i = 0; i < accessors.Size(); i++)
		{
			rapidjson::Value& accessor = accessors[i];
			unsigned int nBufferIndex = accessor["bufferView"].GetUint();
			unsigned int nByteOffset = accessor["byteOffset"].GetUint();
			unsigned int nCount = accessor["count"].GetUint();
			unsigned int nComponentType = accessor["componentType"].GetUint();
			const char* dtype = accessor["type"].GetString();
			std::string strDataType(dtype);

			GLTFAccessor gltfAccessor;
			gltfAccessor.m_nBufferViewIndex = nBufferIndex;
			gltfAccessor.m_nByteOffset = nByteOffset;
			gltfAccessor.m_nCount = nCount;
			gltfAccessor.m_nComponentType = nComponentType;
			gltfAccessor.m_strType = StringUtil::UTF8_to_UNICODE(strDataType);
			if (gltfAccessor.m_strType == U("VEC3") && accessor.HasMember("max") && accessor.HasMember("min"))
			{
				rapidjson::Value& vMax = accessor["max"];
				rapidjson::Value& vMin = accessor["min"];
				gltfAccessor.m_vMax = Vector3d(vMax[0].GetDouble(), vMax[1].GetDouble(), vMax[2].GetDouble());
				gltfAccessor.m_vMin = Vector3d(vMin[0].GetDouble(), vMin[1].GetDouble(), vMin[2].GetDouble());
			}

			pTileInfos->m_mapAccessors[i] = gltfAccessor;
		}
		return true;
	}

	bool GLTFParser::ParseBufferViews(GLTFTileInfos_2* pTileInfos, rapidjson::Document& doc)
	{
		rapidjson::Value& bufferViews = doc["bufferViews"];
		for (unsigned int i = 0; i < bufferViews.Size(); i++)
		{
			rapidjson::Value& bufferView = bufferViews[i];
			unsigned int nBufferIndex = bufferView["buffer"].GetUint();
			unsigned int nByteOffset = bufferView["byteOffset"].GetUint();
			unsigned int nByteStride = 0;
			if (bufferView.HasMember("byteStride"))
			{
				nByteStride = bufferView["byteStride"].GetUint();
			}
			unsigned int nByteLength = bufferView["byteLength"].GetUint();
			GLTFBufferView gltfBufferView;
			gltfBufferView.m_nBufferIndex = nBufferIndex;
			gltfBufferView.m_nByteOffset = nByteOffset;
			gltfBufferView.m_nByteStride = nByteStride;
			gltfBufferView.m_nByteLength = nByteLength;
			pTileInfos->m_mapBufferViews[i] = gltfBufferView;
		}
		return true;
	}

	bool GLTFParser::ParseMaterials(GLTFTileInfos_2* pTileInfos, rapidjson::Document& doc)
	{
		if (doc.HasMember("materials"))
		{
			rapidjson::Value& materials = doc["materials"];
			if (materials.IsArray())
			{
				for (unsigned int i = 0; i < materials.Size(); i++)
				{
					GLTFMaterial gltfMaterial;
					rapidjson::Value& material = materials[i];
					if (material.HasMember("name"))
					{
						rapidjson::Value& name = material["name"];
						gltfMaterial.m_strName = StringUtil::UTF8_to_UNICODE(name.GetString());
					}
					else
					{
						gltfMaterial.m_strName = BaseUtils::CreateGuid();
					}

					if (material.HasMember("pbrMetallicRoughness"))
					{
						rapidjson::Value& pbrMetallicRoughness = material["pbrMetallicRoughness"];
						if (pbrMetallicRoughness.HasMember("baseColorTexture"))
						{
							rapidjson::Value& baseColorTexture = pbrMetallicRoughness["baseColorTexture"];
							if (baseColorTexture.HasMember("index"))
							{
								gltfMaterial.m_PBR.m_nBaseColorTextureIndex = baseColorTexture["index"].GetInt();
							}
							if (baseColorTexture.HasMember("texCoord"))
							{
								gltfMaterial.m_PBR.m_nBaseColorTextureCoordIndex = baseColorTexture["texCoord"].GetInt();
							}
						}
						if (pbrMetallicRoughness.HasMember("metallicRoughnessTexture"))
						{
							rapidjson::Value& baseColorTexture = pbrMetallicRoughness["metallicRoughnessTexture"];
							if (baseColorTexture.HasMember("index"))
							{
								gltfMaterial.m_PBR.m_nMetallicRoughnessTextureIndex = baseColorTexture["index"].GetInt();
							}
							if (baseColorTexture.HasMember("texCoord"))
							{
								gltfMaterial.m_PBR.m_nMetallicRoughnessTextureCoordIndex = baseColorTexture["texCoord"].GetInt();
							}
						}
						if (pbrMetallicRoughness.HasMember("baseColorFactor"))
						{
							rapidjson::Value& baseColorFactor = pbrMetallicRoughness["baseColorFactor"];
							Vector4d color;
							color.x = baseColorFactor[0].GetFloat();
							color.y = baseColorFactor[1].GetFloat();
							color.z = baseColorFactor[2].GetFloat();
							color.w = baseColorFactor[3].GetFloat();
							gltfMaterial.m_PBR.m_vec4BaseColor = color;
						}
						if (pbrMetallicRoughness.HasMember("metallicFactor"))
						{
							float fMetallicFactor = pbrMetallicRoughness["metallicFactor"].GetFloat();
							gltfMaterial.m_PBR.m_fMetallicFactor = fMetallicFactor;
						}
						if (pbrMetallicRoughness.HasMember("roughnessFactor"))
						{
							float fRoughnessFactor = pbrMetallicRoughness["roughnessFactor"].GetFloat();
							gltfMaterial.m_PBR.m_fRoughnessFactor = fRoughnessFactor;
						}
						if (material.HasMember("emissiveTexture"))
						{
							rapidjson::Value& emissiveTexture = material["emissiveTexture"];
							if (emissiveTexture.HasMember("index"))
							{
								gltfMaterial.m_nEmissiveTextureIndex = emissiveTexture["index"].GetInt();
							}
							if (emissiveTexture.HasMember("texCoord"))
							{
								gltfMaterial.m_nEmissiveTextureCoordIndex = emissiveTexture["texCoord"].GetInt();
							}
						}
						if (material.HasMember("emissiveFactor"))
						{
							rapidjson::Value& emissiveFactor = material["emissiveFactor"];
							Vector3d color;
							color.x = emissiveFactor[0].GetFloat();
							color.y = emissiveFactor[1].GetFloat();
							color.z = emissiveFactor[2].GetFloat();
							gltfMaterial.m_vec3EmissiveFactor = color;
						}
						if (material.HasMember("doubleSided"))
						{
							gltfMaterial.m_bDoubleSided = material["doubleSided"].GetBool();
						}
						if (material.HasMember("alphaMode"))
						{
							std::string alphaMode(material["alphaMode"].GetString());
							if (alphaMode == "BLEND")
							{
								gltfMaterial.m_AlphaMode = PBRAM_BLEND;
							}
							else if (alphaMode == "OPAQUE")
							{
								gltfMaterial.m_AlphaMode = PBRAM_OPAQUE;
							}
							else if (alphaMode == "MASK")
							{
								gltfMaterial.m_AlphaMode = PBRAM_MASK;
							}
						}
						if (material.HasMember("normalTexture"))
						{
							rapidjson::Value& normalTexture = material["normalTexture"];
							if (normalTexture.HasMember("index"))
							{
								gltfMaterial.m_nNormalTextureIndex = normalTexture["index"].GetInt();
							}
							if (normalTexture.HasMember("texCoord"))
							{
								gltfMaterial.m_nNormalTextureCoordIndex = normalTexture["texCoord"].GetInt();
							}
							if (normalTexture.HasMember("scale"))
							{
								gltfMaterial.m_fNormalTextureScale = normalTexture["scale"].GetFloat();
							}
						}
						if (material.HasMember("occlusionTexture"))
						{
							rapidjson::Value& occlusionTexture = material["occlusionTexture"];
							if (occlusionTexture.HasMember("index"))
							{
								gltfMaterial.m_nOcclusionTextureIndex = occlusionTexture["index"].GetInt();
							}
							if (occlusionTexture.HasMember("texCoord"))
							{
								gltfMaterial.m_nOcclusionTextureCoordIndex = occlusionTexture["texCoord"].GetInt();
							}
							if (occlusionTexture.HasMember("strength"))
							{
								gltfMaterial.m_fOcclusionTextureStrength = occlusionTexture["strength"].GetFloat();
							}
						}
						if (material.HasMember("alphaCutoff"))
						{
							gltfMaterial.m_fAlphaCutoff = material["alphaCutoff"].GetFloat();
						}
					}

					if (material.HasMember("extensions"))
					{
						rapidjson::Value& extensions = material["extensions"];
						if (extensions.HasMember("KHR_techniques_webgl"))
						{
							rapidjson::Value& KHR_techniques_webgl = extensions["KHR_techniques_webgl"];
							if (KHR_techniques_webgl.HasMember("values"))
							{
								rapidjson::Value& values = KHR_techniques_webgl["values"];
								if (values.HasMember("u_diffuse"))
								{
									rapidjson::Value& u_diffuse = values["u_diffuse"];
									if (u_diffuse.HasMember("index"))
									{
										gltfMaterial.m_PBR.m_nBaseColorTextureIndex = u_diffuse["index"].GetInt();
									}
								}

								if (values.HasMember("u_sampler0"))
								{
									rapidjson::Value& sampler = values["u_sampler0"];
									if (sampler.HasMember("index"))
									{
										gltfMaterial.m_PBR.m_nBaseColorTextureIndex = sampler["index"].GetInt();
									}
								}
								if (values.HasMember("u_sampler1"))
								{
									rapidjson::Value& sampler = values["u_sampler1"];
									if (sampler.HasMember("index"))
									{
										gltfMaterial.m_PBR.m_nBaseColorTextureIndex = sampler["index"].GetInt();
									}
								}
								if (values.HasMember("u_tex"))
								{
									rapidjson::Value& u_tex = values["u_tex"];
									if (u_tex.HasMember("index"))
									{
										gltfMaterial.m_PBR.m_nBaseColorTextureIndex = u_tex["index"].GetInt();
									}
									if (u_tex.HasMember("texCoord"))
									{
										gltfMaterial.m_PBR.m_nBaseColorTextureCoordIndex = u_tex["texCoord"].GetInt();
									}
								}
							}
						}
						else if (extensions.HasMember("KHR_technique_webgl"))
						{
							rapidjson::Value& KHR_techniques_webgl = extensions["KHR_technique_webgl"];
							if (KHR_techniques_webgl.HasMember("values"))
							{
								rapidjson::Value& values = KHR_techniques_webgl["values"];
								if (values.HasMember("diffuse"))
								{
									gltfMaterial.m_PBR.m_nBaseColorTextureIndex = values["diffuse"].GetInt();

									if (material.HasMember("values"))
									{
										rapidjson::Value& val = material["values"];
										if (val.HasMember("diffuse"))
										{
											gltfMaterial.m_PBR.m_nBaseColorTextureIndex = val["diffuse"]["index"].GetInt();
										}
									}
								}
							}
						}
					}
					pTileInfos->m_mapMaterials[i].push_back(gltfMaterial);
				}
			}
		}
		return true;
	}

	bool GLTFParser::ParseTextures(GLTFTileInfos_2* pTileInfos, rapidjson::Document& doc)
	{
		if (doc.HasMember("textures"))
		{
			rapidjson::Value& textures = doc["textures"];
			if (textures.IsArray())
			{
				for (unsigned int i = 0; i < textures.Size(); i++)
				{
					rapidjson::Value& texture = textures[i];
					if (texture.HasMember("source"))
					{
						GLTFTexture gltfTexture;
						gltfTexture.m_nImageIndex = texture["source"].GetInt();
						if (texture.HasMember("sampler"))
						{
							rapidjson::Value& sampler = texture["sampler"];
							if (sampler.IsObject())
							{
								if (sampler.HasMember("wrapS"))
								{
									gltfTexture.m_GLTFSampler.m_nWrapS = sampler["wrapS"].GetInt();
								}
								if (sampler.HasMember("wrapT"))
								{
									gltfTexture.m_GLTFSampler.m_nWrapT = sampler["wrapT"].GetInt();
								}
							}
						}
						pTileInfos->m_mapTextures[i] = gltfTexture;
					}
				}
			}
		}
		return true;
	}

	bool GLTFParser::ParseImages(GLTFTileInfos_2* pTileInfos, rapidjson::Document& doc)
	{
		std::wstring strFileName;
		if (doc.HasMember("images"))
		{
			rapidjson::Value& images = doc["images"];
			if (images.IsArray())
			{
				for (int i = 0; i < images.Size(); i++)
				{
					rapidjson::Value& image = images[i];
					GLTFImage gltfImage;
					if (image.HasMember("name"))
					{
						const char* ch = image["name"].GetString();
						strFileName = StringUtil::UTF8_to_UNICODE(std::string(ch));
					}
					if (image.HasMember("extras"))
					{
						rapidjson::Value& extras = image["extras"];
						if (extras.HasMember("compressedImage3DTiles"))
						{
							rapidjson::Value& compressedImage3DTiles = extras["compressedImage3DTiles"];
							if (compressedImage3DTiles.HasMember("crunch"))
							{
								rapidjson::Value& crunch = compressedImage3DTiles["crunch"];
								if (crunch.HasMember("bufferView"))
								{
									gltfImage.m_nBufferViewIndex = crunch["bufferView"].GetInt();
									if (crunch.HasMember("mimeType"))
									{
										const char* imageType = crunch["mimeType"].GetString();
										gltfImage.m_strImageType = StringUtil::UTF8_to_UNICODE(std::string(imageType));
									}

									pTileInfos->m_mapImages[i] = gltfImage;
								}
							}
						}
					}
					else if (image.HasMember("bufferView"))
					{
						gltfImage.m_nBufferViewIndex = image["bufferView"].GetInt();
						if (image.HasMember("mimeType"))
						{
							const char* imageType = image["mimeType"].GetString();
							gltfImage.m_strImageType = StringUtil::UTF8_to_UNICODE(std::string(imageType));
						}

						pTileInfos->m_mapImages[i] = gltfImage;
					}
					else if (image.HasMember("uri") || image.HasMember("url"))
					{
						if (image.HasMember("name"))
						{
							const char* ch = image["name"].GetString();
							strFileName = StringUtil::UTF8_to_UNICODE(std::string(ch));
						}

						std::wstring uri;
						if (image.HasMember("uri"))
						{
							const char* ch = image["uri"].GetString();
							uri = StringUtil::UTF8_to_UNICODE(std::string(ch));
						}
						else if (image.HasMember("url"))
						{
							const char* ch = image["url"].GetString();
							uri = StringUtil::UTF8_to_UNICODE(std::string(ch));
						}
						if (strFileName == U("") && uri == U(""))
						{
							return false;
						}

						gltfImage.m_strFileName = strFileName;
						gltfImage.m_strFile = uri;

						gltfImage.m_nBufferViewIndex = -i - 1;
						pTileInfos->m_mapImages[i] = gltfImage;
					}
				}
			}
		}

		return true;
	}

	bool GLTFParser::ParseBuffers(GLTFTileInfos_2* pTileInfos, rapidjson::Document& doc)
	{
		rapidjson::Value& buffers = doc["buffers"];
		for (unsigned int i = 0; i < buffers.Size(); i++)
		{
			rapidjson::Value& buffer = buffers[i];

			unsigned int nByteLength = buffer["byteLength"].GetUint();

			GLTFBuffer gltfBuffer;
			gltfBuffer.m_nByteLength = nByteLength;

			if (buffer.HasMember("uri"))
			{
				std::string strUri(buffer["uri"].GetString());
				gltfBuffer.m_strFile = StringUtil::UTF8_to_UNICODE(strUri);
			}
			else if (buffer.HasMember("url"))
			{
				std::string strUrl(buffer["url"].GetString());
				gltfBuffer.m_strFile = StringUtil::UTF8_to_UNICODE(strUrl);
			}
			pTileInfos->m_mapBuffers[i] = gltfBuffer;
		}
		return true;
	}

	bool GLTFParser::ParseBuffers(GLTFTileInfos_2* pTileInfos, GLBChunk& chunk)
	{
		if (chunk.m_pData == nullptr)
		{
			return false;
		}

		pTileInfos->m_mapBuffers[0] = GLTFBuffer();
		pTileInfos->m_mapBuffers[0].m_nByteLength = chunk.m_nLength;
		pTileInfos->m_mapBuffers[0].m_pBuffer = chunk.Give();
		pTileInfos->m_mapBuffers[0].m_bManageBuffer = true;
		return true;
	}

	bool GLTFParser::ParseExtensions(GLTFTileInfos_2* pTileInfos, rapidjson::Document& doc)
	{
		rapidjson::Value& extensions = doc["extensions"];
		if (extensions.Empty())
		{
			return false;
		}

		rapidjson::Value& cesium_rtc = extensions["CESIUM_RTC"];
		if (cesium_rtc.Empty())
		{
			return false;
		}

		rapidjson::Value& jCenter = cesium_rtc["center"];
		if (jCenter.Empty())
		{
			return false;
		}

		if (jCenter.IsArray())
		{
			pTileInfos->m_vCesuimRTC.x = jCenter[0].GetDouble();
			pTileInfos->m_vCesuimRTC.y = jCenter[1].GetDouble();
			pTileInfos->m_vCesuimRTC.z = jCenter[2].GetDouble();
		}

		return true;
	}

	void GLTFParser::DealGLTFFilePath(GLTFImage &gltfImage, std::wstring strFilePath)
	{
		if (gltfImage.m_strFile == U("") || strFilePath == U(""))
		{
			return;
		}

		std::wstring strDir = StringUtil::GetDir(strFilePath);
		std::wstring strKey = U("data:image/jpeg;base64,");
		int nLength = gltfImage.m_strFile.length();
		if (gltfImage.m_strFile.find(strKey) < nLength)
		{
			gltfImage.m_bCreatImage = true;
			gltfImage.m_strFile = gltfImage.m_strFile.substr(strKey.length());

			//做Base64解压
			std::wstring strDecode = BaseUtils::Base64Decode(gltfImage.m_strFile, nLength);
			std::string strBuffer = StringUtil::UNICODE_to_UTF8(strDecode);

			std::string strTexFile = StringUtil::UNICODE_to_UTF8(strDir + gltfImage.m_strFileName);

			FILE* file = fopen(strTexFile.c_str(), "wb");
			int i = fwrite(strBuffer.c_str(), sizeof(unsigned char), strBuffer.size(), file);
			fclose(file);

			gltfImage.m_strFile = strDir + gltfImage.m_strFileName;
		}
		else
		{
			gltfImage.m_strFile = StringUtil::GetAbsolutePath(strDir, gltfImage.m_strFile);
		}
	}
}
