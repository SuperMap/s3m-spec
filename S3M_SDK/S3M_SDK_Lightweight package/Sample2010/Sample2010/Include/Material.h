#ifndef  _MATERIAL_H_
#define  _MATERIAL_H_

#include "JsonParser.h"
#include "Skeleton.h"
#define S3MB_MATERIAL	 "material"
using namespace std;
namespace S3MB
{


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
	//! \brief 顶点颜色跟踪的材质属性
	enum TrackVertexColourType
	{
		TVC_NONE = 0x0,
		TVC_AMBIENT = 0x1,
		TVC_DIFFUSE = 0x2,
		TVC_SPECULAR = 0x4,
		TVC_EMISSIVE = 0x8
	};
	//! \brief 渲染引擎用的的面的正面环绕方向
	enum PolygonFrontFace
	{
		//! \brief 两面都有
		PFF_NONE = 1,
		//! \brief 顺时针
		PFF_CW = 2,
		//! \brief 逆时针.
		PFF_CCW = 3,
		//! \brief 表示使用数据默认的环绕方式
		PFF_DEFAULT = 4,
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
		/// Equal to: min=FO_LINEAR, mag=FO_LINEAR, mip=FO_LINEAR
		FO_TRILINEAR,
		// Similar to FO_LINEAR, but compensates for the angle of the texture plane
		FO_ANISOTROPIC
	};

	class PBRParams
	{
	public:
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
	public:
		PBRParams();

		virtual MaterialParamType GetType() = 0;

	public:
		// 控制自发光强度的因子
		Vector3d m_vec3EmissiveFactor;
		// 自发光纹理 RGB纹理
		string m_strEmissiveTexture;
		// 法线纹理，物体表面的凹凸细节 float格式RGB纹理
		string m_strNormalTexture;
		// 遮挡图，用于物体表面凹凸性对光照的影响，比如缝隙处就暗 float灰度纹理
		string m_strOcclusionTexture;
		// 物体的基本颜色
		Vector4d m_vec4BaseColor;
		// 物体基本颜色的纹理
		string	m_strBaseColorTexture;
		// AlphaMode
		AlphaMode m_AlphaMode;
		// 当alphaMode为Mask时，着色器根据这个值和baseColor的Alpha值进行比较决定是否丢弃
		float m_fAlphaCutoff;
	};

	// 金属粗糙度模型
	class PBRMetallicRough : public PBRParams
	{
	public:
		PBRMetallicRough();

		virtual MaterialParamType GetType() { return MAT_PBRMetallicRough; }
	public:
		// 金属度和粗糙性纹理,R通道存储金属度，G通道存储粗糙度
		string m_strMetallicRoughnessTexture;
		// 控制金属性强弱的因子
		float m_fMetallicFactor;
		// 控制粗糙性强弱的因子
		float m_fRoughnessFactor;
	};

	// 镜面光高光模型
	class PBRSpecularGlossy : public PBRParams
	{
	public:
		PBRSpecularGlossy();

		virtual MaterialParamType GetType() { return MAT_PBRSpecularGlossy; }
	public:
		// 散射颜色
		Vector4d m_vec4DiffuseFactor;
		// 镜面光颜色
		Vector3d m_vec3SpecularFactor;
		// 高光强度
		float m_fGlossinessFactor;
		// 散射颜色纹理
		string m_strDiffuseTexture;
		// 镜面光高光强度纹理，RGB通道存镜面光颜色，A通道存高光强度
		string m_strSpecularGlossinessTexture;
	};

	class TextureUnitState
	{
	public:
		enum TextureAddressingMode
		{

			TAM_WRAP,

			TAM_MIRROR,

			TAM_CLAMP,

			TAM_BORDER,
		};

		struct UVWAddressingMode
		{
			TextureAddressingMode u, v, w;
		};

		TextureUnitState();

	public:
		// optional name for the TUS
		string m_strName;

		// 纹理单元使用的纹理名称
		string m_strTextureName;

		// 纹理引用的文件
		string m_strTexureURL;

		// 设置纹理坐标分配模式
		UVWAddressingMode m_AddressMode;

		//纹理滤波的统一方式
		FilterOptions m_TextureFilteringOption;

		// 缩小时的滤波类型
		FilterOptions m_MinFilter;

		// 放大时的滤波类型
		FilterOptions m_MaxFilter;

		// 纹理矩阵
		mutable Matrix4d m_TexModMatrix;
	};

	class Pass
	{
	public:
		Pass();
		~Pass();
	public:
		// 通道名
		string m_strName;
		// 环境光
		ColorValue m_Ambient;
		// 散射光
		ColorValue m_Diffuse;
		// 反射光
		ColorValue m_Specular;
		// 发光，影响发射光点的大小
		float m_Shininess;
		// 渲染引擎用的的裁剪模式
		CullingMode m_CullMode;
		// 透明物体深度排序
		bool m_TransparentSorting;
		// PBR材质
		PBRParams* m_pPRBMaterial;
		//! \brief 顶点颜色跟踪
		unsigned int m_Tracking;
		//是否进行Alpha混合
		bool m_bBlendAlpha;
		//点尺寸
		float m_dPointSize;
		float m_dPointMinSize;
		float m_dPointMaxSize;
		//! \brief多边形偏移量常量部分
		float m_fConstantPolygonOffset;
		//! \brief多边形偏移量深度坡度因子部分
	//! \brief最终偏移量 = maxSlope * m_fSlopeScalePolygonOffset + m_fConstantPolygonOffset
		float m_fSlopeScalePolygonOffset;

		//! \brief  是否多重纹理
		bool m_bMutiTexture;
		/// Storage of texture unit states
		typedef std::vector<TextureUnitState*> TextureUnitStates;
		TextureUnitStates m_pTextureUnitStates;

		//! \brief 获得正面环绕模式
		//! \return 环绕模式
		int GetPFFMode();

		//! \brief 设置正面环绕模式
		//! \param mode 环绕模式
		void SetPFFMode(PolygonFrontFace mode);

		TextureUnitState* CreateTextureUnitState();
		//获取TextureUnitState的数量
		unsigned short GetTextureUnitStatesSize(void) const;

		//获取TextureUnitState
		TextureUnitState* GetTextureUnitState(unsigned short index);
	};

	class Technique
	{
	public:
		Technique();
		~Technique();
		Pass* CreatePass();
		typedef vector<Pass*> Passes;
		/// List of primary passes
		Passes mPasses;
		/** Retrieves the number of passes. */
		unsigned short getNumPasses(void) const;
		/** Retrieves the Pass with the given index. */
		Pass* getPass(unsigned short index);
		/** Retrieves the Pass matching name.*/
		Pass* getPass(const string& name);
		/** Removes all Passes from this Technique. */
		void removeAllPasses(void);
	};

	class Material
	{
	public:
		Material();
		~Material();
		Technique* CreateTechnique();
		string m_strName;
		typedef vector<Technique*> Techniques;
		Techniques mTechniques;
		/** Retrieves the number of techniques. */
		int getNumTechniques(void) const;
		/** Gets the indexed technique. */
		Technique* getTechnique(unsigned short index);
		/** Removes all the techniques in this Material. */
		void removeAllTechniques(void);
	};

}
#endif 
