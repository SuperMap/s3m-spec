#include "S3MBReader.h"
#include "S3MBTools.h"
#include <assert.h>

namespace S3MB
{
	S3MBReader::S3MBReader()
	{
		m_pROGroup = nullptr;
	}

	S3MBReader::~S3MBReader()
	{
		Clear();
	}

	bool S3MBReader::LoadS3MBFromStream(void* buffer, unsigned int bufferSize)
	{
		MemoryStream stream;
		stream.Init(buffer, bufferSize);
		// 判断版本
		float fVersion = 0;
		stream >> fVersion;
		assert(m_pROGroup == NULL);
		if (m_pROGroup != NULL)
		{
			delete m_pROGroup;
			m_pROGroup = NULL;
		}
		m_pROGroup = new RenderOperationGroup();
		MemoryStream streamUnZipped;
		MemoryStream streamShell, streamSkeleton, streamTexture;
		MemoryStream streamIDInfo;
		wstring strJsonMaterials;

		S3MBTools::LoadStreamDataFromStream(stream, streamUnZipped, streamShell,
			streamSkeleton, streamTexture, streamIDInfo, strJsonMaterials, m_strExtensions, fVersion);		

		std::map<wstring, Skeleton*>& mapSkeleton = m_pROGroup->GetSkeleton();
		S3MBTools::LoadSkeleton(streamSkeleton, mapSkeleton, fVersion);

		std::map<wstring, Material*>& mapMaterial = m_pROGroup->GetMaterials();
		S3MBTools::LoadMaterial(strJsonMaterials, mapMaterial, fVersion);

		std::map<wstring, TextureDataInfo*>& mapTexture = m_pROGroup->GetTextureData();
		S3MBTools::LoadTexture(streamTexture, mapTexture,fVersion);

		S3MBTools::LoadShell(streamShell, m_pROGroup, fVersion);
		
		if (streamIDInfo.GetWritePosition() != 0)
		{
			S3MBTools::LoadIDInfo(mapSkeleton, streamIDInfo, fVersion);
		}

		return true;
	}

	void S3MBReader::OutputSkeletonInfoToConsole()
	{
		if (m_pROGroup == nullptr)
		{
			std::wcout <<U("获取信息为空！请先调用ReadS3MBAsROGroup()!")<< std::endl;
			return;
		}
		int iNum = m_pROGroup->GetSkeleton().size();
#ifdef WIN32
		wcout.imbue(std::locale("chs"));
#endif
        std::wcout << U("The Number of Skeleton: ")<< iNum << std::endl;
		for (int i = 0; i < iNum; i++)
		{
            std::wcout << U("The ") << i << U(" Children's (Skeleton - Material - Texture)'s info:" )<< std::endl;
			int nMaterial3D = 0;
			int nTexture = 0;

			// 获得Skeleton
			std::map<wstring, Skeleton*>::iterator itorSkeleton = m_pROGroup->GetSkeleton().begin();
			for (int j = 0;j<i;j++)
			{
				itorSkeleton++;
			}

			Skeleton* pSkeleton = itorSkeleton->second;
			if (pSkeleton != NULL)
			{
				pSkeleton->SplitIndexPackageByIDInfo();
                std::wcout << U("This is ")<< i << U(" Skeleton.")<< std::endl;
				pSkeleton->m_strGeoName = pSkeleton->m_strGeoName;
				std::wcout << U("SkeletonName：") << pSkeleton->m_strGeoName << std::endl;

				std::wcout << U("MaterialName：") << pSkeleton->m_strMaterialName << std::endl;

				VertexDataPackage* m_pVertexDataPackage = pSkeleton->m_pVertexDataPackage;
				int nVerticesCount = m_pVertexDataPackage->m_nVerticesCount;
				int nDim = m_pVertexDataPackage->m_nVertexDimension;
                std::wcout << U("Point Number: ") << nVerticesCount << U(" ") << std::endl;
                std::wcout << U("Point's Dimension: ") << nDim << endl;
				for (int i = 0; i < nVerticesCount; i++)
				{
					for (int j = 0; j < nDim; j++)
					{
						wcout << m_pVertexDataPackage->m_pVertices[i * nDim + j];
						if (j == nDim - 1)
						{
							wcout << U("    ");
						}
						else
						{
							wcout << U(",");
						}
					}
				}
				wcout << endl;

				for (int nUVIdx = 0; nUVIdx < 8; nUVIdx++)
				{
					if (m_pVertexDataPackage->m_TexCoordCount[nUVIdx] == 0)
					{
						break;
					}
                    wcout << U("The ") << nUVIdx + 1 << U(" UV:") << endl;
                    wcout << U("UV Number: ") << m_pVertexDataPackage->m_TexCoordCount[nUVIdx] << U(" ") << endl;
                    wcout << U("UV Dimension: ") << m_pVertexDataPackage->m_nTexDimensions[nUVIdx] << endl;
					for (int i = 0; i < m_pVertexDataPackage->m_TexCoordCount[nUVIdx]; i++)
					{
						int dim = m_pVertexDataPackage->m_nTexDimensions[nUVIdx];
						for (int j = 0; j < dim; j++)
						{
							if (dim == 17 && j == 16)
							{
								wcout << (unsigned int)m_pVertexDataPackage->m_pTexCoords[nUVIdx][i * dim + j];
							}
							else
							{
								wcout << m_pVertexDataPackage->m_pTexCoords[nUVIdx][i * dim + j];
							}
							if (j == dim - 1)
							{
								wcout << U("    ");
							}
							else
							{
								wcout << U(",");
							}
						}
					}
					wcout << endl;
				}
				wcout << endl;

                wcout << U("Normal: ") << endl;
                wcout << U("Normal Number: ") << m_pVertexDataPackage->m_nNormalCount << U(" ") << endl;
                wcout << U("Normal Dimension: ") << m_pVertexDataPackage->m_nNormalDimension << endl;
				for (int i = 0; i < m_pVertexDataPackage->m_nNormalCount; i++)
				{
					int dim = m_pVertexDataPackage->m_nNormalDimension;
					for (int j = 0; j < dim; j++)
					{
						{
							wcout << m_pVertexDataPackage->m_pNormals[i * dim + j];
						}
						if (j == dim - 1)
						{
							wcout << U("    ");
						}
						else
						{
							wcout << U(",");
						}
					}
				}
				wcout << endl;

				for (size_t iIdxCount = 0; iIdxCount < pSkeleton->m_arrIndexPackage.size(); iIdxCount++)
				{
					bool bUseUint;// 是否用unsigned int指针存储索引
					unsigned int nIndexCount = pSkeleton->m_arrIndexPackage[iIdxCount]->m_nIndexesCount;// 顶点索引的实际大小
					switch (pSkeleton->m_arrIndexPackage[iIdxCount]->m_enIndexType)
					{
					case IT_16BIT:
						bUseUint = false;
						break;
					case IT_32BIT:
						bUseUint = true;
						break;
					}
                    wcout <<U("The ") <<iIdxCount << U(" ") << U("Index Package, Index Number: ") << nIndexCount << U("  ") << endl;
					void* pIndexes = pSkeleton->m_arrIndexPackage[iIdxCount]->m_pIndexes;
					int i = 0;
					for (; i < nIndexCount; i++)
					{
						if (bUseUint)
						{
							wcout << ((unsigned int*)pIndexes)[i];
						}
						else
						{
							wcout << ((unsigned short*)pIndexes)[i];
						}
						if ((i + 1) % 3 == 0)
						{
							wcout << U("   ");
						}
						else
						{
							wcout << U(",");
						}
					}
					wcout << endl;
					wcout << endl;
				}
			}
			
			std::wcout << U("=============================") << std::endl;
		}
	}

	RenderOperationGroup* S3MBReader::GetRenderOperationGroup()
	{
		return m_pROGroup;
	}

	std::wstring S3MBReader::GetExtensions() const
	{
		return m_strExtensions;
	}

	void S3MBReader::Clear()
	{
		if (m_pROGroup != nullptr)
		{
			delete m_pROGroup;
			m_pROGroup = nullptr;
		}
	}

	void S3MBReader::ReadFile(const wstring& strFile)
	{
#ifdef WIN32
		if (strFile.find(L':') == wstring::npos)
		{
			m_strFilePath = StringUtil::GetAbsolutePath(strFile);
		}
		else {
			m_strFilePath = strFile;
		}
		std::ifstream ifs(m_strFilePath, ios::in | ios::binary);
#else
		if (strFile.substr(0, 1) == U("/") || strFile.substr(0, 1) == U("~")) {
			m_strFilePath = strFile;
		}
		else {
			m_strFilePath = StringUtil::GetAbsolutePath(strFile);
		}
		string filePath = StringUtil::UnicodeToANSI(m_strFilePath);
		std::ifstream ifs(filePath, ios::in | ios::binary);
#endif

		if (!ifs)
		{
			return;
		}
		ifs.seekg(0, ios::end);
		int size = ifs.tellg();
		ifs.seekg(0, ios::beg);
		char* buffer = new char[size];
		ifs.read(buffer, size);
		ifs.close();
		LoadS3MBFromStream(buffer, size);
		delete buffer;
		buffer = nullptr;
	}
}
