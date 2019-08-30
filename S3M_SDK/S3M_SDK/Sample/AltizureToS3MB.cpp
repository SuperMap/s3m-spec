#include <fstream>
#include "AltizureToS3MB.h"
#include "CacheBuilder3D/UGS3MBSCP.h"

AltizureToS3MB::AltizureToS3MB()
{
	m_pTreeNode = NULL;

	priCoordSysType = EmPrjCoordSysType::PCS_USER_DEFINED;

	UGStringEx strTileMessage;
	strTileMessage.LoadResString(UGS_QUARTER_OBJ_TO_S3MB_EXPORT);
	m_nTotalRecCount = 0;
	m_nPrecent = 0;
	m_nCurrentIndex = 0;
}

AltizureToS3MB::~AltizureToS3MB()
{
	if(m_pTreeNode !=NULL)
	{
		deleteQTreeNode(m_pTreeNode);
		m_pTreeNode = NULL;
	}
}

void AltizureToS3MB::SetFileName(const UGString & strFile)
{
	strConfigJson = strFile;
}
// 判断是2的几次方（代表层数）
static UGuint Get2n(UGuint n)
{
	UGuint i = 0;
	while(n)
	{
		i++;
		n = n >> 1;
	}
	return i - 1;
}


void AltizureToS3MB::SetOutputFilePath(const UGString & strOutputPath)
{
	strOutputFilePath =	strOutputPath;
}

void AltizureToS3MB::ParseJson()
{
	if (!UGFile::IsExist(strConfigJson))
	{
		return;
	}

	UGJsonValue *jsonValue = NULL;
	if (!UGJsonUtils::LoadFromStdFile(strConfigJson, jsonValue, UGString::UTF8))
	{
		return;
	}

	UGJsonValue *utmJsonValue = NULL;
	if (jsonValue->GetValue(_U("utm_zone"), utmJsonValue))
	{
		UGString strLatitude;
		UGint nLongitude = 0;
		utmJsonValue->GetValue(_U("latitude_zone"), strLatitude);
		utmJsonValue->GetValue(_U("longitude_zone"), nLongitude);

		UGint nNum = nLongitude - 1;

		if (strLatitude.CompareNoCase(_U("N")) == 0 || 
			strLatitude.CompareNoCase(_U("P")) == 0 ||
			strLatitude.CompareNoCase(_U("Q")) == 0 ||
			strLatitude.CompareNoCase(_U("R")) == 0 ||
			strLatitude.CompareNoCase(_U("S")) == 0 ||
			strLatitude.CompareNoCase(_U("T")) == 0 ||
			strLatitude.CompareNoCase(_U("U")) == 0 ||
			strLatitude.CompareNoCase(_U("V")) == 0 ||
			strLatitude.CompareNoCase(_U("W")) == 0 ||
			strLatitude.CompareNoCase(_U("X")) == 0 )
		{
			// WGS UTM ZONE 1-60 North
			priCoordSysType = EmPrjCoordSysType(EmPrjCoordSysType::PCS_WGS_1984_UTM_1N + nNum);
		}
		else if (strLatitude.CompareNoCase(_U("M")) == 0 || 
				 strLatitude.CompareNoCase(_U("L")) == 0 ||
				 strLatitude.CompareNoCase(_U("K")) == 0 ||
				 strLatitude.CompareNoCase(_U("J")) == 0 ||
				 strLatitude.CompareNoCase(_U("H")) == 0 ||
				 strLatitude.CompareNoCase(_U("G")) == 0 ||
				 strLatitude.CompareNoCase(_U("F")) == 0 ||
				 strLatitude.CompareNoCase(_U("E")) == 0 ||
				 strLatitude.CompareNoCase(_U("D")) == 0 ||
				 strLatitude.CompareNoCase(_U("C")) == 0 )
		{
			// WGS UTM ZONE 1-60 South
			priCoordSysType = EmPrjCoordSysType(EmPrjCoordSysType::PCS_WGS_1984_UTM_1S + nNum);
		}

		if (!utmJsonValue)
		{
			delete utmJsonValue;
			utmJsonValue = NULL;
		}
	}

	UGJsonValue *modelTransformJsonValue = NULL;
	if (jsonValue->GetValue(_U("model_transform"), modelTransformJsonValue))
	{
		// 转化矩阵
		if(modelTransformJsonValue->GetArrarySize() != 0)
		{
			UGJsonValue *modelTransform0 = new UGJsonValue(UGJsonValue::arrayValue);
			UGdouble dModelTransform03 = 0;
			if (modelTransformJsonValue->GetAt(0, modelTransform0) &&
				modelTransform0->GetAt(3, dModelTransform03))
			{
				m_postion.x = dModelTransform03;
			}

			UGJsonValue *modelTransform1 = new UGJsonValue(UGJsonValue::arrayValue);
			UGdouble dModelTransform13 = 0;
			if (modelTransformJsonValue->GetAt(1, modelTransform1) &&
				modelTransform1->GetAt(3, dModelTransform13))
			{
				m_postion.y = dModelTransform13;
			}

			UGJsonValue *modelTransform2 = new UGJsonValue(UGJsonValue::arrayValue);
			UGdouble dModelTransform23 = 0;
			if (modelTransformJsonValue->GetAt(2, modelTransform2) &&
				modelTransform2->GetAt(3, dModelTransform23))
			{
				m_postion.z = dModelTransform23;
			}

			delete modelTransform0;
			modelTransform0 = NULL;
			delete modelTransform1;
			modelTransform1 = NULL;
			delete modelTransform2;
			modelTransform2 = NULL;
		}

		if (!modelTransformJsonValue) 
		{
			delete modelTransformJsonValue;
			modelTransformJsonValue = NULL;
		}
	}

	UGJsonValue *tileJsonValue = NULL;
	if (jsonValue->GetValue(_U("tile"), tileJsonValue))
	{
		UGint tileLength = 0;
		if (tileJsonValue->GetArrarySize() != 0 && 
			tileJsonValue->GetAt(0, tileLength)) 
		{
			nTileLength = tileLength;
			nLodNum = Get2n(nTileLength);
		}

		if (!tileJsonValue)
		{
			delete tileJsonValue;
			tileJsonValue = NULL;
		}
	}

	if (!jsonValue) 
	{
		delete jsonValue;
		jsonValue = NULL;
	}
}
static void PrjCoordTransBBox(UGBoundingBox & bbox, UGPoint3D & pos, UGPrjCoordSys * pPrjCoordSys)
{
	if(pPrjCoordSys == NULL)
	{
		return;
	}
	UGVector3d min = bbox.GetMin();
	UGVector3d max = bbox.GetMax();

	UGPoint3D center = pos;
	if(center.x <= 180 && center.y <=90 && center.x >= -180 && center.y >= -90)
	{
		pPrjCoordSys->Forward(&center, 1);
	}

	UGPoint3D minp , maxp;

	minp.x = center.x + min.x;
	minp.y = center.y + min.y;
	minp.z = center.z + min.z;

	maxp.x = center.x + max.x;
	maxp.y = center.y + max.y;
	maxp.z = center.z + max.z;

	pPrjCoordSys->Inverse(&minp, 1);
	pPrjCoordSys->Inverse(&maxp, 1);

	min.x = minp.x;
	min.y = minp.y;
	min.z = minp.z;

	max.x = maxp.x;
	max.y = maxp.y;
	max.z = maxp.z;
	UGBoundingBox box(min,max);

	bbox = box;

}
void AltizureToS3MB::GenerateScp()
{
	UGS3MBSCP* pS3MBSCP = new UGS3MBSCP;

	UGBoundingBox box = m_pTreeNode->boundBox;
	UGPoint3D pos = m_postion;
	if(priCoordSysType != EmPrjCoordSysType::PCS_USER_DEFINED)
	{
		UGPrjCoordSys* pPrjCoordSys = new UGPrjCoordSys(priCoordSysType);

		if(!(pos.x <=180 && pos.x >=-180 && pos.y <=90 && pos.y >=-90))
		{
			pPrjCoordSys->Inverse(&pos, 1);
		}

		PrjCoordTransBBox(box, m_postion, pPrjCoordSys);

		delete pPrjCoordSys;
		pPrjCoordSys = NULL;
	}

	pS3MBSCP->SetAsset(_U("SuperMap"));
	pS3MBSCP->SetPosition(pos);

	pS3MBSCP->SetGeoBounds(UGRect2D(box.GetMin().x,box.GetMax().y,box.GetMax().x,box.GetMin().y));

	std::vector<UGString> vecRootFiles;
	std::vector<UGBoundingBox> vecRootBoundboxs;
	UGString strFileName = UGFile::ChangeExt(m_pTreeNode->strFileName, _U(".s3mb"));
	vecRootFiles.push_back(strFileName);
	vecRootBoundboxs.push_back(m_pTreeNode->boundBox);

	pS3MBSCP->SetTiles(vecRootFiles,vecRootBoundboxs);

	UGString strScpName = UGFile::GetTitle(strConfigJson)+_U(".scp");
	UGString strScpFilePatch = UGFile::GetAbsolutePath(strOutputFilePath, strScpName);
	pS3MBSCP->SaveToJsonFile(strScpFilePatch);
	delete pS3MBSCP;
	pS3MBSCP = NULL;
}

void AltizureToS3MB::GenerateS3MB()
{
	if (!UGFile::IsExist(strOutputFilePath))
	{
		UGFile::MkDirEx(strOutputFilePath);
	}

	UGStringEx strMessage;
	strMessage.LoadResString(UGS_QUARTER_OBJ_TO_S3MB_EXPORT);
	m_nTotalRecCount = pow(2, nLodNum * 2) - 1;
	// 建立四分树
	MakeQuarterTree();
	// 生成S3M
	strMessage.LoadResString(UGS_QUARTER_OBJ_TO_S3MB_TREE);
	m_nCurrentIndex = 0;
	m_nPrecent = 0;
	Convert2RO(m_pTreeNode);
	m_nTotalRecCount = 0;
	m_nPrecent = 0;
	m_nCurrentIndex = 0;
}

void AltizureToS3MB::Convert(OBJLodQuarterTreeNode* pNode)
{

}

void AltizureToS3MB::AppendRO(UGRenderOperationGroup* pGroupSrc, 
						 UGRenderOperationGroup* pGroupDes, UGRenderOperationGeode *pGeodeDes)
{
	std::map<UGString, UGRenderOperationGeometry*> mapGeometry = pGroupSrc->GetGeometry();
	std::map<UGString, UGRenderOperationGeometry*>::iterator itGeo = mapGeometry.begin();
	for (itGeo = mapGeometry.begin(); itGeo != mapGeometry.end(); itGeo++)
	{
		pGroupDes->AddGeometry(itGeo->first, itGeo->second);
		pGeodeDes->AddRenderOperationGeoemtry(itGeo->second);
	}
	std::map<UGString, UGMaterial3D*> mapMaterial = pGroupSrc->GetMaterial3Ds();
	std::map<UGString, UGMaterial3D*>::iterator itMat = mapMaterial.begin();
	for (itMat = mapMaterial.begin(); itMat != mapMaterial.end(); itMat++)
	{
		pGroupDes->AddMaterial3D(itMat->first, itMat->second);
	}

	std::map<UGString,  UGRenderOperationNode::TextureDataInfo*> mapTex= pGroupSrc->GetTextureData();
	std::map<UGString, UGRenderOperationNode::TextureDataInfo*>::iterator itTex = mapTex.begin();
	for (itTex = mapTex.begin(); itTex != mapTex.end(); itTex++)
	{
		pGroupDes->AddTextureData(itTex->first, itTex->second);
	}
}

UGRenderOperationGroup* AltizureToS3MB::GetRO(const UGString strObjFile)
{
	UGModelImportParams params;
	params.SetFilePathName(strObjFile);
	params.SetImportMode(UGImportParams::ModeOnlyReadOSG);	
	UGint nFileType = params.GetFileType();
	UGFileParseModel* pFileParser = (UGFileParseModel*)UGFileParseManager::CreateFileParser(nFileType);
	pFileParser->SetParseModelNode(FALSE);
	pFileParser->SetIgnoreLod(TRUE);
	if(!pFileParser->Open(params))
	{
		delete pFileParser;
		pFileParser = NULL;
		return NULL;
	}

	UGRenderOperationGroup* pGroup = NULL;
	pFileParser->GetRenderOperationGroup(pGroup);

	delete pFileParser;
	pFileParser = NULL;
	return pGroup;
}

void AltizureToS3MB::WriteRO(UGRenderOperationGroup* pGroup, const UGString strDesFile)
{
	// 四合一存S3M
	UGFileParser* pFileParserS3M = UGFileParseManager::CreateFileParser(UGFileType::S3MB);
	if(pFileParserS3M == NULL)
	{
		UGASSERT(FALSE);
		return;
	}
	UGExportParams params;
	params.SetFilePathName(strDesFile);
	((UGFileParseModel*)pFileParserS3M)->Save(params, pGroup);
	delete pFileParserS3M;
	pFileParserS3M = NULL;
}

void AltizureToS3MB::Convert2RO(OBJLodQuarterTreeNode* pNode)
{
	if(pNode == NULL)
	{
		return;
	}

	UGuint nSize = pNode->childs.size();
	if(nSize == 0)
	{
		m_nCurrentIndex++;
		m_nPrecent = (m_nCurrentIndex / m_nTotalRecCount)*100;
		return;
	}
	UGBoundingBox bboxMerged;
	UGRenderOperationGroup* pGroup  = new UGRenderOperationGroup();
	// 把四分的子节点，合并成一层
	for(UGuint i=0; i<nSize; i++)
	{
		OBJLodQuarterTreeNode* pChildNode = pNode->childs[i];

		//填充 pGeodeObj
		UGString strObjFileName = pChildNode->strFileName;

		UGString strFullPath = UGFile::GetAbsolutePath(UGFile::GetDir(strConfigJson), strObjFileName);
		UGRenderOperationGroup* pGroupObj = GetRO(strFullPath);
		if(pGroupObj == NULL)
		{
			continue;
		}

		UGRenderOperationPagedLOD* pPagedLOD = new UGRenderOperationPagedLOD();
		UGRenderOperationGeode* pGeodeObj = new UGRenderOperationGeode();
		pGroup->AddChild(pPagedLOD);
		pPagedLOD->AddChild(pGeodeObj);

		AppendRO(pGroupObj, pGroup, pGeodeObj);

		// 包围盒
		pGeodeObj->ComputerBoundingBox();
		UGBoundingBox bboxObj = pGeodeObj->GetBoundingBox();
		bboxMerged.Merge(bboxObj);
		UGBoundingSphere bSphere(bboxObj);
		pGeodeObj->SetBoudingSphere(bSphere.GetCenter(), bSphere.GetRadius());

		//对应的切换文件
		UGString strLodFile = UGFile::ChangeExt(pNode->childs[i]->strFileName, _U(".s3mb"));
		pPagedLOD->GetFileNames().push_back(strLodFile);
		pPagedLOD->SetRangeMode(UGRangeMode::PIXEL_SIZE_ON_SCREEN);
		// 计算LOD变换的距离
		UGdouble nLodDis = UGMIN(1024, bSphere.m_radius * pow(2.0, (nLodNum - pNode->nLod - 1)));
		pPagedLOD->GetRanges().push_back(std::vector<std::pair<UGfloat, UGfloat> >::value_type(0, nLodDis));
		pPagedLOD->SetBoudingSphere(bSphere.m_center, bSphere.m_radius);
	}

	if(pGroup->GetNumChildren() == 0)
	{
		return;
	}

	// 清空精细层的patch
	pNode->boundBox = bboxMerged;

	UGString strS3MFilePath = UGFile::GetAbsolutePath(strOutputFilePath, pNode->strFileName);
	strS3MFilePath = UGFile::ChangeExt(strS3MFilePath, _U(".s3mb"));
	WriteRO(pGroup, strS3MFilePath);
	delete pGroup;
	pGroup = NULL;

	m_nCurrentIndex++;
	m_nPrecent = (m_nCurrentIndex / m_nTotalRecCount)*100;

	//处理子节点
	for(UGuint i=0; i<nSize; i++)
	{
		Convert2RO(pNode->childs[i]);
	}
}

void AltizureToS3MB::MakeQuarterTree()
{	
	GetQuarterChild(0, m_pTreeNode, 0, nTileLength, 0, nTileLength);
}

AltizureToS3MB::OBJLodQuarterTreeNode* AltizureToS3MB::CreateQTreeNode(UGuint nLod, UGuint nRow, UGuint nCol)
{
	UGString strTileName;
	strTileName.Format(_U("tile_%d_%d_%d_tex"),nLod, nCol, nRow);
	OBJLodQuarterTreeNode* pChildNodeOne = new OBJLodQuarterTreeNode;
	UGString strObjFileName = UGFile::ChangeExt(strTileName, _U(".obj"));
	pChildNodeOne->strFileName = strObjFileName;
	pChildNodeOne->nLod = nLod;
	pChildNodeOne->nCol = nCol;
	pChildNodeOne->nRow = nRow;

	return pChildNodeOne;
}

void AltizureToS3MB::deleteQTreeNode(OBJLodQuarterTreeNode* pNode)
{
	UGuint nSize= pNode->childs.size();
	for(UGuint i=0; i<nSize; i++)
	{
		deleteQTreeNode(pNode->childs[i]);
	}

	delete pNode;
}

void AltizureToS3MB::GetQuarterChild(UGuint nLod,OBJLodQuarterTreeNode* pNode,UGuint nMinRow, UGuint nMaxRow,UGuint nMinCol,UGuint nMaxCol)
{
	UGuint nCol = nMinCol,nRow = nMinRow;
	UGuint nD = (nMaxRow - nMinRow) / 2;

	if(pNode == NULL && m_pTreeNode == NULL)
	{
		m_pTreeNode = CreateQTreeNode(nLod, nMinRow,nMinCol);
		pNode = m_pTreeNode;

		m_nCurrentIndex++;
		m_nPrecent = (m_nCurrentIndex / m_nTotalRecCount)*100;
	}

	if(pNode == NULL)
	{
		return;
	}

	for(UGuint i=0; i<2; i++)
	{
		nRow += i*nD;
		nCol = nMinCol;
		for(UGuint j=0; j<2; j++)
		{
			nCol += j*nD;
			OBJLodQuarterTreeNode* pChild = CreateQTreeNode(nLod + 1, nRow, nCol);
			pNode->childs.push_back(pChild);

			m_nCurrentIndex++;
			m_nPrecent = (m_nCurrentIndex / m_nTotalRecCount)*100;
			
			UGuint n = nLod + 1;
			if(n < nLodNum)
			{
				GetQuarterChild(n, pChild, nRow, nRow+nD, nCol, nCol+nD);
			}				
		}

	}
}
