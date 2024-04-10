#if !defined(UTILS_H__9D4EC59F_85EF_49B8_B1F3_5F9B9ECF0053__INCLUDED_)
#define UTILS_H__9D4EC59F_85EF_49B8_B1F3_5F9B9ECF0053__INCLUDED_

#pragma once
#include <string>
#include "stdafx.h"
#include "Vector.h"
#include "Texture.h"
#include "Material.h"
#include "Common.h"
#include <math.h>

namespace S3MB
{
	#define snormSize 16

	// 判断是2的几次方
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
		// 创建guid
		static std::wstring CreateGuid();

		static std::wstring Base64Decode(const std::wstring& Data, int DataByte);

		static bool Bigendian();

		//! \brief OCT压缩器
		static void OctDecode(const short values[2], float vec[3]);
		static void OctEncode(const float vec[3], short values[2]);
	};

	class STK_API VectorUtils
	{
	public:
		//! \brief 求两个向量的叉积
		static Vector3d CrossProduct(const Vector3d &v1, const Vector3d &v2);

		//! \brief 求两个向量的点积
		static double DotProduct(const Vector3d& v1, const Vector3d& v2);
	};

	class STK_API MatrixUtils
	{
	public:
		//! \brief 矩阵平移。
		//! \param x X轴移动大小[in]。
		//! \param y Y轴移动大小[in]。
		//! \param z Z轴移动大小[in]。
		//! \return 平移后矩阵。
		static Matrix4d Translation(double x, double y, double z);

		//! \brief 旋转。
		//! \param rotationX 绕X轴旋转角度[in]。
		//! \param rotationY 绕Y轴旋转角度[in]。
		//! \param rotationZ 绕Z轴旋转角度[in]。
		//! \return 旋转后矩阵。
		static Matrix4d RotationXYZ(double rotationX, double rotationY, double rotationZ);

		//! \brief 绕X轴旋转。
		//! \param angle 绕X轴旋转角度[in]。
		//! \return 旋转后矩阵。		
		static Matrix4d RotationX(double angle);

		//! \brief 绕Y轴旋转。
		//! \param angle 绕Y轴旋转角度[in]。
		//! \return 旋转后矩阵。
		static Matrix4d RotationY(double angle);

		//! \brief 绕Z轴旋转。
		//! \param angle [in]。
		//! \return 旋转后矩阵。		
		static Matrix4d RotationZ(double angle);

		//! \brief 构建一个缩放矩阵
		//! param x X轴缩放大小[in]。
		//! param y Y轴缩放大小[in]。
		//! param z Z轴缩放大小[in]。
		//! return 缩放矩阵。
		static Matrix4d Scaling(double x, double y, double z);

		//! \brief  四元数转化成旋转矩阵
		static Matrix4d QuaternionToRotationMatrix(Vector4d quat);
	};

	class STK_API TextureUtils
	{
	public:
		static void UpdateTextureTier(TextureDataInfo*& pTexDataInfo, TextureCompressType enCompressType);

		//! \brief 对纹理数据进行缩放
		//! \brief nMaxW nMaxH 宽高限制； =0 表示不限制
		//! \brief 返回 comp
		static int ScaleTextureData(TextureData* pTextureData, const int nMaxW = 0, const int nMaxH = 0, bool bForceMax = false);

		//! \brief 调整贴图大小为2n次方
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
		//移除index对应的TextureUnitState
		static void RemoveTextureUnitState(Pass* pPass, unsigned short nIndex);
	};

	class STK_API ColorUtils
	{
	public:
		static unsigned int RGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

		static unsigned char RedValue(unsigned int clr);
		static unsigned char GreenValue(unsigned int clr);
		static unsigned char BlueValue(unsigned int clr);
		static unsigned char AlphaValue(unsigned int clr);
	};

	class STK_API ImageUtils
	{
	public:
		// RGB,RGBA 到 BGR,BRGA之间相互转换,直接改变原始数据
		// RGB时components = 3,RGBA components = 4
		static void ImageSwapRB(const unsigned int components, unsigned int widthin, unsigned int heightin, unsigned char* in, bool bIsContainMipmap = false);
	
	private:
		// RGB,RGBA 到 BGR,BRGA之间相互转换,直接改变原始数据
		// RGB时components = 3,RGBA components = 4
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

        // 从某个目录中查找符合给定规则的所有文件
        //! strFileList 得到的符合给定规则的文件名数组 [out]
        //! strPath 要查找的目录 [in]
        //! strPattern 给定的规则 [in]
        //! nFlags 判断的一些处理方式 [in]
        //! 返回找到符合规则的文件个数
        static int ListFiles(std::vector<std::wstring>& strFileList, const std::wstring& strPath,
            const std::wstring& strPattern = U("*"), unsigned int nFlags = LIST_MATCH_ALL);

        //! 得到临时路径
        //! 返回临时路径
        static std::wstring GetTemporaryPath();

    private:
        // Match filenames using *, ?, [^a-z], and so on
        //!  判断文件名是否符合一定的规则(模式)
        //!  strPattern 规定的模式
        //!  strFileName 要判断的文件名
        //!  nFlags 判断的一些处理方式
        //!  文件名符合规则返回true,否则返回false
        static bool Match(const std::wstring& strPattern, const std::wstring& strFileName,
            unsigned int nFlags = (FILEMATCH_NOESCAPE | FILEMATCH_FILE_NAME));

        static int ugfilematch(const wchar_t *pattern, const wchar_t *str, unsigned int flags = (FILEMATCH_NOESCAPE | FILEMATCH_FILE_NAME));
        static int DoMatch(const wchar_t *pattern, const wchar_t *str, unsigned int flags);
    };
}

#endif