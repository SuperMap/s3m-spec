#if !defined(UTILS_H__9D4EC59F_85EF_49B8_B1F3_5F9B9ECF0053__INCLUDED_)
#define UTILS_H__9D4EC59F_85EF_49B8_B1F3_5F9B9ECF0053__INCLUDED_

#pragma once
#include <string>
#include "stdafx.h"
#include "Vector.h"
#include "Skeleton.h"
#include "Texture.h"
#include "Material.h"
#include "Common.h"
#include <math.h>

namespace S3MB
{
	#define snormSize 16

	// �ж���2�ļ��η�
	inline unsigned int get2n(unsigned int n)
	{
		unsigned int i = 0;
		while (n)
		{
			i++;
			n = n >> 1;
		}
		return i - 1;
	}

	inline float signNotZero(float v) {
		return (v < 0.0f) ? -1.0f : 1.0f;
	}

	inline float clamp(float val, float low, float hi) {
		if (val <= low) {
			return low;
		}
		else if (val >= hi) {
			return hi;
		}
		else {
			return val;
		}
	}

	inline float round(float f) {
		return floor(f + 0.5f);
	}

	/** The underlying representation is always int in this implementation.
	With template tricks or a type argument one can implement
	a variant that chooses between int8, int16, int32, and int64 for
	the internal representation...or use the somewhat more obscure C
	"int m_bits:bitcount;" syntax, which does not actually save
	space below 8 bits because of alignment requirements.
	*/
	template<int bitcount>
	class Snorm {
	private:
		typedef short T;
		T   m_bits;

		/** Private to prevent illogical conversions without explicitly
		stating that the input should be treated as bits; see fromBits. */
		explicit Snorm(T b) : m_bits(b) {}

	public:

		explicit Snorm() : m_bits(0) {}

		/** Equivalent to: \code snorm8 u = reinterpret_cast<const snorm8&>(255);\endcode */
		static Snorm fromBits(T b) {
			return Snorm(b);
		}

		/** Maps f to the underlying bits of round(f * ((2^(bitcount - 1) - 1)).*/
		explicit Snorm(float f) {
            m_bits = (T)round(clamp(f, -1.0f, 1.0f) * (((unsigned long)(1) << (bitcount - 1)) - 1));
		}

		/** this function will be used to find the top left corner of the corresponding square to allow for discrete searching*/
		static Snorm flooredSnorms(float f) {
            return fromBits((T)(floor(clamp(f, -1.0f, 1.0f) * (((unsigned long)(1) << (bitcount - 1)) - 1))));
		}

		/** Returns a number on [0.0f, 1.0f] */
		operator float() const {
            return float(clamp(int(m_bits) * (1.0f / float(((unsigned long)(1) << (bitcount - 1)) - 1)), -1.0f, 1.0f));
		}

		T bits() const {
			return m_bits;
		}
	};

	class STK_API BaseUtils
	{
	public:
		// ����guid
		static std::wstring CreateGuid();

		static std::wstring Base64Decode(const std::wstring& Data, int DataByte);

		static bool Bigendian();

		//! \brief OCTѹ����
		static void OctDecode(const short values[2], float vec[3]);
		static void OctEncode(const float vec[3], short values[2]);
	};

	class STK_API VectorUtils
	{
	public:
		//! \brief �����������Ĳ��
		static Vector3d CrossProduct(const Vector3d &v1, const Vector3d &v2);

		//! \brief �����������ĵ��
		static double DotProduct(const Vector3d& v1, const Vector3d& v2);
	};

	class STK_API MatrixUtils
	{
	public:
		//! \brief ����ƽ�ơ�
		//! \param x X���ƶ���С[in]��
		//! \param y Y���ƶ���С[in]��
		//! \param z Z���ƶ���С[in]��
		//! \return ƽ�ƺ����
		static Matrix4d Translation(double x, double y, double z);

		//! \brief ��ת��
		//! \param rotationX ��X����ת�Ƕ�[in]��
		//! \param rotationY ��Y����ת�Ƕ�[in]��
		//! \param rotationZ ��Z����ת�Ƕ�[in]��
		//! \return ��ת�����
		static Matrix4d RotationXYZ(double rotationX, double rotationY, double rotationZ);

		//! \brief ��X����ת��
		//! \param angle ��X����ת�Ƕ�[in]��
		//! \return ��ת�����		
		static Matrix4d RotationX(double angle);

		//! \brief ��Y����ת��
		//! \param angle ��Y����ת�Ƕ�[in]��
		//! \return ��ת�����
		static Matrix4d RotationY(double angle);

		//! \brief ��Z����ת��
		//! \param angle [in]��
		//! \return ��ת�����		
		static Matrix4d RotationZ(double angle);

		//! \brief ����һ�����ž���
		//! param x X�����Ŵ�С[in]��
		//! param y Y�����Ŵ�С[in]��
		//! param z Z�����Ŵ�С[in]��
		//! return ���ž���
		static Matrix4d Scaling(double x, double y, double z);

		//! \brief  ��Ԫ��ת������ת����
		static Matrix4d QuaternionToRotationMatrix(Vector4d quat);
	};

	class STK_API SkeletonUtils
	{
	public:
		//! \brief ȥ��ûʹ�õĶ��㣬�ؽ�����
		static void RebuildVertexAndIndexPackage(VertexDataPackage* pVertexDataPackage, std::vector<IndexPackage*>& vecIndexPackage, std::vector<IDInfo*>& vecIDInfo);

		// fan/strip�������͵�����תtriangle list���͵�����
		static bool TranslateIndices(IndexPackage *pIndexPackage);
	};

	class STK_API TextureUtils
	{
	public:
		static void UpdateTextureTier(TextureDataInfo*& pTexDataInfo, TextureCompressType enCompressType);

		//! \brief ���������ݽ�������
		//! \brief nMaxW nMaxH ������ƣ� =0 ��ʾ������
		//! \brief ���� comp
		static int ScaleTextureData(TextureData* pTextureData, const int nMaxW = 0, const int nMaxH = 0, bool bForceMax = false);

		//! \brief ������ͼ��СΪ2n�η�
		static int AdjustTextureSize(unsigned int nSize);
	};

	class STK_API MaterialUtils
	{
	public:
		static void SetTextureName(Material* pMaterial, const std::vector<std::wstring>& vecTexNames);

		static void MakeDefault(Material*& pMaterial);
	};

	class STK_API PassUtils
	{
	public:
		//�Ƴ�index��Ӧ��TextureUnitState
		static void RemoveTextureUnitState(Pass* pPass, unsigned short nIndex);
	};

	class STK_API ColorUtils
	{
	public:
		static ColorValue White;

		static unsigned int RGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

		static unsigned char RedValue(unsigned int clr);
		static unsigned char GreenValue(unsigned int clr);
		static unsigned char BlueValue(unsigned int clr);
		static unsigned char AlphaValue(unsigned int clr);
	};

	class STK_API ImageUtils
	{
	public:
		// RGB,RGBA �� BGR,BRGA֮���໥ת��,ֱ�Ӹı�ԭʼ����
		// RGBʱcomponents = 3,RGBA components = 4
		static void ImageSwapRB(const unsigned int components, unsigned int widthin, unsigned int heightin, unsigned char* in, bool bIsContainMipmap = false);
	
	private:
		// RGB,RGBA �� BGR,BRGA֮���໥ת��,ֱ�Ӹı�ԭʼ����
		// RGBʱcomponents = 3,RGBA components = 4
		static void SwapRB(const unsigned int components, unsigned int widthin, unsigned int heightin, unsigned char* in);
	};

	class STK_API BoundingBoxUtils
	{
	public:
		static BoundingBox MultiplyMatrix(const BoundingBox& box, const Matrix4d& mat);

		static BoundingBox OBBToAABB(const OrientedBoundingBox& obb);
	};

    class STK_API FileUtils
    {
    public:

        // ��ĳ��Ŀ¼�в��ҷ��ϸ�������������ļ�
        //! strFileList �õ��ķ��ϸ���������ļ������� [out]
        //! strPath Ҫ���ҵ�Ŀ¼ [in]
        //! strPattern �����Ĺ��� [in]
        //! nFlags �жϵ�һЩ����ʽ [in]
        //! �����ҵ����Ϲ�����ļ�����
        static int ListFiles(std::vector<std::wstring>& strFileList, const std::wstring& strPath,
            const std::wstring& strPattern = U("*"), unsigned int nFlags = LIST_MATCH_ALL);

        //! �õ���ʱ·��
        //! ������ʱ·��
        static std::wstring GetTemporaryPath();

		// ����
		static bool LoadBuffer(const std::wstring& strPath, unsigned char*& pBuffer, unsigned long long& nBufferSize);

    private:
        // Match filenames using *, ?, [^a-z], and so on
        //!  �ж��ļ����Ƿ����һ���Ĺ���(ģʽ)
        //!  strPattern �涨��ģʽ
        //!  strFileName Ҫ�жϵ��ļ���
        //!  nFlags �жϵ�һЩ����ʽ
        //!  �ļ������Ϲ��򷵻�true,���򷵻�false
        static bool Match(const std::wstring& strPattern, const std::wstring& strFileName,
            unsigned int nFlags = (FILEMATCH_NOESCAPE | FILEMATCH_FILE_NAME));

        static int ugfilematch(const wchar_t *pattern, const wchar_t *str, unsigned int flags = (FILEMATCH_NOESCAPE | FILEMATCH_FILE_NAME));
        static int DoMatch(const wchar_t *pattern, const wchar_t *str, unsigned int flags);
    };
}

#endif