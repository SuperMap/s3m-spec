//////////////////////////////////////////////////////////////////////////
//                    _   _   ____   ____                    
//   Project         | | | | / ___) / ___)                   
//                   | | | || | __ | |                       
//                   | |_| || |_\ || \___                    
//                    \____/ \____| \ ___)     7.2           
//                                                           
//!  \file UGModelConfigParams.h
//!  \brief 模型文件数据导入导出参数设置。
//!  \details 主要包括UGModelImportParams导入参数设置类和UGModelExportParams导出参数设置类。
//!  \author 数据转换组。
//!  \attention 
//!   Copyright (c)   SuperMap Software Limited Company.  <br> 
//!   All Rights Reserved.
//!  \version 7.2
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_UGMODELIMPORTPARAMS_H__5D6357D7_FC27_48BC_8BA4_3A6788F5DD2F__INCLUDED_)
#define AFX_UGMODELIMPORTPARAMS_H__5D6357D7_FC27_48BC_8BA4_3A6788F5DD2F__INCLUDED_

#include "Stream/ugexports.h"
#include "FileParser/UGFileParserConfigParams.h"
#include "FileParser/UGVectorConfigParams.h"

#if _MSC_VER > 1000
#pragma once
#endif

namespace UGC {

	class  UGVectorFileLayer;
	class UGSpatialIndexInfo;


	//! \brief 模型文件导入的参数设置类。
	//! \remarks 继承自UGVectorImportParams矢量导入参数设置类。
	class FILEPARSER_API UGModelImportParams  : public UGVectorImportParams
	{
		friend class UGWorkspaceImportParams;
	public:
		//! \brief 构造函数。
		//! \remarks 构造一个新的导入参数配置对象。
		UGModelImportParams();
	
		//! \brief 析构函数。
		//! \remarks 释放导入参数配置信息。
		virtual ~UGModelImportParams();

		//! \brief 拷贝构造函数。
		//! \param tmp [in] 给定的UGModelImportParams对象。
		//! \remarks 根据给定的UGModelImportParams对象构造一个与其完全相同的新对象。
		UGModelImportParams(const UGModelImportParams& tmp);

		//! \brief 获取文件格式类型。
		//! \return 文件格式类型。
		virtual UGint GetFileType() const;

		//! \brief 设置模型的定位点。
		//! \param pntPosition3D [in] 定位点。
		void SetPosition(const UGPoint3D pntPosition3D);

		//! \brief 获取模型的定位地点。
		//! \return 定位点。
		UGPoint3D GetPosition()const;

		//! \brief 设置模型的缩放比例
		void SetScale(UGdouble fScaleX,UGdouble fScaleY,UGdouble fScaleZ);	

		//! \brief 获取模型的缩放比例
		UGPoint3D GetScale() const;	

		//! \brief  设置对象的旋转量
		void SetRotation(UGdouble fRotateX,UGdouble fRotateY,UGdouble fRotateZ);	

		//! \brief 获取对象的旋转量
		UGPoint3D GetRotation() const;

		//! \brief 是否分解ModeNode后再存储；FBX格式默认分解
		void SetDecompose(UGbool bDecompose);
		UGbool GetDecompose() const;

		//! \brief obj解析参数
		void SetObjRotateOpt(UGint opt);
		UGint GetObjRotateOpt() const;

	private:
		//! \brief 模型导入的定位点。
		//! \attention 这个参数由用户来传递进来。
		UGPoint3D m_pntPosition;

		//! \brief 模型导入的缩放系数。
		//! \attention 这个参数由用户来传递进来。
		UGPoint3D m_pntScale;

		//! \brief 模型导入的旋转量。
		//! \attention 这个参数由用户来传递进来。
		UGPoint3D m_pntRotate;

		//! \biref 存储时是否分解ModeNode为Geode
		UGbool m_bDecompose;

		UGint m_nObjRotateOpt;
	};


	//! \brief 模型文件导出的参数设置类。
	//! \remarks 继承自UGExportParams导出参数设置类。
	class FILEPARSER_API UGModelExportParams : public UGExportParams
	{
	public:
		//! \brief 构造函数。
		//! \remarks 构造一个新的导出参数配置对象。
		UGModelExportParams(){
			m_strDatasetName = _U("");
		}

		//! \brief 析构函数。
		//! \remarks 释放导出参数配置信息。
		virtual ~UGModelExportParams(){}

	public:
		//! \brief 设置要导出的矢量数据集名称。
		//! \param  name [in] 导出的矢量数据集名称。
		void SetDtNameToBeExported(const UGString& name);

		//! \brief 获取要导出的矢量数据集名称。
		//! \return 要导出的矢量数据集名称。
		UGString GetDtNameToBeExported() const;

		//! \brief 设置要导出的矢量数据集名称数组。
		//! \param arrNames [in] 数据集名称数组。
		//! \remarks 为了多个数据集导出使用。
		//! \attention 此接口是虚函数, 支持多数据集导出功能的格式需要重载。
		virtual void SetDtNamesBeExported(const UGStrings& arrNames){ ;}

		//! \brief 获取要导出的矢量数据集名称数组。
		//! \param arrNames [out] 数据集名称数组。
		//! \return true获取成功，false获取失败。
		//! \remarks 为了多个数据集导出使用。
		//! \attention 此接口是虚函数, 支持多数据集导出功能的格式需要重载。
		virtual UGbool GetDtNamesBeExported(UGStrings& arrNames) const {return FALSE;}

	private:	
		// \brief 要导出的数据集名称。
		UGString m_strDatasetName;
	};

} //namespace UGC
#endif // !defined(AFX_UGVECTORIMPORTPARAMS_H__5D6357D7_FC27_48BC_8BA4_3A6788F5DD2F__INCLUDED_)

