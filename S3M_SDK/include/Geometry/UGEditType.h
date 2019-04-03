// UGEditType.h: interface for the UGEditType class.
//
//=====================================================================================
// 项目名：			SuperMap Universal GIS class (UGC) Library 2.0
// 作  者：			宋关福
// 单  位：			北京超图软件股份有限公司
// 最后修改时间：	以文件日期为准
//-------------------------------------------------------------------------------------
// 重要声明：		1. 类库使用范围为Supermap GIS软件开发组成员，未经许可不得擅自传播。
//					2. 类库使用者不得修改本文件内容，否则会导致类库使用错误。
//					
//-------------------------------------------------------------------------------------
// 文件名：			UGEditType.h
// 类  名：			UGEditType
// 父  类：			
// 子  类：			
// 功能说明：		定义SuperMap中的编辑常数
// 调用说明：		
//=====================================================================================
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UGEDITTYPE_H__87D77BB2_EE0B_4864_B131_DA7B320AC881__INCLUDED_)
#define AFX_UGEDITTYPE_H__87D77BB2_EE0B_4864_B131_DA7B320AC881__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Stream/ugdefs.h"

namespace UGC {

typedef UGArray<UGPoint2D> UGPnt2DArray;
typedef UGArray<UGPnt2DArray*> UGLineArray;

class GEOMETRY_API UGEditType  
{
public:
	// 编辑工具形状（类型）
    enum EditToolShape
	{
		// 空
		ET_Default			= 0,			
		// 选择
        ET_Selection        = 1, 
		// 点
        ET_Point            = 21, 
		// 直线
        ET_LineSect         = 22,  
		// 椭圆弧
        ET_EllipseArc       = 23,
		// 折线,支持subaction
        ET_PolyLine         = 24,
		// 贝兹曲线,支持subaction
        ET_BezierCurve      = 25,	
		// 矩形
        ET_Rectangle        = 26,		
		// 圆角矩形
        ET_RoundRect        = 27,		
		// 平行四边形
        ET_Parallelogram    = 28,
		// 圆心圆
        ET_Circle           = 29,	
		// 椭圆	
        ET_Ellipse          = 30,
		// 斜椭圆
        ET_ObliqueEllipse   = 31,	
		// 多边形,支持subaction
        ET_PolyGon          = 32,
		// 多点
		ET_MulitiPoint		= 33,
		// 三点弧（圆弧）
		//ET_ARC3P			= 33,
		// 注记
        ET_Text             = 34,
		// B样条,支持subaction
        ET_BSpline          = 35,			
		//等值线
        //sET_SameDistline     = 36,
		//沿线注记	
        ET_StreamText       = 37,			
		//多段线,已废除，请使用ET_PolyLineEx、ET_PolygonEx代替
		//ET_Mulitiline       = 38,         
		//路径
		ET_Path				= 39,           
		ET_PolyLineEx       = 42,
		ET_PolygonEx        = 43,
		//三点弧
		ET_Arc3P            = 44,           
		//三点圆
		ET_Circle3P         = 45,           
		//派对象 
		ET_Pie              = 46,           
		//两点圆
		ET_Circle2P			= 47,           
		//平行线,支持subaction
		ET_Parallel         = 48,			
		//扩展对象
		//ET_Custom           = 49,	        
		//半自动跟踪线
		ET_TrackPolyLine	= 55,			
		//半自动跟踪回退
		ET_TrackBreak		= 56,			
		//跟踪面
		ET_TrackPolyPolygon	= 57,			
		//画笔，自由画线，支持subaction		
		ET_FreeLine			= 58,			
		//Cardinal曲线，支持subaction
        ET_Cardinal		    = 59,		
		//Hatch面
		ET_Hatch			= 60,	
		//测地线
		ET_Geodesic			= 61,
		//设置选中对象的捕捉点，移动的时候就用设置的这个捕捉点捕捉，否则就用鼠标点
		ET_SetSnapPoint		= 70,			
		//图片对象(GeoPicture)
		ET_Picture			= 1101,			
		//geomap对象，布局中
		ET_Map				= 2001,			
		//GeoTable对象,布局中
		ET_Table			= 2002,			
		//geomapRegion对象，布局中
		//ET_MapRegion		= 2004,			
		//geoscale对象，布局中
		ET_Scale			= 2005,			
		//geomap对象，布局中
		ET_Direction		= 2008,			
		//GeoArtText对象, 布局中
		ET_ArtText			= 2010,
		//GeoLegend对象,布局中
		ET_Legend			= 2011,

		ET_GraphicObject	= 9001			
    };

    enum EditToolType 
	{
        EditTool_Select ,
        EditTool_FixHandle ,
        EditTool_DynHandle ,
        EditTool_Text      ,
    };

	//选择模式
    enum EditSelectMode
	{
		//空
        ESM_None,		
		//移动
        ESM_Move,				
		//移动节点
        ESM_MoveNode,			
		//改变大小
        ESM_Size,				
		//移动旋转基点
		ESM_MoveRotateBase,
		//移动旋转改变大小
		ESM_MoveSizeRotate, 
    };

	//跟踪状态
	enum EditTrackerState
	{
        ETS_Normal,		
        ETS_Selected,
        ETS_CtrlPoints
    };
	
	//编辑交互方式
	enum EditAccessibilityAction
	{
		//无
        EAA_None,	
		//编辑节点
        EAA_EditPoint,			
		//添加节点
        EAA_AddPoint,
		//面
        EAA_RgnMode,			
		//线
        EAA_LineMode
    };

	//删除节点返回值
    enum EditDelNodeReturnValue
	{
		//无变动
        EDHRV_NoChange,		
		//已经被删除
        EDHRV_Deleted,			
		//对象被删除
        EDHRV_DelObj,			
    };

	//节点编辑模式
    enum EditDragNodeMode
	{
		//无
        EDHM_NoNode = 0,		
		//对单个节点进行操作
        EDHM_SingleNode = 1,	
		//选择节点方式
        EDHM_LessNode = 2,		
		//选择节点方式
        EDHM_MoreNode = 3,		
    };

	//撤销缓冲区类型
    enum EditUndoMode
	{
		//无法撤销
        EUNM_CanntUndo   ,	
		//新对象
        EUNM_NewObj      ,		
		//删除对象
        EUNM_DelObj      ,		
		//添加子对象
        EUNM_AddSubObj   ,		
		//删除子对象
        EUNM_DelSubObj   ,		
		//添加节点
        EUNM_AddNode     ,		
		//删除节点
        EUNM_DelNode     ,		
		//移动节点
        EUNM_MoveNode    ,		
		//移动对象
        EUNM_Move        ,		
		//改变对象大小
        EUNM_Size        ,		
		//旋转对象
        EUNM_Rotate      ,		
		//注记对象文本发生变化
		EUNM_CharChange  ,		
		//保存新的对象
        EUNM_SaveNewObj  ,		
		//保存旧对象
        EUNM_SaveOldObj  ,		
    };
    
	//捕捉对象类型
    enum EditSnapObjType
	{
		//点对象
        ESOT_PointObj   ,	
		//线或面对象
        ESOT_LineRgnObj ,		
		//特殊对象
        ESOT_SpecialObj 		
    };
    
	//镜像方式
    enum EditMirrorType
	{
		//水平镜像
		EMiRT_Horizontal = 0,	
		//垂直镜像
        EMiRT_Vertical   = 1,	
		//从左上到右下镜像
        EMiRT_LeftTop    = 2,	
		//从右下到左上镜像
        EMiRT_RightTop   = 3	
    };

	enum CursorType
	{
		//箭头
		ecArrow,		
		//十字
        ecCross,				
		//旋转
        ecRotation,				
		//工字
        ecBeam,					
		//移动
        ecMove,					
		//南北移动
		ecNorthSouth,			
		//东西移动
		ecWestEast,				
		//西北到东南的移动
		ecNorthwestSoutheast,   
		//东北到西南的移动
		ecNortheastSoutnwest,   
		//点捕捉
		ecCrossSnapPoint,		
		//顶点捕捉
		ecCrossSnapVertex,		
		//线段中点捕捉
		ecCrossSnapMidpoint,	
		//线上捕捉
		ecCrossSnapLine,		
		//垂直捕捉
		ecCrossSnapVertical,	
		//平行捕捉
		ecCrossSnapParallel,	
		//旋转基点
		ecMoveRotateBase,		

    };//编辑工具光标类型

	//编辑handle的类型，暂时分为三类，一种是普通的handle，一种是改变了颜色的handle
	//主要用于弧段等对象修改长短轴的弧段，一种是辅助点类型，画一个小叉
	enum EditHandleType
	{
		//表示没有对应的handle
		UnKnown          = -1,  
		NormalHandle  = 1,
		LittleHandle  = 2,
		ControlHandle = 3,
		MiddleHandle = 4, // 线段中间点
	};

	// 编辑的辅助点，包括点坐标和辅助点类型
	struct EditAssistantPoint 
	{
		//点坐标和
		EditHandleType nType;
		//辅助点类型
		UGPoint2D	pnt2D;
	};
};

}

#endif // !defined(AFX_UGEDITTYPE_H__87D77BB2_EE0B_4864_B131_DA7B320AC881__INCLUDED_)

