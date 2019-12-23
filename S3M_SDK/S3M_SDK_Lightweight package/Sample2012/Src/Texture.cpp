#include "Texture.h"
#include "ImageOperator.h"
TextureInfo::~TextureInfo()
{
	delete []m_pBuffer;
	m_pBuffer=NULL;
	m_enFormat=IPF_UNKNOWN;
	m_nSize = 0;
	m_nWidth = 0;
	m_nHeight = 0;
	m_nMipLev = 0;
	m_nCompress = CodecType::encNONE;
	m_bLoadImage = true;
}

const TextureInfo& TextureInfo::operator=( const TextureInfo& Info )
{
	if (&Info == this)
	{
		return *this;
	}

	delete []m_pBuffer;
	m_pBuffer = NULL;

	m_enFormat = Info.m_enFormat;
	m_nWidth = Info.m_nWidth;
	m_nHeight = Info.m_nHeight;
	m_nSize = Info.m_nSize;
	m_strPath = Info.m_strPath;
	m_nMipLev = Info.m_nMipLev; 
	m_strName = Info.m_strName;
	m_nCompress = Info.m_nCompress;

	if (Info.m_pBuffer != NULL && Info.m_nSize > 0)
	{
		m_pBuffer = (unsigned char*)malloc(Info.m_nSize);
		memcpy(m_pBuffer,Info.m_pBuffer,Info.m_nSize);
	}

	m_bLoadImage = Info.m_bLoadImage;
	return *this;
}

TextureInfo::TextureInfo( const TextureInfo& Info )
{
	m_enFormat = Info.m_enFormat;
	m_nWidth = Info.m_nWidth;
	m_nHeight = Info.m_nHeight;
	m_nSize = Info.m_nSize;
	m_pBuffer = NULL;
	if (m_nSize > 0)
	{
		m_pBuffer = (unsigned char*)malloc(m_nSize);
		memcpy(m_pBuffer,Info.m_pBuffer,m_nSize);
	}
	m_strPath = Info.m_strPath;
	m_nMipLev = Info.m_nMipLev; 
	m_strName = Info.m_strName;
	m_nCompress = Info.m_nCompress;
	m_bLoadImage = Info.m_bLoadImage;
}

TextureInfo::TextureInfo()
{
	m_enFormat = IPF_UNKNOWN;
	m_nWidth = 0;
	m_nHeight = 0;
	m_nSize = 0;
	m_pBuffer = NULL;
	m_nMipLev = 0;
	m_nCompress = CodecType::encNONE;
	m_bLoadImage = true;
}

TextureInfo::TextureInfo( PixelFormat enFormat,unsigned int Width,unsigned int Height,
							 void* data,unsigned int nSize, const string& path)
{
	m_enFormat = enFormat;
	m_nWidth = Width;
	m_nHeight = Height;
	m_nSize = nSize;
	m_pBuffer = (unsigned char*)data;
	m_strPath = path;
	m_nCompress = CodecType::encNONE;
	m_nMipLev = 0;
	m_bLoadImage = true;
}

int TextureInfo::GetDataSize()
{
	int nSize = 0;
	nSize += m_strPath.size() + sizeof(int);
	nSize += m_strName.size() + sizeof(int);
	nSize = sizeof(m_enFormat);
	nSize += sizeof(m_nCompress);
	nSize += sizeof(m_nHeight);
	nSize += sizeof(m_nWidth);
	nSize += sizeof(m_nMipLev);
	nSize += sizeof(m_nSize);
	nSize += sizeof(Byte) * m_nSize;
	return nSize;
}

void TextureInfo::Save(MemoryStream& fStream)
{
	fStream<<m_strPath;
	fStream<<m_strName;
	fStream<<m_enFormat;
	fStream<<m_nCompress;
	fStream<<m_nHeight;
	fStream<<m_nWidth;
	fStream<<m_nMipLev;
	fStream<<m_nSize;
	if(m_nSize > 0 && m_pBuffer != NULL)
	{
		fStream.Save(m_pBuffer, m_nSize);
	}
}

void TextureInfo::Load(MemoryStream& fStream)
{
	fStream>>m_strPath;
	fStream>>m_strName;
	fStream>>(int&)m_enFormat;
	fStream>>m_nCompress;
	fStream>>m_nHeight;
	fStream>>m_nWidth;
	fStream>>m_nMipLev;
	fStream>>m_nSize;
	if(m_nSize > 0)
	{
		m_pBuffer = (unsigned char*)malloc(m_nSize);
		fStream.Load(m_pBuffer, m_nSize);
	}
}

TextureData::TextureData()
{
	m_pBuffer=NULL;
	m_nWidth=0;
	m_nHeight=0;
	m_nDepth=1;
	m_enFormat=PF_UNKNOWN;
	m_nSize = 0;
	m_CompressType = CodecType::encNONE;
}

TextureData::TextureData(const TextureData& other)
{
	m_nWidth=other.m_nWidth;
	m_nHeight=other.m_nHeight;
	m_nDepth=other.m_nDepth;
	int nLen= 0;
	if (other.m_enFormat == PF_BYTE_RGB || other.m_enFormat == PF_BYTE_BGR)
	{
		nLen=m_nWidth*m_nHeight*3;
	}
	else if(other.m_enFormat == PF_L8)
	{
		nLen=m_nWidth*m_nHeight;
	}
	else if(other.m_enFormat == PF_BYTE_LA)
	{
		nLen = m_nWidth * m_nHeight * 2;
	}
	else
	{
		nLen=m_nWidth*m_nHeight*4;
	}

	m_pBuffer=NULL;
	m_enFormat=other.m_enFormat;
	m_nSize = other.m_nSize;
	m_CompressType = other.m_CompressType;
	if (nLen>0||m_nSize>0)
	{
		if (m_CompressType == 0)
		{
			m_pBuffer=new unsigned char[nLen];
			memcpy(m_pBuffer,other.m_pBuffer,nLen);	
		}
		else
		{
			m_pBuffer=new unsigned char[m_nSize];
			memcpy(m_pBuffer,other.m_pBuffer,m_nSize);	
		}
	}
	else
	{
		m_pBuffer=NULL;
	}
}

TextureData* TextureData::Clone()
{
	return new TextureData(*this);
}

TextureData::TextureData(const TextureInfo& textureInfo)
{
	m_nWidth=textureInfo.m_nWidth;
	m_nHeight=textureInfo.m_nHeight;
	m_nDepth=1;//textureInfo.m_nDepth; 目前textureInfo上没有m_nDepth

	if (textureInfo.m_enFormat == IPF_RGB)
	{
		m_enFormat = PF_BYTE_RGB;
	}
	else if (textureInfo.m_enFormat == IPF_RGBA)
	{
		m_enFormat = PF_BYTE_RGBA;
	}
	m_nSize = textureInfo.m_nSize;
	m_CompressType = textureInfo.m_nCompress;
	m_pBuffer=NULL;
	if (textureInfo.m_nSize>0)
	{
		m_pBuffer=new unsigned char[textureInfo.m_nSize];
		memcpy(m_pBuffer,textureInfo.m_pBuffer,textureInfo.m_nSize);
	}
}

TextureData& TextureData::operator=(const TextureData& other)
{
	if (this== &other)
	{
		return *this;
	}
	if (m_pBuffer!=NULL)
	{
		delete[] m_pBuffer;
		m_pBuffer=NULL;
	}
	m_nWidth=other.m_nWidth;
	m_nHeight=other.m_nHeight;
	m_nDepth=other.m_nDepth;
	int nLen = 0;
	if (other.m_enFormat == PF_BYTE_RGB || other.m_enFormat == PF_BYTE_BGR)
	{
		nLen=m_nWidth*m_nHeight*m_nDepth*3;
	}
	else if(other.m_enFormat == PF_L8)
	{
		nLen=m_nWidth*m_nHeight*m_nDepth;
	}
	else if(other.m_enFormat == PF_BYTE_LA)
	{
		nLen = m_nWidth * m_nHeight * m_nDepth * 2;
	}
	else
	{
		nLen=m_nWidth*m_nHeight*m_nDepth*4;
	}
	m_enFormat=other.m_enFormat;
	m_nSize = other.m_nSize;
	m_CompressType = other.m_CompressType;
	if (nLen>0)
	{
		if (m_CompressType == 0)
		{
			m_pBuffer=new unsigned char[nLen];
			memcpy(m_pBuffer,other.m_pBuffer,nLen);	
		}
		else
		{
			m_pBuffer=new unsigned char[m_nSize];
			memcpy(m_pBuffer,other.m_pBuffer,m_nSize);	
		}
	}
	return *this;
}

TextureData::~TextureData()
{
	Release();
}

void TextureData::Release()
{
	if (m_pBuffer!=NULL) 
	{
		delete []m_pBuffer;
		m_pBuffer=NULL;
		m_nSize = 0;
	}
}

bool TextureData::Load(MemoryStream& fStream ,unsigned int eCodecType)
{
	fStream>>m_CompressType;
	switch (m_CompressType)
	{
	case 0://encNONE
		{			
		}
		break;
	case 14://enrS3TCDXTN
		{
			fStream>>m_nWidth;
			fStream>>m_nHeight;
			int nFormat;
			fStream>>nFormat;
			m_enFormat = (_3DPixelFormat)nFormat;
			fStream>>m_nSize;
			int nZipSize = 0;
			fStream>>nZipSize;
			unsigned char* pOutZip = new(std::nothrow) unsigned char[nZipSize];
			if(pOutZip == NULL)
			{
				return false;
			}

			fStream.Load(pOutZip,nZipSize);

			m_pBuffer = new(std::nothrow) unsigned char[m_nSize];
			if(m_pBuffer == NULL)
			{
				delete[] pOutZip;
				pOutZip = NULL;
				return false;
			}

			if (m_pBuffer != NULL)
			{
				bool bResult = false;
				unsigned long sizeTemp = m_nSize;
				int nResult = uncompress(m_pBuffer,&sizeTemp,pOutZip,nZipSize);
				//防错处理
				switch (nResult)
				{
				case Z_OK:
					bResult = true;
					break;
				case Z_MEM_ERROR:
					cout<<"内存分配不足"<<endl;
					break;
				case Z_BUF_ERROR:
					cout<<"输出缓存没有足够的空间"<<endl;
					break;
				case Z_STREAM_ERROR:
					cout<<"压缩文件参数非法"<<endl;
					break;
				case Z_DATA_ERROR:
					//数据或者密码错误 //调换顺序，解决桌面打开工作空间未弹出输密码的对话框之前，先报错“数据错误”的问题 DFAB-1100		
					cout<<"数据错误"<<endl;
					break;
				default:
					//不可识别的错误，
					cout<<"不可识别的错误！"<<endl; 
					break;
				}
				m_nSize = sizeTemp;
			}
#if defined OPENGL_ES_VERSION2
			CompressTypeDXTToNONE();
#endif

			delete[] pOutZip;
			pOutZip = NULL;
		}
		break;
	case 16://enrGIF
		{
			fStream>>m_nSize;
			m_pBuffer = new(std::nothrow) unsigned char[m_nSize];
			if(m_pBuffer == NULL)
			{
				return false;
			}

			fStream.Load(m_pBuffer,m_nSize);
		}
		break;
	}	

	return true;
}

#if defined OPENGL_ES_VERSION2
void TextureData::CompressTypeDXTToNONE()
{
	if (m_pBuffer != NULL)
	{
		//解压DXT
		const unsigned int comp = 4;
		unsigned char* pOut  = NULL;
		unsigned int nSize =  ImageOperator::Decode(comp,m_nWidth,m_nHeight,&pOut,m_pBuffer ,DataCodec::enrS3TCDXTN);

		if (pOut != NULL)
		{
			//释放原有压缩数据
			Release();

			//转为非压缩类型
			m_CompressType = 0;
			m_nSize = nSize;
			m_enFormat = PF_A8B8G8R8;//在读取的时候已处理成RGBA

			m_pBuffer = new(std::nothrow) unsigned char[m_nSize];
			int i = 0,j = 0,k = 0;
			for (i =0;i<m_nHeight*m_nWidth;i++,j+=4,k+=4)//压缩时做了转换，再转换回来
			{
				m_pBuffer[j]   = pOut[k+2];	//r
				m_pBuffer[j+1] = pOut[k+1];	//g
				m_pBuffer[j+2] = pOut[k];	//b
				m_pBuffer[j+3] = pOut[k+3]; //a
			}

			delete[] pOut;
			pOut = NULL;
		}		
	}	
}
#endif

//! \brief 存入流文件中
void TextureData::Save(MemoryStream& fStream,unsigned int eCodecType)
{
	fStream<<eCodecType;
	switch (eCodecType)
	{
	case 0://encNONE
		{	
		}
		break;
	case 14://enrS3TCDXTN
		{
			if (m_CompressType == eCodecType)
			{
				unsigned int nCompressedTextureSize = m_nSize;
				unsigned char* pOutZip  = new unsigned char[m_nSize];

				bool bResult = false;
				unsigned long sizeTemp = nCompressedTextureSize;
				int nResult = compress2(pOutZip,&sizeTemp,m_pBuffer,m_nSize,8);
				nCompressedTextureSize = sizeTemp;

				// 防错处理
				if (nResult == Z_OK)
				{
					bResult = true;	
				}
				else if (nResult == Z_MEM_ERROR)
				{
					cout<<"!内存分配不足"<<endl;
				}
				else if (nResult == Z_BUF_ERROR)
				{
					cout<<"!输出缓存没有足够空间"<<endl;
				}
				else if (nResult == Z_STREAM_ERROR)
				{
					cout<<"!压缩文件参数非法"<<endl;
				}
				else
				{ // 不可识别的错误，
					cout<<"!不能识别的错误"<<endl;
				}

				fStream<<m_nWidth;
				fStream<<m_nHeight;
				fStream<<m_enFormat;
				fStream<<m_nSize;
				fStream<<nCompressedTextureSize;
				fStream.Save(pOutZip,nCompressedTextureSize);

				delete[] pOutZip;
				pOutZip = NULL;

			}
			else if(m_CompressType == encNONE)
			{
				BuildTextureTier(fStream);
			}
		}
		break;
	case 16://enrGIF
		{
			fStream<<m_nSize;
			fStream.Save(m_pBuffer,m_nSize);
		}
		break;
	}	
}

bool TextureData::BuildTextureTier(MemoryStream& fStream)
{
	unsigned int nWidth = m_nWidth;
	unsigned int nHeight = m_nHeight;

	// 把图片缩放到最合适的范围内
	unsigned int nAdjustW = ImageOperator::NextP2(m_nWidth);
	unsigned int nAdjustH = ImageOperator::NextP2(m_nHeight);

	unsigned int comp = 1;

	if (m_enFormat == PF_BYTE_RGB || m_enFormat == PF_BYTE_BGR)
	{
		comp = 3;
	}
	else if (m_enFormat == PF_BYTE_RGBA ||  m_enFormat == PF_BYTE_BGRA)
	{
		comp = 4;
	}

	unsigned char* pData    = new unsigned char[nAdjustW*nAdjustH*4];
	unsigned char* pTexture = new unsigned char[nAdjustW*nAdjustH*4];

	if (pData == NULL || pTexture == NULL)
	{
		delete []pData;
		delete []pTexture;
		return false;
	}

	if (nWidth != nAdjustW || nHeight != nAdjustH)
	{
		ImageOperator::Scale(comp,nWidth,nHeight,m_pBuffer,
			nAdjustW,nAdjustH,pTexture);
	}
	else
	{
		memcpy(pTexture,m_pBuffer,nWidth*nHeight*comp);
	}

	nWidth  = nAdjustW;
	nHeight = nAdjustH;

	// 调整像素格式排列成RGBA
	int i=0;
	int j=0;
	int k=0;

	if (comp == 3)
	{
		for (i=0;i<nHeight*nWidth;i++,j+=4,k+=3)
		{
			pData[j]   = pTexture[k+2]; //r
			pData[j+1] = pTexture[k+1]; //g
			pData[j+2] = pTexture[k];	//b
			pData[j+3] = 255;
		}
	}
	else
	{
		for (i=0;i<nHeight*nWidth;i++,j+=4,k+=4)
		{
			pData[j]   = pTexture[k+2];	//r
			pData[j+1] = pTexture[k+1];	//g
			pData[j+2] = pTexture[k];	//b
			pData[j+3] = pTexture[k+3]; //a
		}
	}

	unsigned int size = 0;
	unsigned int twidth  = nWidth;
	unsigned int theight = nHeight;

	//内部分配空间
	unsigned char* pOut  = NULL;
	unsigned int nCompressedTextureSize= ImageOperator::Encode(comp,twidth,theight,pData,&pOut,enrS3TCDXTN,true);

	unsigned char* pOutZip  = new unsigned char[nCompressedTextureSize];

	m_nSize = nCompressedTextureSize;
	ToolsEngine::Zip(pOutZip,nCompressedTextureSize,pOut,nCompressedTextureSize);

	fStream<<nWidth;
	fStream<<nHeight;
	fStream<<m_enFormat;
	fStream<<m_nSize;
	fStream<<nCompressedTextureSize;
	fStream.Save(pOutZip,nCompressedTextureSize);

	delete[] pData;
	pData = NULL;

	delete[] pOut;
	pOut = NULL;

	delete[] pOutZip;
	pOutZip = NULL;

	delete[] pTexture;
	pTexture = NULL;

	return true;
}

unsigned int ToolsEngine::NextP2(unsigned int a)
{
	unsigned int rval = 1;
	while (rval < a) rval <<= 1;
	return rval;
}

bool ToolsEngine::Zip(unsigned char *pvDestBuffer, unsigned int &dwDestLen, const unsigned char *pvSrcBuffer, unsigned int dwSrcLen, int nLevel /*= 8*/)
{
	unsigned long ulDestLen = dwDestLen;	//	WhiteBox_Ignore
	// 这里搞得这么麻烦，是因为compress2的第二个参数是一个指向unsigned long型的指针，需要类型完全匹配，
	// uInt实际上就是SmUint32(unsigned int)，而unsigned long在64位系统（如Solaris）上是64位的，就出问题了
	//return compress2(pvDestBuffer, &dwDestLen, pvSrcBuffer, dwSrcLen, nLevel) == Z_OK;
	bool bResult = false;
	int nResult = compress2(pvDestBuffer, &ulDestLen, pvSrcBuffer, dwSrcLen, nLevel);
	dwDestLen = (unsigned int )ulDestLen;

	// 防错处理
	switch (nResult)
	{
	case Z_OK:
		bResult = true;
		break;
	case Z_MEM_ERROR:
		cout << "内存分配不足" << endl;
		break;
	case Z_BUF_ERROR:
		cout << "输出缓存没有足够的空间" << endl;
		break;
	case Z_STREAM_ERROR:
		cout << "压缩文件参数非法" << endl;
		break;
	case Z_DATA_ERROR:
		//数据或者密码错误 //调换顺序，解决桌面打开工作空间未弹出输密码的对话框之前，先报错“数据错误”的问题 DFAB-1100		
		cout << "数据错误" << endl;
		break;
	default:
		//不可识别的错误，
		cout << "不可识别的错误！" << endl;
		break;
	}
	return bResult;
}
