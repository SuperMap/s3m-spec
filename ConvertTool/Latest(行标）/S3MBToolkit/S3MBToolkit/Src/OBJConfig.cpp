#include "OBJConfig.h"
#include "JsonValue.h"
#include "S3MBUtils.h"
#include "Utils/Utils.h"

#include <osg/ref_ptr>
#include <osgDB/ReadFile>

namespace S3MB
{
	OBJConfig::OBJConfig()
	{
		m_bHasSrs = false;
		m_matModelTran = Matrix4d::IDENTITY;
	}

	OBJConfig::~OBJConfig()
	{

	}

	bool OBJConfig::LoadFromFile(const std::wstring& strPath)
	{
		JsonValue jsonConfig;
		if (!jsonConfig.LoadFromFile(strPath))
		{
			return false;
		}

		std::wstring strSrs;
		if (jsonConfig.GetValue(U("srs"), strSrs))
		{
			std::wstring strENU = strSrs.substr(strSrs.find_first_of(':') + 1);

			std::vector<std::wstring> arrSplit;
			StringUtil::Split(strENU, arrSplit, ',');
			if (arrSplit.size() > 1)
			{
				m_pntPostion.x = stod(arrSplit[1], nullptr);
				m_pntPostion.y = stod(arrSplit[0], nullptr);
				if (arrSplit.size() > 2)
				{
					m_pntPostion.z = stod(arrSplit[2], nullptr);
				}
				m_bHasSrs = true;
			}
		}

		JsonValue* pModelTransform = nullptr;
		if (jsonConfig.GetValue(U("model_transform"), pModelTransform) &&
			pModelTransform->GetArraySize() > 0)
		{
			// ×ª»¯¾ØÕó
			if (m_bHasSrs)
			{
				double* m = m_matModelTran.GetPointer();
				for (int i = 0; i < 3; i++)
				{
					JsonValue element(arrayValue);
					if (pModelTransform->GetAt(i, &element))
					{
						for (int j = 0; j < 3; j++)
						{
							element.GetAt(j, m[4 * i + j]);
						}

						element.GetAt(3, m[4 * 3 + i]);
					}
				}
			}
			else
			{
				double* tmp = new double[3];
				for (int i = 0; i < 3; i++)
				{
					JsonValue element(arrayValue);
					if (pModelTransform->GetAt(i, &element))
					{
						element.GetAt(3, tmp[i]);
					}
				}
				m_pntPostion.x = tmp[0];
				m_pntPostion.y = tmp[1];
				m_pntPostion.z = tmp[2];
			}
		}
		delete pModelTransform;
		pModelTransform = nullptr;

		JsonValue* pTile = nullptr;
		if (jsonConfig.GetValue(U("tile"), pTile) &&
			pTile->GetArraySize() > 0)
		{
			pTile->GetAt(0, m_nTileLength);
			m_nLodNum = get2n(m_nTileLength);
		}
		delete pTile;
		pTile = nullptr;

		return true;
	}

	const Point3D& OBJConfig::GetPosition() const
	{
		return m_pntPostion;
	}

	bool OBJConfig::GetHasSrs() const
	{
		return m_bHasSrs;
	}

	const Matrix4d& OBJConfig::GetModelTransform() const
	{
		return m_matModelTran;
	}

	int OBJConfig::GetLodNum() const
	{
		return m_nLodNum;
	}

	int OBJConfig::GetTileLength() const
	{
		return m_nTileLength;
	}
}