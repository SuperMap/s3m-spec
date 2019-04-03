// OgdcRasterBlock.h: interface for the OgdcRasterBlock class.
//
//////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)            
//                                                           
//!  \file OgdcRasterBlock.h
//!  \brief 栅格块
//!  \attention 
//!   Copyright(c) 2007-2012  SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OGDCRASTERBLOCK_H__A17D0A45_99AE_4A52_94AD_DFC0C2727811__INCLUDED_)
#define AFX_OGDCRASTERBLOCK_H__A17D0A45_99AE_4A52_94AD_DFC0C2727811__INCLUDED_

#pragma warning (disable:4244)

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Element/OgdcRasterScanline.h"
#include "Base/OgdcRect2D.h"
#include "Base/OgdcColorset.h"


namespace OGDC {
//!  \brief 栅格块类
class ELEMENT_API OgdcRasterBlock
{
public:
	//! \brief 默认构造函数
	OgdcRasterBlock();
	//! \brief 默认析构函数
	virtual ~OgdcRasterBlock();
	
public:
	//! \brief 创建一个栅格块
    //! \param nPixelFormat   像素格式
	//! \param nBlockRowIndex 所在行索引
	//! \param nBlockColIndex 所在列索引
	//! \param nPixelRowIndex 开始的行数(像素单位)
	//! \param nPixelColIndex 开始的列数(像素单位)
	//! \param nWidth		  宽度
	//! \param nHeight		  高度
	//! \param nValidWidth	  有效宽度
	//! \param nValidHeight	  有效高度
	//! \param pColorset	  颜色
	//! \param nWidthBytes 字节宽度
	void Create(PixelFormat nPixelFormat, 
					OgdcInt nBlockRowIndex, 
					OgdcInt nBlockColIndex, 
					OgdcInt nPixelRowIndex,
					OgdcInt nPixelColIndex,
					OgdcInt nWidth,
					OgdcInt nHeight,
					OgdcInt nValidWidth,
					OgdcInt nValidHeight,
					OgdcColorset * pColorset,
					OgdcBool bInited,
					OgdcInt nWidthBytes);

	//! \brief 将红蓝的BYTE顺序交换
	void SwapBlueRed();

	//! \brief  获取字节宽度
	OgdcInt GetWidthBytes();

	//! \brief  是否修改。
	OgdcBool IsModified() const;
	//! \brief 设置修改标记
	void SetModifiedFlag(OgdcBool bFlag = TRUE);

	//! \brief  清空数据，释放内存
	//! \param bIsClearData 是否清空数据
	void Empty(OgdcBool bIsClearData = TRUE);

	//! \brief  是否有效
	OgdcBool IsValid() const;
	
	//! \brief  获取设置内存大小。
	OgdcInt GetByteSize() const;
	//! \brief 设置内存大小
	//! \param nNewSize 新的内存大小
	void SetByteSize(const OgdcInt nNewSize);
			
	//! \brief  获取字节数据。
	const OgdcByte* GetBlockData() const;

private:
	//! \brief  设置像素颜色
	//! \param x		x坐标值
	//! \param y		y坐标值
	//! \param nPixel	像素颜色值
	OgdcBool SetPixel(OgdcInt x,OgdcInt y,OgdcColor nPixel);
	
	//! \brief  获取像素颜色
	//! \param x		x坐标值
	//! \param y		y坐标值
	OgdcColor GetPixel(OgdcInt x,OgdcInt y);
	
public:
	//! \brief  设置像素值
	//! \param x		x坐标值
	//! \param y		y坐标值
	//! \param dValue	像素值
	OgdcBool SetValue(OgdcInt x,OgdcInt y,OgdcDouble dValue);
	
	//! \brief  获取像素值
	//! \param x		x坐标值
	//! \param y		y坐标值
	OgdcDouble GetValue(OgdcInt x,OgdcInt y);
	
	//! \brief  获取指定行索引的扫描线
	//! \param nRowIndex 行索引值[in]
	//! \param ScanLine  获取的扫描线[out]
	//! \param nStartPosInSL 传出的扫描线中的位置[in]
	//! \param nStartPosInBlk 被复制的数据在块中的起始位置[in]
	OgdcBool GetScanline(OgdcInt nRowIndex,OgdcRasterScanline& ScanLine, 
			OgdcInt nStartPosInSL,OgdcInt nStartPosInBlk = 0 );
	
	//! \brief  通过行列索引设置扫描线
	//! \param nRowIndex 行索引值[in]
	//! \param ScanLine  获取的扫描线[out]
	//! \param nStartPosInSL 传入的扫描线的起始位置[in]
	//! \param nStartPosInBlk 被设置的数据在块中的起始位置[in]
	OgdcBool SetScanline(OgdcInt nRowIndex,	const OgdcRasterScanline& scanline, 
			OgdcInt nStartPosInSL, OgdcInt nStartPosInBlk = 0 );

	//! \brief  初始化	
	OgdcBool Init();

	//! \brief  Image的Block使用最大值或最小值初始化
	OgdcBool InitImage(OgdcInt nValue);

	//! \brief  Grid的Block使用NoValue初始化
	OgdcBool InitGrid(OgdcInt nNoValue);
	//! \brief  计算栅格块极值，指定的无值不参与运算
	//! \param dMaxValue 最大值[out]
	//! \param dMinValue 最小值[out]
	//! \param nNoValue 无值[in]
	OgdcBool CalcuExtremum(OgdcDouble& dMaxValue, OgdcDouble& dMinValue, OgdcDouble nNoValue = OGDC_NOVALUE);
	
	//! \brief  计算栅格块极值，还原block块的每个栅格值，无值也是作为一个有效的栅格值参与运算
	//! \param dMaxValue 最大值[out]
	//! \param dMinValue 最小值[out]
	OgdcBool CalcuExtremumWithNoValue(OgdcDouble& dMaxValue, OgdcDouble& dMinValue);

	//! \brief  获取栅格块最小值
	//! \param nNoValue 无值[in]
	OgdcDouble GetMinValue(OgdcDouble nNoValue = OGDC_NOVALUE);
	//! \brief  获取栅格块最大值
	//! \param nNoValue 无值[in]
	OgdcDouble GetMaxValue(OgdcDouble nNoValue = OGDC_NOVALUE);

	//! \brief Block的版本号,配合版本管理时使用
	OgdcInt GetVersionID() { return m_nVersionID; }
	void SetVersionID(OgdcInt nVersionID) { m_nVersionID = nVersionID; }
	
	//{{管理引用基数,在使用ImgBlockRef操作ImgBlock时,记录当前ImgBlock的被引用次数, 
	//! \brief 增加引用基数
	//! \attention 请勿在外部使用该方法,否则会导致内存泄漏
	inline void AddRefCount(){ m_nRefCount++; }

	//! \brief 在使用ImgBlockRef操作ImgBlock时,记录当前ImgBlock的被引用次数, 减少引用基数
	inline void ReduceRefCount() { m_nRefCount--; }

	//! \brief 引用基数归0
	inline void ResetRefCount() { m_nRefCount = 0; }

	//! \brief 获取当前引用基数
	inline OgdcInt GetRefCount() { return m_nRefCount; }
	//}}
protected:
	//! \brief 获取像素颜色值（像素格式：1位，单色）
	//! \param x	x坐标值
	//! \param y	y坐标值
	OgdcColor GetPixel1(OgdcInt x, OgdcInt y);
	//! \brief 获取像素颜色值（像素格式：4位，16色）
	//! \param x	x坐标值
	//! \param y	y坐标值
	OgdcColor GetPixel4(OgdcInt x, OgdcInt y);
	//! \brief 获取像素颜色值（像素格式：8位，256色）
	//! \param x	x坐标值
	//! \param y	y坐标值
	OgdcColor GetPixel8(OgdcInt x, OgdcInt y);
	//! \brief 获取像素颜色值（像素格式：8位，256色）
	//! \param x	x坐标值
	//! \param y	y坐标值
	OgdcColor GetPixelU8(OgdcInt x, OgdcInt y);
	//! \brief 获取像素颜色值（像素格式：16位，彩色）
	//! \param x	x坐标值
	//! \param y	y坐标值
	OgdcColor GetPixel16(OgdcInt x, OgdcInt y);
	//! \brief 获取像素颜色值（像素格式：16位无符号，彩色）
	//! \param x	x坐标值
	//! \param y	y坐标值
	OgdcColor GetPixelU16(OgdcInt x, OgdcInt y);
	//! \brief 获取像素颜色值（像素格式：24位，真彩色）
	//! \param x	x坐标值
	//! \param y	y坐标值
	OgdcColor GetPixel24(OgdcInt x, OgdcInt y);
	//! \brief 获取像素颜色值（像素格式：32位，增强真彩色）
	//! \param x	x坐标值
	//! \param y	y坐标值
	OgdcColor GetPixel32(OgdcInt x, OgdcInt y);
	//! \brief 获取像素颜色值（像素格式：32位长整型）
	//! \param x	x坐标值
	//! \param y	y坐标值
	OgdcColor GetPixel32UGint(OgdcInt x, OgdcInt y);
    //! \brief 获取像素颜色值（像素格式：64位Doouble）
	//! \param x	x坐标值
	//! \param y	y坐标值
	OgdcColor GetPixelDouble(OgdcInt x, OgdcInt y);
	//! \brief 设置像素颜色值（像素格式：1位，单色）
	//! \param x	x坐标值
	//! \param y	y坐标值
	//! \param color 颜色值
	OgdcBool SetPixel1(OgdcInt x, OgdcInt y, OgdcColor color);
	//! \brief 设置像素颜色值（像素格式：4位，16色）
	//! \param x	x坐标值
	//! \param y	y坐标值
	//! \param color 颜色值
	OgdcBool SetPixel4(OgdcInt x, OgdcInt y, OgdcColor color);
	//! \brief 设置像素颜色值（像素格式：8位，256色）
	//! \param x	x坐标值
	//! \param y	y坐标值
	//! \param color 颜色值
	OgdcBool SetPixel8(OgdcInt x, OgdcInt y, OgdcColor color);
	//! \brief 设置像素颜色值（像素格式：8位，256色）
	//! \param x	x坐标值
	//! \param y	y坐标值
	//! \param color 颜色值
	OgdcBool SetPixelU8(OgdcInt x, OgdcInt y, OgdcColor color);
	//! \brief 设置像素颜色值（像素格式：16位，彩色）
	//! \param x	x坐标值
	//! \param y	y坐标值
	//! \param color 颜色值
	OgdcBool SetPixel16(OgdcInt x, OgdcInt y, OgdcColor color);
	//! \brief 设置像素颜色值（像素格式：16位无符号，彩色）
	//! \param x	x坐标值
	//! \param y	y坐标值
	//! \param color 颜色值
	OgdcBool SetPixelU16(OgdcInt x, OgdcInt y, OgdcColor color);
	//! \brief 设置像素颜色值（像素格式：24位，真彩色）
	//! \param x	x坐标值
	//! \param y	y坐标值
	//! \param color 颜色值
	OgdcBool SetPixel24(OgdcInt x, OgdcInt y, OgdcColor color);
	//! \brief 设置像素颜色值（像素格式：32位，增强真彩色）
	//! \param x	x坐标值
	//! \param y	y坐标值
	//! \param color 颜色值
	OgdcBool SetPixel32(OgdcInt x, OgdcInt y, OgdcColor color);
	//! \brief 设置像素颜色值（像素格式：32位长整型）
	//! \param x	x坐标值
	//! \param y	y坐标值
	//! \param color 颜色值
	OgdcBool SetPixel32UGint(OgdcInt x, OgdcInt y, OgdcColor color);	
	//! \brief 设置像素颜色值（像素格式：64位Doouble)
	//! \param x	x坐标值
	//! \param y	y坐标值
	//! \param color 颜色值
	OgdcBool SetPixelDouble(OgdcInt x, OgdcInt y, OgdcColor color);	


public:
	//! \brief  像素的格式
	PixelFormat m_nPixelFormat;

	//! \brief  所处的行索引
	OgdcInt m_nBlockRowIndex;

	//! \brief  所处的列索引
	OgdcInt m_nBlockColIndex;

	//! \brief  开始的列数(像素单位)
	OgdcInt m_nPixelColIndex;
	
	//! \brief  开始的行数(像素单位)
	OgdcInt m_nPixelRowIndex;

	//! \brief  宽度
	OgdcInt m_nWidth;

	//! \brief  高度
	OgdcInt m_nHeight;

	//! \brief  深度
	OgdcInt m_nDepth;

	//! \brief  字节宽度
	OgdcInt m_nWidthBytes;

	//! \brief  有效宽度
	OgdcInt m_nValidWidth;

	//! \brief  有效高度
	OgdcInt m_nValidHeight;

	//! \brief  地理范围
	OgdcRect2D m_rcBounds;

	//! \brief  最小值
	OgdcDouble m_dMinValue;

	//! \brief  最大值
	OgdcDouble m_dMaxValue;

	//! \brief  标记最大、最小值是否需要重新计算
	OgdcBool m_bIsExtremumDirty;	

	//! \brief  调色板，只是一个引用，不需要释放。
	OgdcColorset* m_pColorset;	

	//! \brief  是否已经初始化
	OgdcBool m_bInited;	

	//! \brief  标记该块是从数据集中获取，还是新添加的。
	OgdcBool m_bNewBlock;
	
	// 指向空闲队列中的前一块
	OgdcRasterBlock* m_pLruPrev;

	// 指向空闲队列中的后一块
	OgdcRasterBlock* m_pLruNext;

	// 指向哈希表中的下一块
	OgdcRasterBlock* m_pNext;

private:
	//! \brief  像素数组
	OgdcArray<OgdcUchar> m_data;

	//! \brief  像素数组头指针
	OgdcByte* m_lpBits;

	//! \brief  判断是否修改
	OgdcBool m_bModified;    
	OgdcInt m_nVersionID;
	//! \brief 数据块引用基数
	OgdcInt m_nRefCount;

  };

}

#endif // !defined(AFX_OGDCRASTERBLOCK_H__A17D0A45_99AE_4A52_94AD_DFC0C2727811__INCLUDED_)

