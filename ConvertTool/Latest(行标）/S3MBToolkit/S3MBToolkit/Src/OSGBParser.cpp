#include "OSGBParser.h"
#include "ImageOperator.h"
#include "S3MBCommon.h"
#include "S3MBUtils.h"
#include "Utils/Utils.h"

#include "tinyxml2.h"
#include <osg/BlendFunc>
#include <osg/Material>
#include <osg/MatrixTransform>
#include <osg/PagedLOD>
#include <osg/ref_ptr>
#include <osgDB/ReadFile>

#include <assert.h>

using namespace tinyxml2;

USE_OSGPLUGIN(jpeg)
USE_OSGPLUGIN(png)
USE_OSGPLUGIN(osg2)
USE_OSGPLUGIN(obj)
USE_SERIALIZER_WRAPPER_LIBRARY(osg)

namespace S3MB
{
	OSGBConfig::OSGBConfig()
	{
		m_fVersion = 1.0;
		m_nEpsgCode = 4326;
	}

	OSGBConfig::~OSGBConfig()
	{

	}

	bool OSGBConfig::LoadFromFile(const std::wstring& strPath)
	{
		const std::wstring mode = U("rb");
#ifdef WIN32
		FILE* pFile = _wfopen(strPath.c_str(), mode.c_str());
#else
        FILE* pFile = (FILE*)fopen((StringUtil::UnicodeToANSI(strPath)).c_str(), (StringUtil::UnicodeToANSI(mode)).c_str());
#endif
		if (pFile == nullptr)
		{
			return false;
		}

		XMLDocument doc;
		if (doc.LoadFile(pFile) != XML_SUCCESS)
		{
			fclose(pFile);
			return false;
		}

		XMLElement* pElement = nullptr;
		XMLElement* pRoot = doc.RootElement();
		if (pRoot != nullptr)
		{
			pElement = pRoot->FirstChildElement("sml:Version");
			if (pElement != nullptr)
			{
				m_fVersion = atof(pElement->GetText());
			}

			pElement = pRoot->FirstChildElement("sml:FileType");
			if (pElement != nullptr)
			{
				m_strFileType = StringUtil::UTF8_to_UNICODE(pElement->GetText());
			}

			pElement = pRoot->FirstChildElement("sml:Position");
			if (pElement != nullptr)
			{
				XMLElement* pPosComp = pElement->FirstChildElement("sml:X");
				if (pPosComp != nullptr)
				{
					m_pntPosition.x = strtod(pPosComp->GetText(), nullptr);
				}

				pPosComp = pElement->FirstChildElement("sml:Y");
				if (pPosComp != nullptr)
				{
					m_pntPosition.y = strtod(pPosComp->GetText(), nullptr);
				}

				pPosComp = pElement->FirstChildElement("sml:Z");
				if (pPosComp != nullptr)
				{
					m_pntPosition.z = strtod(pPosComp->GetText(), nullptr);
				}
			}
			
			pElement = pRoot->FirstChildElement("sml:OSGFiles");
			if (pElement != nullptr)
			{
				XMLElement* pFiles = pElement->FirstChildElement("sml:Files");
				while (pFiles)
				{
					XMLElement* pFileName = pFiles->FirstChildElement("sml:FileName");
					if (pFileName != nullptr)
					{
						m_vecFileName.push_back(StringUtil::UTF8_to_UNICODE(pFileName->GetText()));
					}
					pFiles = pFiles->NextSiblingElement();
				}
			}

			pElement = pRoot->FirstChildElement("sml:CoordinateReferenceSystem");
			if (pElement != nullptr)
			{
				XMLElement* pEpsg = pElement->FirstChildElement("sml:EPSGCode");
				if (pEpsg != nullptr)
				{
					m_nEpsgCode = atoi(pEpsg->GetText());
				}
			}
		}

		return true;
	}

	float OSGBConfig::GetVersion() const
	{
		return m_fVersion;
	}

	const wstring& OSGBConfig::GetFileType() const
	{
		return m_strFileType;
	}

	const Point3D& OSGBConfig::GetPosition() const
	{
		return m_pntPosition;
	}

	const std::vector<std::wstring>& OSGBConfig::GetFileNames() const
	{
		return m_vecFileName;
	}

	int OSGBConfig::GetEpsgCode() const
	{
		return m_nEpsgCode;
	}

	//=================================================================================================================

	OSGBParser::OSGBParser()
	{
		m_pROGroup = nullptr;
	}

	OSGBParser::~OSGBParser()
	{
		Clear();
	}

	bool OSGBParser::ParseNodeFile(const wstring& strPath, const std::wstring& strOption)
	{
		Clear();

		if (!StringUtil::IsFileExist(strPath))
		{
			return false;
		}

		osg::ref_ptr<osg::Node> pNode = ReadNodeFile(strPath, strOption);
		if (pNode != nullptr)
		{
			m_pROGroup = new RenderOperationGroup;
			m_pROGroup->SetName(BaseUtils::CreateGuid());
			Matrix4d mat = Matrix4d::IDENTITY;
			std::wstring strNameID;
			std::vector<std::wstring> vecChildName;
			ProcessNode(pNode, m_pROGroup, m_pROGroup, mat, strNameID, strPath, vecChildName);
		}

		return true;
	}

	void OSGBParser::GetGroup(RenderOperationGroup*& pGroup, bool bSetNull)
	{
		pGroup = m_pROGroup;
		if (bSetNull)
		{
			m_pROGroup = nullptr;
		}
	}

	osg::ref_ptr<osg::Node> OSGBParser::ReadNodeFile(const wstring& strPath, const wstring& strOption)
	{
		osg::ref_ptr<osg::Node> pNode = nullptr;
		try
		{
			std::string fileName = StringUtil::UNICODE_to_UTF8(strPath);
			fileName = StringUtil::UTF8_to_ANSI(fileName);
			std::string opt = StringUtil::UNICODE_to_UTF8(strOption);
			opt = StringUtil::UTF8_to_ANSI(opt);
			osg::ref_ptr<osgDB::Options> pOption = new osgDB::Options(opt);
			pNode = osgDB::readNodeFile(fileName, pOption);
		}
		catch (const osg::ref_ptr<osgDB::InputException>& pException)
		{
			if (pNode != nullptr)
			{
				pNode.release();
				pNode = nullptr;
			}
		}

		return pNode;
	}

	void OSGBParser::ProcessNode(osg::Node* pNode, RenderOperationGroup* pGroup, RenderOperationGroup* pRootGroup,
		Matrix4d mat4, std::wstring strNameID, const std::wstring& strFilePath, std::vector<std::wstring>& vecLodName)
	{
		if (pGroup == nullptr || pRootGroup == nullptr || pNode == nullptr)
		{
			return;
		}

		std::wstring strClassName = StringUtil::UTF8_to_UNICODE(pNode->className());
		if (m_mapRootNodesMatrix.size() != 0 && !StringUtil::CompareNoCase(strClassName, U("MatrixTransform"))
			&& mat4 == Matrix4d::IDENTITY)
		{
			//子节点从根节点上取矩阵
			std::wstring strDir = StringUtil::GetDir(strFilePath);
			if (m_mapRootNodesMatrix.find(strDir) != m_mapRootNodesMatrix.end())
			{
				mat4 = m_mapRootNodesMatrix[strDir];
			}
		}

		if (pNode->asGroup())
		{
			if (StringUtil::CompareNoCase(strClassName, S3MB_SHELL_GROUP))
			{
				osg::ref_ptr<osg::Group> pOsgGroup = (osg::Group*)pNode;
				std::wstring strGroupName = StringUtil::UTF8_to_UNICODE(pOsgGroup->getName());
				if (!strGroupName.empty())
				{
					pGroup->SetName(strGroupName);
				}
				strNameID = strGroupName;

				unsigned int nChildCount = pOsgGroup->getNumChildren();
				for (unsigned int i = 0; i < nChildCount; i++)
				{
					ProcessNode(pOsgGroup->getChild(i), pGroup, pRootGroup, mat4, strNameID, strFilePath, vecLodName);
				}
			}
			else if (StringUtil::CompareNoCase(strClassName, S3MB_SHELL_PAGEDLOD))
			{
				osg::ref_ptr<osg::PagedLOD> pOsgPagedLOD = (osg::PagedLOD*)pNode;
				if ((pOsgPagedLOD->getNumChildren() == 0) && (pOsgPagedLOD->getNumFileNames() != 0))
				{
					//存储根节点矩阵
					std::wstring strDir = StringUtil::GetDir(strFilePath);
					m_mapRootNodesMatrix[strDir] = mat4;

					std::wstring strFileName = StringUtil::UTF8_to_UNICODE(pOsgPagedLOD->getFileName(0));
					std::wstring strNewPath = strDir + strFileName;

					osg::ref_ptr<osg::Node> pNewNode = nullptr;
					try
					{
						pNewNode = osgDB::readNodeFile(StringUtil::UNICODE_to_UTF8(strNewPath));
					}
					catch (const osg::ref_ptr<osgDB::InputException>& pException)
					{
						if (pNewNode != nullptr)
						{
							pNewNode.release();
							pNewNode = nullptr;
						}
					}

					ProcessNode(pNewNode, pGroup, pRootGroup, mat4, strFileName, strFilePath, vecLodName);
				}
				else
				{
					RenderOperationPagedLOD* pROPagedLOD = new RenderOperationPagedLOD;
					pROPagedLOD->SetParentNode(pGroup);
					pGroup->AddChild(pROPagedLOD);

					std::wstring strLodName = StringUtil::UTF8_to_UNICODE(pOsgPagedLOD->getName());
					pROPagedLOD->SetName(strLodName);
					strNameID = strLodName;
					// 子节点名
					unsigned int nChildCount = pOsgPagedLOD->getNumFileNames();
					for (unsigned int i = 0; i < nChildCount; i++)
					{
						std::wstring strFileName = StringUtil::UTF8_to_UNICODE(pOsgPagedLOD->getFileName(i));
						if (strFileName.empty())
						{
							continue;
						}

						pROPagedLOD->GetFileNames().push_back(strFileName);

						if (nChildCount > 2)
						{
							vecLodName.push_back(strFileName);
						}
					}

					// 包围球
					osg::Vec3d vec3d = pOsgPagedLOD->getCenter();
					double dRadius = pOsgPagedLOD->getRadius();
					Vector3d vecCenter;
					vecCenter.x = vec3d[0];
					vecCenter.y = vec3d[1];
					vecCenter.z = vec3d[2];

					vecCenter = vecCenter.MultiplyMatrix(mat4);
					pROPagedLOD->SetBoundingSphere(vecCenter, dRadius);

					osg::LOD::RangeMode nMode = pOsgPagedLOD->getRangeMode();
					pROPagedLOD->SetRangeMode(RangeMode(nMode));

					// 切换距离
					const osg::LOD::RangeList rl = pOsgPagedLOD->getRangeList();
					for (unsigned int i = 0; i < rl.size(); i++)
					{
						pROPagedLOD->GetRanges().push_back(std::vector<std::pair<float, float> >::value_type(rl[i].first, rl[i].second));
					}

					nChildCount = pOsgPagedLOD->getNumChildren();
					for (unsigned int i = 0; i < nChildCount; i++)
					{
						ProcessNode(pOsgPagedLOD->getChild(i), pROPagedLOD, pRootGroup, mat4, strNameID, strFilePath, vecLodName);
					}

					if (!pROPagedLOD->GetBoundingSphere().valid() && nChildCount > 0)
					{
						BoundingBox boudingBox;
						for (unsigned int i = 0; i < nChildCount; i++)
						{
							RenderOperationNode* pRONode = pROPagedLOD->GetChild(i);
							if (pRONode == nullptr || !StringUtil::CompareNoCase(pRONode->ClassName(), S3MB_SHELL_GEODE))
							{
								continue;
							}

							RenderOperationGeode* pGeode = pRONode->AsGeode();
							BoundingBox bGeoBox = pGeode->GetBoundingBox();
							Matrix4d matTemp = pGeode->GetPosition();
							bGeoBox.Transform(matTemp);
							boudingBox.Merge(bGeoBox);
						}

						pROPagedLOD->SetBoundingSphere(boudingBox.GetCenter(), (boudingBox.GetMax() - boudingBox.GetMin()).Length() / 2);
					}
				}

			}
			else if (StringUtil::CompareNoCase(strClassName, U("MatrixTransform")))
			{
				osg::ref_ptr<osg::MatrixTransform> pTransform = (osg::MatrixTransform*)pNode;
				osg::Matrix mat = pTransform->getMatrix();
				Matrix4d matLocalView;

				for (int t = 0; t < 4; t++)
				{
					for (int s = 0; s < 4; s++)
					{
						matLocalView[s][t] = mat(s, t);
					}
				}
				matLocalView = matLocalView * mat4;

				// 获得属性ID
				strNameID = StringUtil::UTF8_to_UNICODE(pTransform->getName());
				std::wstring strDir = StringUtil::GetDir(strFilePath);
				unsigned int nChildCount = pTransform->getNumChildren();
				for (unsigned int i = 0; i < nChildCount; i++)
				{
					std::wstring strChildClassName = StringUtil::UTF8_to_UNICODE(pTransform->getChild(i)->className());
					if (StringUtil::CompareNoCase(strChildClassName, S3MB_SHELL_GROUP))
					{

						m_mapRootNodesMatrix[strDir] = matLocalView;
					}

					ProcessNode(pTransform->getChild(i), pGroup, pRootGroup, matLocalView, strNameID, strFilePath, vecLodName);
				}
			}
			else
			{
				osg::ref_ptr<osg::Group> pOsgGroup = (osg::Group*)pNode;
				unsigned int nChildCount = pOsgGroup->getNumChildren();
				for (unsigned int i = 0; i < nChildCount; i++)
				{
					ProcessNode(pOsgGroup->getChild(i), pGroup, pRootGroup, mat4, strNameID, strFilePath, vecLodName);
				}
			}
		}

		if (pNode->asGeode())
		{
			RenderOperationGeode* pGeode = new RenderOperationGeode;
			pGroup->AddChild(pGeode);

			if (pGeode == nullptr)
			{
				return;
			}

			osg::ref_ptr<osg::Geode> pOsgGeode = (osg::Geode*)pNode;

			if (strNameID.empty())
			{
				// 获得属性ID
				strNameID = StringUtil::UTF8_to_UNICODE(pOsgGeode->getName());
			}

			if (!strNameID.empty())
			{
				int nStart = strNameID.find(U("["));
				int nEnd = strNameID.find(U("]"));
				if (nStart < nEnd)
				{
					std::string strNum = StringUtil::UNICODE_to_UTF8(strNameID.substr(nStart + 1, nEnd - nStart));
					std::wstring strGeodeName = StringUtil::UTF8_to_UNICODE(pOsgGeode->getName());
					pGeode->SetName(strGeodeName);
					pGeode->m_GeodeInfo.m_strTableName = strNameID;
					pGeode->m_GeodeInfo.m_nID = strtol(strNum.c_str(), nullptr, 0);
				}
			}

			pGeode->SetPosition(mat4);

			for (unsigned int i = 0; i < pOsgGeode->getNumDrawables(); i++)
			{
				ProcessDrawable(pOsgGeode->getDrawable(i), pGeode, pRootGroup, mat4, strFilePath);
			}

			pGeode->ComputerBoundingBox();
		}
	}

	void OSGBParser::ProcessDrawable(osg::Drawable* pDrawable, RenderOperationGeode* pGeode, RenderOperationGroup* pRootGroup, Matrix4d mat4, const std::wstring& strFilePath)
	{
		if (pGeode == nullptr || pRootGroup == nullptr)
		{
			return;
		}

		if (pDrawable->asGeometry())
		{
			osg::ref_ptr<osg::Geometry> pOsgGeometry = (osg::Geometry*)pDrawable;
			if (pOsgGeometry == nullptr || pOsgGeometry->empty())
			{
				return;
			}
			
            std::wstring strSkeleton = pRootGroup->GetName();
#ifdef WIN32
            strSkeleton += StringUtil::Format(U("_%p"), pOsgGeometry.get());
#else
            strSkeleton += StringUtil::Format(U("_%p").c_str(), pOsgGeometry.get());
#endif
            Skeleton* pSkeleton = pRootGroup->GetSkeleton(strSkeleton);
            if (pSkeleton == nullptr)
			{
                Skeleton* pSke = new Skeleton;
                pSke->m_strGeoName = strSkeleton;
                pRootGroup->AddSkeleton(strSkeleton, pSke);
                pGeode->AddSkeleton(pSke);
                ProcessGeometry(pOsgGeometry, pSke, pRootGroup, strFilePath);

                BoundingBox bBox = pSke->m_BoundingBox;
				bBox.Transform(mat4);

				BoundingSphere bRootSphere = pRootGroup->GetBoundingSphere();
				bRootSphere.ExpandBy(BoundingSphere(bBox));
				pRootGroup->SetBoundingSphere(bRootSphere.GetCenter(), bRootSphere.GetRadius());
			}
			else
			{
                pGeode->AddSkeleton(pSkeleton);

                BoundingBox bBox = pSkeleton->m_BoundingBox;
				bBox.Transform(mat4);

				BoundingSphere bRootSphere = pRootGroup->GetBoundingSphere();
				bRootSphere.ExpandBy(BoundingSphere(bBox));
				pRootGroup->SetBoundingSphere(bRootSphere.GetCenter(), bRootSphere.GetRadius());
			}
		}
	}

    void OSGBParser::ProcessGeometry(osg::Geometry* geometry, Skeleton* pROSke, RenderOperationGroup* pRootGroup, const std::wstring& strFilePath)
	{
        if (pROSke == nullptr || pRootGroup == nullptr)
		{
			return;
		}

        pROSke->m_pVertexDataPackage = new VertexDataPackage();

        ReadVertex(pROSke->m_pVertexDataPackage, geometry);

        ReadColorArray(pROSke, geometry);

        ReadTextureCoords(pROSke->m_pVertexDataPackage, geometry, pROSke->m_bInstanceBatch);

        ReadIndexPackage(pROSke->m_arrIndexPackage, geometry);

		if (geometry->getNormalArray())
		{
            ReadNormal(pROSke->m_pVertexDataPackage, geometry);
		}

        pROSke->ComputerBoundingBox();

		osg::ref_ptr<osg::StateSet> state = geometry->getStateSet();
		if (state == nullptr)
		{
			state = geometry->getParent(0)->getStateSet();
		}

		if (state)
		{
            ProcessMaterial(pROSke, state, pRootGroup, strFilePath);
		}
		else
		{
            ProcessDefaultMaterial(pROSke, pRootGroup);
		}
	}

	bool OSGBParser::ReadVertex(VertexDataPackage* pVertexDataPackage, osg::Geometry* pOsgGeometry)
	{
		if (pVertexDataPackage == nullptr || pOsgGeometry == nullptr || pOsgGeometry->getVertexArray() == nullptr)
		{
			assert(false);
			return false;
		}

		int nVerticesCount = 0;
		switch (pOsgGeometry->getVertexArray()->getType())
		{
		case osg::Array::FloatArrayType:
			break;
		case osg::Array::Vec2ArrayType:
			break;
		case osg::Array::Vec3ArrayType:
		{
			osg::ref_ptr<osg::Vec3Array> pVertexArray = (osg::Vec3Array*)pOsgGeometry->getVertexArray();
			nVerticesCount = pVertexArray->size();
			pVertexDataPackage->SetVertexNum(nVerticesCount);
			memcpy(pVertexDataPackage->m_pVertices, (float*)pVertexArray->getDataPointer(), nVerticesCount * sizeof(float) * 3);
		}
		break;
		case osg::Array::Vec4ArrayType:
		{
			osg::ref_ptr<osg::Vec4Array> pVertexArray = (osg::Vec4Array*)pOsgGeometry->getVertexArray();
			nVerticesCount = pVertexArray->size();
			pVertexDataPackage->m_nVertexDimension = 4;
			pVertexDataPackage->SetVertexNum(nVerticesCount);
			for (int i = 0; i < nVerticesCount; i++)
			{
				pVertexDataPackage->m_pVertices[4 * i] = (*pVertexArray)[i]._v[0];
				pVertexDataPackage->m_pVertices[4 * i + 1] = (*pVertexArray)[i]._v[1];
				pVertexDataPackage->m_pVertices[4 * i + 2] = (*pVertexArray)[i]._v[2];
				pVertexDataPackage->m_pVertices[4 * i + 3] = (*pVertexArray)[i]._v[3];
			}
		}
		break;
		default:
			break;
		}
		return true;
	}

    bool OSGBParser::ReadColorArray(Skeleton* pROSke, osg::Geometry* pOsgGeometry, unsigned int nFileType)
	{
        VertexDataPackage* pVertexDataPackage = pROSke->m_pVertexDataPackage;
		int nVerticesCount = pVertexDataPackage->m_nVerticesCount;
		osg::Array* pColorArray = pOsgGeometry->getColorArray();

		if (pColorArray)
		{
			osg::ref_ptr<osg::UIntArray> pColorUintArray = nullptr;
			if (pColorArray->getType() == osg::Array::Vec4ArrayType)
			{
				pColorUintArray = new osg::UIntArray;
				unsigned int colorUintValue;
				ColorValue colorValue;
				float r, g, b, a;
				float* pColorData = (float*)pColorArray->getDataPointer();
				if (pColorArray->getBinding() == osg::Array::BIND_PER_VERTEX)
				{
					for (int i = 0; i < nVerticesCount; i++)
					{
						r = pColorData[i * 4];
						g = pColorData[i * 4 + 1];
						b = pColorData[i * 4 + 2];
						a = pColorData[i * 4 + 3];

						colorValue.SetValue(r, g, b, a);
						colorUintValue = colorValue.GetValue();
						pColorUintArray->push_back(colorUintValue);
					}
				}
				else if (pColorArray->getBinding() == osg::Array::BIND_OVERALL ||
					pColorArray->getBinding() == osg::Array::BIND_PER_PRIMITIVE_SET)
				{
					r = pColorData[0];
					g = pColorData[1];
					b = pColorData[2];
					a = pColorData[3];

					colorValue.SetValue(r, g, b, a);
					colorUintValue = colorValue.GetValue();
					for (int i = 0; i < nVerticesCount; i++)
					{
						pColorUintArray->push_back(colorUintValue);
					}
				}
			}
			else if (pColorArray->getType() == osg::Array::UIntArrayType)
			{
				if (pColorArray->getBinding() == osg::Array::BIND_PER_VERTEX)
				{
					pColorUintArray = (osg::UIntArray*)pColorArray;
				}
				else if (pColorArray->getBinding() == osg::Array::BIND_OVERALL)
				{
					pColorUintArray = new osg::UIntArray;
					unsigned int* pColorData = (unsigned int*)pColorArray->getDataPointer();
					for (int i = 0; i < nVerticesCount; i++)
					{
						pColorUintArray->push_back(*pColorData);
					}
				}
				else
				{
					pColorUintArray = (osg::UIntArray*)pColorArray;
				}
			}
			if (pColorUintArray)
			{
				pVertexDataPackage->SetColorNum(nVerticesCount);
				memcpy(pVertexDataPackage->m_pVertexColor, (unsigned int*)pColorUintArray->getDataPointer(), nVerticesCount * sizeof(unsigned int));
				pVertexDataPackage->m_nVertexOptions |= VO_USE_SINGLE_COLOR;
			}
		}

		osg::ref_ptr<osg::UIntArray> pSecondColorArray = (osg::UIntArray*)pOsgGeometry->getSecondaryColorArray();
		if (pSecondColorArray && pSecondColorArray->getType() == osg::Array::UIntArrayType)
		{
			unsigned int* pSecondColorData = (unsigned int*)pSecondColorArray->getDataPointer();
			std::map<unsigned int, std::vector<SelectInfo*> > mapSelectInfo;
			unsigned int nID = pSecondColorData[0];

			SelectInfo* pSelectInfo = new SelectInfo;
			pSelectInfo->m_nVertexColorOffset = 0;
			pSelectInfo->m_nVertexColorCount = 0;

			std::vector<SelectInfo*> vecSelectInfo;
			vecSelectInfo.push_back(pSelectInfo);
			mapSelectInfo[nID] = vecSelectInfo;

			for (unsigned int i = 0; i < nVerticesCount; i++)
			{
				unsigned int nCurrentID = pSecondColorData[i];
				if (nCurrentID == nID)
				{
					pSelectInfo->m_nVertexColorCount++;
				}
				else
				{
					pSelectInfo = new SelectInfo;
					pSelectInfo->m_nVertexColorOffset = i;
					pSelectInfo->m_nVertexColorCount = 1;

					if (mapSelectInfo.find(nCurrentID) == mapSelectInfo.end())
					{
						std::vector<SelectInfo*> vecCurrentSelectInfo;
						vecCurrentSelectInfo.push_back(pSelectInfo);
						mapSelectInfo[nCurrentID] = vecCurrentSelectInfo;
					}
					else
					{
						mapSelectInfo[nCurrentID].push_back(pSelectInfo);
					}
					nID = nCurrentID;
				}
			}

			unsigned int nErrorID = 0xffffffff;
			if (mapSelectInfo.find(nErrorID) != mapSelectInfo.end())
			{
				std::vector<SelectInfo*>& vecIDInfo = mapSelectInfo[nErrorID];
				for (unsigned int i = 0; i < vecIDInfo.size(); i++)
				{
					delete vecIDInfo[i];
					vecIDInfo[i] = nullptr;
				}
				mapSelectInfo.erase(nErrorID);
			}

			std::map<unsigned int, std::vector<SelectInfo*> >::iterator itor;
			for (itor = mapSelectInfo.begin(); itor != mapSelectInfo.end(); itor++)
			{
				IDInfo* pIDInfo = new IDInfo;
				pIDInfo->m_nID = itor->first;
				std::vector<std::pair<int, int> >& vecOffsetAndCount = pIDInfo->m_arrVertexColorOffsetAndCount;
				std::vector<SelectInfo*>& vecIDInfo = itor->second;
				for (unsigned int i = 0; i < vecIDInfo.size(); i++)
				{
					vecOffsetAndCount.push_back(std::make_pair(vecIDInfo[i]->m_nVertexColorOffset, vecIDInfo[i]->m_nVertexColorCount));
					delete vecIDInfo[i];
					vecIDInfo[i] = nullptr;
				}
				vecIDInfo.clear();
                pROSke->AddIDInfo(pIDInfo);
			}
			mapSelectInfo.clear();
			pVertexDataPackage->m_nVertexOptions |= VO_USE_SINGLE_COLOR;
		}
		return true;
	}

	bool OSGBParser::ReadTextureCoords(VertexDataPackage* pVertexDataPackage, osg::Geometry* pOsgGeometry, bool& bInstanceBatch)
	{
		bInstanceBatch = false;
		int nVerticesCount = pVertexDataPackage->m_nVerticesCount;
		int iOsgTag = 0;
		int nOsgTagSize = pOsgGeometry->getTexCoordArrayList().size();
		for (; iOsgTag < nOsgTagSize; iOsgTag++)
		{
			osg::ref_ptr<osg::Vec2Array> pTexCoordArray = (osg::Vec2Array*)pOsgGeometry->getTexCoordArray(iOsgTag);
			if (pTexCoordArray.valid() && pTexCoordArray->getDataPointer() != nullptr)
			{
				break;
			}
		}

		for (int i = 0; i < SMSCN_MAX_TEXTURE_COORD_SETS; i++, iOsgTag++)
		{
			osg::ref_ptr<osg::Array> pTexCoordArray = (osg::Array*)pOsgGeometry->getTexCoordArray(iOsgTag);
			if (!pTexCoordArray.valid() || pTexCoordArray->getDataPointer() == nullptr)
			{
				break;
			}

			osg::Array::Type nType = pTexCoordArray->getType();
			unsigned int nElementSize = pTexCoordArray->getNumElements();
			nElementSize = nElementSize > nVerticesCount ? nVerticesCount : nElementSize;
			if (nType == osg::Array::FloatArrayType)
			{
				bInstanceBatch = true;
				float* pFloatArray = (float*)pTexCoordArray->getDataPointer();
				//第一个存储维度，第二个存储实例个数
				int nDimension = pFloatArray[0];
				int nTexCoordCount = pFloatArray[1];
				pFloatArray = pFloatArray + 2;
				pVertexDataPackage->m_nTexDimensions[i] = nDimension;
				pVertexDataPackage->SetTexCoordsNum(i, nTexCoordCount);
				memcpy(pVertexDataPackage->m_pTexCoords[i], \
					pFloatArray, nDimension * nTexCoordCount * sizeof(float));
			}
			else
			{
				if (nType == osg::Array::Vec2ArrayType)
				{
					pVertexDataPackage->m_nTexDimensions[i] = 2;
				}
				else if (nType == osg::Array::Vec3ArrayType)
				{
					pVertexDataPackage->m_nTexDimensions[i] = 3;
				}
				else if (nType == osg::Array::Vec4ArrayType)
				{
					pVertexDataPackage->m_nTexDimensions[i] = 4;
				}
				pVertexDataPackage->SetTexCoordsNum(i, nVerticesCount);
				memcpy(pVertexDataPackage->m_pTexCoords[i], (float*)pTexCoordArray->getDataPointer(),
					nElementSize * sizeof(float) * pVertexDataPackage->m_nTexDimensions[i]);
			}
		}
		return true;
	}

	bool OSGBParser::ReadIndexPackage(std::vector<IndexPackage*>& vecIndexPackage, osg::Geometry* pOsgGeometry)
	{
		{
			int nPriSetCount = pOsgGeometry->getNumPrimitiveSets();
			if (nPriSetCount > 0)
			{
				osg::Geometry::PrimitiveSetList pList = pOsgGeometry->getPrimitiveSetList();
				osg::Geometry::PrimitiveSetList::iterator ite;

				for (ite = pList.begin(); ite != pList.end(); ++ite)
				{
					bool bLoad = false;
					IndexPackage* pIndexPackage = new IndexPackage;
					osg::ref_ptr<osg::PrimitiveSet> pPrimiset = ite->get();
					switch (pPrimiset->getType())
					{
					case osg::PrimitiveSet::DrawElementsUShortPrimitiveType:
						bLoad = ProcessUShortIndexPackage(pPrimiset, pIndexPackage);
						break;
					case osg::PrimitiveSet::DrawElementsUBytePrimitiveType:
						bLoad = ProcessUByteIndexPackage(pPrimiset, pIndexPackage);
						break;
					case osg::PrimitiveSet::DrawElementsUIntPrimitiveType:
						bLoad = ProcessUIntIndexPackage(pPrimiset, pIndexPackage);
						break;
					case osg::PrimitiveSet::DrawArraysPrimitiveType:
						bLoad = ProcessArraysIndexPackage(pPrimiset, pIndexPackage);
						break;
					case osg::PrimitiveSet::DrawArrayLengthsPrimitiveType:
						bLoad = ProcessArrayLengthsIndexPackage(pPrimiset, pIndexPackage);
						break;
					default:
						assert(false);
						break;
					}

					if (bLoad)
					{
						vecIndexPackage.push_back(pIndexPackage);
					}
					else
					{
						delete pIndexPackage;
					}
				}
			}
			else
			{
				int nVerticesCount = GetVertexCount(pOsgGeometry);
				IndexPackage* pIndexPackage = new IndexPackage;
				pIndexPackage->m_enIndexType = IT_16BIT;
				pIndexPackage->m_OperationType = OT_TRIANGLE_LIST;
				pIndexPackage->m_nIndexesCount = nVerticesCount;
				pIndexPackage->m_pIndexes = new unsigned short[nVerticesCount];

				int i = 0;
				while (i < nVerticesCount)
				{
					pIndexPackage->m_pIndexes[i] = i++;
				}
				vecIndexPackage.push_back(pIndexPackage);
			}
		}
		return true;
	}

	bool OSGBParser::ReadNormal(VertexDataPackage* pVertexDataPackage, osg::Geometry* pOsgGeometry)
	{
		if (pOsgGeometry->getNormalArray() == nullptr)
		{
			return false;
		}
		int nVerticesCount = GetVertexCount(pOsgGeometry);
		switch (pOsgGeometry->getNormalArray()->getType())
		{
		case osg::Array::Vec2sArrayType:
		{
			osg::ref_ptr<osg::Vec2sArray> normalArray = (osg::Vec2sArray*)pOsgGeometry->getNormalArray();
			pVertexDataPackage->m_nNormalDimension = 3;
			pVertexDataPackage->SetNormalNum(nVerticesCount);
			//解压法线
			if (pOsgGeometry->getNormalBinding() == osg::Geometry::BIND_PER_VERTEX)
			{
				for (int i = 0; i < nVerticesCount; i++)
				{
					short envec[2];
					envec[0] = (*normalArray)[i]._v[0];
					envec[0] = (*normalArray)[i]._v[1];
					float vec[3];
					BaseUtils::OctDecode(envec, vec);
					Vector3d nomal(vec[0], vec[1], vec[2]);
					nomal.Normalize();
					pVertexDataPackage->m_pNormals[3 * i] = nomal.x;
					pVertexDataPackage->m_pNormals[3 * i + 1] = nomal.y;
					pVertexDataPackage->m_pNormals[3 * i + 2] = nomal.z;
				}
			}
		}
		break;
		case osg::Array::Vec3ArrayType:
		{
			osg::ref_ptr<osg::Vec3Array> normalArray = (osg::Vec3Array*)pOsgGeometry->getNormalArray();
			pVertexDataPackage->m_nNormalDimension = 3;
			pVertexDataPackage->SetNormalNum(nVerticesCount);

			if (pOsgGeometry->getNormalBinding() == osg::Geometry::BIND_PER_VERTEX)
			{
				memcpy(pVertexDataPackage->m_pNormals, (float*)normalArray->getDataPointer(), nVerticesCount * sizeof(float) * 3);
			}
			else if (pOsgGeometry->getNormalBinding() == osg::Geometry::BIND_OVERALL)
			{
				std::vector<osg::Vec3f> arrNormal = normalArray->asVector();
				for (int i = 0; i < nVerticesCount; i++)
				{
					osg::Vec3f v = arrNormal[0];
					pVertexDataPackage->m_pNormals[i * 3] = v[0];
					pVertexDataPackage->m_pNormals[i * 3 + 1] = v[1];
					pVertexDataPackage->m_pNormals[i * 3 + 2] = v[2];
				}
			}
		}
		break;
		default:
			break;
		}
		return true;
	}

    void OSGBParser::ProcessMaterial(Skeleton* pROSke, osg::StateSet* state, RenderOperationGroup* pRootGroup, const std::wstring& strFilePath)
	{
        if (pROSke == nullptr || pRootGroup == nullptr)
		{
			return;
		}

		std::wstring strStateName = pRootGroup->GetName();
#ifdef WIN32
		strStateName += StringUtil::Format(U("_%p"), state);
#else
        strStateName += StringUtil::Format(U("_%p").c_str(), state);
#endif
        pROSke->m_strMaterialName = strStateName;
        if (pRootGroup->IsMaterialExist(strStateName))
		{
            SetRoGeoMaterialName(pROSke, pRootGroup, strStateName);

			return;
		}

		Material* pMaterial = new Material;
		Technique* pTechnique = pMaterial->CreateTechnique();
		Pass* pPass = pTechnique->CreatePass();

		ReadMaterial(state, pPass);

		std::map<std::wstring, TextureDataInfo*> mapTextureExisted = pRootGroup->GetTextureData();
		std::vector<std::wstring> vecTexNameExisted;
		std::map<std::wstring, TextureDataInfo*>::iterator itTexutre;
		for (itTexutre = mapTextureExisted.begin(); itTexutre != mapTextureExisted.end(); itTexutre++)
		{
			vecTexNameExisted.push_back(itTexutre->first);
		}
		std::vector<std::wstring> vecTexName;
		std::vector<TextureData *> vecTextureData;
		std::vector<bool> vecHasMipMap;
		std::vector<int> vecLevel;
		std::vector<Matrix4d> vecMatrix;
		std::map<std::wstring, UVWAddressingMode> mapTexAddressingMode;

		ReadTextures(state, strFilePath, vecTexNameExisted, \
			vecTexName, vecTextureData, vecMatrix, vecHasMipMap, vecLevel, mapTexAddressingMode);
		assert(vecTexName.size() == vecTextureData.size());

		for (unsigned int i = 0; i < vecTexName.size(); i++)
		{
			TextureUnitState* pTexUnit = pPass->CreateTextureUnitState();
			pTexUnit->m_strTextureName = vecTexName[i];
			pTexUnit->m_TexModMatrix = vecMatrix[i];

			if (mapTexAddressingMode.size() != 0)
			{
				if (mapTexAddressingMode.find(pTexUnit->m_strTextureName) != mapTexAddressingMode.end())
				{
					pTexUnit->m_AddressMode = mapTexAddressingMode[pTexUnit->m_strTextureName];
					if (m_mapTexAddressingMode.find(pTexUnit->m_strTextureName) == m_mapTexAddressingMode.end())
					{
						m_mapTexAddressingMode[pTexUnit->m_strTextureName] = pTexUnit->m_AddressMode;
					}
				}
			}
			else if (m_mapTexAddressingMode.find(pTexUnit->m_strTextureName) != m_mapTexAddressingMode.end())
			{
				pTexUnit->m_AddressMode = m_mapTexAddressingMode[pTexUnit->m_strTextureName];
			}
			if (vecTextureData[i] == nullptr)
			{
				continue;
			}

			TextureDataInfo* pTextureDataInfo = new TextureDataInfo;
			pTextureDataInfo->m_pTextureData = vecTextureData[i];
			pTextureDataInfo->m_bMipmap = vecHasMipMap[i];
			pTextureDataInfo->m_nLevel = vecLevel[i];
			pTextureDataInfo->m_nLevel = MAX(pTextureDataInfo->m_nLevel, 0);
			pRootGroup->AddTextureData(vecTexName[i], pTextureDataInfo);
		}

		pMaterial->m_strName = MaterialHashFunc(pPass, strFilePath);
		pPass->m_strName = pMaterial->m_strName;

		pRootGroup->AddMaterial(strStateName, pMaterial);

        SetRoGeoMaterialName(pROSke, pRootGroup, strStateName);

		mapTexAddressingMode.clear();
	}

    bool OSGBParser::ProcessDefaultMaterial(Skeleton* pROSke, RenderOperationGroup* pRootGroup)
	{
        if (pROSke->m_arrIndexPackage.size() == 0)
		{
			return false;
		}
        if (pROSke->m_arrIndexPackage[0]->m_OperationType == OT_POINT_LIST)
		{
            if (!pRootGroup->IsMaterialExist(U("OSGBRenderPoint")))
			{
				Material* pMaterial = new Material;
				Technique* pTechnique = pMaterial->CreateTechnique();
				Pass* pPass = pTechnique->CreatePass();
				pMaterial->m_strName = U("OSGBRenderPoint");
				pPass->m_strName = pMaterial->m_strName;
				pRootGroup->AddMaterial(U("OSGBRenderPoint"), pMaterial);
			}

            SetRoGeoMaterialName(pROSke, pRootGroup, U("OSGBRenderPoint"));
		}
		else
		{
            if (!pRootGroup->IsSkeletonExist(U("OSGBEmpty")))
			{
				Material* pMaterial = new Material;
				Technique* pTechnique = pMaterial->CreateTechnique();
				Pass* pPass = pTechnique->CreatePass();
				pMaterial->m_strName = U("OSGBEmpty");
				pPass->m_strName = pMaterial->m_strName;
				pRootGroup->AddMaterial(U("OSGBEmpty"), pMaterial);
			}
            SetRoGeoMaterialName(pROSke, pRootGroup, U("OSGBEmpty"));
		}
		return true;
	}

	void OSGBParser::ReadMaterial(osg::StateSet* pState, Pass* pPass)
	{
		osg::ref_ptr<osg::Material> material = (osg::Material*)pState->getAttribute(osg::StateAttribute::MATERIAL);
		if (material)
		{
			osg::Vec4 vec4 = material->getAmbient(osg::Material::FRONT_AND_BACK);
			pPass->m_Ambient.SetValue(vec4[0], vec4[1], vec4[2], vec4[3]);

			vec4 = material->getDiffuse(osg::Material::FRONT_AND_BACK);
			pPass->m_Diffuse.SetValue(vec4[0], vec4[1], vec4[2], vec4[3]);

			vec4 = material->getSpecular(osg::Material::FRONT_AND_BACK);
			pPass->m_Specular.SetValue(vec4[0], vec4[1], vec4[2], vec4[3]);

			float value = material->getShininess(osg::Material::FRONT_AND_BACK);
			pPass->m_Shininess = value;
		}

		osg::ref_ptr<osg::BlendFunc> blendFunc = (osg::BlendFunc*)pState->getAttribute(osg::StateAttribute::BLENDFUNC);
		if (pState->getMode(GL_CULL_FACE) == osg::StateAttribute::ON)
		{
			pPass->SetPFFMode(PFF_CCW);
		}

		if (pState->getMode(GL_BLEND) == osg::StateAttribute::ON ||
			pState->getRenderingHint() == osg::StateSet::TRANSPARENT_BIN)
		{
			pPass->m_AlphaMode = AlphaMode::PBRAM_BLEND;
		}
	}

	void OSGBParser::ReadTextures(osg::StateSet* state, const std::wstring& strFilePath, \
		std::vector<std::wstring> &vecTexutreExisted, std::vector<std::wstring>& vecTexName, \
		std::vector<TextureData *>& vecTextureData, std::vector<Matrix4d>& vecMatrix, \
		std::vector<bool>& vecHasMipMap, std::vector<int>& vecLevel, std::map<std::wstring, UVWAddressingMode>& mapTexAddressingMode)
	{
		for (unsigned int i = 0; i < state->getNumTextureAttributeLists(); i++)
		{
			osg::ref_ptr<osg::Texture> texture = (osg::Texture*)state->getTextureAttribute(i, osg::StateAttribute::TEXTURE);
			if (!texture.valid())
			{
				continue;
			}

			osg::ref_ptr<osg::Image>image = texture->getImage(osg::Material::FRONT_AND_BACK);
			if (image == nullptr)
			{
				continue;
			}
			if (image->getPixelFormat() == GL_COMPRESSED_RGBA_S3TC_DXT5_EXT)
			{
				std::cout << "come in jiexi DXT" << std::endl;
				// 目前只考虑了DXT5情况
				int internalTextureFormat = 4;
				switch (image->getPixelFormat())
				{
				case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
					internalTextureFormat = 4;
					break;
				default:
					return;
				}

				unsigned char* pInBuffer = (unsigned char*)image->getDataPointer();
				int nWidth = image->s();
				int nHeight = image->t();

				unsigned char* pBuffer = nullptr;
				ImageOperator::Decode(internalTextureFormat, nWidth, nHeight, &pBuffer, pInBuffer, TC_DXT5);

				if (pBuffer == nullptr)
				{
					return;
				}

				unsigned char* pSquareBuffer = new unsigned char[nWidth*nHeight * 3];
				int i = 0, j = 0, k = 0;
				for (i = 0; i < nHeight*nWidth; i++, j += 3, k += 4)
				{
					pSquareBuffer[j] = pBuffer[k];	// R
					pSquareBuffer[j + 1] = pBuffer[k + 1];	// G
					pSquareBuffer[j + 2] = pBuffer[k + 2];	// B
				}

				image->setImage(nWidth, nHeight, 1, internalTextureFormat, GL_RGB, GL_UNSIGNED_BYTE, pSquareBuffer, osg::Image::USE_NEW_DELETE);

				delete[] pBuffer;
			}

			Matrix4d matTexture;
			ReadTextureMatrix(state, i, matTexture);
			vecMatrix.push_back(matTexture);

			std::wstring strImageFileName = StringUtil::UTF8_to_UNICODE(image->getFileName());
			if (strImageFileName.empty())
			{
				strImageFileName = BaseUtils::CreateGuid();
			}

			// 查找纹理是否已经保存
			bool bExistTexture = false;
			for (unsigned int j = 0; j < vecTexutreExisted.size(); j++)
			{
				if (StringUtil::CompareNoCase(strImageFileName, vecTexutreExisted[j]))
				{
					bExistTexture = true;
					break;
				}
			}

			if (bExistTexture)
			{
				vecTexName.push_back(strImageFileName);
				vecTextureData.push_back(nullptr);
				continue;
			}
			vecTexutreExisted.push_back(strImageFileName);

			std::wstring strTexPath = StringUtil::GetAbsolutePath(StringUtil::GetDir(strFilePath), strImageFileName);

			TextureData * osgTextureData = new TextureData();
			bool bMipmap = false;
			int nLevel = 0;
			ReadTexture(image, osgTextureData, strTexPath, bMipmap, nLevel);

			if (osgTextureData != nullptr)
			{
				if (osgTextureData->m_pBuffer == nullptr)
				{
					delete osgTextureData;
					osgTextureData = nullptr;
				}
			}

			vecTexName.push_back(strImageFileName);
			vecTextureData.push_back(osgTextureData);
			vecHasMipMap.push_back(bMipmap);
			vecLevel.push_back(nLevel);

			UVWAddressingMode addressingMode;
			addressingMode.u = GetTextureWrapMode(texture->getWrap(osg::Texture::WRAP_R));
			addressingMode.v = GetTextureWrapMode(texture->getWrap(osg::Texture::WRAP_S));
			addressingMode.w = GetTextureWrapMode(texture->getWrap(osg::Texture::WRAP_T));
			mapTexAddressingMode[strImageFileName] = addressingMode;
		}
	}

	void OSGBParser::ReadTextureMatrix(osg::StateSet* state, int i, Matrix4d& matResult)
	{
		osg::TexMat* texMat = (osg::TexMat*)state->getTextureAttribute(i, osg::StateAttribute::TEXMAT);
		Convert2Matrix(texMat, matResult);
	}

	void OSGBParser::ReadTexture(osg::Image* image, TextureData * osgTextureData, \
		std::wstring& strTexPath, bool& bMipmap, int &nLevel)
	{
		std::wstring strExt = StringUtil::GetExt(strTexPath);
		if (!StringUtil::IsFileExist(strTexPath) || !StringUtil::CompareNoCase(strExt, U(".rgb")))
		{
			GLenum osgPixelFormat = image->getPixelFormat();
			STKPixelFormat nPixelFormat = GlPixelFormat2Format(osgPixelFormat);
			if (nPixelFormat == STK_PF_Unknow)
			{
				return;
			}

			int nByteSize = image->getTotalSizeInBytes();
			if (image->getNumMipmapLevels() > 1)
			{
				nByteSize = image->getTotalSizeInBytesIncludingMipmaps();
			}

			if (nByteSize < 1)
			{
				nByteSize = image->s() * image->t() * 4;
			}

			unsigned char * pUchar = (unsigned char*)image->getDataPointer();
			if (pUchar == nullptr)
			{
				assert(false);
				return;
			}

			unsigned char * pData = nullptr;
			osgTextureData->m_nWidth = image->s();
			osgTextureData->m_nHeight = image->t();
			osgTextureData->m_nDepth = image->r();

			int nCompressType = 0;
			if (osgPixelFormat == GL_COMPRESSED_RGBA_S3TC_DXT5_EXT)
			{
				nCompressType = TC_DXT5;
			}

			if (nCompressType != 0)
			{
#ifndef OPENGL_ES_VERSION2
				unsigned char* pTempData = nullptr;
				int nPerPixelBytes = (nPixelFormat == STK_PF_RGB8) ? 3 : 4;
				osgTextureData->m_nSize = ImageOperator::Decode(nPerPixelBytes, osgTextureData->m_nWidth, osgTextureData->m_nHeight,
					&pTempData, pUchar, nCompressType);

				osgTextureData->m_pBuffer = pTempData;
				pData = pTempData;
#endif
				osgTextureData->m_enFormat = PF_RGBA8;
			}
			else
			{
				if (nPixelFormat == STK_PF_PF_RGBA8
					&& image->getDataType() == GL_UNSIGNED_SHORT)
				{
					nByteSize = nByteSize / 2;
					pData = new unsigned char[nByteSize];
					if (pData == nullptr)
					{
						return;
					}
					osgTextureData->m_pBuffer = pData;
					osgTextureData->m_nSize = nByteSize;
					osgTextureData->m_enFormat = PF_RGBA8;

					unsigned short* pDataSrc = (unsigned short*)pUchar;
					for (int i = 0; i < nByteSize; i++)
					{
						float fUShortMax = USHRT_MAX;
                        pData[i] = (unsigned char)(pDataSrc[i] / fUShortMax * UCHAR_MAX);
					}
				}
				else if (nPixelFormat == STK_PF_RGB8)
				{
					pData = new unsigned char[nByteSize];
					if (pData == nullptr)
					{
						return;
					}
					osgTextureData->m_pBuffer = pData;
					osgTextureData->m_nSize = nByteSize;
					osgTextureData->m_enFormat = PF_RGB8;

					memcpy(pData, pUchar, nByteSize * sizeof(unsigned char));
				}
				else
				{
					pData = new unsigned char[nByteSize];
					if (pData == nullptr)
					{
						return;
					}
					osgTextureData->m_pBuffer = pData;
					osgTextureData->m_nSize = nByteSize;
					osgTextureData->m_enFormat = PF_RGBA8;

					switch (nPixelFormat)
					{
					case STK_PF_PF_RGBA8:
						osgTextureData->m_enFormat = PF_RGBA8;
						break;
					default:
						if (pData)
						{
							delete[]pData;
							pData = nullptr;
							osgTextureData->m_pBuffer = nullptr;
						}
						break;
					}
					if (pData != nullptr)
					{
						memcpy(pData, pUchar, nByteSize * sizeof(unsigned char));
					}
				}
			}
			if (image->getNumMipmapLevels() > 1 && nCompressType == 0)
			{
				bMipmap = true;
				nLevel = image->getNumMipmapLevels();
			}
		}
		else
		{
			GLenum          osgPixelFormat = image->getPixelFormat();
			STKPixelFormat nPixelFormat = GlPixelFormat2Format(osgPixelFormat);
			unsigned char * pUchar = (unsigned char*)image->getDataPointer();
			if (nullptr != pUchar)
			{
				int nCom = 4;
				if (nPixelFormat == STK_PF_RGB8)
				{
					nCom = 3;
					osgTextureData->m_enFormat = PF_RGB8;
				}
				else
				{
					osgTextureData->m_enFormat = PF_RGBA8;
				}

				osgTextureData->m_nWidth = image->s();
				osgTextureData->m_nHeight = image->t();
				osgTextureData->m_nSize = image->s() * image->t() * nCom;
				osgTextureData->m_pBuffer = new unsigned char[osgTextureData->m_nSize];

				memcpy(osgTextureData->m_pBuffer, pUchar, osgTextureData->m_nSize);
			}
		}
	}

	bool OSGBParser::ProcessUShortIndexPackage(osg::PrimitiveSet* pPrimitiveSet, IndexPackage*& pIndexPackage)
	{
		if (pPrimitiveSet == nullptr || pIndexPackage == nullptr)
		{
			return false;
		}

		int nIndexCount = 0;
		if (pPrimitiveSet->getMode() == osg::PrimitiveSet::TRIANGLE_STRIP)
		{
			nIndexCount = (pPrimitiveSet->getTotalDataSize() / sizeof(unsigned short) - 2) * 3;
		}
		else if (pPrimitiveSet->getMode() == osg::PrimitiveSet::QUADS)
		{
			nIndexCount = pPrimitiveSet->getTotalDataSize() / sizeof(unsigned short) / 4 * 2 * 3;
		}
		else
		{
			nIndexCount = pPrimitiveSet->getTotalDataSize() / sizeof(unsigned short);
		}

		if (pPrimitiveSet->getMode() == osg::PrimitiveSet::POLYGON && nIndexCount < 3)
		{
			return false;
		}

		pIndexPackage->m_nIndexesCount = nIndexCount;
		pIndexPackage->m_pIndexes = new unsigned short[nIndexCount];

		if (pPrimitiveSet->getMode() == osg::PrimitiveSet::POINTS)
		{
			pIndexPackage->m_OperationType = OT_POINT_LIST;
		}
		else if (pPrimitiveSet->getMode() == osg::PrimitiveSet::LINES)
		{
			pIndexPackage->m_OperationType = OT_LINE_LIST;
		}
		else if (pPrimitiveSet->getMode() == osg::PrimitiveSet::LINE_STRIP)
		{
			pIndexPackage->m_OperationType = OT_LINE_STRIP;
		}
		else
		{
			pIndexPackage->m_OperationType = OT_TRIANGLE_LIST;
		}

		unsigned short* pIndex = (unsigned short*)pPrimitiveSet->getDataPointer();
		nIndexCount = pPrimitiveSet->getTotalDataSize() / sizeof(unsigned short);
		if (pPrimitiveSet->getMode() == osg::PrimitiveSet::TRIANGLE_STRIP)
		{
			int nIndex = 0;
			for (int t = 2; t < nIndexCount; t++)
			{
				if (t % 2 == 0)
				{
					pIndexPackage->m_pIndexes[(nIndex++)] = pIndex[t - 2];
					pIndexPackage->m_pIndexes[(nIndex++)] = pIndex[t - 1];
					pIndexPackage->m_pIndexes[(nIndex++)] = pIndex[t];
				}
				else
				{
					pIndexPackage->m_pIndexes[(nIndex++)] = pIndex[t - 1];
					pIndexPackage->m_pIndexes[(nIndex++)] = pIndex[t - 2];
					pIndexPackage->m_pIndexes[(nIndex++)] = pIndex[t];
				}
			}
		}
		else if (pPrimitiveSet->getMode() == osg::PrimitiveSet::QUADS)
		{
			unsigned int nQuadNum = nIndexCount / 4;
			int nIndex = 0;
			for (unsigned int t = 0; t < nQuadNum; t++)
			{
				pIndexPackage->m_pIndexes[nIndex++] = pIndex[t * 4 + 1];
				pIndexPackage->m_pIndexes[nIndex++] = pIndex[t * 4];
				pIndexPackage->m_pIndexes[nIndex++] = pIndex[t * 4 + 3];

				pIndexPackage->m_pIndexes[nIndex++] = pIndex[t * 4 + 1];
				pIndexPackage->m_pIndexes[nIndex++] = pIndex[t * 4 + 3];
				pIndexPackage->m_pIndexes[nIndex++] = pIndex[t * 4 + 2];
			}
		}
		else if (pPrimitiveSet->getMode() == osg::PrimitiveSet::POLYGON)
		{
			const unsigned int nTriangleNum = nIndexCount - 2;

			pIndexPackage->m_nIndexesCount = nTriangleNum * 3;
			delete[] pIndexPackage->m_pIndexes;
			pIndexPackage->m_pIndexes = new unsigned short[pIndexPackage->m_nIndexesCount];

			const int firstIndex = pIndex[0];
			int nIndex = 0;
			for (unsigned int t = 0; t < nTriangleNum; t++)
			{
				pIndexPackage->m_pIndexes[nIndex++] = firstIndex;
				pIndexPackage->m_pIndexes[nIndex++] = pIndex[t + 1];
				pIndexPackage->m_pIndexes[nIndex++] = pIndex[t + 2];
			}
		}
		else
		{
			memcpy(pIndexPackage->m_pIndexes, pIndex, nIndexCount * sizeof(unsigned short));
		}

		return true;
	}

	bool OSGBParser::ProcessUByteIndexPackage(osg::PrimitiveSet* pPrimitiveSet, IndexPackage*& pIndexPackage)
	{
		if (pPrimitiveSet == nullptr || pIndexPackage == nullptr)
		{
			return false;
		}

		int nIndexCount = 0;
		if (pPrimitiveSet->getMode() == osg::PrimitiveSet::TRIANGLE_STRIP || pPrimitiveSet->getMode() == osg::PrimitiveSet::TRIANGLE_FAN)
		{
			nIndexCount = (pPrimitiveSet->getTotalDataSize() / sizeof(unsigned char) - 2) * 3;
		}
		else if (pPrimitiveSet->getMode() == osg::PrimitiveSet::QUADS)
		{
			nIndexCount = pPrimitiveSet->getTotalDataSize() / sizeof(unsigned char) / 4 * 2 * 3;
		}
		else
		{
			nIndexCount = pPrimitiveSet->getTotalDataSize() / sizeof(unsigned char);
		}

		if (pPrimitiveSet->getMode() == osg::PrimitiveSet::POLYGON && nIndexCount < 3)
		{
			return false;
		}

		pIndexPackage->m_nIndexesCount = nIndexCount;
		pIndexPackage->m_pIndexes = new unsigned short[nIndexCount];

		if (pPrimitiveSet->getMode() == osg::PrimitiveSet::POINTS)
		{
			pIndexPackage->m_OperationType = OT_POINT_LIST;
		}
		else if (pPrimitiveSet->getMode() == osg::PrimitiveSet::LINES)
		{
			pIndexPackage->m_OperationType = OT_LINE_LIST;
		}
		else if (pPrimitiveSet->getMode() == osg::PrimitiveSet::LINE_STRIP)
		{
			pIndexPackage->m_OperationType = OT_LINE_STRIP;
		}
		else
		{
			pIndexPackage->m_OperationType = OT_TRIANGLE_LIST;
		}

		unsigned char* pIndex = (unsigned char*)pPrimitiveSet->getDataPointer();
		nIndexCount = pPrimitiveSet->getTotalDataSize() / sizeof(unsigned char);
		if (pPrimitiveSet->getMode() == osg::PrimitiveSet::TRIANGLE_STRIP)
		{
			int nIndex = 0;
			for (int t = 2; t < nIndexCount; t++)
			{
				if (t % 2 == 0)
				{
					pIndexPackage->m_pIndexes[(nIndex++)] = pIndex[t - 2];
					pIndexPackage->m_pIndexes[(nIndex++)] = pIndex[t - 1];
					pIndexPackage->m_pIndexes[(nIndex++)] = pIndex[t];
				}
				else
				{
					pIndexPackage->m_pIndexes[(nIndex++)] = pIndex[t - 1];
					pIndexPackage->m_pIndexes[(nIndex++)] = pIndex[t - 2];
					pIndexPackage->m_pIndexes[(nIndex++)] = pIndex[t];
				}
			}
		}
		else if (pPrimitiveSet->getMode() == osg::PrimitiveSet::TRIANGLE_FAN)
		{
			int nIndex = 0;
			for (int t = 2; t < nIndexCount; t++)
			{

				pIndexPackage->m_pIndexes[(nIndex++)] = pIndex[0];
				pIndexPackage->m_pIndexes[(nIndex++)] = pIndex[t - 1];
				pIndexPackage->m_pIndexes[(nIndex++)] = pIndex[t];

			}
		}
		else if (pPrimitiveSet->getMode() == osg::PrimitiveSet::QUADS)
		{
			unsigned int nQuadNum = nIndexCount / 4;
			int nIndex = 0;
			for (unsigned int t = 0; t < nQuadNum; t++)
			{
				pIndexPackage->m_pIndexes[nIndex++] = pIndex[t * 4 + 1];
				pIndexPackage->m_pIndexes[nIndex++] = pIndex[t * 4];
				pIndexPackage->m_pIndexes[nIndex++] = pIndex[t * 4 + 3];

				pIndexPackage->m_pIndexes[nIndex++] = pIndex[t * 4 + 1];
				pIndexPackage->m_pIndexes[nIndex++] = pIndex[t * 4 + 3];
				pIndexPackage->m_pIndexes[nIndex++] = pIndex[t * 4 + 2];
			}
		}
		else if (pPrimitiveSet->getMode() == osg::PrimitiveSet::POLYGON)
		{
			const unsigned int nTriangleNum = nIndexCount - 2;

			pIndexPackage->m_nIndexesCount = nTriangleNum * 3;
			delete[] pIndexPackage->m_pIndexes;
			pIndexPackage->m_pIndexes = new unsigned short[pIndexPackage->m_nIndexesCount];

			const int firstIndex = pIndex[0];
			int nIndex = 0;
			for (unsigned int t = 0; t < nTriangleNum; t++)
			{
				pIndexPackage->m_pIndexes[nIndex++] = firstIndex;
				pIndexPackage->m_pIndexes[nIndex++] = pIndex[t + 1];
				pIndexPackage->m_pIndexes[nIndex++] = pIndex[t + 2];
			}
		}
		else if (pPrimitiveSet->getMode() == osg::PrimitiveSet::TRIANGLES)
		{
			for (int t = 0; t < nIndexCount; t++)
			{
				pIndexPackage->m_pIndexes[t] = pIndex[t];
			}
		}
		else
		{
			memcpy(pIndexPackage->m_pIndexes, pIndex, nIndexCount * sizeof(unsigned short));
		}

		return true;
	}

	bool OSGBParser::ProcessUIntIndexPackage(osg::PrimitiveSet* pPrimitiveSet, IndexPackage*& pIndexPackage)
	{
		if (pPrimitiveSet == nullptr || pIndexPackage == nullptr)
		{
			return false;
		}

		int nIndexCount = 0;

		if (pPrimitiveSet->getMode() == osg::PrimitiveSet::TRIANGLE_STRIP)
		{
			nIndexCount = (pPrimitiveSet->getTotalDataSize() / sizeof(int) - 2) * 3;
		}
		else if (pPrimitiveSet->getMode() == osg::PrimitiveSet::QUADS)
		{
			nIndexCount = pPrimitiveSet->getTotalDataSize() / sizeof(int) / 4 * 2 * 3;
		}
		else
		{
			nIndexCount = pPrimitiveSet->getTotalDataSize() / sizeof(int);
		}

		if (pPrimitiveSet->getMode() == osg::PrimitiveSet::POLYGON && nIndexCount < 3)
		{
			return false;
		}

		pIndexPackage->m_enIndexType = IT_32BIT;
		pIndexPackage->m_nIndexesCount = nIndexCount;
		pIndexPackage->m_pIndexes = new unsigned short[nIndexCount * 2];

		if (pPrimitiveSet->getMode() == osg::PrimitiveSet::POINTS)
		{
			pIndexPackage->m_OperationType = OT_POINT_LIST;
		}
		else if (pPrimitiveSet->getMode() == osg::PrimitiveSet::LINES)
		{
			pIndexPackage->m_OperationType = OT_LINE_LIST;
		}
		else
		{
			pIndexPackage->m_OperationType = OT_TRIANGLE_LIST;
		}

		int* pIndex = (int*)pPrimitiveSet->getDataPointer();
		nIndexCount = pPrimitiveSet->getTotalDataSize() / sizeof(int);
		if (pPrimitiveSet->getMode() == osg::PrimitiveSet::TRIANGLE_STRIP)
		{
			int nIndex = 0;
			for (int t = 2; t < nIndexCount; t++)
			{
				if (t % 2 == 0)
				{
					((unsigned int*)pIndexPackage->m_pIndexes)[(nIndex++)] = pIndex[t - 2];
					((unsigned int*)pIndexPackage->m_pIndexes)[(nIndex++)] = pIndex[t - 1];
					((unsigned int*)pIndexPackage->m_pIndexes)[(nIndex++)] = pIndex[t];
				}
				else
				{
					((unsigned int*)pIndexPackage->m_pIndexes)[(nIndex++)] = pIndex[t - 1];
					((unsigned int*)pIndexPackage->m_pIndexes)[(nIndex++)] = pIndex[t - 2];
					((unsigned int*)pIndexPackage->m_pIndexes)[(nIndex++)] = pIndex[t];
				}
			}
		}
		else if (pPrimitiveSet->getMode() == osg::PrimitiveSet::QUADS)
		{
			unsigned int nQuadNum = nIndexCount / 4;
			int nIndex = 0;
			for (unsigned int t = 0; t < nQuadNum; t++)
			{
				((unsigned int*)pIndexPackage->m_pIndexes)[nIndex++] = pIndex[t * 4 + 1];
				((unsigned int*)pIndexPackage->m_pIndexes)[nIndex++] = pIndex[t * 4];
				((unsigned int*)pIndexPackage->m_pIndexes)[nIndex++] = pIndex[t * 4 + 3];

				((unsigned int*)pIndexPackage->m_pIndexes)[nIndex++] = pIndex[t * 4 + 1];
				((unsigned int*)pIndexPackage->m_pIndexes)[nIndex++] = pIndex[t * 4 + 3];
				((unsigned int*)pIndexPackage->m_pIndexes)[nIndex++] = pIndex[t * 4 + 2];
			}
		}
		else if (pPrimitiveSet->getMode() == osg::PrimitiveSet::POLYGON)
		{
			const unsigned int nTriangleNum = nIndexCount - 2;

			pIndexPackage->m_nIndexesCount = nTriangleNum * 3;
			delete[] pIndexPackage->m_pIndexes;
			pIndexPackage->m_pIndexes = (unsigned short*)(new unsigned int[pIndexPackage->m_nIndexesCount]);

			unsigned int* pIndexes = (unsigned int*)(pIndexPackage->m_pIndexes);
			const int firstIndex = pIndex[0];
			int nIndex = 0;
			for (unsigned int t = 0; t < nTriangleNum; t++)
			{
				pIndexes[nIndex++] = firstIndex;
				pIndexes[nIndex++] = pIndex[t + 1];
				pIndexes[nIndex++] = pIndex[t + 2];
			}
		}
		else
		{
			for (int t = 0; t < nIndexCount; t++)
			{
				*((unsigned int*)pIndexPackage->m_pIndexes + t) = pIndex[t];
			}
		}

		return true;
	}

	bool OSGBParser::ProcessArraysIndexPackage(osg::PrimitiveSet* pPrimitiveSet, IndexPackage*& pIndexPackage)
	{
		if (pPrimitiveSet == nullptr || pIndexPackage == nullptr)
		{
			return false;
		}

		int nOffset = 0;
		int nIndexCount = pPrimitiveSet->getNumIndices();

		if (pPrimitiveSet->getMode() == osg::PrimitiveSet::TRIANGLE_STRIP)
		{
			nIndexCount = (nIndexCount - 2) * 3;
			pIndexPackage->m_OperationType = OT_TRIANGLE_LIST;
		}
		else if (pPrimitiveSet->getMode() == osg::PrimitiveSet::QUAD_STRIP)
		{
			nIndexCount = (nIndexCount / 2 - 1) * 2 * 3;
			pIndexPackage->m_OperationType = OT_TRIANGLE_LIST;
		}
		else if (pPrimitiveSet->getMode() == osg::PrimitiveSet::QUADS)
		{
			nIndexCount = (nIndexCount / 4) * 2 * 3;
			pIndexPackage->m_OperationType = OT_TRIANGLE_LIST;
		}

		if (pPrimitiveSet->getMode() == osg::PrimitiveSet::POLYGON && nIndexCount < 3)
		{
			return false;
		}

		pIndexPackage->m_nIndexesCount = nIndexCount;

		if (pPrimitiveSet->getType() == osg::PrimitiveSet::DrawArraysPrimitiveType)
		{
			osg::ref_ptr<osg::DrawArrays> dArrays = (osg::DrawArrays*)pPrimitiveSet;
			nOffset = dArrays->getFirst();
		}
		else if (pPrimitiveSet->getType() == osg::PrimitiveSet::DrawArrayLengthsPrimitiveType)
		{
			osg::ref_ptr<osg::DrawArrayLengths> dArrays = (osg::DrawArrayLengths*)pPrimitiveSet;
			nOffset = dArrays->getFirst();
		}

		bool b32BIT = (nIndexCount + nOffset) > 65535;
		if (b32BIT)
		{
			pIndexPackage->m_enIndexType = IT_32BIT;
			pIndexPackage->m_pIndexes = new unsigned short[nIndexCount * 2];
		}
		else
		{
			pIndexPackage->m_enIndexType = IT_16BIT;
			pIndexPackage->m_pIndexes = new unsigned short[nIndexCount];
		}

		if (pPrimitiveSet->getMode() == osg::PrimitiveSet::TRIANGLE_STRIP)
		{
			int nIndex = 0;
			nIndexCount = pPrimitiveSet->getNumIndices();
			if (b32BIT)
			{
				for (int t = 2; t < nIndexCount; t++)
				{
					if (t % 2 == 0)
					{
						((unsigned int*)pIndexPackage->m_pIndexes)[(nIndex++)] = t - 2 + nOffset;
						((unsigned int*)pIndexPackage->m_pIndexes)[(nIndex++)] = t - 1 + nOffset;
						((unsigned int*)pIndexPackage->m_pIndexes)[(nIndex++)] = t + nOffset;
					}
					else
					{
						((unsigned int*)pIndexPackage->m_pIndexes)[(nIndex++)] = t - 1 + nOffset;
						((unsigned int*)pIndexPackage->m_pIndexes)[(nIndex++)] = t - 2 + nOffset;
						((unsigned int*)pIndexPackage->m_pIndexes)[(nIndex++)] = t + nOffset;
					}
				}
			}
			else
			{
				for (int t = 2; t < nIndexCount; t++)
				{
					if (t % 2 == 0)
					{
						pIndexPackage->m_pIndexes[(nIndex++)] = t - 2 + nOffset;
						pIndexPackage->m_pIndexes[(nIndex++)] = t - 1 + nOffset;
						pIndexPackage->m_pIndexes[(nIndex++)] = t + nOffset;
					}
					else
					{
						pIndexPackage->m_pIndexes[(nIndex++)] = t - 1 + nOffset;
						pIndexPackage->m_pIndexes[(nIndex++)] = t - 2 + nOffset;
						pIndexPackage->m_pIndexes[(nIndex++)] = t + nOffset;
					}
				}
			}
		}
		else if (pPrimitiveSet->getMode() == osg::PrimitiveSet::QUAD_STRIP)
		{
			int nIndex = 0;
			nIndexCount = pPrimitiveSet->getNumIndices();
			unsigned int nQuadNum = nIndexCount / 2 - 1;
			if (b32BIT)
			{
				for (unsigned int t = 0; t < nQuadNum; t++)
				{
					((unsigned int*)pIndexPackage->m_pIndexes)[nIndex++] = t * 2 + 1 + nOffset;
					((unsigned int*)pIndexPackage->m_pIndexes)[nIndex++] = t * 2 + nOffset;
					((unsigned int*)pIndexPackage->m_pIndexes)[nIndex++] = t * 2 + 2 + nOffset;

					((unsigned int*)pIndexPackage->m_pIndexes)[nIndex++] = t * 2 + 1 + nOffset;
					((unsigned int*)pIndexPackage->m_pIndexes)[nIndex++] = t * 2 + 2 + nOffset;
					((unsigned int*)pIndexPackage->m_pIndexes)[nIndex++] = t * 2 + 3 + nOffset;
				}
			}
			else
			{
				for (unsigned int t = 0; t < nQuadNum; t++)
				{
					pIndexPackage->m_pIndexes[nIndex++] = t * 2 + 1 + nOffset;
					pIndexPackage->m_pIndexes[nIndex++] = t * 2 + nOffset;
					pIndexPackage->m_pIndexes[nIndex++] = t * 2 + 2 + nOffset;

					pIndexPackage->m_pIndexes[nIndex++] = t * 2 + 1 + nOffset;
					pIndexPackage->m_pIndexes[nIndex++] = t * 2 + 2 + nOffset;
					pIndexPackage->m_pIndexes[nIndex++] = t * 2 + 3 + nOffset;
				}
			}
		}
		else if (pPrimitiveSet->getMode() == osg::PrimitiveSet::QUADS)
		{
			int nIndex = 0;
			nIndexCount = pPrimitiveSet->getNumIndices();
			unsigned int nQuadNum = nIndexCount / 4;
			if (b32BIT)
			{
				for (unsigned int t = 0; t < nQuadNum; t++)
				{
					((unsigned int*)pIndexPackage->m_pIndexes)[nIndex++] = t * 4 + 1 + nOffset;
					((unsigned int*)pIndexPackage->m_pIndexes)[nIndex++] = t * 4 + nOffset;
					((unsigned int*)pIndexPackage->m_pIndexes)[nIndex++] = t * 4 + 3 + nOffset;

					((unsigned int*)pIndexPackage->m_pIndexes)[nIndex++] = t * 4 + 1 + nOffset;
					((unsigned int*)pIndexPackage->m_pIndexes)[nIndex++] = t * 4 + 3 + nOffset;
					((unsigned int*)pIndexPackage->m_pIndexes)[nIndex++] = t * 4 + 2 + nOffset;
				}
			}
			else
			{
				for (unsigned int t = 0; t < nQuadNum; t++)
				{
					pIndexPackage->m_pIndexes[nIndex++] = t * 4 + 1 + nOffset;
					pIndexPackage->m_pIndexes[nIndex++] = t * 4 + nOffset;
					pIndexPackage->m_pIndexes[nIndex++] = t * 4 + 3 + nOffset;

					pIndexPackage->m_pIndexes[nIndex++] = t * 4 + 1 + nOffset;
					pIndexPackage->m_pIndexes[nIndex++] = t * 4 + 3 + nOffset;
					pIndexPackage->m_pIndexes[nIndex++] = t * 4 + 2 + nOffset;
				}
			}
		}
		else if (pPrimitiveSet->getMode() == osg::PrimitiveSet::POLYGON)
		{
			const unsigned int nTriangleNum = nIndexCount - 2;

			pIndexPackage->m_nIndexesCount = nTriangleNum * 3;
			delete[] pIndexPackage->m_pIndexes;
			if (b32BIT)
			{
				pIndexPackage->m_pIndexes = (unsigned short*)(new unsigned int[pIndexPackage->m_nIndexesCount]);
			}
			else
			{
				pIndexPackage->m_pIndexes = new unsigned short[pIndexPackage->m_nIndexesCount];
			}

			const int firstIndex = 0 + nOffset;
			int nIndex = 0;
			if (b32BIT)
			{
				for (unsigned int t = 0; t < nTriangleNum; t++)
				{
					((unsigned int*)pIndexPackage->m_pIndexes)[nIndex++] = firstIndex;
					((unsigned int*)pIndexPackage->m_pIndexes)[nIndex++] = t + 1 + nOffset;
					((unsigned int*)pIndexPackage->m_pIndexes)[nIndex++] = t + 2 + nOffset;
				}
			}
			else
			{
				for (unsigned int t = 0; t < nTriangleNum; t++)
				{
					pIndexPackage->m_pIndexes[nIndex++] = firstIndex;
					pIndexPackage->m_pIndexes[nIndex++] = t + 1 + nOffset;
					pIndexPackage->m_pIndexes[nIndex++] = t + 2 + nOffset;
				}
			}
		}
		else
		{
			if (b32BIT)
			{
				for (unsigned int t = 0; t < pIndexPackage->m_nIndexesCount; t++)
				{
					((unsigned int*)pIndexPackage->m_pIndexes)[t] = t + nOffset;
				}
			}
			else
			{
				for (unsigned int t = 0; t < pIndexPackage->m_nIndexesCount; t++)
				{
					pIndexPackage->m_pIndexes[t] = t + nOffset;
				}
			}
		}

		return true;
	}

	bool OSGBParser::ProcessArrayLengthsIndexPackage(osg::PrimitiveSet* pPrimitiveSet, IndexPackage*& pIndexPackage)
	{
		if (pPrimitiveSet == nullptr || pIndexPackage == nullptr)
		{
			return false;
		}

		assert(pPrimitiveSet->getType() == osg::PrimitiveSet::DrawArrayLengthsPrimitiveType);

		int nOffset = 0;
		osg::ref_ptr<osg::DrawArrayLengths> dArrays = (osg::DrawArrayLengths*)pPrimitiveSet;
		nOffset = dArrays->getFirst();

		int nIndexCount = 0;
		if (pPrimitiveSet->getMode() == osg::PrimitiveSet::TRIANGLE_STRIP ||
			pPrimitiveSet->getMode() == osg::PrimitiveSet::POLYGON)
		{
			for (osg::DrawArrayLengths::const_iterator primItr = dArrays->begin();
				primItr < dArrays->end(); ++primItr)
			{
				int nSub = (*primItr - 2) * 3;
				nIndexCount += nSub;
			}
		}
		else
		{
			nIndexCount = pPrimitiveSet->getNumIndices();
		}
		pIndexPackage->m_nIndexesCount = nIndexCount;

		bool b32BIT = (nIndexCount + nOffset) > 65535;

		if (b32BIT)
		{
			pIndexPackage->m_enIndexType = IT_32BIT;
			pIndexPackage->m_pIndexes = new unsigned short[nIndexCount * 2];
		}
		else
		{
			pIndexPackage->m_enIndexType = IT_16BIT;
			pIndexPackage->m_pIndexes = new unsigned short[nIndexCount];
		}

		if (pPrimitiveSet->getMode() == osg::PrimitiveSet::TRIANGLE_STRIP)
		{
			int nIndex = 0;
			if (b32BIT)
			{
				for (osg::DrawArrayLengths::const_iterator primItr = dArrays->begin();
					primItr < dArrays->end(); ++primItr)
				{
					int nSubCount = *primItr;
					for (int t = 2; t < nSubCount; t++)
					{
						if (t % 2 == 0)
						{
							((unsigned int*)pIndexPackage->m_pIndexes)[(nIndex++)] = t - 2 + nOffset;
							((unsigned int*)pIndexPackage->m_pIndexes)[(nIndex++)] = t - 1 + nOffset;
							((unsigned int*)pIndexPackage->m_pIndexes)[(nIndex++)] = t + nOffset;
						}
						else
						{
							((unsigned int*)pIndexPackage->m_pIndexes)[(nIndex++)] = t - 1 + nOffset;
							((unsigned int*)pIndexPackage->m_pIndexes)[(nIndex++)] = t - 2 + nOffset;
							((unsigned int*)pIndexPackage->m_pIndexes)[(nIndex++)] = t + nOffset;
						}
					}
					nOffset += nSubCount;
				}
			}
			else
			{
				for (osg::DrawArrayLengths::const_iterator primItr = dArrays->begin();
					primItr < dArrays->end(); ++primItr)
				{
					int nSubCount = *primItr;
					for (int t = 2; t < nSubCount; t++)
					{
						if (t % 2 == 0)
						{
							pIndexPackage->m_pIndexes[(nIndex++)] = t - 2 + nOffset;
							pIndexPackage->m_pIndexes[(nIndex++)] = t - 1 + nOffset;
							pIndexPackage->m_pIndexes[(nIndex++)] = t + nOffset;
						}
						else
						{
							pIndexPackage->m_pIndexes[(nIndex++)] = t - 1 + nOffset;
							pIndexPackage->m_pIndexes[(nIndex++)] = t - 2 + nOffset;
							pIndexPackage->m_pIndexes[(nIndex++)] = t + nOffset;
						}
					}
					nOffset += nSubCount;
				}
			}
		}
		else if (pPrimitiveSet->getMode() == osg::PrimitiveSet::POLYGON)
		{
			int nIndex = 0;
			if (b32BIT)
			{
				for (osg::DrawArrayLengths::const_iterator primItr = dArrays->begin();
					primItr < dArrays->end(); ++primItr)
				{
					int nSubCount = *primItr;
					for (int i = 0; i < nSubCount - 2; i++)
					{
						((unsigned int*)pIndexPackage->m_pIndexes)[(nIndex++)] = 0 + nOffset;
						((unsigned int*)pIndexPackage->m_pIndexes)[(nIndex++)] = 1 + i + nOffset;
						((unsigned int*)pIndexPackage->m_pIndexes)[(nIndex++)] = 2 + i + nOffset;
					}
					nOffset += nSubCount;
				}
			}
			else
			{
				for (osg::DrawArrayLengths::const_iterator primItr = dArrays->begin();
					primItr < dArrays->end(); ++primItr)
				{
					int nSubCount = *primItr;
					for (int i = 0; i < nSubCount - 2; i++)
					{
						pIndexPackage->m_pIndexes[(nIndex++)] = 0 + nOffset;
						pIndexPackage->m_pIndexes[(nIndex++)] = 1 + i + nOffset;
						pIndexPackage->m_pIndexes[(nIndex++)] = 2 + i + nOffset;
					}
					nOffset += nSubCount;
				}
			}
		}
		else if (pPrimitiveSet->getMode() == osg::PrimitiveSet::LINES)
		{
			if (b32BIT)
			{
				int nIndex = 0;
				for (osg::DrawArrayLengths::const_iterator primItr = dArrays->begin();
					primItr < dArrays->end(); ++primItr)
				{
					int nSubCount = *primItr;
					for (int t = 0; t < nSubCount; t++)
					{
						((unsigned int*)pIndexPackage->m_pIndexes)[(nIndex++)] = t + nOffset;
					}
					nOffset += nSubCount;
				}
			}
			else
			{
				int nIndex = 0;
				for (osg::DrawArrayLengths::const_iterator primItr = dArrays->begin();
					primItr < dArrays->end(); ++primItr)
				{
					int nSubCount = *primItr;
					for (int t = 0; t < nSubCount; t++)
					{
						pIndexPackage->m_pIndexes[(nIndex++)] = t + nOffset;
					}
					nOffset += nSubCount;
				}
			}
			pIndexPackage->m_OperationType = OT_LINE_LIST;
		}
		else
		{
			if (b32BIT)
			{
				int nIndex = 0;
				for (osg::DrawArrayLengths::const_iterator primItr = dArrays->begin();
					primItr < dArrays->end(); ++primItr)
				{
					int nSubCount = *primItr;
					for (int t = 0; t < nSubCount; t++)
					{
						((unsigned int*)pIndexPackage->m_pIndexes)[(nIndex++)] = t + nOffset;
					}
					nOffset += nSubCount;
				}
			}
			else
			{
				int nIndex = 0;
				for (osg::DrawArrayLengths::const_iterator primItr = dArrays->begin();
					primItr < dArrays->end(); ++primItr)
				{
					int nSubCount = *primItr;
					for (int t = 0; t < nSubCount; t++)
					{
						pIndexPackage->m_pIndexes[(nIndex++)] = t + nOffset;
					}
					nOffset += nSubCount;
				}
			}
		}

		return true;
	}

	int OSGBParser::GetVertexCount(osg::Geometry* pOsgGeometry)
	{
		if (pOsgGeometry->getVertexArray() == nullptr)
		{
			assert(false);
			return 0;
		}

		int nVerticesCount = 0;
		switch (pOsgGeometry->getVertexArray()->getType())
		{
		case osg::Array::FloatArrayType:
			break;
		case osg::Array::Vec2ArrayType:
			break;
		case osg::Array::Vec3ArrayType:
		{
			osg::ref_ptr<osg::Vec3Array> vertexArray = (osg::Vec3Array*)pOsgGeometry->getVertexArray();
			nVerticesCount = vertexArray->size();
		}
		break;
		case osg::Array::Vec4ArrayType:
		{
			osg::ref_ptr<osg::Vec4Array> vertexArray = (osg::Vec4Array*)pOsgGeometry->getVertexArray();
			nVerticesCount = vertexArray->size();
		}
		break;
		default:
			break;
		}
		return nVerticesCount;
	}

    void OSGBParser::SetRoGeoMaterialName(Skeleton* pSkeleton, RenderOperationGroup* pRootGroup, std::wstring strMaterialName)
	{
        if (pSkeleton->m_arrIndexPackage.size() <= 0)
		{
			return;
		}

		std::map<int, int> mapIndexCount;
        for (unsigned int i = 0; i < pSkeleton->m_arrIndexPackage.size(); i++)
		{
            if (pSkeleton->m_arrIndexPackage[i]->m_OperationType == OT_TRIANGLE_LIST)
			{
                mapIndexCount[OT_TRIANGLE_LIST] += pSkeleton->m_arrIndexPackage[i]->m_nIndexesCount - pSkeleton->m_arrIndexPackage[i]->m_nIndexesCount % 3;
			}
            else if (pSkeleton->m_arrIndexPackage[i]->m_OperationType == OT_LINE_STRIP)
			{
                mapIndexCount[OT_LINE_STRIP] += pSkeleton->m_arrIndexPackage[i]->m_nIndexesCount + 1;
			}
			else
			{
                mapIndexCount[pSkeleton->m_arrIndexPackage[i]->m_OperationType] += pSkeleton->m_arrIndexPackage[i]->m_nIndexesCount;
			}
		}
		bool bMixedData = mapIndexCount.size() > 1 ? true : false;

		std::vector<std::wstring> vecType;
		std::map<std::wstring, std::pair<IndexPackage*, int> > mapIndexPackage;
		std::map<std::wstring, std::pair<IndexPackage*, int> >::iterator itoIndexPackageNew;

        if (pSkeleton->m_pVertexDataPackage->m_nVerticesCount > 65535)
		{
            for (unsigned int i = 0; i < pSkeleton->m_arrIndexPackage.size(); i++)
			{
				IndexPackage* pIndexPackageNew = nullptr;
                OperationType nOperationType = pSkeleton->m_arrIndexPackage[i]->m_OperationType;
				std::wstring strType = OperationTypeToString(nOperationType);
                int nOneCount = pSkeleton->m_arrIndexPackage[i]->m_nIndexesCount;
				int nOffset = 0;
				itoIndexPackageNew = mapIndexPackage.find(strType);
				if (itoIndexPackageNew == mapIndexPackage.end())
				{
					pIndexPackageNew = new IndexPackage();
					pIndexPackageNew->m_OperationType = nOperationType;
					pIndexPackageNew->m_enIndexType = IT_32BIT;
					pIndexPackageNew->SetIndexNum(mapIndexCount[nOperationType]);
					if (nOperationType == OT_LINE_STRIP)
					{
						mapIndexPackage[strType] = std::pair<IndexPackage*, int>(pIndexPackageNew, nOneCount + 1);
						unsigned long nMax = pow(2.0, 32) - 1;
						memset(pIndexPackageNew->m_pIndexes, nMax, pIndexPackageNew->m_nIndexesCount * sizeof(unsigned int));
					}
					else
					{
						mapIndexPackage[strType] = std::pair<IndexPackage*, int>(pIndexPackageNew, nOneCount);
					}
					vecType.push_back(strType);
				}
				else
				{
					pIndexPackageNew = itoIndexPackageNew->second.first;
					nOffset = itoIndexPackageNew->second.second;
					if (nOperationType == OT_LINE_STRIP)
					{
						itoIndexPackageNew->second.second += nOneCount + 1;
					}
					else
					{
						itoIndexPackageNew->second.second += nOneCount;
					}
				}

				unsigned int *pIndexesNewInd = (unsigned int *)pIndexPackageNew->m_pIndexes + nOffset;
				int nValidIndexCount = nOneCount;
                if (pSkeleton->m_arrIndexPackage[i]->m_OperationType == OT_TRIANGLE_LIST)
				{
					nValidIndexCount = nOneCount - nOneCount % 3;
				}
                if (pSkeleton->m_arrIndexPackage[i]->m_enIndexType == IT_16BIT)
				{
					for (int j = 0; j < nValidIndexCount; j++)
					{
                        *pIndexesNewInd = pSkeleton->m_arrIndexPackage[i]->m_pIndexes[j];
						pIndexesNewInd++;
					}
				}
				else
				{
                    assert(pIndexPackageNew->m_enIndexType == pSkeleton->m_arrIndexPackage[i]->m_enIndexType);
                    memcpy(pIndexesNewInd, pSkeleton->m_arrIndexPackage[i]->m_pIndexes,
						nValidIndexCount * sizeof(int));
				}

                delete pSkeleton->m_arrIndexPackage[i];
                pSkeleton->m_arrIndexPackage[i] = nullptr;
			}
		}
		else
		{
            for (unsigned int i = 0; i < pSkeleton->m_arrIndexPackage.size(); i++)
			{
				IndexPackage* pIndexPackageNew = nullptr;
                OperationType nOperationType = pSkeleton->m_arrIndexPackage[i]->m_OperationType;
                int nOneCount = pSkeleton->m_arrIndexPackage[i]->m_nIndexesCount;
				std::wstring strType = OperationTypeToString(nOperationType);

				int nOffset = 0;
				itoIndexPackageNew = mapIndexPackage.find(strType);
				if (itoIndexPackageNew == mapIndexPackage.end())
				{
					pIndexPackageNew = new IndexPackage();
					pIndexPackageNew->m_OperationType = nOperationType;
					pIndexPackageNew->m_enIndexType = IT_16BIT;
					pIndexPackageNew->SetIndexNum(mapIndexCount[nOperationType]);
					if (nOperationType == OT_LINE_STRIP)
					{
						mapIndexPackage[strType] = std::pair<IndexPackage*, int>(pIndexPackageNew, nOneCount + 1);
						unsigned long nMax = pow(2.0, 16) - 1;
						memset(pIndexPackageNew->m_pIndexes, nMax, pIndexPackageNew->m_nIndexesCount * sizeof(unsigned short));
					}
					else
					{
						mapIndexPackage[strType] = std::pair<IndexPackage*, int>(pIndexPackageNew, nOneCount);
					}
					vecType.push_back(strType);
				}
				else
				{
					pIndexPackageNew = itoIndexPackageNew->second.first;
					nOffset = itoIndexPackageNew->second.second;
					if (nOperationType == OT_LINE_STRIP)
					{
						itoIndexPackageNew->second.second += nOneCount + 1;
					}
					else
					{
						itoIndexPackageNew->second.second += nOneCount;
					}
				}

				unsigned short *pIndexesNewInd = pIndexPackageNew->m_pIndexes + nOffset;
				int nValidIndexCount = nOneCount;
                if (pSkeleton->m_arrIndexPackage[i]->m_OperationType == OT_TRIANGLE_LIST)
				{
					nValidIndexCount = nOneCount - nOneCount % 3;
				}

                if (pSkeleton->m_arrIndexPackage[i]->m_enIndexType == IT_32BIT)
				{
                    int *pValue = (int*)pSkeleton->m_arrIndexPackage[i]->m_pIndexes;
					for (int j = 0; j < nValidIndexCount; j++)
					{
						*pIndexesNewInd = pValue[j];
						pIndexesNewInd++;
					}
				}
				else
				{
                    assert(pIndexPackageNew->m_enIndexType == pSkeleton->m_arrIndexPackage[i]->m_enIndexType);
                    memcpy(pIndexesNewInd, pSkeleton->m_arrIndexPackage[i]->m_pIndexes,
						nValidIndexCount * sizeof(unsigned short));

					pIndexesNewInd += nValidIndexCount;
				}

                delete pSkeleton->m_arrIndexPackage[i];
                pSkeleton->m_arrIndexPackage[i] = nullptr;
			}
		}

        pSkeleton->m_arrIndexPackage.clear();

		for (int i = 0; i < vecType.size(); i++)
		{
			std::wstring strExt = vecType[i];
			IndexPackage* pIndexPackage = mapIndexPackage[strExt].first;
			if (pIndexPackage == nullptr)
			{
				continue;
			}

			Material* pMaterialSrc = pRootGroup->GetMaterial(strMaterialName);
			if (bMixedData && pMaterialSrc != nullptr)
			{
				Material* pMaterial = new Material(*pMaterialSrc);
				pMaterial->m_strName += strExt;
				int nTech = pMaterial->getNumTechniques();
				for (int i = 0; i < nTech; i++)
				{
					Technique* pTechnique = pMaterial->getTechnique(i);
					if (pTechnique != nullptr)
					{
						int nPasses = pTechnique->getNumPasses();
						for (int j = 0; j < nPasses; j++)
						{
							Pass* pPass = pTechnique->getPass(j);
							if (pPass != nullptr)
							{
								pPass->m_strName += strExt;
							}
						}
					}
				}
				pRootGroup->AddMaterial(strMaterialName + strExt, pMaterial);
				pIndexPackage->m_strPassName.push_back(strMaterialName + strExt);
			}
			else
			{
				pIndexPackage->m_strPassName.push_back(strMaterialName);
			}
            pSkeleton->m_arrIndexPackage.push_back(pIndexPackage);
		}

		if (bMixedData)
		{
			std::map<std::wstring, Material*>::iterator itorMaterial = pRootGroup->GetMaterials().find(strMaterialName);
			if (itorMaterial != pRootGroup->GetMaterials().end())
			{
				delete itorMaterial->second;
				itorMaterial->second = nullptr;
				pRootGroup->GetMaterials().erase(itorMaterial);
			}
		}
	}

	std::wstring OSGBParser::OperationTypeToString(OperationType nType)
	{
		std::wstring strType;
		switch (nType)
		{
		case OT_LINE_LIST:
			strType = U("OT_LINE_LIST");
			break;

		case OT_LINE_STRIP:
			strType = U("OT_LINE_STRIP");
			break;

		case OT_POINT_LIST:
			strType = U("OT_POINT_LIST");
			break;

		case OT_TRIANGLE_LIST:
			strType = U("OT_TRIANGLE_LIST");
			break;

		case OT_TRIANGLE_STRIP:
			strType = U("OT_TRIANGLE_STRIP");
			break;

		case OT_TRIANGLE_FAN:
			strType = U("OT_TRIANGLE_FAN");
			break;

		case OT_QUAD_STRIP:
			strType = U("OT_QUAD_STRIP");
			break;

		case OT_QUAD_LIST:
			strType = U("OT_QUAD_LIST");
			break;
		default:
			break;
		}

		return strType;
	}

	void OSGBParser::Convert2Matrix(osg::TexMat* texMat, Matrix4d& matResult)
	{
		if (texMat == nullptr)
		{
			return;
		}

		osg::Matrix mat = texMat->getMatrix();
		for (int t = 0; t < 4; t++)
		{
			for (int s = 0; s < 4; s++)
			{
				matResult[s][t] = mat(s, t);
			}
		}
	}

	STKPixelFormat OSGBParser::GlPixelFormat2Format(GLenum glPixelFormat)
	{
		STKPixelFormat pixelFormat = STK_PF_Unknow;
		switch (glPixelFormat)
		{
		case GL_RGB:
			pixelFormat = STK_PF_RGB8;
			break;
		case GL_RGBA:
			pixelFormat = STK_PF_PF_RGBA8;
			break;
		case GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG:
			pixelFormat = STK_PF_RGB8;
			break;
		case GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG:
			pixelFormat = STK_PF_PF_RGBA8;
			break;
		case GL_ETC1_RGB8_OES:
			pixelFormat = STK_PF_PF_RGBA8;
			break;
		default:
			break;
		}

		return pixelFormat;
	}

	TextureAddressingMode OSGBParser::GetTextureWrapMode(osg::Texture::WrapMode osgMode)
	{
		TextureAddressingMode texMode;
		switch (osgMode)
		{
		case osg::Texture::REPEAT:
			texMode = TAM_WRAP;
			break;
		case osg::Texture::MIRROR:
			texMode = TAM_MIRROR;
			break;
		case osg::Texture::CLAMP:
		case osg::Texture::CLAMP_TO_EDGE:
		case osg::Texture::CLAMP_TO_BORDER:
		default:
			texMode = TAM_CLAMP;
			break;
		}
		return texMode;
	}

	std::wstring OSGBParser::MaterialHashFunc(Pass* pPass, const std::wstring& strFilePath)
	{
		if (pPass == nullptr)
		{
			return U("");
		}

		std::wstring strName;
		if (pPass->GetTextureUnitStatesSize() > 0)
		{
			for (int i = 0; i < pPass->GetTextureUnitStatesSize(); i++)
			{
				TextureUnitState* pTexUnit = pPass->GetTextureUnitState(i);
				if (pTexUnit != nullptr)
				{
					strName += pTexUnit->m_strTextureName;
				}
			}
		}
		else
		{
			std::wstring strTemp;
#ifdef WIN32
			strTemp = StringUtil::Format(U("%x_"), pPass->m_Ambient.GetValue());
#else
            strTemp = StringUtil::Format(U("%x_").c_str(), pPass->m_Ambient.GetValue());
#endif
			strName += strTemp;
#ifdef WIN32
			strTemp = StringUtil::Format(U("%x_"), pPass->m_Diffuse.GetValue());
#else
            strTemp = StringUtil::Format(U("%x_").c_str(), pPass->m_Diffuse.GetValue());
#endif
			strName += strTemp;
#ifdef WIN32
			strTemp = StringUtil::Format(U("%x_"), pPass->m_Specular.GetValue());
#else
            strTemp = StringUtil::Format(U("%x_").c_str(), pPass->m_Specular.GetValue());
#endif
			strName += strTemp;
#ifdef WIN32
			strTemp = StringUtil::Format(U("%6f_"), pPass->m_Shininess);
#else
            strTemp = StringUtil::Format(U("%6f_").c_str(), pPass->m_Shininess);
#endif
			strName += strTemp;
#ifdef WIN32
			strTemp = StringUtil::Format(U("%d"), pPass->GetPFFMode());
#else
            strTemp = StringUtil::Format(U("%d").c_str(), pPass->GetPFFMode());
#endif
			strName += strTemp;
		}

		return strName;
	}

	void OSGBParser::Clear()
	{
		m_mapRootNodesMatrix.clear();
		m_mapTexAddressingMode.clear();

		if (m_pROGroup != nullptr)
		{
			delete m_pROGroup;
			m_pROGroup = nullptr;
		}
	}
}