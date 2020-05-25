#ifndef  _SKELETON_H_
#define  _SKELETON_H_

#include <iostream>
#include <vector>
//#include <wtypesbase.h>
#include "MemoryStream.h"
using namespace std;
namespace S3MB
{

#define DBLMAX 1.7976931348623158e+308 /* max value */
#define DBLMIN 2.2250738585072014e-308 /* min positive value */
#define FLTMAX 3.402823466e+38F  /* max value */
#define FLTMIN 1.175494351e-38F  /* min positive value */

#ifndef IS0(x)
	//! \brief  极小值定义
#define EP 1e-10
#define NEP -1e-10
#define IS0(x) (((x) < EP) && ((x) > NEP))
#endif // !IS0(x)

// 多重纹理最大个数。
#define SMSCN_MAX_TEXTURE_COORD_SETS 8

#define SIZE_PER_INSTANCE 17
#define SIZE_PER_DM_INSTANCE  29

// 数据绘制的属性
	enum VertexOptions
	{
		VO_NORMALS = 1,                  // GL_NORMAL_ARRAY + glNormalPointer()
		VO_TEXTURE_COORDS = 2,           // GL_TEXTURE_COORD_ARRAY + glTexCoordPointer()
		VO_DIFFUSE_COLOURS = 4,          // GL_COLOR_ARRAY + glColorPointer()
		VO_SPECULAR_COLOURS = 8,         // Secondary Color
		VO_BLEND_WEIGHTS = 16,
		VO_USE_SINGLE_COLOR = 32,        // use only one kind of color
		VO_USE_POINT_SMOOTHING = 64,     // 启动点反走样
		VO_MATERIAL = 128,               // 使用材质
		VO_TEXTURE_COLOR = 256,          // 使用材质
		VO_VERTEX_DOUBLE = 512,          // 使用Double类型的顶点
		VO_TEXTURE_COORD_Z_IS_MATRIX = 1024,  // 表示顶点属性的Z值是一个矩阵
	};

	enum VertexCompressOptions
	{
		SVC_Vertex = 1,				//顶点带压缩
		SVC_Normal = 2,				//法线带压缩
		SVC_VertexColor = 4,				//顶点颜色带压缩
		SVC_SecondColor = 8,				//SecondColor带压缩
		SVC_TexutreCoord = 16,				//纹理坐标带压缩
		SVC_TexutreCoordIsW = 32,  // 表示第一重纹理坐标存储顶点的W位
	};

	// 索引的类型
	enum VertexIndexType
	{
		IT_16BIT,
		IT_32BIT,

		//! \brief 附带属性的索引号
		IT_16BIT_2,
		IT_32BIT_2,
	};

	// 数据绘制的方式
	enum OperationType
	{
		/// A list of points, 1 vertex per point
		OT_POINT_LIST = 1,
		/// A list of lines, 2 vertices per line
		OT_LINE_LIST = 2,
		/// A strip of connected lines, 1 vertex per line plus 1 start vertex
		OT_LINE_STRIP = 3,
		/// A list of triangles, 3 vertices per triangle
		OT_TRIANGLE_LIST = 4,
		/// A strip of triangles, 3 vertices for the first triangle, and 1 per triangle after that 
		OT_TRIANGLE_STRIP = 5,
		/// A fan of triangles, 3 vertices for the first triangle, and 1 per triangle after that
		OT_TRIANGLE_FAN = 6,

		OT_QUAD_STRIP = 8,

		OT_QUAD_LIST = 9,

		OT_POLYGON = 10,

		OT_PICTURE = 12,

	};

	// 四维矩阵
	class Matrix4d
	{
	public:
		Matrix4d();
		Matrix4d(
			double m00, double m01, double m02, double m03,
			double m10, double m11, double m12, double m13,
			double m20, double m21, double m22, double m23,
			double m30, double m31, double m32, double m33);
		//4X4零矩阵
		//所有元素为零
		static const Matrix4d ZERO;
		//4X4 单位矩阵
		//对角线元素为1，其他为0
		static const Matrix4d IDENTITY;

		//! \brief 重载*，完成矩阵与矩阵相乘。
		//! \param &m2 乘数矩阵[in]。
		//! \return 运算完成后矩阵。
		Matrix4d operator * (const Matrix4d& m2) const;

		//! \brief 矩阵相乘。
		//! \param &m2 另外一个4X4矩阵[in]。
		//! \return 完成运算后4X4矩阵。
		Matrix4d Concatenate(const Matrix4d& m2) const;

		//! \brief 重载[]，取出对应行。
		//! \param iRow 行号，有效范围为零到三[in]。
		//! \return 矩阵指定一行数据，返回行数据指针。
		double* operator[] (unsigned int iRow);

		//! \brief 取出只读矩阵行数据。
		//! \param iRow 行号，有效范围为零到三[in]。
		//! \return 行数据。
		const double* const operator [] (unsigned int iRow) const;

		//! \brief 判断两个矩阵是否相等。
		//! \param &m2 另外一个矩阵[in]。
		//! \return 若相等返回为true，若不相等则返回为false。
		bool operator == (const Matrix4d& m2) const;

		//! \brief 判断两个矩阵是否不相等。
		//! \param &m2 另外一个矩阵[in]。
		//! \return 若不相等返回为true，否则返回false。
		bool operator != (const Matrix4d& m2) const;

		double m[4][4];

		//! \brief 获取矩阵的指针
		double* GetPointer() { return (double*)&m; }
	};

	// 三维向量
	struct Vector3d
	{
		Vector3d() :
			x(0), y(0), z(0) {}

		Vector3d(double fx, double fy, double fz) :
			x(fx), y(fy), z(fz) {}

		//! \brief 重载的取值运算符，取索引i处的分量
		double operator [] (const unsigned i) const;

		//! \brief 重载的取值运算符，取索引i处的分量的引用
		double& operator [] (const unsigned i);

		//! \brief 重载的加法运算符，求两个向量之和
		Vector3d operator + (const Vector3d& vec) const;

		//! \brief 重载的减法运算符，求两个向量之差
		Vector3d operator - (const Vector3d& vec) const;

		//! \brief 求向量的长度
		double Length() const;

		//! \brief 求向量的长度的平方
		double Length2() const;

		//! \brief 取两个向量每个分类的最小值形成一个新的向量
		void MakeFloor(const Vector3d& cmp);

		//! \brief 取两个向量每个分类的最大值形成一个新的向量
		void MakeCeil(const Vector3d& cmp);

		//! \brief 该乘法支持D3D矩阵运算
		//! \brief 本系统可以直接使用
		Vector3d MultiplyMatrix(const Matrix4d& m) const;

		void Normalize();

		double x, y, z;
	};

	// 四维向量
	struct Vector4d
	{
		Vector4d();

		Vector4d(const double fx, const double fy, const double fz, const double fw) :
			x(fx), y(fy), z(fz), w(fw) {}

		//! \brief 重载的取值运算符，取索引i处的分量
		double operator [] (const unsigned i) const;

		//! \brief 重载的取值运算符，取索引i处的分量的引用
		double& operator [] (const unsigned i);

		double x, y, z, w;
	};

	class BoundingBox
	{
	public:
		BoundingBox();
		BoundingBox(Vector3d vMin, Vector3d vMax);

		//! \brief 判断是否是合法实数。
	//! \param dValue 实数[in]。
	//! \return true为合法，false为非法。
		bool IsRealNaN(double dValue);

		//! \brief 判断有效性
		//! \return 包围盒是否有效
		bool IsVaild();

		//! \brief 将两个包围盒进行合并。
		//! \param BoundBox 要进行合并的包围盒[in]。
		void Merge(const BoundingBox& BoundBox);

		//构成包围盒的最小向量。
		//! \brief 设置构成包围盒的最小向量。
		//! \param vMin 构成包围盒的最小向量[in]。
		void SetMin(Vector3d vMin);

		//! \brief 设置构成包围盒的最大向量。
		//! \param vMax 构成包围盒的最大向量[in]。
		void SetMax(Vector3d vMax);

		//! \brief 设置构成包围盒的最小向量。
		//! \param x,y,z 构成包围盒的最小向量[in]。
		void SetMin(double x, double y, double z);

		//! \brief 设置构成包围盒的最大向量。
		//! \param x,y,z 构成包围盒的最大向量[in]。
		void SetMax(double x, double y, double z);

		//! \brief 设置构成包围盒的最大、最小向量。
		//! \param vMin 构成包围盒的最大最小向量[in]。
		//! \param vMax 构成包围盒的最大向量[in]。
		void SetExtents(const Vector3d& vMin, const Vector3d& vMax);

		//! \brief 设置构成包围盒的最大、最小向量。
		//! \param (minx,miny,minz) 构成包围盒的最大最小向量[in]。
		//! \param (maxx,maxy,maxz) 构成包围盒的最大向量[in]。
		void SetExtents(double minx, double miny, double minz,
			double maxx, double maxy, double maxz);

		//! \brief 设置包围盒为空
		void SetNULL(void) { m_bNull = true; }

		//! \brief  得到构成包围盒的最小向量
		const Vector3d& GetMin(void) const;

		//! \brief 得到构成包围盒的最大向量
		const Vector3d& GetMax(void) const;

		//! \brief 得到包围盒的中心点。
	//! \remarks 这个中心点是构成包围盒的最小向量和最大向量的平均值。
		Vector3d GetCenter() const;

		//! \brief 对包围盒进行矩阵变换
		//! \param matrix 对包围盒进行变换的矩阵[in]。
		void Transform(Matrix4d& matrix);
	protected:
		//! \brief 根据最大最小向量更新包围盒的8个角点。
		void UpdateCorners(void);
	private:
		Vector3d m_vMin
			;
		//构成包围盒的最大向量。
		Vector3d m_vMax;

		//构成包围盒8个角点。
		Vector3d m_vCorner[8];

		//标志包围盒是否为空。
		bool m_bNull;
	};

	class Rect2D
	{
	public:
		//! \brief 构造函数一个二维矩形对象
		Rect2D() { left = right = top = bottom = 0; }
		Rect2D(double dLeft, double dTop, double dRight, double dBottom)
			:left(dLeft), top(dTop), right(dRight), bottom(dBottom) {}
	public:
		//! \brief 矩形左边界
		double left;
		//! \brief 矩形上边界
		double top;
		//! \brief 矩形右边界。left<right
		double right;
		//! \brief 矩形下边界。bottom < top
		double bottom;
	};
	// 顶点包
	template<typename T>
	class VertexDataPackageTemplate
	{
	public:
		VertexDataPackageTemplate();
		~VertexDataPackageTemplate();
		void clear();
	public:
		//数据绘制属性
		int m_nVertexOptions;
		//数据压缩标记
		unsigned int m_nCompressOptions;
		//是否有指定压缩项
		bool HasCompressOptions(VertexCompressOptions enOptions)
		{
			return (m_nCompressOptions & enOptions) == enOptions;
		}

		//顶点个数
		unsigned int m_nVerticesCount;
		//顶点维数
		unsigned short m_nVertexDimension;
		//顶点偏移
		unsigned short m_nVertexStride;
		//顶点数组
		T* m_pVertices;
		//! \brief 顶点各分量的最小值
		float m_minVerticesValue[4];
		//! \brief 顶点压缩归一化常量
		float m_fVertCompressConstant;

		//法线个数
		unsigned int m_nNormalCount;
		//法线维数
		unsigned short m_nNormalDimension;
		//法线偏移
		unsigned short m_nNormalStride;
		//法线数组
		float* m_pNormals;

		//顶点颜色个数
		unsigned int m_nVertexColorCount;
		//顶点颜色偏移
		unsigned short m_nVertexColorStride;
		//顶点颜色数组
		unsigned int* m_pVertexColor;

		//顶点第二颜色个数
		unsigned int m_nSecondVertexColorCount;
		//顶点第二颜色偏移
		unsigned short m_nSecondVertexColorStride;
		//顶点第二颜色数组
		unsigned int* m_pSecondVertexColor;

		//贴图坐标数目
		unsigned int m_TexCoordCount[SMSCN_MAX_TEXTURE_COORD_SETS];

		//贴图坐标的维数
		unsigned short m_nTexDimensions[SMSCN_MAX_TEXTURE_COORD_SETS];

		//每组贴图坐标的Stride
		unsigned short m_TexCoordStride[SMSCN_MAX_TEXTURE_COORD_SETS];

		//贴图坐标数据
		float* m_pTexCoords[SMSCN_MAX_TEXTURE_COORD_SETS];

		//! \brief 纹理坐标各分量的最小值
		float m_minTexCoordValue[SMSCN_MAX_TEXTURE_COORD_SETS][4];

		//! \brief 纹理坐标压缩归一化常量
		float m_texCoordCompressConstant[SMSCN_MAX_TEXTURE_COORD_SETS];

		void SetVertexNum(unsigned int nVertexCount, unsigned short nVertexStride = 0)
		{
			m_nVerticesCount = nVertexCount;
			if (m_pVertices != NULL)
			{
				delete[] m_pVertices;
				m_pVertices = NULL;
			}
			m_pVertices = new T[nVertexCount * m_nVertexDimension];
			memset(m_pVertices, 0, sizeof(T) * nVertexCount * m_nVertexDimension);
			m_nVertexStride = nVertexStride;
		}
		void SetNormalNum(unsigned int nNormalCount, unsigned short nNormalStride = 0)
		{
			m_nNormalCount = nNormalCount;
			if (m_pNormals != NULL)
			{
				delete[] m_pNormals;
				m_pNormals = NULL;
			}
			m_nNormalStride = nNormalStride;
			if (nNormalCount == 0)
			{
				return;
			}

			m_pNormals = new float[nNormalCount * m_nNormalDimension];
			memset(m_pNormals, 0, sizeof(float) * nNormalCount * m_nNormalDimension);
		}
		void SetColorNum(unsigned int nColorCount, unsigned short nColorStride = 0)
		{
			m_nVertexColorCount = nColorCount;
			if (m_pVertexColor != NULL)
			{
				delete[] m_pVertexColor;
				m_pVertexColor = NULL;
			}
			m_pVertexColor = new unsigned int[nColorCount];
			memset(m_pVertexColor, 0, sizeof(unsigned int) * nColorCount);
			m_nVertexColorStride = nColorStride;
		}
		void SetSecondColorNum(unsigned int nColorCount, unsigned short nColorStride = 0)
		{
			m_nSecondVertexColorCount = nColorCount;


			if (m_pSecondVertexColor != NULL)
			{
				delete[] m_pSecondVertexColor;
				m_pSecondVertexColor = NULL;
			}
			m_pSecondVertexColor = new unsigned int[nColorCount];
			memset(m_pSecondVertexColor, 0, sizeof(unsigned int) * nColorCount);

			m_nSecondVertexColorStride = nColorStride;
		}
		bool SetTexCoordsNum(unsigned short nTexUnit, unsigned int nTexCoordCount, unsigned short nTexCoordStride = 0)
		{
			if (nTexUnit >= SMSCN_MAX_TEXTURE_COORD_SETS)
			{
				return false;
			}
			m_TexCoordCount[nTexUnit] = nTexCoordCount;

			if (m_pTexCoords[nTexUnit] != NULL)
			{
				delete[] m_pTexCoords[nTexUnit];
				m_pTexCoords[nTexUnit] = NULL;
			}
			m_pTexCoords[nTexUnit] = new float[nTexCoordCount * m_nTexDimensions[nTexUnit]];
			memset(m_pTexCoords[nTexUnit], 0, sizeof(float) * nTexCoordCount * m_nTexDimensions[nTexUnit]);
			m_TexCoordStride[nTexUnit] = nTexCoordStride;
			return true;
		}
	};
	template<typename T>
	void VertexDataPackageTemplate<T>::clear()
	{
		if (m_pVertices != NULL)
		{
			delete[] m_pVertices;
			m_pVertices = NULL;
		}
		m_nVerticesCount = 0;

		if (m_pNormals != NULL)
		{
			delete[] m_pNormals;
			m_pNormals = NULL;
		}
		m_nNormalCount = 0;

		if (m_pVertexColor != NULL)
		{
			delete[] m_pVertexColor;
			m_pVertexColor = NULL;
		}
		if (m_pSecondVertexColor != NULL)
		{
			delete[] m_pSecondVertexColor;
			m_pSecondVertexColor = NULL;
		}

		m_nVertexColorCount = 0;
		m_nSecondVertexColorCount = 0;

		unsigned short i = 0;
		for (i = 0; i < SMSCN_MAX_TEXTURE_COORD_SETS; i++)
		{
			if (m_pTexCoords[i] != NULL)
			{
				delete[] m_pTexCoords[i];
				m_pTexCoords[i] = NULL;
			}
			m_TexCoordCount[i] = 0;
		}
	}
	template<typename T>
	VertexDataPackageTemplate<T>::VertexDataPackageTemplate()
	{
		m_nCompressOptions = 0;
		m_nVertexDimension = 3;
		m_nNormalDimension = 3;
		m_pVertices = NULL;
		m_nVerticesCount = 0;
		m_nVertexStride = 0;
		m_fVertCompressConstant = 0.0;
		m_pNormals = NULL;
		m_nNormalCount = 0;
		m_nNormalStride = 0;
		m_pVertexColor = NULL;
		m_pSecondVertexColor = NULL;
		m_nVertexColorCount = 0;
		m_nSecondVertexColorCount = 0;
		m_nVertexColorStride = 0;
		m_nSecondVertexColorStride = 0;

		m_nVertexOptions = sizeof(T) == sizeof(double) ? VO_VERTEX_DOUBLE : 0;
		m_nVertexOptions = m_nVertexOptions | VO_NORMALS | VO_DIFFUSE_COLOURS | VO_TEXTURE_COORDS;

		unsigned short i = 0;
		for (i = 0; i < SMSCN_MAX_TEXTURE_COORD_SETS; i++)
		{
			m_pTexCoords[i] = NULL;
			m_TexCoordStride[i] = 0;
			m_TexCoordCount[i] = 0;
			m_nTexDimensions[i] = 2;
			m_texCoordCompressConstant[i] = -1.0;
			m_minTexCoordValue[i][0] = m_minTexCoordValue[i][1] = m_minTexCoordValue[i][2] = m_minTexCoordValue[i][3] = 0.0;
		}
	}

	template<typename T>
	VertexDataPackageTemplate<T>::~VertexDataPackageTemplate()
	{
		clear();
	}

	// float精度的顶点
	typedef VertexDataPackageTemplate<float> VertexDataPackage;

	// double精度的顶点
	typedef VertexDataPackageTemplate<double> VertexDataPackageExact;

	//索引包
	struct IndexPackage
	{
		IndexPackage();
		~IndexPackage();
		//索引数目
		unsigned int m_nIndexesCount;

		//索引数组
		unsigned short* m_pIndexes;

		//是否使用无符整型数据类型
		VertexIndexType m_enIndexType;

		//是否使用索引
		bool m_bUseIndex;

		//使用的Pass的名称数组
		vector<string> m_strPassName;

		//数据绘制的方式
		OperationType m_OperationType;

		//加载索引信息
		bool Load(MemoryStream& stream);
		void Save(MemoryStream& stream);
		void SetIndexNum(int nIndexNum);
	};

	//骨架类
	class Geometry
	{
	public:
		Geometry();
		~Geometry();
		//名字
		string m_strGeoName;

		//模型顶点信息
		VertexDataPackage* m_pVertexDataPackage;

		//索引信息
		vector<IndexPackage*> m_arrIndexPackage;

		//是否是一个实例批次
		bool m_bInstanceBatch;

		//材质名
		string m_strMaterialName
			;
		//法线是否需要压缩
		bool m_bNormalDecode;

		//位置矩阵
		Matrix4d m_matWorldView;

		//索引次数
		int m_nUseIndex;

		//包围盒
		BoundingBox m_BoundingBox;

		//计算包围盒
		void ComputerBoundingBox();

	};
}
#endif