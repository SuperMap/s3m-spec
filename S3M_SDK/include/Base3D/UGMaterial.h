//////////////////////////////////////////////////////////////////////////
//                    _   _   ____   ____                    
//   Project         | | | | / ___) / ___)                   
//                   | | | || | __ | |                       
//                   | |_| || |_\ || \___                    
//                    \____/ \____| \ ___)     6.0           
//                                                           
//!  \file UGMaterial.h
//!  \brief 材质类。
//!  \details 完成度材质类的封装。
//!  \author malq wangzhp。
//!  \attention 
//!   Copyright (c) 1996-2010 SuperMap Software Co., Ltd. <br> 
//!   All Rights Reserved.
//!  \version 6.0
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_UGMATERIAL_H__E85252C6_682E_4604_B276_C6EDA7B0F030__INCLUDED_)
#define AFX_UGMATERIAL_H__E85252C6_682E_4604_B276_C6EDA7B0F030__INCLUDED_

#include "Base3D/UGPrerequisites3D.h"
#include "Base3D/UGTextureData.h"
#include "Stream/UGFileStream.h"
namespace UGC
{
//! \brief　增加flag用于标识TextureData支持GIF与DDS压缩
#define UG_MATERIAL_MODIFIED_FLAG 0X20120613

//! \brief 三维材质类。
class BASE3D_API UGMaterial 
{
public:

	enum MaterialEffectType
	{
		NONE = 0,
		WATER = 1,
	};

	//! \brief 构造函数。
	UGMaterial();
	//! \brief 拷贝构造函数。
	UGMaterial(const UGMaterial& other);
	//! \brief 赋值函数。
	UGMaterial& operator=(const UGMaterial& other);
	//! \brief 构造函数。
	~UGMaterial();
	//!\brief 枚举构造函数
	UGMaterial(MaterialEffectType type);

public:
	//! \brief 获取特效枚举。
	//! \param  [in]。
	//! \return 返回特效枚举。
	//! \remarks 。
	//! \attention 。
	MaterialEffectType GetEffectType();

	//! \brief 设置材质名称。
	//! \param value 材质名称[in]。
	//! \return 。
	//! \remarks 。
	//! \attention 。
	void SetName(UGString value);

	//! \brief 获取材质名称。
	//! \param  [in]。
	//! \return 返回材质名称。
	//! \remarks 。
	//! \attention 。
	UGString GetName();
	
	//! \brief 设置纹理的路径。
	//! \param value 纹理路径字段[in]。
	//! \return 。
	//! \remarks 。
	//! \attention 。
	void SetTexPath(UGString value,UGuint nIndex = 0);
	
	//! \brief 。
	//! \param  [in]。
	//! \return 。
	//! \remarks 。
	//! \attention 。
	UGString GetTexPath(UGuint nIndex = 0);
	
	//! \brief 设置漫反射参数，UGfloat型。
	//! \param r 红色分量[in]。
	//! \param g 绿色分量[in]。
	//! \param b 蓝色分量[in]。
	//! \param alpha alpha分量[in]。
	//! \return 。
	//! \remarks 。
	//! \attention 。
	void SetDiffuse(UGfloat r, UGfloat g, UGfloat b, UGfloat alpha);
	//! \brief 设置漫反射参数，UGfloat型。
	//! \param ambient[4] 分别是红色绿色蓝色和alpha分量[in]。
	//! \return 。
	//! \remarks 。
	//! \attention 。
	void SetDiffuse(UGfloat diffuse[4]);//---这些地方还是要统一
	
	//! \brief 设置环境光参数，UGfloat型。
	//! \param r 红色分量[in]。
	//! \param g 绿色分量[in]。
	//! \param b 蓝色分量[in]。
	//! \param alpha alpha分量[in]。
	//! \return 。
	//! \remarks 。
	//! \attention 。
	void SetAmbient(UGfloat r, UGfloat g, UGfloat b, UGfloat alpha);
	//! \brief 设置环境光参数，UGfloat型。
	//! \param ambient[4] 分别是红色绿色蓝色和alpha分量[in]。
	//! \return 。
	//! \remarks 。
	//! \attention 。
	void SetAmbient(UGfloat ambient[4]);


	//! \brief 设置镜面反射参数，UGfloat型。
	//! \param r 红色分量[in]。
	//! \param g 绿色分量[in]。
	//! \param b 蓝色分量[in]。
	//! \param alpha alpha分量[in]。
	//! \return 。
	//! \remarks 。
	//! \attention 。
	void SetSpecular(UGfloat r, UGfloat g, UGfloat b, UGfloat alpha);
	//! \brief 设置镜面反射参数，UGfloat型。
	//! \param ambient[4] 分别是红色绿色蓝色和alpha分量[in]。
	//! \return 。
	//! \remarks 。
	//! \attention 。
	void SetSpecular(UGfloat specular[4]);//---这些地方还是要统一


	//! \brief 设置自发光参数，UGfloat型。
	//! \param r 红色分量[in]。
	//! \param g 绿色分量[in]。
	//! \param b 蓝色分量[in]。
	//! \param alpha alpha分量[in]。
	//! \return 。
	//! \remarks 。
	//! \attention 。
	void SetEmmissive(UGfloat r, UGfloat g, UGfloat b, UGfloat alpha);
	//! \brief 设置自发光参数，UGfloat型。
	//! \param ambient[4] 分别是红色绿色蓝色和alpha分量[in]。
	//! \return 。
	//! \remarks 。
	//! \attention 。
	void SetEmmissive(UGfloat emmissive[4]);//---这些地方还是要统一

	//! \brief 设置光照强度，UGfloat型。
	//! \param value 光照强度[in]。
	//! \return 。
	//! \remarks 。
	//! \attention 。
	void SetShininess(UGfloat value);

	//! \brief 设置纹理数据，UGTextureData型。
	//! \param value 纹理数据[in]。
	//! \return 。
	//! \remarks 。
	//! \attention 外部new后传入，内部负责释放。
	void SetTextureData(UGTextureData* pTextureData,UGuint nLevel = 0);
	
	//! \brief 获取漫反射参数。
	//! \param  [in]。
	//! \return UGfloat型指针。
	//! \remarks 。
	//! \attention 。
	UGfloat *GetDiffuse();

	//! \brief 获取环境光参数。
	//! \param  [in]。
	//! \return UGfloat型指针。
	//! \remarks 。
	//! \attention 。
	UGfloat *GetAmbient();

	//! \brief 获取镜面反射参数。
	//! \param  [in]。
	//! \return UGfloat型指针。
	//! \remarks 。
	//! \attention 。
	UGfloat *GetSpecular();
	
	//! \brief 获取自发光参数。
	//! \param  [in]。
	//! \return UGfloat型指针。
	//! \remarks 。
	//! \attention 。
	UGfloat *GetEmmissive();

	//! \brief 光照强度。
	//! \param  [in]。
	//! \return UGfloat型。
	//! \remarks 。
	//! \attention 。
	UGfloat GetShininess();

	//! \brief 暂时没用用到。
	UGushort GetLodIndexSquaredDepth(UGfloat squaredDistance) const;

	//! \brief 纹理数据。
	//! \param  [in]。
	//! \return UGTextureData类型
	//! \remarks 。
	//! \attention 贴图信息先看m_pTextureData是不是为空，如果为空才去用m_strTexPath加载路径。
	UGTextureData* GetTextureData(UGuint nLevel = 0 );


	//! \brief 从流文件中加载
	//! \param eCodecType 纹理数据压缩格式[in]。
	UGbool Load(UGStream& fStream ,UGuint eCodecType = 0);

	//! \brief 存入流文件中
	//! \param eCodecType 纹理数据压缩格式[in]。
	void Save(UGStream& fStream,UGuint eCodecType = 0);

	//! \brief 获取数据存储时的大小
	//! \return 返回存储大小
	//! \remark 暂未考虑编码情况，与Save()一起维护
	//! \attention 字符串的长度要+4
	virtual UGint GetDataSize() const;

	//! \brief 是否要求保存纹理数据,默认为TRUE
	void IsSaveTexture(UGbool bSaveTexture){m_bSaveTexture=bSaveTexture;}

	//! \brief 是否含有多通道用于多重纹理
	UGbool IsMultiTexture();
	
	//! \brief 添加纹理路径
	void  AddTexturePath(UGString strTexturePath);
	
	//! \brief 删除纹理名为strTexName的纹理数据
	void RemoveTextureData(UGString strTexName);

	//! \brief 获取多重纹理个数
	UGuint GetTextureUnits(){ return (UGint)m_arrTexPath.GetSize();}

	//! \brief 追加纹理数据
	void AddTextureData(UGTextureData* pTextureData);

	//! \brief 追加纹理动画信息
	//! \param  strTexName,纹理名称
	//! \param  timeTable 每帧的持续时间列表。
	void AddAnimation(UGString strTexName,std::vector<UGuint>timeTable);
	
	//! \brief 材质中含有多少个动画纹理
	UGint GetNumOfAnimation();

	//! \brief 索引为nIndex的纹理有多少帧
	//! \param  nIndex,纹理索引
	UGint GetNumFrameByIndex(UGint nIndex);

	//! \brief 索引为nIndex的纹理名称
	//! \param  nIndex,纹理索引
	UGString GetAnimationByIndex(UGint nIndex);


	//! \brief 返回索引为nIndex的纹理的第nIndexFrame持续时间
	//! \param  nIndex,纹理索引
	//! \param  nIndex,帧索引
	UGuint  GeFrameDelayTimeByIndex(UGint nIndex,UGint nIndexFrame);

	//! \brief 返回该纹理是否是动画
	UGbool IsAnimation(UGString strTexName);
protected:
	//! \brief 材质名称。
	//! \remarks 备注信息。
	UGString m_strName;				    /* name of material */

	//! \brief 漫反射参数。
	//! \remarks 备注信息。
	UGfloat m_diffuse[4];			       /* diffuse component */

	//! \brief 环境光参数。
	//! \remarks 备注信息。
	UGfloat m_ambient[4];			       /* ambient component */

	//! \brief 镜面反射参数。
	//! \remarks 备注信息。
	UGfloat m_specular[4];		   	   /* specular component */
	
	//! \brief 自发光参数。
	//! \remarks 备注信息。
	UGfloat m_emmissive[4];			/* emmissive component */
	
	//! \brief 光照强度。
	//! \remarks 备注信息。
	UGfloat m_fShininess;			/* specular exponent */ 

	//! \brief 纹理数据。
	//! \remarks 贴图信息先看m_pTextureData是不是为空，
	//! \remarks 如果为空才去用m_strTexPath加载路径
	UGArray<UGTextureData*> m_pTextureDataArray;

	//! \brief 是否保存纹理数据
	UGbool m_bSaveTexture;
	
	//! \brief 纹理路径名称列表。
	//! \remarks 第一个纹理路径为默认的纹理。
	UGArray<UGString> m_arrTexPath;

	//! \brief 纹理路径和纹理每帧持续时间列表。
	std::map<UGString,std::vector<UGuint> > m_animationMap;

	//! \brief 特效材质枚举
	//! \remarks 备注信息。
	MaterialEffectType m_nType;
} ;
}

#endif // !defined(AFX_UGMATERIAL_H__E85252C6_682E_4604_B276_C6EDA7B0F030__INCLUDED_)


