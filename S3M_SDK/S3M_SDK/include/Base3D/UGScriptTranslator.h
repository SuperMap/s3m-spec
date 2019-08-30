//////////////////////////////////////////////////////////////////////////
//                    _   _   ____   ____                    
//   Project         | | | | / ___) / ___)                   
//                   | | | || | __ | |                       
//                   | |_| || |_\ || \___                    
//                    \____/ \____| \ ___)     6.1           
//                                                           
//!  \file UGMaterial.h
//!  \brief 三维材质翻译器类。
//!  \details 完成材质类的封装
//!  \author sunyl
//!  \attention 
//!   Copyright (c) 1996-2012 SuperMap Software Co., Ltd. <br> 
//!   All Rights Reserved.
//!  \version 6.1
//////////////////////////////////////////////////////////////////////////

#ifndef __SCRIPTTRANSLATOR_H__
#define __SCRIPTTRANSLATOR_H__

#if _MSC_VER > 1000
#pragma once
#endif

#include "Base3D/UGPrerequisites3D.h"
#include "Base3D/UGScriptCompiler.h"
#include "Base3D/UGScriptCompilerManager.h"
#include "Base3D/UGMaterial3D.h"
#include "Base3D/UGTechnique.h"
#include "Base3D/UGGpuProgramManager.h"
#include "Stream/UGStream.h"

namespace UGC
{

//! \brief 三维材质类。
class BASE3D_API UGScriptTranslator
{
public:
	//! \brief 构造函数。
	UGScriptTranslator();

public:
	/**
	* This function translates the given node into Ogre resource(s).
	* @param compiler The compiler invoking this translator
	* @param node The current AST node to be translated
	*/
	virtual void translate(UGScriptCompiler *compiler, const AbstractNodePtr &node) = 0;
protected:
	// needs virtual destructor
	virtual ~UGScriptTranslator() {}
	/// Retrieves a new translator from the factories and uses it to process the give node
	void processNode(UGScriptCompiler *compiler, const AbstractNodePtr &node);

	/// Retrieves the node iterator at the given index
	static AbstractNodeList::const_iterator getNodeAt(const AbstractNodeList &nodes, UGint index);
	/// Converts the node to a boolean and returns true if successful
	static UGbool getBoolean(const AbstractNodePtr &node, UGbool *result);
	/// Converts the node to a string and returns true if successful
	static UGbool getString(const AbstractNodePtr &node, UGString *result);
	/// Converts the node to a Real and returns true if successful
	static UGbool getDouble(const AbstractNodePtr &node, UGdouble *result);
	/// Converts the node to a float and returns true if successful
	static UGbool getFloat(const AbstractNodePtr &node, UGfloat *result);
	/// Converts the node to an integer and returns true if successful
	static UGbool getInt(const AbstractNodePtr &node, UGint *result); 
	/// Converts the node to an unsigned integer and returns true if successful
	static UGbool getUInt(const AbstractNodePtr &node, UGuint *result); 
	/// Converts the range of nodes to a ColourValue and returns true if successful
	static UGbool getColour(AbstractNodeList::const_iterator i, AbstractNodeList::const_iterator end, UGColorValue3D *result, UGint maxEntries = 4);
	/// Converts the node to a SceneBlendFactor enum and returns true if successful
	static UGbool getSceneBlendFactor(const AbstractNodePtr &node, SceneBlendFactor *sbf);
	/// Converts the node to a CompareFunction enum and returns true if successful
	static UGbool getCompareFunction(const AbstractNodePtr &node, CompareFunction *func);
	/// Converts the range of nodes to a Matrix4 and returns true if successful
	static UGbool getMatrix4(AbstractNodeList::const_iterator i, AbstractNodeList::const_iterator end, UGMatrix4d *m);
	/// Converts the range of nodes to an array of ints and returns true if successful
	static UGbool getInts(AbstractNodeList::const_iterator i, AbstractNodeList::const_iterator end, UGint *vals, UGint count);
	/// Converts the range of nodes to an array of floats and returns true if successful
	static UGbool getFloats(AbstractNodeList::const_iterator i, AbstractNodeList::const_iterator end, UGfloat *vals, UGint count);
	/// Converts the node to a StencilOperation enum and returns true if successful
	static UGbool getStencilOp(const AbstractNodePtr &node, StencilOperation *op); 
	/// Converts the node to a GpuConstantType enum and returns true if successful
	static UGbool getConstantType(AbstractNodeList::const_iterator i, GpuConstantType *op); 


} ;

class  BASE3D_API UGScriptTranslatorManager
{
public:
	// required - virtual destructor
	virtual ~UGScriptTranslatorManager() {}

	/// Returns the number of translators being managed
	virtual UGuint getNumTranslators() const = 0;
	/// Returns a manager for the given object abstract node, or null if it is not supported
	virtual UGScriptTranslator *getTranslator(const AbstractNodePtr&) = 0;
};

/**************************************************************************
* Material compilation section
*************************************************************************/
class UGMaterial3D;
class BASE3D_API UGMaterialTranslator : public UGScriptTranslator
{
protected:
	UGMaterial3D* mMaterial;

	/// Alias / Texture name pair (first = alias, second = texture name)
	typedef std::map<UGString, UGString> AliasTextureNamePairList;
	AliasTextureNamePairList mTextureAliases;
public:
	UGMaterialTranslator();
	virtual void translate(UGScriptCompiler *compiler, const AbstractNodePtr &node);
};

class BASE3D_API UGTechniqueTranslator : public UGScriptTranslator
{
protected:
	UGTechnique *mTechnique;
public:
	UGTechniqueTranslator();
	virtual void translate(UGScriptCompiler *compiler, const AbstractNodePtr &node);
};

class BASE3D_API UGPassTranslator : public UGScriptTranslator
{
protected:
	UGPass *mPass;
public:
	UGPassTranslator();
	virtual void translate(UGScriptCompiler *compiler, const AbstractNodePtr &node);
protected:
	void translateVertexProgramRef(UGScriptCompiler *compiler, ObjectAbstractNode *node);
	void translateGeometryProgramRef(UGScriptCompiler *compiler, ObjectAbstractNode *node);
	void translateFragmentProgramRef(UGScriptCompiler *compiler, ObjectAbstractNode *node);
	void translateShadowCasterVertexProgramRef(UGScriptCompiler *compiler, ObjectAbstractNode *node);
	void translateShadowReceiverVertexProgramRef(UGScriptCompiler *compiler, ObjectAbstractNode *node);
	void translateShadowReceiverFragmentProgramRef(UGScriptCompiler *compiler, ObjectAbstractNode *node);
};

class BASE3D_API UGTextureUnitTranslator : public UGScriptTranslator
{
protected:
	UGTextureUnitState *mUnit;
public:
	UGTextureUnitTranslator();
	virtual void translate(UGScriptCompiler *compiler, const AbstractNodePtr &node);
};

class BASE3D_API UGTextureSourceTranslator : public UGScriptTranslator
{
public:
	UGTextureSourceTranslator();
	virtual void translate(UGScriptCompiler *compiler, const AbstractNodePtr &node);
};

class BASE3D_API UGGpuProgramTranslator : public UGScriptTranslator
{	
public:
	UGGpuProgramTranslator();
	virtual void translate(UGScriptCompiler *compiler, const AbstractNodePtr &node);
protected:
	void translateGpuProgram(UGScriptCompiler *compiler, ObjectAbstractNode *obj);
	void translateHighLevelGpuProgram(UGScriptCompiler *compiler, ObjectAbstractNode *obj);
	void translateUnifiedGpuProgram(UGScriptCompiler *compiler, ObjectAbstractNode *obj);
public:
	/// Shared pointer used to hold references to GpuProgramParameters instances
	typedef UGSharedPtr<UGGpuProgramParameters> GpuProgramParametersSharedPtr;
	static void translateProgramParameters(UGScriptCompiler *compiler, GpuProgramParametersSharedPtr params, ObjectAbstractNode *obj);
};

/**************************************************************************
* BuiltinScriptTranslatorManager
*************************************************************************/
/// This class manages the builtin translators
class BASE3D_API UGBuiltinScriptTranslatorManager : public UGScriptTranslatorManager
{
private:
	UGMaterialTranslator mMaterialTranslator;
	UGTechniqueTranslator mTechniqueTranslator;
	UGPassTranslator mPassTranslator;
	UGTextureUnitTranslator mTextureUnitTranslator;
	UGTextureSourceTranslator mTextureSourceTranslator;
	UGGpuProgramTranslator mGpuProgramTranslator;
	/*
	SharedParamsTranslator mSharedParamsTranslator;
	ParticleSystemTranslator mParticleSystemTranslator;
	ParticleEmitterTranslator mParticleEmitterTranslator;
	ParticleAffectorTranslator mParticleAffectorTranslator;
	CompositorTranslator mCompositorTranslator;
	CompositionTechniqueTranslator mCompositionTechniqueTranslator;
	CompositionTargetPassTranslator mCompositionTargetPassTranslator;
	CompositionPassTranslator mCompositionPassTranslator;
	CompositionPassClearTranslator mCompositionPassClearTranslator;
	CompositionPassStencilTranslator mCompositionPassStencilTranslator;*/
public:
	UGBuiltinScriptTranslatorManager();
	/// Returns the number of translators being managed
	virtual UGuint getNumTranslators() const;
	/// Returns a manager for the given object abstract node, or null if it is not supported
	virtual UGScriptTranslator *getTranslator(const AbstractNodePtr &node);
};



}

#endif 


