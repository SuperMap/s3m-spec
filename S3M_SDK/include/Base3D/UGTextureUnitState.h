// UGTextureUnitState.h: interface for the UGTextureUnitState class.
//
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//                    _   _   ____   ____                    
//   Project         | | | | / ___) / ___)                   
//                   | | | || | __ | |                       
//                   | |_| || |_\ || \___                    
//                    \____/ \____| \ ___)     6.0           
//                                                           
//!  \file headerfile.h
//!  \brief Class 的接口定义。
//!  \details 文件详细信息。
//!  \author 三维组
//!  \attention 
//!   Copyright (c) 1996-2010 SuperMap Software Co., Ltd. <br> 
//!   All Rights Reserved.
//!  \version 6.0
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_UGTEXTUREUNITSTATE_H__8CD48F14_6B32_4535_AAFC_1AE07A7A4CF6__INCLUDED_)
#define AFX_UGTEXTUREUNITSTATE_H__8CD48F14_6B32_4535_AAFC_1AE07A7A4CF6__INCLUDED_

#include "Base3D/UGPrerequisites3D.h"
#include "Base3D/UGLayerBlendMode.h"
#include "Base3D/UGColorValue3D.h"
#include "Base3D/UGMatrix4d.h"
#include "Stream/UGStream.h"


namespace UGC 
{

class UGFrustum;

class BASE3D_API UGTextureUnitState  
{

public:
	 enum TextureEffectType
    {
        /// 以相机和定点生成所有的纹理坐标
        ET_ENVIRONMENT_MAP,
        /// 以视口生成纹理坐标
        ET_PROJECTIVE_TEXTURE,
        
        ET_UVSCROLL,
		
        ET_USCROLL,
		
        ET_VSCROLL,
       
        ET_ROTATE,
       
        ET_TRANSFORM,

    };
	  
    enum EnvMapType
    {
       
        ENV_PLANAR,
       
        ENV_CURVED,
        
        ENV_REFLECTION,
       
        ENV_NORMAL,
    };


    enum TextureAddressingMode
    {
       
        TAM_WRAP,
       
        TAM_MIRROR,
       
        TAM_CLAMP,
       
        TAM_BORDER,
    };

	/** Enum identifying the type of content this texture unit contains.
	*/
	enum ContentType
	{
		/// Normal texture identified by name
		CONTENT_NAMED = 0,
		/// A shadow texture, automatically bound by engine
		CONTENT_SHADOW = 1,
		/// A compositor texture, automatically linked to active viewport's chain
		CONTENT_COMPOSITOR = 2
	};

	/** Useful enumeration when dealing with procedural transforms.
	@note
	Note that these have no effect when using the programmable pipeline, since their
	effect is overridden by the vertex / fragment programs.
	*/
	enum TextureTransformType
	{
		TT_TRANSLATE_U,
		TT_TRANSLATE_V,
		TT_SCALE_U,
		TT_SCALE_V,
		TT_ROTATE
	};

	/** The type of unit to bind the texture settings to. */
	enum BindingType
	{
		/** Regular fragment processing unit - the default. */
		BT_FRAGMENT = 0,
		/** Vertex processing unit - indicates this unit will be used for 
		a vertex texture fetch.
		*/
		BT_VERTEX = 1
	};
	
    struct TextureEffect {
        TextureEffectType type;  //纹理效果类型
        UGint subtype;
        UGdouble arg1, arg2;
        WaveformType waveType;
        UGdouble base;
        UGdouble frequency;
        UGdouble phase;
        UGdouble amplitude;
        const UGFrustum* pFrustum;  //视景体
    };

	struct UVWAddressingMode
	{
		TextureAddressingMode u, v, w;
	};
	typedef UGDict<TextureEffectType, TextureEffect> EffectMap;
public:
	UGTextureUnitState();

	UGTextureUnitState(const UGTextureUnitState& oth);

	virtual ~UGTextureUnitState();

	UGbool IsBlank(void) const;
	//! \brief 获取纹理名称
	UGString GetTextureName(void) const;
		
	//! \brief 获取纹理在图层的索引
	UGuint GetTextureCoordSet(void) const	;
	
	//! \brief 为指定的类型进行纹理过滤
	FilterOptions GetTextureFiltering(UGFilterType ftpye) const;	
	
	UGuint GetTextureAnisotropy() const;
	
	//! \brief 获取纹理融合的多重纹理
	const UGLayerBlendModeEx& GetColourBlendMode(void) const;
	
	//! \brief 获取alpha融合模式.
	const UGLayerBlendModeEx& GetAlphaBlendMode(void) const;
	
	//! \brief获取纹理的寻址方式
	const UVWAddressingMode& GetTextureAddressingMode(void) const;
	
	//! \brief 获取纹理边界颜色值
	const UGColorValue3D& GetTextureBorderColour(void) const;
	
	//! \brief 获取当前纹理转换矩阵.
	const UGMatrix4d& GetTextureTransform(void) const;

	//! \从XML字符串读取参数设置
	//! \param strXML XML字符串  [in]
	//! \return 返回是否成功
	UGbool FromXML(UGString& strXML);

	//! \保存参数设置到XML字符串
	//! \param strXML XML字符串  [in]
	//! \return 返回是否成功
	UGbool ToXML(UGString& strXML);
	
	//! \brief 从流文件中加载
	//! \param fStream 文件流[in]。
	UGbool Load(UGStream& fStream);

	//! \brief 存入流文件中
	//! \param fStream 文件流[in]。
	void Save(UGStream& fStream);

	//! \brief 获取数据存储时的大小
	//! \return 返回存储大小
	//! \remark 暂未考虑编码情况，与Save()一起维护
	//! \attention 字符串的长度要+4
	UGint GetDataSize() const;

protected:
	//! \brief 计算纹理矩阵的内部方法
	void RecalcTextureMatrix(void) const;
	
public:
	
	//! \brief 纹理效果数组
	EffectMap m_Effects;

	//////////////////////////////////////////////////////////////////////////
	//为了传递给Ogre参数而增加的属性

	// optional name for the TUS
	UGString m_strName;

	// optional alias for texture frames
	UGString m_strTextureNameAlias;

	// 纹理单元使用的纹理名称
	UGString m_strTextureName;

	// 纹理引用的文件
	UGString m_strTexureURL;

	// 立方体纹理各个面的纹理的名称
	UGString *m_pTextureNames;

	TextureType m_TextureType;

	// 纹理载入是希望的像素格式
	UG3DPixelFormat m_DesireFormat;

	// 纹理单元包含多少层mipmaps
	UGint m_nNumMipmaps;

	// 纹理在单通道载入时，是否作为Alpha通道载入
	UGbool m_bIsAlpha;

	// 纹理建立时采样是否使用Gamma校正
	UGbool m_bHardwareGammaEnabled;

	// 使用的立方体纹理名称
	UGString m_strCubicTextureName;

	/**forUVW Set to true if you want a single 3D texture addressable with 3D texture coordinates rather than
	6 separate textures. Useful for cubic environment mapping.*/
	UGbool m_bForUVW;

	// The index of the texture coordinate set to use.
	UGuint m_unTextureCoordSetIndex;

	// 设置纹理坐标分配模式
	UVWAddressingMode m_AddressMode;

	//! \brief 纹理边界颜色
	UGColorValue3D m_TextureBorderColour;

	//纹理滤波的统一方式
	FilterOptions m_TextureFilteringOption;

	//! \brief 缩小时的滤波类型
	FilterOptions m_MinFilter;

	//! \brief 放大时的滤波类型
	FilterOptions m_MaxFilter;

	//! \brief Mipmap时滤波类型
	FilterOptions m_MipFilter;

	// 纹理的各向等级，默认为1，不开启各向异性。开启时应当在2和显卡支持的最大各向异性之间 
	UGuint m_MaxAniso;

	// 计算mipmap时的偏移值
	UGdouble m_MipmapBias;

	/** 该纹理层如何与在它之下的纹理进行融合，（如果这层纹理是0层，则是与几何对象的散射颜色融合的方式）*/
	LayerBlendOperation m_ColorOperation;

	//! \brief 颜色融合模式
	UGLayerBlendModeEx m_ColourBlendMode;

	//! \brief Alpha融合模式
	UGLayerBlendModeEx m_AlphaBlendMode;

	SceneBlendFactor m_ColourBlendFallbackSrc;
	SceneBlendFactor m_ColourBlendFallbackDest;


	/* 采用的环境映射类型。环境映射使得物体看起来具有反射效果*/
	EnvMapType m_EnvironmentMap;
	// 是否启用环境映射
	UGbool m_bEnvironmentMapEnabled;

	//! \brief 纹理V,U的缩放
	UGdouble m_dUScale, m_dVScale;
	//! \brief 是否重新计算纹理矩阵
	mutable UGbool m_bRecalcTexMatrix;
	//! \brief 
	UGdouble m_dUMod, m_dVMod;
	//! \brief 纹理旋转角度+
	UGdouble m_dRotateAngle;
	//Animation
	UGdouble m_dUSpeed, m_dVSpeed;
	UGdouble m_dRotateSpeed;
	TextureTransformType m_TransformAnimationType;
	WaveformType m_TransformAnimationWaveFormType;
	UGdouble m_dTransformAnimationBase;
	UGdouble m_dTransformAnimationfrequency; 
	UGdouble m_dTransformAnimationPhase; 
	UGdouble m_dTransformAnimationAmplitude;

	//! \brief 纹理矩阵
	mutable UGMatrix4d m_TexModMatrix;

	BindingType m_BindingType;

	// 纹理单元引用内容的类型
	ContentType m_ContentType;

	///The data that references the compositor
	UGString m_strCompositorRefName;
	UGString m_strCompositorRefTexName;
	/// The index of the referenced texture if referencing an MRT in a compositor
	UGuint m_nCompositorRefMrtIndex;

	//////////////////////////////////////////////////////////////////////////
protected:
	UGbool m_bBlank;
	
	//! \brief 当前桢
	UGuint m_unCurrentFrame;
	
	//! \brief 是否使用默认滤波	
	UGbool m_bDefaultFiltering;	
	
	//! \brief 桢列表
	UGList<UGString> m_Frames;
	
	//! \brief 是否使用各向异向性
	UGbool m_bDefaultAniso;
};
}
#endif // !defined(AFX_UGTEXTUREUNITSTATE_H__8CD48F14_6B32_4535_AAFC_1AE07A7A4CF6__INCLUDED_)

