//////////////////////////////////////////////////////////////////////////
//                    _   _   ____   ____                    
//   Project         | | | | / ___) / ___)                   
//                   | | | || | __ | |                       
//                   | |_| || |_\ || \___                    
//                    \____/ \____| \ ___)     6.0           
//                                                           
//!  \file headerfile.h
//!  \brief 纹理数据的封装类。
//!  \details 完成对纹理数据的封装。
//!  \author 。
//!  \attention 
//!   Copyright (c) 1996-2010 SuperMap Software Co., Ltd. <br> 
//!   All Rights Reserved.
//!  \version 6.0
//////////////////////////////////////////////////////////////////////////


#if !defined(AFX_UGTEXTUREDATA_H__208EFE4E_3502_4F4D_9705_1198D58E48C1__INCLUDED_)
#define AFX_UGTEXTUREDATA_H__208EFE4E_3502_4F4D_9705_1198D58E48C1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Base3D/UGPrerequisites3D.h"
#include "Toolkit/UGTextureInfo.h"
#include "Stream/UGStream.h"

namespace UGC
{
//! \brief 纹理数据的封装类。
class BASE3D_API UGTextureData
{
public:
	//! \brief 构造函数
	UGTextureData();
	//! \brief 拷贝构造函数
	UGTextureData(const UGTextureData& other);
	//! \brief 拷贝构造函数
	UGTextureData(const UGTextureInfo& textureInfo);
	//! \brief 赋值函数
	UGTextureData& operator=(const UGTextureData& other);

	//! \brief 克隆自己的数据
	virtual UGTextureData* Clone();
	//! \brief 析构函数
	virtual ~UGTextureData();
	//! \brief 释放纹理数据
	void Release();

	//! \brief 从流文件中加载
	UGbool Load(UGStream& fStream ,UGuint eCodecType = 0);

	//! \brief 存入流文件中
	void Save(UGStream& fStream,UGuint eCodecType = 0);

private:
	//! \brief 存入流文件中
	//! \remarks 实现纹理的DDS编码及zip压缩
	UGbool BuildTextureTier(UGStream& fStream);
#if defined OPENGL_ES_VERSION2
	//! \brief 压缩类型由DXT转抽象为NONE
	void CompressTypeDXTToNONE();
#endif
public:
	//! \brief 纹理数据内存
	UGuchar* m_pBuffer;
	//! \brief 纹理的高
	UGuint m_nHeight;
	//! \brief 纹理的宽
	UGuint m_nWidth;
	//! \brief 纹理的深度
	UGuint m_nDepth;
	//! \brief 纹理的像素格式，目前只支持RGBA
	UG3DPixelFormat m_enFormat;

	// 压缩纹理类型
	UGuint m_CompressType;

	// 数据的内存大小
	UGuint m_nSize;
};

class BASE3D_API UGTextureDataAnimation:public UGTextureData
{
public:
	// 初始化函数
	UGTextureDataAnimation(std::vector<UGTextureData> textureDataTable,std::vector<UGuint>timeTable);
	

	// 根据索引获取该帧的持续时间
	UGuint GetFrameTimeByIndex(UGint nIndex);

	// 根据索引获取该帧的纹理数据
	UGTextureData GetFrameDataByIndex(UGint nIndex);

	virtual UGTextureData* Clone();
	
	// 获取动画纹理的总帧数
	UGint GetNumFrame();
private:
	std::vector<UGuint> m_timeTable;
	std::vector<UGTextureData> m_textureDataTable;
};
}

#endif // !defined(AFX_UGTEXTUREDATA_H__208EFE4E_3502_4F4D_9705_1198D58E48C1__INCLUDED_)

