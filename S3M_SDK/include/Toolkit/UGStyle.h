
/*!
   **************************************************************************************
    \file     UGStyle.h 
   **************************************************************************************
    \author   陈国雄
    \brief    几何风格类                                                             
    \attention   
   ----------------------------------------------------------------------------------<br>
    Copyright (c) 2000-2010 SuperMap Software Co., Ltd.                           <br>
    All Rights Reserved.                                                             <br>
   ----------------------------------------------------------------------------------<br> 
   
   **************************************************************************************
    \version 2005-07-06  陈国雄    初始化版本.                                       <br>
   **************************************************************************************
*/

/*	修改记录 1
修改人：陆国伟
修改原因：二三维风格统一，对外使用方式基本不变
具体修改：
1新增SytleEx成员，用来负责三维风格的存储
2新增三维相关接口
3二三维中目前接口合并,此3个属性默认值以二维的为标准，共3*2个,分别为SetMarkerStyle、SetLineWidth、SetLineStyle
4宽度属性保存为int，外部接口为double，精度保证在小数点1，即0.1
5调整接口，颜色统一为UGColor，替换三维以前的UGColorValue3D
6兼容XML：新读新，新读旧，旧读旧，但不支持旧读新
7二进制兼容，还未处理，等以后CAD保存需要时再处理，避免兼容性问题
修改时间：2011-1-6
*/

#ifndef UGSTYLE_H
#define UGSTYLE_H

#include "Stream/ugdefs.h"
#include "Stream/UGMemoryStream.h"

#include "Toolkit/UGStyleEx.h"
#include "Toolkit/UGColorValue3DEx.h"

namespace UGC {

	  //!  \brief Toolkit
typedef enum tagBrushGradientMode
{
	  //!  \brief Toolkit
	scbGradientNone	= 0,
	  //!  \brief Toolkit
	scbGradientLinear	= 1,	
	  //!  \brief Toolkit
	scbGradientRadial	= 2,
	  //!  \brief Toolkit
	scbGradientConical	= 3,
	  //!  \brief Toolkit
	scbGradientSquare	= 4,
		
}UGBrushGradientMode;


//!  \brief Toolkit
class UGStream;

	  //!  \brief Toolkit
class TOOLKIT_API UGStyle  
{
public:
	//! \brief
	  //!  \brief Toolkit
	UGStyle();
	//! \brief
	  //!  \brief Toolkit
	UGStyle(const UGStyle& style);
	//! \brief
	  //!  \brief Toolkit
	UGStyle& operator = (const UGStyle &style);
	//! \brief
	  //!  \brief Toolkit
	UGbool operator == (const UGStyle &style) const;
	//! \brief
	  //!  \brief Toolkit
	UGbool operator != (const UGStyle &style) const;
	//! \brief
	  //!  \brief Toolkit
	~UGStyle();

public:	
	//! \brief 得到存储需要的size
	static UGint GetStoreSize();
	//! \brief
	  //!  \brief Toolkit
	//! \remarks 旧格式保存的是RGB，现在需要保存RGBA，增加对Alpha位的信息吧，在String读取中会考虑旧的兼容性
	static UGString ConvertRGBColor(UGColor crRGB);
	//! \brief
	  //!  \brief Toolkit
	static UGColor ConvertRGBColor(const UGString& strRGB);
	//! \brief
	  //!  \brief Toolkit
	  //!  \brief Toolkit
	static UGbool IsSameFillStyle(const UGStyle &style1, const UGStyle &style2);
	//! \brief
	  //!  \brief Toolkit
	static UGbool IsSameLineStyle(const UGStyle &style1, const UGStyle &style2);
	//! \brief
	  //!  \brief Toolkit
	static UGbool IsSameMarkerStyle(const UGStyle &style1, const UGStyle &style2);
	//! \brief
	  //!  \brief Toolkit
	static UGbool IsSameMarkerSymbolStyle(const UGStyle &style1, const UGStyle &style2);
	//! \brief
	  //!  \brief Toolkit
	static UGbool IsSameLineSymbolStyle(const UGStyle &style1, const UGStyle &style2);
	//! \brief
	  //!  \brief Toolkit
	static UGbool IsSameFillSymbolStyle(const UGStyle &style1, const UGStyle &style2);

	//! \brief
	//!  \brief Toolkit
	static UGbool IsSame3DSymbolStyle(const UGStyle &style1, const UGStyle &style2);

	//! \brief
	  //!  \brief Toolkit
	static UGbool IsSameDimensionStyle(const UGStyle &style1, const UGStyle &style2, UGint nDimension);

	  //!  \brief Toolkit
	//! \brief 16进制字符串转换为对应的Byte形式的Buffer
	//! \param pBuffer		转换成的字节串[in/out]
	  //!  \brief Toolkit
	//! \param nLength      字节Buffer长度[in]
	//! \param strValue     要转换的字符[in]
	//! \return 转换是否成功
	//! \remarks 在保存独立符号到xml中时，需要将独立符号的二进制流保存在xml中，此两个函数的作用主要就是将二进制流转换为对应的16进制格式的字符串来进行保存，同时提供对应的将xml中保存的16进制格式的字符串解析成二进制流的格式的函数
	static UGbool StringConver2Byte(UGbyte*& pBuffer,const UGint& nLength,UGString strValue);
	//! \brief Byte形式的Buffer转换为对应的16进制字符串
	//! \param pBuffer		转换成的字节串[in]
	//! \param nLength      字节Buffer长度[in]
	//! \param strResult     要转换的字符[in/out]
	//! \return 转换是否成功
	static UGbool ByteConver2String(const UGbyte* pBuffer,UGint nLength,UGString& strResult);

	//! \brief
	  //!  \brief Toolkit
	UGbool ToOgdcStyle(OgdcStyle& ogdcStyle) const; 
	//! \brief
	  //!  \brief Toolkit
	UGbool FromOgdcStyle(const OgdcStyle& ogdcStyle);

	//! \brief 填充背景色
	UGColor GetFillBackColor() const;
	//! \brief
	void SetFillBackColor(UGColor color);
	
	//! \brief 填充前景色
	UGColor GetFillForeColor() const;
	//! \brief
	void SetFillForeColor(UGColor color);
	
	//! \brief 填充风格
	UGint GetFillStyle() const;
	//! \brief
	void SetFillStyle(UGint nFillStyle);
	
	//! \brief 线型颜色
	UGColor GetLineColor() const;
	//! \brief
	void SetLineColor(UGColor color);

	//! \brief 线型类型
	UGint GetLineStyle() const;
	void SetLineStyle(UGint nLineStyle);
	
	//! \brief 线型宽度
	//! \remarks 二维中线宽单位为毫米，三维中单位为像素，精度保留小数点一位0.1
	UGdouble GetLineWidth() const;
	void SetLineWidth(UGdouble dLineWidth);
	
	//! \brief 符号角度
	UGdouble GetMarkerAngle() const;
	//! \brief
	void SetMarkerAngle(UGdouble dMarkerAngle);

	//! \brief 符号大小
	UGdouble GetMarkerSize() const;

	//! \brief 目前类库中有三个变量表示符号大小，该属性语意发生变化，如果设置他者三个大小值都相同
	void SetMarkerSize(UGdouble dMarkerSize);

	//! \brief 符号风格
	UGint GetMarkerStyle() const;
	//! \brief
	void SetMarkerStyle(UGint nMarkerStyle);

	//! \brief 填充背景不透明
	UGbool GetFillBackOpaque() const;
	//! \brief
	void SetFillBackOpaque(UGbool bOpaque);

	//! \brief 填充不透明度
	UGbyte GetFillOpaqueRate() const;
	//! \brief
	void SetFillOpaqueRate(UGbyte btFillOpaqueRate);

	//! \brief 填充渐变类型
	UGbyte GetFillGradientType() const;
	//! \brief
	void SetFillGradientType(UGbyte btGradientType);

	//! \brief 填充角度
	UGdouble GetFillAngle() const;
	//! \brief
	void SetFillAngle(UGdouble dFillAngle);

	//! \brief 填充渐变中心X方向偏移
	UGshort GetFillCenterOffsetX() const;
	//! \brief
	void SetFillCenterOffsetX(UGshort nFillCenterOffsetX);

	//! \brief 填充渐变中心Y方向偏移
	UGshort GetFillCenterOffsetY() const;
	//! \brief
	void SetFillCenterOffsetY(UGshort nFillCenterOffsetY);

	//! \brief Load Style
	//! \remarks 可以读取 30 和 50 的风格
	UGbool LoadAll(UGStream& stream, UGint nStyleSize);
	//! \brief Save Style
	//! \remarks 只支持50的存储
	//! \return 返回保存的style的size
	void SaveAll(UGStream& stream) const;
	//! \brief
	UGint Load(UGStream& stream, UGint nDimension);
	//! \brief
	void Save(UGStream& stream, UGint nDimension) const;
	//! \brief

	//! \brief
	UGbool FromXML(const UGString& strXML, UGint nVersion = 0);
	//! \brief
	UGString ToXML(UGint nVersion = 0, const UGString& strElemName = (_U(""))) const;
	//! \brief 设置符号的缩放比例系数参照符号本身还是符号的边框(255,255)
	//! \param bSymbolScale 真为符号 假为边框
	void SetScaleBySymbol(UGbool bSymbolScale)
	{
	  //!  \brief Toolkit
		m_bSymbolScale = bSymbolScale;
	}
	  //!  \brief Toolkit
	UGbool IsScaleBySymbol() const
	{
	  //!  \brief Toolkit
		return m_bSymbolScale;
	}

	//! \brief 获取风格对象的hashcode
	UGint GetHashCode() const;

	//! \brief 获取不包括旋转角度的风格对象的hashcode，用于减少符号绘制次数
	// add by gouyu 2012-1-17
	UGint GetHashCodeWithoutAngle() const;

	//! \brief 设置点符号宽度,支持点符号拉伸效果，但对TrueType下的符号不支持此功能
	//! \param nWidth	点符号的宽度[in]
	void SetMarkerWidth(UGdouble dWidth);
	//! \brief 获取点符号宽度
	UGdouble GetMarkerWidth() const;
	//! \brief 设置点符号高度,支持点符号拉伸效果，但对TrueType下的符号不支持此功能
	//! \param nWidth	点符号的高度[in]
	void SetMarkerHeight(UGdouble dHeight);
	//! \brief 获取点符号宽度
	UGdouble GetMarkerHeight() const;

	//! \brief 设置点符号风格，风格可以独立于符号库
	//! \param pMarkerSymbolStream	点符号的Stream流信息[in]
	//! \param nMarkerSymbolVersion 点符号的版本[in]
	//! \return 设置是否成功,失败的话不破坏以前的符号信息
	//! \remarks 对于流信息，用户可以将点符号通过Save的方式封装成为Stream的形式，然后保存在风格中，另外，对于Stream流的释放则由用户自己负责
	UGbool SetMarkSymbolStyle(UGMemoryStream* pMarkerSymbolStream, UGuint nMarkerSymbolVersion);
	//! \brief 设置点符号风格，风格可以独立于符号库
	//! \param pMarkerSymbolBuffer	 点符号Buffer[in]
	//! \param nMarkerSymbolLength   点符号Buffer长度[in]
	//! \param nMarkerSymbolVersion  点符号的版本[in]
	//! \param bOwned				 是否风格类负责该Buffer的释放[in]
	//! \return 设置是否成功,失败的话不破坏以前的符号信息
	//! \remarks 对于Buffer信息，用户可以将点符号封装成为UGbyte的形式，然后保存在风格中，另外，对于传入的Buffer的释放，如果bOwned为true则风格中会负责释放，如果bOwned为false则用户负责，默认为false
	UGbool SetMarkSymbolStyle(UGbyte* pMarkerSymbolBuffer,UGuint nMarkerSymbolLength, UGuint nMarkerSymbolVersion,UGbool bOwned = false);
	//! \brief 获取点符号风格，风格可以独立于符号库
	//! \remarks 获取风格中的点符号信息，用户可以修改信息内容，但是释放还是有风格来负责！
	UGbyte* GetMarkSymbolStyle()const;
	//! \brief 获取点符号字节长度
	UGuint GetMarkerSymbolLength()const;
	//! \brief 获取点符号版本
	UGuint GetMarkerSymbolVersion()const;
	//! \brief 清空点符号风格
	void ClearMarkSymbolStyle();

	//! \brief 设置线符号风格，风格可以独立于符号库
	//! \param pLineSymbolStream		线符号的Stream流信息[in]
	//! \param nLineSymbolVersion	    线符号的版本[in]
	//! \return 设置是否成功,失败的话不破坏以前的符号信息
	//! \remarks 对于流信息，用户可以将线符号通过Save的方式封装成为Stream的形式，然后保存在风格中，另外，对于Stream流的释放则由用户自己负责
	UGbool SetLineSymbolStyle(UGMemoryStream* pLineSymbolStream,UGuint nLineSymbolVersion);
	//! \brief 设置点符号风格，风格可以独立于符号库
	//! \param pLineSymbolBuffer	 线符号Buffer[in]
	//! \param nLineSymbolLength     线符号Buffer长度[in]
	//! \param nLineSymbolVersion    线符号的版本[in]
	//! \param bOwned				 是否风格类负责该Buffer的释放[in]
	//! \return 设置是否成功,失败的话不破坏以前的符号信息
	//! \remarks 对于Buffer信息，用户可以将线符号封装成为UGbyte的形式，然后保存在风格中，另外，对于传入的Buffer的释放，如果bOwned为true则风格中会负责释放，如果bOwned为false则用户负责,默认为false
	UGbool SetLineSymbolStyle(UGbyte* pLineSymbolBuffer,UGuint nLineSymbolLength,UGuint nLineSymbolVersion,UGbool bOwned = false );
	//! \brief 获取线符号风格，风格可以独立于符号库
	//! \remarks 获取风格中的线符号信息，用户可以修改信息内容，但是释放还是有风格来负责！
	UGbyte* GetLineSymbolStyle()const;
	//! \brief 获取线符号字节长度
	UGuint GetLineSymbolLength()const;
	//! \brief 获取线符号版本
	UGuint GetLineSymbolVersion()const;
	//! \brief 清空线符号风格
	void ClearLineSymbolStyle();

	//! \brief 设置填充符号风格，风格可以独立于符号库
	//! \param pFillSymbolStream		填充符号的Stream流信息[in]
	//! \param nFillSymbolVersion	    填充符号的版本[in]
	//! \return 设置是否成功,失败的话不破坏以前的符号信息
	//! \remarks 对于流信息，用户可以将填充符号通过Save的方式封装成为Stream的形式，然后保存在风格中，另外，对于Stream流的释放则由用户自己负责
	UGbool SetFillSymbolStyle(UGMemoryStream* pFillSymbolStream,UGuint nFillSymbolVersion);
	//! \brief 设置填充符号风格，风格可以独立于符号库
	//! \param pFillSymbolBuffer	 填充符号Buffer[in]
	//! \param nFillSymbolLength     填充符号Buffer长度[in]
	//! \param nFillSymbolVersion    填充符号的版本[in]
	//! \param bOwned				 是否风格类负责该Buffer的释放[in]
	//! \return 设置是否成功,失败的话不破坏以前的符号信息
	//! \remarks 对于Buffer信息，用户可以将填充符号封装成为UGbyte的形式，然后保存在风格中，另外，对于传入的Buffer的释放，如果bOwned为true则风格中会负责释放，如果bOwned为false则用户负责,默认为false
	UGbool SetFillSymbolStyle(UGbyte* pFillSymbolBuffer,UGuint nFillSymbolLength,UGuint nFillSymbolVersion,UGbool bOwned = false  );
	//! \brief 获取填充符号风格，风格可以独立于符号库
	//! \remarks 获取风格中的填充符号信息，用户可以修改信息内容，但是释放还是有风格来负责！
	UGbyte* GetFillSymbolStyle()const;
	//! \brief 获取填充符号字节长度
	UGuint GetFillSymbolLength()const;
	//! \brief 获取填充符号版本
	UGuint GetFillSymbolVersion()const;
	//! \brief 清空线符号风格
	void ClearFillSymbolStyle();

	//! \brief 清空符号风格
	void ClearSymbolStyle();

	//! \brief 清空符号风格
	void ClearSymbolStyle3D();

	// fansc Add 2013-2-19
	//! brief 初始化成员变量
	void Init();

	//GeoRegion是否跨越经度180显示
	UGbool  GetAcrossLongtitude();
	void  SetAcrossLongtitude(UGbool IsAcrossLongtitude = FALSE);


	void SetIsVisible(UGbool bVisible);
	UGbool IsVisible() const;//设置动态层图形是否可见，其他图层不使用，不需要存储

public:
	//! \breif 把2DStyle的线面颜色转换到3DStyle中
	void Convert2DTo3DStyle();

	//! \breif 把3DStyle的线面颜色转换到2DStyle中
	void Convert3DTo2DStyle();

	//! \brief 获取符号风格大小，在当前维度下的大小与保存自身记录的四个字节的总和
	UGuint GetStyleSize(UGint nDimension)const;
	

public: //三维风格相关接口

	UGbool Has3DStyle();
	//! \brief 设置填充模式。
	void SetFill3DMode(const UGFill3DMode& Fill3DMode);

	//! \brief 获取填充模式。
	UGFill3DMode GetFill3DMode() const;

	//! \brief 设置依地模式。
	void SetAltitudeMode(const AltitudeMode& Mode);

	//! \brief 获取依地模式。
	AltitudeMode GetAltitudeMode() const;

	//! \brief 设置点地标纹理的名称。
	void SetIconName(const UGString& str);

	//! \brief 获取点地标纹理的名称。
	UGString GetIconName() const;

	//! \brief 设置点地标纹理的缩放比。
	void SetIconScale(const UGfloat& fScale);

	//! \brief 获取点地标纹理的缩放比。
	UGfloat GetIconScale() const;

	//! \brief 设置简单点大小。
	//! \param fsize [in] 简单点大小，单位是OpenGL中的像素点大小。
	void SetPointSize(const UGfloat &fSize);

	//! \brief 返回简单点大小。
	UGfloat GetPointSize() const;

	//! \brief 设置点颜色。
	void SetPointColor(const UGColorValue3D &color);

	//! \brief 返回点颜色。
	UGColorValue3D GetPointColor() const;

	//! \brief 设置线宽度。
	//! \param fsize [in] 线宽度，单位是OpenGL中的像素点大小。
	//void SetLineWidth(const UGfloat &fWidth);

	//! \brief 返回线宽度。
	//UGfloat GetLineWidth() const;

	//! \brief 设置线颜色。
	//! \remarks 供二维专用 
	void SetLineColor3D(const UGColorValue3D &color);

		//! \brief 返回线颜色。
	UGColorValue3D GetLineColor3D() const;

	//! \brief 设置面颜色。
	void SetFillColor(const UGColorValue3D &color); 

	//! \brief 返回面颜色
	//! \remarks 内部和GetFillForeColor作用一致，用于二三维中填充颜色
	UGColorValue3D GetFillColor() const;

	//! \brief 设置底部高度。
	//! \param str [in] 高度值单位米。
	void SetBottomAltitude(const UGfloat &fvalue);

	//! \brief 返回底部高度。
	UGfloat GetBottomAltitude() const;

	//! \brief 设置拉伸高度。
	//! \param str [in] 高度值单位米。
	void SetExtendHeight(const UGfloat &fvalue);

	//! \brief 返回拉伸高度。
	UGfloat GetExtendHeight() const;

	//! \brief 设置线拉伸面的纹理。
	//! \param str [in] 侧面纹理路径。
	void SetSideTexture(const UGString &str);

	//! \brief 返回线拉伸侧面纹理。
	UGString GetSideTexture() const;

	//! \brief 设置顶面的纹理。
	//! \param str [in] 顶面纹理路径。
	void SetTopTexture(const UGString &str);

	//! \brief 返回顶面纹理。
	UGString GetTopTexture() const;

	//! \brief 设置纹理U方向的循环次数。
	//! \param str [in] 指定纹理坐标。
	void SetTilingU(const UGfloat &fvalue);

	//! \brief 返回纹理U方向的循环次数。
	UGfloat GetTilingU() const;

	//! \brief 设置纹理V方向的循环次数。
	//! \param str [in] 指定纹理坐标。
	void SetTilingV(const UGfloat &fvalue);

	//! \brief 返回纹理V方向的循环次数。
	UGfloat GetTilingV() const;

	//! \brief 设置顶面纹理U方向的循环次数。
	//! \param str [in] 指定纹理坐标。
	void SetTopTilingU(const UGfloat &fvalue);

	//! \brief 返回顶面纹理U方向的循环次数。
	UGfloat GetTopTilingU() const;

	//! \brief 设置顶面纹理V方向的循环次数。
	//! \param str [in] 指定纹理坐标。
	void SetTopTilingV(const UGfloat &fvalue);

	//! \brief 返回顶面纹理V方向的循环次数。
	UGfloat GetTopTilingV() const;

	//! \brief 设置纹理重复类型。
	//! \param TextureRepeatMode [in] 纹理重复类型。
	void SetTextureRepeatMode(const UGTextureRepeatMode& textureRepeatMode);

	//! \brief 返回纹理重复类型。
	UGTextureRepeatMode GetTextureRepeatMode() const;

	//! \brief 设置顶面纹理重复类型。
	//! \param TextureRepeatMode [in] 纹理重复类型。
	void SetTopTextureRepeatMode(const UGTextureRepeatMode& textureRepeatMode);

	//! \brief 返回顶面纹理重复类型。
	UGTextureRepeatMode GetTopTextureRepeatMode() const;

	//! \fn void SetIconAnchorPoint(const UGPoint2D& pntAnchor);
	//! \brief 设置地标图片的锚点位置
	//! \param const UGPoint2D& pntAnchor
	void SetIconAnchorPoint(const UGPoint2D& pntAnchor);

	//! \fn UGPoint2D GetIconAnchorPoint() const;
	//! \brief 获取地标图片的锚点位置
	//! \remarks UGPoint2D 
	//! \return UGPoint2D 
	UGPoint2D GetIconAnchorPoint() const;

	//! \brief 设置是否固定大小。
	//! \param str [in] 指定纹理坐标。
	void SetFixedPixelSize(const UGbool& bfixed);

	//! \brief 返回是否固定大小。
	UGbool GetFixedPixelSize() const;

	//! \brief 设置billboard类型。
	//! \param BBmode [in] billboard类型。
	void SetBillBoardMode(const UGBillBoardMode& BBmode);

	//! \brief 返回billboard类型。
	UGBillBoardMode GetBillBoardMode() const;

	//! \brief 设置锚点偏移方式。
	//! \param offsetMode [in] 偏移方式。
	void SetAnchorOffsetMode(const UGAnchorOffsetMode& offsetMode);

	//! \brief 返回偏移方式。
	UGAnchorOffsetMode GetAnchorOffsetMode() const;

	//! \brief 设置线风格。
	//! \param nLineStyle3D [in] 默认风格为简单线。
	//! \remarks 内部和SetLineStyle作用一致，用于二三维中线风格
	void SetLineStyle(const UGLineStyle3D& nLineStyle3D);

	//! \brief 返回线风格。
	//UGLineStyle3D GetLineStyle() const; // test lugw

	//! \brief 设置管线的片段数。
	//! \param nLineStyle3D [in] 默认为8。
	void SetPipeSides(const UGuint& nPipeSides);

	//! \brief 返回管线片段数。
	UGuint GetPipeSides() const;

	//! \brief 设置风格被修改。
	//! \param bModified [in] 默认风格为true。
	void SetModifiedFlag(UGbool bModified = true);

	//! \brief 返回风格是否被修改。
	UGbool IsModified() const;

	//! \brief 返回管线接缝处是否使用倒角
	UGbool IsFiletEnable()const;

	//! \brief 设置管线接缝处是否使用倒角
	void SetFiletEnable(UGbool bFiletEnable);

	//! \brief 返回线在依地时是否依地形进行插值
	UGbool IsTessellate()const;

	//! \brief 设置线在依地时是否依地形进行插值
	void SetTessellate(UGbool bTessellate);

	//! \brief 导成XML信息
	//! \param 0 [in]。
	//! \param "" [in]。
	//! \return 。
	//! \remarks 。
	//! \attention 。
	//UGString ToXML(UGint nVersion = 0, UGString strName = "")const;

	//! \brief 读取XML信息
	//! \param strXML [in]。
	//! \param true [in]。
	//! \param 0 [in]。
	//! \return 。
	//! \remarks 。
	//! \attention 。
	//UGbool FromXML(const UGString& strXML,UGbool bIgnoreCaption = true, UGint nVersion = 0);

	//! \brief 获取符号风格
	//UGint GetMarkerStyle() const; // test lugw 合并
	//! \brief 设置符号风格
	//void SetMarkerStyle(UGint nMarkerStyle); // test lugw 合并

	//! \brief 是否使用三维符号
	void SetMarker3D(UGbool bIsMarker3D);
	//! \brief 是否使用三维符号
	UGbool IsMarker3D()const;


	//! \brief 设置符号缩放比例
	void SetMarker3DScaleX(const UGdouble& dScaleX);
	void SetMarker3DScaleY(const UGdouble& dScaleY);
	void SetMarker3DScaleZ(const UGdouble& dScaleZ);
	//! \brief 获取符号缩放比例
	UGdouble GetMarker3DScaleX() const;
	UGdouble GetMarker3DScaleY() const;
	UGdouble GetMarker3DScaleZ() const;


	//! \brief 设置符号旋转角度
	void SetMarker3DRotateX(const UGdouble& dRotateX);
	void SetMarker3DRotateY(const UGdouble& dRotateY);
	void SetMarker3DRotateZ(const UGdouble& dRotateZ);
	//! \brief 获取符号旋转角度
	UGdouble GetMarker3DRotateX() const;
	UGdouble GetMarker3DRotateY() const;
	UGdouble GetMarker3DRotateZ() const;

	UGString GetPicturePath() const;
	void SetPicturePath(UGString picturePath);

public:

	//! \brief 是否使用用户自定义的固定颜色
	UGbool IsFixedColor() const;
	//! \brief 设置是否使用用户自定义的固定颜色
	void   SetIsFixedColor(UGbool bFixedColor);

	//! \brief 获取用户自定义的固定颜色
	UGColor GetFixedColor() const;
	//! \brief 设置用户自定义的固定颜色
	void    SetFixedColor(UGColor color);

	//! \brief 用于唯一标识风格所对应的材质
	UGString GetMaterialName(UGint nDimension);


	//! \brief 线型的纵向偏移量，主要用在显示路况的地方，临时存储
	//! \remarks 不要在其他地方使用，避免误伤
	UGdouble GetLineVertOffset() const;
	void SetLineVertOffset(UGdouble dOffset);
private:
	void AddSymbolStyleToArray(UGint nSymStyle, UGArray<UGint>& arrStyleIDs) const;

protected:
	UGbool m_bFixedColor;
	UGColor m_crFixedColor;

protected:
	// 用于二三维共用属性的合并，避免出现歧义，主要是在导入导出XML时使用
	void UpdateStyleAttribute();

private:	
	UGint m_nMarkerStyle;
	UGint m_nMarkerAngle;		//单位：0.1度
	UGint m_nMarkerSize;
	UGint m_nMarkerWidth;
	UGint m_nMarkerHeight;
	UGString m_PicturePath;

	UGint m_nLineStyle;
	UGColor m_LineColor;
	UGint m_nLineWidth;
	
	UGint m_nFillStyle;
	UGColor m_FillBackColor;
	UGColor m_FillForeColor;
	UGbool m_bFillBackOpaque;
	UGbyte m_btFillOpaqueRate;
	UGbyte m_btFillGradientType;	
	UGshort m_nFillAngle;		//单位：0.1度
	UGshort m_nFillCenterOffsetX;
	UGshort m_nFillCenterOffsetY;
	
	UGbool	m_bSymbolScale;		//点符号的Size是Symbol的size还是边框(255,255)的size
								//TRUE Symbol的size 
								//FALSE 边框 dongfei adds 2007-03-29

	//符号库信息
	UGbyte* m_pMarkerSymbolBuffer;
	UGuint m_nMarkerSymbolLength;
	UGuint m_nMarkerSymbolVersion;
	UGbyte* m_pLineSymbolBuffer;
	UGuint m_nLineSymbolLength;
	UGuint m_nLineSymbolVersion;
	UGbyte* m_pFillSymbolBuffer;
	UGuint m_nFillSymbolLength;
	UGuint m_nFillSymbolVersion;
	UGStyleEx* m_pStyleEx;

	//标识GeoRegion是否跨越+-180度经线显示
	UGbool m_bAcrossLongtitude;

	// 线型的纵向偏移量，主要用在显示路况的地方，临时存储，不需要写到流里面 by gouyu 2016-8-30
	UGdouble m_dLineVertOffset;

	UGint m_nStyle;		
};

extern TOOLKIT_API UGArray<UGint>	g_arrLineStyleID;			// 矢量缓存统计用到的线符号id数组，精简工作空间符号库用
extern TOOLKIT_API UGArray<UGint>	g_arrMarkerStyleID;			// 矢量缓存统计用到的点符号id数组，精简工作空间符号库用
extern TOOLKIT_API UGArray<UGint>	g_arrFillStyleID;			// 矢量缓存统计用到的填充符号id数组，精简工作空间符号库用
extern TOOLKIT_API UGbool			g_bStatisticsSymbolID;		// 是否统计符号id，精简工作空间符号库用
}

#endif

