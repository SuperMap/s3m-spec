/*! \file	 UGGeometry.h
*  \brief	 几何对象基类
*  \author	 ugc team
*  \attention 
*  Copyright(c) 2000-2010 SuperMap Software Co., Ltd.<br>
*  All Rights Reserved
*/

#ifndef UGGEOMETRY_H
#define UGGEOMETRY_H

#include "Toolkit/UGAutoPtr.h"
#include "Toolkit/UGStyle.h"
#include "Geometry/UGEditType.h"
#include "Geometry/UGDataCodec.h"
#if	!defined SYMBIAN60 && !defined  OS_ANDROID && !defined (IOS) 
#include "Element/OgdcElemFactory.h"
#endif
#include "Projection/UGPrjCoordSys.h"
#include "Projection/UGPrjTranslator.h"
#include "Projection/UGRefTranslator.h"
#include "Toolkit/UGTextureInfo.h"


//#include "Symbol/UGSymbol.h"
namespace UGC {

struct UGGeodesicVector  //避免调用UGVector3D，自己定义一个结构体使用,用于测地线的计算
{
	double x;
	double y;
	double z;

	UGGeodesicVector(double Vx, double Vy, double Vz)
	{
		x = Vx;
		y = Vy;
		z = Vz;
	}

	UGGeodesicVector()
	{
		x = 0;
		y = 0;
		z = 0;
	}

	void Normalize()
	{
		UGdouble fLength = 0.0;
		fLength = sqrt(x*x+y*y+z*z);

		if(!UGIS0(fLength))
		{
			x /= fLength;
			y /= fLength;
			z /= fLength;
		}		
	}

	UGGeodesicVector CrossProduct(UGGeodesicVector vec)
	{
		UGGeodesicVector TempVector;
		TempVector.x = y*vec.z- z*vec.y;
		TempVector.y = z*vec.x - x*vec.z;
		TempVector.z = x*vec.y - y*vec.x;

		return TempVector;
	}

	double DotProduct(UGGeodesicVector vec)
	{
		return x*vec.x + y*vec.y + z*vec.z;
	}
}; 

class UGGeoLine;
class UGGeoRegion;
class UGGeoMultiPoint;
class UGGeoPicture;

#define  BOUND_HANDLE                   9      //固定句柄数目

//! \brief 几何对象基类 
#ifdef SYMBIAN60
class GEOMETRY_APIC UGGeometry  
#else
class GEOMETRY_API UGGeometry  
#endif
{
public:
	//! UGC6.0 新增加的Geometry枚举类型取值范围规定：
	//! 1001-2000 Geometry;		2001-4000 CAD;		4001-6000 三维;
	//! 6001-8000 Layout;		8001-10000 预留;	>=10001 用户自定义.UDB引擎中预留了16位存储Geometry类型，因些用户自定义最大值不能超过2^16
	//! \brief 几何对象类型定义
	enum Type
	{
		//! 空类型
		Null				= 0, 
		//! \brief 点类型	
		GeoPoint			= 1, 
		//! \brief 复合点
		GeoMultiPoint		= 2, 
		//! \brief 线类型
		GeoLine				= 3, 
		//! \brief 面类型
		GeoRegion			= 5, 
		//! \这里注意：MongoGridFS的二进制参数独占   Type值6的Geometry类型以后不允许拓展
		//! \brief 文本类型
		GeoText				= 7, 	
		//! \brief 参数化线对象 added by pj 
		GeoParamLine		=16,
		//! \brief 参数化线对象 added by pj 
		GeoParamRegion		=17,
		//! \brief 复合的参数化线对象 added by pj 
		GeoCompoundLine		=8, 
		//! \brief 复合的参数化面对象 added by pj 
		GeoCompoundRegion	=9,
		//! \brief 矩形和斜矩形统一为带旋转角的矩形
		GeoRect				=12,
		//! \brief 带旋转角的圆角矩形
		GeoRectRound		=13,	
		//! \brief 圆
		GeoCircle			=15,
		//! \brief 椭圆和斜椭圆统一为带旋转角的椭圆
		GeoEllipse			=20,	
		//! \brief 椭圆扇面
		GeoPie				=21,	
		//! \brief 弦, 暂不提供, 不要开放出去
		GeoChord			=23,
		//! \brief 圆弧
		GeoArc				=24,	
		//! \brief 椭圆弧
		GeoEllipticArc		=25,	
		//! \brief Cardinal曲线,保证曲线一定通过控制点
		GeoCardinal			=27,	
		//! \brief 曲线 for dgn,具体在数学定义上是哪种类型的曲线,搞不太清楚了
		GeoCurve			=28,	
		//! \brief 贝叶斯曲线
		GeoBSpline			=29,	
		//! \brief 含线性坐标系的线(即带有M值的GeoLine)
		GeoLineM			=35, 
		//! \brief 三维点
		GeoPoint3D			=101,	
		//! \brief 三维多点, 暂不提供, 不要开放出去
		GeoMultiPoint3D		=102,	
		//! \brief 三维线
		GeoLine3D			=103,	
		//! \brief 三维面
		GeoRegion3D			=105,	
		//! \brief 三维文本, 暂不提供, 不要开放出去
		GeoText3D			=107,
		//! \brief 三维图标, 暂不提供, 不要开放出去
		GeoPlacemark        =108,

		//! \brief 三角形, 暂不提供, 不要开放出去
		GeoTriangle			=109,	
		//! \brief 立方体, 暂不提供, 不要开放出去
		GeoCube				=111,	
		//! \brief 测量用, 暂不提供, 不要开放出去
		GeoTraverseZ		=133,	
		//! \brief 贝叶斯曲面, 暂不提供, 不要开放出去
		GeoBSurface			=135,	
		//! \brief 多面体, 暂不提供, 不要开放出去
		GeoPolyhedron		=137,	
		//! \brief 不规则三角网(TIN地形)
		GeoTINTile = 138,
		//! \brief 不规则三角面, Tin
		GeoTinBlock			=139,
		//! \brief 不规则四面体结构
		GeoTIM = 140,
		//! \brief 复合对象, 该值同时代表Geometry对象类型的上限
		GeoCompound			=1000,
		GeoUser				=1001,		
		GeoPicture			=1101, 
		GeoModel			=1201,
		//! \brief malq-三维中依地形的图片
		GeoGroundOverlay    =1202,
		//! fengzhh三维规则几何体
		GeoSphere           =1203,
		GeoHemiSphere       =1204,
		GeoBox              =1205,
		GeoCylinder         =1206,
		GeoCone             =1207,
		GeoPyramid          =1208,
		GeoPie3D            =1209,
		GeoCircle3D         =1210,
		GeoPieCylinder      =1211,
		GeoEllipsoid        =1212,
		//! \brief 粒子
		GeoParticle			=1213,

		GeoTexture			=1214,

		GeoModelEntitySkeleton		= 1215,
		GeoModelEntityTexture		=1216,
		GeoModelEntityMaterial3D	=1217,
		GeoModelPro					=1218,
		GeoBillboard				=1220,


		//added by xielin Layout相关的geometry从2000~3000
		GeoPaper			=2000,
		GeoMap				=2001,
		GeoTable			=2002,
		GeoTableCell		=2003,
		/*		GeoMapRegion		=2004,*/
		GeoScale            =2005,
		// 		GeoRulerLine		=2006, 
		// 		GeoRuler			=2007,
		GeoDirection		=2008,	
		GeoMapDecorateBorder=2009,
		GeoArtText			=2010,
		GeoLegend			=2011,
		GeoMapGrid			=2012,//added by xuxr 2011-10-27

		GraphicObject		= 3000, //test zhangyl
		//GraphicObject3D		= 3001

		// 清华山维对象
		GeoPointEPS			=4000,
		GeoLineEPS			=4001,
		GeoRegionEPS		=4002,
		GeoTextEPS			=4003,

		GeoChart            = 5000, // 图表对象类型
	};

	//! \brief GML版本信息定义
	enum GMLVersionType
	{
		SML = 0,	//supermap自定义格式
		GML212		= 212,	//GML2.1.2版本
		GML310		= 310,	//GML3.1.0版本
		GML311		= 311,	//GML2.1.1版本
		KML100		= 100,  //KML1.0.0版本
		KML210		= 210,  //KML2.1.0版本
		KML220		= 220   //KML2.2.0版本		
	};

public: //! 构造和析构函数
	//! \brief 构造函数
	GEOMETRY_APIF UGGeometry();

	//! \brief 析构函数
	virtual GEOMETRY_APIF ~UGGeometry();
	GEOMETRY_APIF UGGeometry(const UGGeometry& geometry);

public: //! 得到和设置一些信息

	//! \brief 让Geometry拥有风格对象,如果m_pStyle为空创建一个新对象,否则什么也不干。
	//! \return 内存分配失败是返回FALSE,其他情况返回TRUE。
	//! \remarks UDB里用到。
	virtual GEOMETRY_APIF UGbool CreateStyle();	

	//! \brief 清空地理数据
	virtual GEOMETRY_APIF void Clear() =0;

	//! \brief 得到维度, 点为0维, 线/曲线等为1维, 面/曲面等为2维, GeoCompound/GeoText等为-1
	virtual GEOMETRY_APIF UGint GetDimension() const = 0;

	//! \brief 得到类型
	virtual GEOMETRY_APIF Type GetType() const = 0;

	//! \brief 得到内点（在Geometry内部，靠近中央的点）
	//! \return 返回内点
	virtual GEOMETRY_APIF UGPoint2D GetInnerPoint() const = 0;

	//! \brief 得到内点（在Geometry内部，靠近中央的点）
	//! \param pnt 传出的点坐标数组[out]。
	//! \return 是否成功得到内点
	//	virtual GEOMETRY_APIF UGbool GetInnerPoint(UGPoint2D &pnt) const = 0;	

	//! \brief 得到空间数据, 主要用于需要按照维度相同时
	//! 来做统一处理的时候, 例如一些空间判断和操作的算法
	//! \param pPoints 传出的点坐标数组[out]。
	//! \param pPolyCount 传出的每个子对象点个数的数组[out]。
	//! \param nSubCount 传出的子对象个数[out]。
	//! \param nSegmentCount 对参数化对象, 每个弧段分割点的个数[in]。
	//! \remarks 参数采用了智能指针的技术,调用者不用释放内存空间
	//! \return 成功返回true,否则返回false。
	virtual GEOMETRY_APIF UGbool GetSpatialData(UGAutoPtr<UGPoint2D>& pPoints, 
		UGAutoPtr<UGint>& pPolyCount, UGint& nSubCount, UGint nSegmentCount=0) const = 0;	

	//! \brief 得到空间数据, 主要用于需要按照维度相同时
	//! 来做统一处理的时候, 例如一些空间判断和操作的算法
	//! \param pPoints 传出的点坐标数组[out]。
	//! \param pPolyCount 传出的每个子对象点个数的数组[out]。
	//! \param nSubCount 传出的子对象个数[out]。
	//! \param nSegmentCount 对参数化对象, 每个弧段分割点的个数[in]。
	//! \remarks 参数采用了智能指针的技术,调用者不用释放内存空间
	//! \return 成功返回true,否则返回false。
	virtual GEOMETRY_APIF UGbool GetGeodesicSpatialData(UGAutoPtr<UGPoint2D>& pPoints, 
		UGAutoPtr<UGint>& pPolyCount, UGint& nSubCount,UGRefTranslator *pPJTranslator ,UGint nSegmentCount=0) const;

	//! \brief 得到顶点索引序列，仅Region和Region3D对象能够获取
	//! \param arrTriIndex 传出的顶点序列数组
	//! \return 成功返回true,否则返回false
	virtual UGbool GetVertexSeq(UGArray<UGushort>& arrTriIndex);

	//! \brief 几何对象是否有效
	//! \return 有效返回true,无效返回false
	virtual GEOMETRY_APIF UGbool IsValid() const = 0;

	//! \brief 点击测试, 用于对象点选
	//! \param pntHitTest 用来测试选中的点
	//! \param dTolerance 容限
	//! \return 选中返回true, 否则返回false
	virtual GEOMETRY_APIF UGbool HitTest( const UGPoint2D &pntHitTest, UGdouble dTolerance) const = 0;

	//! \brief 缩放
	//! \param dRatioX X方向缩放比率
	//! \param dRatioY Y方向缩放比率
	virtual GEOMETRY_APIF void Inflate( UGdouble dRatioX, UGdouble dRatioY) = 0;

	//! \brief 缩放
	//! \param dRatio 缩放比率(X,Y方向相同)
	virtual GEOMETRY_APIF void Inflate( UGdouble dRatio) = 0;

	//! \brief 平移
	//! \param dX 平移的X偏移量
	//! \param dY 平移的Y偏移量
	virtual GEOMETRY_APIF void Offset( UGdouble dX, UGdouble dY) = 0;

	//! \brief 平移
	//! \param 偏移量(X,Y方向相同)
	virtual GEOMETRY_APIF void Offset( UGdouble dOffset) = 0;

	//! \brief 平移
	//! \param 平移的偏移量(分别用UGSize2D中的x,y代表X,Y方向的偏移量)
	virtual GEOMETRY_APIF void Offset( const UGSize2D &szOffset) = 0;

	//! \brief 旋转
	//! \param pntOrigin 旋转的中心点(锚点,不动点)
	//! \param dAngle 旋转角度
	virtual GEOMETRY_APIF void Rotate(const UGPoint2D& pntOrigin, UGdouble dAngle);

	//! \brief 旋转,增加一些参数,以便内部可以不用进行cos,sin, 在批量处理的时候有利于提高效率
	//! \remarks dAngle看似多余,但GeoText需要,所以要保留
	//! \param pntOrigin 旋转的中心点(锚点,不动点)
	//! \param dCosAngle 旋转的cos值
	//! \param dSinAngle 旋转的sin值
	//! \param dAngle 旋转角度
	virtual GEOMETRY_APIF void Rotate( const UGPoint2D& pntOrigin, UGdouble dCosAngle, UGdouble dSinAngle,double dAngle = 0) = 0;

	//! \brief 调整几何数据,限定在新的Bounds中
	//! \param rcNewBounds 新的,限定范围的Bounds
	//! \return 成功返回true,失败返回false
	virtual GEOMETRY_APIF UGbool Resize(const UGRect2D& rcNewBounds) = 0;

	//! \brief 两点镜像
	//! \param pntMirror1 构成镜面的第一个点
	//! \param pntMirror2 构成镜面的第二个点
	//! \return 成功返回true,失败返回false
	virtual GEOMETRY_APIF UGbool Mirror(const UGPoint2D &pntMirror1, const UGPoint2D &pntMirror2);

public:
	//! \brief 得到对象面积
	//! \attention Dimension为2的对象才支持
	//! \return 返回面积
	virtual GEOMETRY_APIF UGdouble GetArea() const;

	//! \brief 得到对象长度
	//! \attention Dimension为1or2的对象才支持
	//! \return 返回长度
	virtual GEOMETRY_APIF UGdouble GetLength() const;

	//! \brief 得到子对象的个数
	//! \return 返回子对象的个数
	virtual GEOMETRY_APIF UGint GetSubCount() const;

	//! \brief 得到某个子对象的点的个数。
	//! \param nSubIndex [in] 子对象索引。
	//! \return 返回某个子对象的点的个数。
	virtual UGint GetSubPntCount(UGint nSubIndex) const;

	//! \brief 几何对象的存储空间
	//! \return 返回几何对象的存储空间
	virtual GEOMETRY_APIF UGint GetDataSize() const {return 0;};

	virtual GEOMETRY_APIF UGGeometry* GetSubPtr( UGint nSubIndex ) {return 0;};

public:
	//Projection..

	//! \brief 根据投影转化器的设置和转化方向要求, 进行投影转化
	//! \param pPJTranslator 投影转化器
	//! \param bForward 转化方向, true代表从UGRefTranslator的源(Src)转化到目的(Des), false则相反
	//! \param bClip 是否裁剪，投影转换需要裁减部分图形，但checkbounds()等计算数据集bounds不能参与裁剪。
	//! \param return返回裁剪后是否还显示该图形，兼容以前，默认返回true
	virtual GEOMETRY_APIF UGbool PJConvert( UGRefTranslator *pPJTranslator, UGbool bForward ,UGbool bClip );

	//! \brief 根据投影转化器的设置和转化方向要求, 进行投影转化
	//! \param pPJTranslator 投影转化器
	//! \param bForward 转化方向, true代表从UGRefTranslator的源(Src)转化到目的(Des), false则相反
	virtual GEOMETRY_APIF void PJConvert( UGRefTranslator *pPJTranslator, UGbool bForward = true);
	//! \brief 从经纬坐标系统转化为投影坐标系统
	//! \param pCoordSys 投影坐标系统
	virtual GEOMETRY_APIF void PJForward( UGPrjCoordSys *pCoordSys );

	//! \brief 从投影坐标系统转化为经纬坐标
	//! \param pCoordSys 投影坐标系统
	virtual GEOMETRY_APIF void PJInverse( UGPrjCoordSys *pCoordSys );

public: //! 存储到外部介质中,或者从外部介质中生成

	//! \brief 得到ID
	//! \return 返回ID
	GEOMETRY_APIF UGint GetID() const;

	//! \brief 设置ID
	//! \param 要设置的ID
	GEOMETRY_APIF void SetID( UGint nID );

	//! \brief 得到外接矩形
	//! \return 返回外接矩形
	const GEOMETRY_APIF UGRect2D& GetBounds() const;

	//! \brief 设置外接矩形,一般不要使用, 内部会维护Bounds的正确性
	//! \param rcBounds 要设置的Bounds
	GEOMETRY_APIF void SetBounds( const UGRect2D& rcBounds );

	//! \brief 设置风格
	//! \param pStyle 风格指针
	//! \remarks 如果pStyle==NULL,则Geometry内部风格指针被删除; 否则会复制一份风格
	//! 对于文本对象等, 设置风格不起作用,因为文本对象有自己的TextStyle
	virtual GEOMETRY_APIF void SetStyle(const UGStyle* pStyle);

	//! \brief 得到风格对象的指针,如果没有返回NULL
	//! \return 返回风格的指针,有可能为NULL
	virtual GEOMETRY_APIF UGStyle* GetStyle() const;

	//! \brief 得到风格对象的指针,如果没有返回NULL
	//! \return 返回风格的指针,有可能为NULL
	virtual GEOMETRY_APIF void SetCancelColor(UGColor color);

public:
	//! \brief 拷贝Geometry基类中的成员变量, 包括ID, Style, Bounds等
	//! \param geo 提供信息的Gemetry
	GEOMETRY_APIF void CopyBaseData(const UGGeometry& geo);

	//! \brief 保存到存储流重
	//! \param stream 存储用的流, 调用之前应该先Open
	//! \param eCodecType 存储的编码方式
	//! \param bIgnoreStyle 是否忽略对象风格
	//! \return 成功返回true,失败返回false
	virtual GEOMETRY_APIF UGbool Save(UGStream& stream, UGDataCodec::CodecType eCodecType=UGDataCodec::encNONE, 
		UGbool bIgnoreStyle = TRUE) const;

	//! \brief 从流中装载数据
	//! \param stream 流
	//! \param iJuge新增一个参数EPS调用不含属性名字的数据
	//! \remarks Load时,由于很多信息已经存储在stream中了,所以存储编码和是否保存风格等参数就不需要了
	//! \return 成功返回true,失败返回false
	GEOMETRY_APIF UGbool Load(UGStream& stream,UGint iJuge=0);

public:
	// zengzm 2005.8.16 被SDBPlus所迫,不得不public之
	//! \brief 只保存几何数据部分
	//! \param stream 存储流, 调用之前应该先Open
	//! \param eCodecType 存储的编码方式
	//! \param bSDBPlus 是否存储为SDBPlus的格式
	//! \remarks 由于SDBPlus引擎和数据库引擎的存储上差别较大,所以必须直接调用该函数,
	//! 并增加变量进行分别处理
	//! \return 成功返回true,失败返回false
	virtual GEOMETRY_APIF UGbool SaveGeoData(UGStream& stream, 
		UGDataCodec::CodecType eCodecType,UGbool bSDBPlus = FALSE) const = 0;

	// zengzm 2005.8.16 被SDBPlus所迫,不得不public之
	//! \brief 只保存几何数据部分
	//! \param stream 存储流, 调用之前应该先Open
	//! \param eCodecType 存储的编码方式
	//! \param bSDBPlus 是否存储为SDBPlus的格式
	//! \param fCacheVersion = 1.4f 缓存版本号
	//! \remarks 由于SDBPlus引擎和数据库引擎的存储上差别较大,所以必须直接调用该函数,
	//! 并增加变量进行分别处理
	//! \return 成功返回true,失败返回false
	virtual GEOMETRY_APIF UGbool SaveVecData(UGStream& stream,
		UGDataCodec::CodecType eCodecType,UGbool bSDBPlus = FALSE, UGfloat fCacheVersion = 1.4f) const;

	//! \brief 存储额外的附加数据,不和Geometry存储在一起的数据
	//! \brief 模型里边存储纹理和模型分开
	//! \return 成功返回TRUE,失败返回FLASE
	virtual GEOMETRY_APIF UGbool SaveExtData(UGArray<UGTextureInfo>& arryTextureInfo){return FALSE;};

	//! \brief 通过几何对象中保存的纹理路径初始化纹理信息
	//! \return 成功返回TRUE,失败返回FLASE
	virtual GEOMETRY_APIF UGbool InitialTextureInfo(UGArray<UGTextureInfo>& arrayTextureInfos){return FALSE;};

	//! \brief 加载额外纹理数据到几何对象中
	//! \return 成功返回TRUE,失败返回FLASE
	virtual GEOMETRY_APIF UGbool LoadExtData(const UGArray<UGTextureInfo>& arrayTextureInfos){return FALSE;};

	//! \brief 只转载几何数据部分
	//! \param stream 流 
	//! \param eCodecType 编码方式
	//! \attention 还需要增加 UGbool bSDBPlus = FALSE 的参数
	//! \return 成功返回true,失败返回false
	virtual GEOMETRY_APIF UGbool LoadGeoData( UGStream& stream , 
		UGDataCodec::CodecType eCodecType = UGDataCodec::encNONE,UGbool bSDBPlus = FALSE ) = 0;

	//! \brief 只转载几何数据部分
	//! \param stream 流 
	//! \param eCodecType 编码方式
	//! \param eCodecType 用户采用的Load方式的nGuge判断参数
	//! \attention 还需要增加 UGbool bSDBPlus = FALSE 的参数
	//! \return 成功返回true,失败返回false
	virtual GEOMETRY_APIF UGbool LoadGeoData( UGStream& stream , UGint iGuge,
		UGDataCodec::CodecType eCodecType = UGDataCodec::encNONE,UGbool bSDBPlus = FALSE);

	//! \brief 只转载几何数据部分
	//! \param stream 流 
	//! \param eCodecType 编码方式
	//! \attention 还需要增加 UGbool bSDBPlus = FALSE 的参数
	//! \param fCacheVersion = 1.4f 缓存版本号
	//! \return 成功返回true,失败返回false
	virtual GEOMETRY_APIF UGbool LoadVecData( UGStream& stream,
		UGDataCodec::CodecType eCodecType = UGDataCodec::encNONE,UGbool bSDBPlus = FALSE, UGfloat fCacheVersion = 1.4f);

	virtual GEOMETRY_APIF UGbool LoadOneData( UGStream& stream );

	virtual GEOMETRY_APIF UGbool LoadAllData( UGbyte* pByte );

	//! \brief 转化为线对象
	//! \param geoLine[out] 得到的线对象
	//! \param nSegmentCount 当曲线转化为线时,每一段用多少个点来模拟
	//! \return 成功返回true,失败返回false
	virtual GEOMETRY_APIF UGbool ConvertToLine( UGGeoLine &geoLine, UGint nSegmentCount = 0 ) const;

	//! \brief 转化为测地线对象
	//! \param geoLine[out] 得到的测地线对象
	//! \param nSegmentCount 当曲线转化为测地线时,每一段用多少个点来模拟
	//! \return 成功返回true,失败返回false
	virtual GEOMETRY_APIF UGbool ConvertToGeodesicLine( UGGeoLine &geoLine, UGRefTranslator *pPJTranslator,UGint nSegmentCount = 0 ) const;

	//! \brief 转化为面对象
	//! \param geoRegion[out] 得到的面对象
	//! \param nSegmentCount 当曲面转化为面时,每一段用多少个点来模拟
	//! \return 成功返回true,失败返回false
	virtual GEOMETRY_APIF UGbool ConvertToRegion( UGGeoRegion &geoRegion, UGint nSegmentCount = 0 ) const;

	//! \brief 转换成多点对象，可以把3D点、3D多点对象转换成多点对象显示。
	//! \param geoMultiPoint [out] 输出多点几何对象。
	virtual GEOMETRY_APIF UGbool ConvertToPoint( UGGeoMultiPoint &geoMultiPoint) const;	

	//! \brief 转换成图片，3D类型几何对象，以及用户类型几何对象可以通过先转换成图片，再显示。
	//! \param geoPicture [out] 输出图片几何对象。
	//! \param dXAngle [in] 水平倾角。
	//! \param dZAngle [in] 旋转倾角。
	virtual GEOMETRY_APIF UGbool ConvertToPicture( UGGeoPicture &geoPicture, UGdouble dResolution, 
		UGdouble dXAngle = 0, UGdouble dZAngle = 0) const;	

	//! \brief 是否是3D类型的几何对象。
	virtual GEOMETRY_APIF UGbool Is3D( ) const;	
#if !defined SYMBIAN60 && !defined  OS_ANDROID && !defined (IOS)
	//! \brief Geometry与Element的转换
	//! \param pElement,返回的pElement指针在函数内分配内存,外面负责删除，否则会有内存泄漏
	virtual GEOMETRY_APIF UGbool ToElement(OgdcElement*& pElement)  const;

	//! \brief Geometry与Element的转换
	//! \param pElement,传入的Element，如果无法转换则返回False，否则返回True
	virtual GEOMETRY_APIF UGbool FromElement(const OgdcElement* pElement);
#endif
public:
	//! \param versionType GML的版本
	//! \param nPace 缩进的空格数目,使得xml字符串更加易于人工阅读; 
	//! 主要用于如果返回的xml字符串是一个大标签中的一部分, 通过指定空格数目,可以使层次清晰
	//! \return 返回指定GML版本的xml字符串
	virtual GEOMETRY_APIF UGString ToXML(GMLVersionType versionType, UGint nPace = 0)  const = 0;

	//! \brief 从XML字符串中生成Geometry
	//! \param strXML xml格式字符串
	//! \param versionType xml字符串的GML版本
	//! \return 返回指定GML版本的xml字符串
	virtual GEOMETRY_APIF UGbool FromXML(const UGString& strXML, GMLVersionType versionType) = 0;

public:
	//added by xielin ^_^ 编辑相关函数,把编辑和geometry统一

	//! \brief 获得编辑对象的句柄的数目
	//! \remarks 对于固定句柄对象为9包括了改变大小的8个控制点和一个旋转控制点
	virtual GEOMETRY_APIF UGint  GetHandleCount();

	//! \brief 获取传入的Handle在对象的子对象的索引号
	//! \remarks 对只有多个子对象的才重载这个方法
	virtual GEOMETRY_APIF UGint IndexOfHandle(UGint nHandle);
	//nHandle:的值与控制点的关系如右图                         
	//									1---+   	  +--2    	+----3	
	//                                      |         |         |  
	//                                      |         |         |  
	//										x---------x---------x     9
	//										|   ____________    |     |
	//										|  /            \   | x---+
	//									4---x |              |  x-----5
	//										|  \____________/   |
	//										|                   |
	//										x---------x---------x
	//										|		  |         |
	//										|		  |         |
	//								    6---+         +-----7   +-----8

	//! \brief 根据句柄获得相应控制点的值
	//! \param nHandle 控制点句柄
	//! \param point 句柄对应的控制点的坐标值
	//! \return 返回辅助点的类型
	virtual GEOMETRY_APIF UGint GetHandle( UGint nHandle, UGPoint2D& point);


	//! \brief 移动控制点到指定坐标,主要用于绘制对象，修改对象节点等操作
	//! \param nHandle 控制点句柄
	//! \param point 控制点坐标
	virtual GEOMETRY_APIF void MoveHandle( UGint nHandle, const UGPoint2D& pnt2D,
		UGEditType::EditToolShape nEditType=UGEditType::ET_Default);

	//! \brief 节点编辑时可用来插入一个控制点,也可以用于绘制对象时加入节点
	//! \param nHandle 插入位置
	//! \param pnt2D 插入点的地理坐标
	virtual GEOMETRY_APIF void AddHandle( UGint nHandle, const UGPoint2D& pnt2D,
		UGEditType::EditToolShape nEditType=UGEditType::ET_Default);

	//! \brief 删除编辑对象的指定节点
	//! \param nHandle 节点句柄
	virtual GEOMETRY_APIF UGuint DelNode( UGint nHandle );

	//! \brief 计算对象绘制时的辅助线，返回的是一个复合对象
	//! \param pPoints 点串
	//! \param nPntCount  点的个数
	//! \param nEditType 绘制对象的类型，参考UGEditType
	//! \param pStyle	 辅助线的风格，如果传入为NULL，用默认的风格
	virtual GEOMETRY_APIF UGGeometry*  CalAssistantLine(const UGPoint2D* pPoints,
		UGint nPntCount,UGEditType::EditToolShape nEditType,UGStyle* pStyle=NULL);


	//! \brief 计算对象绘制时的辅助点，返回的是点的数目,
	//! nHandleType返回的是辅助点的类型,参考UGEditType::EditHandleType
	//! \param pPoints 点串
	//! \param nPntCount  点的个数
	//! \param eAssistantPoints 点串,返回的辅助点点串
	//! \param nEditType 绘制对象的类型，参考UGEditType
	virtual GEOMETRY_APIF void CalAssistantPoints(const UGPoint2D* pPoints, UGint nPntCount,
		UGArray<UGEditType::EditAssistantPoint>& eAssistantPoints,
		UGEditType::EditToolShape nEditType);

	//! \brief 根据传入的点串生成对象，主要用于编辑中的参数化等没有内部数组保存点串的对象，
	//! 也用于自定义对象,对于有内部数组保存点串的对象，该函数直接return既可，
	//! 比如GeoLine等对象就直接用AddHandle来生成，效率比较高 
	//! \param pPoints 点串
	//! \param nPntCount  点的个数
	virtual GEOMETRY_APIF void MakeWithPoints(UGPoint2D* pPoints,UGint nPntCount,
		UGEditType::EditToolShape nEditType=UGEditType::ET_Default);
	//! \brief 根据传入的点串生成对象，主要用于编辑中的参数化等没有内部数组保存点串的对象，
	//! 也用于自定义对象,对于有内部数组保存点串的对象，该函数直接return既可，
	//! 比如GeoLine等对象就直接用AddHandle来生成，效率比较高 
	//! \param pPoints 3维点串
	//! \param nPntCount  点的个数
	virtual GEOMETRY_APIF void MakeWithPoints(UGPoint3D* pPoints,UGint nPntCount,
		UGEditType::EditToolShape nEditType=UGEditType::ET_Default);

	//! \brief 构造多个子对象的复杂Geometry.
	//! \param pPoints [in] 点串指针。
	//! \param pPolyCount [in] 子对象中点数的数组。
	//! \param nSubCount [in] 子对象数目。
	//! \return 所有子对象的点数都小于2个,返回false。
	virtual GEOMETRY_APIF UGbool Make( const UGPoint2D* pPoints, const UGint* pPolyCount, UGint nSubCount ){return false;}

	//! \brief 构造多个子对象的复杂Geometry.
	//! \param pPoints [in] 点串指针。
	//! \param pPolyCount [in] 子对象中点数的数组。
	//! \param nSubCount [in] 子对象数目。
	//! \return 所有子对象的点数都小于2个,返回false。
	virtual GEOMETRY_APIF UGbool Make( const UGPoint3D* pPoints, const UGint* pPolyCount, UGint nSubCount ){return false;}

	virtual GEOMETRY_APIF UGbool Make( const UGGeometry& pGeometry){return FALSE;};

	//! \brief 获取对象需要捕捉的点串
	//! \param aryPnts 点串，传出去的要捕捉的点串
	//! \return 如果是true表示有需要捕捉的点串，如果返回false表示该对象没有需要捕捉的点串
	virtual GEOMETRY_APIF UGbool GetSnapPoints(UGArray<UGPoint2D>& aryPnts);

	//! \brief 获取对象需要捕捉的线段数组
	//! \param aryLines 需要捕捉的线数组（如果有多个part，那么aryLines就应该有多少个count）
	//! \return 如果是true表示有需要捕捉的线，如果返回false表示该对象没有需要捕捉的线
	virtual GEOMETRY_APIF UGbool GetSnapLines(UGLineArray& aryLines);

	//! \brief 编辑（拉伸，编辑节点）结束后，通知geometry，对你的编辑结束了，主要针对geomap这样的对象，因为需要在结束编辑后去刷新地图内容
	virtual GEOMETRY_APIF void AfterModified();

	//added by xielin ^_^ 编辑相关函数,把编辑和geometry统一 end

	//! \brief 设置几何对象中字符串的Charset,主要针对文本对象，复合对象中的文本字符串,只set，不转换字符串内容
	virtual GEOMETRY_APIF void SetTextCharset(UGString::Charset nCharset);

	//! \brief 改变几何对象中字符串的Charset,主要针对文本对象，复合对象中的文本字符串,转换字符串内容
	virtual GEOMETRY_APIF void ChangeTextCharset(UGString::Charset nCharset);

	virtual GEOMETRY_APIF void SetGeoMapClipGeometry(const UGGeometry* pGeometry);
	virtual GEOMETRY_APIF const UGGeometry* GetGeoMapClipGeometry();

	//added by yanbg 09.1.4 
	//! \brief 判断鼠标是否处在对象的边上，以此来处理改变鼠标形状或进行其它操作
	//! \brief 例如，对于表格对象，如果鼠标处于表格的分割线上，则返回true
	//! \param pntHitTest 用来测试选中的点
	//! \param dTolerance 容限
	//! \return 选中返回true, 否则返回false
	virtual GEOMETRY_APIF UGbool HitTestEdge(const UGPoint2D &pntHitTest, UGdouble dTolerance) const;

	//! \brief		设置Bounds是否脏了
	//! \param		bBoundsDirty	Bounds是否脏, 默认为设置脏
	GEOMETRY_APIF void SetBoundsDirty(UGbool bBoundsDirty=true);

	//! \brief		判断Bounds是否脏了
	//! \return		如果脏了, 返回true; 不脏返回false
	GEOMETRY_APIF UGbool IsBoundsDirty() const;

	//! \brief		设置合法状态；UDB中直接调用各Geometry的LoadGeoData，缺一个状态设置
	GEOMETRY_APIF void SetValidState(UGint nValidState);

	virtual GEOMETRY_APIF UGPoint2D GetAnchor()const;

public:
	//! \brief 返回一个point3d来描述这个对象，对于3维点就是他本身，对于其他未定义
	virtual GEOMETRY_APIF UGPoint3D Get3DPoint();
	virtual GEOMETRY_APIF UGbool	  Set3DPoint(const UGPoint3D& pnt3d);	

	UGbool HasModifiedVertexSeq();
	//added end
public:

	//! \brief 真正计算Bounds
	//! \return 返回得到的Bounds
	virtual GEOMETRY_APIF UGRect2D ComputeBounds() const = 0;	


	void SetModifyVertexSeq(UGbool bModifyVertexSeq=TRUE);
	void SetIsClip(UGbool bClip);
	UGbool IsClip();
public:
	//计算侧地线
	static UGbool ComputeGeodesic(const UGPoint2D pntStart, const UGPoint2D pntEnd,UGGeometry *&pGeoLineResult,
		UGPrjCoordSys *pPrjCoordSys = NULL, UGint semicircleSegment = 18000);

	// 算法从Base3D中移植而来，为了解决三维数据处理时Z容限与XY容限不一致的情形。

	//! \brief 球面坐标系的点转换成笛卡尔坐标系的点。
	//! \param pnt3D [in] 球面坐标的三维点，x，y值必须是经纬数据，而且是度为单位，z值以米为单位。
	//! \return 返回以地球球心为原点的坐标系的点坐标。
	static UGPoint3D SphericalToCartesian(const UGPoint3D& pnt3D);

	//! \brief 笛卡尔坐标系的点转换成球面坐标系的点。
	//! \param pnt3D [in] 以地球球心为原点的坐标系点坐标。
	//! \return 返回的点x,y 为度为单位的经纬度坐标，z值以米为单位。
	static UGPoint3D CartesianToSpherical(const UGPoint3D& pnt3D);

protected:
	//! \brief 几何对象ID，默认为0
	UGint m_nID;

	//! \brief 几何对象自带风格，默认为NULL
	UGStyle* m_pStyle;

	//! \brief 几何对象BOUNDs
	UGRect2D m_rcBounds;
private:
	//! \brief 几何对象内部存储的Bounds是否已经脏了（和实际数据不相同了）
	UGbool m_bBoundsDirty;

	//\brief 是否修改了顶点索引序列
	UGbool m_bModifyVertexSeq;
	//是否已经被裁减过
	UGbool m_bClip;
protected:
	UGint m_nValidState;  // 对象是否合法标志，0表示未验证，1表是合法，-1表示不合法
};

}

#endif // !defined(AFX_UGGEOMETRY_H__8844E5E9_172A_4931_913C_5B1322A02D82__INCLUDED_)

