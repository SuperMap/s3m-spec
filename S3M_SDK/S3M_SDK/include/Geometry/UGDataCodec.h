/*! \file	 UGDataCodec.h
 *  \brief	 空间数据编码类的定义
 *  \author	 ugc team
 *  \attention 
 *  Copyright(c) 2000-2010 SuperMap Software Co., Ltd.<br>
 *  All Rights Reserved
 *  \version $Id: UGDataCodec.h,v 1.12 2009/11/05 09:29:28 siyh Exp $
 */


#ifndef UGDATACODEC_H
#define UGDATACODEC_H

#include "Element/OgdcRasterBlock.h"
#include "Stream/UGMemoryStream.h"
//#include "CacheFile/UGAbstractCacheFile.h"

namespace UGC {

typedef OgdcRasterBlock UGImgBlock;
typedef OgdcRasterScanline UGImgScanline;

#define UGMAXWORD 0xffff
//! 空间数据编码类
class GEOMETRY_API UGDataCodec  
{
public:
	//! 编码类型
	enum CodecType
	{
		//! 不使用编码方式
		encNONE		= 0,
		//{{保留原编码选项，但不使用，在下个版本去掉
 		//! 使用BYTE(1)类型存储
 		encBYTE		= 1,	
 		//! 使用WORD(2)类型存储
 		encWORD		= 2,
 		//! 使用3字节类型存储
 		enc3BYTE	= 3,
 		//! 使用4字节类型存储
 		encDWORD	= 4,	
 		//! 使用float类型存储
 		//encFLOAT	= 5,	
 		//! 使用doubel类型存储	
 		//encDOUBLE	= 6,	
 		//! Image的压缩
 		encDCT		= 8,	
 		//! SuperMap Grid LZW encoded.
 		encSGL		= 9,	
 		//! another Grid LZW encoded.
 		encLGL		= 10,	
 		//! 适用于索引色影像的压缩方式，无损
 		encLZW		= 11,	
 		//! PNG压缩算法
 		encPNG		= 12,	
		//}}
		
	//! \brief 矢量编码
		//! \brief 不使用编码方式
		envNONE		= 0,
		//! \brief 使用doubel类型存储	
		//envDOUBLE	= 6,
		//! \brief 使用BYTE(1)类型存储
		envBYTE		= 1,
		//! \brief 使用WORD(2)类型存储
		envWORD		= 2,
		//! \brief 使用3字节类型存储
		env3BYTE	= 3,
		//! \brief 使用4字节类型存储
		envDWORD	= 4,
		//! \brief ZIP无损压缩方式
 		envLZW		= 11,	
		//! \brief 使用哈夫曼编码	 
		envHfm		= 13,
		
	//! \brief 栅格编码
		//! \brief 不使用编码方式
		enrNONE		= 0,
		//! \brief Image的压缩
		enrDCT		= 8,
		//! \brief SuperMap Grid 游程编码.
		enrSGL		= 9,
		//! \brief another Grid 游程编码. 不再支持该类编码的创建
 		enrLGL		= 10,	
		//! \brief 适用于索引色影像的压缩方式，无损,用的是ZIP压缩方法
		enrLZW		= 11,	
		//! \brief PNG压缩算法
		enrPNG		=12,
		//! \brief 使用哈夫曼编码
		enrHfm		= 13,

		//! \brief s3t dxtn纹理压缩
		enrS3TCDXTN	= 14,

		//! \brief lzma无损压缩
		enrLZMA	= 15,

		//! \brief Gif编码
		enrGIF	= 16,

		//! \brief 混合编码
		enrCompound = 17,

		//! \brief DDS+ZIP编码
		enrDDSZIP = 18,

		//! \brief pvr PVRTC2  2 or 4 bpp,may be ipad4 will support... wxh
		enrPVRTPF_PVRTC2 = 19,

		//! \brief pvr PVRTC  2 or 4 bpp the ipad3 only support pvrtc1 default 2bpp
		enrPVRTPF_PVRTC = 20,
        
        //! \brief pvr PVRTC  2 or 4 bpp this is 4bpp
        enrPVRTPF_PVRTC_4bpp = 21,

		//! \brief Ericsson Texture compression version 1, standard format for openGL ES 2.0.  added by shansg
		enrPVRTPF_ETC1 = 22,

		//! \brief JPG
		enrJPG = 23,

		//! \brief pvr PVRTCFAST
		enrPVRTPF_PVRTC_FAST = 24,
	};

public:
	//! \brief 把一个点串按照指定的编码方式进行编码，并输出到给定的缓存中
	//! \param pPoints 要编码的点串的指针
	//! \param nPointCount 要编码的点串的点个数
	//! \param buf[out] 输出的缓存
	//! \param eType 编码方式, 支持encBYTE, encWORD, enc3BYTE, encDWORD四种编码方式
	//! \param rcBounds 点串的Bounds, 函数内部要使用;
	//!		对于Geometry,可以用Geometry的GetBounds方法;对于非Geometry内的点串,可以用UGOpPoints的Bounds方法
	//! \remarks 主要用于矢量数据集编码使用
	//! \return 成功返回true,失败返回false
	static UGbool Encode( const UGPoint2D* pPoints, UGint nPointCount,
		UGArray<UGuchar>& buf, CodecType eType, const UGRect2D& rcBounds);

	//! \brief Encode SDBPlus
	static UGbool EncodeSDB( const UGPoint2D* pPoints, UGint nPointCount,
		UGArray<UGuchar>& buf, CodecType eType, const UGRect2D& rcBounds);

	//! \brief 和Encode功能类似,不同在于输出中不包含left,bottom和GridSize
	//! \remarks 一般使用Encode, 本函数仅在缓存文件等特殊地方使用
	static UGbool EncodeStatic( const UGPoint2D* pPoints, UGint nPointCount,
		UGArray<UGuchar>& buf, CodecType eType, UGdouble dLeft,UGdouble dBottom,
		UGdouble dXGridSize,UGdouble dYGridSize);

	/*static UGbool Encode(const UGPoint2GSetReferencePoints( UGPoint2D* pPoints, UGint nCount )
		CodecType eType, 
		const UGRect2DComputeBounds*/

	//! \brief 从数据区中解码得到点串, 和 Encode 方法相对应
	//! \param pPoints 得到的点串, 注意点串的空间要事先分配好
	//! \param pRawData 数据区指针
	//! \param nRawSize 数据区的大小(字节)
	//! \param eType 编码方式, 支持encBYTE, encWORD, enc3BYTE, encDWORD四种编码方式
	//! \return 成功返回true,失败返回false
	static UGbool Decode(UGPoint2D* pPoints, const UGuchar* pRawData, UGint nRawSize, CodecType eType);

	//! \brief Decode SDBPlus
	static UGbool DecodeSDB(UGPoint2D* pPoints, UGint nPointCount,UGStream &stream, CodecType eType,const UGRect2D& rcBounds);	

	//! \brief 和 EncodeStatic 对应的解码方法
	static UGbool DecodeStatic(UGPoint2D* pPoints,const UGuchar* pRawData,UGint nRawSize,
		UGdouble dLeft,UGdouble dBottom,UGdouble dXGridSize,UGdouble dYGridSize, CodecType eType);

	//! \brief 影像块编码
	static UGint Encode(UGImgBlock * pImgBlock,
		OGDC::PixelFormat ipf,
		UGbyte *&pBuffer,
		UGuint &nBufferSize,
		UGint nNoValue,
		CodecType eType,
		UGint nQuality = 75);

	//! \brief 影像块解码
	static UGbool Decode(UGImgBlock* pImgBlock,
		const UGuchar* pRawData,
		UGint nRawSize,
		UGint nNoValue,
		CodecType encType);

private:
	//! \brief 把uint类型的最小的三个字节存储到stream中
	static void Save3Byte(UGStream& stream, UGuint value);
	//! \brief 从stream中读取三个字节（最小），构成一个uint
	static UGuint Load3Byte(UGStream& stream);

	static UGint Block2DCT(UGbyte * &pData,UGImgBlock* ImgBlock,UGint nQuality);
	static UGint DCT2Block(UGbyte * pData,UGint nSize,UGImgBlock* ImgBlock);

	//PNG 压缩
	static UGint Block2PNG(UGbyte * &pData,UGImgBlock* ImgBlock);
	static UGbool SavePNGHeader(void* pPngStruct, void* pPngInfo, OGDC::PixelFormat nPixelFormat, UGint nWidth, UGint nHeight);
	static UGint PNG2Block(UGbyte * pData,UGint nSize,UGImgBlock* ImgBlock);


// 	//! \brief 		压缩数据
// 	//! \return 	UGint			返回数据长度
// 	//! \param 		nPixelFormat	DEM数据的像素格式，可用值：
// 	//!								IPF_FLOAT, IPF_DOUBLE
// 	//! \param 		*pSrcData       数据指针
// 	//! \param 		lWidth			数据大小
// 	//! \param 		cx				数据长宽
// 	//! \param 		cy
// 	//! \param 		dMinZ			最大值
// 	//! \param 		dMaxZ           最小值
// 	//! \param 		*pDestData      压缩后的数据指针
// 	//! \remarks 		
// 	static UGint EncDEM(UGint nPixelFormat, 
// 					 UGbyte *pSrcData, 
// 					 UGint lWidth,
// 					 UGint cx,UGint cy,
// 					 UGdouble dMinZ, 
// 					 UGdouble dMaxZ, 
// 					 UGbyte *pDestData);
// 
	//! \brief 		解压缩
	//! \return 	UGbool
	//! \param 		nPixelFormat DEM数据的像素格式
	//! \param 		*pSrcData 数据内存指针
	//! \param 		lWidth 数据内存字节长度
	//! \param 		cx 数据块长
	//! \param 		cy 数据块宽 
	//! \param 		&dMinZ 最大值
	//! \param 		&dMaxZ 最小值
	//! \param 		*pDestData 压缩后的数据内存指针
	//! \param      nNoValue nodata的值
	static UGbool DecDEM(UGint nPixelFormat,
					 UGbyte *pSrcData, 
					 UGint lWidth,
					 UGint cx,UGint cy,
					 UGdouble &dMinZ, 
					 UGdouble &dMaxZ, 
					 UGbyte *pDestData,
					 UGint nNoValue);

	//! \brief 		压缩
	//! \return 	UGbool
	//! \param 		nPixelFormat DEM数据的像素格式
	//! \param 		*pSrcData 数据内存指针
	//! \param 		lWidth 数据内存字节长度
	//! \param 		cx 数据块长
	//! \param 		cy 数据块宽 
	//! \param 		&dMinZ 最大值
	//! \param 		&dMaxZ 最小值
	//! \param 		*pDestData 压缩后的数据内存指针
	static UGint EncDEM(UGImgBlock& imgBlock, UGMemoryStream& streamBlock, UGint nNoValue);

//	static UGbool DecDEM(UGImgBlock& imgBlock, UGMemoryStream& streamBlock, UGint nNoValue);

	
	// SuperMap Grid LZW 编码 
	static UGbool EncSGL(UGImgBlock& imgBlock, UGMemoryStream& streamBlock, UGint nNoValue);
	//{{
	static void SGL_SaveOneValue(UGdouble dValue, UGint nBitCount, UGMemoryStream& streamBlock );

	static void SGL_SaveSeperatorValues(UGdouble* pValues, UGint nCount, UGMemoryStream& streamBlock, 
										UGdouble dMinValue, UGint nBitCount);
	static void SGL_SaveSameValues(UGdouble dValue, UGint nCount, UGMemoryStream& streamBlock, 
									UGdouble dMinValue, UGint nBitCount);
	static void SGL_SaveNoValues(UGint nCount, UGMemoryStream& streamBlock);

	static void SGL_EndSave(UGMemoryStream& streamBlock, UGint nStartPos);
	//}}
	// SuperMap Grid LZW 解码
	static UGbool DecSGL(UGImgBlock& imgBlock, UGMemoryStream& streamBlock, UGint nNoValue);
	// {{
	static UGdouble SGL_LoadOneValue(UGMemoryStream& streamBlock, UGint nValueSize);
	static void SGL_LoadSeperatorValues(UGdouble *dValues, UGint nCount,
										UGMemoryStream& streamBlock,  UGint nTileType, UGdouble dMinValue);
	static UGbyte GetStoreBitCount(UGint nValue);
	// }}

};
}

#endif /*UGDATACODEC_H*/

