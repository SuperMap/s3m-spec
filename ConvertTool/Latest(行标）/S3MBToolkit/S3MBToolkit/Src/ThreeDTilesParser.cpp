#include "ThreeDTilesParser.h"
#include "GLTFParser.h"
#include "ImageOperator.h"
#include "MathEngine.h"
#include "Shell.h"
#include "S3MBCommon.h"
#include "S3MBConfig.h"
#include "S3MBUtils.h"
#include "S3MBWriter.h"
#include "Utils/Utils.h"

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

	GLTFTileInfos_1* ThreeDTilesParser::ParseGLTF_V1(unsigned char*& pBuffer, unsigned int jsonLength, std::wstring strOutputDir)
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

		GLTFTileInfos_1* pTileInfos = GLTFParser::ParseGLTFTileInfos_1(jsonFile, pBuffer + jsonLength, m_nAxisUpType);
		remove(strTemp.c_str());
		return pTileInfos;
	}

	GLTFTileInfos_2* ThreeDTilesParser::ParseGLTF(GLTFTreeNode* pNode, unsigned char*& buffer, unsigned int jsonLength, std::wstring strSrcFile)
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

		std::wstring strImageDir = StringUtil::GetDir(StringUtil::GetAbsolutePath(pNode->m_strParentDir, pNode->m_strFile));
		GLTFTileInfos_2* pTileInfos = GLTFParser::ParseGLTFTileInfos_2(jsonFile, buffer + jsonLength, m_nBufferSize, strImageDir, m_nAxisUpType);
		remove(strTemp.c_str());

		if (pTileInfos != nullptr && pTileInfos->m_mapImages.size() > 0)
		{
			ProcessTextureCompressType(pTileInfos->m_mapImages[0]);
		}

		return pTileInfos;
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
