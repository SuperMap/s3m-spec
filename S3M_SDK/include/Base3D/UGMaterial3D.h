//////////////////////////////////////////////////////////////////////////
//                    _   _   ____   ____                    
//   Project         | | | | / ___) / ___)                   
//                   | | | || | __ | |                       
//                   | |_| || |_\ || \___                    
//                    \____/ \____| \ ___)     6.1           
//                                                           
//!  \file UGMaterial.h
//!  \brief 三维材质类。
//!  \details 完成材质类的封装
//!  \author sunyl
//!  \attention 
//!   Copyright (c) 1996-2012 SuperMap Software Co., Ltd. <br> 
//!   All Rights Reserved.
//!  \version 6.1
//////////////////////////////////////////////////////////////////////////

#ifndef __MATERIAL3D_H__
#define __MATERIAL3D_H__

#if _MSC_VER > 1000
#pragma once
#endif

#include "Base3D/UGPrerequisites3D.h"
#include "Base3D/UGSharedPtr.h"
#include "Base3D/UGMaterialScriptManager.h"
#include "Base3D/UGTechnique.h"
#include "Base3D/UGMaterial.h"

namespace UGC
{

class UGTechnique;

//! \brief 三维材质类。
class BASE3D_API UGMaterial3D
{
	friend class UGMaterialScriptManager;
public:
	/// distance list used to specify LOD
	typedef std::vector<UGdouble> LodValueList;
public:
	//! \brief 构造函数。
	UGMaterial3D();
	//! \brief 构造函数。
	UGMaterial3D(const UGString& strName, const UGString& strGroupName);
	//! \brief 拷贝构造函数。
	UGMaterial3D(const UGMaterial3D& other);

	//! \brief 赋值函数
	UGMaterial3D& operator=(const UGMaterial3D& other);
	
	//! \brief 构造函数。
	~UGMaterial3D();
public:
	const UGString& GetMaterialName() const
	{
		return m_strName;
	}

	const UGString& GetMaterialGroupName() const
	{
		return m_strGroupName;
	}

	/** Sets the distance at which level-of-detail (LOD) levels come into effect.*/
	void setLodLevels(const LodValueList& lodValues);
	/** Sets whether objects using this material will receive shadows.*/
	void setReceiveShadows(UGbool enabled) { mReceiveShadows = enabled; };
	/** Returns whether or not objects using this material will receive shadows. */
	UGbool getReceiveShadows(void) const { return mReceiveShadows; };
	/** Sets whether objects using this material be classified as opaque to	the shadow caster system.*/
	void setTransparencyCastsShadows(UGbool enabled) { mTransparencyCastsShadows = enabled; };
	/** Returns whether or not objects using this material be classified as opaque to the shadow caster system. */
	UGbool getTransparencyCastsShadows(void) const { return mTransparencyCastsShadows; };
	/** Creates a new Technique for this Material.*/
	UGTechnique* createTechnique(void);
	/** Gets the indexed technique. */
	UGTechnique* getTechnique(UGushort index);
	/** searches for the named technique.
	Return 0 if technique with name is not found
	*/
	UGTechnique* getTechnique(const UGString& name);
	/** Retrieves the number of techniques. */
	UGuint getNumTechniques(void) const;
	/** Removes the technique at the given index. */		
	void removeTechnique(UGushort index);		
	/** Removes all the techniques in this Material. */
	void removeAllTechniques(void);
	/** Applies texture names to Texture Unit State with matching texture name aliases.
	All techniques, passes, and Texture Unit States within the material are checked.
	If matching texture aliases are found then true is returned.*/
	typedef std::map<UGString, UGString> AliasTextureNamePairList;
	UGbool applyTextureAliases(const AliasTextureNamePairList& aliasList, UGbool apply = TRUE) const;

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
public:

	//! \brief 材质名称。
	//! \remarks 备注信息。
	//UGString m_strName;

	
	//const LodStrategy *mLodStrategy;
	//UGbool mReceiveShadows;
	//UGbool mTransparencyCastsShadows;
	///// Does this material require compilation?
	//UGbool mCompilationRequired;
	///// Text description of why any techniques are not supported
	//UGString mUnsupportedReasons;

public:
	//! \brief 材质名称。
	//! \remarks 备注信息。
	UGString m_strName;

	//! \brief 材质所在组的名称
	UGString m_strGroupName;

	typedef std::vector<UGTechnique*> Techniques;
	/// All techniques, supported and unsupported
	Techniques mTechniques;
	/// Supported techniques of any sort
	Techniques mSupportedTechniques;
	//typedef map<unsigned short, UGTechnique*>::type LodTechniques;
	////typedef map<unsigned short, LodTechniques*>::type BestTechniquesBySchemeList;
	///** Map of scheme -> list of LOD techniques. 
	//Current scheme is set on MaterialManager,
	//and can be set per Viewport for auto activation.
	//*/
	////BestTechniquesBySchemeList mBestTechniquesBySchemeList;

	LodValueList mUserLodValues;
	LodValueList mLodValues;
	UGbool mReceiveShadows;
	UGbool mTransparencyCastsShadows;

	//! \brief 特效材质枚举
	//! \remarks 备注信息。
	UGMaterial::MaterialEffectType m_nType;

	//! \brief 材质版本号
	UGdouble m_dVersion;
};

//! \brief 材质对象的智能指针实现
//class BASE3D_API UGMaterial3DPtr : public UGSharedPtr<UGMaterial3D>
//{
//public:	
//	//! \brief 构造函数
//	UGMaterial3DPtr() : UGSharedPtr<UGMaterial3D>(){}
//
//	//! \brief 拷贝构造函数
//	explicit UGMaterial3DPtr(UGMaterial3D* rep):UGSharedPtr<UGMaterial3D>(rep){}
//};


}

#endif 


