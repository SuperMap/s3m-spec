#include "Skeleton.h"
#include "S3MBCommon.h"
#include "zlib/include/zconf.h"
#include <assert.h>
#include <math.h>
#include <map>


namespace S3MB
{
	IndexPackage::IndexPackage()
		: m_nIndexesCount(0)
		, m_pIndexes(NULL)
		, m_enIndexType(IT_16BIT)
		, m_bUseIndex(true)
		, m_OperationType(OT_TRIANGLE_LIST)
	{
	}

	IndexPackage::~IndexPackage()
	{
		if (m_pIndexes != NULL)
		{
			delete[] m_pIndexes;
			m_pIndexes = NULL;
		}
		m_nIndexesCount = 0;
	}

	bool IndexPackage::Load(MemoryStream& stream)
	{
		unsigned char nByte = 0;
		int nCount = 0;
		stream >> nCount;
		stream >> (unsigned char&)m_enIndexType;
		stream >> m_bUseIndex;
		stream >> (unsigned char&)m_OperationType;
		stream >> nByte;

		if (nCount > 0)
		{
			m_nIndexesCount = nCount;
			if (m_enIndexType == IT_32BIT)
			{
				m_pIndexes = new(std::nothrow) unsigned short[m_nIndexesCount * 2];
				if (m_pIndexes == NULL)
				{
					return false;
				}
				stream.Load(m_pIndexes, m_nIndexesCount * 2);
			}
			else
			{
				m_pIndexes = new(std::nothrow) unsigned short[m_nIndexesCount];
				if (m_pIndexes == NULL)
				{
					return false;
				}
				stream.Load(m_pIndexes, m_nIndexesCount);
				if (m_nIndexesCount % 2 != 0)
				{
					stream >> nByte;
					stream >> nByte;
				}
			}
		}

		int i = 0;
		stream >> nCount;

		for (i = 0; i < nCount; i++)
		{
			wstring strName;
			stream >> strName;
			m_strPassName.push_back(strName);
		}
		// 四字节对齐
		int nPos = stream.GetReadPosition();
		if (nPos % 4 != 0)
		{
			int nReserved = 4 - nPos % 4;
			for (int j = 0; j < nReserved; j++)
			{
				stream >> nByte;
			}
		}
		return true;
	}

	void IndexPackage::Save(MemoryStream& stream)
	{
		unsigned char nByte = 0;
		stream << m_nIndexesCount;

		stream << (unsigned char)m_enIndexType;
		stream << m_bUseIndex;
		stream << (unsigned char)m_OperationType;
		stream << nByte;

		if (m_nIndexesCount > 0 && m_pIndexes != NULL)
		{
			if (m_enIndexType == IT_32BIT)
			{
				stream.Save(m_pIndexes, m_nIndexesCount * 2);
			}
			else
			{
				stream.Save(m_pIndexes, m_nIndexesCount);
				if (m_nIndexesCount % 2 != 0)
				{
					stream << nByte;
					stream << nByte;
				}
			}
		}

		int nCount = m_strPassName.size();
		stream << nCount;
		for (int i = 0; i < nCount; i++)
		{
			wstring strName = m_strPassName[i];
			stream << strName;
		}
		// 四字节对齐
		int nPos = stream.GetWritePosition();
		if (nPos % 4 != 0)
		{
			int nReserved = 4 - nPos % 4;
			for (int j = 0; j < nReserved; j++)
			{
				stream << nByte;
			}
		}
	}

	void IndexPackage::SetIndexNum(int nIndexNum)
	{
		m_nIndexesCount = nIndexNum;
		if (m_pIndexes != NULL)
		{
			delete[] m_pIndexes;
			m_pIndexes = NULL;
		}
		if (m_enIndexType == IT_16BIT)
		{
			m_pIndexes = new unsigned short[nIndexNum];
			memset(m_pIndexes, 0, sizeof(unsigned short) * nIndexNum);
		}
		else
		{
			m_pIndexes = (unsigned short*)new unsigned int[nIndexNum];
			memset(m_pIndexes, 0, sizeof(unsigned int) * nIndexNum);
		}
	}

	IDInfo::IDInfo()
		:m_nID(0)
	{
	};

	IDInfo& IDInfo::operator=(const IDInfo& other)
	{
		m_nID = other.m_nID;
		m_arrInstanceIndex = other.m_arrInstanceIndex;
		m_arrVertexColorOffsetAndCount = other.m_arrVertexColorOffsetAndCount;
		return *this;
	}

	IDInfo::IDInfo(const IDInfo& other)
	{
		*this = other;
	}

	IDInfo::~IDInfo()
	{
	}

	Skeleton::Skeleton() :
		m_pVertexDataPackage(NULL),
		m_strMaterialName(U("")),
		m_strGeoName(U("")),
		m_bInstanceBatch(false) {}

	Skeleton::~Skeleton()
	{
		if (m_pVertexDataPackage != NULL)
		{
			delete m_pVertexDataPackage;
			m_pVertexDataPackage = NULL;
		}
		for (int i = 0; i < m_arrIndexPackage.size(); i++)
		{
			if (m_arrIndexPackage.at(i) != NULL)
			{
				delete m_arrIndexPackage.at(i);
				m_arrIndexPackage.at(i) = NULL;
			}
		}
		for (auto& pIDinfo : m_arrIDInfo)
		{
			if (pIDinfo != nullptr)
			{
				delete pIDinfo;
				pIDinfo = nullptr;
			}
		}
	}

	void Skeleton::ComputerBoundingBox()
	{
		if (m_pVertexDataPackage == NULL || m_pVertexDataPackage->m_nVerticesCount < 1)
		{
			return;
		}

		Vector3d vecMax, vecMin;
		vecMax.x = -DBLMAX;
		vecMax.y = -DBLMAX;
		vecMax.z = -DBLMAX;
		vecMin.x = DBLMAX;
		vecMin.y = DBLMAX;
		vecMin.z = DBLMAX;

		int nVertexDim = m_pVertexDataPackage->m_nVertexDimension;
		int nValidVertexDim = nVertexDim > 3 ? 3 : nVertexDim;
		short* pCompressVertex = (short*)m_pVertexDataPackage->m_pVertices;
		for (unsigned int i = 0; i < m_pVertexDataPackage->m_nVerticesCount; i++)
		{
			Vector3d vertChange;
			for (int j = 0; j < nValidVertexDim; j++)
			{
				if (m_pVertexDataPackage->m_nCompressOptions & SVC_Vertex)
				{
					short encodeVal = pCompressVertex[i * nVertexDim + j];
					vertChange[j] = encodeVal * m_pVertexDataPackage->m_fVertCompressConstant + m_pVertexDataPackage->m_minVerticesValue[j];
				}
				else
				{
					vertChange[j] = m_pVertexDataPackage->m_pVertices[i * nVertexDim + j];
				}
			}

			if (vertChange.x > vecMax.x) vecMax.x = vertChange.x;
			if (vertChange.y > vecMax.y) vecMax.y = vertChange.y;
			if (vertChange.z > vecMax.z) vecMax.z = vertChange.z;
			if (vertChange.x < vecMin.x) vecMin.x = vertChange.x;
			if (vertChange.y < vecMin.y) vecMin.y = vertChange.y;
			if (vertChange.z < vecMin.z) vecMin.z = vertChange.z;
		}
		if (m_pVertexDataPackage->m_vecInstanceInfo.size() > 0)
		{
			BoundingBox EntireBox;
			for (int iMat = 0; iMat < m_pVertexDataPackage->m_vecInstanceInfo.size(); iMat++)
			{
				float* pBuffer = m_pVertexDataPackage->m_vecInstanceInfo[iMat]->pMatrixValues;
				Matrix4d mat = Matrix4d::IDENTITY;
				for (int i = 0; i < 3; i++)
				{
					for (int j = 0; j < 4; j++)
						mat.m[j][i] = *pBuffer++;
				}
				BoundingBox tmpBox;
				tmpBox.SetExtents(vecMin.MultiplyMatrix(mat), vecMax.MultiplyMatrix(mat));
				EntireBox.Merge(tmpBox);
			}
			m_BoundingBox = EntireBox;
		}
		else
		{
			m_BoundingBox.SetExtents(vecMin, vecMax);
		}
		m_BoundingBox.ToOrientedBoundingBox(m_OBB);
	}


	bool Skeleton::SetInstanceInfoAndIDInfo(std::vector<Matrix4d>& vecMats, std::vector<unsigned int>& vecIDs)
	{
		if (vecMats.size() != vecIDs.size())
		{
			return false;
		}
		if (m_pVertexDataPackage == NULL || m_pVertexDataPackage->m_nVerticesCount < 1)
		{
			return false;
		}

		// 填充InstanceInfo
		for (int iInstance = 0; iInstance < vecMats.size(); iInstance++)
		{
			InstanceInfo* pInstanceInfo = new InstanceInfo;
			pInstanceInfo->nCount = 16;
			pInstanceInfo->pMatrixValues = new float[16];
			float* pBuffer = pInstanceInfo->pMatrixValues;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 4; j++)
					*pBuffer++ = static_cast<float>(vecMats[iInstance].m[j][i]);
			}
			*pBuffer++ = 1.0f;
			*pBuffer++ = 1.0f;
			*pBuffer++ = 1.0f;
			*pBuffer++ = 1.0f;
			m_pVertexDataPackage->m_vecInstanceInfo.push_back(pInstanceInfo);
		}

		// 填充IDInfo
		for (int iID = 0; iID < vecIDs.size(); iID++)
		{
			IDInfo* pIDifo = new IDInfo;
			pIDifo->m_nID = vecIDs[iID];
			pIDifo->m_arrInstanceIndex.push_back(iID);
			m_arrIDInfo.push_back(pIDifo);
		}

		return true;
	}


	void Skeleton::AddIDInfo(IDInfo* pDInfo)
	{
		m_arrIDInfo.push_back(pDInfo);
	}

	void Skeleton::SplitIndexPackageByIDInfo()
	{
		if (m_arrIDInfo.size() == 0 || m_arrIDInfo[0]->m_arrVertexColorOffsetAndCount.size() == 0)
		{
			return;
		}
		vector<IndexPackage*> totalPack;
		vector<IndexPackage*> remainPack;
		for (vector<IndexPackage*>::iterator itor = m_arrIndexPackage.begin(); itor != m_arrIndexPackage.end();itor++)
		{
			if ((*itor)->m_OperationType == OT_TRIANGLE_LIST)
			{
				totalPack.push_back(*itor);
			}
			else
			{
				remainPack.push_back(*itor);
			}
		}

		m_arrIndexPackage = remainPack;

		for (int i = 0; i < totalPack.size();i++)
		{		
			IndexPackage* pIndexPackage = totalPack[i];
			if (pIndexPackage == nullptr)
			{
				continue;
			}

			std::map<unsigned int, vector<unsigned int>> mapIdxData;
			for (int j = 0; j < totalPack[i]->m_nIndexesCount; j += 3)
			{
				unsigned int nIndex0 = pIndexPackage->m_pIndexes[j];
				unsigned int nIndex1 = pIndexPackage->m_pIndexes[j + 1];
				unsigned int nIndex2 = pIndexPackage->m_pIndexes[j + 2];

				if (pIndexPackage->m_enIndexType == IT_32BIT)
				{
					unsigned int* pIdx = (unsigned int*)(totalPack[i]->m_pIndexes);
					nIndex0 = pIdx[j];
					nIndex1 = pIdx[j + 1];
					nIndex2 = pIdx[j + 2];
				}

				for (int k = 0; k < m_arrIDInfo.size(); k++)
				{
					bool bFind = false;
					for (std::vector<std::pair<int, int>>::iterator itor = m_arrIDInfo[k]->m_arrVertexColorOffsetAndCount.begin();
						itor != m_arrIDInfo[k]->m_arrVertexColorOffsetAndCount.end(); itor++)
					{
						if (nIndex0 >= itor->first && nIndex0 < (itor->first + itor->second))
						{
							mapIdxData[m_arrIDInfo[k]->m_nID].push_back(nIndex0);
							mapIdxData[m_arrIDInfo[k]->m_nID].push_back(nIndex1);
							mapIdxData[m_arrIDInfo[k]->m_nID].push_back(nIndex2);
							bFind = true;
							break;
						}
					}
					if (bFind)
					{
						break;
					}
				}
			}

			for (std::map<unsigned int, vector<unsigned int>>::iterator itor = mapIdxData.begin(); itor != mapIdxData.end(); itor++)
			{
				unsigned int nIndexCount = itor->second.size();
				if (nIndexCount == 0)
				{
					continue;
				}
				IndexPackage* pIdxPack = new IndexPackage;
				pIdxPack->m_strPassName = pIndexPackage->m_strPassName;
				pIdxPack->m_enIndexType = pIndexPackage->m_enIndexType;
				pIdxPack->m_OperationType = pIndexPackage->m_OperationType;
				pIdxPack->m_bUseIndex = pIndexPackage->m_bUseIndex;
				pIdxPack->SetIndexNum(nIndexCount);
				vector<unsigned int>& vecIndex = itor->second;
				for (int j = 0; j < nIndexCount; j++)
				{
					if (pIdxPack->m_enIndexType == IT_16BIT)
					{
						pIdxPack->m_pIndexes[j] = vecIndex[j];
					}
					else
					{
						unsigned int* pIdx = (unsigned int*)pIdxPack->m_pIndexes;
						pIdx[j] = vecIndex[j];
					}
				}
				m_arrIndexPackage.push_back(pIdxPack);
			}

			if (pIndexPackage != nullptr)
			{
				delete pIndexPackage;
				pIndexPackage = nullptr;
			}

			mapIdxData.clear();
		}
	}
}