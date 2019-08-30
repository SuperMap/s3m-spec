/*!
**************************************************************************************
\file     UGStyleEx.h 
**************************************************************************************
\author   陈国雄
\brief    几何风格扩展类                                                             
\attention   
----------------------------------------------------------------------------------<br>
Copyright (c) 2000-2010 SuperMap Software Co., Ltd.                           <br>
All Rights Reserved.                                                             <br>
----------------------------------------------------------------------------------<br> 

**************************************************************************************
\version 2010-12-29												                   <br>
\remarks 用于二三维风格统一所用，迁移自三维符号风格，不对外开放！！
**************************************************************************************
*/

#ifndef UGSTYLEEX_H
#define UGSTYLEEX_H

#include "Stream/ugdefs.h"
#include "Toolkit/UGColorValue3DEx.h"
#include "Stream/UGStream.h"

#define UG_STYLE3D_VERSION_2012	0X20120514
namespace UGC {

	//! \brief 2维3维一体化时面的填充模式
	enum UGFill3DMode
	{
		//! \brief 显示填充
		FILL_FACE = 1, 
		//! \brief 显示轮廓线
		FILL_LINE = 2, 
		//! \brief 显示填充和轮廓
		FILL_FACEANDLINE = 3, 
	};

	//! \brief 依地形的三种模式定义
	enum AltitudeMode
	{
		//! \brief  依地形
		ClampToGround = 0,
		//! \brief  相对地形
		RelativeToGround = 1,
		//! \brief  绝对高度
		Absolute = 2,				// 点和文本的拉伸可以设置拉伸高度，也可以设置成此模式
		//! \brief  相对于地下
		RelativeToUnderground = 3,
		RelativeUnderGround = RelativeToUnderground,
		//! \brief  地下绝对高度
		AbsoluteUnderGround = 4,
		//! \brief  修改地形高度
		ModifyTerrain = 5,
		//! \brief 依模型
		ClampToObject = 6,
		//! \brief 陆地边界
		LandBoundary = 7,
		//! \brief 海洋边界
		OceanBoundary = 8,
	};

	//! \brief 布告板模式
	enum UGBillBoardMode
	{
		SCREEN_ALIGNED,//不贴地，对齐于屏幕的，始终面向观察点
		AUTO_Z_AXIAL,//轴对齐布告板,围绕某个固定轴旋转
		FIXED_ANGLE,//贴地，可旋转固定角度（目前是贴地，具体角度以后可再开接口）
		FIXED_ROTATE_ANGLE // 绕世界坐标系X/Y/Z轴分别旋转一定角度
	};

	//! \brief 锚点偏移像素模式
	enum UGAnchorOffsetMode
	{
		//! \brief 按照锚点按文字/图片比例偏移
		FixedScale = 0,
		//! \brief 按照具体像素偏移
		FixedPixel = 1,
	};

	//! \brief 纹理重复模式
	enum UGTextureRepeatMode
	{
		//! \brief 纹理重复次数
		RepeatTimes = 0,
		//! \brief 纹理实际大小
		RealSize = 1, 
	};

	enum UGLineStyle3D
	{
		//! \brief 普通线
		LINE_SIMPLE = 0, 
		//! \brief 三维管线
		LINE_PIPELINE = 1, 
	};

class TOOLKIT_API UGStyleEx
{
public:
	//! \brief 构造函数。
	UGStyleEx();

	//! \brief 析构函数。
	virtual ~UGStyleEx();

	//! \brief 重载=。
	//! \param &style [in]。
	//! \return 。
	//! \remarks 。
	//! \attention 。
	void operator = (const UGStyleEx &style);

	//! \brief 重载==。
	//! \param &style [in]。
	//! \return 。
	//! \remarks 。
	//! \attention 。
	UGbool operator == (const UGStyleEx &style);

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
	void SetLineWidth(const UGfloat &fWidth);

	//! \brief 返回线宽度。
	UGfloat GetLineWidth() const;

	//! \brief 设置线颜色。
	void SetLineColor(const UGColorValue3D &color);

	//! \brief 返回线颜色。
	UGColorValue3D GetLineColor() const;

	//! \brief 设置面颜色。
	void SetFillColor(const UGColorValue3D &color);

	//! \brief 返回面颜色。
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
	void SetLineStyle(const UGLineStyle3D& nLineStyle3D);

	//! \brief 返回线风格。
	UGLineStyle3D GetLineStyle() const;

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

	//! \brief 导成XML信息
	//! \param 0 [in]。
	//! \param "" [in]。
	//! \return 。
	//! \remarks 。
	//! \attention 。
	UGString ToXML(UGint nVersion = 0, UGString strName = _U(""))const;

	//! \brief 读取XML信息
	//! \param strXML [in]。
	//! \param true [in]。
	//! \param 0 [in]。
	//! \return 。
	//! \remarks 。
	//! \attention 。
	UGbool FromXML(const UGString& strXML,UGint nVersion = 0);

	//! \brief 格式转换
	//! \param strRGBA [in]。
	//! \return 。
	//! \remarks 。
	//! \attention 。
	static UGColorValue3D ConvertRGBAColor(const UGString& strRGBA);

	//! \brief 获取符号风格
	UGint GetMarkerStyle() const;
	//! \brief 设置符号风格
	void SetMarkerStyle(UGint nMarkerStyle);

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

	//! \brief
	UGint Load(UGStream& stream, UGint nDimension);
	//! \brief
	void Save(UGStream& stream, UGint nDimension) const;

	//! \brief 获取符号风格大小，在当前维度下的大小与保存自身记录的四个字节的总和
	UGuint GetStyleSize(UGint nDimension)const;

	//! \brief 返回管线接缝处是否使用倒角
	UGbool IsFiletEnable()const;

	//! \brief 设置管线接缝处是否使用倒角
	void SetFiletEnable(UGbool bFiletEnable);

	//! \brief 返回线在依地时是否依地形进行插值
	UGbool IsTessellate()const;

	//! \brief 设置线在依地时是否依地形进行插值
	void SetTessellate(UGbool bTessellate);

public:
	//! \brief 填充模式
	UGFill3DMode m_Fill3DMode;

	//! \brief 依地模式
	AltitudeMode m_AltitudeMode;

	//! \brief 点地标纹理的名称。
	UGString m_strIconName;

	//! \brief 图标的比例。保留
	UGfloat m_fIconScale;

	//! \brief 简单点大小。
	UGfloat m_fPointSize;

	//! \brief 点的颜色。
	UGColorValue3D m_ColorPoint;

	//! \brief 线的宽度。
	UGdouble m_dLineWidth;

	//! \brief 线颜色，包括线和轮廓线。
	UGColorValue3D m_ColorLine;

	//! \brief 填充颜色。
	UGColorValue3D m_ColorFill;

	//! \brief 底部高度
	UGdouble m_dBottomAltitude;

	//! \brief 拉伸高度。
	UGdouble m_dExtendHeight;

	//! \brief 拉高成面侧面纹理的名称。
	UGString m_strSideTexture;

	//! \brief 上顶面纹理的名称。
	UGString m_strTopTexture;

	//! \brief 纹理U方向的重复次数
	UGfloat m_fTilingU;

	//! \brief 纹理V方向的重复次数
	UGfloat m_fTilingV;

	//! \brief 纹理重复模式
	UGTextureRepeatMode m_eTextureRepeatMode;

	//! \brief 顶面纹理U方向的重复次数
	UGfloat m_fTopTilingU;

	//! \brief 顶面纹理V方向的重复次数
	UGfloat m_fTopTilingV;

	//! \brief 顶面纹理重复模式
	UGTextureRepeatMode m_eTopTextureRepeatMode;

	//! brief 地标图片的锚点位置,位于0.0 ~ 1.0之间的数值
	UGPoint2D m_pntIconAnchor;

	UGbool m_bMarker3D;
	UGint m_nMarkerStyle;

	UGdouble m_dScale[3];
	UGdouble m_dRotate[3];

	//! brief 是否固定显示大小
	UGbool m_bFixedPixelSize;

	//! \brief 是否在管线接缝处设置倒角
	UGbool m_bIsFiletEnable;

	//! \ brief 风格是否改变
	UGbool m_bModified;

	//! \brief 线的类型目前是普通线,拉伸成面和管线。
	UGLineStyle3D m_nLineStyle3D;

	//! \brief 管线片段数，默认为8。8为圆管，4为方管，3为三角管
	UGuint m_nPipeSides;

	UGbool m_bTessellate;

private:
	//! brief 布告板属性
	//! \remarks 第16位:锚点偏移方式
	//! \remarks 低4位 :布告板类型
	UGuint m_nBillboardOption;
};

}
#endif
