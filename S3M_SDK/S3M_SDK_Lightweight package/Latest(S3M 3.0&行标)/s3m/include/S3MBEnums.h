#if !defined(S3MBENUMS_H__5152B939_374B_4E1E_971E_6A82B1FB7D75__INCLUDED_)
#define S3MBENUMS_H__5152B939_374B_4E1E_971E_6A82B1FB7D75__INCLUDED_

#pragma once

namespace S3MB
{
	// LOD�л�ģʽ
	enum RangeMode
	{
		Distance_From_EyePoint,  // ���ݵ�����ľ����л�
		Pixel_Size_OnScreen,	 // ������Ļ���ش�С�л�
		Geometric_Error			 // ���ݼ����л����
	};

	// ���ݻ��Ƶ�����
	enum VertexOptions
	{
		VO_NORMALS = 1,                  // GL_NORMAL_ARRAY + glNormalPointer()
		VO_TEXTURE_COORDS = 2,           // GL_TEXTURE_COORD_ARRAY + glTexCoordPointer()
		VO_DIFFUSE_COLOURS = 4,          // GL_COLOR_ARRAY + glColorPointer()
		VO_SPECULAR_COLOURS = 8,         // Secondary Color
		VO_BLEND_WEIGHTS = 16,
		VO_USE_SINGLE_COLOR = 32,        // use only one kind of color
		VO_USE_POINT_SMOOTHING = 64,     // �����㷴����
		VO_MATERIAL = 128,               // ʹ�ò���
		VO_TEXTURE_COLOR = 256,          // ʹ�ò���
		VO_VERTEX_DOUBLE = 512,          // ʹ��Double���͵Ķ���
		VO_TEXTURE_COORD_Z_IS_MATRIX = 1024,  // ��ʾ�������Ե�Zֵ��һ������
	};

	enum VertexCompressOptions
	{
		SVC_Vertex = 1,						// �����ѹ��
		SVC_Normal = 2,						// ���ߴ�ѹ��
		SVC_VertexColor = 4,				// ������ɫ��ѹ��
		SVC_SecondColor = 8,				// SecondColor��ѹ��
		SVC_TexutreCoord = 16,				// ���������ѹ��
		SVC_TexutreCoordIsW = 32,			// ��ʾ��һ����������洢�����Wλ
	};

	// ����������
	enum VertexIndexType
	{
		IT_16BIT,
		IT_32BIT
	};

	// ���Ե�����
	enum VertexAttributeType
	{
		AT_32BIT = 0,
		AT_FLOAT = 1,
		AT_DOUBLE = 2,
		AT_16BIT = 3,
		AT_8BIT = 4,
	};

	// ���ݻ��Ƶķ�ʽ
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

	// ��Ⱦ�����õĵĲü�ģʽ
	enum CullingMode
	{
		// Hardware never culls triangles and renders everything it receives.
		CULL_NONE = 1,
		// Hardware culls triangles whose vertices are listed clockwise in the view (default).
		CULL_CLOCKWISE = 2,
		// Hardware culls triangles whose vertices are listed anticlockwise in the view.
		CULL_ANTICLOCKWISE = 3
	};

	// ��Ⱦ�����õĵ�������滷�Ʒ���
	enum PolygonFrontFace
	{
		// ���涼��
		PFF_NONE = 1,
		// ˳ʱ��
		PFF_CW = 2,
		// ��ʱ��.
		PFF_CCW = 3,
		// ��ʾʹ������Ĭ�ϵĻ��Ʒ�ʽ
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
		// �����ֲڶ�ģ��
		MAT_PBRMetallicRough = 1,
		// �����߹�ģ��
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
		// ƬԪ��alphaֵ���Ϊ1.0,
		PBRAM_OPAQUE = 1,
		// ƬԪ����alpha����
		PBRAM_MASK = 2,
		// ƬԪ��alphaֵ���baseColor��alphaֵ
		PBRAM_BLEND = 3,
	};

	enum TextureAddressingMode
	{
		TAM_WRAP,

		TAM_MIRROR,

		TAM_CLAMP,

		TAM_BORDER,
	};

	//! ��������
	enum TextureCompressType
	{
		//! ��ʹ�ñ��뷽ʽ
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
		// 24λ,���ɫ
		PF_RGB8 = 32849,
		// 32λ,��ǿ���ɫ
		PF_RGBA8 = 32856,
		// 96λ��ʽ��Rͨ��32λ��float����Bͨ��32λ��float����Gͨ��32λ��float��
		PF_RGB32F = 34837,
		// 128λ��ʽ��Rͨ��32λ��float����Bͨ��32λ��float����Gͨ��32λ��float��, Aͨ��32λ��float��
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

	// ֧�ֵ�����
	enum VarType
	{
		// δ���������
		VT_Null = 0,
		// ���ֽ�����
		VT_Byte = 1,
		// ˫�ֽ���������
		VT_Short = 2,
		// ���ֽ���������
		VT_Integer = 3,
		// ���ֽ���������
		VT_Long = 4,
		// ���ֽڸ�������
		VT_Float = 5,
		// ���ֽڸ�������
		VT_Double = 6,
		// ʱ������
		VT_Time = 7,
		// ����������
		VT_Binary = 8,
		// �ַ���
		VT_String = 9,
		// ��������
		VT_Date = 10,
		// ʱ������� 
		VT_TimeStamp = 11,
		// ��������
		VT_Boolean = 12
	};// Ϊ����FieldInfo�е���������ʱ���������ֿ�����ԭ����time������չΪtime date timestamp������ο�FieldInfo�ж�Ӧ������

	enum S3MBVertexTag
	{
		SV_Unkown = 0,				// δ֪
		SV_Standard = 1,			// ��������
		// SV_Compressed = 2,		
		SV_DracoCompressed = 3,		// draco ѹ��
		// SV_Dracoed = 4,			
	};

	enum SCPSDataType
	{
		SDT_Unknown,
		SDT_RealityMesh,			// ��б
		SDT_BIM,					// BIM
		SDT_PointCloud,				// ����
		SDT_Vector,					// ʸ��
		SDT_Network,				// ����
		SDT_InstanceModel,			// ʵ����
		SDT_ArtificialModel			// ��ģ
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

	// �ֶ�����
	enum FieldType
	{
		// ��Ч���ֶ����͡�
		FT_UnKnown = 0,
		// ����ֵ�����ֽڣ�true,false��
		FT_Boolean = 1,
		// �޷��ŵ��ֽڣ�0-255�� 
		FT_Byte = 2,
		// �����ͣ�2�ֽڡ�
		FT_INT16 = 3,
		// �����ͣ�4�ֽڡ�
		FT_INT32 = 4,
		// �����ͣ�8�ֽڡ�
		FT_INT64 = 5,
		// �����ȸ����ͣ�4�ֽڡ�
		FT_Float = 6,
		// ˫���ȸ����ͣ�8�ֽڡ�
		FT_Double = 7,
		// �����ͣ��ꡢ�¡��գ�����ʱ�䡣
		FT_Date = 8,
		// �������ַ����͡�
		FT_Text = 9,
		// ʱ���ͣ�Сʱ���֡��룬�������ڡ�
		FT_Time = 10,
		// ʱ����ͣ��ꡢ�¡��ա�Сʱ���֡��롣
		FT_TimeStamp = 11,
	};
}

#endif