#include "Utils.h"
#include "S3MBUtils.h"
#include "ImageOperator.h"
#include "MathEngine.h"
#ifdef WIN32
#include <math.h>
#include <atlstr.h>
#include <codecvt>
#include <io.h>
#else
#include <cmath>
#include <dlfcn.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#endif

#if (!defined(__linux__))
#include <objbase.h>
#else
#if (__GNUC__ <= 4 && __GNUC_MINOR__ < 9)
#include <uuid/uuid.h>
#endif
#endif

namespace S3MB
{
	std::wstring BaseUtils::CreateGuid()
	{
		std::string strGUID;
#if (!defined(__linux__))
		GUID guid;
		if (S_OK == CoCreateGuid(&guid))
		{
			char buf[64] = { 0 };
			sprintf_s(buf, sizeof(buf), "%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X",
				guid.Data1, guid.Data2, guid.Data3,
				guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
				guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);
			strGUID = std::string(buf);
		}
#else
#if (__GNUC__ <= 4 && __GNUC_MINOR__ < 9)
		uuid_t guid;
		uuid_generate(guid);
		char buf[64] = { 0 };
		snprintf(buf, sizeof(buf), "%02X%02X%02X%02X-%02X%02X-%02X%02X-%02X%02X-%02X%02X%02X%02X%02X%02X",
			guid[0], guid[1], guid[2], guid[3],
			guid[4], guid[5], guid[6], guid[7],
			guid[8], guid[9], guid[10], guid[11],
			guid[12], guid[13], guid[14], guid[15]);
		strGUID = std::string(buf);
#endif
#endif

		return S3MB::StringUtil::UTF8_to_UNICODE(strGUID);
	}

	std::wstring BaseUtils::Base64Decode(const std::wstring& Data, int DataByte)
	{
		//解码表
		const wchar_t DecodeTable[] =
		{
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	//24
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,				//19
			62, // '+'																//1
			0, 0, 0,																//3
			63, // '/'																//5
			52, 53, 54, 55, 56, 57, 58, 59, 60, 61, // '0'-'9'						//10
			0, 0, 0, 0, 0, 0, 0,													//7
			0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,								//13
			13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, // 'A'-'Z'			//13
			0, 0, 0, 0, 0, 0,														//6
			26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,						//13
			39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, // 'a'-'z'			//13
		};
		//返回值
		std::wstring strDecode;
		int nValue;
		int i = 0;
		int j = 0;
		while (i < DataByte)
		{
			if (Data[j] != '\r' && Data[j] != '\n')
			{
				nValue = DecodeTable[Data[j++]] << 18;
				nValue += DecodeTable[Data[j++]] << 12;
				strDecode += (nValue & 0x00FF0000) >> 16;
				if (Data[0] != '=')
				{
					nValue += DecodeTable[Data[j++]] << 6;
					strDecode += (nValue & 0x0000FF00) >> 8;
					if (Data[0] != '=')
					{
						nValue += DecodeTable[Data[j++]];
						strDecode += nValue & 0x000000FF;
					}
				}
				i += 4;
			}
			else// 回车换行,跳过
			{
				j++;
				i++;
			}
		}
		return strDecode;
	}

	bool BaseUtils::Bigendian()
	{
		static union
		{
			int nTest;
			char cTest[sizeof(int)];
		}un_bigendina = { 1 };

		return un_bigendina.cTest[0] != 1;
	}

	//! \brief 对 values 进行解码，结果保存在 vec 中
	void BaseUtils::OctDecode(const short values[2], float vec[3])
	{
		Snorm<snormSize> projected[2];
		projected[0] = Snorm<snormSize>::fromBits(values[0]);
		projected[1] = Snorm<snormSize>::fromBits(values[1]);

		vec[0] = float(projected[0]);
		vec[1] = float(projected[1]);
		vec[2] = 1.0f - (fabs(vec[0]) + fabs(vec[1]));

		if (vec[2] < 0.0f) {
			float oldX = vec[0];
			vec[0] = ((1.0f) - fabs(vec[1])) * signNotZero(oldX);
			vec[1] = ((1.0f) - fabs(oldX))   * signNotZero(vec[1]);
		}
	}

	//! \brief 对 vec 进行编码，结果保存在 values 中
	void BaseUtils::OctEncode(const float vec[3], short values[2])
	{
		Snorm<snormSize> projected[2];
		const float invL1Norm = (1.0f) / (fabs(vec[0]) + fabs(vec[1]) + fabs(vec[2]));

		if (vec[2] < 0.0f) {
			projected[0] = Snorm<snormSize>((1.0f - float(fabs(vec[1] * invL1Norm))) * signNotZero(vec[0]));
			projected[1] = Snorm<snormSize>((1.0f - float(fabs(vec[0] * invL1Norm))) * signNotZero(vec[1]));
		}
		else {
			projected[0] = Snorm<snormSize>(vec[0] * invL1Norm);
			projected[1] = Snorm<snormSize>(vec[1] * invL1Norm);
		}

		values[0] = projected[0].bits();
		values[1] = projected[1].bits();
	}

	Vector3d VectorUtils::CrossProduct(const Vector3d &v1, const Vector3d &v2)
	{
		Vector3d tempVector;
		tempVector.x = v1.y * v2.z - v1.z * v2.y;
		tempVector.y = v1.z * v2.x - v1.x * v2.z;
		tempVector.z = v1.x * v2.y - v1.y * v2.x;

		return tempVector;
	}

	double VectorUtils::DotProduct(const Vector3d& v1, const Vector3d& v2)
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

	Matrix4d MatrixUtils::Translation(double x, double y, double z)
	{
		Matrix4d solution
		(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			x, y, z, 1
		);

		return solution;
	}

	Matrix4d MatrixUtils::RotationXYZ(double rotationX, double rotationY, double rotationZ)
	{
		return RotationX(rotationX)*RotationY(rotationY)*RotationZ(rotationZ);
	}

	Matrix4d MatrixUtils::RotationX(double angle)
	{
		double dSin = (double)sin(angle);
		double dCos = (double)cos(angle);

		Matrix4d solution
		(
			1, 0, 0, 0,
			0, dCos, dSin, 0,
			0, -dSin, dCos, 0,
			0, 0, 0, 1
		);
		return solution;
	}

	Matrix4d MatrixUtils::RotationY(double angle)
	{
		double dSin = (double)sin(angle);
		double dCos = (double)cos(angle);

		Matrix4d solution
		(
			dCos, 0, -dSin, 0,
			0, 1, 0, 0,
			dSin, 0, dCos, 0,
			0, 0, 0, 1
		);
		return solution;
	}

	Matrix4d MatrixUtils::RotationZ(double angle)
	{
		double dSin = (double)sin(angle);
		double dCos = (double)cos(angle);

		Matrix4d solution
		(
			dCos, dSin, 0, 0,
			-dSin, dCos, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		);
		return solution;
	}

	Matrix4d MatrixUtils::Scaling(double x, double y, double z)
	{
		Matrix4d solution
		(
			x, 0, 0, 0,
			0, y, 0, 0,
			0, 0, z, 0,
			0, 0, 0, 1
		);
		return solution;
	}

	Matrix4d MatrixUtils::QuaternionToRotationMatrix(Vector4d quat)
	{
		double fTx = 2.0*quat.x;
		double fTy = 2.0*quat.y;
		double fTz = 2.0*quat.z;
		double fTwx = fTx * quat.w;
		double fTwy = fTy * quat.w;
		double fTwz = fTz * quat.w;
		double fTxx = fTx * quat.x;
		double fTxy = fTy * quat.x;
		double fTxz = fTz * quat.x;
		double fTyy = fTy * quat.y;
		double fTyz = fTz * quat.y;
		double fTzz = fTz * quat.z;

		Matrix4d m;
		m[0][0] = 1.0 - (fTyy + fTzz);
		m[1][0] = fTxy - fTwz;
		m[2][0] = fTxz + fTwy;
		m[3][0] = 0.0;

		m[0][1] = fTxy + fTwz;
		m[1][1] = 1.0 - (fTxx + fTzz);
		m[2][1] = fTyz - fTwx;
		m[3][1] = 0.0;

		m[0][2] = fTxz - fTwy;
		m[1][2] = fTyz + fTwx;
		m[2][2] = 1.0 - (fTxx + fTyy);
		m[3][2] = 0.0;

		m[0][3] = 0.0;
		m[1][3] = 0.0;
		m[2][3] = 0.0;
		m[3][3] = 1.0;
		return m;
	}

	void TextureUtils::UpdateTextureTier(TextureDataInfo*& pTexDataInfo, TextureCompressType enCompressType)
	{
		if (pTexDataInfo == nullptr || 
			pTexDataInfo->m_pTextureData == nullptr || 
            (pTexDataInfo->m_bMipmap && pTexDataInfo->m_pTextureData->m_eCompressType == enCompressType))
		{
			return;
		}

		int comp = ScaleTextureData(pTexDataInfo->m_pTextureData);

		PixelFormat enFormat = pTexDataInfo->m_pTextureData->m_enFormat;
		unsigned int nAdjustW = pTexDataInfo->m_pTextureData->m_nWidth;
		unsigned int nAdjustH = pTexDataInfo->m_pTextureData->m_nHeight;
		unsigned char *pDest = pTexDataInfo->m_pTextureData->m_pBuffer;

		unsigned int i = 0, j = 0, k = 0;
		unsigned char* pRGBAData = new unsigned char[nAdjustW*nAdjustH * 4];
		if (comp == 3)
		{
			if (enFormat == PF_RGB8)
			{
				for (i = 0; i < nAdjustH*nAdjustW; i++, j += 4, k += 3)
				{
					pRGBAData[j] = pDest[k]; //r
					pRGBAData[j + 1] = pDest[k + 1]; //g
					pRGBAData[j + 2] = pDest[k + 2];	//b
					pRGBAData[j + 3] = 255;
				}
			}
			else
			{
				for (i = 0; i < nAdjustH*nAdjustW; i++, j += 4, k += 3)
				{
					pRGBAData[j] = pDest[k + 2]; //r
					pRGBAData[j + 1] = pDest[k + 1]; //g
					pRGBAData[j + 2] = pDest[k];	//b
					pRGBAData[j + 3] = 255;
				}
			}
		}
		else
		{
			if (enFormat == PF_RGBA8)
			{
				for (i = 0; i < nAdjustH*nAdjustW; i++, j += 4, k += 4)
				{
					pRGBAData[j] = pDest[k];	//r
					pRGBAData[j + 1] = pDest[k + 1];	//g
					pRGBAData[j + 2] = pDest[k + 2];	//b
					pRGBAData[j + 3] = pDest[k + 3]; //a
				}
			}
			else
			{
				for (i = 0; i < nAdjustH*nAdjustW; i++, j += 4, k += 4)
				{
					pRGBAData[j] = pDest[k + 2];	//r
					pRGBAData[j + 1] = pDest[k + 1];	//g
					pRGBAData[j + 2] = pDest[k];	//b
					pRGBAData[j + 3] = pDest[k + 3]; //a
				}
			}
		}

		delete pTexDataInfo->m_pTextureData;
		pTexDataInfo->m_pTextureData = nullptr;

		//创建mipmap，压缩为DXT格式
		unsigned char* pOut = nullptr;
		TextureData* pTextureData = new TextureData;
		pTextureData->m_enFormat = PF_RGBA8;
        unsigned int nCompressedTextureSize = ImageOperator::Encode(4, nAdjustW, nAdjustH, pRGBAData, &pOut, enCompressType, true);
		pTextureData->m_nSize = nCompressedTextureSize;
		pTextureData->m_pBuffer = pOut;
		pTextureData->m_nWidth = nAdjustW;
		pTextureData->m_nHeight = nAdjustH;
        pTextureData->m_eCompressType = enCompressType;
		pTexDataInfo->m_nLevel = ImageOperator::GetMipMapLevel(nAdjustW, nAdjustH);

		pTexDataInfo->m_pTextureData = pTextureData;
        pTexDataInfo->m_bMipmap = true;

		delete[] pRGBAData;
		pRGBAData = nullptr;
	}

	int TextureUtils::ScaleTextureData(TextureData* pTextureData, const int nMaxW, const int nMaxH, bool bForceMax)
	{
		unsigned int nWidth = pTextureData->m_nWidth;
		unsigned int nHeight = pTextureData->m_nHeight;

		unsigned int nAdjustW = nWidth, nAdjustH = nHeight;
		if (!bForceMax)
		{
			unsigned int nWidthRemainder = nWidth % 4;
			unsigned int nHeightRemainder = nHeight % 4;
			nAdjustW += nWidthRemainder > 0 ? 4 - nWidthRemainder : 0;
			nAdjustH += nHeightRemainder > 0 ? 4 - nHeightRemainder : 0;
			nAdjustW = (nMaxW == 0) ? nAdjustW : (MIN((int)nAdjustW, nMaxW));
			nAdjustH = (nMaxH == 0) ? nAdjustH : (MIN((int)nAdjustH, nMaxH));
		}
		else
		{
			nAdjustH = nMaxH;
			nAdjustW = nMaxW;
		}

		PixelFormat enFormat = pTextureData->m_enFormat;
		int comp = 1;
		if (enFormat == PF_RGB8 || enFormat == PF_RGB32F)
		{
			comp = 3;
		}
		else if (enFormat == PF_RGBA8 || enFormat == PF_RGBA32F)
		{
			comp = 4;
		}

		//若纹理已压缩，则需解压
		unsigned char* pOutData = nullptr;
        if (pTextureData->m_eCompressType != TextureCompressType::TC_NONE)
		{
            unsigned int nOutDataSize = ImageOperator::Decode(comp, nAdjustW, nAdjustH, &pOutData, pTextureData->m_pBuffer, pTextureData->m_eCompressType, false);
			comp = 4;
			delete[] pTextureData->m_pBuffer;
			pTextureData->m_pBuffer = pOutData;
			pTextureData->m_nSize = nOutDataSize;
            pTextureData->m_eCompressType = TextureCompressType::TC_NONE;
		}

		unsigned char* pSrc = pTextureData->m_pBuffer;
		if (nWidth != nAdjustW || nHeight != nAdjustH)
		{
			//缩放纹理图片为2的N次方
			unsigned char* pDest = new unsigned char[nAdjustW * nAdjustH * comp];
			MathEngine::ScaleImageInternal(comp, nWidth, nHeight, pSrc, nAdjustW, nAdjustH, pDest);
			delete[] pTextureData->m_pBuffer;
			pTextureData->m_nSize = nAdjustW * nAdjustH * comp;
			pTextureData->m_pBuffer = pDest;
		}

		pTextureData->m_nWidth = nAdjustW;
		pTextureData->m_nHeight = nAdjustH;
		return comp;
	}

	int TextureUtils::AdjustTextureSize(unsigned int nSize)
	{
		int texSize = 4;
		while (nSize > texSize * 1.75)
		{
			texSize *= 2;
		}
		return texSize;
	}

	void MaterialUtils::SetTextureName(Material* pMaterial, const std::vector<std::wstring>& vecTexNames)
	{
		if (pMaterial == nullptr || vecTexNames.size() == 0)
		{
			return;
		}

		Technique* pTech = nullptr;
		if (pMaterial->getNumTechniques() == 0)
		{
			pTech = pMaterial->CreateTechnique();
		}
		else
		{
			pTech = pMaterial->getTechnique(0);
		}
		if (pTech->getNumPasses() == 0)
		{
			pTech->CreatePass();
		}
		Pass* pPass = pTech->getPass(0);

		std::map<std::wstring, std::wstring> mapTexNames;
		for (int i = 0; i < vecTexNames.size(); i++)
		{
			std::wstring strTextureName = vecTexNames[i];
			mapTexNames[strTextureName] = strTextureName;
		}

		for (int i = 0; i < pPass->GetTextureUnitStatesSize(); i++)
		{
			TextureUnitState* pTexUnit = pPass->GetTextureUnitState(i);
			if (pTexUnit == nullptr)
			{
				continue;
			}

			if (mapTexNames.find(pTexUnit->m_strTextureName) == mapTexNames.end())
			{
				PassUtils::RemoveTextureUnitState(pPass, i);
				i--;
			}
		}

		for (int i = 0; i < vecTexNames.size(); i++)
		{
			std::wstring strTextureName = vecTexNames[i];
			TextureUnitState* pTexUnit = pPass->GetTextureUnitState(strTextureName);
			if (pTexUnit == nullptr)
			{
				pTexUnit = pPass->CreateTextureUnitState();
				pTexUnit->m_strTextureName = strTextureName;
			}
		}
	}

	void MaterialUtils::MakeDefault(Material*& pMaterial)
	{
		Technique* pTech = pMaterial->CreateTechnique();
		Pass* pPass = pTech->CreatePass();
		pPass->m_strName = pMaterial->m_strName;
	}

	void PassUtils::RemoveTextureUnitState(Pass* pPass, unsigned short nIndex)
	{
		std::vector<TextureUnitState*>& vecState = pPass->m_pTextureUnitStates;
		if (nIndex >= vecState.size())
		{
			return;
		}

		delete vecState[nIndex];
		vecState.erase(vecState.begin() + nIndex);
	}

	unsigned int ColorUtils::RGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
	{
		unsigned int nValue = 0;
		if (BaseUtils::Bigendian())
		{
			nValue = (((unsigned int)(unsigned char)(r) << 24) | ((unsigned int)(unsigned char)(g) << 16) | ((unsigned int)(unsigned char)(b) << 8) | ((unsigned int)(unsigned char)(a)));
		}
		else
		{
			nValue = (((unsigned int)(unsigned char)(r)) | ((unsigned int)(unsigned char)(g) << 8) | ((unsigned int)(unsigned char)(b) << 16) | ((unsigned int)(unsigned char)(a) << 24));
		}
		return nValue;
	}

	unsigned char ColorUtils::RedValue(unsigned int clr)
	{
		unsigned char nValue = 0;
		if (ISBIGENDIAN)
		{
			nValue = ((unsigned char)(((clr) >> 24) & 0xff));
		}
		else
		{
			nValue = ((unsigned char)((clr) & 0xff));
		}
		return nValue;
	}

	unsigned char ColorUtils::GreenValue(unsigned int clr)
	{
		unsigned char nValue = 0;
		if (ISBIGENDIAN)
		{
			nValue = ((unsigned char)(((clr) >> 16) & 0xff));
		}
		else
		{
			nValue = ((unsigned char)(((clr) >> 8) & 0xff));
		}
		return nValue;
	}

	unsigned char ColorUtils::BlueValue(unsigned int clr)
	{
		unsigned char nValue = 0;
		if (ISBIGENDIAN)
		{
			nValue = ((unsigned char)(((clr) >> 8) & 0xff));

		}
		else
		{
			nValue = ((unsigned char)(((clr) >> 16) & 0xff));

		}
		return nValue;
	}

	unsigned char ColorUtils::AlphaValue(unsigned int clr)
	{
		unsigned char nValue = 0;
		if (ISBIGENDIAN)
		{
			nValue = ((unsigned char)(((clr)) & 0xff));
		}
		else
		{
			nValue = ((unsigned char)(((clr) >> 24) & 0xff));
		}
		return nValue;
	}

	void ImageUtils::ImageSwapRB(const unsigned int components, unsigned int nWidth, unsigned int nHeight, unsigned char* in, bool bIsContainMipmap)
	{
		unsigned char* pBuffer = in;
		do
		{
			SwapRB(components, nWidth, nHeight, pBuffer);
			pBuffer += nWidth * nHeight * 4;
			if (nWidth == 1 && nHeight == 1)
			{
				break;
			}

			nWidth /= 2;
			nHeight /= 2;

			if (0 == nWidth) nWidth = 1;
			if (0 == nHeight) nHeight = 1;

		} while (bIsContainMipmap);
	}

	void ImageUtils::SwapRB(const unsigned int components, unsigned int widthin, unsigned int heightin, unsigned char* in)
	{
		if (widthin == 0 || heightin == 0 || in == NULL)
		{
			return;
		}

		unsigned int size = components * widthin * heightin;
		unsigned int idx = 0;
		unsigned char swap = 0;

		for (; idx < size; idx += components)
		{
			swap = in[idx];
			in[idx] = in[idx + 2];
			in[idx + 2] = swap;
		}
	}

	BoundingBox BoundingBoxUtils::MultiplyMatrix(const BoundingBox& box, const Matrix4d& mat)
	{
		if (box.IsNULL())
		{
			return box;
		}

		const Vector3d* pCorners = box.GetCorners();
		Vector3d vMax, vMin, vCorner;
		for (int i = 0; i < 8; i++)
		{
			vCorner = pCorners[i].MultiplyMatrix(mat);

			if (i == 0)
			{
				vMax = vMin = vCorner;
			}
			else
			{
				vMax.x = MAX(vMax.x, vCorner.x);
				vMax.y = MAX(vMax.y, vCorner.y);
				vMax.z = MAX(vMax.z, vCorner.z);

				vMin.x = MIN(vMin.x, vCorner.x);
				vMin.y = MIN(vMin.y, vCorner.y);
				vMin.z = MIN(vMin.z, vCorner.z);
			}
		}
		return BoundingBox(vMin, vMax);
	}

	BoundingBox BoundingBoxUtils::OBBToAABB(const OrientedBoundingBox& obb)
	{
		OrientedBoundingBox oriOBB(obb.GetCenter(), obb.GetHalfAxes());
		Vector3d vCenter = oriOBB.GetCenter();
		Matrix3d matHalfAxes = oriOBB.GetHalfAxes();

		const Vector3d& halfAxisX = matHalfAxes.GetColumn(0);
		const Vector3d& halfAxisY = matHalfAxes.GetColumn(1);
		const Vector3d& halfAxisZ = matHalfAxes.GetColumn(2);

		Vector3d extent;
		extent.x = std::abs(halfAxisX.x) + std::abs(halfAxisY.x) + std::abs(halfAxisZ.x);
		extent.y = std::abs(halfAxisX.y) + std::abs(halfAxisY.y) + std::abs(halfAxisZ.y);
		extent.z = std::abs(halfAxisX.z) + std::abs(halfAxisY.z) + std::abs(halfAxisZ.z);

		const Vector3d& min = vCenter - extent;
		const Vector3d& max = vCenter + extent;

		BoundingBox box;
		box.SetExtents(min, max);
		return box;
	}

    int FileUtils::ListFiles(std::vector<std::wstring>& filelist, const std::wstring& path, const std::wstring& pattern, unsigned int flags)
    {
        // Init
        int count = 0;
        unsigned int matchmode = FILEMATCH_FILE_NAME | FILEMATCH_NOESCAPE;
        std::wstring strPathName;
        std::wstring strName;

        if (flags&LIST_CASEFOLD)
        {
            matchmode |= FILEMATCH_CASEFOLD;
        }

#ifdef WIN32
        // 复制文件夹名称
        strPathName = path;
        if (!ISPATHSEP(strPathName[strPathName.length() - 1]))
        {
            strPathName += PATHSEPSTRING;
        }
        strPathName += U("*");
        struct _wfinddata_t c_file;
		auto lIsFindFile = _wfindfirst(strPathName.c_str(), &c_file);
        if (lIsFindFile != -1)
        {
            // Loop over directory entries
            do
            {
                // 获取名称
                strName = c_file.name;

                if (!(c_file.attrib&_A_SUBDIR)
                    && ((flags&LIST_NO_FILES)
                        || ((c_file.attrib&_A_HIDDEN) && !(flags&LIST_HIDDEN_FILES))
                        || (!(flags&LIST_ALL_FILES) && !Match(pattern, strName, matchmode))))
                {
                    continue;
                }

                // Filter out directories; even more tricky!
                if ((c_file.attrib&_A_SUBDIR)
                    && ((flags&LIST_NO_DIRS)
                        || ((c_file.attrib&_A_HIDDEN) && !(flags&LIST_HIDDEN_DIRS))
                        || (strName == U(".") || strName == U("..")) // add by zengzm 2007-7-17 如果是. 或者..,则跳过
                        || (strName[0] == U('.') && (strName[1] == 0 || (strName[1] == U('.') && strName[2] == 0 && (flags&LIST_NO_PARENT))))
                        || (!(flags&LIST_ALL_DIRS) && !Match(pattern, strName, matchmode))))
                {
                    continue;
                }

                filelist.push_back(strName);
                count++;
            } while (_wfindnext(lIsFindFile, &c_file) == 0);

            _findclose(lIsFindFile);
        }
#else
        DIR *pDir =NULL;
        struct stat statInf;
        struct dirent *dp = NULL;
        memset(&statInf, 0, sizeof(statInf));
        std::string strPath = StringUtil::UnicodeToANSI(path);
        pDir=opendir(strPath.c_str());
        if(pDir)
        {
            while((dp=readdir(pDir))!=NULL)
            {
                // 赋值strName
                std::wstring strTempName = StringUtil::ANSIToUnicode(dp->d_name);
                strName = strTempName;

                // 赋值strPathName
                strPathName = path;

                if(!ISPATHSEP(strPathName[strPathName.length()-1]))
                {
                    strPathName += PATHSEPSTRING;
                }

                strPathName += strName;

                const wchar_t* wstrSymbol = U(".").c_str();
                if( !S_ISDIR(statInf.st_mode)
                    && ((flags&LIST_NO_FILES)
                       || (strName[0]==*wstrSymbol && !(flags&LIST_HIDDEN_FILES))
                       || (!(flags&LIST_ALL_FILES) && !Match(pattern,strName,matchmode))))
                {
                    continue;
                }

                if(S_ISDIR(statInf.st_mode)
                    && ((flags&LIST_NO_DIRS)
                        || strName.length()<=2
                        || (strName[0]==*wstrSymbol && (strName[1]==0 || (strName[1]==*wstrSymbol && strName[2]==0 && (flags&LIST_NO_PARENT)) || (strName[1]!=*wstrSymbol && !(flags&LIST_HIDDEN_DIRS))))
                        || (!(flags&LIST_ALL_DIRS) && !Match(pattern,strName,matchmode))))
                {
                    continue;
                }

                filelist.push_back(strName);
                count++;
            }
            closedir(pDir);
        }
#endif
        return count;
    }


    std::wstring FileUtils::GetTemporaryPath()
    {
#ifdef WIN32
        wchar_t cpBuffer[MAX_PRO_STRING_LENGTH];
        wchar_t cpLongBuffer[MAX_PRO_STRING_LENGTH];
        ::GetTempPath(MAX_PRO_STRING_LENGTH, cpBuffer);

        ::GetLongPathName(cpBuffer, cpLongBuffer, MAX_PRO_STRING_LENGTH);

        std::wstring strResult(cpLongBuffer);
        return strResult;
#else
        if(!access("/tmp/", F_OK))
        {			// In linux, return "/tmp" directly, not defined variable TMP & TEMP.
            return U("/tmp/");
        }
        else
        {		// if /tmp doesn't exists, return current work directory.
            char buf[PATH_MAX];
            if(!getcwd(buf, PATH_MAX))
            {
                return U("");
            }
            else
            {
                std::string strMB(buf);
                std::wstring strU = StringUtil::ANSIToUnicode(strMB);
                return strU;
            }
        }
#endif
    }

    bool FileUtils::Match(const std::wstring& pattern, const std::wstring& file, unsigned int flags)
    {
        return ugfilematch(pattern.c_str(), file.c_str(), flags);
    }

    // Public API to matcher
    int FileUtils::ugfilematch(const wchar_t *pattern, const wchar_t *str, unsigned int flags)
    {
        const wchar_t *p = pattern;
        const wchar_t *q = str;
        int level = 0;
        if (p && q)
        {
        nxt:	if (DoMatch(p, q, flags))
        {
            return 1;
        }
                for (level = 0; *p && 0 <= level; )
                {
                    switch (*p++)
                    {
                    case L'\\':
                        if (*p)
                        {
                            p++;
                        }
                        break;
                    case L'(':
                        level++;
                        break;
                    case L')':
                        level--;
                        break;
                    case L'|':
                    case L',':
                        if (level == 0)
                        {
                            goto nxt;
                        }
                    }
                }
        }
        return 0;
    }

    int FileUtils::DoMatch(const wchar_t *pattern, const wchar_t *str, unsigned int flags)
    {
        const wchar_t *p = pattern;
        const wchar_t *q = str;
        const wchar_t *s = NULL;
        wchar_t c, cs, ce, cc, neg;
        int level = 0;
        while ((c = *p++) != L'\0')
        {
            switch (c)
            {
            case L'?':
                if (*q == L'\0')
                {
                    return 0;
                }
                if ((flags&FILEMATCH_FILE_NAME) && ISPATHSEP(*q))
                {
                    return 0;
                }
                if ((flags&FILEMATCH_PERIOD) && (*q == L'.') && ((q == str) || ((flags&FILEMATCH_FILE_NAME) && ISPATHSEP(*(q - 1)))))
                {
                    return 0;
                }
                q++;
                break;
            case L'*':
                c = *p;
                while (c == L'*')
                {
                    c = *++p;
                }
                if ((flags&FILEMATCH_PERIOD) && (*q == L'.') && ((q == str) || ((flags&FILEMATCH_FILE_NAME) && ISPATHSEP(*(q - 1)))))
                {
                    return 0;
                }
                if (c == L'\0')
                {    // Optimize for case of trailing '*'
                    if (flags&FILEMATCH_FILE_NAME)
                    {
                        for (s = q; *s; s++)
                        {
                            if (ISPATHSEP(*s))
                            {
                                return 0;
                            }
                        }
                    }
                    return 1;
                }
                while (!DoMatch(p, q, flags&~FILEMATCH_PERIOD))
                {
                    if ((flags&FILEMATCH_FILE_NAME) && ISPATHSEP(*q))
                    {
                        return 0;
                    }
                    if (*q++ == L'\0')
                    {
                        return 0;
                    }
                }
                return 1;
            case L'[':
                if (*q == L'\0')
                {
                    return 0;
                }
                if ((flags&FILEMATCH_PERIOD) && (*q == L'.') && ((q == str) || ((flags&FILEMATCH_FILE_NAME) && ISPATHSEP(*(q - 1)))))
                {
                    return 0;
                }
                cc = FOLD(*q);
                if ((neg = ((*p == L'!') || (*p == L'^'))), neg)
                {
                    p++;
                }
                c = *p++;
                do
                {
                    if (c == L'\\' && !(flags&FILEMATCH_NOESCAPE))
                    {
                        c = *p++;
                    }
                    cs = ce = FOLD(c);
                    if (c == L'\0')
                    {
                        return 0;
                    }
                    c = *p++;
                    c = FOLD(c);
                    if ((flags&FILEMATCH_FILE_NAME) && ISPATHSEP(c))
                    {
                        return 0;
                    }
                    if (c == L'-' && *p != L']')
                    {
                        ce = *p++;
                        if (ce == L'\\' && !(flags&FILEMATCH_NOESCAPE))
                        {
                            c = *p++;
                        }
                        if (c == L'\0')
                        {
                            return 0;
                        }
                        ce = FOLD(c);
                        c = *p++;
                    }

                    if (((unsigned short)cs) <= ((unsigned short)cc) && ((unsigned short)cc) <= ((unsigned short)ce))
                    {
                        goto match;
                    }
                } while (c != L']');
                if (!neg)
                {
                    return 0;
                }
                q++;
                break;
            match:  while (c != L']')
            {
                if (c == L'\0')
                {
                    return 0;
                }
                c = *p++;
                if (c == L'\\' && !(flags&FILEMATCH_NOESCAPE))
                {
                    p++;
                }
            }
                    if (neg)
                    {
                        return 0;
                    }
                    q++;
                    break;
            case L'(':
            nxt:    if (DoMatch(p, q, flags))
            {
                return 1;
            }
                    for (level = 0; *p && 0 <= level; )
                    {
                        switch (*p++)
                        {
                        case L'\\':
                            if (*p)
                            {
                                p++;
                            }
                            break;
                        case L'(':
                            level++;
                            break;
                        case L')':
                            level--;
                            break;
                        case L'|':
                        case L',':
                            if (level == 0)
                            {
                                goto nxt;
                            }
                        }
                    }
                    return 0;
            case L')':
                break;
            case L'||':
            case L',':
                for (level = 0; *p && 0 <= level; )
                {
                    switch (*p++)
                    {
                    case L'\\':
                        if (*p)
                        {
                            p++;
                        }
                        break;
                    case L'(':
                        level++;
                        break;
                    case L')':
                        level--;
                        break;
                    }
                }
                break;
            case L'\\':
                if (*p && !(flags&FILEMATCH_NOESCAPE))
                {
                    c = *p++;   // Trailing escape represents itself
                }
            default:
                if (FOLD(c) != FOLD(*q))
                {
                    return 0;
                }
                q++;
                break;
            }
        }
        return (*q == L'\0') || (ISPATHSEP(*q) && (flags&FILEMATCH_LEADING_DIR));
    }
}
