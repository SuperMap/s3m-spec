//////////////////////////////////////////////////////////////////////////
//                    _   _   ____   ____                    
//   Project         | | | | / ___) / ___)                   
//                   | | | || | __ | |                       
//                   | |_| || |_\ || \___                    
//                    \____/ \____| \ ___)     6.1           
//                                                           
//!  \file UGTechnique.h
//!  \brief 三维材质技术类。
//!  \details 完成材质技术类的封装
//!  \author sunyl
//!  \attention 
//!   Copyright (c) 1996-2012 SuperMap Software Co., Ltd. <br> 
//!   All Rights Reserved.
//!  \version 6.1
//////////////////////////////////////////////////////////////////////////

#ifndef __TECHNIQUE_H__
#define __TECHNIQUE_H__

#if _MSC_VER > 1000
#pragma once
#endif

#include "Base3D/UGPrerequisites3D.h"
#include "UGPass.h"

namespace UGC
{

//! \brief 三维材质类。
class BASE3D_API UGTechnique 
{
public:
	/** Directive used to manually control technique support based on the
	inclusion or exclusion of some factor.
	*/
	enum IncludeOrExclude
	{
		/// Inclusive - only support if present
		INCLUDE = 0,
		/// Exclusive - do not support if present
		EXCLUDE = 1
	};
	/// Rule controlling whether technique is deemed supported based on GPU vendor
	struct GPUVendorRule
	{
		GPUVendor vendor;
		IncludeOrExclude includeOrExclude;
		GPUVendorRule()
			: vendor(GPU_UNKNOWN), includeOrExclude(EXCLUDE) {}
		GPUVendorRule(GPUVendor v, IncludeOrExclude ie)
			: vendor(v), includeOrExclude(ie) {}
	};
	/// Rule controlling whether technique is deemed supported based on GPU device name
	struct GPUDeviceNameRule
	{
		UGString devicePattern;
		IncludeOrExclude includeOrExclude;
		UGbool caseSensitive;
		GPUDeviceNameRule()
			: includeOrExclude(EXCLUDE), caseSensitive(false) {}
		GPUDeviceNameRule(const UGString& pattern, IncludeOrExclude ie, UGbool caseSen)
			: devicePattern(pattern), includeOrExclude(ie), caseSensitive(caseSen) {}
	};
	typedef std::vector<GPUVendorRule> GPUVendorRuleList;
	typedef std::vector<GPUDeviceNameRule> GPUDeviceNameRuleList;
protected:
	GPUVendorRuleList mGPUVendorRules;
	GPUDeviceNameRuleList mGPUDeviceNameRules;

public:
	/// Constructor
	UGTechnique();
	/// Copy constructor
	UGTechnique(const UGTechnique& oth);
	~UGTechnique();

public:
	/** Set the name of the technique.*/
	void setName(const UGString& name) { mName = name;};
	/// Gets the name of the technique
	const UGString& getName(void) const { return mName; };
	/** Set the 'scheme name' for this technique.*/
	void setSchemeName(const UGString& schemeName) { mSchemeName = schemeName;};
	/** Returns the scheme to which this technique is assigned.*/
	const UGString& getSchemeName(void) const {return mSchemeName;};
	/** Assigns a level-of-detail (LOD) index to this Technique.*/
	void setLodIndex(UGushort index) { mLodIndex = index;};
	/** Gets the level-of-detail index assigned to this Technique. */
	UGushort getLodIndex(void) const { return mLodIndex; };
	/** return this material specific  shadow casting specific material*/
	const UGString& getShadowCasterMaterial() const { return mShadowCasterMaterialName;};
	/** set this material specific  shadow casting specific material*/
	void setShadowCasterMaterial(const UGString &name) { mShadowCasterMaterialName = name;};
	/** return this material specific shadow receiving specific material*/
	const UGString& getShadowReceiverMaterial() const { return mShadowReceiverMaterialName;};
	/** set this material specific  shadow receiving specific material*/
	void setShadowReceiverMaterial(const UGString &name) { mShadowReceiverMaterialName = name;};

	/** Add a rule which manually influences the support for this technique based
	on a pattern that matches a GPU device name (e.g. '*8800*').*/
	void addGPUDeviceNameRule(const UGString& devicePattern, IncludeOrExclude includeOrExclude, UGbool caseSensitive = FALSE);
	/** Add a rule which manually influences the support for this technique based
	on a pattern that matches a GPU device name (e.g. '*8800*').*/
	void addGPUDeviceNameRule(const GPUDeviceNameRule& rule);
	/** Removes a matching device name rule.*/
	void removeGPUDeviceNameRule(const UGString& devicePattern);

	/** Creates a new Pass for this Technique.*/
	UGPass* createPass(void);
	/** Retrieves the Pass with the given index. */
	UGPass* getPass(UGushort index);
	/** Retrieves the Pass matching name.*/
	UGPass* getPass(const UGString& name);
	/** Retrieves the number of passes. */
	UGushort getNumPasses(void) const;
	/** Removes the Pass with the given index. */
	void removePass(UGushort index);
	/** Removes all Passes from this Technique. */
	void removeAllPasses(void);

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
protected:
	typedef std::vector<UGPass*> Passes;
	/// List of primary passes
	Passes mPasses;

	UGString mName; // optional name for the technique
	UGString mSchemeName;
	UGushort mLodIndex;
	UGString mShadowCasterMaterialName;
	UGString mShadowReceiverMaterialName;

	//// illumination pass state type
	//enum IlluminationPassesState
	//{
	//	IPS_COMPILE_DISABLED = -1,
	//	IPS_NOT_COMPILED = 0,
	//	IPS_COMPILED = 1
	//};

	//typedef vector<UGPass*>::type Passes;
	///// List of primary passes
	//Passes mPasses;
	///// List of derived passes, categorised into IlluminationStage (ordered)
	////IlluminationPassList mIlluminationPasses;
	//UGMaterial3D* mParent; // raw pointer since we don't want child to stop parent's destruction
	//UGbool mIsSupported;
	//IlluminationPassesState mIlluminationPassesCompilationPhase;
	///// LOD level
	//unsigned short mLodIndex;
	///** Scheme index, derived from scheme name but the names are held on
	//MaterialManager, for speed an index is used here.
	//*/
	//unsigned short mSchemeIndex;
	//UGString mName; // optional name for the technique

	///** When casting shadow, if not using default Ogre shadow casting material, or 
	//* nor using fixed function casting, mShadowCasterMaterial let you customize per material
	//* shadow caster behavior
	//*/
	//UGMaterial3D mShadowCasterMaterial;
	///** When casting shadow, if not using default Ogre shadow casting material, or 
	//* nor using fixed function casting, mShadowCasterMaterial let you customize per material
	//* shadow caster behavior.There only material name is stored so that it can be loaded once all file parsed in a resource group.
	//*/
	//UGString mShadowCasterMaterialName;
	///** When receiving shadow, if not using default Ogre shadow receiving material, or 
	//* nor using fixed function texture projection receiving, mShadowReceiverMaterial let you customize per material
	//* shadow caster behavior
	//*/
	//UGMaterial3D mShadowReceiverMaterial;
	///** When receiving shadow, if not using default Ogre shadow receiving material, or 
	//* nor using fixed function texture projection receiving, mShadowReceiverMaterial let you customize per material
	//* shadow caster behavior. There only material name is stored so that it can be loaded once all file parsed in a resource group.
	//*/
	//UGString mShadowReceiverMaterialName;	

} ;
}

#endif 


