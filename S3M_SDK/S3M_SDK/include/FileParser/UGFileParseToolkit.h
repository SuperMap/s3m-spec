
#if !defined(AFX_UGFILEPARSETOOLKIT_H__AAED9863_4B12_4F48_8406_F32E3EB0CF52__INCLUDED_)
#define AFX_UGFILEPARSETOOLKIT_H__AAED9863_4B12_4F48_8406_F32E3EB0CF52__INCLUDED_

#include "Geometry/UGGeometry.h"
#include "Stream/ugdefs.h"
#include "Stream/UGFileStream.h"
#include "Base3D/UGTextureData.h"

namespace UGC {

//class FileParserRaster;
struct UGImageData;
class UGExportParams;
class UGImportParams;
//#if !defined OS_ANDROID && !defined IOS
class UGFeatureClassDefn;
class UGFeatureClassInfos;
//#endif
class RasterInfo;
class RasterInfos;
typedef OGDC::PixelFormat UGPixelFormat;

//typedef UGArray<UGFeatureClassDefn*> UGFeatureClassInfos;

//! \brief FileParser使用的工具类。
class FILEPARSER_API UGFileParseToolkit  
{
public:

	//! \brief 定义RGB24bit的结构
	struct UGRGB24bit{
		UGuchar R;
		UGuchar G;
		UGuchar B;
	};

	//! \brief 定义RGB48bit的结构
	struct UGRGB48bit{
		UGushort R;
		UGushort G;
		UGushort B;
	};

	//! \brief 定义RGBA32bit的结构
	struct UGRGBA32bit{
		UGuchar R;
		UGuchar G;
		UGuchar B;
		UGuchar A;
	};

	typedef struct FMETileFunc
	{	
		FMETileFunc()
		{
			iBnad		    = 0;
			lRow		    = 0;
			lCol		    = 0;
			pDatasetRaster  = 0;
			pProgresAddress = 0;
			bIsHasProgress  = FALSE;
			ePixelFormat    = IPF_UNKNOWN;
		}
		UGint		  iBnad;
		UGlong		  lRow; 
		UGlong		  lCol; 
		UGlong		  pDatasetRaster;
		UGlong		  pProgresAddress;
		UGbool		  bIsHasProgress;
		UGPixelFormat ePixelFormat;
	}UGFMETileFucParms;

	struct exRegionInfo
	{
		exRegionInfo()
		{
			subRegionIndex = -1;
			subRegionBoundsArea = 0.0;
			subRegionPointOffset = 0;
			subRegionPointCount = 0;
		}

	public:
		UGint subRegionIndex;
		UGdouble subRegionBoundsArea;
		UGint subRegionPointOffset;
		UGint subRegionPointCount;
	};

	//! \brief 构造函数。
	UGFileParseToolkit();
	
	//! \brief 析构函数。
	~UGFileParseToolkit();
public:
	//Add by tangzq in 2010-7-16
	static UGMutex m_mutex;
public:

	//! \brief 		根据象素格式返回调色板的个数
	//! \return 	UGint
	//! \param 		ePixelFormat		
	static UGint GetColorCount(UGPixelFormat ePixelFormat);

	//! \brief 		根据颜色的个数来得到向素格式
	//! \return 	PixelFormat 返回象素的格式
	//! \param 		nColorEntryCount  给出颜色的个数
	//! \remarks 	只有调色板的使用有效	
	static UGPixelFormat GetPixelFormat(UGint nColorEntryCount);

	//! \brief 从位深得到所对应的字节长度。
	//! \param pixformat [in] 位深。
	//! \return 位深对应的直接长度。
	static UGint GetPixelFormatBitSize(UGPixelFormat pixformat);	

	//! \brief 		根据BitCount来得到象素格式
	//! \return 	PixelFormat 返回象素格式
	//! \param 		nBitCount  	
	static UGPixelFormat Bit2Format(UGint nBitCount);
	
	//! \brief 		得到扫描行的字节个数
	//! \return 	UGint
	//! \param 		ePixelFormat 象素的格式
	//! \param 		nWidth       一行象素的个数	
	static UGint GetWidthBytes(UGPixelFormat ePixelFormat, UGint nWidth);


	//! \brief 扩展名找文件类型。
	//! \param nFileType [in]。
	static UGString GetExtNameByFileType(UGint nFileType);	

	//! \brief 文件类型找扩展名。
	//! \param strFileExtName [in]。
	static UGint GetFileTypeByExtName(UGString strFileExtName);	
 	
	//! \brief 输出UGImageData内容到文件。
	//! \param pParams [in] 栅格输出参数。
	//! \param imageData [in] 要输出的Image数据。
	//! \param bReverse [in] 是否反向。
	//! \param bound [in] 图像对应的地理范围
	//! \return 成功返回true，失败返回false。
	//! \attention imageData.pBits 不能为空。
	static UGbool SaveImageData(UGExportParams* pParams, const UGImageData& imageData, UGbool bReverse=FALSE, const UGRect2D& bound=UGRect2D());

	//! \brief 获取整副影像的数据。
	//! \param rFileParser [in] 栅格解析对象。
	//! \param strFilePathName [in] 要获取数据的格式。
	//! \param aryImageData [out] 输出数据。
	//! \param bReverse [in] 是否反向。
	//! \param iBand [in] 读取第几个波段(从0开始，小于0默认所有波段)。
	static UGbool GetImageData(UGImportParams* pParams, 
		UGArray<UGImageData*>& aryImageData, UGbool bReverse=FALSE, UGint iBand = -1);

	//! \brief 指定范围获取影像的数据。
	//! \param pParams [in] 。
	//! \param aryImageData [in]。
	//! \param rcBound [in] 范围要在图像内，且传进来时已经裁剪好。
	//! \param FALSE [in]。
	static UGbool GetImageData(UGImportParams* pParams, UGArray<UGImageData*>& aryImageData, const UGRect& rcView);	

	//! \brief 指定范围获取影像的数据。
	//! \param pParams [in] 。
	//! \param aryImageData [in]。
	//! \param rcBound [in] 范围要在图像内，且传进来时已经裁剪好
	//! \所取数据的波段索引 nBandIndex
	//! \param FALSE [in]。
	static UGbool GetImageData(UGImportParams* pParams, UGImageData*& pImageData, const UGRect& rcView,UGint nBandIndex=0);

	//! \brief 从内存文件中解析得到影像的数据
	//! \param nfileType [in] 文件类型
	//! \param arrInPut [in] 文件的二进制流
	//! \param arrOutPut [out] 输出影像数据。
	//! \return 成功返回1，失败返回-1。
	//! \attention arrOutPut 函数内部分配空间, 用户负责释放。
	static UGint GetImageData(UGint nfileType, const UGByteArray& arrInPut, UGArray<UGImageData *>& arrOutPut);

	static UGint GetImageData(UGint nfileType, const UGImageData* inPut, UGByteArray& arroutPut);
	
	//! \brief 从文件解析出纹理数据
	static UGbool GetTextureData(const UGString strFilePath, UGTextureData*& pTextureData,UGbool bReverse = TRUE);
	
	//! \brief对ImageData进行编码，并将其保存到UGByteArray中
	//! \param nCodeType[in] 编码类型
	//! \param input [in] 要编码的ImageData数据
	//! \param arrOutput[out] 数据压缩后保存在这里
	//! \param bSwapRB[in] 是否交换R和B通道的值
	//! \return 成功返回TRUE，失败返回FALSE
	//! \remarks 目前只支持DXTZ PVRZ编码格式,且像素格式必须为RGBA add by shansg 2013-7-16
	static UGbool EncodeImgDataAndSaveToByteArray(UGDataCodec::CodecType nCodeType, UGImageData& input, UGByteArray& arrOutput, UGbool bSwapRB = TRUE);

	//! \brief 拼接影像的Tile。
	//! \param blocks [in] 瓦片组。
	//! \param data [out] 拼接后的数据。
	//! \return 是否成功。
	//! \remarks blocks里面的Block都要设置其的瓦片位置索引，data需要设置好各种大小，并分配好内存。
	static UGbool JoinBlocks(UGArray<UGImgBlock> blocks, UGImageData& data);
	
	
	//! \brief 从RGBRGBRGB...组织形式的3波段中取出一个波段。
	//! \param input [in]。
	//! \param inByteCount [in]。
	//! \param pixformat [in]。
	//! \param iBand [in]。
	//! \param output [out]。
	//! \remarks 输出数组需要外部释放内存。
	static UGbool GetABandFromBands(const UGbyte* input, UGint inByteCount,
		UGPixelFormat pixformat, UGint bandCount, UGint iBand, UGbyte** output);	

	//! \brief 获取通过参数获取的影像的基本信息 即跟参数存在关系。
	//! \param pParams [in]。
	//! \attention 内部分配内存，外部释放。
	static RasterInfo* GetRasterInfo(UGImportParams* pParams);	

	//! \brief 获取原始影像的基本信息。
	//! \param pParams [in]。
	//! \attention 内部分配内存，外部释放。
	static RasterInfo* GetSrcRasterInfo(UGImportParams* pParams);

	//! \brief 获取通过参数获取的影像的基本信息集合 即跟参数存在关系。。
	//! \param pParams [in]。
	//! \attention 内部分配内存，外部释放。
	static RasterInfos* GetRasterInfos(UGImportParams* pParams);

	//! \brief 获取原始影像的基本信息集合集合。
	//! \param pParams [in]。
	//! \attention 内部分配内存，外部释放。
	static RasterInfos* GetSrcRasterInfos(UGImportParams* pParams);

	//! \brief CMYK转换到RGBA。
	//! \param pParams [in]。
	//! \param arrDatasetInfos [in] 返回矢量数据中的基本信息，内部分配内存，用户外部释放。
	static UGbool CMYK2RGBA(UGbyte** data, UGint pixelCount, UGbyte aValue);

	//! \brief RGBA转化成CMYK。
	//! \param pParams [in]。
	//! \param arrDatasetInfos [in] 返回矢量数据中的基本信息，内部分配内存，用户外部释放。
	static UGbool RGBA2CMYK(UGbyte** data, UGint pixelCount);

    //! \brief 交换双字节数字字节位置，用来处理大小端机的区别
	//! \param sValue [in] 要交换的数值
	//! \return 交换了位置后的数值
	static UGshort Flip16(UGshort sValue);

	//! \brief 交换双字节数字字节位置，用来处理大小端机的区别
	//! \param psValue [in/out] 要交换的数值数组指针
	//! \param nCount [/in] 要交换的数值个数
	//! \return 交换了位置后的数值
	static void Flip16(UGshort* psValue, UGint nCount);
 
	//! \brief 交换4字节数字字节顺序，用来处理大小端机的区别
	//! \param nValue [in] 要交换的数值
	//! \return 交换了位置后的数值
    static UGint Flip32(UGint nValue);

    
	//! \brief 交换4字节数字字节顺序，用来处理大小端机的区别
	//! \param pnValue [in/out] 要交换的数值数组
	//! \param nCount 要交换的数值的个数
	//! \return void
	static void  Flip32(UGint* pnValue, UGint nCount);



	//! \brief 根据主文件找到坐标参考文件。
	//! \param pParams [in] strMainFile 影像文件。
	//! \return 找到的影像坐标文件。
	//! \ 查找顺序为 *.tfw, .tifw, .wld
	//! \attention 此函数为 ReadWorldFile 函数服务
	static UGString RetrieveWorldFile(const UGString& strMainFile);

	//! \brief 读取ESRI样式的world file。	
	//! \remarks This function reads an ESRI style world file, and formats a geotransform
	//!  from it's contents.  It does the same as GDALLoadWorldFile() function, but
	//! it will form the filename for the worldfile from the filename of the raster
	//!  file referred and the suggested extension.  If no extension is provided,
	//!  the code will internally try the unix style and windows style world file
	//!  extensions (eg. for .tif these would be .tfw and .tifw). 
	//! 
	//!  The world file contains an affine transformation with the parameters
	//!  in a different order than in a geotransform array. 
	//! 
	//! 
	//!  geotransform[1] : width of pixel
	//!  geotransform[4] : rotational coefficient, zero for north up images.
	//!  geotransform[2] : rotational coefficient, zero for north up images.
	//!  geotransform[5] : height of pixel (but negative)
	//!  geotransform[0] + 0.5 * geotransform[1] + 0.5 * geotransform[2] : x offset to center of top left pixel.
	//!  geotransform[3] + 0.5 * geotransform[4] + 0.5 * geotransform[5] : y offset to center of top left pixel.
	//! 
	//! 
	//! \param strfilePath the world file name.
	//! \param padfGeoTransform the six double array into which the 
	//! \attention geotransformation should be placed. 
	//! \return TRUE on success or FALSE on failure.		
	static UGbool ReadWorldFile(const UGString& strfilePath, UGdouble* padfGeoTransform);

	//! \brief 创建ESRI样式的world file。	
	//! \param strfilePath [in] 。
	//! \param strExtension [in] the extension to use (ie. ".wld"). Must not be NULL。
	//! \param padfGeoTransform [in] the six double array from which the geotransformation should be read.。
	//! \return 。
	//! \remarks  This function writes an ESRI style world file from the passed geotransform. 
	//!  geotransform[1] : width of pixel
	//!  geotransform[4] : rotational coefficient, zero for north up images.
	//!  geotransform[2] : rotational coefficient, zero for north up images.
	//!  geotransform[5] : height of pixel (but negative)
	//!  geotransform[0] + 0.5 * geotransform[1] + 0.5 * geotransform[2] : x offset to center of top left pixel.
	//!  geotransform[3] + 0.5 * geotransform[4] + 0.5 * geotransform[5] : y offset to center of top left pixel.
	//! \attention 。
	static UGint WriteWorldFile(const UGString& strfilePath, const UGString& strExtension, UGdouble * padfGeoTransform);


	//////////////////////////////////////////////////////////////////////////
	// 矢量文件的相关工具函数
	//////////////////////////////////////////////////////////////////////////
	
//#if !defined OS_ANDROID && !defined IOS
	//! \brief 得到矢量文件中要素层的基本信息。
	//! \param pParams [in]。
	//! \param arrDatasetInfos [in] 返回矢量数据中的基本信息，内部分配内存，用户外部释放。
	static UGbool GetFeatureClassDefn(UGImportParams* pParams, UGFeatureClassInfos& arrDatasetInfos);

	//! \brief 得到工程文件中要素层的名字和地图名字。
	//! \param pParams [in]。
	static UGbool GetWorkspaceLayerAndMapNames(UGImportParams* pParams, 
		UGArray<UGString>& oLayerNames, 
		UGArray<UGString>& oMapNames);
//#endif

	//! \brief 转换扫描行数据的格式
	//! \param inSL [in] 原始的扫描行
	//! \param outSL [out] 转换后的扫描行。函数内会对其进行适当空间大小的分配。
	//! \param pNoValue[2] [in] 转换前后的无值。如果没有设置，则表示数据中没有无值。建议设置。
	//! \param pMaxAndMin[4] [in] 转换前后的最值，分别保存:转换前最大值、转换前最小值转换后最大值、转换后最小值。建议设置此值，默认的处理方法产生的结果可能不太好。
	//! \return TRUE 转换成功，FALSE 转换失败。
	//! \attention outSL需要在函数外创建，并设置好像素格式。 
	static UGbool TransScanlineIPF(UGImgScanline &inSL ,UGImgScanline &outSL 
		,const UGdouble pNoValue[2] = NULL
		,const UGdouble pMaxAndMin[4] = NULL);

	//! \brief 转换影像块的数据格式 
	//! \param inBlk [in] 原始的影像块 
	//! \param outBlk [out] 转换后的影像块。函数内会对其进行适当空间大小的分配。
	//! \param pNoValue[2] [in] 转换前后的无值。如果没有设置，则表示数据中没有无值。建议设置。
	//! \param pMaxAndMin[4] [in] 转换前后的最值，分别保存:转换前最大值、转换前最小值转换后最大值、转换后最小值。建议设置此值，默认的处理方法产生的结果可能不太好。
	//! \return TRUE 转换成功，FALSE 转换失败
	//! \remarks 函数的实现主要是调用TransScanlineIPF(...)。
	//! \attention outBlk需要在函数外创建对象。 
	static UGbool TransImgBlockIPF(UGImgBlock &inBlk ,UGImgBlock &outBlk 
		,const UGdouble pNoValue[2] = NULL
		,const UGdouble pMaxAndMin[4] = NULL);

	//! \brief 获取一个可用的名字
	//! \param strName [in] 原来的名字
	//! \return 合法的名字
	//! \remarks 主要检查名字中是否有非法字符，如果存在，则将非法字符去掉
	//! 合法的名字：只含有数字、字母和下划线，并且第一个字符只能是字母
	//! 本函数实现主要参考UGDataSource::IsValidFieldOrTableName
	static UGString GetAvailableName(UGString &strName);

	//! \brief 从外部颜色变成UGColor（交换RB）。XRGB->ABGR
	//! \param [in]。
	//! \return 返回UGColor。
	static inline UGColor UGCOLOR(UGint color)
	{
		UGbyte* c = (UGbyte*)&color;
		//UGbyte tmp = c[1];c[1] = c[3];c[3] = tmp;
		UGColor abgr =  UGRGB(c[2],c[1],c[0]);
		//UGbyte* pbgr = (UGbyte*)&abgr;
		return abgr;
	}

	// 获取多帧gif数据 [6/10/2011 林文玉]
	//! \brief 读取gif数据各帧数据，静态函数。
	//! \param strGifFile [in] gif文件路径。
	//! \param arrPicture [out] 返回影像数据。
	//! \return 是否读取成功。用户负责释放arrPicture内存
	static UGbool GetDynamicIMGData(const UGString& strGifFile, UGArray<UGGeoPicture*>& arrPicture,UGbool bReverse);

	//! \brief 根据类型和文件名获取ImageData，数据量较小时使用，比如网络瓦片数据
	//! \param nfileType[in] 文件类型
	//! \param strFilePath[in] 文件路径
	//! \return 数据的ImageData
	static UGImageData* LoadImageData(UGint nfileType, const UGString& strFilePath);

	//! \brief 根据内存流获取ImageData，数据量较小时使用，比如网络瓦片数据
	//! \param nfileType[in] 文件类型
	//! \param ImageBytes[in] 文件内存流
	//! \return 数据的ImageData
	static UGImageData* LoadImageData(UGint nfileType, const UGByteArray& ImageBytes);

	//! \brief 根据面所有子对象点集、各个子对象点个数，子对象个数获取面的子对象之间的关系
	//! \param pPoint2Ds[in] 面所有子对象点集
	//! \param pPolyCount[in] 各个子对象点个数
	//! \param nSubCount[in] 子对象个数
	//! \return 面关系的子对象索引，外层数组表示多面或面，size为1表示面，多个表示多面，内层数组表示岛洞面，size为1表示无岛洞，多个表示岛洞，数组中的值为面子对象下标
	static UGArray<UGArray<UGint> > GetRegionRel(const UGPoint2D* pPoint2Ds, const UGint* pPolyCount, UGint nSubCount);
};

} // namespace UGC

#endif // !defined(AFX_UGFILEPARSETOOLKIT_H__AAED9863_4B12_4F48_8406_F32E3EB0CF52__INCLUDED_)

