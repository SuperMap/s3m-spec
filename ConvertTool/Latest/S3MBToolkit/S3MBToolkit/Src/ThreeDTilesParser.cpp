#include "ThreeDTilesParser.h"
#include "ImageOperator.h"
#include "MathEngine.h"
#include "Shell.h"
#include "S3MBCommon.h"
#include "S3MBConfig.h"
#include "S3MBUtils.h"
#include "S3MBWriter.h"
#include "Utils.h"

#include <fstream>
#include <strstream>

namespace S3MB
{
	ThreeDTilesParser::ThreeDTilesParser()
	{
		m_bIsNEU = false;

		m_nAxisUpType = AxisUpType::Y_UP;
		m_nRootCount = 0;
		m_bValidPosition = false;
		m_isPnts = false;
		m_nLodNum = 0;
		m_bAddMode = false;
		m_bIsNormal = false;
		m_bIsDistanceChange = false;
		m_nTotalNum = 0;
		m_nBatchIDNum = 0;
		m_textureCompressType = TextureCompressType::TC_NONE;
		m_nBufferSize = 0;

		S3MBFieldInfo idFieldInfo;
		idFieldInfo.m_bRequired = true;
		idFieldInfo.m_nType = FT_INT32;
		idFieldInfo.m_nSize = 4;
		idFieldInfo.m_strName = U("SmID");
		idFieldInfo.m_strForeignName = U("SmID");
		m_mapFieldIndex[U("SmID")] = 0;
		m_vecFieldInfo.push_back(idFieldInfo);
	}

	ThreeDTilesParser::~ThreeDTilesParser()
	{
	}

	void ThreeDTilesParser::SetIsNEU(bool bNEU)
	{
		m_bIsNEU = bNEU;
	}

	bool ThreeDTilesParser::GetIsNEU() const
	{
		return m_bIsNEU;
	}

	void ThreeDTilesParser::SetPosCenter(const Point3D& pos)
	{
		m_posCenter = pos;
	}

	const Point3D& ThreeDTilesParser::GetPosCenter() const
	{
		return m_posCenter;
	}

	const Point3D& ThreeDTilesParser::GetPosExtract() const
	{
		return m_posExtract;
	}

	const Matrix4d& ThreeDTilesParser::GetGlobalMat() const
	{
		return m_matGlobalMat;
	}

	void ThreeDTilesParser::SetGLTFNode(unsigned int i, GLTFTreeNode* pNode)
	{
		if (i < m_vecGLTFNodes.size())
		{
			m_vecGLTFNodes[i] = pNode;
		}
	}

	const std::vector<GLTFTreeNode*>& ThreeDTilesParser::GetGLTFNodes() const
	{
		return m_vecGLTFNodes;
	}

	void ThreeDTilesParser::ResetFieldInfos()
	{
		m_fieldInfos.RemoveAll();
		for (unsigned int i = 0; i < m_vecFieldInfo.size(); i++)
		{
			m_fieldInfos.add(m_vecFieldInfo[i]);
		}
	}

	const S3MBFieldInfos& ThreeDTilesParser::GetFieldInfos() const
	{
		return m_fieldInfos;
	}

	void ThreeDTilesParser::SetBatchIDNum(unsigned int nNum)
	{
		m_nBatchIDNum = nNum;
	}

	unsigned int ThreeDTilesParser::GetBatchIDNum() const
	{
		return m_nBatchIDNum;
	}

	void ThreeDTilesParser::AddFeature(Feature* pFeature)
	{
		m_vecFeature.push_back(pFeature);
	}

	const std::vector<Feature*>& ThreeDTilesParser::GetFeatures() const
	{
		return m_vecFeature;
	}

	void ThreeDTilesParser::ClearFeatures()
	{
		m_vecFeature.clear();
	}

	bool ThreeDTilesParser::GetIsAddMode() const
	{
		return m_bAddMode;
	}

	bool ThreeDTilesParser::GetIsDistanceChange() const
	{
		return m_bIsDistanceChange;
	}

	bool ThreeDTilesParser::GetIsValidPosition() const
	{
		return m_bValidPosition;
	}

	void ThreeDTilesParser::SetTextureCompressType(TextureCompressType nType)
	{
		m_textureCompressType = nType;
	}

	TextureCompressType ThreeDTilesParser::GetTextureCompressType() const
	{
		return m_textureCompressType;
	}

	void ThreeDTilesParser::SetBufferSize(unsigned int nSize)
	{
		m_nBufferSize = nSize;
	}

	unsigned int ThreeDTilesParser::GetBufferSize() const
	{
		return m_nBufferSize;
	}

	const I3DMIDInfo& ThreeDTilesParser::GetI3DMInfo() const
	{
		return m_i3dmInfo;
	}

	void ThreeDTilesParser::AddAttributeIndexInfo(const std::wstring& strKey, GLTFAttributeIndexInfo& info)
	{
		if (m_mapAttributeIndexInfos.find(strKey) == m_mapAttributeIndexInfos.end())
		{
			m_mapAttributeIndexInfos[strKey] = std::vector<GLTFAttributeIndexInfo>();
		}

		m_mapAttributeIndexInfos[strKey].push_back(info);
	}

	const std::map<std::wstring, std::vector<GLTFAttributeIndexInfo> >& ThreeDTilesParser::GetAttributeIndexInfos() const
	{
		return m_mapAttributeIndexInfos;
	}

	const std::map<std::wstring, std::vector<std::wstring> >& ThreeDTilesParser::GetMeshSets_V1()
	{
		return m_mapMeshSet_V1;
	}

	const GLTFMesh& ThreeDTilesParser::GetMesh(const std::wstring& strKey)
	{
		return m_mapMeshs_V1[strKey];
	}

	const Matrix4d& ThreeDTilesParser::GetMeshToLocalViewMatrix(const std::wstring& strKey)
	{
		return m_mapMeshTOLocalView_V1[strKey];
	}

	const GLTFAccessor& ThreeDTilesParser::GetAccessor(const std::wstring& strKey)
	{
		return m_mapAccessors_V1[strKey];
	}

	const GLTFBufferView& ThreeDTilesParser::GetBufferView(const std::wstring& strKey)
	{
		return m_mapBufferViews_V1[strKey];
	}

	const GLTFMaterial& ThreeDTilesParser::GetMaterial(const std::wstring& strKey)
	{
		return m_mapMaterials_V1[strKey];
	}

	const GLTFTexture& ThreeDTilesParser::GetTexture(const std::wstring& strKey)
	{
		return m_mapTextures_V1[strKey];
	}

	const GLTFImage& ThreeDTilesParser::GetImage(const std::wstring& strKey)
	{
		return m_mapImages_V1[strKey];
	}

	const std::map<unsigned int, std::vector<unsigned int> >& ThreeDTilesParser::GetMeshSets()
	{
		return m_mapMeshSet;
	}

	const GLTFMesh& ThreeDTilesParser::GetMesh(unsigned int nKey)
	{
		return m_mapMeshs[nKey];
	}

	const Matrix4d& ThreeDTilesParser::GetMeshToLocalViewMatrix(unsigned int nKey)
	{
		return m_mapMeshToLocalView[nKey];
	}

	const GLTFAccessor& ThreeDTilesParser::GetAccessor(unsigned int nKey)
	{
		return m_mapAccessors[nKey];
	}

	const GLTFBufferView& ThreeDTilesParser::GetBufferView(unsigned int nKey)
	{
		return m_mapBufferViews[nKey];
	}

	const GLTFBuffer& ThreeDTilesParser::GetBuffer(unsigned int nKey)
	{
		return m_mapBuffers[nKey];
	}

	const std::vector<GLTFMaterial>& ThreeDTilesParser::GetMaterials(unsigned int nKey)
	{
		return m_mapMaterials[nKey];
	}

	const GLTFTexture& ThreeDTilesParser::GetTexture(unsigned int nKey)
	{
		return m_mapTextures[nKey];
	}

	const GLTFImage& ThreeDTilesParser::GetImage(unsigned int nKey)
	{
		return m_mapImages[nKey];
	}

	bool ThreeDTilesParser::ParseTileset(std::wstring strPath, GLTFTreeNode* pParentNode, int nLod, Matrix4d matParent)
	{
		if (!StringUtil::IsFileExist(strPath))
		{
			return false;
		}

		// 读取Tileset.json文件
        std::string strTemp = StringUtil::UNICODE_to_UTF8(strPath);
		strTemp = StringUtil::UTF8_to_ANSI(strTemp);
        std::ifstream stream(strTemp.c_str());
		rapidjson::IStreamWrapper wrapper(stream);
		rapidjson::Document doc;
		doc.ParseStream(wrapper);
		if (doc.HasParseError())
		{
			return false;
		}

		if (doc.HasMember("asset"))
		{
			rapidjson::Value& asset = doc["asset"];
			if (asset.HasMember("gltfUpAxis"))
			{
				std::string strUpAxis = asset["gltfUpAxis"].GetString();
				if (strUpAxis == "Z")
				{
					m_nAxisUpType = AxisUpType::Z_UP;
				}
			}
		}

		ParseRoot(doc, StringUtil::GetDir(strPath), pParentNode, nLod, matParent);

		return true;
	}

	void ThreeDTilesParser::ComputeTransMat()
	{
		if (m_matGlobalMat == Matrix4d::IDENTITY)
		{
			return;
		}
		for (int i = 0; i < m_vecGLTFNodes.size(); i++)
		{
			RecurComputerRelatMat(m_vecGLTFNodes[i]);
		}
		if (!m_bIsNEU)
		{
			//获得矩阵上的平移点为插入点
			Vector3d vecCenterTemp = m_matGlobalMat.GetTrans();
			//计算椭圆上经纬度  单位弧度
			Vector3d vec3Position = MathEngine::CartesianToWGS84(vecCenterTemp.x, vecCenterTemp.y, vecCenterTemp.z, GS_ELLIPSOID);
			vec3Position.z -= GLOBAL_RADIUS;
			//计算出插入点的经纬度
			m_posCenter = Point3D(vec3Position.x * RTOD, vec3Position.y * RTOD, vec3Position.z);

			Vector3d vUp(vecCenterTemp.x * wgs84OneOverRadiiSquared.x, vecCenterTemp.y * wgs84OneOverRadiiSquared.y, vecCenterTemp.z * wgs84OneOverRadiiSquared.z);
			vUp.Normalize();

			Vector3d vEast(-vecCenterTemp.y, vecCenterTemp.x, 0.0);
			vEast.Normalize();

			Vector3d vNorth = VectorUtils::CrossProduct(vUp, vEast);
			vNorth.Normalize();

			Matrix4d matRot = Matrix4d(
                vEast.x, vEast.y, vEast.z, 0.0,
                vNorth.x, vNorth.y, vNorth.z, 0.0,
                vUp.x, vUp.y, vUp.z, 0.0,
                0, 0, 0, 1.0);
			//矩阵直接取原始3dtiles的旋转矩阵
			m_matENUWorldView = m_matGlobalMat;

			matRot = MatrixUtils::RotationXYZ(-m_posCenter.y* DTOR, m_posCenter.x* DTOR, 0);
			//圆球笛卡尔
			Vector3d vecCenter = MathEngine::SphericalToCartesian(m_posCenter.x * DTOR, m_posCenter.y * DTOR, m_posCenter.z + GLOBAL_RADIUS);
			m_matSphereWorldView = matRot * MatrixUtils::Translation(vecCenter.x, vecCenter.y, vecCenter.z);
		}
		else
		{
			//获得矩阵上的平移点为插入点 
			Vector3d vecCenterTemp = m_matGlobalMat.GetTrans();
			//计算椭圆上经纬度  单位弧度
			Vector3d vec3Position = MathEngine::CartesianToWGS84(vecCenterTemp.x, vecCenterTemp.y, vecCenterTemp.z);
			vec3Position.z -= GLOBAL_RADIUS;
			//计算出插入点的经纬度
			m_posCenter = Point3D(vec3Position.x * RTOD, vec3Position.y * RTOD, vec3Position.z);
			m_matENUWorldView = MathEngine::EllipsoidalENUToWGS84(Vector3d(vec3Position.x*RTOD, vec3Position.y*RTOD, vec3Position.z));
		}
	}

	bool ThreeDTilesParser::ParseInstanceInfo(rapidjson::Document& doc, unsigned char* buffer)
	{
		ResetInstanceInfo();

		//实例化个数（必要）
		rapidjson::Value& instanceLen = doc["INSTANCES_LENGTH"];
		m_i3dmInfo.m_nIdLen = instanceLen.GetUint();

		if (doc.HasMember("BATCH_ID"))
		{
			unsigned int offset_ID = doc["BATCH_ID"].GetUint();
			unsigned short* pID = (unsigned short*)(buffer + offset_ID);
			for (unsigned int i = 0; i < m_i3dmInfo.m_nIdLen; i++)
			{
				m_i3dmInfo.m_vecId.push_back(pID[i]);
				m_i3dmInfo.m_vecMat.push_back(Matrix4d::IDENTITY);
			}
		}

		if (doc.HasMember("NORMAL_UP"))
		{
			unsigned int offset_NormalU = doc["NORMAL_UP"]["byteOffset"].GetUint();
			unsigned int offset_NormalR = doc["NORMAL_RIGHT"]["byteOffset"].GetUint();
			std::vector<Vector3d> vecNormalUp;
			std::vector<Vector3d> vecNormalRight;
			float* pNormalU = (float*)(buffer + offset_NormalU);
			float* pNormalR = (float*)(buffer + offset_NormalR);
			for (unsigned int i = 0; i < m_i3dmInfo.m_nIdLen; i++)
			{
				Vector3d v3Up(pNormalU[3 * i], pNormalU[3 * i + 1], pNormalU[3 * i + 2]);
				Vector3d v3Right(pNormalR[3 * i], pNormalR[3 * i + 1], pNormalR[3 * i + 2]);
				v3Up.Normalize();
				v3Right.Normalize();
				vecNormalUp.push_back(v3Up);
				vecNormalRight.push_back(v3Right);
				
				Vector3d vNorth = VectorUtils::CrossProduct(vecNormalUp[i], vecNormalRight[i]);
				vNorth.Normalize();
				Matrix4d matRot = Matrix4d(
					vecNormalRight[i].x, vecNormalRight[i].y, vecNormalRight[i].z, 0.0,		// E
					vecNormalUp[i].x, vecNormalUp[i].y, vecNormalUp[i].z, 0.0,			// U
					-vNorth.x, -vNorth.y, -vNorth.z, 0.0,	// N
					0, 0, 0, 1.0);
				m_i3dmInfo.m_vecMat[i] = m_i3dmInfo.m_vecMat[i] * matRot;
			}
		}
		else if (doc.HasMember("NORMAL_UP_OCT32P"))
		{
			unsigned int offset_NormalU_O = doc["NORMAL_UP_OCT32P"]["byteOffset"].GetUint();
			unsigned int offset_NormalR_O = doc["NORMAL_RIGHT_OCT32P"]["byteOffset"].GetUint();
			std::vector<Vector3d> vecNormalUp;
			std::vector<Vector3d> vecNormalRight;
			unsigned short* pNormalU = (unsigned short*)(buffer + offset_NormalU_O);
			unsigned short* pNormalR = (unsigned short*)(buffer + offset_NormalR_O);
			for (unsigned int i = 0; i < m_i3dmInfo.m_nIdLen; i++)
			{
				vecNormalUp.push_back(GetDecodeNormal(pNormalU[2 * i], pNormalU[2 * i + 1]));
				vecNormalRight.push_back(GetDecodeNormal(pNormalR[2 * i], pNormalR[2 * i + 1]));

				Vector3d vNorth = VectorUtils::CrossProduct(vecNormalUp[i], vecNormalRight[i]);
				vNorth.Normalize();
				Matrix4d matRot = Matrix4d(
					vecNormalRight[i].x, vecNormalRight[i].y, vecNormalRight[i].z, 0.0,		// E
					vecNormalUp[i].x, vecNormalUp[i].y, vecNormalUp[i].z, 0.0,			// U
					-vNorth.x, -vNorth.y, -vNorth.z, 0.0,	// N
					0, 0, 0, 1.0);
				m_i3dmInfo.m_vecMat[i] = m_i3dmInfo.m_vecMat[i] * matRot;
			}
		}
		else if (doc.HasMember("EAST_NORTH_UP"))
		{
			bool bENU = doc["EAST_NORTH_UP"].GetBool();
		}

		if (doc.HasMember("SCALE"))
		{
			unsigned int offset_Scale = doc["SCALE"]["byteOffset"].GetUint();
			float* pScale = (float*)(buffer + offset_Scale);
			for (unsigned int i = 0; i < m_i3dmInfo.m_nIdLen; i++)
			{
				m_i3dmInfo.m_vecMat[i] = m_i3dmInfo.m_vecMat[i] * MatrixUtils::Scaling(pScale[i], pScale[i], pScale[i]);
			}
		}
		else if (doc.HasMember("SCALE_NON_UNIFORM"))
		{
			unsigned int offset_Scale = doc["SCALE_NON_UNIFORM"]["byteOffset"].GetUint();
			float* pScale = (float*)(buffer + offset_Scale);
			for (unsigned int i = 0; i < m_i3dmInfo.m_nIdLen; i++)
			{
				m_i3dmInfo.m_vecMat[i] = m_i3dmInfo.m_vecMat[i] * MatrixUtils::Scaling(pScale[3 * i], pScale[3 * i + 1], pScale[3 * i + 2]);
			}
		}

		if (doc.HasMember("POSITION"))
		{
			unsigned int offset_Position = doc["POSITION"]["byteOffset"].GetUint();
			float* pPos = (float*)(buffer + offset_Position);
			for (unsigned int i = 0; i < m_i3dmInfo.m_nIdLen; i++)
			{
				m_i3dmInfo.m_vecMat[i] = m_i3dmInfo.m_vecMat[i] * MatrixUtils::Translation(pPos[3 * i], pPos[3 * i + 1], pPos[3 * i + 2]);
			}
		}
		else if (doc.HasMember("POSITION_QUANTIZED"))
		{
			unsigned int offset_Position_Q = doc["POSITION_QUANTIZED"]["byteOffset"].GetUint();
			Vector3d vecScale = Vector3d(doc["QUANTIZED_VOLUME_SCALE"][0].GetFloat(), doc["QUANTIZED_VOLUME_SCALE"][1].GetFloat(),
				doc["QUANTIZED_VOLUME_SCALE"][2].GetFloat());
			Vector3d vecOffset = Vector3d(doc["QUANTIZED_VOLUME_OFFSET"][0].GetFloat(), doc["QUANTIZED_VOLUME_OFFSET"][1].GetFloat(),
				doc["QUANTIZED_VOLUME_OFFSET"][2].GetFloat());
			unsigned short* pPos = (unsigned short*)(buffer + offset_Position_Q);
			for (unsigned int i = 0; i < m_i3dmInfo.m_nIdLen; i++)
			{
				Vector3d pos = (Vector3d(pPos[3 * i] * vecScale[0], pPos[3 * i + 1] * vecScale[1], pPos[3 * i + 2] * vecScale[2]) / 65535.0) + vecOffset;
				m_i3dmInfo.m_vecMat[i].SetTrans(pos);
			}
		}

		return true;
	}

	void ThreeDTilesParser::ResetInstanceInfo()
	{
		m_i3dmInfo = I3DMIDInfo();
	}

	void ThreeDTilesParser::ConvertPoint(GLTFTreeNode* pNode, float* pPosData, const unsigned int nVertexCount, const unsigned int nPosDim, const Matrix4d matLocal, const Point3D& pntCenter)
	{
		//没有设置插入点，就不需要转换，只有设置正确插入点才走这里面
		if (m_posCenter == Point3D() && m_matGlobalMat == Matrix4d::IDENTITY)
		{
			if (nVertexCount == 0)
			{
				return;
			}
			Vector3d vecPos;
			//当中心点为0时从数据中提取中心点
			if (m_posExtract == Point3D())
			{
				vecPos = Vector3d(pPosData[0], pPosData[1], pPosData[2]).MultiplyMatrix(matLocal);
				Vector3d vecExtract = MathEngine::CartesianToWGS84(vecPos[0] + pntCenter.x, vecPos[1] + pntCenter.y, vecPos[2] + pntCenter.z);
				vecExtract.z -= GLOBAL_RADIUS;
				m_posExtract.x = vecExtract.x * RTOD;
				m_posExtract.y = vecExtract.y * RTOD;
				m_posExtract.z = vecExtract.z;
				if (!m_bIsNEU)
				{//转圆球
					Matrix4d matRot = MatrixUtils::RotationXYZ(-m_posExtract.y* DTOR, m_posExtract.x* DTOR, 0);
					//圆球笛卡尔
					Vector3d vecCenter = MathEngine::SphericalToCartesian(m_posExtract.x * DTOR, m_posExtract.y * DTOR, m_posExtract.z + GLOBAL_RADIUS);
					m_matSphereWorldView = matRot * MatrixUtils::Translation(vecCenter.x, vecCenter.y, vecCenter.z);
				}
				else
				{//转椭球
					m_matENUWorldView = MathEngine::EllipsoidalENUToWGS84(Vector3d(vecExtract.x*RTOD, vecExtract.y*RTOD, vecExtract.z));
				}
			}

			for (int i = 0; i < nVertexCount; i++)
			{
				vecPos = Vector3d(pPosData[3 * i + 0], pPosData[3 * i + 1], pPosData[3 * i + 2]).MultiplyMatrix(matLocal);
				Vector3d vecPnt;
				if (!m_bIsNEU)
				{
					vecPnt = MathEngine::CartesianToWGS84(vecPos[0] + pntCenter.x, vecPos[1] + pntCenter.y, vecPos[2] + pntCenter.z);
					vecPnt = MathEngine::SphericalToCartesian(vecPnt.x, vecPnt.y, vecPnt.z);
					vecPnt = vecPnt.MultiplyMatrix(m_matSphereWorldView.Invert());
				}
				else
				{
					vecPnt = Vector3d(vecPos[0] + pntCenter.x, vecPos[1] + pntCenter.y, vecPos[2] + pntCenter.z);
					vecPnt = vecPnt.MultiplyMatrix(m_matENUWorldView.Invert());
				}
				pPosData[3 * i + 0] = vecPnt.x; pPosData[3 * i + 1] = vecPnt.y; pPosData[3 * i + 2] = vecPnt.z;
			}
			return;
		}
		Matrix4d tempMat;
		Vector3d posCenter;

		Vector3d v3Position, v3Center, v3OriPos;
		Point3D point3D;
		int nVertexDimension = nPosDim;
		int nNumDim = nVertexDimension > 3 ? 3 : nVertexDimension;
		for (int i = 0; i < nVertexCount; i++)
		{
			for (int j = 0; j < nNumDim; j++)
			{
				v3OriPos[j] = pPosData[nVertexDimension * i + j];
			}

			//乘以矩阵  现在是相对总插入点的局部坐标
			v3Center = v3OriPos.MultiplyMatrix(matLocal);

			//加上相对中心点
			v3Center.x = v3Center.x + pntCenter.x;
			v3Center.y = v3Center.y + pntCenter.y;
			v3Center.z = v3Center.z + pntCenter.z;
			v3Center = v3Center.MultiplyMatrix(pNode->m_RelatMat);

			if (!m_bIsNEU)
			{//转圆球
				//归到原点 算真实坐标系
				v3Center = v3Center.MultiplyMatrix(m_matENUWorldView);

				//坐标变换
				point3D.x = v3Center.x;
				point3D.y = v3Center.y;
				point3D.z = v3Center.z;

				Vector3d v3CurLat = MathEngine::CartesianToWGS84(point3D.x, point3D.y, point3D.z);
				v3Position = MathEngine::SphericalToCartesian(v3CurLat.x, v3CurLat.y, v3CurLat.z);
				v3Position = v3Position.MultiplyMatrix(m_matSphereWorldView.Invert());
			}
			else
			{//转椭球
				//归到原点 算真实坐标系
				v3Center = v3Center.MultiplyMatrix(m_matGlobalMat);
                v3Center = v3Center.MultiplyMatrix(m_matENUWorldView.Invert());
				v3Position = v3Center;
			}
			for (int j = 0; j < nNumDim; j++)
			{
				pPosData[nVertexDimension * i + j] = v3Position[j];
			}
		}
	}

	void ThreeDTilesParser::Clear()
	{
		m_mapMeshs_V1.clear();
		m_mapMeshTOLocalView_V1.clear();
		m_mapMeshSet_V1.clear();
		m_mapAccessors_V1.clear();
		m_mapBuffers_V1.clear();
		m_mapBufferViews_V1.clear();
		m_mapMaterials_V1.clear();
		m_mapImages_V1.clear();
		m_mapTextures_V1.clear();

		m_mapMeshs.clear();
		m_mapMeshToLocalView.clear();
		m_mapMeshSet.clear();
		m_mapAccessors.clear();
		m_mapBuffers.clear();
		m_mapBufferViews.clear();
		m_mapMaterials.clear();
		m_mapImages.clear();
		m_mapTextures.clear();

		m_nBufferSize = 0;
		ResetInstanceInfo();
	}

	bool ThreeDTilesParser::ParseRoot(rapidjson::Document& doc, std::wstring strDir, GLTFTreeNode* pParentNode, int nLod, Matrix4d matParent)
	{
		if (!doc.HasMember("root"))
		{
			return false;
		}

		rapidjson::Value& properties = doc["properties"];
		if (!properties.Empty())
		{
			rapidjson::Value& latitude = properties["Latitude"];
			Vector3d vMax, vMin;
			if (!latitude.Empty())
			{
				double dMin = latitude["minimum"].GetDouble() * RTOD;
				double dMax = latitude["maximum"].GetDouble() * RTOD;

				vMax.y = dMax;
				vMin.y = dMin;
			}

			rapidjson::Value& longitude = properties["Longitude"];
			if (!longitude.Empty())
			{
				double dMin = longitude["minimum"].GetDouble() * RTOD;
				double dMax = longitude["maximum"].GetDouble() * RTOD;

				vMax.x = dMax;
				vMin.x = dMin;
			}

			// 求一个半径出来
			vMax = MathEngine::SphericalToCartesian(vMax.x * DTOR, vMax.y * DTOR, vMax.z + GLOBAL_RADIUS);
			vMin = MathEngine::SphericalToCartesian(vMin.x * DTOR, vMin.y * DTOR, vMin.z + GLOBAL_RADIUS);

			BoundingBox bbox(vMin, vMax);
			m_boundingSphere = BoundingSphere(bbox);

			for (auto itor = properties.MemberBegin(); itor != properties.MemberEnd(); itor++)
			{
				auto name = (itor->name).GetString();
				std::string strName(name);
				std::wstring wstrName = StringUtil::UTF8_to_UNICODE(strName);

				S3MBFieldInfo fieldInfo;
				fieldInfo.m_nType = FieldType::FT_Text;
				fieldInfo.m_nSize = 255;
				fieldInfo.m_strName = wstrName;
				fieldInfo.m_strForeignName = wstrName;
				fieldInfo.m_bRequired = false;

				if (m_mapFieldIndex.find(wstrName) == m_mapFieldIndex.end())
				{
					m_mapFieldIndex[wstrName] = m_vecFieldInfo.size();
					m_vecFieldInfo.push_back(fieldInfo);
				}
			}
		}

		rapidjson::Value& root = doc["root"];

		//矩阵
		if (root.HasMember("transform"))
		{
			rapidjson::Value& transform = root["transform"];
			if (transform.IsArray())
			{
				Matrix4d mat(
					transform[0].GetDouble(),
					transform[1].GetDouble(),
					transform[2].GetDouble(),
					transform[3].GetDouble(),
					transform[4].GetDouble(),
					transform[5].GetDouble(),
					transform[6].GetDouble(),
					transform[7].GetDouble(),
					transform[8].GetDouble(),
					transform[9].GetDouble(),
					transform[10].GetDouble(),
					transform[11].GetDouble(),
					transform[12].GetDouble(),
					transform[13].GetDouble(),
					transform[14].GetDouble(),
					transform[15].GetDouble()
				);

				if (m_matGlobalMat == Matrix4d::IDENTITY && matParent == Matrix4d::IDENTITY)
				{
					m_matGlobalMat = mat;
				}
				matParent = mat * matParent;
			}
		}

		BoundingSphere sphere;
		if (root.HasMember("boundingVolume"))
		{
			rapidjson::Value& boundingVolume = root["boundingVolume"];

			if (boundingVolume.HasMember("sphere"))
			{
				rapidjson::Value& boundingSphere = boundingVolume["sphere"];
				if (boundingSphere.IsArray())
				{
					Vector3d center(
						boundingSphere[0].GetDouble(),
						boundingSphere[1].GetDouble(),
						boundingSphere[2].GetDouble());

					double radius = boundingSphere[3].GetDouble();

					sphere.m_center = center;
					sphere.m_radius = radius;
				}
			}
			else if (boundingVolume.HasMember("region"))
			{
				rapidjson::Value& region = boundingVolume["region"];
				if (region.IsArray())
				{
					Vector3d vMin(
						region[0].GetDouble(),
						region[1].GetDouble(),
						region[4].GetDouble());

					Vector3d vMax(
						region[2].GetDouble(),
						region[3].GetDouble(),
						region[5].GetDouble());

					vMin = MathEngine::SphericalToCartesian(region[0].GetDouble(), region[1].GetDouble(), region[4].GetDouble() + GLOBAL_RADIUS);
					vMax = MathEngine::SphericalToCartesian(region[2].GetDouble(), region[3].GetDouble(), region[5].GetDouble() + GLOBAL_RADIUS);

					BoundingBox box(vMin, vMax);
					BoundingSphere tempSphere(box);

					sphere = tempSphere;
					m_boundingSphere = sphere;
				}
			}
			else if (boundingVolume.HasMember("box"))
			{
				rapidjson::Value& box = boundingVolume["box"];
				Point3D pos(
					box[0].GetDouble(),
					box[1].GetDouble(),
					box[2].GetDouble());

				Vector3d vMin(
					pos.x - box[3].GetDouble(),
					pos.y - box[7].GetDouble(),
					pos.z - box[11].GetDouble());

				Vector3d vMax(
					pos.x + box[3].GetDouble(),
					pos.y + box[7].GetDouble(),
					pos.z + box[11].GetDouble());

				BoundingBox Boundingbox(vMin, vMax);
				BoundingSphere tempSphere(Boundingbox);

				sphere = tempSphere;
				m_boundingSphere = sphere;
			}
		}

		GLTFTreeNode* pTreeNode = nullptr;
		if (root.HasMember("content"))
		{
			rapidjson::Value& content = root["content"];
			if (content.HasMember("url") || content.HasMember("uri"))
			{
				rapidjson::Value& url = content["url"];
				if (content.HasMember("uri"))
				{
					url = content["uri"];
				}
				std::wstring strFile = StringUtil::UTF8_to_UNICODE(url.GetString());

				if (!strFile.empty())
				{
					std::vector<std::wstring> vecStr;
					StringUtil::Split(strFile, vecStr, '?');
					strFile = vecStr[0];
				}

				std::wstring wstrExt = StringUtil::GetExt(strFile);
				if (wstrExt == U(".json"))
				{
					strFile = StringUtil::GetAbsolutePath(strDir, strFile);
					ParseTileset(strFile, pParentNode, nLod, matParent);
				}
				else if (!strFile.empty())
				{
					if (wstrExt == U(".pnts"))
					{
						//点云 
						m_isPnts = true;
					}
					pTreeNode = new GLTFTreeNode;
					pTreeNode->m_strFile = strFile;
					strFile = StringUtil::GetAbsolutePath(strDir, strFile);
					if (StringUtil::IsFileExist(strFile))
					{
						pTreeNode->m_bFileFind = true;
					}
					pTreeNode->m_boundSphere = sphere;
					pTreeNode->m_strParentDir = strDir;
					pTreeNode->m_nLod = nLod;
					pTreeNode->m_RootMat = matParent;

					if (root.HasMember("geometricError"))
					{
						pTreeNode->m_dGeometryError = root["geometricError"].GetDouble();
					}

					m_nLodNum = MAX(m_nLodNum, nLod);
					m_nTotalNum++;
					if (pParentNode != nullptr)
					{
						pParentNode->m_vecChildNode.push_back(pTreeNode);
						pTreeNode->m_pParentNode = pParentNode;
					}
				}
				else
				{
					pTreeNode = pParentNode;
				}
			}
		}
		else
		{
			if (root.HasMember("children"))
			{
				rapidjson::Value& children = root["children"];
				if (children.IsArray() && children.Size() == 1)
				{
					rapidjson::Value& child = children[0];

					Matrix4d matChild = Matrix4d::IDENTITY;
					ParseChildren(child, strDir, pParentNode, nLod + 1, matParent);
					return true;
				}
				if (children.HasMember("content") && pParentNode == nullptr)
				{
					pTreeNode = new GLTFTreeNode;
					std::wstring strRootName = U("Root_") + BaseUtils::CreateGuid();
					strRootName = StringUtil::GetAbsolutePath(strDir, strRootName);
					pTreeNode->m_strFile = strRootName;
					pTreeNode->m_strParentDir = strDir;
					pTreeNode->m_boundSphere = sphere;
					pTreeNode->m_nLod = nLod;
					pTreeNode->m_RootMat = matParent;

					if (root.HasMember("geometricError"))
					{
						pTreeNode->m_dGeometryError = root["geometricError"].GetDouble();
					}

					m_nLodNum = MAX(m_nLodNum, nLod);
				}
			}
		}

		if (root.HasMember("children"))
		{
			rapidjson::Value& children = root["children"];
			if (children.IsArray())
			{
				int nChildren = children.Size();
				for (int i = 0; i < nChildren; i++)
				{
					rapidjson::Value& child = children[i];
					Matrix4d matChild = Matrix4d::IDENTITY;
					ParseChildren(child, strDir, pTreeNode, nLod + 1, matParent);
				}
			}
		}
		//追加还是替换
		if (root.HasMember("refine"))
		{
			rapidjson::Value& refine = root["refine"];
			std::string strFile = refine.GetString();
			if (strFile == "ADD")
			{
				m_bAddMode = true;
			}
		}
		if (pTreeNode && pParentNode == nullptr)
		{
			m_vecGLTFNodes.push_back(pTreeNode);
		}
		return true;
	}

	bool ThreeDTilesParser::ParseInnerRoot(rapidjson::Value& value, std::wstring strDir, GLTFTreeNode* pParentNode, int nLod, Matrix4d matParent)
	{
		GLTFTreeNode* pTreeNode = nullptr;
		BoundingSphere sphere;
		if (value.HasMember("boundingVolume"))
		{
			rapidjson::Value& boundingVolume = value["boundingVolume"];
			if (boundingVolume.HasMember("sphere"))
			{
				rapidjson::Value& boundingSphere = boundingVolume["sphere"];
				if (boundingSphere.IsArray())
				{
					Vector3d center(
						boundingSphere[0].GetDouble(),
						boundingSphere[1].GetDouble(),
						boundingSphere[2].GetDouble());

					double radius = boundingSphere[3].GetDouble();

					sphere.m_center = center;
					sphere.m_radius = radius;
				}
			}
			else if (boundingVolume.HasMember("region"))
			{
				rapidjson::Value& region = boundingVolume["region"];
				if (region.IsArray())
				{
					Vector3d vMin(
						region[0].GetDouble(),
						region[1].GetDouble(),
						region[4].GetDouble());

					Vector3d vMax(
						region[2].GetDouble(),
						region[3].GetDouble(),
						region[5].GetDouble());

					vMin = MathEngine::SphericalToCartesian(region[0].GetDouble(), region[1].GetDouble(), region[4].GetDouble() + GLOBAL_RADIUS);
					vMax = MathEngine::SphericalToCartesian(region[2].GetDouble(), region[3].GetDouble(), region[5].GetDouble() + GLOBAL_RADIUS);

					BoundingBox box(vMin, vMax);
					sphere = BoundingSphere(box);
				}
			}
			else if (boundingVolume.HasMember("box"))
			{
				rapidjson::Value& box = boundingVolume["box"];
				Point3D pos(
					box[0].GetDouble(),
					box[1].GetDouble(),
					box[2].GetDouble());

				Vector3d vMin(
					pos.x - box[3].GetDouble(),
					pos.y - box[7].GetDouble(),
					pos.z - box[11].GetDouble());

				Vector3d vMax(
					pos.x + box[3].GetDouble(),
					pos.y + box[7].GetDouble(),
					pos.z + box[11].GetDouble());

				BoundingBox boundingBox(vMin, vMax);
				sphere = BoundingSphere(boundingBox);
				m_boundingSphere = sphere;
			}
		}

		if (value.HasMember("content"))
		{
			rapidjson::Value & content = value["content"];
			if (content.HasMember("url") || content.HasMember("uri"))
			{
				rapidjson::Value & url = content["url"];
				if (content.HasMember("uri"))
				{
					url = content["uri"];
				}

				std::wstring strFile = StringUtil::UTF8_to_UNICODE(url.GetString());

				if (!strFile.empty())
				{
					std::vector<std::wstring> vecStr;
					StringUtil::Split(strFile, vecStr, '?');
					strFile = vecStr[0];
				}
				
				std::wstring strExt = StringUtil::GetExt(strFile);
				if (strExt == U(".json"))
				{
					strFile = StringUtil::GetAbsolutePath(strDir, strFile);
					ParseTileset(strFile, pParentNode, nLod, matParent);
				}
				else if (!strFile.empty())
				{
					if (strExt == U(".pnts"))
					{
						//点云 
						m_isPnts = true;
					}
					pTreeNode = new GLTFTreeNode;
					pTreeNode->m_strFile = strFile;
					strFile = StringUtil::GetAbsolutePath(strDir, strFile);
					if (StringUtil::IsFileExist(strFile))
					{
						pTreeNode->m_bFileFind = true;
					}
					pTreeNode->m_boundSphere = sphere;
					pTreeNode->m_strParentDir = strDir;
					pTreeNode->m_nLod = nLod;
					pTreeNode->m_RootMat = matParent;

					if (value.HasMember("geometricError"))
					{
						pTreeNode->m_dGeometryError = value["geometricError"].GetDouble();
					}

					m_nTotalNum++;
					m_nLodNum = MAX(m_nLodNum, nLod);

					if (pParentNode)
					{
						pParentNode->m_vecChildNode.push_back(pTreeNode);
						pTreeNode->m_pParentNode = pParentNode;
					}
				}
				else
				{
					pTreeNode = pParentNode;
				}
			}
		}

		if (value.HasMember("children"))
		{
			rapidjson::Value & children = value["children"];
			if (children.IsArray())
			{
				int nChildren = children.Size();
				for (int i = 0; i < nChildren; i++)
				{
					rapidjson::Value& child = children[i];
					if (pTreeNode == nullptr)
					{
						ParseInnerRoot(child, strDir, pTreeNode, 0, matParent);
					}
					else
					{
						ParseChildren(child, strDir, pTreeNode, nLod + 1, matParent);
					}
				}
			}
		}

		if (pTreeNode && pParentNode == nullptr)
		{
			m_vecGLTFNodes.push_back(pTreeNode);
		}
		return true;
	}

	bool ThreeDTilesParser::ParseChildren(rapidjson::Value& value, std::wstring strDir, GLTFTreeNode* pParentNode, int nLod, Matrix4d matParent)
	{
		if (value.IsNull())
		{
			return false;
		}

		if (value.HasMember("transform"))
		{
			rapidjson::Value& transform = value["transform"];
			if (transform.IsArray())
			{
				Matrix4d mat(
					transform[0].GetDouble(),
					transform[1].GetDouble(),
					transform[2].GetDouble(),
					transform[3].GetDouble(),
					transform[4].GetDouble(),
					transform[5].GetDouble(),
					transform[6].GetDouble(),
					transform[7].GetDouble(),
					transform[8].GetDouble(),
					transform[9].GetDouble(),
					transform[10].GetDouble(),
					transform[11].GetDouble(),
					transform[12].GetDouble(),
					transform[13].GetDouble(),
					transform[14].GetDouble(),
					transform[15].GetDouble()
				);

				if (m_matGlobalMat == Matrix4d::IDENTITY && matParent == Matrix4d::IDENTITY)
				{
					m_matGlobalMat = mat;
				}
				matParent = mat * matParent;
			}
		}

		BoundingSphere sphere;
		if (value.HasMember("boundingVolume"))
		{
			rapidjson::Value& boundingVolume = value["boundingVolume"];

			if (boundingVolume.HasMember("sphere"))
			{
				rapidjson::Value& boundingSphere = boundingVolume["sphere"];
				if (boundingSphere.IsArray())
				{
					Vector3d center(
						boundingSphere[0].GetDouble(),
						boundingSphere[1].GetDouble(),
						boundingSphere[2].GetDouble());

					double radius = boundingSphere[3].GetDouble();

					sphere.m_center = center;
					sphere.m_radius = radius;
				}
			}
			else if (boundingVolume.HasMember("region"))
			{
				rapidjson::Value & region = boundingVolume["region"];

				if (region.IsArray())
				{
					Vector3d vMin(
						region[0].GetDouble(),
						region[1].GetDouble(),
						region[4].GetDouble());

					Vector3d vMax(
						region[2].GetDouble(),
						region[3].GetDouble(),
						region[5].GetDouble());

					vMin = MathEngine::SphericalToCartesian(region[0].GetDouble(), region[1].GetDouble(), region[4].GetDouble() + GLOBAL_RADIUS);
					vMax = MathEngine::SphericalToCartesian(region[2].GetDouble(), region[3].GetDouble(), region[5].GetDouble() + GLOBAL_RADIUS);

					BoundingBox box(vMin, vMax);
					BoundingSphere tempSphere(box);

					sphere = tempSphere;
				}
			}
			else if (boundingVolume.HasMember("box"))
			{
				rapidjson::Value & box = boundingVolume["box"];
				Point3D pos(
					box[0].GetDouble(),
					box[1].GetDouble(),
					box[2].GetDouble());

				Vector3d vMin(
					pos.x - box[3].GetDouble(),
					pos.y - box[7].GetDouble(),
					pos.z - box[11].GetDouble());

				Vector3d vMax(
					pos.x + box[3].GetDouble(),
					pos.y + box[7].GetDouble(),
					pos.z + box[11].GetDouble());

				BoundingBox Boundingbox(vMin, vMax);
				BoundingSphere tempSphere(Boundingbox);

				sphere = tempSphere;
				m_boundingSphere = sphere;
			}
		}

		GLTFTreeNode* pTreeNode = nullptr;
		if (value.HasMember("content"))
		{
			rapidjson::Value& content = value["content"];
			if (content.HasMember("url") || content.HasMember("uri"))
			{
				rapidjson::Value& url = content["url"];
				if (content.HasMember("uri"))
				{
					url = content["uri"];
				}
				std::wstring strFile = StringUtil::UTF8_to_UNICODE(url.GetString());

				if (!strFile.empty())
				{
					std::vector<std::wstring> vecStr;
					StringUtil::Split(strFile, vecStr, '?');
					strFile = vecStr[0];
				}

				std::wstring strExt = StringUtil::GetExt(strFile);
				if (strExt == U(".json"))
				{
					strFile = StringUtil::GetAbsolutePath(strDir, strFile);
					ParseTileset(strFile, pParentNode, nLod, matParent);
				}
				else if (!strFile.empty())
				{
					if (strExt == U(".pnts"))
					{
						//点云 
						m_isPnts = true;
					}
					pTreeNode = new GLTFTreeNode;
					pTreeNode->m_strFile = strFile;
					strFile = StringUtil::GetAbsolutePath(strDir, strFile);
					if (StringUtil::IsFileExist(strFile))
					{
						pTreeNode->m_bFileFind = true;
					}
					pTreeNode->m_boundSphere = sphere;
					pTreeNode->m_strParentDir = strDir;
					pTreeNode->m_nLod = nLod;
					pTreeNode->m_RootMat = matParent;

					if (value.HasMember("geometricError"))
					{
						pTreeNode->m_dGeometryError = value["geometricError"].GetDouble();
					}

					m_nTotalNum++;
					m_nLodNum = MAX(m_nLodNum, nLod);

					if (pParentNode)
					{
						pParentNode->m_vecChildNode.push_back(pTreeNode);
						pTreeNode->m_pParentNode = pParentNode;
					}
				}
				else
				{
					pTreeNode = pParentNode;
				}
			}
		}

		if (value.HasMember("children"))
		{
			rapidjson::Value& children = value["children"];
			if (children.IsArray())
			{
				int nChildren = children.Size();
				for (int i = 0; i < nChildren; i++)
				{
					rapidjson::Value& child = children[i];
					if (pTreeNode == nullptr)
					{
						ParseInnerRoot(child, strDir, pTreeNode, nLod + 1, matParent);
					}
					else
					{
						ParseChildren(child, strDir, pTreeNode, nLod + 1, matParent);
					}
				}
			}
		}

		if (pTreeNode && pParentNode == nullptr)
		{
			m_vecGLTFNodes.push_back(pTreeNode);
		}
		return true;
	}

	bool ThreeDTilesParser::ParseGLTF_V1(unsigned char*& pBuffer, unsigned int jsonLength, std::wstring strOutputDir, Point3D& pntCenter)
	{
		std::wstring jsonDir = StringUtil::GetDir(strOutputDir);
		std::wstring jsonFile = StringUtil::GetAbsolutePath(jsonDir, BaseUtils::CreateGuid() + U(".json"));

		if (!StringUtil::IsFileExist(jsonDir))
		{
			FileUtil::MkDirEx(jsonDir);
		}

		std::string strTemp = StringUtil::UNICODE_to_UTF8(jsonFile);
		strTemp = StringUtil::UTF8_to_ANSI(strTemp);

		// 解析gltf的json
		unsigned char* pJsonBuffer = new unsigned char[jsonLength];
		memcpy(pJsonBuffer, pBuffer, jsonLength);

		FILE* file = fopen(strTemp.c_str(), "wb");

		fwrite(pJsonBuffer, sizeof(unsigned char), jsonLength, file);
		fclose(file);
		delete[] pJsonBuffer;
		pJsonBuffer = nullptr;

		std::ifstream ifs(strTemp.c_str());
		rapidjson::IStreamWrapper isw(ifs);

		rapidjson::Document doc;
		doc.ParseStream(isw);

		if (doc.HasParseError())
		{
			rapidjson::ParseErrorCode code = doc.GetParseError();
		}

		if (doc.HasMember("extensions"))
		{
			rapidjson::Value & extensions = doc["extensions"];
			if (extensions.HasMember("CESIUM_RTC"))
			{
				rapidjson::Value & rtc = extensions["CESIUM_RTC"];
				if (rtc.HasMember("center"))
				{
					rapidjson::Value & center = rtc["center"];
					pntCenter.x = center[0].GetDouble();
					pntCenter.y = center[1].GetDouble();
					pntCenter.z = center[2].GetDouble();
				}
			}
		}

		ParseScene_V1(doc);

		ParseMesh_V1(doc);

		ParseAccessors_V1(doc);

		ParseBufferViews_V1(doc);

		ParseMaterials_V1(doc);

		ParseTextures_V1(doc);

		ParseImages_V1(doc);

		ParseBuffers_V1(doc);

		ParseExtensions_V1(doc, pntCenter);

		ifs.close();
		remove(strTemp.c_str());

		return true;
	}

	bool ThreeDTilesParser::ParseScene_V1(rapidjson::Document& doc)
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
			ParseNode_V1(doc, nodeName);
		}

		return true;
	}

	bool ThreeDTilesParser::ParseNode_V1(rapidjson::Document& doc, const char* nodeName)
	{
		rapidjson::Value& nodes = doc["nodes"];
		if (nodes.Empty())
			return false;

		Matrix4d matGeode = Matrix4d::IDENTITY;
		ProcessGLTFNode_V1(nodes, nodeName, matGeode, U("Root"));

		return true;
	}

	void ThreeDTilesParser::ProcessGLTFNode_V1(rapidjson::Value& nodes, const char* nodeName, Matrix4d& matGeode, std::wstring parentName)
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
					m_nAxisUpType = AxisUpType::Z_UP;
				}
				else if (strName.find(U("Y_UP")) < nNameLength)
				{
					m_nAxisUpType = AxisUpType::Y_UP;
				}
				else
				{
					m_nAxisUpType = AxisUpType::X_UP;
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
				if (m_nAxisUpType == Y_UP)
				{
					matR = Matrix4d(
						1, 0, 0, 0,
						0, 0, 1, 0,
						0, -1, 0, 0,
						0, 0, 0, 1);
				}
				m_mapMeshTOLocalView_V1[strMeshName] = matR * tempMat;

				std::map<std::wstring, std::vector<std::wstring> >::iterator it = m_mapMeshSet_V1.find(parentName);
				if (it == m_mapMeshSet_V1.end())
				{
					m_mapMeshSet_V1[parentName] = std::vector<std::wstring>();
				}

				m_mapMeshSet_V1[parentName].push_back(strMeshName);
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

				ProcessGLTFNode_V1(nodes, childNodeName, tempMat, strNodeName);
			}
		}
	}

	bool ThreeDTilesParser::ParseMesh_V1(rapidjson::Document& doc)
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
				m_mapMeshs_V1[strMeshName] = mesh;
			}

			it++;
		}

		return true;
	}

	bool ThreeDTilesParser::ParseAccessors_V1(rapidjson::Document& doc)
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
			m_mapAccessors_V1[strAccessorName] = gltfAccessor;

			it++;
		}

		return true;
	}

	bool ThreeDTilesParser::ParseBufferViews_V1(rapidjson::Document& doc)
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

			m_mapBufferViews_V1[strBufferViewName] = gltfBufferView;

			it++;
		}

		return true;
	}

	bool ThreeDTilesParser::ParseMaterials_V1(rapidjson::Document& doc)
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

					m_mapMaterials_V1[strMaterialName] = gltfMaterial;
				}

				it++;
			}
		}

		return true;
	}

	bool ThreeDTilesParser::ParseTextures_V1(rapidjson::Document& doc)
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
					m_mapTextures_V1[strTextureName] = gltfTexture;
				}

				it++;
			}
		}

		return true;
	}

	bool ThreeDTilesParser::ParseImages_V1(rapidjson::Document& doc)
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

							m_mapImages_V1[strImageName] = gltfImage;
						}
					}
				}

				it++;
			}
		}
		return true;
	}

	bool ThreeDTilesParser::ParseBuffers_V1(rapidjson::Document& doc)
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

			m_mapBuffers_V1[strBufferName] = gltfBuffer;

			it++;
		}

		return true;
	}

	bool ThreeDTilesParser::ParseExtensions_V1(rapidjson::Document& doc, Point3D& center)
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
			center.x = jCenter[0].GetDouble();
			center.y = jCenter[1].GetDouble();
			center.z = jCenter[2].GetDouble();
		}

		return true;
	}

	void * ThreeDTilesParser::GetAttributeData_V1(unsigned char *& buffer, std::wstring & strName, int & nDim)
	{
		if (strName.length() == 0)
		{
			return nullptr;
		}

		GLTFAccessor & gltfAccessor = m_mapAccessors_V1[strName];
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


		GLTFBufferView & gltfBufferView = m_mapBufferViews_V1[gltfAccessor.m_strBufferViewName];

		// 获取buffer
		unsigned int nBufferSize = nCount * nConWidth * nByteWidth;
		unsigned char* pBuffer = new unsigned char[nBufferSize];
		memcpy(pBuffer, buffer + gltfBufferView.m_nByteOffset + nByteOffset, nBufferSize);

		unsigned int nDataCount = nBufferSize / nByteWidth;

		unsigned int* pUIntData = nullptr;
		unsigned short* pUshortData = nullptr;
		float* pFloatData = nullptr;

		if (nConWidth == 1)
		{
			if (nByteWidth == 4)
			{
				pUIntData = new unsigned int[nDataCount];
			}
			else
			{
				pUshortData = new unsigned short[nDataCount];
			}
		}
		else
		{
			pFloatData = new float[nDataCount];
		}
		MemoryStream stream;
		stream.Init(pBuffer, nBufferSize);

		for (unsigned int j = 0; j < nDataCount; j++)
		{
			if (pUIntData)
			{
				stream >> pUIntData[j];
			}
			else if (pFloatData)
			{
				stream >> pFloatData[j];
			}
			else if (pUshortData)
			{
				stream >> pUshortData[j];
			}

		}
		delete[] pBuffer;

		stream.Close();

		return (pUshortData == nullptr ? ((pUIntData == nullptr) ? (void*)pFloatData : (void*)pUIntData) : (void*)pUshortData);
	}

	bool ThreeDTilesParser::ParseGLTF(GLTFTreeNode* pNode, unsigned char*& buffer, unsigned int jsonLength, std::wstring strSrcFile, Point3D& ptTileCenter)
	{
		std::wstring jsonDir = StringUtil::GetDir(strSrcFile);
		std::wstring strGUID = BaseUtils::CreateGuid();
		std::wstring jsonFile = StringUtil::GetAbsolutePath(jsonDir, strGUID + U(".json"));
		if (!StringUtil::IsFileExist(jsonDir))
		{
			FileUtil::MkDirEx(jsonDir);
		}

		std::string strTemp = StringUtil::UNICODE_to_UTF8(jsonFile);
		strTemp = StringUtil::UTF8_to_ANSI(strTemp);
		//解析gltf的json
		unsigned char* jsonBuffer = new unsigned char[jsonLength];
		memcpy(jsonBuffer, buffer, jsonLength);

		FILE* file = fopen(strTemp.c_str(), "wb");
		fwrite(jsonBuffer, sizeof(unsigned char), jsonLength, file);
		fclose(file);
		delete[] jsonBuffer;
		jsonBuffer = nullptr;

		std::ifstream ifs(strTemp.c_str());
		rapidjson::IStreamWrapper isw(ifs);

		rapidjson::Document doc;
		doc.ParseStream(isw);

		if (doc.HasParseError())
		{
			rapidjson::ParseErrorCode code = doc.GetParseError();
		}

		if (doc.HasMember("extensions"))
		{
			rapidjson::Value & extensions = doc["extensions"];
			if (extensions.HasMember("CESIUM_RTC"))
			{
				rapidjson::Value & rtc = extensions["CESIUM_RTC"];
				if (rtc.HasMember("center"))
				{
					rapidjson::Value & center = rtc["center"];
					ptTileCenter.x = center[0].GetDouble();
					ptTileCenter.y = center[1].GetDouble();
					ptTileCenter.z = center[2].GetDouble();
				}
			}
		}

		ParseNode(doc);

		ParseMesh(doc);

		ParseAccessors(doc);

		ParseBufferViews(doc);

		ParseMaterials(doc);

		ParseTextures(doc);

		ParseImages(doc);

		ParseBuffers(doc);

		//处理纹理外挂路径，如果非纹理外挂，会判断返回
		std::wstring strImageDir = StringUtil::GetDir(StringUtil::GetAbsolutePath(pNode->m_strParentDir, pNode->m_strFile));
		for (int i = 0; i < m_mapImages.size(); i++)
		{
			DealGLTFFilePath(m_mapImages[i], strImageDir);
		}

		std::wstring strKey = U("data:application/octet-stream;base64,");
		for (int i = 0; i < m_mapBuffers.size(); i++)
		{
			std::wstring strFile = m_mapBuffers[i].m_strFile;
			if (strFile == U(""))
			{
				if (m_mapBuffers[i].m_nByteLength > m_nBufferSize)
				{
					ifs.close();
					remove(strTemp.c_str());
					return false;
				}

				unsigned char* tempBuffer = buffer + jsonLength + 8;
				m_mapBuffers[i].m_pBuffer = tempBuffer;
				m_mapBuffers[i].m_bManageBuffer = false;
			}
            else if (strFile.find(strKey) < strFile.length())
			{	
				strFile = strFile.substr(strKey.length());
				m_mapBuffers[i].m_strEncodeBuffer = strFile;
				m_mapBuffers[i].m_strFile = U("");
				m_mapBuffers[i].m_strEncodeMode = U("base64");

				//做Base64解压						
				std::wstring strDecode = BaseUtils::Base64Decode(m_mapBuffers[i].m_strEncodeBuffer, m_mapBuffers[i].m_strEncodeBuffer.length());
				std::string strBuffer = StringUtil::UNICODE_to_UTF8(strDecode);

				const char* bufferOut = strBuffer.c_str();
				unsigned char* pBuffer = new unsigned char[strBuffer.size()];
				memcpy(pBuffer, bufferOut, strBuffer.size());

				m_mapBuffers[i].m_pBuffer = pBuffer;
				m_mapBuffers[i].m_bManageBuffer = true;
			}
			else
			{
				std::wstring strBufferFile = strImageDir + m_mapBuffers[i].m_strFile;
				std::string strBuffer = StringUtil::UNICODE_to_UTF8(strBufferFile);

				unsigned int size = m_mapBuffers[i].m_nByteLength;
				unsigned char* extendBuffer = new unsigned char[size];
				FILE* bufferFile = fopen(strBuffer.c_str(), "rb");

				unsigned int bytesRead = fread(extendBuffer, sizeof(unsigned char), size, bufferFile);
				fclose(bufferFile);

				m_mapBuffers[i].m_pBuffer = extendBuffer;
				m_mapBuffers[i].m_bManageBuffer = true;
			}
		}

		ifs.close();
		remove(strTemp.c_str());

		if (m_mapImages.size() > 0)
		{
			ProcessTextureCompressType(m_mapImages[0]);
		}

		return true;
	}

	bool ThreeDTilesParser::ParseNode(rapidjson::Document & doc)
	{
		rapidjson::Value& scenes = doc["scenes"];
		rapidjson::Value& nodes = scenes[0]["nodes"];
		for (int i = 0; i < nodes.Size(); i++)
		{
			Matrix4d matGeode = Matrix4d::IDENTITY;
			int k = nodes[i].GetInt();
			ProcessGLTFNode(doc["nodes"], k, matGeode, 0);
		}
		return true;
	}

	void ThreeDTilesParser::ProcessGLTFNode(rapidjson::Value& value, int index, Matrix4d& matGeode, unsigned int parentIndex)
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
					m_nAxisUpType = AxisUpType::Z_UP;
				}
				else if (strName == "Y_UP")
				{
					m_nAxisUpType = AxisUpType::Y_UP;
				}
				else
				{
					m_nAxisUpType = AxisUpType::X_UP;
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
			if (m_nAxisUpType == Y_UP)
			{
				matR = Matrix4d(
					1, 0, 0, 0,
					0, 0, 1, 0,
					0, -1, 0, 0,
					0, 0, 0, 1);
			}
			m_mapMeshToLocalView[meshIndex] = tempMat * matR;

			std::map<unsigned int, std::vector<unsigned int> >& mapMeshSet = m_mapMeshSet;
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
				ProcessGLTFNode(value, children[i].GetInt(), tempMat, index);
			}
		}
	}

	bool ThreeDTilesParser::ParseMesh(rapidjson::Document& doc)
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
				m_mapMeshs[i] = mesh;
			}
		}
		return true;
	}

	bool ThreeDTilesParser::ParseAccessors(rapidjson::Document& doc)
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

			m_mapAccessors[i] = gltfAccessor;
		}
		return true;
	}

	bool ThreeDTilesParser::ParseBufferViews(rapidjson::Document& doc)
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
			m_mapBufferViews[i] = gltfBufferView;
		}
		return true;
	}

	bool ThreeDTilesParser::ParseMaterials(rapidjson::Document& doc)
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
					m_mapMaterials[i].push_back(gltfMaterial);
				}
			}
		}
		return true;
	}

	bool ThreeDTilesParser::ParseTextures(rapidjson::Document& doc)
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
						m_mapTextures[i] = gltfTexture;
					}
				}
			}
		}
		return true;
	}

	bool ThreeDTilesParser::ParseImages(rapidjson::Document& doc)
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

									m_mapImages[i] = gltfImage;
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

						m_mapImages[i] = gltfImage;
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
						m_mapImages[i] = gltfImage;
					}
				}
			}
		}

		return true;
	}

	bool ThreeDTilesParser::ParseBuffers(rapidjson::Document& doc)
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
			m_mapBuffers[i] = gltfBuffer;
		}
		return true;
	}

	void ThreeDTilesParser::RecurComputerRelatMat(GLTFTreeNode* pNode)
	{
		pNode->m_RelatMat = pNode->m_RootMat * m_matGlobalMat.Invert();
		for (int i = 0; i < pNode->m_vecChildNode.size(); i++)
		{
			RecurComputerRelatMat(pNode->m_vecChildNode[i]);
		}
	}

	void ThreeDTilesParser::ProcessTextureCompressType(GLTFImage gltfImage)
	{
		std::wstring strImage;
		if (!gltfImage.m_strFileName.empty())
		{
			strImage = gltfImage.m_strFileName;
		}
		else if (!gltfImage.m_strFile.empty())
		{
			strImage = gltfImage.m_strFile;
		}
		else
		{
			strImage = gltfImage.m_strImageType;
		}

		int nLength = strImage.length();
		if (strImage.find(U("webp")) < nLength)
		{
			m_textureCompressType = TextureCompressType::TC_WEBP;
		}
		else if (strImage.find(U("crn")) < nLength ||
			strImage.find(U("CRN")) < nLength)
		{
			m_textureCompressType = TextureCompressType::TC_CRN;
		}
		else
		{
			m_textureCompressType = TextureCompressType::TC_DXT5;
		}
	}

	void ThreeDTilesParser::DealGLTFFilePath(GLTFImage &gltfImage, std::wstring strFilePath)
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

	Vector3d ThreeDTilesParser::GetDecodeNormal(unsigned short normalX, unsigned short normalY)
	{
		int nNormalQuantizationBits = 8;
		float normalScale = float((1 << (nNormalQuantizationBits - 1)) - 1);
		Vector3d vecNormal;
		vecNormal.x = normalX / normalScale;
		vecNormal.y = normalY / normalScale;
		vecNormal.z = 1.0 - normalX - normalY;
		vecNormal.Normalize();
		return vecNormal;
	}
}
