#if !defined(S3MBENUMS_H__5152B939_374B_4E1E_971E_6A82B1FB7D75__INCLUDED_)
#define S3MBENUMS_H__5152B939_374B_4E1E_971E_6A82B1FB7D75__INCLUDED_

#pragma once

namespace S3MB
{
	// LOD切换模式
	enum RangeMode
	{
		Distance_From_EyePoint,  // 根据到相机的距离切换
		Pixel_Size_OnScreen,	 // 根据屏幕像素大小切换
		Geometric_Error			 // 根据几何切换误差
	};

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
		SVC_Vertex = 1,						// 顶点带压缩
		SVC_Normal = 2,						// 法线带压缩
		SVC_VertexColor = 4,				// 顶点颜色带压缩
		SVC_SecondColor = 8,				// SecondColor带压缩
		SVC_TexutreCoord = 16,				// 纹理坐标带压缩
		SVC_TexutreCoordIsW = 32,			// 表示第一重纹理坐标存储顶点的W位
	};

	// 索引的类型
	enum VertexIndexType
	{
		IT_16BIT,
		IT_32BIT
	};

	// 属性的类型
	enum VertexAttributeType
	{
		AT_32BIT = 0,
		AT_FLOAT = 1,
		AT_DOUBLE = 2,
		AT_16BIT = 3,
		AT_8BIT = 4,
	};

	// 数据绘制的方式
	enum OperationType
	{

		OT_POINT_LIST = 1,

		OT_LINE_LIST = 2,

		OT_LINE_STRIP = 3,

		OT_TRIANGLE_LIST = 4,

		OT_TRIANGLE_STRIP = 5,

		OT_TRIANGLE_FAN = 6,

		OT_QUAD_STRIP = 8,

		OT_QUAD_LIST = 9,

		OT_POLYGON = 10,

	};

	// 渲染引擎用的的裁剪模式
	enum CullingMode
	{
		// Hardware never culls triangles and renders everything it receives.
		CULL_NONE = 1,
		// Hardware culls triangles whose vertices are listed clockwise in the view (default).
		CULL_CLOCKWISE = 2,
		// Hardware culls triangles whose vertices are listed anticlockwise in the view.
		CULL_ANTICLOCKWISE = 3
	};

	// 渲染引擎用的的面的正面环绕方向
	enum PolygonFrontFace
	{
		// 两面都有
		PFF_NONE = 1,
		// 顺时针
		PFF_CW = 2,
		// 逆时针.
		PFF_CCW = 3,
		// 表示使用数据默认的环绕方式
		PFF_DEFAULT = 4,
	};

	enum MaterialType
	{
		MAT_COMMON = 0,
		MAT_PBR = 1
	};

	enum MaterialParamType
	{
		MAT_Unknown = 0,
		// 金属粗糙度模型
		MAT_PBRMetallicRough = 1,
		// 镜面光高光模型
		MAT_PBRSpecularGlossy = 2,
	};

	// Filtering options for textures / mipmaps. 
	enum FilterOptions
	{
		// No filtering, used for FILT_MIP to turn off mipmapping
		FO_NONE,
		// Use the closest pixel
		FO_POINT,
		// Average of a 2x2 pixel area, denotes bilinear for MIN and MAG, trilinear for MIP
		FO_LINEAR,
		// Equal to: min=FO_LINEAR, mag=FO_LINEAR, mip=FO_LINEAR
		FO_TRILINEAR,
		// Similar to FO_LINEAR, but compensates for the angle of the texture plane
		FO_ANISOTROPIC
	};

	enum AlphaMode
	{
		PBRAM_UnKnown = 0,
		// 片元中alpha值输出为1.0,
		PBRAM_OPAQUE = 1,
		// 片元进行alpha过滤
		PBRAM_MASK = 2,
		// 片元的alpha值输出baseColor的alpha值
		PBRAM_BLEND = 3,
	};

	enum TextureAddressingMode
	{
		TAM_WRAP,

		TAM_MIRROR,

		TAM_CLAMP,

		TAM_BORDER,
	};

	//! 编码类型
	enum TextureCompressType
	{
		//! 不使用编码方式
		TC_NONE = 0,

		TC_DXT1_RGB = 33776,

		TC_DXT1_RGBA = 33777,

		TC_DXT3 = 33778,

		TC_DXT5 = 33779,

		TC_PVR = 35843,

		TC_ETC1 = 36196,

		TC_ETC2 = 36496,

		TC_WEBP = 38000,

		TC_CRN = 38001,

		TC_KTX2 = 38002,
	};

	enum PixelFormat
	{
		// 24位,真彩色
		PF_RGB8 = 32849,
		// 32位,增强真彩色
		PF_RGBA8 = 32856,
		// 96位格式，R通道32位（float），B通道32位（float），G通道32位（float）
		PF_RGB32F = 34837,
		// 128位格式，R通道32位（float），B通道32位（float），G通道32位（float）, A通道32位（float）
		PF_RGBA32F = 34836
	};

	enum JsonValueType
	{
		nullValue = 0, /// < 'null' value
		intValue,      /// < signed integer value
		uintValue,     /// < unsigned integer value
		realValue,     /// < double value
		stringValue,   /// < UTF-8 string value
		booleanValue,  /// < bool value
		arrayValue,    /// < array value (ordered list)
		objectValue    /// < object value (collection of name/value pairs).
	};

	enum StringEncodeType
	{
		UTF8,
		Ansi,
		Unicode
	};

	// 支持的类型
	enum VarType
	{
		// 未定义的类型
		VT_Null = 0,
		// 单字节类型
		VT_Byte = 1,
		// 双字节整型类型
		VT_Short = 2,
		// 四字节整型类型
		VT_Integer = 3,
		// 八字节整型类型
		VT_Long = 4,
		// 四字节浮点类型
		VT_Float = 5,
		// 八字节浮点类型
		VT_Double = 6,
		// 时间类型
		VT_Time = 7,
		// 二进制类型
		VT_Binary = 8,
		// 字符串
		VT_String = 9,
		// 日期类型
		VT_Date = 10,
		// 时间戳类型 
		VT_TimeStamp = 11,
		// 布尔类型
		VT_Boolean = 12
	};// 为了与FieldInfo中的三种日期时间类型区分开，把原来的time类型扩展为time date timestamp，定义参考FieldInfo中对应的类型

	enum S3MBVertexTag
	{
		SV_Unkown = 0,				// 未知
		SV_Standard = 1,			// 正常数据
		// SV_Compressed = 2,		
		SV_DracoCompressed = 3,		// draco 压缩
		// SV_Dracoed = 4,			
	};

	enum SCPSDataType
	{
		SDT_Unknown,
		SDT_RealityMesh,			// 倾斜
		SDT_BIM,					// BIM
		SDT_PointCloud,				// 点云
		SDT_Vector,					// 矢量
		SDT_Network,				// 网络
		SDT_InstanceModel,			// 实例化
		SDT_ArtificialModel			// 精模
	};

	enum SCPSPyramidSplitType
	{
		SSP_Unknown,
		SSP_Octree,
		SSP_QuadTree
	};

	enum SCPSLODType
	{
		SLT_Unknown,
		SLT_Add,
		SLT_Replace,
	};

	enum PrjCoordSys
	{
		GCS_WGS_1984 = 4326,
	};

	// 字段类型
	enum FieldType
	{
		// 无效的字段类型。
		FT_UnKnown = 0,
		// 布尔值，单字节，true,false。
		FT_Boolean = 1,
		// 无符号单字节，0-255。 
		FT_Byte = 2,
		// 短整型，2字节。
		FT_INT16 = 3,
		// 长整型，4字节。
		FT_INT32 = 4,
		// 长整型，8字节。
		FT_INT64 = 5,
		// 单精度浮点型，4字节。
		FT_Float = 6,
		// 双精度浮点型，8字节。
		FT_Double = 7,
		// 日期型，年、月、日，不带时间。
		FT_Date = 8,
		// 不定长字符串型。
		FT_Text = 9,
		// 时间型，小时、分、秒，不带日期。
		FT_Time = 10,
		// 时间戳型，年、月、日、小时、分、秒。
		FT_TimeStamp = 11,
	};
}

#endif