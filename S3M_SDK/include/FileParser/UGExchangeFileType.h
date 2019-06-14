//////////////////////////////////////////////////////////////////////////
//                    _   _   ____   ____                    
//   Project         | | | | / ___) / ___)                   
//                   | | | || | __ | |                       
//                   | |_| || |_\ || \___                    
//                    \____/ \____| \ ___)     7.2           
//                                                           
//!  \file UGExchangeFileType.h
//!  \brief 常量定义。
//!  \details 包括地图缓存剖分方式、文件格式组、文件格式类型等。
//!  \author 数据转换组。
//!  \attention 
//!   Copyright (c)   SuperMap Software Limited Company.  <br> 
//!   All Rights Reserved.
//!  \version 7.2
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_UGEXCHANGEFILETYPE_H__EAB125D4_D53A_4784_A77E_5A8636BC43BD__INCLUDED_)
#define AFX_UGEXCHANGEFILETYPE_H__EAB125D4_D53A_4784_A77E_5A8636BC43BD__INCLUDED_

#include "Stream/ugdefs.h"
#include "Stream/UGByteArray.h"

namespace UGC {

//! \brief Web服务类型。
#define UG_WEB_SERTYPE _U("WebMapType")

//! \brief 地图缓存剖分方式。
enum  TileType
{
	//! \brief 全球剖分方式。
	Global = 0,
	//! \brief 区域剖分方式，用比例尺分层。
	Local  = 1,
};

//! \brief 数据转换支持的文件类型。
class FILEPARSER_API UGFileType
{
public:

	//! \brief 文件格式分组。
	enum EmGroup
	{
		//! \brief 不确定组
		GroupUncertain = 0,		
		//! \brief 矢量类型的文件格式组
		GroupVector	= 1,		
		//! \brief 用户定义的矢量格式组
		GroupUserVector	= 2,		
		//! \brief 栅格类型的文件格式组
		GroupRaster = 3,	
		//! \brief 用户定义的栅格格式组
		GroupUserRaster = 4 ,
		//! \brief 工作空间文件格式（混合数据集）组
		GroupWorkSpace =  5,
	};

	//! \brief 文件格式类型。
	enum EmType
	{
		//! \brief 未知数据格式。
		none = 0,
		//! \brief AutoCAD DWG 交换文件。
		DWG				= 2,
		//! \brief AutoCAD DXF 交换文件。
		DXF				= 3,
		//! \brief Arc/Info Coverage。
		AIBinCov		= 6,
		//! \brief Arc/Info E00交换文件。
		E00				= 7,
		//! \brief ArcView Shape文件。
		Shape			= 8,
		//! \brief MapInfo TAB。
		TAB				= 11,
		//! \brief MapInfo MIF 交换文件。
		MIF				= 12,
		//! \brief MapInfo 工作空间文件。
		WOR             = 13,
		//! \brief Microstation DGN文件。
		DGN				= 16,
		//! \brief ASPRS Lidar Data Exchange Format (LAS)。
		Lidar			= 17,
		//! \brief 国家标准矢量文件 *.VCT。
		VCT				= 22,
		//! \brief EMF文件(windows metaFile:元增强文件)。
		//! \remarks 暂不支持。
		EMF				= 34,
		//! \brief WMF文件(windows metaFile:元文件)。
		//! \remarks 暂不支持。
		WMF				= 31,
		//! \brief EPS文件。
		//! \remarks 暂不支持。
		EPS				= 32,
		//! \brief PDF文件。
		//! \remarks 暂不支持。
		PDF				= 33,
		//! \brief 电子海图格式。
		ENC				= 40,
		//! \brief 电信Vector格式。
		TEMSVector		= 41,
		//! \brief 电信Building vector格式。
		TEMSBldngVctr	= 42,
		//! \brief 电信TEXT格式。
		TEMSTEXT        = 43,
		//! \brief OpenGIS 定义的XML文档。
		GML				= 51,
		//! \brief SuperMap定义的XML文档。
		//! \remarks 暂不支持。
		SML				= 52,
		//! \brief google的KML文件格式。
		KML				= 53,
		//! \brief google的KML格式的压缩文件。
		KMZ				= 54,
		//! \brief MapGis明码文件wat、wal、wap。
		MAPGIS			= 55,
		//! \brief dbf 数据库文件。
		DBF				= 61,
		//! \brief 三维模型缓存文件。
		SCV				= 63,
		//! \brief 文本文件。
		CSV				= 64,
		//! \brief Fme工作空间的导入模式。
		//! \remarks 暂不支持。
		FMEWorkBench    = 65,
		//! \brief 矢量数据使用fme进行导入。
		//! \remarks 暂不支持。
		FMEVector       = 66,
		//! \brief 栅格数据使用fme进行导入。
		//! \remarks 暂不支持。
		FMERaster       = 67,
		//! \brief SDE的矢量格式。
		SDEVector		= 68,
		//! \brief SDE的栅格格式。
		SDERaster		= 69,
		//! \brief Esri file geodatabase vector .gdb文件。
		GDBFVector      = 70,
		//! \brief Esri file geodatabase raster .gdb文件。
		GDBFRaster      = 71,
		//! \brief Esri Personal geodatabase vector .mdb文件。
		GDBPVector		= 72,
		//! \brief 军用数字地图矢量数据。
		GJB5068			= 73,
		//! \brief GeoJSON地理空间信息数据交换格式。
		GeoJSON			= 74,
		//! \brief OpenStreetMap开源wiki地图数据。
		OSM				= 75,
		//! \brief Json格式
		SimpleJson      = 76,
		//! \brief hdfs文件系统
		HDFS            = 77,
		//! \brief Erdas Image文件。
		IMG				= 101,
		//! \brief MrSid文件。
		SID				= 102,
		//! \brief Tiff/BigTIFF/GeoTIFF文件。
		GTiff			= 103,
		//! \brief VRT文件。
		VRT				= 104,
		//! \brief Er-Mapper ECW文件。
		ECW				= 106,
		//! \brief Idrisi 栅格文件。
		//! \remarks 暂不支持。
		IDR				= 107,
		//! \brief Landsat－7 FST文件，后缀名(*.fst)。
		//! \remarks 暂不支持。
		FST				= 108,
		//! \brief Grib1和Grib2文件。
		GRIB			= 120,
		//! \brief bitmap文件。
		BMP				= 121,
		//! \brief jpg文件。
		JPG				= 122,	
		//! \brief portal network graphic(PNG)。
		PNG				= 123,
		//! \brief gif文件。
		GIF				= 124,
		//! \brief tga文件。
		//! \remarks 暂不支持。
		TGA				= 125,
		//! \brief DXT + ZIP格式。
		//! \remarks 暂不支持。
		DXTZ           = 126,
		//! \brief PVR + ZIP格式。
		//! \remarks 暂不支持。
		PVRZ           = 127,
		//! \brief ETC+ ZIP格式。
		//! \remarks 暂不支持。
		KTXZ           = 128,
		//! \brief BIL格式的grd文件。
		BIL				= 141,
		//! \brief Arc/Info ASCII 交换文件。
		AIASCIIGrid		= 142,
		//! \brief 国标格网数据文件。
		DEM				= 143,
		//! \brief USGS格网数据文件 美国标准。
		USGSGrid		= 144,
		//! \brief Arc/Info binary grid文件。
		AIBinGrid		= 145,
		//! \brief 电信Raster格式。
		TEMSClutter		= 146,
		//! \brief JPG和PNG图片混合格式。
		//! \remarks 暂不支持。
		JPGPNG          = 147,
		//! \brief 3DSMAX CS特有格式。
		BIP              =148,
		//! \brief 遥感图像存储格式。
		BSQ              =149,
		//! \brief JPEG2000。
		JP2              = 150,
		//! \brief raw文件。
		RAW				= 161,
		//! \brief SuperMap压缩影像 SuperMap Image Tower。
		SIT				= 204,
		//! \breif  军用栅格格式
		EGC            = 205,
		//! \breif  GeoPackage
		GeoPackage     = 206,
	    //! \breif Orange Tab
		ORANGETAB      = 207,
		//! \breif google protocol buffers数据序列化格式
		PBF				= 208,
		//! \breif NetCDF	格式
		NetCDF	        = 209,

		//! \brief 用户自定义矢量开始。
		//! \remarks 暂不支持。
		UserVectorBegin	= 401,
		//! \brief 用户自定义矢量结束。
		//! \remarks 暂不支持。
		UserVectorEnd	= 450,
		//! \brief 用户自定义栅格开始。
		//! \remarks 暂不支持。
		UserRasterBegin	= 451,
		//! \brief 用户自定义栅格结束。
		//! \remarks 暂不支持。
		UserRasterEnd	= 500,

		//! \brief 3DS模型文件。
		Model3DS        = 501,
		//! \brief sgm模型文件。
		//! \remarks 暂不支持。
		SGM             = 502,
		//! \brief x模型文件。
		ModelX          = 503,
		//! \brief dae模型文件。
		//! \remarks 暂不支持。
		ModelDAE        = 504,
		//! \brief osg模型文件。
		ModelOSG        = 505,
		//! \brief SuperMapTileEncodeType。
		//! \remarks 暂不支持。
		STET            = 506,
		//! \brief DXF模型。
		ModelDXF         = 507,
		//! \brief Fbx模型。
		ModelFBX         = 508,
		//! \brief OpenFlight模型。
		ModelOpenFlight  = 509,
		//! \brief Ogre骨骼动画模型。
		//! \remarks 暂不支持。
		OgreSkeleton     = 510,
		//! \brief 点云。
		ModelPointCloud     = 510,
		//! \brief SuperMap 3D Model 
		ModelS3M     = 511,
		//! \brief GLTF(gl Transform Format) 3D Model 
		ModelGLTF     = 512,
		//! \brief DD2015-06 三维地质模型数据交换格式
		Geo3DML     = 513,
		//! \brief sketchup数据格式
		ModelSKP			= 514,
		//! \brief s3mb标准格式
		S3MB			= 515,
		//! \brief ZIP压缩的s3mb标准格式
		S3MBZ			= 516,
		//! \brief WEBP数据格式
		WEBP			= 517,
	};


	//! \brief 根据文件格式常量判断这种格式属于哪一个组。
	//! \param eFileType [in] 文件格式类型。
	//! \return 文件格式分组。
	static EmGroup GetFileGroup(EmType eFileType);

	//! \brief 根据后缀名得到文件类型。
	//! \param strExtName [in] 带.的后缀名。
	//! \return 文件格式类型。
	static EmType ExtNameToFileType(const UGString& strExtName);

	//! \brief 根据文件类型得到后缀名。
	//! \param eFileType [in] 文件格式类型。
	//! \return 带.的后缀名。
	static UGString FileTypeToExtName(EmType eFileType);

	//! \brief 根据文件类型得到文件详述。
	//! \param etype [in] 文件格式类型。
	//! \return 文件详述。
	static UGString DetailByFileType(EmType etype);

	//! \brief 根据文件类型判断是否是矢量类型。
	//! \param eFileType [in] 文件格式类型。
	//! \return true是矢量，false不是矢量。
	static UGbool IsVector(EmType eFileType);

	//! \brief 根据文件类型判断是否是模型文件。
	//! \param eFileType [in] 文件格式类型。
	//! \return true是模型文件，false不是模型文件。
	static UGbool IsModelFile(EmType eFileType);

	//! \brief 根据文件类型判断是否可以导入成CAD类型。
	//! \param eFileType [in] 文件格式类型。
	//! \return true可以导入成CAD类型，false不可以导入成CAD类型。
	//! \attention 目前只FME中使用。
	static UGbool IsCanBeCAD(EmType eFileType);

	//! \brief 根据文件类型判断导入的数据集名称是否要加后缀。
	//! \param eFileType [in] 文件格式类型。
	//! \return true要加后缀，false不加后缀。
	//! \attention 目前只FME中使用。
	static UGbool IsNeedAddEx(EmType eFileType);

	//! \brief 根据文件类型判断导入的类型是否是Engine类型，如SDE。
	//! \param eFileType [in] 文件格式类型。
	//! \return true是Engine类型，false不是Engine类型。
	//! \attention 目前只FME中使用。
	static UGbool IsNoEngineType(EmType eFileType);
};

//! \brief 回调函数参数。
//! \remarks 单张瓦片的信息。
struct UGDownloadTileInfo
{
	//! \brief 比例尺
	UGdouble m_dScale;

	//! \brief  地理范围
	UGRect2D m_rcBounds;

	//! \brief 行号
	UGint m_nRow;

	//! \brief 列号
	UGint m_nCol;

	//! \brief 下载成功时为0，非0为有错误
	UGint m_eErrorType;

	//! \brief 重载 ==。
	//! \param TileInfo [in] 回调函数参数。
	//! \return 若两个请求相等则返回true。
	UGbool operator==(const UGDownloadTileInfo& TileInfo) const
	{
		if (m_dScale == TileInfo.m_dScale && m_nRow == TileInfo.m_nRow && m_nCol == TileInfo.m_nCol)
		{
			return TRUE;
		}

		return FALSE;
	}

	//! \brief 重载=。
	//! \param &TileInfo [in] 回调函数参数。
	void operator = (const UGDownloadTileInfo &TileInfo)
	{
		m_eErrorType = TileInfo.m_eErrorType;
		m_rcBounds = TileInfo.m_rcBounds;
		m_nRow = TileInfo.m_nRow;
		m_nCol = TileInfo.m_nCol;
		m_dScale = TileInfo.m_dScale;
	}
};

//! \brief 瓦片信息参数。
class UGTile
{
public:
	//! \brief 构造函数。
	//! \remarks 构造一个新的瓦片信息参数对象。
	UGTile()
	{
		m_nRow = 0;
		m_nCol = 0;
		m_nFileType = 123;
		m_nTileSize = 256;
		m_dScale = 0.0;
        m_sFilePath = _U("#");
	}

	//! \brief 析构函数。
	//! \remarks 释放瓦片信息参数。
	~UGTile()
	{
	}

	//! \brief 设置比例尺。
	//! \param dScale [in] 比例尺。
	void SetScale(UGdouble dScale)
	{
		m_dScale = dScale;
	}

	//! \brief 获取比例尺。
	//! \return 比例尺。
	UGdouble GetScale() const
	{
		return m_dScale;
	}
    
    void SetFilePath(UGString filePath)
    {
        m_sFilePath = filePath;
    }
    
    UGString& GetFilePath()
    {
        return m_sFilePath;
    }
	//! \brief 设置瓦片行号。
	//! \param nRow [in] 行号。
	void SetTileRow(UGint nRow)
	{
		m_nRow = nRow;
	}

	//! \brief 获取瓦片行号。
	//! \return 行号。
	UGint GetTileRow()
	{
		return m_nRow;
	}

	//! \brief 设置瓦片列号。
	//! \param nCol [in] 列号。
	void SetTileCol(UGint nCol)
	{
		m_nCol = nCol;
	}

	//! \brief 获取瓦片列号。
	//! \return 列号。
	UGint GetTileCol()
	{
		return m_nCol;
	}

	//! \brief 设置瓦片图像格式。
	//! \param nFileType [in] 图像格式。
	void SetFileType(UGint nFileType)
	{
		m_nFileType = nFileType;
	}

	//! \brief 获取瓦片图像格式。
	//! \return 图像格式。
	UGint GetFileType()
	{
		return m_nFileType; 
	}

	//! \brief 设置瓦片二进制文件流。
	//! \param &ByteArray [in] 二进制文件流。
	void SetByteArray(const UGByteArray &ByteArray)
	{
		m_ByteArray = ByteArray;
	}

	//! \brief 获取瓦片二进制文件流。
	//! \return 二进制文件流。
	UGByteArray &GetByteArray()
	{
		return m_ByteArray;
	}

	//! \brief 设置瓦片位置。
	//! \param nRow [in] 瓦片行号。
	//! \param nCol [in] 瓦片列号。
	void SetTilePos(int nRow, int nCol) 
	{
		m_nRow = nRow;
		m_nCol = nCol;
	}

	//! \brief 获取瓦片位置。
	//! \param &nRow [out] 瓦片行号。
	//! \param &nCol [out] 瓦片列号。
	void GetTilePos(int &nRow, int &nCol) const 
	{
		nRow = m_nRow;
		nCol = m_nCol;
	}

	//! \brief 设置瓦片的地理范围。
	//! \param &rcBounds [in] 地理范围。
	void SetMPBounds(const UGRect2D &rcBounds) 
	{
		m_rcBounds = rcBounds;
	}

	//! \brief 获取瓦片的地理范围。
	//! \return 地理范围。
	const UGRect2D &GetMPBounds() const 
	{
		return m_rcBounds;
	}

	//! \brief 设置瓦片的大小。
	//! \param nTileSize [in] 瓦片的大小。
	void SetTileSize(UGint nTileSize) 
	{
		m_nTileSize = nTileSize;
	}

	//! \brief 获取瓦片的大小。
	//! \return 瓦片的大小。
	UGint GetTileSize() const 
	{
		return m_nTileSize;
	}

private:
	//! \brief 比例尺。
	UGdouble m_dScale;

	//! \brief Tile位置行号。
	int m_nRow;

	//! \brief Tile位置列号。
	int m_nCol;

	//! \brief  地理范围
	UGRect2D m_rcBounds;

	//! \brief  Tile的图像格式
	UGint m_nFileType;

	//! \brief  Tile的文件流
	UGByteArray m_ByteArray;

	//! \brief  Tile的像素宽高
	UGint m_nTileSize;
    
    //文件位置
    UGString m_sFilePath;

};


//! \brief 数据变化回调函数参数。
//! \remarks 该结构在多线程中使用。
struct UGDataChangeInfo
{
	//! \brief 变化类型。
	enum ChangeType
	{
		//! \brief 网络下载引起的变化。
		DownLoad = 0,
		//! \brief 缩放引起的变化。
		Zoom = 1,
		//! \brief 移动引起的变化。
		Pan = 2
	};

	//! \brief 引起数据变化的类型
	ChangeType m_eChangeType;
	//! \brief 地图比例尺
	UGdouble m_dMapScale;
	//! \brief 地图对应瓦片的最近比例尺
	UGdouble m_dTileScale;
	//! \brief 地图MP->LP的转换比率
	UGdouble m_dCoordRatio;
	//! \brief  数据变化区域(地理范围)
	UGRect2D m_rcChangeBounds;
	//! \brief  数据不变的区域(地理范围)
	UGRect2D m_rcClipBounds;
	//! \brief 网络下载成功时为0，非0为有错误
	UGint m_eErrorType;
	//! \brief 数据变化区域(设备范围)
	UGint m_nChangeWidth;
	//! \brief 数据变化区域(设备范围)
	UGint m_nChangeHeight;
	//! \brief 行号(便于调试)
	UGint m_nRow;
	//! \brief 行号(便于调试)
	UGint m_nCol;

	//! \brief 构造函数。
	//! \remarks 构造一个新的数据变化回调函数参数对象。
	UGDataChangeInfo()
	{
		m_eChangeType = DownLoad;
		m_dMapScale = 0.0;
		m_dTileScale = 0.0;
		m_dCoordRatio = 0.0;
		m_rcChangeBounds.SetEmpty();
		m_rcClipBounds.SetEmpty();
		m_eErrorType = 0;
		m_nChangeWidth = 0;
		m_nChangeHeight = 0;
		m_nRow = 0;
		m_nCol = 0;
	}

	//! \brief 重载 ==。
	//! \param ChangeInfo [in] 数据变化回调函数参数对象。
	//! \return true相等，false不相等。
	UGbool operator==(const UGDataChangeInfo& ChangeInfo) const
	{
		if (m_dMapScale == ChangeInfo.m_dMapScale && m_rcChangeBounds == ChangeInfo.m_rcChangeBounds)
		{
			return TRUE;
		}

		return FALSE;
	}

	//! \brief 重载=。
	//! \param ChangeInfo [in] 数据变化回调函数参数对象。
	void operator = (const UGDataChangeInfo& ChangeInfo)
	{
		m_eChangeType = ChangeInfo.m_eChangeType;
		m_dMapScale = ChangeInfo.m_dMapScale;
		m_dTileScale = ChangeInfo.m_dTileScale;
		m_dCoordRatio = ChangeInfo.m_dCoordRatio;
		m_rcChangeBounds = ChangeInfo.m_rcChangeBounds;
		m_rcClipBounds = ChangeInfo.m_rcClipBounds;
		m_eErrorType = ChangeInfo.m_eErrorType;
		m_nChangeWidth = ChangeInfo.m_nChangeWidth;
		m_nChangeHeight = ChangeInfo.m_nChangeHeight;
		m_nRow = ChangeInfo.m_nRow;
		m_nCol = ChangeInfo.m_nCol;
	}

};

//! \brief 瓦片信息参数数组。
typedef UGArray<UGTile*> UGTiles;


} //namespace UGC
#endif // !defined(AFX_UGEXCHANGEFILETYPE_H__EAB125D4_D53A_4784_A77E_5A8636BC43BD__INCLUDED_)


