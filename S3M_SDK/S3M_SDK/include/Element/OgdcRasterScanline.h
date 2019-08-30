// OgdcRasterScanline.h: interface for the OgdcRasterScanline class.
//
//////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)                
//                                                           
//!  \file OgdcRasterScanline.h
//!  \brief 栅格行
//!  \attention 
//!   Copyright(c) 2007-2012  SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OGDCRASTERSCANLINE_H__0F996C74_D952_4E97_BBF1_D9453140A029__INCLUDED_)
#define AFX_OGDCRASTERSCANLINE_H__0F996C74_D952_4E97_BBF1_D9453140A029__INCLUDED_

#pragma warning (disable:4244)

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Base/OgdcArray.h"

#define OGDC_NOVALUE -9999 

namespace OGDC {

//!  \brief 栅格扫描行类
class ELEMENT_API OgdcRasterScanline:public OgdcArray<OgdcUchar>
{
public:
	//! \brief 默认构造函数
	OgdcRasterScanline();
	//! \brief 默认析构函数
	~OgdcRasterScanline();

public:

	//! \brief 得到一个像素的位数。
	//! \param pf 像素格式
	static OgdcInt GetBitCount(PixelFormat pf); 

	//! \brief 将红蓝的BYTE顺序交换
	void SwapBlueRed();	
	
	//! \brief 将左右的BYTE顺序交换
	void SwapLeftRight();
	
	//! \brief 设置像素
	//! \param x 像素位置
	//! \param nPixel 像素值
	OgdcBool SetPixel(OgdcInt x, OgdcInt nPixel);
	
	//! \brief 获取像素
	//! \param x 像素位置
	OgdcInt GetPixel(OgdcInt x);
	
	//! \brief 设置像素值
	//! \param x 像素位置
	//! \param dValue 像素值
	OgdcBool SetValue(OgdcInt x, OgdcDouble dValue);
	
	//! \brief 获取像素值
	//! \param x 像素位置
	OgdcDouble GetValue(OgdcInt x);

	//! \brief 获取像素格式
	PixelFormat GetPixelFormat() const;

	//! \brief 设置像素格式
	//! \param nPixelFormat 像素格式
	void SetPixelFormat(PixelFormat nPixelFormat);

	//! \brief 获取扫描线所占字节数
	OgdcInt GetWidthBytes() const;

	//! \brief 设置扫描线所占字节数
	//! \param nNewSize 字节数
	void SetWidthBytes(const OgdcInt nNewSize);

	//! \brief 获取扫描线宽度
	OgdcInt GetWidth() const;

	//! \brief 设置扫描线宽度
	//! \param nWidth 扫描线宽度
	void SetWidth(OgdcInt nWidth);
	
	//! \brief  计算扫描线极值
	//! \param dMaxValue 最大值[out]
	//! \param dMinValue 最小值[out]
	//! \param nNoValue  无值[in]
	OgdcBool CalcuExtremum(OgdcDouble& dMaxValue, OgdcDouble& dMinValue, OgdcInt nNoValue = OGDC_NOVALUE);
		
private:
	//! \brief 象素格式
	PixelFormat m_nPixelFormat;

	//! \brief 扫描线宽度
	OgdcInt m_nWidth;

};

}

#endif // !defined(AFX_OGDCRASTERSCANLINE_H__0F996C74_D952_4E97_BBF1_D9453140A029__INCLUDED_)

