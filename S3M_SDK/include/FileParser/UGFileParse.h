//////////////////////////////////////////////////////////////////////////
//                    _   _   ____   ____                    
//   Project         | | | | / ___) / ___)                   
//                   | | | || | __ | |                       
//                   | |_| || |_\ || \___                    
//                    \____/ \____| \ ___)     7.2           
//                                                           
//!  \file UGFileParse.h
//!  \brief 相关文件的基类定义。
//!  \details 主要包括UGFileLayer文件层基类和UGFileParser文件解析基类。
//!  \author 数据转换组。
//!  \attention 
//!   Copyright (c)   SuperMap Software Limited Company.  <br> 
//!   All Rights Reserved.
//!  \version 7.2
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_UGFILEPARSE_H__F2ACB033_F582_4263_B9E9_DCA2E56F87DE__INCLUDED_)
#define AFX_UGFILEPARSE_H__F2ACB033_F582_4263_B9E9_DCA2E56F87DE__INCLUDED_

#include "FileParser/UGFileParserConfigParams.h"
#include "FileParser/UGExchangeFileType.h"

namespace UGC 
{
	//! \brief 文件层基类。
	//! \remarks 所有有关文件的层都要继承此类。
	class FILEPARSER_API UGFileLayer
	{
	public:
		//! \brief 文件层的类型。
		enum FILELAYER_TYPE
		{
			//! \brief 矢量Layer。
			FL_VECTOR,
			//! \brief 栅格Layer。
			FL_RASTER,
			//! \brief 配置文件Layer。
			FL_CONFIG,
			//! \brief 其他Layer。
			FL_OTHER,
		};
	public:
		//! \brief 获取文件层的类型。
		//! \return 文件层的类型。
		virtual FILELAYER_TYPE GetType() = 0;
	};


	//! \brief 文件解析基类。
	//! \remarks 所有有关文件分析的都要继承此类。
	class FILEPARSER_API UGFileParser  
	{
	public:
		//! \brief 构造函数。
		//! \remarks 构造一个新的文件解析对象。
		UGFileParser();

		//! \brief 析构函数。
		//! \remarks 释放文件解析对象。
		virtual ~UGFileParser();

	public:
		//! \brief 打开文件。
		//! \param impParams [in] 导入设置参数。
		//! \return true文件打开成功，false文件打开失败。
		//! \remarks 读取文件前调用。
		virtual UGbool Open(const UGImportParams& impParams) = 0;
	
		//! \brief 关闭文件	
		virtual void Close() = 0;

		//! \brief 根据文件格式类型判断属于哪一个组。
		//! \return 文件格式分组。
		virtual UGFileType::EmGroup GetFileGroup() const = 0;

		//! \brief 按FMEWorkBench的方式导入。
		//! \param impParams [in] 导入设置参数。
		//! \param resultFilePath [in] 结果文件路径。
		//! \return 0成功，-1失败。
		//! \attention 此接口有且仅供FMEWorkBench的导入方式使用，请勿随便使用。
		virtual UGint ImportByFMEWorkBench(const UGImportParams& impParams, UGString& resultFilePath){return -1;}

	public:	
		//! \brief 获取源数据中数据集个数。
		//! \return 数据集个数。
		virtual UGint GetLayerCount();

		//! \brief 获取源数据中子数据集个数。
		//! \return 子数据集个数。
		virtual UGint GetSubLayerCount();

		//! \brief 通过索引得到源数据中的一个数据集。
		//! \param nlayer [in] 数据集索引。
		//! \return 成功返回数据集，失败返回NULL。
		virtual UGFileLayer* GetLayer(UGint nlayer);

		//! \brief 通过索引得到源数据中的一个子数据集。
		//! \param nSublayer [in] 子数据集索引。
		//! \return 成功返回子数据集，失败返回NULL。
		virtual UGFileLayer* GetSubLayer(UGint nSublayer);

	public:
		//! \brief 文件是否打开。
		//! \return true打开，false未打开。
		UGbool IsOpen() const;

		//! \brief 是否是矢量类型。
		//! \return true是矢量，false不是矢量。
		UGbool IsVector() const;

		//! \brief 是否是栅格类型。
		//! \return true是栅格，false不是栅格。
		UGbool IsRaster() const; 

		//! \brief 是否是工作空间类型。
		//! \return true是工作空间，false不是工作空间。
		UGbool IsWorkspace() const; 	

		//! \brief 获取文件格式类型。
		//! \return 文件格式类型。
		UGFileType::EmType GetFileParseType(){return m_nFileType;}

	protected:
		//! \brief 文件格式类型。
		UGFileType::EmType m_nFileType;
		//! \brief 文件打开标志。
		UGbool m_bOpen;
	};

} //namespace UGC
#endif // !defined(AFX_UGFILEPARSE_H__F2ACB033_F582_4263_B9E9_DCA2E56F87DE__INCLUDED_)

