#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "MemoryStream.h"
#include "zlib/include/zlib.h"
#include <string>
#include <iostream>

using namespace std;

//! 编码类型
enum CodecType
{
	//! 不使用编码方式
	encNONE		= 0,
	//{{保留原编码选项，但不使用，在下个版本去掉
	//! 使用unsigned char(1)类型存储
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
	//! \brief 使用unsigned char(1)类型存储
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

enum PixelFormat 
{
	//! \brief 尚未设定, 内部使用
	IPF_UNKNOWN			= 0,
	//! \brief 1位,单色
	IPF_MONO			= 1,
	//! \brief 4位,16色
	IPF_FBIT			= 4,
	//! \brief 8位无符号,256色
	IPF_UBYTE			= 8,
	//! \brief 8位有符号,256色,新增
	IPF_BYTE			= 80,
	//! \brief 16位有符号,彩色
	IPF_TBYTE			= 16,
	//! \brief 16位无符号,彩色，新增
	IPF_UTBYTE			= 160,
	//! \brief 24位,真彩色
	IPF_RGB				= 24,
	//! \brief 32位,增强真彩色
	IPF_RGBA			= 32,
	//! \brief 48位,真彩色
	IPF_TRGB			= 48,
	//! \brief 64位,长整型
	IPF_LONGLONG		= 64,
	//! \brief LONG
	IPF_LONG			= 320,
	//! \brief LONG
	IPF_ULONG			= 321,
	//! \brief 32位,浮点型
	IPF_FLOAT			= 3200,
	//! \brief 96位格式，R通道32位（float），B通道32位（float），G通道32位（float）
	IPF_FLOAT32_RGB     = 4800,
	//! \brief 128位格式，R通道32位（float），B通道32位（float），G通道32位（float）, A通道32位（float）
	IPF_FLOAT32_RGBA     = 4801,
	//! \brief 64位,双精度浮点型
	IPF_DOUBLE			= 6400	
};

class TextureInfo
{
public:
	TextureInfo();
	~TextureInfo();
	const TextureInfo& operator=(const TextureInfo& Info);
	TextureInfo(const TextureInfo& Info);
	// 接管了外边给的内存
	TextureInfo(PixelFormat enFormat,unsigned int Width,unsigned int Height,void* data,unsigned int nSize, const string& path = "");

	//! \brief 从流文件中加载
	//! \param fStream 文件流[in]。
	void Load(MemoryStream& fStream);

	//! \brief 存入流文件中
	//! \param fStream 文件流[in]。
	void Save(MemoryStream& fStream);

	//! \brief 获取数据存储时的大小
	//! \return 返回存储大小
	//! \remark 暂未考虑编码情况，与Save()一起维护
	//! \attention 字符串的长度要+4
	int GetDataSize();
public:
	bool			m_bLoadImage;

	// 纹理图片的像素格式
	PixelFormat		m_enFormat;

	// 纹理图片的宽度
	unsigned int	m_nWidth;

	// 纹理图片的高度
	unsigned int	m_nHeight;

	// 纹理图片流数据
	unsigned char*		m_pBuffer;

	// 纹理图片流数据的长度,带压缩时很重要
	unsigned int		m_nSize;

	// 纹理图片的索引路径
	string	m_strPath;

	string	m_strName;

	// 纹理图片数据中一共有几层mipmap
	unsigned int	m_nMipLev;

	// 纹理图片的压缩类型
	CodecType	m_nCompress;
};
//! \brief
enum _3DPixelFormat
{
	//! \brief 未知像素格式.
	PF_UNKNOWN = 0,
	//! \brief 8位像素，用于亮度.
	PF_L8 = 1,
	PF_BYTE_L = PF_L8,
	//! \brief 16位像素，用于亮度.
	PF_L16 = 2,
	PF_SHORT_L = PF_L16,
	//! \brief 8位像素，用于alpha值.
	PF_A8 = 3,
	PF_BYTE_A = PF_A8,
	//! \brief 8位像素, 4位alpha值, 4位亮度.
	PF_A4L4 = 4,
	//! \brief 两字节像素, 一个用于alpha值，一个用于亮度
	//! \brief 8位像素, 4位亮度, 4位alpha.
	PF_L4A4 = 35,
	//! \brief 16位像素, 每4位用于BGRA.
	PF_B4G4R4A4 = 36,
	//! \brief 24位像素, 每8位用于BRG.
	PF_B8R8G8 = 37,
	//! \brief 32位像素, 每10位用于BGR, 2位用于alpha.
	PF_B10G10R10A2 = 38,
	PF_BYTE_LA = 5,
	//! \brief 16位像素格式, R,G,B为5，6，5.
	PF_R5G6B5 = 6,
	//! \brief 16位像素格式, B,G,R为5，6，5.
	PF_B5G6R5 = 7,
	//! \brief 8位像素, B,G,R为2，3，3.
	PF_R3G3B2 = 31,
	//! \brief 16位像素, alpha，R,G,B为4,4,4,4.
	PF_A4R4G4B4 = 8,
	//! \brief 16位像素, alpha，R,G,B为1,5,5,5.
	PF_A1R5G5B5 = 9,
	//! \brief 24位像素, R,G,B为8,8,8.
	PF_R8G8B8 = 10,
	//! \brief 24位像素, B,G,R为8,8,8.
	PF_B8G8R8 = 11,
	//! \brief 32位像素, alpha,R,G,B为8,8,8,8.
	PF_A8R8G8B8 = 12,
	//! \brief 32位像素, B, G, R,alpha为8,8,8,8.
	PF_A8B8G8R8 = 13,
	//! \brief 32位像素, B, G, R,alpha为8,8,8,8.
	PF_B8G8R8A8 = 14,
	//! \brief 32位像素, R, G, B,alpha为8,8,8,8.
	PF_R8G8B8A8 = 28,
	// 128位像素, 每个元素占32位
	PF_FLOAT32_RGBA = 25,
	//! \brief 32位像素, R, G, B为8，8,8。alpha无效
	PF_X8R8G8B8 = 26,
	//! \brief 32位像素, B G, R为8，8,8。alpha无效
	PF_X8B8G8R8 = 27,
	//! \brief 3字节像素, 每个颜色占一个字节
	PF_BYTE_RGB = PF_B8G8R8,
	//! \brief 3字节像素, 每个颜色占一个字节
	PF_BYTE_BGR = PF_R8G8B8,
	//! \brief 4字节像素, 每个颜色和alpha各占一个字节
	PF_BYTE_BGRA = PF_A8R8G8B8,
	//! \brief 4字节像素, 每个颜色和alpha各占一个字节
	PF_BYTE_RGBA = PF_A8B8G8R8,
	//! \brief 32位像素 两位用于alpha，每个颜色占10位
	PF_A2R10G10B10 = 15,
	//! \brief 32位像素, 两位用于alpha，每个颜色占10位
	PF_A2B10G10R10 = 16,
	//! \brief DirectDraw Surface (DDS) DXT1 format
	PF_DXT1 = 17,
	//! \brief DirectDraw Surface (DDS) DXT2 format
	PF_DXT2 = 18,
	//! \brief DirectDraw Surface (DDS) DXT3 format
	PF_DXT3 = 19,
	//! \brief DirectDraw Surface (DDS) DXT4 format
	PF_DXT4 = 20,
	//! \brief DirectDraw Surface (DDS)) DXT5 format
	PF_DXT5 = 21,
	// 16位像素, 用于R
	PF_FLOAT16_R = 32,
	// 48位像素, 每16位float用于RGB
	PF_FLOAT16_RGB = 22,
	// 64位像素,每16位float用于RGBA
	PF_FLOAT16_RGBA = 23,
	// 16位像素，float用于R
	PF_FLOAT32_R = 33,
	// 96位像素, 每32位float用于RGB
	PF_FLOAT32_RGB = 24,
	// 128位像素, 每32位float用于RGBA
	// 用于深度像素
	PF_DEPTH = 29,
	// 64位像素, 每16位float用于RGBA
	PF_SHORT_RGBA = 30,
	// 当前定义像素格式的数目
	PF_COUNT = 34						
};
//! \brief 纹理数据的封装类。
class TextureData
{
public:
	//! \brief 构造函数
	TextureData();
	//! \brief 拷贝构造函数
	TextureData(const TextureData& other);
	//! \brief 拷贝构造函数
	TextureData(const TextureInfo& textureInfo);
	//! \brief 赋值函数
	TextureData& operator=(const TextureData& other);

	//! \brief 克隆自己的数据
	virtual TextureData* Clone();
	//! \brief 析构函数
	virtual ~TextureData();
	//! \brief 释放纹理数据
	void Release();

	//! \brief 从流文件中加载
	bool Load(MemoryStream& fStream ,unsigned int eCodecType = 0);

	//! \brief 存入流文件中
	void Save(MemoryStream& fStream,unsigned int eCodecType = 0);


private:
	//! \brief 存入流文件中
	//! \remarks 实现纹理的DDS编码及zip压缩
	bool BuildTextureTier(MemoryStream& fStream);
#if defined OPENGL_ES_VERSION2
	//! \brief 压缩类型由DXT转抽象为NONE
	void CompressTypeDXTToNONE();
#endif
public:
	//! \brief 纹理数据内存
	unsigned char* m_pBuffer;
	//! \brief 纹理的高
	unsigned int m_nHeight;
	//! \brief 纹理的宽
	unsigned int m_nWidth;
	//! \brief 纹理的深度
	unsigned int m_nDepth;
	//! \brief 纹理的像素格式，目前只支持RGBA
	_3DPixelFormat m_enFormat;

	// 压缩纹理类型
	CodecType m_CompressType;

	// 数据的内存大小
	unsigned int m_nSize;
};

struct TextureDataInfo
{
	TextureData* m_pTextureData;

	bool m_bMipmap;

	int m_nLevel;

	TextureDataInfo()
	{
		m_pTextureData = NULL;
		m_bMipmap = false;
		m_nLevel = 0;
	}

	TextureDataInfo(const TextureDataInfo& other)
	{
		m_pTextureData = NULL;
		*this = other;
	}

	TextureDataInfo& operator = (const TextureDataInfo& other)
	{
		if(m_pTextureData != NULL)
		{
			delete m_pTextureData;
			m_pTextureData = NULL;
		}

		if(other.m_pTextureData != NULL)
		{
			m_pTextureData = new TextureData(*other.m_pTextureData);
		}

		m_bMipmap = other.m_bMipmap;
		m_nLevel = other.m_nLevel;
		return *this;
	}

	~TextureDataInfo()
	{
		if(m_pTextureData != NULL)
		{
			delete m_pTextureData;
			m_pTextureData = NULL;
		}
	}
};

class ToolsEngine
{
public:
	//! \brief  返回比a大的，并且是最接近a的2的次方的数。
	static unsigned int NextP2(unsigned int a);

	static bool Zip(unsigned char *pvDestBuffer, unsigned int &dwDestLen,
		const unsigned char *pvSrcBuffer, unsigned int dwSrcLen, int nLevel = 8);
};
#endif