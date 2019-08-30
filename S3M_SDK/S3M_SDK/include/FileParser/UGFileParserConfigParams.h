//////////////////////////////////////////////////////////////////////////
//                    _   _   ____   ____                    
//   Project         | | | | / ___) / ___)                   
//                   | | | || | __ | |                       
//                   | |_| || |_\ || \___                    
//                    \____/ \____| \ ___)     7.2           
//                                                           
//!  \file UGFileParserConfigParams.h
//!  \brief 数据转换参数、导入参数、导出参数定义文件。
//!  \details 主要包括数据转换参数基类、导入参数类和导出参数类。
//!  \author 数据转换组。
//!  \attention 
//!   Copyright (c)   SuperMap Software Limited Company.  <br> 
//!   All Rights Reserved.
//!  \version 7.2
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_UGFILEPARSERCONFIGPARAMS_H__08BADC84_B1E9_40CA_B1AE_A7755CC63F16__INCLUDED_)
#define AFX_UGFILEPARSERCONFIGPARAMS_H__08BADC84_B1E9_40CA_B1AE_A7755CC63F16__INCLUDED_

#include "Projection/UGPrjCoordSys.h"
#include "Element/OgdcFieldInfo.h"
#include "Geometry/UGDataCodec.h"
#include "Geometry/UGFeature.h"
#include "Stream/UGByteArray.h"
#include "Base3D/UGVector3d.h"

namespace UGC {

	//! \brief 数据转换参数基类。
	//! \remarks 备注信息。
	class FILEPARSER_API UGExchangeParams  
	{
		friend class UGExchangeParamsManager;
		friend class UGFileParseToolkit;

	public:
		//! \brief 构造函数。
		//! \remarks 构造一个新的转换参数基类对象。
		UGExchangeParams();
	
		//! \brief 析构函数。
		//! \remarks 释放转换参数基类对象。
		virtual ~UGExchangeParams();

	public:
		//! \brief 设置数据文件名。
		//! \param strFilePathName [in] 文件全路径名。
		void SetFilePathName(const UGString& strFilePathName);

		//! \brief 获取数据文件名。
		//! \return 文件全路径。
		UGString GetFilePathName() const;

		//! \brief 设置是否使用的是FME导入模式。
		//! \param bIsUserFME [in] 是否使用的是FME导入模式。
		void SetIsUseFME(UGbool bIsUserFME = FALSE);

		//! \brief 获取是否使用的是FME导入模式。
		//! \return 是否使用的是FME导入模式。
		UGbool GetIsUseFME() const;

		//! \brief 获取文件类型。
		//! \return 文件类型。
		//! \remarks 各个子类自己实现。
		virtual inline UGint GetFileType() const {return 0;}

		//! \brief 设置是否显示进度。
		//! \param bProgress [in] 是否显示进度。
		void SetShowProgress(UGbool bProgress = TRUE);

		//! \brief 获取是否显示进度。
		//! \return 是否显示进度。
		UGbool GetShowProgress() const;

		//! \brief 设置是否强制覆盖。
		//! \param bOverwrite [in] 是否强制覆盖。
		void SetOverWrite(UGbool bOverwrite= TRUE);

		//! \brief 获取是否强制覆盖。
		//! \return 是否强制覆盖。
		UGbool GetOverWrite() const;

		//! \brief 判断当前使用的转换参数是否是导入参数。
		//! \return 是否是导入参数。
		//! \remarks 主要用于转换参数合法性检查，与GetFileType结合使用，可以判断出当前使用的参数是否是指定文件类型的导入参数。
		virtual inline UGbool IsImportParams() const {return FALSE;};

		//! \brief 判断当前使用的转换参数是否是导出参数。
		//! \return 是否是导出参数。
		//! \remarks 主要用于转换参数合法性检查，与GetFileType结合使用，可以判断出当前使用的参数是否是指定文件类型的导出参数。
		virtual inline UGbool IsExportParams()  const {return FALSE;};

		//! \brief 设置导入导出编码。
		//! \param Charset [in] 导入导出编码。
		//! \return true成功，false失败。
		//! \remarks 某些文件类型是不允许修改的，依据文件类型而定。
		virtual UGbool SetImportOrExportCharset(UGString::Charset Charset);

		//! \brief 获取导入导出编码。
		//! \return 导入导出编码。
		//! \remarks 某些文件类型是不允许修改的，依据文件类型而定。
		UGString::Charset GetImportOrExportCharset() const;

	protected:
		//! \brief 数据转换选项。
		enum ExchangeOptions
		{	
			//! \brief 显示进度。
			ShowProgress			= 0x00000001,
			//! \brief 忽略属性。
			//! \remarks 矢量数据属性信息，栅格数据的属性表。
			IgnoreAttribute			= 0x00000002,
			//! \brief 忽略坐标系统。
			IgnoreCoordSys			= 0x00000004,
			//! \brief 忽略空数据。
			//! \remarks 没有空间数据的要素，如mif格式。
			IgnoreEmptySpatialData = 0x00000008,
			//! \brief 建空间索引。
			BuildSpatialIndex		= 0x00000010,
			//! \brief 忽略CAD数据中的“块”，DGN数据中的“单元”。
			IgnoreSymbol			= 0x00000020,
			//! \brief 忽略风格。
			IgnoreStyle				= 0x00000040,
			//! \brief 导入为多波段。
			ImportAsMultiBands		= 0x00000080,
			//! \brief 自动建立影像金字塔。
			AutoBuildPyramid		= 0x00000100,
			//!	\brief 是否从外部文件获取投影。
			SpatialRefFromFile		= 0x00000200,
			//! \brief 忽略拓扑。
			IgnoreTopo				= 0x00000400,
			//! \brief 是否按照单波段读取数据。
			ReadAsOneBand			= 0x00000800,
			//! \brief 是否开启强制位深转换功能。
			ForcePixelEnable		= 0x00001000,
		};

		//! \brief 记录文件路径有没有变更。
		//! \remarks 这个参数为数据导入的时候，读取文件参数用的,通过这个参数，在文件获取编码的时候就不用重复打开文件。
		UGbool m_bFileChange;

		//! \brief 文件路径。
		UGString m_strFilePathName;

		//! \brief 转换中的选项。
		//! \remarks 只能是ExchangeOptions中的选项 。
		UGuint m_nOption;

		//! \brief 强制覆盖数据标志。
		UGbool m_bOverWrite;

		//! \brief 导入导出字符集。
		//! \remarks 如果导入文件，默认和m_FileCharset保持一致，外部用户可以设置。<br>
		//!	如果导出文件，编码可能是当前系统编码或者文件的编码，依据当前文件的格式，某些文件类型，可能无法修改。
		UGString::Charset m_ImportOrExportCharset;

		//! \brief 是否使用FME的方式导入。
		UGbool m_bIsUseFME;
	};


	//! \brief 导入参数类。
	//! \remarks 继承自UGExchangeParams数据转换参数基类。
	class FILEPARSER_API UGImportParams : public UGExchangeParams
	{
		friend class UGExchangeParamsManager;

	public:
		//! \brief 构造函数。
		//! \remarks 构造一个新的导入参数对象。
		UGImportParams();
	
		//! \brief 析构函数。
		//! \remarks 释放导入参数信息。
		virtual ~UGImportParams();

		//! \brief 重载=。
		//! \param imp [in] 导入参数对象。
		//! \return 导入参数对象。
		UGImportParams& operator=(const UGImportParams& imp);

	public:
		//! \brief 导入模式。
		enum ImportMode
		{
			//! \brief 导入为简单数据集。
			ModeGIS		= 0,
			//! \brief 导入为复合数据集。
			ModeCAD		= 1,
			//! \brief 导入为IMG数据集。
			ModeIMG		= 2,
			//! \brief 导入为Grid数据集。
			ModeGrid	= 3,
			//! \brief 导入为Dem数据集。
			ModeDEM		= 4,
			//! \brief 导入只读OSG。
			ModeOnlyReadOSG = 5,
			//! \brief 导入大文件型OSG。
			ModeOSGToCompactFile = 6,
			//! \brief 导入只读点云。
			ModelReadPointCloud = 8,
			//! \brief 导入大文件型S3M。
			ModeS3MToCompactFile = 9
		};

		//! \brief 设置导入模式。
		//! \param nimpMode [in] 导入模式。
		void SetImportMode(UGint nimpMode);

		//! \brief 获取导入模式。
		//! \return 导入模式。
		UGint GetImportMode() const;

		//! \brief 获取是否采用用户设置参数导入。
		//! \return 是否采用用户设置参数导入。
		UGbool GetIsUserSetting(){return m_bUserSetting;}

		//! \brief 设置是否追加导入。
		//! \param append [in] 是否追加导入。
		//! \attention 设置了这个参数后将忽略是否覆盖数据集的参数设置。
		void SetAppend(UGbool append = TRUE);

		//! \brief 获取是否追加导入。
		//! \return 是否追加导入。
		UGbool GetAppend() const;

		//! \brief 设置是否忽略属性。
		//! \param bIgonreAttribue [in] 是否忽略属性。
		//! \remarks 包括矢量属性信息，栅格数据属性表。
		void SetIgnoreAttribute(UGbool bIgonreAttribue = FALSE);

		//! \brief 获取是否忽略属性。
		//! \return 是否忽略属性。
		UGbool GetIgnoreAttribute() const;

		//! \brief 设置是否忽略坐标系统。
		//! \param bIgnoreCoordSys [in] 是否忽略坐标系统。
		void SetIgnoreCoordSys(UGbool bIgnoreCoordSys = FALSE);

		//! \brief 获取是否忽略坐标系统。
		//! \return 是否忽略坐标系统。
		UGbool GetIgnoreCoordSys() const;

		//! \brief 导入后，设置目标数据集的坐标系统。
		//! \param spatialRef [in] 目标数据集的坐标系统。
		//! \remarks 我们认为参数中传入的投影是数据的真实投影，不负责检查投影是否一致，不做投影转换，仅设置投影而已。
		void SetDestCoordSys(const UGPrjCoordSys& spatialRef);

		//! \brief 获取坐标系统。
		//! \return 坐标系统。
		UGPrjCoordSys* GetDestCoordSys() const;

		//! \brief 设置数据编码方式。
		//! \param encType [in] 编码方式。
		void SetDatasetEncType(UGDataCodec::CodecType encType);

		//! \brief 获取数据编码方式。
		//! \return 数据编码方式。
		UGDataCodec::CodecType GetDatasetEncType() const;

		//! \brief 设置目标数据集名称。
		//! \param strDTName [in] 目标数据集名称。
		void SetDestDTName(const UGString& strDTName);

		//! \brief 获取目标数据集名称。
		//! \return 目标数据集名称。
		UGString GetDestDTName() const;

		//! \brief 获取目标数据集名称,本方法只返回用户设置的数据集名称，没有设置返回空
		UGString GetDTName() const { return m_strDTName; }

		//! \brief 获取是否以多层导入。
		//! \return 是否以多层导入。
		virtual UGbool GetImportByLayer() const{return FALSE;}

		//! \brief 获取导入文件编码。
		//! \return 导入文件编码。
		//! \remarks 如果文件重新设置，这个要重新获取，同时GetImportOrExportCharset有可能变更。<br>
		//!	如果格式没有规定使用特定的编码，那么就采用用户设置的编码。
		virtual UGString::Charset GetFileCharset();

		//! \brief 设置大文件数据。
		//! \param pByteArray [in] 大文件数据。
		void SetByteArray(UGByteArray* pByteArray) { m_pByteArray = pByteArray; }

		//! \brief 获取大文件数据。
		//! \return 大文件数据。
		UGByteArray* GetByteArray() const { return m_pByteArray; }

		//! \brief 设置是否导入空数据集。
		//! \param bImportEmptyDT [in] 是否导入空数据集。
		void SetImportEmptyDT(UGbool bImportEmptyDT){m_bImportEmptyDT = bImportEmptyDT; }

		//! \brief 获取是否导入空数据集。
		//! \return 是否导入空数据集。
		UGbool GetImportEmptyDT() const {return m_bImportEmptyDT; }

		//! \brief 设置导入的比例因子。
		//! \return 是否成功。
		UGbool SetScalingFactor(UGdouble dx,UGdouble dy,UGdouble dz);
		//! \brief 获取比例因子。
		UGVector3d GetScalingFactor(){return m_ScalingFactor;};
		
	private:
		//! \brief 判断当前使用的转换参数是否是导入参数。
		//! \remarks 主要用于转换参数合法性检查，与GetFileType结合使用，可以判断出当前使用
		//!	的参数是否是指定文件类型的导入参数。
		virtual inline UGbool IsImportParams() const {return TRUE;};
	
		//! \brief 判断当前使用的转换参数是否是导出参数。
		//! \remarks 主要用于转换参数合法性检查，与GetFileType结合使用，可以判断出当前使用
		//!	的参数是否是指定文件类型的导出参数。
		virtual inline UGbool IsExportParams()  const {return FALSE;};

	protected:
		//! \brief 导入模式。
		ImportMode m_ImportMode;

		//! \brief 是否为追加导入
		UGbool m_bAppend;

		//! \brief 结果数据集编码方式
		UGDataCodec::CodecType m_encType;

		//! \brief 目标坐标系统。
		UGPrjCoordSys* m_pSpatialRef;

		//! \brief 是否采用用户设置。
		//! \remarks 默认为False，此时全部按默认导入，当用户调用SetImportDatasetInfos时，采用用户设置
		UGbool m_bUserSetting;

		//! \brief 目标数据集名称
		UGString m_strDTName;

		//! \brief 针对导入文件的编码，这个方法是自动检测文件编码之后，存储文件的编码。
		UGString::Charset m_FileCharset;

		//! \brief OSG数据
		UGByteArray* m_pByteArray;

		//! \brief 是否导入空表
		UGbool m_bImportEmptyDT;

		//! \brief 比例因子，用于缩放坐标。例如用户数据是毫米坐标，需要* 0.001得到米坐标
		UGVector3d m_ScalingFactor;
	};


	//! \brief 导出参数类。
	//! \remarks 继承自UGExchangeParams数据转换参数基类。
	class FILEPARSER_API UGExportParams : public UGExchangeParams
	{
		friend class UGExchangeParamsManager;

	public:
		//! \brief 构造函数。
		//! \remarks 构造一个新的导出参数对象。
		UGExportParams();

		//! \brief 析构函数。
		//! \remarks 释放导出参数信息。
		virtual ~UGExportParams();

		//! \brief 重载=。
		//! \param exp [in] 导出参数对象。
		//! \return 导出参数对象。
		UGExportParams& operator=(const UGExportParams& exp);

	private:	
		//! \brief 判断当前使用的转换参数是否是导入参数。
		//! \remarks 主要用于转换参数合法性检查，与GetFileType结合使用，可以判断出当前使用
		//!	的参数是否是指定文件类型的导入参数。
		virtual inline UGbool IsImportParams() const {return FALSE;}
	
		//! \brief 判断当前使用的转换参数是否是导出参数。
		//! \remarks 主要用于转换参数合法性检查，与GetFileType结合使用，可以判断出当前使用
		//!	的参数是否是指定文件类型的导出参数。
		virtual inline UGbool IsExportParams()  const {return TRUE;}
	};

} //namespace UGC
#endif // !defined(AFX_UGFILEPARSERCONFIGPARAMS_H__08BADC84_B1E9_40CA_B1AE_A7755CC63F16__INCLUDED_)

