//////////////////////////////////////////////////////////////////////////
//                    _   _   ____   ____                    
//   Project         | | | | / ___) / ___)                   
//                   | | | || | __ | |                       
//                   | |_| || |_\ || \___                    
//                    \____/ \____| \ ___)     7.2           
//                                                           
//!  \file UGVectorConfigParams.h
//!  \brief 矢量基本信息及参数设置文件。
//!  \details 主要包括UGFeatureClassDefn要素描述信息类、UGFeatureClassInfos数据集基本信息类、<br>
//!		UGVectorImportParams矢量导入参数设置类和UGVectorExportParams矢量导出参数设置类。
//!  \author 数据转换组。
//!  \attention 
//!   Copyright (c)   SuperMap Software Limited Company.  <br> 
//!   All Rights Reserved.
//!  \version 7.2
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_UGVECTORIMPORTPARAMS_H__5D6357D7_FC27_48BC_8BA4_3A6788F5DD2F__INCLUDED_)
#define AFX_UGVECTORIMPORTPARAMS_H__5D6357D7_FC27_48BC_8BA4_3A6788F5DD2F__INCLUDED_

#include "Stream/ugexports.h"
#include "FileParser/UGFileParserConfigParams.h"

#if _MSC_VER > 1000
#pragma once
#endif

namespace UGC {

	class  UGVectorFileLayer;
	class UGSpatialIndexInfo;


	//! \brief 要素描述信息类。
	//! \remarks 包括导入后结果数据集类型、对象个数、数据集范围等。
	class FILEPARSER_API UGFeatureClassDefn
	{
		friend class UGVectorFileLayer;

	public:
		//! \brief 构造函数。
		//! \remarks 构造一个新的要素描述信息对象。
		UGFeatureClassDefn();

		//! \brief 拷贝构造函数。
		//! \param ClassDef [in] 要素描述信息对象。
		//! \remarks 根据给定要素描述信息对象构造一个相同的要素描述信息对象。
		UGFeatureClassDefn(const UGFeatureClassDefn& ClassDef);

		//! \brief 析构函数。
		//! \remarks 释放要素描述信息。
		virtual ~UGFeatureClassDefn();

		//! \brief 重载操作符 =。
		//! \param fcdef [in] 要素描述信息。
		//! \return 要素描述信息。
		UGFeatureClassDefn& operator=(const UGFeatureClassDefn& fcdef);

	public:
		//! \brief 导入后结果数据集类型。
		//! \remarks 枚举值和 UGDataset::DatasetType 的值一致。
		enum FeatureClassType
		{
			//! \brief 不认识。
			UnKnown		=-1,
			//! \brief 只有属性表，没有几何对象。
			Tabular		=0,
			//! \brief 点数据集。
			Point		=1,
			//! \brief 线数据集。
			Line		=3,
			//! \brief 网络数据集。
			Network		=4,
			//! \brief 多边形数据集。
			Region		=5,
			//! \brief 文本数据集。
			Text		=7,
			//! \brief 参数化线数据集。
			CompoundLine   = 8,
			//! \brief 参数化面数据集。
			CompoundRegion = 9,
			//! \brief 路线数据集。
			LineM		=35,
			//! \brief 三维点。
			PointZ		=101,
			//! \brief 三维线。
			LineZ		=103,
			//! \brief 三维面。
			RegionZ		=105,
			//! \brief 带法向量的Point3D。
			PointGL		=137,
			//! \brief TIN数据集。
			TIN			=139,
			//! \brief CAD数据集。
			CAD			=149,
			//! \brief Topo数据集。
			Topo		=154,
			//! \brief 新的Tin数据和以前的区分开。
			TIN2		=155,
			//! 模型数据集
			Model       =203,
		};

		//! \brief 设置数据集类型。
		//! \param nType [in] 数据集类型。
		void SetType(UGint nType);

		//! \brief 获取数据集类型。
		//! \return 数据集类型。
		UGint GetType() const;

		//! \brief 设置结果数据集名称。
		//! \param strNewName [in] 结果数据集名称。
		void SetName(const UGString& strNewName);

		//! \brief 获取结果数据集名称。
		//! \return 结果数据集名称。
		UGString GetName() const;

		//! \brief 设置对象个数。
		//! \param nCount [in] 对象个数。
		void SetFeatureCount(UGint nCount);

		//! \brief 获取对象个数。
		//! \return 对象个数。
		UGint GetFeatureCount() const;

		//! \brief 设置数据集范围。
		//! \param rect2D [in] 数据集范围。
		void SetBound(const UGRect2D& rect2D);

		//! \brief 获取数据集范围。
		//! \return 数据集范围。
		UGRect2D GetBound() const;

		//! \brief 获取字段个数。
		//! \return 字段个数。
		UGint GetFieldCount() const;

		//! \brief 获取指定索引位置字段信息。
		//! \param nIndex [in] 指定索引位置。
		//! \return 字段信息。
		//! \remarks nIndex取值:[0 and GetFieldCount()-1]。
		//! \attention 用户不得删除返回的字段指针。
		UGFieldInfo* GetFieldDefn(UGint nIndex) const;

		//! \brief 设置字段信息。
		//! \param fieldInfos [in] 字段信息。
		void SetFieldInfos(const UGFieldInfos& fieldInfos);

		//! \brief 获取字段信息。
		//! \return 字段信息的拷贝。
		UGFieldInfos GetFieldInfos() const;

		//! \brief 添加一个新的字段。
		//! \param newFieldDefn [in] 新字段。
		//! \return true添加成功，false添加失败。
		//! \attention 函数内部复制传入的参数，用户负责释放参数。
		UGbool AddFieldDefn(UGFieldInfo* newFieldDefn);

		//! \brief 移除指定索引位置字段。
		//! \param nfield [in] 指定索引位置。
		//! \remarks nfield取值:[0 and GetFieldCount()-1]。
		void RemoveFieldDefn(UGint nfield);

		//! \brief 移除所有字段。
		void RemoveAllFieldDefn();

		//! \brief 按照指定的两个索引位置交换字段顺序。
		//! \param nFirst [in] 第一个索引位置。
		//! \param nSecond [in] 第二个索引位置。
		//! \return true交换成功，false交换失败。
		//! \remarks nFirst和nSecond取值:[0 and GetFieldCount()-1]。
		UGbool ChangeFieldDefn(UGint nFirst, UGint nSecond);

		//! \brief 按照指定的两个索引位置交换CAD扩展数据的的字段顺序。
		//! \param nFirst [in] 第一个索引位置。
		//! \param nSecond [in] 第二个索引位置。
		//! \return true交换成功，false交换失败。
		//! \remarks nFirst和nSecond取值:[0 and m_xDataFieldInfos.GetSize()-1]。
		UGbool ShiftxDataField(UGint nFirst, UGint nSecond);

		//! \brief 设置layer的原始名称。
		//! \param strNewName [in] 新名称。
		void SetOriginalName(const UGString& strNewName);

		//! \brief 获取layer的原始名称。
		//! \return 原始名称。
		UGString GetOriginalName() const;

		//! \brief 设置CAD扩展字段名称。
		//! \param DictxDataFieldNames [in] CAD扩展字段名称字典。
		//! \attention 用户也仅限于修改字段名称的权限。
		void SetxDataFieldNames(UGDict<UGString,UGString>& DictxDataFieldNames);

		//! \brief 获取源字段名称。
		//! \return 源字段名称数组。
		UGStrArray GetSourceFieldNames() const;

		//! \brief 获取CAD扩展字段名称。
		//! \return CAD扩展字段名称字典。
		UGDict<UGString,UGString> GetxDataFieldNames() const;

		//! \brief 设置源数据的空间坐标系。
		//! \param pSpatialRef [in] 空间坐标系。
		void SetSpatialRef(const UGPrjCoordSys* pSpatialRef);

		//! \brief 获取源数据的空间坐标系。
		//! \return 源数据的空间坐标系。
		UGPrjCoordSys* GetSpatialRef() const {return m_pSpatialRef;}

		//! \brief 设置DWG/DXF扩展数据的字段。
		//! \param xDataFieldInfos [in] DWG/DXF扩展数据的字段。
		void SetxDataFieldInfos(const UGFieldInfos& xDataFieldInfos);

		//! \brief 获取DWG/DXF扩展数据的字段。
		//! \return DWG/DXF扩展数据的字段信息。
		UGFieldInfos GetxDataFieldInfos()const;

		//! \brief 设置S57数据扩展信息。
		//! \param strExtInfo [in] S57数据扩展信息。
		void SetS57ExtInfo(const UGString& strExtInfo);

		//! \brief 获取S57数据扩展信息。
		//! \return S57数据扩展信息。
		UGString GetS57ExtInfo() const;

		//! \brief 设置原始文件的编码信息。
		//! \param fileEncoding [in] 原始文件的编码信息。
		void SetEncoding(UGString::Charset fileEncoding);

		//! \brief 获取原始文件的编码信息。
		//! \return 原始文件的编码信息。
		UGString::Charset GetEncoding() const;
		
		//! \brief 设置数据集最小Z值。
		//! \param dMinZ [in] 最小Z值。
		void SetMinZ(UGdouble dMinZ);

		//! \brief 获取数据集最小Z值。
		//! \return 最小Z值。
		UGdouble GetMinZ() const;

		//! \brief 设置数据集最大Z值。
		//! \param dMaxZ [in] 最大Z值。
		void SetMaxZ(UGdouble dMaxZ);

		//! \brief 获取数据集最大Z值。
		//! \return 最大Z值。
		UGdouble GetMaxZ() const;

	private:
		//! \brief 初始化要素描述信息参数。
		void Init();

		//! \brief 释放字段字典。
		void ClearFieldDict();

		//! \brief 通过字段名称获取该字段的索引。
		//! \param strFieldName [in] 字段名称。
		//! \return 字段的索引。
		UGint FindIndexByName(const UGString& strFieldName)const;

	private:
		//! \brief 要素类型。
		FeatureClassType m_Type;
	
		//! \brief 对象个数。
		UGint m_nFeatureCount;
	
		//! \brief 数据集的范围。
		UGRect2D m_Bound;

		//! \brief 字段结构。
		UGDict<UGString, UGFieldInfo*> m_dictFieldInfos;

		//! \brief 字段名称数组。
		//! \remarks 维护字段的顺序关系，使之和原始字段顺序一致。
		UGStrArray m_arrfieldNames;

		//! \brief 字段名称数组备份。
		//! \remarks 以防删除m_arrfieldNames中的字段名称。
		UGStrArray m_arrCopyfieldNames;

		//! \brief 结果数据集名称。
		UGString m_strAliasName;

		//! \brief 源数据Layer名称。
		//! \remarks 数据中Layer的真实名称，不能修改。
		UGString m_strLayerName;

		//! \brief CAD扩展数据的字段名称。
		UGDict<UGString,UGString> m_DictxDataFieldNames;
	
		//! \brief CAD数据中扩展属性字段结构。
		UGFieldInfos m_xDataFieldInfos;

		//! \brief  数据集投影
		UGPrjCoordSys* m_pSpatialRef;

		//! \brief S57数据扩展信息。
		UGString m_strExtInfo;

		//! \brief 原始文件读取的编码信息。
		UGString::Charset m_Encoding;

		//! \brief 数据集的最小Z值。
		UGdouble m_dMinZ;

		//! \brief 数据集的最大Z值。
		UGdouble m_dMaxZ;
	};


	//! \brief 数据集基本信息类。
	//! \remarks 由多个要素描述信息组成。
	class FILEPARSER_API UGFeatureClassInfos : public UGArray<UGFeatureClassDefn> 
	{
	public:
		//! \brief 构造函数。
		//! \remarks 构造一个新的数据集基本信息对象。
		UGFeatureClassInfos();
	
		//! \brief 析构函数。
		//! \remarks 释放数据集基本信息。
		virtual ~UGFeatureClassInfos();

	public:
		//! \brief 通过名称获取一个层要素定义。
		//! \param strName [in] 层名称。
		//! \return 层要素定义。
		const UGFeatureClassDefn* GetFeatureDefnByName(const UGString& strName) const;
	};


	//! \brief 矢量导入参数设置类。
	//! \remarks 继承自UGImportParams导入参数设置类，包括数据集基本信息和空间索引等。
	class FILEPARSER_API UGVectorImportParams  : public UGImportParams
	{
		friend class UGWorkspaceImportParams;
	public:
		//! \brief 构造函数。
		//! \remarks 构造一个新的导入参数配置对象。
		UGVectorImportParams();
	
		//! \brief 析构函数。
		//! \remarks 释放导入参数配置信息。
		virtual ~UGVectorImportParams();
	
		//! \brief 拷贝构造函数。
		//! \param tmp [in] 矢量导入参数对象。
		//! \remarks 根据给定矢量导入参数对象构造一个相同的导入参数配置对象。
		UGVectorImportParams(const UGVectorImportParams& tmp);

		//! \brief 重载=。
		//! \param tmp [in] 矢量导入参数。
		//! \return 矢量导入参数。
		UGVectorImportParams& operator=(const UGVectorImportParams& tmp);

	public:
		//! \brief 设置导入的数据集结果信息。
		//! \param impDatasetInfos [in] 数据集结果信息。
		void SetImportDatasetInfos(const UGFeatureClassInfos& impDatasetInfos);

		//! \brief 获取导入的数据集结果信息。
		//! \return 导入的数据集结果信息。
		UGFeatureClassInfos& GetImportDatasetInfos();

		//! \brief 设置是否忽略空数据集。
		//! \param bHoldNoGeometryDT [in] true忽略空数据集，false不忽略空数据集。
		//! \remarks 没有记录的数据集为空数据集。
		void SetIgnoreNoGeometryDataset(UGbool bHoldNoGeometryDT = TRUE);

		//! \brief 获取是否忽略空数据集。
		//! \return true忽略空数据集，false不忽略空数据集。
		//! \remarks 没有记录的数据集为空数据集。
		UGbool GetIgnoreNoGeometryDataset() const;

		//! \brief 设置导入后建立的空间索引类型。
		//! \param spatialIndx [in] 空间索引类型。
		void SetBuildSpatialIndex(const UGSpatialIndexInfo* spatialIndx);

		//! \brief 获取建立的空间索引类型。
		//! \return 空间索引类型。
		//! \remarks 内部管理的成员，外面不能释放。
		UGSpatialIndexInfo* GetBuildSpatialIndex() const;

		//! \brief 设置目标数据集名称。
		//! \param strDtName [in] 目标数据集名称。
		void SetDestDTVectorName(const UGString& strDtName);

		//! \breif 设置指定名称字段的值
		void SetFieldValue(const UGString& strFieldName, const UGVariant& varValue);

		//! \breif 获取指定的字段名称的字段值
		void GetFieldValue(UGString& strFieldName, UGVariant& varValue);

		//! \breif 获取只导入空表
		UGbool GetOnlyImportEmptyTable() const;

		//! \breif 设置只导入空表 
		void SetOnlyImportEmptyTable(const UGbool bImportEmptyTable);

	private:
		//! \brief 数据集结果信息。
		UGFeatureClassInfos m_dtInfos;

		//! \brief 空间索引信息。
		UGSpatialIndexInfo* m_pSpatialIndex;

		//! \breif 字段值变体
		UGVariant m_varValue;

		//! \breif 字段名称
		UGString m_strFieldName;

		//! \breif 是否只导入空表
		UGbool m_bImportEmptyTable;
	};


	//! \brief 矢量导出参数设置类。
	//! \remarks 继承自UGExportParams导出参数设置类，包括导出数据集信息，导出忽略字段等信息。
	class FILEPARSER_API UGVectorExportParams : public UGExportParams
	{
	public:
		//! \brief 构造函数。
		//! \remarks 构造一个新的导出参数配置对象。
		UGVectorExportParams();

		//! \brief 析构函数。
		//! \remarks 释放导出参数配置信息。
		virtual ~UGVectorExportParams();

	public:
		//! \brief 设置要导出的矢量数据集名称。
		//! \param name [in] 设置的矢量数据集名称。
		void SetDtNameToBeExported(const UGString& name);

		//! \brief 获取要导出的矢量数据集名称。
		//! \return 矢量数据集名称。
		UGString GetDtNameToBeExported() const;

		//! \brief 设置要导出的矢量数据集名称数组。
		//! \param arrNames [in] 设置的数据集名称数组。
		//! \remarks 为了多个数据集导出使用。
		//! \attention 此接口是虚函数, 支持多数据集导出功能的格式需要重载。
		virtual void SetDtNamesBeExported(const UGStrings& arrNames){ ;}

		//! \brief 获取要导出的矢量数据集名称数组。
		//! \param arrNames [out] 数据集名称数组。
		//! \return true获取成功，false获取失败。
		//! \remarks 为了多个数据集导出使用。
		//! \attention 此接口是虚函数, 支持多数据集导出功能的格式需要重载。
		virtual UGbool GetDtNamesBeExported(UGStrings& arrNames) const {return FALSE;}

		//! \brief 设置不导出字段。
		//! \param ignoreFieldNames [in] 不导出字段名称数组。
		virtual void SetIgnoreFieldsBeExported(UGArray<UGString> &ignoreFieldNames);

		//! \brief 获取不导出字段。
		//! \return 不导出字段名称数组。
		virtual UGArray<UGString> GetIgnoreFieldsBeExported();

		//! \brief 设置要导出的矢量数据集的过滤条件。
		//! \param strRecordFilter [in] 设置的过滤条件。
		virtual void SetRecordFilter(const UGString& strRecordFilter);

		//! \brief 获取要导出的矢量数据集的过滤条件。
		//! \return 过滤条件。
		virtual UGString GetRecordFilter() const;

		//! \brief 设置修改字段信息。
		//! \param fieldInfos [in] 修改字段信息。
		virtual void SetModifyFieldInfos(const UGFieldInfos& fieldInfos);

		//! \brief 获取修改字段信息。
		//! \return 修改字段信息的拷贝。
		virtual UGFieldInfos GetModifyFieldInfos() const;

	private:
		//! \brief 导出数据集信息字典。
		UGDict <UGint, UGFeatureClassDefn> m_ExportDatasetInfos;

		//! \brief 要导出的数据集名称。
		UGString m_strDatasetName;

		//! \brief 不导出字段名称数组。
		UGArray<UGString> m_arrIgnoreExportFieldNames;

		//! \brief 矢量导出的过滤条件。
		UGString m_strRecordFilter;

		//! \brief 待修改字段的信息，根据字段名，可以修改类型，宽度，精度。
		UGFieldInfos m_modifyFieldInfos;
	};

} //namespace UGC
#endif // !defined(AFX_UGVECTORIMPORTPARAMS_H__5D6357D7_FC27_48BC_8BA4_3A6788F5DD2F__INCLUDED_)

