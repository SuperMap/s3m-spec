//////////////////////////////////////////////////////////////////////////
//                    _   _   ____   ____                    
//   Project         | | | | / ___) / ___)                   
//                   | | | || | __ | |                       
//                   | |_| || |_\ || \___                    
//                    \____/ \____| \ ___)     6.1           
//                                                           
//!  \file headerfile.h
//!  \brief 材质脚本解析器管理器类
//!  \details 完成对材质脚本解析器管理器类的封装。
//!  \author sunyl。
//!  \attention 
//!   Copyright (c) 1996-2012 SuperMap Software Co., Ltd. <br> 
//!   All Rights Reserved.
//!  \version 6.1
//////////////////////////////////////////////////////////////////////////


#ifndef __SCRIPTCOMPILERMANAGER_H__
#define __SCRIPTCOMPILERMANAGER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Base3D/UGPrerequisites3D.h"
#include "Toolkit/UGSingleton.h"
#include "Stream/UGStream.h"
#include "Base3D/UGScriptCompiler.h"
#include "Base3D/UGScriptTranslatorManager.h"

namespace UGC
{
	class UGScriptTranslator;
	class UGScriptTranslatorManager;

	class  BASE3D_API UGScriptCompilerManager : public UGSingleton<UGScriptCompilerManager>
	{
	public:
		UGScriptCompilerManager();
		virtual ~UGScriptCompilerManager();

	public:
		

	protected:
		// A list of patterns loaded by this compiler manager
		std::vector<UGString> mScriptPatterns;

		// A pointer to the listener used for compiling scripts
		//ScriptCompilerListener *mListener;

		// Stores a map from object types to the translators that handle them
		std::vector<UGScriptTranslatorManager*> mManagers;

		// A pointer to the built-in ScriptTranslatorManager
		UGScriptTranslatorManager *mBuiltinTranslatorManager;

		// A pointer to the specific compiler instance used
		UGScriptCompiler *mScriptCompiler;

	public:
		/*/// Sets the listener used for compiler instances
		void setListener(ScriptCompilerListener *listener);
		/// Returns the currently set listener used for compiler instances
		ScriptCompilerListener *getListener();*/

		/// Adds the given translator manager to the list of managers
		void addTranslatorManager(UGScriptTranslatorManager *man);
		/// Removes the given translator manager from the list of managers
		void removeTranslatorManager(UGScriptTranslatorManager *man);
		/// Clears all translator managers
		void clearTranslatorManagers();
		/// Retrieves a ScriptTranslator from the supported managers
		UGScriptTranslator* getTranslator(const AbstractNodePtr &node);

		/// Adds a script extension that can be handled (e.g. *.material, *.pu, etc.)
		void addScriptPattern(const UGString &pattern);
		/// @copydoc ScriptLoader::getScriptPatterns
		const std::vector<UGString>& getScriptPatterns(void) const;
		/// @copydoc ScriptLoader::parseScript
		void parseScript(UGStream& stream, const UGString& groupName);
		/// @copydoc ScriptLoader::getLoadingOrder
		UGdouble getLoadingOrder(void) const;

		static UGScriptCompilerManager& GetSingleton();
		static UGScriptCompilerManager* GetSingletonPtr();	
	};
}
#endif // !defined(AFX_UGSINGLETON_H__E89D0670_EB32_4782_8964_7F60F1421BCF__INCLUDED_)

