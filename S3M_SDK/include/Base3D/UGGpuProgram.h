//////////////////////////////////////////////////////////////////////////
//                    _   _   ____   ____                    
//   Project         | | | | / ___) / ___)                   
//                   | | | || | __ | |                       
//                   | |_| || |_\ || \___                    
//                    \____/ \____| \ ___)     6.1           
//                                                           
//!  \file UGMaterial.h
//!  \brief 三维GPU程序类
//!  \details 完成GPU程序的封装
//!  \author sunyl
//!  \attention 
//!   Copyright (c) 1996-2012 SuperMap Software Co., Ltd. <br> 
//!   All Rights Reserved.
//!  \version 6.1
//////////////////////////////////////////////////////////////////////////

#ifndef __GPUPROGRAM_H__
#define __GPUPROGRAM_H__

#if _MSC_VER > 1000
#pragma once
#endif

#include "Base3D/UGPrerequisites3D.h"
#include "Base3D/UGGpuProgramParams.h"
#include "Base3D/UGSharedPtr.h"

namespace UGC
{
	/** Defines a program which runs on the GPU such as a vertex or fragment program. 
	@remarks
	This class defines the low-level program in assembler code, the sort used to
	directly assemble into machine instructions for the GPU to execute. By nature,
	this means that the assembler source is rendersystem specific, which is why this
	is an abstract class - real instances are created through the RenderSystem. 
	If you wish to use higher level shading languages like HLSL and Cg, you need to 
	use the HighLevelGpuProgram class instead.
	*/
	class BASE3D_API UGGpuProgram
	{
	public:
		UGString m_strName;
		//所属于的资源组名称
		UGString m_strResourceGroup;
		/// The type of the program
		GpuProgramType m_enType;
		/// The name of the file to load source from (may be blank)
		UGString m_strFilename;
		/// The assembler source of the program (may be blank until file loaded)
		UGString m_strSource;
		/// Whether we need to load source from file or not
		UGbool m_bLoadFromFile;
		/// Syntax code e.g. arbvp1, vs_2_0 etc
		UGString m_strSyntaxCode;
		/// Does this (vertex) program include skeletal animation?
		UGbool m_bSkeletalAnimation;
		/// Does this (vertex) program include morph animation?
		UGbool m_bMorphAnimation;
		/// Does this (vertex) program include pose animation (count of number of poses supported)
		UGushort m_uPoseAnimation;
		/// Does this (vertex) program require support for vertex texture fetch?
		UGbool m_bVertexTextureFetch;
		/// Does this (geometry) program require adjacency information?
		UGbool m_bNeedsAdjacencyInfo;
		/// The default parameters for use with this object
		UGGpuProgramParametersSharedPtr m_DefaultParams;
	public:

		UGGpuProgram();

		UGGpuProgram(const UGGpuProgram& oth);

		virtual ~UGGpuProgram();
	public:
		void SetParameter(const UGString& strParamName, const UGString& strParamValue);


		//! \从XML字符串读取参数设置
		//! \param strXML XML字符串  [in]
		//! \return 返回是否成功
		UGbool FromXML(UGString& strXML);

		//! \保存参数设置到XML字符串
		//! \param strXML XML字符串  [in]
		//! \return 返回是否成功
		UGbool ToXML(UGString& strXML);
	private:
		/** Converts a String to a boolean. 
		@remarks
		Returns true if case-insensitive match of the start of the string
		matches "true", "yes" or "1", false otherwise.
		*/
		bool ParseBool(const UGString& val, bool defaultValue = 0);

		bool StartsWith(const UGString& str, const UGString& pattern);
	};
}

#endif 


