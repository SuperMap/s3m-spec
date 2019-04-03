//////////////////////////////////////////////////////////////////////////
//                    _   _   ____   ____                    
//   Project         | | | | / ___) / ___)                   
//                   | | | || | __ | |                       
//                   | |_| || |_\ || \___                    
//                    \____/ \____| \ ___)     6.0           
//                                                           
//!  \file headerfile.h
//!  \brief 三维颜色类。
//!  \details 封装了三维颜色类。
//!  \author 。
//!  \attention 
//!   Copyright (c) 1996-2010 SuperMap Software Co., Ltd. <br> 
//!   All Rights Reserved.
//!  \version 6.0
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_UGCOLORVALUE3DEX_H__77F37090_FF08_46BD_9EEB_312314BBBD47__INCLUDED_)
#define AFX_UGCOLORVALUE3DEX_H__77F37090_FF08_46BD_9EEB_312314BBBD47__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Stream/ugdefs.h"


namespace  UGC 
{

#define UGGetAValue(rgb)      ((UGbyte)((rgb)>>24))
#define UGGetRValue(rgb)      ((UGbyte)(rgb))
#define UGGetGValue(rgb)      ((UGbyte)(((UGushort)(rgb)) >> 8))
#define UGGetBValue(rgb)      ((UGbyte)((rgb)>>16))

typedef UGuint RGBA;
typedef UGuint ARGB;
typedef UGuint ABGR;

//! \brief 三维颜色类。
class TOOLKIT_API UGColorValue3D  
{
public:
	//! \brief 静态变量黑色。
	static UGColorValue3D Black;
	//! \brief 静态变量黑色
	static UGColorValue3D White;
	//! \brief 静态变量黑色
	static UGColorValue3D Red;
	//! \brief 静态变量黑色
	static UGColorValue3D Green;
	//! \brief 静态变量黑色
	static UGColorValue3D Blue;
	//! \brief 静态变量黑色
	static UGColorValue3D Yellow;
	//! \brief 三维颜色类的构造函数。
	//! \param color 32位的颜色变量[in]。
	UGColorValue3D(UGuint color);	
	//! \brief 三维颜色类的构造函数。
	//! \param red 红色分量，范围0-1[in]。
	//! \param green 红色分量，范围0-1[in]。
	//! \param blue  蓝色分量，范围0-1[in]。
	//! \param alpha 不透明分量，范围0-1[in]。
	UGColorValue3D( UGdouble red = 1.0f,
				    UGdouble green = 1.0f,
					UGdouble blue = 1.0f,
					UGdouble alpha = 1.0f )	: r(red), g(green), b(blue), a(alpha)
	{ }		
	//! \brief 重载等值判断运算符。
	//! \param rhs [in]。
	//! \return 是否相等。
	UGbool operator==(const UGColorValue3D& rhs) const;	
	//! \brief 重载不等值判断运算符。
	//! \param rhs [in]。
	//! \return 是否不等。
	UGbool operator!=(const UGColorValue3D& rhs) const;	
	//! \brief 红色分量，范围0-1。
	UGdouble r;
	//! \brief 绿色分量，范围0-1。
	UGdouble g;
	//! \brief 蓝色分量，范围0-1。
	UGdouble b;
	//! \brief 不透明分量，范围0-1。
	UGdouble a;
	//! \brief 转换为以RGBA32位整型表示的颜色。
	//! \return RGBA，32位整型表示的颜色。
	RGBA GetAsLongRGBA(void) const;	
	//! \brief 得到现有颜色的反色。
	//! \return 现有颜色的反色。
	UGColorValue3D GetReverseColor() const;
	//! \brief 传入R、G、B、A分量构建三维颜色 。
	//! \param nRed 红色分量，范围0-255[in]。
	//! \param nGreen 绿色分量，范围0-255[in]。
	//! \param nBlue 蓝色分量，范围0-255[in]。
	//! \param nAlpha 不透明度分量，范围0-255[in]。
	void SetValue(UGint nRed,UGint nGreen,UGint nBlue,UGint nAlpha);	
	//! \brief 传入R、G、B、A分量构建三维颜色。
	//! \param dRed 红色分量，范围0-1.0[in]。
	//! \param dGreen 绿色分量，范围0-1[in]。
	//! \param dBlue 蓝色分量，范围0-1[in]。
	//! \param dAlpha 不透明度分量，范围0-1[in]。
	void SetValue(UGdouble dRed,UGdouble dGreen,UGdouble dBlue,UGdouble dAlpha);	
	//! \brief  将UGColor转换为UGColorValue3D
	void SetValue(UGColor color);
	//! \brief  将UGColorValue3D转换为UGColor
	UGColor GetValue() const;
	//! \brief 得到R、G、B、A分量颜色 。
	//! \param nRed 红色分量，范围0-255[out]。
	//! \param nGreen 绿色分量，范围0-255[out]。
	//! \param nBlue 蓝色分量，范围0-255[out]。
	//! \param nAlpha 不透明度分量，范围0-255[out]。
	void GetRGBA(UGint& nRed,UGint& nGreen,UGint& nBlue,UGint& nAlpha) const; 
	//! \brief 转换为以ARGB 32位整型表示的颜色。
	//! \return ARGB，32位整型表示的颜色。
	ARGB GetAsLongARGB(void) const;
	//! \brief 转换为以ABGR 32位整型表示的颜色。
	//! \return ABGR，32位整型表示的颜色。
	ABGR GetAsLongABGR(void) const;
	//! \brief 转换为以ABGR 32位整型表示的颜色。
	//! \return ABGR，32位整型表示的颜色。
	ABGR GetAsCOLORREF(void)const;
	//! \brief 转换为以ABGR 32位整型表示的颜色。
	//! \return ABGR，32位整型表示的颜色。
	void FromAsABGR(UGint color);
	//! \brief 转换为以UGColor 32位整型表示的颜色。
	//! \return UGColor，32位整型表示的颜色。
	static UGColor ABGRToColor(UGint color);


};

}
#endif // !defined(AFX_UGCOLORVALUE3DEX_H__77F37090_FF08_46BD_9EEB_312314BBBD47__INCLUDED_)


