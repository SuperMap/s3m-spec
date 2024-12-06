#if !defined(MATERIAL_H__746A408B_9321_4536_BA2C_49197406FB4D__INCLUDED_)
#define MATERIAL_H__746A408B_9321_4536_BA2C_49197406FB4D__INCLUDED_

#pragma once
#include "S3MBCommon.h"
#include "JsonValue.h"

namespace S3MB
{
	// PBR纹理运动参数
	struct S3MB_API TextureMotionParameter
	{
	public:
		// 纹理平移运动的参数
		UVMotionParameter m_OffsetMotion;
		// 纹理缩放的参数
		UVMotionParameter m_TilingMotion;

		TextureMotionParameter();

		TextureMotionParameter(const TextureMotionParameter& other);

		TextureMotionParameter& operator=(const TextureMotionParameter& other);

		~TextureMotionParameter();

		bool ToJson(JsonValue & json);

		bool FromJson(JsonValue & json);
	};

	class S3MB_API PBRParams
	{
	public:
		PBRParams();

		virtual MaterialParamType GetType() = 0;

	public:
		// 控制自发光强度的因子
		Vector3d m_vec3EmissiveFactor;
		// 自发光纹理在纹理数组中的序号 RGB纹理
		int m_nEmissiveTextureIndex;
		// 自发光纹理的纹理坐标在纹理坐标数组中的序号
		int m_nEmissiveTextureCoordIndex;
		// 自发光纹理运动的参数对象
		TextureMotionParameter m_EmissiveTextureMotion;

		// 法线纹理在纹理数组中的序号，物体表面的凹凸细节 float格式RGB纹理
		int m_nNormalTextureIndex;
		// 法线纹理的纹理坐标在纹理坐标数组中的序号
		int m_nNormalTextureCoordIndex;
		// 法线纹理的缩放因子
		float m_fNormalTextureScale;
		

		// 遮挡图在纹理数组中的序号，用于物体表面凹凸性对光照的影响，比如缝隙处就暗 float灰度纹理
		int m_nOcclusionTextureIndex;
		// 遮挡图的纹理坐标在纹理坐标数组中的序号
		int m_nOcclusionTextureCoordIndex;
		// 遮挡图的缩放因子
		float m_fOcclusionTextureStrength;

		// 掩膜纹理在纹理数组中的序号 RGB纹理
		int m_nMaskTextureIndex;
		// 掩膜纹理的纹理坐标在纹理坐标数组中的序号
		int m_nMaskTextureCoordIndex;

		// 语义纹理在纹理数组中的序号 RGB纹理
		int m_nSemanticTextureIndex;
		// 语义纹理的纹理坐标在纹理坐标数组中的序号
		int m_nSemanticTextureCoordIndex;
	};

	// 金属粗糙度模型
	class S3MB_API PBRMetallicRough : public PBRParams
	{
	public:
		PBRMetallicRough();

		virtual MaterialParamType GetType();

	public:
		// 物体的基本颜色
		Vector4d m_vec4BaseColor;
		// 物体基本颜色的纹理在纹理数组中的序号
		int m_nBaseColorTextureIndex;
		// 物体基本颜色的纹理坐标在纹理坐标数组中的序号
		int m_nBaseColorTextureCoordIndex;
		// 基色纹理运动的参数对象
		TextureMotionParameter m_BaseColorTextureMotion;
		// 金属度和粗糙性纹理在纹理数组中的序号(R通道存储金属度，G通道存储粗糙度)
		int m_nMetallicRoughnessTextureIndex;
		// 金属度和粗糙性纹理的纹理坐标在数组中的序号
		int m_nMetallicRoughnessTextureCoordIndex;
		// 控制金属性强弱的因子
		float m_fMetallicFactor;
		// 控制粗糙性强弱的因子
		float m_fRoughnessFactor;
	};

	class S3MB_API TextureUnitState
	{
	public:
		TextureUnitState();

	public:
		// 名称
		wstring m_strName;

		// 纹理单元使用的纹理名称
		wstring m_strTextureName;

		// 纹理引用的文件
		wstring m_strTexureURL;

		// 设置纹理坐标分配模式
		UVWAddressingMode m_AddressMode;

		// 纹理滤波的统一方式
		FilterOptions m_TextureFilteringOption;

		// 缩小时的滤波类型
		FilterOptions m_MinFilter;

		// 放大时的滤波类型
		FilterOptions m_MaxFilter;

		// 纹理矩阵
		mutable Matrix4d m_TexModMatrix;
	};

	class S3MB_API Pass
	{
	public:
		Pass();
		~Pass();
	public:
		// 通道名
		wstring m_strName;
		// 环境光
		ColorValue m_Ambient;
		// 散射光
		ColorValue m_Diffuse;
		// 反射光
		ColorValue m_Specular;
		// 发光，影响发射光点的大小
		float m_Shininess;
		// 裁剪模式
		CullingMode m_CullMode;
		// PBR材质
		PBRParams* m_pPRBMaterial;
		// texture unit states 的数组
		typedef std::vector<TextureUnitState*> TextureUnitStates;
		TextureUnitStates m_pTextureUnitStates;

		// 获得正面环绕模式
		// return 环绕模式
		int GetPFFMode();

		// 设置正面环绕模式
		// mode 环绕模式
		void SetPFFMode(PolygonFrontFace mode);

		TextureUnitState* CreateTextureUnitState();
		// 获取TextureUnitState的数量
		unsigned short GetTextureUnitStatesSize(void) const;

		// 获取TextureUnitState
		TextureUnitState* GetTextureUnitState(unsigned short index);
		// 获取TextureUnitState
		TextureUnitState* GetTextureUnitState(const std::wstring& name);
		// 当alphaMode为Mask时，着色器根据这个值和baseColor的Alpha值进行比较决定是否丢弃
		float m_fAlphaCutoff;
		// 使用的顶点着色器的名字
		wstring strVertexProgram;
		// 使用的片元着色器的名字
		wstring strFragmentProgram;
		// AlphaMode
		AlphaMode m_AlphaMode;

		// 扩展信息。可以自定义各类新的材质参数，如毛发，眼睛等材质的特有参数
		wstring m_strExtensions;
	};

	class S3MB_API Technique
	{
	public:
		Technique();
		~Technique();
		Pass* CreatePass();
		typedef vector<Pass*> Passes;
		// pass列表
		Passes mPasses;
		// 获取pass数量
		unsigned short getNumPasses(void) const;
		// 根据索引获取指定pass
		Pass* getPass(unsigned short index);
		// 根据名称获取指定数据
		Pass* getPass(const wstring& name);
		// 删除所有pass
		void removeAllPasses(void);
	};

	typedef vector<Technique*> Techniques;
	class S3MB_API Material
	{
	public:
		Material();
		~Material();
		Technique* CreateTechnique();

		// 获取Technique数量
		int getNumTechniques(void) const;
		// 根据索引获取指定Technique
		Technique* getTechnique(unsigned short index);
		// 移除所有Technique
		void removeAllTechniques(void);

	public:
		wstring m_strName;
		Techniques mTechniques;
	};
}

#endif