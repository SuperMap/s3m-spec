#include <iostream>
#include <fstream>
#include "S3MBReader.h"
#include "S3MBTools.h"
#include <wchar.h>
#include <fcntl.h>
#include "Skeleton.h"
#include <assert.h>
#include <io.h>
#include "S3MBLayerInfos.h"
#include "S3mbVariant.h"

using namespace std;

void S3MBReader::SetFilePath(const string & strFile)
{
	if (strFile.find(':') == string::npos)
	{
		m_strFilePath = StringHelper::GetAbsolutePath(strFile);
	}
	else
		m_strFilePath = strFile;
}

bool S3MBReader::OpenFileS3MB()
{
	//
	//setlocale(LC_ALL,"zh-CN.UTF-8");

	std::ifstream ifs(m_strFilePath, ios::in | ios::binary);
	if (!ifs)
	{
		return false;
	}
	LoadFileS3MB(ifs);
	ifs.close();
	return true;
}

bool S3MBReader::LoadFileS3MB(ifstream &stream)
{
	//判断版本
	float fVersion = 0;
	//stream >> fVersion;
	stream.read((char*)&fVersion, sizeof(float));
	if (fVersion == 1.0)
	{
		assert(m_pROGroup == NULL);
		if (m_pROGroup != NULL)
		{
			delete m_pROGroup;
			m_pROGroup = NULL;
		}
		m_pROGroup = new RenderOperationGroup();
		MemoryStream streamUnZipped;
		MemoryStream streamShell, streamSkeleton, streamSecondColor, streamTexture;
		MemoryStream streamSelectionInfo;
		string strJsonMaterials;
		S3MBTools::LoadStreamDataFromFile(stream, streamUnZipped, streamShell, streamSkeleton, streamSecondColor, streamTexture, streamSelectionInfo, strJsonMaterials);

		std::map<string, Geometry*> &mapGeometry = m_pROGroup->GetGeometry();
		S3MBTools::LoadSkeleton(streamSkeleton, mapGeometry);

		std::map<string, Material*>& mapMaterial = m_pROGroup->GetMaterials();
		S3MBTools::LoadMaterial(strJsonMaterials, mapMaterial);

		std::map<string, TextureDataInfo*>& mapTexture = m_pROGroup->GetTextureData();
		S3MBTools::LoadTexture(streamTexture, mapTexture);

		S3MBTools::LoadShell(streamShell, m_pROGroup);

		streamUnZipped.Clear();
	}
	return true;
}

void S3MBReader::ReadS3MBAsROGroup()
{
	if (!OpenFileS3MB())
	{
		return;
	}
	int iNum = m_pROGroup->GetGeometry().size();
	std::cout << "Geometry的个数：" << iNum << std::endl;
	for (int i = 0; i < iNum; i++)
	{
		std::cout << "第" << iNum << "个Children的 Geometry - Material - Texture 信息如下：" << std::endl;
		int nGeometry = 0;
		int nMaterial3D = 0;
		int nTexture = 0;

		//获得Geometry
		std::map<string, Geometry*>::iterator itorGeometry = m_pROGroup->GetGeometry().begin();
		for (; itorGeometry != m_pROGroup->GetGeometry().end(); itorGeometry++)
		{
			Geometry* pGeometry = itorGeometry->second;
			if (pGeometry != NULL)
			{
				nGeometry++;
				std::cout << "这是第" << nGeometry << "个Geometry." << std::endl;
				pGeometry->m_strGeoName = pGeometry->m_strGeoName;
				std::cout << "GeometryName：" << pGeometry->m_strGeoName << std::endl;

				std::cout << "MaterialName：" << pGeometry->m_strMaterialName << std::endl;

				VertexDataPackage* m_pVertexDataPackage = pGeometry->m_pVertexDataPackage;
				int nVerticesCount = m_pVertexDataPackage->m_nVerticesCount;
				std::cout << "顶点：" << nVerticesCount << " 个" << std::endl;
				for (int i = 0; i < nVerticesCount; i++)
				{
					std::cout << m_pVertexDataPackage->m_pVertices[i] << ",";
					std::cout << m_pVertexDataPackage->m_pVertices[i + 1] << ",";
					std::cout << m_pVertexDataPackage->m_pVertices[i + 2] << "   ";
				}
				cout << endl;
				cout << "UV: " << m_pVertexDataPackage->m_TexCoordCount[0] << " 个" << endl;
				for (int i = 0; i < m_pVertexDataPackage->m_TexCoordCount[0]; i++)
				{
					cout << m_pVertexDataPackage->m_pTexCoords[0][i] << ",";
					cout << m_pVertexDataPackage->m_pTexCoords[0][i + 1] << "   ";
				}
				cout << endl;
				bool bUseUint;//是否用unsigned int指针存储索引
				bool bHasTexIndex;//是否有纹理索引
				unsigned int nIndexCount;//顶点索引的实际大小
				switch (pGeometry->m_arrIndexPackage[0]->m_enIndexType)
				{
				case IT_16BIT:
					bUseUint = false;
					bHasTexIndex = false;
					nIndexCount = pGeometry->m_arrIndexPackage[0]->m_nIndexesCount;
					break;
				case IT_32BIT:
					bUseUint = true;
					bHasTexIndex = false;
					nIndexCount = pGeometry->m_arrIndexPackage[0]->m_nIndexesCount;
					break;
				case IT_16BIT_2:
					bUseUint = false;
					bHasTexIndex = true;
					nIndexCount = pGeometry->m_arrIndexPackage[0]->m_nIndexesCount / 2;
					break;
				case IT_32BIT_2:
					bUseUint = true;
					bHasTexIndex = true;
					nIndexCount = pGeometry->m_arrIndexPackage[0]->m_nIndexesCount / 2;
					break;
				}
				cout << "索引：" << nIndexCount << " 个" << endl;
				void* pIndexes = pGeometry->m_arrIndexPackage[0]->m_pIndexes;
				int i = 0;
				for (; i < nIndexCount; i++)
				{
					if (bUseUint)
					{
						cout << ((unsigned int*)pIndexes)[i];
					}
					else
					{
						cout << ((unsigned short*)pIndexes)[i];
					}
					if ((i + 1) % 3 == 0)
					{
						cout << "   ";
					}
					else
					{
						cout << ",";
					}
				}
				cout << endl;
				if (bHasTexIndex)
				{
					cout << "纹理索引：" << nIndexCount << " 个" << endl;
					for (; i < nIndexCount * 2; i++)
					{
						if (bUseUint)
						{
							cout << ((unsigned int*)pIndexes)[i];
						}
						else
						{
							cout << ((unsigned short*)pIndexes)[i];
						}
						if ((i + 1) % 3 == 0)
						{
							cout << "   ";
						}
						else
						{
							cout << ",";
						}
					}
				}
				else
				{
					cout << "纹理索引：0 个" << endl;
				}
				cout << endl;
				cout << endl;
				delete pGeometry;//释放内存
				pGeometry = NULL;
			}
		}
		std::cout << "=============================" << std::endl;
		system("pause");
	}
}

void S3MBReader::ReadS3MBAttribute()
{
	setlocale(LC_ALL, "zh-CN.UTF-8");
	//1：首先根据文件夹路径+属性json文件，先进行json文件的解析
	string strJson = m_strFilePath + "\\attribute.json";//_U("D:\\S3MB\\Building@TEST\\attribute.json");
	std::vector<LayerCacheInfo> vecLayerInfos;
	S3MBLayerInfos::LoadLayerInfoFromJson(strJson, vecLayerInfos);
	std::vector<LayerCacheInfo>::const_iterator itorLayerCaheInfo;
	int nMapFeatureMax = -1;
	int nMapFeatureMin = 9999;
	for (itorLayerCaheInfo = vecLayerInfos.begin(); itorLayerCaheInfo != vecLayerInfos.end(); itorLayerCaheInfo++)
	{
		std::pair<int, int> pairIDRange = itorLayerCaheInfo->m_pairIDRange;
		S3mbFieldInfos s3mbLayerInfo = itorLayerCaheInfo->m_fieldInfos;
		if (pairIDRange.second > nMapFeatureMax)
		{
			nMapFeatureMax = pairIDRange.second;
		}
		if (pairIDRange.first < nMapFeatureMin)
		{
			nMapFeatureMin = pairIDRange.first;
		}
	}
	std::map<unsigned int, Feature*> mapFeature;
	for (int i = nMapFeatureMin; i <= nMapFeatureMax; i++)
	{
		mapFeature[i] = NULL;
	}
	//2：然后根据文件夹路径找到各个文件夹下的s3md文件
	std::string strS3MRootPath = m_strFilePath;//"D:\\S3MB\\Building@TEST";
	vector<std::string> fifileNames;
	vector<std::string>::iterator iter;
	//3：获取所有的s3md文件
	GetFiles(strS3MRootPath, fifileNames);

	for (iter = fifileNames.begin(); iter != fifileNames.end(); iter++)
	{
		bool bflag = S3MBLayerInfos::LoadAttributeDataFromFile(*iter, mapFeature);
	}
	//4：对所有s3md文件进行遍历，获取属性值后输出
	int MaxCount = 0;
	for (std::map<unsigned int, Feature*>::iterator iter = mapFeature.begin(); iter != mapFeature.end(); iter++)
	{
		std::cout << "=============================" << std::endl;
		MaxCount++;
		//根据需要输出一定数量的属性信息，本示例输出5组信息
		if (MaxCount > 5)
		{
			break;
		}
		Feature* pFeature = iter->second;
		std::cout << "FeatureID：" << pFeature->m_nID << std::endl;
		vector<Feature::FieldDefine>* arrayFieldDefines = &(pFeature->m_fieldDefines);

		int nSize = arrayFieldDefines->size();
		for (int i = 0; i < nSize; i++)
		{
			Feature::FieldDefine ugFileDefine = arrayFieldDefines->at(i);
			string strFieldName = ugFileDefine.m_strName;
			S3mbVariant vtValule;
			//只需要SmID字段，其他的Sm字段不需要
			if (strFieldName.find("SmID") == -1 && strFieldName.find("Sm") != -1)
			{
				continue;
			}
			pFeature->GetValue(strFieldName, vtValule);
			string strFieldValue = vtValule.ToString();

			std::cout << strFieldName << ":" << strFieldValue << std::endl;
		}
		std::cout << "=============================" << std::endl;
	}
	system("pause");
}

void S3MBReader::GetFiles(const std::string& path, std::vector<std::string>& files)
{
	//文件句柄
	intptr_t hFile = 0;
	//文件信息
	struct _finddata_t fileinfo;
	std::string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			//如果是目录继续迭代
			//如果不是目录，加入文件列表
			if (fileinfo.attrib&_A_SUBDIR)
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
				{
					GetFiles(p.assign(path).append("\\").append(fileinfo.name), files);
				}
			}
			else
			{
				char *s3mattribute = "s3md";
				if (strstr(fileinfo.name, s3mattribute))
				{
					files.push_back(p.assign(path).append("\\").append(fileinfo.name));
				}
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}
