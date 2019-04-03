//////////////////////////////////////////////////////////////////////////
//                    _   _   ____   ____                    
//   Project         | | | | / ___) / ___)                   
//                   | | | || | __ | |                       
//                   | |_| || |_\ || \___                    
//                    \____/ \____| \ ___)     6.1           
//                                                           
//!  \file UGMaterial.h
//!  \brief 三维材质渲染通道类
//!  \details 完成材质渲染通道的封装
//!  \author sunyl
//!  \attention 
//!   Copyright (c) 1996-2012 SuperMap Software Co., Ltd. <br> 
//!   All Rights Reserved.
//!  \version 6.1
//////////////////////////////////////////////////////////////////////////

#ifndef __PASS_H__
#define __PASS_H__

#if _MSC_VER > 1000
#pragma once
#endif

#include "Base3D/UGPrerequisites3D.h"
#include "Base3D/UGColorValue3D.h"
#include "UGTextureUnitState.h"
#include "UGGpuProgram.h"
#include "Base3D/UGVector3d.h"


namespace UGC
{
	enum UGMaterialParamType
	{
		MAT_Unknown		  = 0,
		//! \brief 金属粗糙度模型
		MAT_PBRMetallicRough =	1,
		//! \brief 镜面光高光模型
		MAT_PBRSpecularGlossy = 2,
	};

	class BASE3D_API UGPBRParameter
	{
	public:
		enum UGAlphaMode
		{
			PBRAM_UnKnown  = 0,
			//! \brief 片元中alpha值输出为1.0,
			PBRAM_OPAQUE	= 1,
			//! \brief 片元进行alpha过滤
			PBRAM_MASK	    = 2, 
			//! \brief 片元的alpha值输出baseColor的alpha值
			PBRAM_BLEND		= 3, 
		};

	public:
		UGPBRParameter();
		UGPBRParameter(const UGPBRParameter& other);
		UGPBRParameter & operator=(const UGPBRParameter& other);
		~UGPBRParameter();

		//! \brief 材质类型
		virtual UGMaterialParamType GetType() = 0;

		static UGPBRParameter* CreateFrom(UGPBRParameter* pMaterial);

	public:
		//! \brief 控制自发光强度的因子
		UGVector3d m_vec3EmissiveFactor;
		//! \brief 自发光纹理 RGB纹理
		UGString m_strEmissiveTexture;
		//! \brief 法线纹理，物体表面的凹凸细节 float格式RGB纹理
		UGString m_strNormalTexture;
		//! \brief 遮挡图，用于物体表面凹凸性对光照的影响，比如缝隙处就暗 float灰度纹理
		UGString m_strOcclusionTexture;
		//! \brief 物体的基本颜色
		UGVector4d m_vec4BaseColor;
		//! \brief 物体基本颜色的纹理
		UGString	m_strBaseColorTexture;
		//! \brief UGAlphaMode
		UGAlphaMode	m_AlphaMode;
		//! \brief 当alphaMode为Mask时，着色器根据这个值和baseColor的Alpha值进行比较决定是否丢弃
		UGfloat	 m_fAlphaCutoff;
	};	

	//! \brief 金属粗糙度模型的参数
	class BASE3D_API UGPBRMetallicRough : public UGPBRParameter
	{	
	public:
		UGPBRMetallicRough();
		UGPBRMetallicRough(const UGPBRMetallicRough& other);
		UGPBRMetallicRough & operator=(const UGPBRMetallicRough& other);
		~UGPBRMetallicRough();

		virtual UGMaterialParamType GetType() { return MAT_PBRMetallicRough;}
	public:
		//! \brief 金属度和粗糙性纹理,R通道存储金属度，G通道存储粗糙度
		UGString m_strMetallicRoughnessTexture;
		//! \brief 控制金属性强弱的因子
		UGfloat m_fMetallicFactor;
		//! \brief 控制粗糙性强弱的因子
		UGfloat m_fRoughnessFactor;
	};	

	//! \brief 镜面光高光模型
	class BASE3D_API UGPBRSpecularGlossy : public UGPBRParameter
	{
	public:
		UGPBRSpecularGlossy();
		UGPBRSpecularGlossy(const UGPBRSpecularGlossy& other);
		UGPBRSpecularGlossy & operator=(const UGPBRSpecularGlossy& other);
		~UGPBRSpecularGlossy();

		virtual UGMaterialParamType GetType() { return MAT_PBRSpecularGlossy;}
	public:
		//! \brief 散射颜色
		UGVector4d m_vec4DiffuseFactor;
		//! \brief 镜面光颜色
		UGVector3d m_vec3SpecularFactor;
		//! \brief 高光强度
		UGfloat m_fGlossinessFactor;
		//! \brief 散射颜色纹理
		UGString m_strDiffuseTexture;
		//! \brief 镜面光高光强度纹理，RGB通道存镜面光颜色，A通道存高光强度
		UGString m_strSpecularGlossinessTexture;
	};	

	//! \brief 封装的用于保存RO的渲染状态的结构体
	class BASE3D_API UGPass  
	{	
		friend class UGRenderOperation3DOGRE;
		friend class UGRenderOperationOverlayOGRE;
		friend class UGGraphics3DOGRE;
		friend class UGS3MBTools;
	public:
		UGPass();
		UGPass(const UGPass& other);
		UGPass & operator=(const UGPass& other);
		~UGPass();

		//! \brief  设置雾的模式
		//! \param mode 雾模式，colour 雾颜色，density 雾浓度，start 雾开始的Z值，end 雾结束的Z值、
		void SetFog(FogMode mode, const UGColorValue3D& colour, UGfloat density, UGfloat start, UGfloat end);
		//! \brief 控制当前 pass 与已存在场景内容的融合
		//! \param sourceFactor 混合源因子，destFactor 混合目标因子
		void SetSceneBlending(SceneBlendFactor sourceFactor, SceneBlendFactor destFactor);

		/** Sets the kind of blending this pass has with the existing contents of the scene, separately for color and alpha channels
		@remarks
		Whereas the texture blending operations seen in the TextureUnitState class are concerned with
		blending between texture layers, this blending is about combining the output of the Pass
		as a whole with the existing contents of the rendering target. This blending therefore allows
		object transparency and other special effects. If all passes in a technique have a scene
		blend, then the whole technique is considered to be transparent.
		*/
		void SetSeparateSceneBlending( const SceneBlendType sbt, const SceneBlendType sbta );

		void SetSeparateSceneBlending( const SceneBlendFactor sourceFactor, const SceneBlendFactor destFactor, const SceneBlendFactor sourceFactorAlpha, const SceneBlendFactor destFactorAlpha );

		/** Sets the specific operation used to blend source and destination pixels together.
		By default this operation is +, which creates this equation
		final = (texture * sourceFactor) + (pixel * destFactor)
		By setting this to something other than SBO_ADD you can change the operation to achieve
		a different effect.*/
		void SetSceneBlendingOperation(SceneBlendOperation op);

		/** Sets the specific operation used to blend source and destination pixels together.
		By default this operation is +, which creates this equation
		final = (texture * sourceFactor) + (pixel * destFactor)*/
		void SetSeparateSceneBlendingOperation(SceneBlendOperation op, SceneBlendOperation alphaOp);

		//! \brief 设置正面环绕模式
		//! \param mode 环绕模式
		void SetPFFMode(PolygonFrontFace mode);

		//! \brief 获得正面环绕模式
		//! \return 环绕模式
		UGint GetPFFMode();

		//! \brief 设置Pass的哈希排序优先级	
		void SetHashPriority(UGint nHashPriority);

		//! \brief 获取Pass的哈希排序优先级	
		UGint GetHashPriority();

		//! \brief 设置这个Pass是否遍历每个能够影响物体渲染的灯光，默认是false
		/*
		@param enabled Whether this feature is enabled
		@param onlyForOneLightType If true, the pass will only be run for a single type
		of light, other light types will be ignored.
		@param lightType The single light type which will be considered for this pass
		*/
		void SetIteratePerLight(UGbool enabled,
			UGbool onlyForOneLightType = TRUE, LightTypes lightType = LT_POINT);

		/** 设置点对象的大小如何随着距离变化而变化
		@remarks
		当点的渲染以point sprite方式进行时，点的大小如下计算：
		attenuation = 1 / (constant + linear * dist + quadratic * d^2)
		@param enabled 点放缩是否打开
		@param constant, linear, quadratic 用来控制放缩的参数
		*/
		void SetPointAttenuation(UGbool enabled,
			UGdouble constant = 0.0, UGdouble linear = 1.0, UGdouble quadratic = 0.0);

		//! \brief 设置当前Pass的模板操作
		//! \param CompareFunction 模板测试比较方式
		//! \param refVal 模板测试掩码
		//! \param mask 模板测试参考值
		//! \param passOperation 测试通过时的模板操作方式
		//! \param stencilFailOperation 模板测试失败时的模板操作方式
		//! \param depthFailOperation 深度测试失败时的模板操作方式
		//! \param bTwoSidedOperation 如果设置为TRUE，当进行双面渲染时，设置的对模板的操作会作用于正面，相反作用于反面。比如设置模板值加1，那么反面渲染时模板值就减1。OpenGL2.0的特性
		void SetStencilBufferParams(CompareFunction func, UGuint refVal, UGuint mask, StencilOperation passOperation, 
			StencilOperation stencilFailOperation, StencilOperation depthFailOperation, UGbool bTwoSidedOperation = FALSE);


		UGTextureUnitState* CreateTextureUnitState();

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
		UGbool Load(UGStream& fStream, UGdouble dVersion);

		//! \brief 存入流文件中
		//! \param fStream 文件流[in]。
		void Save(UGStream& fStream, UGdouble dVersion);

		//! \brief 获取数据存储时的大小
		//! \return 返回存储大小
		//! \remark 暂未考虑编码情况，与Save()一起维护
		//! \attention 字符串的长度要+4
		UGint GetDataSize() const;


		//! \brief  名字
		UGString m_strName;

		//绘制模式，枚举值
		PolygonMode m_enPolygonMode;

		// 设置光照是否开启，默认开启 true
		UGbool m_bLightEnabled;

		// Max simultaneous lights
		UGushort m_nMaxSimultaneousLights;

		/** Light index that this pass will start at in the light list.
		Normally the lights passed to a pass will start from the beginning
		of the light list for this object. This option allows you to make this
		pass start from a higher light index, for example if one of your earlier
		passes could deal with lights 0-3, and this pass dealt with lights 4+. 
		This option also has an interaction with pass iteration, in that
		if you choose to iterate this pass per light too, the iteration will
		only begin from light 4.*/
		UGushort m_nStartLight;

		/** Sets whether this pass's chosen detail level can be
		overridden (downgraded) by the camera setting. 
		override true means that a lower camera detail will override this
		pass's detail level, false means it won't (default true).*/
		UGbool m_bPolygonModeOverrideable;

		//点尺寸
		UGfloat m_dPointSize;
		UGfloat m_dPointMinSize;
		UGfloat m_dPointMaxSize;

		/** Sets whether or not rendering points using OT_POINT_LIST will
		render point sprites (textured quads) or plain points (dots).
		@param enabled True enables point sprites, false returns to normal
		point rendering.*/
		UGbool m_bPointSpritesEnabled;

		UGbool m_bPointAttenuationEnabled;
		// constant, linear, quadratic coeffs
		UGdouble m_PointAttenuationCoeffs[3];
		//点的反走样模式
		SmoothHintMode m_enPointSmoothHintMode;
		//线的反走样模式
		SmoothHintMode m_enLineSmoothHintMode;
		//! \brief 设置线绘制参数
		//! \param width:线宽，应大于0。
		//! \param lineStippleEnabled:是否开启点画线模式。如果开启，后面两个参数才起作用。
		//! \param factor：点画线模式重复次数。
		//! \param pattern：点画线样式。
		void SetLineParameters(UGdouble dWidth, UGbool blineStippleEnabled,
			UGint nFactor = 1, UGushort nPattern = 0xffff);

		//! \brief 贴图纹理数据 最多支持8组
		UGString m_strTextures[SMSCN_MAX_TEXTURE_COORD_SETS];
		UGTextureUnitState::ContentType m_ContentType[SMSCN_MAX_TEXTURE_COORD_SETS];
		//! \brief 纹理变换矩阵
		UGMatrix4d m_matTexTransform[SMSCN_MAX_TEXTURE_COORD_SETS];
		//! \brief 纹理的贴图模式
		UGTextureUnitState::TextureAddressingMode m_TexAddressingMode[SMSCN_MAX_TEXTURE_COORD_SETS];
		//! \brief 标记纹理通道
		UGuint m_nTextureIndex[SMSCN_MAX_TEXTURE_COORD_SETS];
		//! \brief 标记纹理Z通道
		UGint m_nTextureZType[SMSCN_MAX_TEXTURE_COORD_SETS];
		//! \brief  是否多重纹理
		UGbool m_bMutiTexture;

		//! \brief 环境光
		UGColorValue3D m_Ambient;
		//! \brief 散射光
		UGColorValue3D m_Diffuse;
		//! \brief 反射光
		UGColorValue3D m_Specular;
		//! \brief 自发光
		UGColorValue3D m_SelfIllumination;
		//! \brief 发光，影响发射光点的大小
		UGfloat m_Shininess;
		//! \brief 顶点颜色跟踪
		UGuint m_Tracking;

		//材质颜色，原来叫SingleColor
		UGColorValue3D m_MaterialColor;
		// m_bReceiveShadow 已废弃,added by likai at 2013.3.14
		//是否使用阴影
		UGbool m_bReceiveShadow;
		//颜色是否能够写入
		UGbool m_bColorWrite;
		//Alpha测试参考值
		UGfloat m_fAlphaReject;
		//Alpha测试方法
		CompareFunction m_AlphaRejectFunc;
		/** Sets whether to use alpha to coverage (A2C) when blending alpha rejected values. 
		Alpha to coverage performs multisampling on the edges of alpha-rejected
		textures to produce a smoother result. It is only supported when multisampling
		is already enabled on the render target, and when the hardware supports
		alpha to coverage (see RenderSystemCapabilities). */
		UGbool m_bAlphaToCoverageEnabled;
		//! \brief 透明物体深度排序
		UGbool m_TransparentSorting;
		/** Sets whether or not transparent sorting is forced.
		@param enabled
		If true depth sorting of this material will be depend only on the value of
		getTransparentSortingEnabled().
		@remarks
		By default even if transparent sorting is enabled, depth sorting will only be
		performed when the material is transparent and depth write/check are disabled.
		This function disables these extra conditions.*/
		UGbool m_bTransparentSortingForced;
		//是否进行深度测试
		UGbool m_bDepthCheck;
		//渲染时是否进行深度写入
		UGbool m_bDepthWrite;
		//深度测试方法
		CompareFunction m_DepthBufferFunc;
		//是否进行模板测试
		UGbool m_bStencilCheck;
		//! \brief多边形偏移量常量部分
		UGfloat m_fConstantPolygonOffset;
		//! \brief多边形偏移量深度坡度因子部分
		//! \brief最终偏移量 = maxSlope * m_fSlopeScalePolygonOffset + m_fConstantPolygonOffset
		UGfloat m_fSlopeScalePolygonOffset;
		/** Sets a factor which derives an additional depth bias from the number 
		of times a pass is iterated. The Final depth bias will be the constant depth bias as set through
		setDepthBias, plus this value times the iteration number. */
		UGfloat m_fDepthBiasPerIteration;
		//是否进行Alpha混合
		UGbool m_bBlendAlpha;
		//使用的顶点着色器的名字
		UGString m_strVertexProgram;
		//使用的片元着色器的名字
		UGString m_strFragmentProgram;
		//使用的几何着色器的名字
		UGString m_strGeometryProgram;
		//使用的阴影投射顶点着色器的名字
		UGString m_strShadowCasterVertexProgram;
		//使用的阴影投射片元着色器的名字
		UGString m_strShadowCasterFragmentProgram;
		//使用的阴影接收顶点着色器的名字
		UGString m_strShadowReceiverVertexProgram;
		//使用的阴影接收片元着色器的名字
		UGString m_strShadowReceiverFragmentProgram;
		// 顶点着色器使用的共享参数列表
		UGString m_strVertexGpuSharedParams;
		// 片元着色器使用的共享参数列表
		UGString m_strFragmentGpuSharedParams;
		//使用的PassHash方法
		HashFunction m_enPassHashFunction;
		//透明度0.0是完全透明，1.0是完全不透明
		//在ogre中没有发现这个成员
		//UGfloat m_fOpacity;
		/* whether or not this pass will be clipped by a scissor rectangle
		encompassing the lights that are being used in it.*/
		UGbool m_bLightScissoring;
		/* whether or not this pass will be clipped by user clips planes
		bounding the area covered by the light.*/
		UGbool m_bLightClipPlanes;
		/* Manually set which illumination stage this pass is a member of.*/
		IlluminationStage m_enIlluminationStage;

		/** set the number of iterations that this pass
		should perform when doing fast multi pass operation.
		@remarks
		Only applicable for programmable passes.
		@param count number of iterations to perform fast multi pass operations.
		A value greater than 1 will cause the pass to be executed count number of
		times without changing the render state.  This is very usefull for passes
		that use programmable shaders that have to iterate more than once but don't
		need a render state change.  Using multi pass can dramatically speed up rendering
		for materials that do things like fur, blur.
		A value of 1 turns off multi pass operation and the pass does
		the normal pass operation.*/
		UGuint m_nPassIterationCount;

		/// Iterate per how many lights?
		UGushort m_nLightsPerIteration;

		/** Sets the manual culling mode, performed by CPU rather than hardware.
		In some situations you want to use manual culling of triangles rather than sending the
		triangles to the hardware and letting it cull them. This setting only takes effect on SceneManager's
		that use it (since it is best used on large groups of planar world geometry rather than on movable
		geometry since this would be expensive), but if used can cull geometry before it is sent to the
		hardware.The default for this setting is MANUAL_CULL_BACK.*/
		ManualCullingMode m_enManualCullMode;

		/** If set to true, this forces normals to be normalised dynamically 
		by the hardware for this pass.*/
		UGbool m_bNormaliseNormals;

		/** Sets the type of light shading required
		The default shading method is Gouraud shading.*/
		ShadeOptions m_enShadeOptions;

		//动画纹理帧数
		UGint m_nNumFrames[SMSCN_MAX_TEXTURE_COORD_SETS];
		//动画纹理帧率
		std::map<UGuint,UGuint> m_mapTimeFrames[SMSCN_MAX_TEXTURE_COORD_SETS];

		//-----------------------------------------------------------
		// Texture Filtering
		FilterOptions m_MinFilter[SMSCN_MAX_TEXTURE_COORD_SETS];
		FilterOptions m_MagFilter[SMSCN_MAX_TEXTURE_COORD_SETS];
		FilterOptions m_MipFilter[SMSCN_MAX_TEXTURE_COORD_SETS];

		// 纹理计数器，GIF的纹理计为0
		UGint m_nMutiLevel;

		// 着色器 使用的宏定义
		std::set<UGString> m_setMacro;
	protected:
		// 渲染引擎用的的裁剪模式
		CullingMode m_CullMode;
		//-------------------------------------------------------------------------
		// 雾模式、颜色、开始的Z值、结束的Z值、浓度
		FogMode m_enFogMode;
		UGColorValue3D m_FogColor;
		UGfloat m_dFogStart;
		UGfloat m_dFogEnd;
		UGfloat m_dFogDensity;

		//-------------------------------------------------------------------------
		//混合源因子、混合目标因子
		SceneBlendFactor m_srcBlendFactor;
		SceneBlendFactor m_dstBlendFactor;
		SceneBlendFactor m_srcBlendFactorAlpha;
		SceneBlendFactor m_dstBlendFactorAlpha;

		// Used to determine if separate alpha blending should be used for color and alpha channels
		UGbool m_SeparateBlend;

		// Blending operations
		SceneBlendOperation m_BlendOperation;
		SceneBlendOperation m_AlphaBlendOperation;
		UGbool m_SeparateBlendOperation;

		//-------------------------------------------------------------------------
		//绘制线的参数
		UGfloat m_dLineWidth;
		UGbool m_bLineStippleEnabled;
		UGint m_nLineFactor;
		UGushort m_nLinePattern;

		//排序优先级
		UGint m_nHashPriority;

		//模板测试方法
		CompareFunction m_StencilFunc;
		//模板测试参考值
		UGuint m_StencilRefVal;
		//模板测试通过的操作
		StencilOperation m_StencilPassOp;
		//模板测试失败的操作
		StencilOperation m_StencilFailOp;
		//深度测试失败的操作
		StencilOperation m_DepthFailOp;
		//模板测试掩码
		UGuint m_nStencilMask;
		//模板操作是否作用于双面渲染
		UGbool m_bTwoSidedStencilOperation;

		//-------------------------------------------------------------------------
		/// Run this pass once per light?
		UGbool m_bIteratePerLight;
		// Should it only be run for a certain light type?
		UGbool m_bRunOnlyForOneLightType;
		LightTypes m_enOnlyLightType;

		/// Storage of texture unit states
		typedef std::vector<UGTextureUnitState*> TextureUnitStates;
		TextureUnitStates m_pTextureUnitStates;

		//这个Pass所使用的顶点着色器的参数
		UGGpuProgramParametersSharedPtr m_pVertexProgramParams;
		//这个Pass所使用的片元着色器的参数
		UGGpuProgramParametersSharedPtr m_pFragmentProgramParams;
		//这个Pass所使用的几何着色器的参数
		UGGpuProgramParametersSharedPtr m_pGeometryProgramParams;
		//这个Pass所使用的阴影投射顶点着色器的参数
		UGGpuProgramParametersSharedPtr m_pShadowCasterVertexParam;
		//这个Pass所使用的阴影接收顶点着色器的参数
		UGGpuProgramParametersSharedPtr m_pShadowReceiverVertexParam;
		//这个Pass所使用的阴影接收片元着色器的参数
		UGGpuProgramParametersSharedPtr m_pShadowReceiverFragmentParam;

	public:
		//获取TextureUnitState
		UGTextureUnitState* GetTextureUnitState(UGushort index);
		//获取TextureUnitState
		UGTextureUnitState* GetTextureUnitState(const UGString& name);
		//获取TextureUnitState的数量
		UGushort GetTextureUnitStatesSize(void) const;
		//移除所有的TextureUnitState
		void removeAllTextureUnitState(void);
		// 设置纹理采样
		void SetTextureFiltering(UGushort nTexUnit, FilterOptions minFilter, FilterOptions magFilter, FilterOptions mipFilter);

		//这个Pass所使用的顶点着色器的参数
		UGGpuProgramParametersSharedPtr GetVertexProgramParamsPtr();
		//这个Pass所使用的片元着色器的参数
		UGGpuProgramParametersSharedPtr GetFragmentProgramParamsPtr();
		//这个Pass所使用的几何着色器的参数
		UGGpuProgramParametersSharedPtr GetGeometryProgramParamsPtr();
		//这个Pass所使用的阴影投射顶点着色器的参数
		UGGpuProgramParametersSharedPtr GetShadowCasterVertexParamPtr();
		//这个Pass所使用的阴影接收顶点着色器的参数
		UGGpuProgramParametersSharedPtr GetShadowReceiverVertexParamPtr();
		//这个Pass所使用的阴影接收片元着色器的参数
		UGGpuProgramParametersSharedPtr GetShadowReceiverFragmentParamPtr();

		//! \brief PBR材质
		UGPBRParameter* m_pPRBMaterial;

	private:
		void _getBlendFlags(SceneBlendType type, SceneBlendFactor& source, SceneBlendFactor& dest);		
	};


}

#endif 


