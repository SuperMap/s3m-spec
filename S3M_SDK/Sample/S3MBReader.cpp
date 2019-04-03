#include <iostream>
#include <vector>
#include"S3MBReader.h"

S3MBReader::S3MBReader()
{
}

S3MBReader::~S3MBReader()
{
}

void S3MBReader::SetFilePath(const UGString & strFile)
{
	m_strFilePath = strFile;
}
void S3MBReader::GetFiles(const std::string& path,std::vector<std::string>& files)
{
	//文件句柄
	long hFile = 0;
	//文件信息
	struct _finddata_t fileinfo;
	std::string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(),&fileinfo))!=-1)
	{
		do 
		{
			//如果是目录继续迭代
			//如果不是目录，加入文件列表
			if (fileinfo.attrib&_A_SUBDIR)
			{
				if (strcmp(fileinfo.name,".")!=0 && strcmp(fileinfo.name,"..")!=0)
				{
					GetFiles(p.assign(path).append("\\").append(fileinfo.name),files);
				}
			} 
			else
			{
				//files.push_back(p.assign(path).append("\\").append(fileinfo.name));
				char *s3mattribute = "s3md";
				if (strstr(fileinfo.name,s3mattribute))
				{
					files.push_back(p.assign(path).append("\\").append(fileinfo.name));
				}	
			}
		} 
		while (_findnext(hFile,&fileinfo)==0);
		_findclose(hFile);
	}
}

void S3MBReader::ReadS3MBAsROGroup()
{
	UGint nFileType = UGFileType::S3MB;

	UGRenderOperationGroup* pGroup = NULL;
	UGString strFileSrc = m_strFilePath;// _U("D:\\S3MB\\Building@TEST\\Tile_-3640_10592_0000\\Tile_-3640_10592_0000.s3mb");

	UGFileParseModel* pFileParserSrc = (UGFileParseModel*)UGFileParseManager::CreateFileParser(nFileType);
	pFileParserSrc->SetParseModelNode(FALSE);
	pFileParserSrc->SetIgnoreLod(FALSE);

	UGModelImportParams paramsImport;
	paramsImport.SetFilePathName(strFileSrc);
	paramsImport.SetImportMode(UGImportParams::ModeGIS);
	if(!pFileParserSrc->Open(paramsImport))
	{
		return ;
	}
	UGModelNode* pModelNode = pFileParserSrc->GetModelNode();

	pFileParserSrc->GetRenderOperationGroup(pGroup);
	UGint iNum = pGroup->GetNumChildren();
	std::cout<<"RenderOperationGroup的Children个数："<<iNum<<std::endl;
	for (int i = 0;i< iNum ; i++)
	{	
		std::cout<<"第"<<iNum<<"个Children的 Geometry - Material - Texture 信息如下："<<std::endl;
		int nGeometry=0;
		int nMaterial3D=0;
		int nTexture =0;
		UGRenderOperationNode* iRONode = pGroup->GetChild(iNum);

		std::map<UGString,UGRenderOperationGeometry*> &mapGeometry = pGroup->GetGeometry();
		std::map<UGString,UGMaterial3D*>& mapMaterial = pGroup->GetMaterial3Ds();
		std::map<UGString,UGRenderOperationNode::TextureDataInfo*>& mapTexture = pGroup->GetTextureData();

		//获得Geometry
		std::map<UGString, UGRenderOperationGeometry*>::iterator itorGeometry = mapGeometry.begin();
		for(itorGeometry = mapGeometry.begin(); itorGeometry != mapGeometry.end(); itorGeometry++)
		{
			UGRenderOperationGeometry* pGeometry = itorGeometry->second;

			if(pGeometry != NULL)
			{
				nGeometry++;
				UGString strGeoName = pGeometry->m_strGeoName;
				std::cout<<"这是第"<<nGeometry<<"个Geometry."<<std::endl;
				std::string stdStrGeoName;
				strGeoName.ToStd(stdStrGeoName);
				std::cout<<"GeometryName："<<stdStrGeoName<<std::endl;

				UGString strMaterialName = pGeometry->m_strMaterialName;
				std::string stdstrMaterialName;
				stdstrMaterialName = strMaterialName.ToStd(stdstrMaterialName);
				std::cout<<"MaterialName："<<stdstrMaterialName<<std::endl;

				UGVertexDataPackage* m_pVertexDataPackage = pGeometry->m_pVertexDataPackage;
				UGint nVerticesCount =  m_pVertexDataPackage->m_nVerticesCount;
				std::cout<<"GeometryVerticesCount："<<nVerticesCount<<std::endl;
			}
		}
		std::cout<<"GeometryCount："<<nGeometry<<std::endl;
		std::cout<<"============================="<<std::endl;
		//获得Material3D
		std::map<UGString, UGMaterial3D*>::iterator itorMaterial3D = mapMaterial.begin();
		for(itorMaterial3D = mapMaterial.begin(); itorMaterial3D != mapMaterial.end(); itorMaterial3D++)
		{
			UGMaterial3D* pMaterial3D = itorMaterial3D->second;
			if(pMaterial3D != NULL)
			{
				nMaterial3D++;
				UGString strMaterialName = pMaterial3D->GetMaterialName();
				std::cout<<"这是第"<<nMaterial3D<<"个Material3D."<<std::endl;
				std::string stdStrMaterialName;
				strMaterialName.ToStd(stdStrMaterialName);
				std::cout<<"Material3DName："<<stdStrMaterialName<<std::endl;
			}

		}
		std::cout<<"Material3DCount："<<nMaterial3D<<std::endl;
		std::cout<<"============================="<<std::endl;

		//获得Material3D
		std::map<UGString,UGRenderOperationNode::TextureDataInfo*>::iterator itorTexture = mapTexture.begin();
		for(itorTexture = mapTexture.begin(); itorTexture != mapTexture.end(); itorTexture++)
		{
			UGRenderOperationNode::TextureDataInfo* pTexture = itorTexture->second;
			if(pTexture != NULL)
			{
				nTexture++;
				UGint nTextureWith = pTexture->m_pTextureData->m_nWidth;
				UGint nTextureHeight = pTexture->m_pTextureData->m_nHeight;
				UGint nTextureDepth = pTexture->m_pTextureData->m_nDepth;
				std::cout<<"这是第"<<nTexture<<"个Texture."<<std::endl;
				std::cout<<"TextureWith："<<nTextureWith<<std::endl;
				std::cout<<"TextureHeight："<<nTextureHeight<<std::endl;
				std::cout<<"TextureDepth："<<nTextureDepth<<std::endl;
			}
		}
		std::cout<<"TextureCount："<<nTexture<<std::endl;
		std::cout<<"============================="<<std::endl;
	}
	system("pause");
}

void S3MBReader::ReadS3MBAttribute()
{
	//1：首先根据文件夹路径+属性json文件，先进行json文件的解析
	UGString strJson = m_strFilePath + _U("\\attribute.json") ;//_U("D:\\S3MB\\Building@TEST\\attribute.json");
	std::vector<UGLayerCacheInfo> vecLayerInfos;
	UGS3MBLayerInfos::LoadLayerInfoFromJson(strJson,vecLayerInfos);
	std::vector<UGLayerCacheInfo>::iterator itorLayerCaheInfo = vecLayerInfos.begin();
	UGint nMapFeatureMax = -1;
	UGint nMapFeatureMin =9999;
	for(itorLayerCaheInfo = vecLayerInfos.begin(); itorLayerCaheInfo != vecLayerInfos.end(); itorLayerCaheInfo++)
	{
		UGLayerCacheInfo layerCacheInfo = *itorLayerCaheInfo;
		OgdcFieldInfos ogdcLayerInfo = layerCacheInfo.m_fieldInfos;
		std::pair<UGint, UGint> pairIDRange = layerCacheInfo.m_pairIDRange;
		if (pairIDRange.second > nMapFeatureMax)
		{
			nMapFeatureMax = pairIDRange.second;
		}
		if (pairIDRange.first <nMapFeatureMin)
		{
			nMapFeatureMin = pairIDRange.first;
		}
	}
	std::map<UGuint, UGFeature*> mapFeature;
	for (UGint i=nMapFeatureMin;i<=nMapFeatureMax;i++)
	{
		mapFeature[i] = NULL;
	}
	//2：然后根据文件夹路径找到各个文件夹下的s3md文件
	std::string strFilePath;
	m_strFilePath.ToStd(strFilePath);
	std::string strS3MRootPath =strFilePath;//"D:\\S3MB\\Building@TEST";
	vector<std::string> fifileNames;
	vector<std::string>::iterator iter;
	//3：获取所有的s3md文件
	GetFiles(strS3MRootPath,fifileNames);

	for (iter = fifileNames.begin();iter!=fifileNames.end();iter++)
	{
		UGString ugFileName;
		ugFileName.FromStd(*iter);
		UGbool bflag = UGS3MBLayerInfos::LoadAttributeDataFromJson(ugFileName,mapFeature);
	}
	//4：对所有s3md文件进行遍历，获取属性值后输出
	int MaxCount = 0;
	for (std::map<UGuint, UGFeature*>::iterator iter = mapFeature.begin();iter!=mapFeature.end();iter++)
	{
		std::cout<<"============================="<<std::endl;
		MaxCount++;
		//根据需要输出一定数量的属性信息，本示例输出5组信息
		if (MaxCount>5)
		{
			break;
		}
		UGFeature* pFeature =iter->second;
		std::cout<<"FeatureID："<<pFeature->GetID()<<std::endl;
		UGArray<UGFieldDefine>* arrayFieldDefines = pFeature->GetFieldDefines();

		UGint nSize = arrayFieldDefines->GetSize();
		for (UGint i=0; i<nSize; i++)
		{
			UGFieldDefine ugFileDefine = arrayFieldDefines->GetAt(i);
			UGString strFieldName = ugFileDefine.m_strName;
			UGVariant vtValule;
			//只需要SmID字段，其他的Sm字段不需要
			if (strFieldName.Find(_U("SmID"))!=-1)
			{
				pFeature->GetValue(strFieldName,vtValule);
				UGString strFieldValue = vtValule.ToString();

				std::string stdstrFieldName;
				stdstrFieldName = strFieldName.ToStd(stdstrFieldName);

				std::string stdstrFieldValue;
				stdstrFieldValue = strFieldValue.ToStd(stdstrFieldValue);

				std::cout<<stdstrFieldName<<":"<<stdstrFieldValue<<std::endl;
			}
			if(strFieldName.Find(_U("Sm"))!=-1)
			{
				continue;
			}
			pFeature->GetValue(strFieldName,vtValule);
			UGString strFieldValue = vtValule.ToString();

			std::string stdstrFieldName;
			stdstrFieldName = strFieldName.ToStd(stdstrFieldName);

			std::string stdstrFieldValue;
			stdstrFieldValue = strFieldValue.ToStd(stdstrFieldValue,UGString::Default);

			std::cout<<stdstrFieldName<<":"<<stdstrFieldValue<<std::endl;
		}
		std::cout<<"============================="<<std::endl;
	}
	system("pause");
}
