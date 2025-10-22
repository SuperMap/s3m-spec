#if !defined(COMMON_H__379DE008_7F90_4734_8A6F_419C128EFC59__INCLUDED_)
#define COMMON_H__379DE008_7F90_4734_8A6F_419C128EFC59__INCLUDED_

#pragma once
#include "stdafx.h"
#include "S3MBEnums.h"

#include <string>
#include <utility>

namespace S3MB
{
    #ifdef WIN32
    typedef unsigned long long     ulong;
    #else
    typedef unsigned long long int ulong;
    #endif

	//! Multiplier for degrees to radians
	#define DTOR    0.0174532925199432957692369077

	//! Multiplier for radians to degrees
	#define RTOD    57.295779513082320876798154814

	//! \brief 地球半径。
	#define GLOBAL_RADIUS  6378137.0

	//! \brief 地球短轴半径
	#define GLOBAL_RADIUS_MINOR 6356752.3142451793

	#ifndef MATH_PI
	#define MATH_PI      3.1415926535897932384626433833
	#endif

	//! \brief 定义象素格式的位数为24位.
	#define IPF_RGB_BITS  24

	#ifndef S3MB_SHELL_GROUP
	#define S3MB_SHELL_GROUP							U("Group")
	#endif

	#ifndef ATT_VERTEXWEIGHT
	#define ATT_VERTEXWEIGHT							U("VertexWeight")
	#endif

	#ifndef ATT_UVREGION
	#define ATT_UVREGION								U("UVRegion")
	#endif

	#ifndef ATT_POINTCLOUD_INTENSITY
	#define ATT_POINTCLOUD_INTENSITY					U("PointCloudIntensity")
	#endif

	#ifndef ATT_POINTCLOUD_CLASSCODE
	#define ATT_POINTCLOUD_CLASSCODE					U("PointCloudClassCode")
	#endif

	#define CONFIG_ATTRIBUTEEXTENTNAME					U("s3m:AttributeExtentName")

    //! If folding case, make lower case
    #define FOLD(c)          ((flags&FILEMATCH_CASEFOLD)?tolower(c):(c))

	// ceil 数值
	#define CEIL(x) ((int)ceil(x))

	#define SIZE_PER_INSTANCE 17

	#ifndef FILE_EXT_GZ
	#define FILE_EXT_GZ U(".gz")
	#endif

	enum AxisUpType
	{
		X_UP = 0,
		Y_UP = 1,
		Z_UP = 2,
	};

	enum ProcessType
	{
		P_Unknow = 0,
		P_3DTilesToS3MB,
		P_S3MBTo3DTiles,
		P_OSGBToS3MB,
		P_OBJToS3MB,
		P_I3SToS3MB,
	};

	enum STKPixelFormat
	{
		STK_PF_Unknow = 0,
		STK_PF_RGB8 = PixelFormat::PF_RGB8,
		STK_PF_PF_RGBA8 = PixelFormat::PF_RGBA8,
		STK_PF_RGB32F = PixelFormat::PF_RGB32F,
		STK_PF_RGBA32F = PixelFormat::PF_RGBA32F,
	};

	enum TileContentType
	{
		TC_B3DM = 0,
		TC_CMPT,
		TC_I3DM
	};

	//! \brief 顶点颜色跟踪的材质属性
	enum TrackVertexColorType
	{
		TVC_NONE = 0x0,
		TVC_AMBIENT = 0x1,
		TVC_DIFFUSE = 0x2,
		TVC_SPECULAR = 0x4,
		TVC_EMISSIVE = 0x8,
	};

	//! \brief 图片文件类型
	enum ImageFileType
	{
		IF_PNG = 0,
		IF_JPG
	};

    enum
    {
        /// No wildcard can ever match `/'
        FILEMATCH_FILE_NAME = 1,
        /// Backslashes don't quote special chars
        FILEMATCH_NOESCAPE = 2,
        /// Leading `.' is matched only explicitly
        FILEMATCH_PERIOD = 4,
        /// Ignore `/...' after a match
        FILEMATCH_LEADING_DIR = 8,
        /// Compare without regard to case
        FILEMATCH_CASEFOLD = 16
    };

    /// Options for listing files
    enum
    {
        /// Matching files and directories
        LIST_MATCH_ALL = 0,
        /// Don't list any files
        LIST_NO_FILES = 1,
        /// Don't list any directories
        LIST_NO_DIRS = 2,
        /// List all files
        LIST_ALL_FILES = 4,
        /// List all directories
        LIST_ALL_DIRS = 8,
        /// List hidden files also
        LIST_HIDDEN_FILES = 16,
        /// List hidden directories also
        LIST_HIDDEN_DIRS = 32,
        /// Don't include '..' in the listing
        LIST_NO_PARENT = 64,
        /// Matching is case-insensitive
        LIST_CASEFOLD = 128
    };

	//24位真彩
	typedef struct
	{
		unsigned char blue;
		unsigned char green;
		unsigned char red;
	} RGB24;

	//32位真彩
	typedef struct
	{
		unsigned char blue;
		unsigned char green;
		unsigned char red;
		unsigned char alpha;
	} RGBA32;

	typedef struct
	{
		// 属性文件名称
		std::wstring m_strAttributeFileName;
		// id范围
		std::pair<int, int> m_pairIdRange;
	} AttributeIndexInfo;
}

#endif