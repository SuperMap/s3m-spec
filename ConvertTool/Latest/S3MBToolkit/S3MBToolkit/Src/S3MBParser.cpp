#include "S3MBParser.h"
#include "B3DMWriter.h"
#include "MathEngine.h"
#include "S3MBConfig.h"
#include "S3MBReader.h"
#include "Utils.h"

#include "document.h"
#include "istreamwrapper.h"

#include <fstream>

namespace S3MB
{
	S3MBParser::S3MBParser()
	{
		m_nThreadNum = 8;
		m_dDis = 0.0;
		m_bEllip = false;
	}

	S3MBParser::~S3MBParser()
	{

	}

	void S3MBParser::SetThreadNum(int nNum)
	{
		m_nThreadNum = nNum;
	}

	bool S3MBParser::ParseSCP(const std::wstring& strPath)
	{
		S3MBConfig config;
		if (!config.ReadFile(strPath))
		{
			return false;
		}

		if (config.GetDataType() == SDT_InstanceModel)
		{
			return false;
		}

		std::vector<std::wstring> vecRootName;
		std::vector<BoundingBox> vecBBox;
		std::vector<OrientedBoundingBox> vecOBB;
        config.GetTiles(vecRootName, vecOBB);

		m_vecTilePath = vecRootName;
		if (!vecBBox.empty())
		{
			m_vecTileBox = vecBBox;
		}
		for (unsigned int i = 0; i < vecOBB.size(); i++)
		{
			m_vecTileBox.push_back(BoundingBoxUtils::OBBToAABB(vecOBB[i]));
		}

		m_pos = config.GetPosition();

		Rect2D bounds = config.GetBounds();
		Vector3d vMax(bounds.right, bounds.top, 1);
		Vector3d vMin(bounds.left, bounds.bottom, 0);
		m_box = BoundingBox(vMin, vMax);

		m_bEllip = config.GetExtension(U("globeType")) == U("Ellipsoid_WGS84");

		BoundingBox box;
		for (int i = 0; i < m_vecTileBox.size(); i++)
		{
			box.Merge(m_vecTileBox[i]);
			m_vecTileSphere.push_back(BoundingSphere(m_vecTileBox[i]));
		}

		if (m_bEllip)
		{
			BoxToSphere(box, m_sphere);
			m_mat = MathEngine::EllipsoidalENUToWGS84(Vector3d(m_pos.x, m_pos.y, m_pos.z));
		}
		else
		{
			CalPrjCoordMatrix(m_pos, box, m_matLocalView, !m_bEllip);
		}

		return true;
	}

	bool S3MBParser::GenerateTilesetAndB3DM(const std::wstring& strPath, const std::wstring& strOutputDir)
	{
		int nSize = m_vecTilePath.size();
		if (nSize == 0)
		{
			return false;
		}

		std::wstring strInputDir = StringUtil::GetDir(strPath);
		std::vector<std::wstring> vecFilePath;
		for (int i = 0; i < nSize; i++)
		{
			std::wstring strTilePath = m_vecTilePath[i];
			std::wstring strTileDir = StringUtil::GetDir(strTilePath);
			std::vector<std::wstring> vecFile;
			std::vector<std::wstring> vecFileExt;
			vecFileExt.push_back(U(".s3mb"));

			S3MBReader reader;
			reader.ReadFile(StringUtil::GetAbsolutePath(strInputDir, strTilePath));
			RenderOperationGroup* pGroup = reader.GetRenderOperationGroup();
			if (pGroup == nullptr)
			{
				reader.Clear();
				continue;
			}

			std::wstring strInputPath = StringUtil::GetAbsolutePath(strInputDir, strTileDir);
			std::wstring strOutputPath = StringUtil::GetAbsolutePath(strOutputDir, strTileDir);
			GetFileList(vecFile, strInputPath, vecFileExt, true);
			vecFilePath.insert(vecFilePath.end(), vecFile.begin(), vecFile.end());
			vecFile.clear();

			if (!StringUtil::IsFileExist(strOutputPath))
			{
				FileUtil::MkDirEx(strOutputPath);
			}

			RenderOperationNode* pNode = pGroup->GetChild(0);
			if (pNode != nullptr)
			{
				std::wstring strJsonFile = StringUtil::GetAbsolutePath(strOutputDir, strTilePath);
				strJsonFile = StringUtil::ChangeExt(strJsonFile, U(".json"));

				float fDis = 0.0;
				RangeMode rangeMode = Pixel_Size_OnScreen;

				if (StringUtil::CompareNoCase(pNode->ClassName(), U("PagedLOD")))
				{
					RenderOperationPagedLOD* pLOD = (RenderOperationPagedLOD*)pNode;
					rangeMode = pLOD->GetRangeMode();
					fDis = pLOD->GetRanges()[0].second;
				}

				if (fDis > m_dDis || fDis == 0)
				{
					m_dDis = fDis;
				}

				BoundingSphere sphere = pGroup->GetBoundingSphere();
				double dError = GetGeometryError(sphere, fDis, rangeMode);

				std::wstring strFileName = StringUtil::GetName(strTilePath);
				strFileName = StringUtil::ChangeExt(strFileName, U(".b3dm"));

				rapidjson::StringBuffer buffer;
				rapidjson::Writer<rapidjson::StringBuffer> jsonWriter(buffer);
				jsonWriter.StartObject();

				WriteAsset(&jsonWriter, "1.0");
				WriteGeometricError(&jsonWriter, dError);

				jsonWriter.Key("root");
				jsonWriter.StartObject();

				WriteBoundingVolume(&jsonWriter, sphere);

				RenderOperationPagedLOD* pLOD = (RenderOperationPagedLOD*)pNode;
				jsonWriter.Key("children");
				jsonWriter.StartArray();

				std::vector<std::wstring> vecChildTilePath;
				for (int iChild = 0; iChild < pGroup->GetNumChildren(); iChild++)
				{
					RenderOperationNode* pNode = pGroup->GetChild(iChild);
					if (pNode != nullptr && StringUtil::CompareNoCase(pNode->ClassName(), U("PagedLOD")))
					{
						RenderOperationPagedLOD* pLOD = (RenderOperationPagedLOD*)pNode;
						std::vector<std::wstring> vecFileName = pLOD->GetFileNames();
						for (int iName = 0; iName < vecFileName.size(); iName++)
						{
							vecChildTilePath.push_back(StringUtil::GetAbsolutePath(strInputPath, vecFileName[iName]));
						}
					}
				}
				ProcessPagedMatchInfo(&jsonWriter, vecChildTilePath, dError, m_mat);
				jsonWriter.EndArray();

				std::string strTemp = StringUtil::UNICODE_to_UTF8(strFileName);
				WriteContent(&jsonWriter, strTemp.c_str());
				WriteGeometricError(&jsonWriter, dError);

				WriteRefine(&jsonWriter, "REPLACE");

				jsonWriter.EndObject();

				jsonWriter.EndObject();

				WriteFile(strJsonFile, buffer.GetString());
			}
			
			reader.Clear();
		}
		nSize = vecFilePath.size();
		std::vector<std::vector<std::wstring> > vecTotalInput;
		for (int i = 0; i < m_nThreadNum; i++)
		{
			vecTotalInput.push_back(std::vector<std::wstring>());
		}
		int nAve = nSize / m_nThreadNum;
		for (unsigned int i = 0; i < nAve; i++)
		{
			for (unsigned int j = 0; j < m_nThreadNum; j++)
			{
				vecTotalInput[j].push_back(vecFilePath[i * m_nThreadNum + j]);
			}
		}

		for (unsigned int i = 0; i < nSize - nAve * m_nThreadNum; i++)
		{
			vecTotalInput[i].push_back(vecFilePath[nAve * m_nThreadNum + i]);
		}

		unsigned int nTotalSize = vecTotalInput.size();
		for (unsigned int i = 0; i < nTotalSize; i++)
		{
			B3DMWriter* pWriter = new B3DMWriter();
			pWriter->SetIsEllipsoid(m_bEllip);
			pWriter->SetLocalView(m_matLocalView);
			pWriter->SetViewSphere(m_matViewSphere);
			pWriter->SetArrInput(vecTotalInput[i]);
			pWriter->SetInput(strInputDir);
			pWriter->SetOutput(strOutputDir);
			pWriter->Write();
			delete pWriter;
			pWriter = nullptr;
		}

		return true;
	}

	void S3MBParser::GenerateTileset(const std::wstring& strPath, const std::wstring& strOutputDir)
	{
		std::wstring strOutputPath = StringUtil::GetAbsolutePath(strOutputDir, U("./tileset.json"));

		// 生成Tileset
		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		writer.StartObject();

		// 生成asset 
		WriteAsset(&writer, "1.0");
		WriteProperties(&writer, m_box);
		WriteGeometricError(&writer, GetGeometryError(m_sphere, m_dDis));
		WriteRoot(&writer);
		writer.EndObject();

		std::string bufferStr = buffer.GetString();

		rapidjson::Document doc;
		doc.Parse(bufferStr.c_str());

		buffer = rapidjson::StringBuffer();
		rapidjson::PrettyWriter<rapidjson::StringBuffer> prettyWriter(buffer);
		doc.Accept(prettyWriter);

		std::string strTemp = StringUtil::UNICODE_to_UTF8(strOutputPath);
		strTemp = StringUtil::UTF8_to_ANSI(strTemp);
		std::ofstream stream(strTemp.c_str());
		if (stream.is_open()) {
			stream << buffer.GetString() << std::endl;
			stream.close();
		}
	}

	void S3MBParser::BoxToSphere(BoundingBox& box, BoundingSphere& sphere, Matrix4d mat)
	{
		if (m_bEllip)
		{
			Vector3d vMin, vMax;
			const Vector3d* pCorner = box.GetCorners();
			for (int i = 0; i < 8; i++)
			{
				Vector3d vPos(pCorner[i]);
				vPos = vPos.MultiplyMatrix(m_matViewSphere);
				vPos = MathEngine::CartesianToSphericalD(vPos.x, vPos.y, vPos.z);
				vPos.x *= RTOD;
				vPos.y *= RTOD;
				vPos.z -= GLOBAL_RADIUS;

				vPos = MathEngine::SphericalToCartesian(vPos.x*DTOR, vPos.y*DTOR, vPos.z + GLOBAL_RADIUS);
				vPos = vPos.MultiplyMatrix(m_matLocalView.Invert());

				if (i == 0)
				{
					vMin = vPos;
					vMax = vPos;
					continue;
				}
				vMax.x = vMax.x < vPos.x ? vPos.x : vMax.x;
				vMax.y = vMax.y < vPos.y ? vPos.y : vMax.y;
				vMax.z = vMax.z < vPos.z ? vPos.z : vMax.z;
				vMin.x = vMin.x > vPos.x ? vPos.x : vMin.x;
				vMin.y = vMin.y > vPos.y ? vPos.y : vMin.y;
				vMin.z = vMin.z > vPos.z ? vPos.z : vMin.z;
			}

			box.SetMin(vMin);
			box.SetMax(vMax);
		}

		sphere.m_center = box.GetCenter();
		sphere.m_radius = (box.GetMax() - box.GetMin()).Length() / 2;
	}

	void S3MBParser::CalPrjCoordMatrix(Point3D pos, BoundingBox& box, Matrix4d& mat, bool bNEU)
	{
		if (bNEU)
		{
			Vector3d vPos(pos.x, pos.y, pos.z);
			mat = MathEngine::EllipsoidalENUToWGS84(vPos);
			vPos = MathEngine::SphericalToCartesian(pos.x*DTOR, pos.y*DTOR, pos.z + GLOBAL_RADIUS);
			m_matViewSphere = MatrixUtils::RotationXYZ(-pos.y * DTOR, pos.x * DTOR, 0) * MatrixUtils::Translation(vPos.x, vPos.y, vPos.z);
			m_mat = mat;
		}
		else
		{
			Matrix4d matRot = MatrixUtils::RotationXYZ(0, -90 * DTOR, -90 * DTOR);
			Vector3d vPos = MathEngine::SphericalToCartesian(pos.x*DTOR, pos.y*DTOR, pos.z + GLOBAL_RADIUS);
			mat = MatrixUtils::RotationXYZ(-pos.y * DTOR, pos.x * DTOR, 0) * MatrixUtils::Translation(vPos.x, vPos.y, vPos.z);
			m_mat = mat * matRot;
		}

		BoxToSphere(box, m_sphere);

		double dt = m_sphere.m_radius / sqrt(2);
		Vector3d vCenter = m_sphere.m_center;

		double dMinX = vCenter.x - dt;
		double dMinY = vCenter.y - dt;
		double dMinZ = vCenter.z - dt;

		double dMaxX = vCenter.x + dt;
		double dMaxY = vCenter.y + dt;
		double dMaxZ = vCenter.z + dt;

		Vector3d vMax = Vector3d(dMaxX, dMaxY, dMaxZ).MultiplyMatrix(mat);
		Vector3d vMin = Vector3d(dMinX, dMinY, dMinZ).MultiplyMatrix(mat);

		if (bNEU)
		{
			vMax = MathEngine::CartesianToWGS84(vMax.x, vMax.y, vMax.z);
			vMin = MathEngine::CartesianToWGS84(vMin.x, vMin.y, vMin.z);
		}
		else
		{
			vMax = MathEngine::CartesianToSphericalD(vMax.x, vMax.y, vMax.z);
			vMin = MathEngine::CartesianToSphericalD(vMin.x, vMin.y, vMin.z);

			vMax.z -= GLOBAL_RADIUS;
			vMin.z -= GLOBAL_RADIUS;
		}

		m_box = BoundingBox(vMin, vMax);
	}

	void S3MBParser::GetFileList(std::vector<std::wstring>& vecFile, const std::wstring& strPath, const std::vector<std::wstring>& vecFileExt, bool bDir)
	{
		std::vector<std::wstring> vecFileName;
        FileUtils::ListFiles(vecFileName, strPath);

		for (int i = 0; i < vecFileName.size(); i++)
		{
			std::wstring strFileName = vecFileName[i];
			std::wstring strFilePath = StringUtil::GetAbsolutePath(strPath, strFileName);
			if (strFileName.find(U(".")) != std::wstring::npos)
			{
				std::wstring strExt = StringUtil::GetExt(strFileName);
				for (int j = 0; j < vecFileExt.size(); j++)
				{
					if (StringUtil::CompareNoCase(strExt, vecFileExt[j]))
					{
						vecFile.push_back((bDir ? strFilePath : strFileName));
					}
				}
			}
			else
			{
				GetFileList(vecFile, strFilePath, vecFileExt, bDir);
			}
		}
	}

	double S3MBParser::GetGeometryError(const BoundingSphere& sphere, double dDis, RangeMode nMode)
	{
		double dError = dDis;
		switch (nMode)
		{
		case Distance_From_EyePoint:
			if (IS0(dDis))
			{
				dDis = 10000;
			}
			dError = dDis;
			break;
		case Pixel_Size_OnScreen:
			if (IS0(dDis))
			{
				dDis = 0.1;
			}
			dError = sphere.m_radius * 16 / dDis;
			break;
		default:
			break;
		}
		return dError;
	}

	void S3MBParser::WriteAsset(WriterPtr pWriter, const std::string& strVersion)
	{
		pWriter->Key("asset");
		pWriter->StartObject();
		pWriter->Key("version");
		pWriter->String(strVersion.c_str());
		pWriter->EndObject();
	}

	void S3MBParser::WriteProperties(WriterPtr pWriter, const BoundingBox& box)
	{
		Vector3d max = box.GetMax();
		Vector3d min = box.GetMin();

		pWriter->Key("properties");
		pWriter->StartObject();
		pWriter->Key("region");
		pWriter->StartObject();
		pWriter->Key("west");
		pWriter->Double(min.x);
		pWriter->Key("south");
		pWriter->Double(min.y);
		pWriter->Key("east");
		pWriter->Double(max.x);
		pWriter->Key("north");
		pWriter->Double(max.y);
		pWriter->Key("minHeight");
		pWriter->Double(min.z);
		pWriter->Key("maxHeight");
		pWriter->Double(max.z);
		pWriter->EndObject();

		pWriter->EndObject();
	}

	void S3MBParser::WriteGeometricError(WriterPtr pWriter, double dError)
	{
		pWriter->Key("geometricError");
		pWriter->Double(dError);
	}

	void S3MBParser::WriteRoot(WriterPtr pWriter)
	{
		pWriter->Key("root");
		pWriter->StartObject();
		WriteBoundingVolume(pWriter, m_sphere);
		
		pWriter->Key("children");
		pWriter->StartArray();
		for (unsigned int i = 0; i < m_vecTileSphere.size(); i++)
		{
			std::wstring strFileName = m_vecTilePath[i];
			strFileName = StringUtil::ChangeExt(strFileName, U(".json"));
			std::string strURL = StringUtil::UNICODE_to_UTF8(strFileName);
			strURL = StringUtil::UTF8_to_ANSI(strURL);
			WriteChild(pWriter, m_vecTileSphere[i], strURL.c_str(), GetGeometryError(m_vecTileSphere[i], m_dDis));
		}
		pWriter->EndArray();

		WriteGeometricError(pWriter, GetGeometryError(m_sphere, m_dDis));
		WriteRefine(pWriter, "REPLACE");
		WriteTransform(pWriter, m_mat);
		pWriter->EndObject();
	}

	void S3MBParser::WriteBoundingVolume(WriterPtr pWriter, BoundingSphere& sphere)
	{
		pWriter->Key("boundingVolume");
		pWriter->StartObject();
		pWriter->Key("sphere");
		pWriter->StartArray();
		pWriter->Double(sphere.m_center.x);
		pWriter->Double(sphere.m_center.y);
		pWriter->Double(sphere.m_center.z);
		pWriter->Double(sphere.m_radius);
		pWriter->EndArray();
		pWriter->EndObject();
	}

	void S3MBParser::WriteChild(WriterPtr pWriter, BoundingSphere& sphere, const std::string& strURL, double dError)
	{
		pWriter->StartObject();
		WriteBoundingVolume(pWriter, sphere);
		WriteContent(pWriter, strURL);
		WriteGeometricError(pWriter, dError);
		pWriter->EndObject();
	}

	void S3MBParser::WriteContent(WriterPtr pWriter, const std::string& strURL)
	{
		pWriter->Key("content");
		pWriter->StartObject();
		pWriter->Key("url");
		pWriter->String(strURL.c_str());
		pWriter->EndObject();
	}

	void S3MBParser::WriteRefine(WriterPtr pWriter, const std::string& strRefine)
	{
		pWriter->Key("refine");
		pWriter->String(strRefine.c_str());
	}

	void S3MBParser::WriteTransform(WriterPtr pWriter, const Matrix4d& mat)
	{
		pWriter->Key("transform");
		pWriter->StartArray();
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				pWriter->Double(mat[i][j]);
			}
		}
		pWriter->EndArray();
	}

	void S3MBParser::WriteFile(const std::wstring& strOutputPath, const std::string& strContent)
	{
		rapidjson::Document doc;
		doc.Parse(strContent.c_str());

		rapidjson::StringBuffer buffer;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
		doc.Accept(writer);

		std::string strTemp = StringUtil::UNICODE_to_UTF8(strOutputPath);
		strTemp = StringUtil::UTF8_to_ANSI(strTemp);
		std::ofstream file(strTemp.c_str());
		if (file.is_open()) {
			file << buffer.GetString() << std::endl;
			file.close();
		}
	}

	void S3MBParser::ProcessPagedMatchInfo(WriterPtr pWriter, std::vector<std::wstring>& vecTilePath, double geometryError, Matrix4d mat, const std::wstring& strExt)
	{
		for (unsigned int i = 0; i < vecTilePath.size(); i++)
		{
			S3MBReader reader;
			reader.ReadFile(vecTilePath[i]);
			std::wstring strTileDir = StringUtil::GetDir(vecTilePath[i]);
			RenderOperationGroup* pGroup = reader.GetRenderOperationGroup();
			if (pGroup == nullptr)
			{
				reader.Clear();
				continue;
			}

			std::wstring strFileName = StringUtil::GetName(vecTilePath[i]);
			strFileName = StringUtil::ChangeExt(strFileName, U(".b3dm"));
			std::string strTemp = StringUtil::UNICODE_to_UTF8(strFileName);

			BoundingSphere sphere = pGroup->GetBoundingSphere();
			RangeMode rangeMode = Pixel_Size_OnScreen;
			float fDis = 0.0;
			RenderOperationNode* pNode = pGroup->GetChild(0);
			if (pNode != nullptr && StringUtil::CompareNoCase(pNode->ClassName(), U("PagedLOD")))
			{
				RenderOperationPagedLOD* pLOD = (RenderOperationPagedLOD*)pNode;
				rangeMode = pLOD->GetRangeMode();
				fDis = pLOD->GetRanges()[0].second;
			}

			double dError = GetGeometryError(sphere, fDis, rangeMode);

			pWriter->StartObject();
			WriteBoundingVolume(pWriter, sphere);
			WriteContent(pWriter, strTemp.c_str());
			WriteGeometricError(pWriter, dError);

			std::vector<std::wstring> vecChildTilePath;
			for (int iChild = 0; iChild < pGroup->GetNumChildren(); iChild++)
			{
				RenderOperationNode* pNode = pGroup->GetChild(iChild);
				if (pNode != nullptr && StringUtil::CompareNoCase(pNode->ClassName(), U("PagedLOD")))
				{
					RenderOperationPagedLOD* pLOD = (RenderOperationPagedLOD*)pNode;
					std::vector<std::wstring> vecFileName = pLOD->GetFileNames();
					for (int iName = 0; iName < vecFileName.size(); iName++)
					{
						vecChildTilePath.push_back(StringUtil::GetAbsolutePath(strTileDir, vecFileName[iName]));
					}
				}
			}

			if (vecChildTilePath.size() > 0)
			{
				pWriter->Key("children");
				pWriter->StartArray();
				ProcessPagedMatchInfo(pWriter, vecChildTilePath, dError, mat, strExt);
				pWriter->EndArray();
			}

			pWriter->EndObject();

			reader.Clear();
		}
	}
}
