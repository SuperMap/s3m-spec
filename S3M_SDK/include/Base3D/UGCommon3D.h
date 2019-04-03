//! \brief//! \brief//! \brief//! \brief//! \brief//! \brief//! \brief//! \brief//! \brief//! \brief//! \brief//! \brief//! \brief//! \brief//! \brief//! \brief//! \brief//! \brief//! \brief//! \brief//! \brief//! \brief//! \brief//! \brief//
//                    _   _   ____   ____                    
//   Project         | | | | / ___) / ___)                   
//                   | | | || | __ | |                       
//                   | |_| || |_\ || \___                    
//                    \____/ \____| \ ___)     6.0           
//                                                           
//!  \file headerfile.h
//!  \brief 三维使用的枚举和常量的定义文件。
//!  \details 封装了三维使用的枚举和常量的定义文件。
//!  \author 。
//!  \attention 
//!   Copyright (c) 1996-2010 SuperMap Software Co., Ltd. <br> 
//!   All Rights Reserved.
//!  \version 6.0
//! \brief//! \brief//! \brief//! \brief//! \brief//! \brief//! \brief//! \brief//! \brief//! \brief//! \brief//! \brief//! \brief//! \brief//! \brief//! \brief//! \brief//! \brief//! \brief//! \brief//! \brief//! \brief//! \brief//! \brief//

#ifndef UG3DCOMMON_H
#define UG3DCOMMON_H

#include "Base3D/UGPrerequisites3D.h"

namespace UGC
{

static UGString g_stStringBlank = _U("");

#define MAX_GRID_VERTICE_NUM  16

#define COARSEST_GRID_VERTICE_NUM  8

#define FINEST_GRID_VERTICE_NUM  64

#define BLOCK_BORDER_WIDTH 8

//! \brief 地球半径。
#undef GLOBAL_RADIUS
#define GLOBAL_RADIUS  6378137.0

//! \brief 地球短轴半径
#undef GLOBAL_RADIUS_MINOR
#define GLOBAL_RADIUS_MINOR 6356752.3142451793

//! \brief 多重纹理最大个数。
#define SMSCN_MAX_TEXTURE_COORD_SETS 8
//! \brief 无效值定义。
#define VALUE_NAN                    UG_FLTMAX
//! \brief  浮点型阈值    
#define FLOAT_EPSILON                1.401298e-45
//! \brief 纹理缓存里存放的最大个数
#define MAX_TEXTURE_NUM              10000
//! \brief 三维场景最大灯光数目。
#define MAX_LIGHTS                   8        // 定义光源最多个数
//! \brief 每个地形块的最大顶点数目。
#define MAX_VERTEX_COUNT             4096  
//! \brief 相机能进入最小高度  
#define MIN_ALTITUDE                 50000

//! \brief 平面0层tile的大小 pow(2.0,30)  
#define LOD0_TILE_SIZE 1073741824

//#define LEVEL_WIDTH 2
//#define MAX_LEVEL_NUM 6
//! \brief 每层由 LEVEL_WIDTH×LEVEL_WIDTH个地形块构成
#define LEVEL_WIDTH 4
//! \brief 视阈内可见层数
#define MAX_LEVEL_NUM 7
//! \brief 影像层数
#define MAX_IMAGE_LEVEL_NUM 10
#define MULTILEVEL_LEVEL_OFFSET 4
//! \brief 高程纹理大小
#define ELEVATION_TEXTURE_SIZE 512
//! \brief 每个Tile上的地形小纹理的边长
#define TILE_ELEVATION_TEXTURE_SIZE 128

//!\brief 全球格网剖分的最大层数
#define GLOBAL_GRID_MAX_LEVEL 22

#define NONEARTH_GRID_MAX_LEVEL 30

#define MORPH_FRAME_NUM 5
#define EAST_DIRECTION 1
#define SOUTH_DIRECTION 2
#define WEST_DIRECTION 4
#define NORTH_DIRECTION 8
#define NORTH_EAST_DIRECTION (NORTH_DIRECTION|EAST_DIRECTION)
#define SOUTH_EAST_DIRECTION (SOUTH_DIRECTION|EAST_DIRECTION)
#define NORTH_WEST_DIRECTION (NORTH_DIRECTION|WEST_DIRECTION)
#define SOUTH_WEST_DIRECTION (SOUTH_DIRECTION|WEST_DIRECTION)

//! \brief 颜色插值影响器包含的最大阶段数
#define MAX_COLOR_INTERPOLATOR_STAGES 6

//! \brief 草图模式材质
#define SKETCHMATERIAL _U("Ogre/SketchModel")

//! \brief 顶点捕捉材质名
#define SNAPPOINT _U("SnapPoint")

//! \brief 渲染优先级间隔
#define RENDER_PRIORITY_INTERVAL 5

//! \brief 允许三角剖分的顶点个数
#define MAX_TRIANGULATE_NUM 65535

//! \brief 历史文件夹格式
#define HISTORY_NAME_FORMAT _U("%Y.%m.%d.%H.%M.%S")

//! \brief 纹理坐标
#define GEN_S_BIT  1
#define GEN_T_BIT  2
#define GEN_Q_BIT  4
#define GEN_R_BIT  8

//! \brief 纹理坐标映射类型 
#define GEN_OBJECT_LINEAR   16
#define GEN_EYE_LINEAR		32
#define GEN_SPHERE_MAP      64
#define GEN_REFLECTION_MAP  128
#define GEN_NORMAL_MAP      256


//! \brief
enum UGSnapMode3D
{
	//! \不捕捉
	SM3D_NONE,
	//! \捕捉点
	SM3D_POINT
};


//! \brief Enumerates the wave types usable with the Ogre engine. 
enum WaveformType
{
    //! \brief Standard sine wave which smoothly changes from low to high and back again.
    WFT_SINE,
    //! \brief An angular wave with a constant increase / decrease speed with pointed peaks.
    WFT_TRIANGLE,
    //! \brief Half of the time is spent at the min, half at the max with instant transition between.
    WFT_SQUARE,
    //! \brief Gradual steady increase from min to max over the period with an instant return to min at the end.
    WFT_SAWTOOTH,
    //! \brief Gradual steady decrease from max to min over the period, with an instant return to max at the end.
    WFT_INVERSE_SAWTOOTH,
	//! \brief Pulse Width Modulation. Works like WFT_SQUARE, except the high to low transition is controlled by duty cycle. 
	//! \brief With a duty cycle of 50% (0.5) will give the same output as WFT_SQUARE.
	WFT_PWM
};

//! \brief Filtering options for textures / mipmaps. 
enum FilterOptions
{
    //! \brief No filtering, used for FILT_MIP to turn off mipmapping
    FO_NONE,
    //! \brief Use the closest pixel
    FO_POINT,
    //! \brief Average of a 2x2 pixel area, denotes bilinear for MIN and MAG, trilinear for MIP
    FO_LINEAR,
	/// Equal to: min=FO_LINEAR, mag=FO_LINEAR, mip=FO_LINEAR
	FO_TRILINEAR,
    //! \brief Similar to FO_LINEAR, but compensates for the angle of the texture plane
    FO_ANISOTROPIC
};

//! \brief
enum UGFilterType
{
    //! \brief The filter used when shrinking a texture
    FT_MIN,
    //! \brief The filter used when magnifiying a texture
    FT_MAG,
    //! \brief The filter used when determining the mipmap
    FT_MIP
};
//! \brief
enum UG3DPixelFormat
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

/** Enum identifying special mipmap numbers
*/
enum TextureMipmap
{
	/// Generate mipmaps up to 1x1
	MIP_UNLIMITED = 0x7FFFFFFF,
	/// Use TextureManager default
	MIP_DEFAULT = -1
};

//! \brief Defines the frame buffer types. 
enum FrameBufferType {
    FBT_COLOUR  = 0x1,
    FBT_DEPTH   = 0x2,
    FBT_STENCIL = 0x4
};
//! \brief 阴影模式
enum ShadeOptions
{
    SO_FLAT,
    SO_GOURAUD,
    SO_PHONG
};
//! \brief 比较方式
enum CompareFunction
{
    CMPF_ALWAYS_FAIL,
    CMPF_ALWAYS_PASS,
    CMPF_LESS,
    CMPF_LESS_EQUAL,
    CMPF_EQUAL,
    CMPF_NOT_EQUAL,
    CMPF_GREATER_EQUAL,
    CMPF_GREATER
};
//! \brief 渲染引擎用的混合模式
enum SceneBlendFactor
{
    SBF_ONE,
    SBF_ZERO,
    SBF_DEST_COLOUR,
    SBF_SOURCE_COLOUR,
    SBF_ONE_MINUS_DEST_COLOUR,
    SBF_ONE_MINUS_SOURCE_COLOUR,
    SBF_DEST_ALPHA,
    SBF_SOURCE_ALPHA,
    SBF_ONE_MINUS_DEST_ALPHA,
    SBF_ONE_MINUS_SOURCE_ALPHA

};

/** Types of blending that you can specify between an object and the existing contents of the scene.*/
enum SceneBlendType
{
	/// Make the object transparent based on the final alpha values in the texture
	SBT_TRANSPARENT_ALPHA,
	/// Make the object transparent based on the colour values in the texture (brighter = more opaque)
	SBT_TRANSPARENT_COLOUR,
	/// Add the texture values to the existing scene content
	SBT_ADD,
	/// Multiply the 2 colours together
	SBT_MODULATE,
	/// The default blend mode where source replaces destination
	SBT_REPLACE
	// TODO : more
};

/** Blending operations controls how objects are blended into the scene. The default operation
is add (+) but by changing this you can change how drawn objects are blended into the
existing scene.
*/
enum SceneBlendOperation
{
	SBO_ADD,
	SBO_SUBTRACT,
	SBO_REVERSE_SUBTRACT,
	SBO_MIN,
	SBO_MAX
};

/** List of valid texture blending operations, for use with TextureUnitState::setColourOperation.
@remarks
This list is a more limited list than LayerBlendOperationEx because it only
includes operations that are supportable in both multipass and multitexture
rendering and thus provides automatic fallback if multitexture hardware
is lacking or insufficient.
*/
enum LayerBlendOperation {
	/// Replace all colour with texture with no adjustment
	LBO_REPLACE,
	/// Add colour components together.
	LBO_ADD,
	/// Multiply colour components together.
	LBO_MODULATE,
	/// Blend based on texture alpha
	LBO_ALPHA_BLEND

};

//! \brief 渲染引擎用的的裁剪模式
enum CullingMode
{
    //! \brief Hardware never culls triangles and renders everything it receives.
    CULL_NONE = 1,
    //! \brief Hardware culls triangles whose vertices are listed clockwise in the view (default).
    CULL_CLOCKWISE = 2,
    //! \brief Hardware culls triangles whose vertices are listed anticlockwise in the view.
    CULL_ANTICLOCKWISE = 3
};

/** Manual culling modes based on vertex normals.
This setting applies to how the software culls triangles before sending them to the 
hardware API. This culling mode is used by scene managers which choose to implement it -
normally those which deal with large amounts of fixed world geometry which is often 
planar (software culling movable variable geometry is expensive). */
enum ManualCullingMode
{
	/// No culling so everything is sent to the hardware.
	MANUAL_CULL_NONE = 1,
	/// Cull triangles whose normal is pointing away from the camera (default).
	MANUAL_CULL_BACK = 2,
	/// Cull triangles whose normal is pointing towards the camera.
	MANUAL_CULL_FRONT = 3
};

//! \brief 渲染引擎用的多边形显示模式
enum PolygonMode
{
	//! \brief Only points are rendered.
    PM_POINTS = 1,
	//! \brief Wireframe models are rendered.
    PM_WIREFRAME = 2,
	//! \brief Solid polygons are rendered.
    PM_SOLID = 3
};

//! \brief 图像绘制的反走样模式
enum SmoothHintMode
{
	//不使用抗锯齿
	SHM_NONE,
	//由具体OpenGL的实现来决定作用那种方式达到点，线的平滑效果
	SHM_DONT_CARE,
	//运行速度最快
	SHM_FASTEST,
	//显示效果最好
	SHM_NICEST
};

//! \brief 渲染引擎用的雾的计算模式
enum FogMode
{
    //! \brief No fog. Duh.
    FOG_NONE,
    //! \brief Fog density increases  exponentially from the camera (fog = 1/e^(distance * density))
    FOG_EXP,
    //! \brief Fog density increases at the square of FOG_EXP, i.e. even quicker (fog = 1/e^(distance * density)^2)
    FOG_EXP2,
    //! \brief Fog density increases linearly between the start and end distances
    FOG_LINEAR
};
//! \brief 渲染引擎用的的向量的计算模式
enum SceneNormalMode
{
	//! \brief 
	SNM_NONE = 1,
	//! \brief 平板
    SNM_FLAT = 2,
	//! \brief 平滑模式
    SNM_SMOOTH = 3
};

//! \brief 渲染引擎用的的面的正面环绕方向
enum PolygonFrontFace
{
	//! \brief 两面都有
	PFF_NONE = 1,
	//! \brief 顺时针
    PFF_CW = 2,
	//! \brief 逆时针.
    PFF_CCW = 3,
	//! \brief 表示使用数据默认的环绕方式
	PFF_DEFAULT = 4,
};

// Categorisation of passes for the purpose of additive lighting
enum IlluminationStage
{
	/// Part of the rendering which occurs without any kind of direct lighting
	IS_AMBIENT,
	/// Part of the rendering which occurs per light
	IS_PER_LIGHT,
	/// Post-lighting rendering
	IS_DECAL, 
	/// Not determined
	IS_UNKNOWN
};

enum LightTypes
{
	//! \brief 点光源
	LT_POINT,
	//! \brief 方向光源
	LT_DIRECTIONAL,
	//! \brief 聚光光源
	LT_SPOTLIGHT,
};

//! \brief 渲染引擎用的纹理的使用模式
enum TextureType
{
	//! \brief 1D texture, used in combination with 1D texture coordinates
	TEX_TYPE_1D = 1,
	//! \brief 2D texture, used in combination with 2D texture coordinates (default)
	TEX_TYPE_2D = 2,
	//! \brief 3D texture, used in combination with 3D texture coordinates
	TEX_TYPE_3D = 3,
	//! \brief 3D cube map, used in combination with 3D texture coordinates
	TEX_TYPE_CUBE_MAP = 4,
};

//! \brief 数据绘制的方式
enum OperationType
{
	/// A list of points, 1 vertex per point
	OT_POINT_LIST = 1,
	/// A list of lines, 2 vertices per line
	OT_LINE_LIST = 2,
	/// A strip of connected lines, 1 vertex per line plus 1 start vertex
	OT_LINE_STRIP = 3,
	/// A list of triangles, 3 vertices per triangle
	OT_TRIANGLE_LIST = 4,
	/// A strip of triangles, 3 vertices for the first triangle, and 1 per triangle after that 
	OT_TRIANGLE_STRIP = 5,
	/// A fan of triangles, 3 vertices for the first triangle, and 1 per triangle after that
	OT_TRIANGLE_FAN = 6,

	OT_QUAD_STRIP = 8,

	OT_QUAD_LIST = 9,

	OT_POLYGON = 10,

	OT_PICTURE = 12,

};

//! \brief 索引的类型
enum VertexIndexType
{
	IT_16BIT,
	IT_32BIT,

	//! \brief 附带属性的索引号
	IT_16BIT_2,
	IT_32BIT_2,
};

//! \brief 数据绘制的属性
enum VertexOptions
{
	VO_NORMALS = 1,                  // GL_NORMAL_ARRAY + glNormalPointer()
	VO_TEXTURE_COORDS = 2,           // GL_TEXTURE_COORD_ARRAY + glTexCoordPointer()
	VO_DIFFUSE_COLOURS = 4,          // GL_COLOR_ARRAY + glColorPointer()
	VO_SPECULAR_COLOURS = 8,         // Secondary Color
	VO_BLEND_WEIGHTS = 16,      
	VO_USE_SINGLE_COLOR = 32,        // use only one kind of color
	VO_USE_POINT_SMOOTHING = 64,     // 启动点反走样
	VO_MATERIAL = 128,               // 使用材质
	VO_TEXTURE_COLOR = 256,          // 使用材质
	VO_VERTEX_DOUBLE = 512,          // 使用Double类型的顶点
	VO_TEXTURE_COORD_Z_IS_MATRIX = 1024,  // 表示顶点属性的Z值是一个矩阵
};

//! \brief 混合因子
enum BlendFactor
{
	BF_ZERO						= 1,
	BF_ONE						= 2,
	BF_SRC_COLOR				= 3,
	BF_ONE_MINUS_SRC_COLOR		= 4,
	BF_DST_COLOR				= 5,
	BF_ONE_MINUS_DST_COLOR		= 6,
	BF_SRC_ALPHA				= 7,
	BF_ONE_MINUS_SRC_ALPHA		= 8,
	BF_DST_ALPHA				= 9,
	BF_ONE_MINUS_DST_ALPHA		= 10,
	BF_CONSTANT_COLOR			= 11,
	BF_ONE_MINUS_CONSTANT_COLOR	= 12,
	BF_CONSTANT_ALPHA			= 13,
	BF_ONE_MINUS_CONSTANT_ALPHA	= 14,
	BF_SRC_ALPHA_SATURATE		= 15,
};

//粒子系统中力矢量对速度矢量的作用方式
enum ForceApplication
{
	ADD,		//累加
	AVERAGE     //平均
};

/** The type of billboard to use. */
enum UGBillboardType
{
	/// Standard point billboard (default), always faces the camera completely and is always upright
	UGC_BBT_POINT,
	/// Billboards are oriented around a shared direction vector (used as Y axis) and only rotate around this to face the camera
	UGC_BBT_ORIENTED_COMMON,
	/// Billboards are oriented around their own direction vector (their own Y axis) and only rotate around this to face the camera
	UGC_BBT_ORIENTED_SELF,
	/// Billboards are perpendicular to a shared direction vector (used as Z axis, the facing direction) and X, Y axis are determined by a shared up-vertor
	UGC_BBT_PERPENDICULAR_COMMON,
	/// Billboards are perpendicular to their own direction vector (their own Z axis, the facing direction) and X, Y axis are determined by a shared up-vertor
	UGC_BBT_PERPENDICULAR_SELF
};
    
enum HashFunction
{
	/** Try to minimise the number of texture changes. */
	MIN_TEXTURE_CHANGE,
	/** Try to minimise the number of GPU program changes.
	@note Only really useful if you use GPU programs for all of your
	materials. 
	*/
	MIN_GPU_PROGRAM_CHANGE,
};

//! \brief 顶点颜色跟踪的材质属性
enum TrackVertexColourType
{
	TVC_NONE        = 0x0,
	TVC_AMBIENT     = 0x1,        
	TVC_DIFFUSE     = 0x2,
	TVC_SPECULAR    = 0x4,
	TVC_EMISSIVE    = 0x8
};

//! \brief 描述Overlay元素的位置/尺寸信息以什么单位记录
enum GuiMetricsMode
{
	GMM_RELATIVE,				//_U('left'), _U('top'), _U('height') and _U('width')等参数转化到0.0到1.0中，表示在父容器/窗体中的相对位置
	GMM_PIXELS					//位置和尺寸以绝对像素坐标方式表示
};

//! \brief GPU 上可以运行的程序类型
enum GpuProgramType
{
	GPT_VERTEX_PROGRAM,
	GPT_FRAGMENT_PROGRAM,
	GPT_GEOMETRY_PROGRAM
};

//! \brief 依地形的三种模式定义
/*
enum AltitudeMode
{
	//! \brief  依地形
	ClampToGround = 0,
	//! \brief  相对地形
	RelativeToGround = 1,
	//! \brief  绝对高度
	Absolute = 2,				// 点和文本的拉伸可以设置拉伸高度，也可以设置成此模式
};*/


//! \brief 绘制模式定义
enum DrawMode
{
	//! \brief  真三维绘制
	DRAW3D = 0,
	//! \brief  实时栅格化
	REAL_TIME_RASTER = 1,	
	//! \brief  绘制到屏幕
	DRAW_SCREEN = 2,
	//! \brief  绘制实时高程
	REAL_TIME_ELEVATION = 3,
	//! \brief  绘制地形法线纹理的纹理
	REAL_TIME_ELEVATION_NORMAL = 4,
	//! \brief  OSGB模型绘制高程
	REAL_TIME_ELEVATION_OSGB = 5,
	//! \brief  地形量算绘制高程
	REAL_TIME_ELEVATION_RULER = 6,
	//! \brief 选择场景绘制
	DRAW_SELECTION = 7,
	//! \brief 顶点捕捉场景绘制
	DRAW_VERTEX = 8,
	//! \brief 绘制查询结果
	DRAW_QUERY_RESULT = 9,
	//! \brief 高度图绘制
	DRAW_HEIGHT = 10,
	//! \brief  实时栅格化海洋边界
	REAL_TIME_RASTER_OCEAN = 11,
	//! \brief 渲染到布告板纹理上
	DRAW_BILLBOARD = 12,
	//! \brief 渲染到法线和深度纹理上
	DRAW_NORMAL_DEPTH = 13,
};

/*
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

//! \brief 布告板模式
enum UGBillBoardMode
{
	SCREEN_ALIGNED,//对齐于屏幕的，始终面向观察点
	AUTO_Z_AXIAL,//轴对齐布告板,围绕某个固定轴旋转
	FIXED_ANGLE//固定角度的（目前是贴地，具体角度以后可再开接口）
};*/


//! 图像采样的方法
enum UGImageSamplerType
{
	IMAGE3D_RESAMPLER_NONE             = 0, // 不做采样      不做任何操作
	IMAGE3D_RESAMPLER_NEAREST_NEIGHBOR = 1, // 最临近采样    实现
	IMAGE3D_RESAMPLER_BILINEAR         = 2, // 双线性采样    实现
	IMAGE3D_RESAMPLER_BICUBIC          = 3, // 立方卷积采样  未来实现
};

//! \brief 定义渲染引擎枚举常量
typedef enum _Graphics3DType
{
	//! \brief 无效类型
	RST_NONE = 0, 
		//! \brief OpenGL渲染引擎
		RST_OGL = 1, 
		//! \brief D3D渲染引擎
		RST_D3D = 2,
		//! \brief OGRE渲染引擎
		RST_OGRE = 3,
}Graphics3DType;

enum SceneType
{
	ST_EARTH_SPHERICAL   = 0,       //球场景
	ST_EARTH_PROJECTION = 1,	//平面的投影场景
	ST_NONEARTH  = 2,           //平面的局部坐标场景
};

//! \biref 全球形状
enum GlobalShape
{
	//! \brief 圆球
	GS_SPHERE,
	//! \brief 椭球
	GS_ELLIPSOID
};

/*
enum UGLineStyle3D
{
	//! \brief 普通线
	LINE_SIMPLE = 0, 
	//! \brief 三维管线
	LINE_PIPELINE = 1, 
};*/



//! 支持立体显示模式
enum StereoMode
{
	ANAGLYPHIC,//互补色方式
	QUAD_BUFFER,//四缓存方式
	HORIZONTAL_SPLIT,//水平分割
	VERTICAL_SPLIT,//垂直分割
	LEFT_EYE,//左眼用
	RIGHT_EYE,//右眼用
	HORIZONTAL_INTERLACE,//水平交错
	VERTICAL_INTERLACE,//垂直交错
	CHECKERBOARD,//棋盘式交错，用于DLP显示器
	OCULUS_VR, //oculus dk2虚拟现实头盔
	HTCVIVE_VR, //HTC VIVE虚拟现实头盔
	AUTOSTEREOSCOPIC3D//裸眼3D
};

//! 支持的立体视差模式
enum ParallaxMode
{
	NEGATIVE_PARALLAX, //负视差，即景物在屏幕前
	POSITIVE_PARALLAX  //正视差，即景物在屏幕后
};

//! 支持的飞行站点行为
enum UGStopMode
{
	FLY_STOP_WAIT,		//飞行到站点后等待
	FLY_STOP_ROTATION	//飞行到站点后绕站点飞行
};

//! 支持的飞行摄像机
enum UGCameraType
{
	CT_GLOBAL,			//球面相机
	CT_LOCAL,			//平面相机	
};

//! 飞行状态
enum UGFlyStatus
{
	STOP,
	PAUSE,
	PLAY
};

/** Enumeration of the types of constant we may encounter in programs. 
@note Low-level programs, by definition, will always use either
float4 or int4 constant types since that is the fundamental underlying
type in assembler.
*/
enum GpuConstantType
{
	GCT_FLOAT1 = 1,
	GCT_FLOAT2 = 2,
	GCT_FLOAT3 = 3,
	GCT_FLOAT4 = 4,
	GCT_SAMPLER1D = 5,
	GCT_SAMPLER2D = 6,
	GCT_SAMPLER3D = 7,
	GCT_SAMPLERCUBE = 8,
	GCT_SAMPLERRECT = 9,
	GCT_SAMPLER1DSHADOW = 10,
	GCT_SAMPLER2DSHADOW = 11,
	GCT_SAMPLER2DARRAY = 12,
	GCT_MATRIX_2X2 = 13,
	GCT_MATRIX_2X3 = 14,
	GCT_MATRIX_2X4 = 15,
	GCT_MATRIX_3X2 = 16,
	GCT_MATRIX_3X3 = 17,
	GCT_MATRIX_3X4 = 18,
	GCT_MATRIX_4X2 = 19,
	GCT_MATRIX_4X3 = 20,
	GCT_MATRIX_4X4 = 21,
	GCT_INT1 = 22,
	GCT_INT2 = 23,
	GCT_INT3 = 24,
	GCT_INT4 = 25,
	GCT_SUBROUTINE = 26,
	GCT_DOUBLE1 = 27,
	GCT_DOUBLE2 = 28,
	GCT_DOUBLE3 = 29,
	GCT_DOUBLE4 = 30,
	GCT_MATRIX_DOUBLE_2X2 = 31,
	GCT_MATRIX_DOUBLE_2X3 = 32,
	GCT_MATRIX_DOUBLE_2X4 = 33,
	GCT_MATRIX_DOUBLE_3X2 = 34,
	GCT_MATRIX_DOUBLE_3X3 = 35,
	GCT_MATRIX_DOUBLE_3X4 = 36,
	GCT_MATRIX_DOUBLE_4X2 = 37,
	GCT_MATRIX_DOUBLE_4X3 = 38,
	GCT_MATRIX_DOUBLE_4X4 = 39,
	GCT_VECTOR3D = 40,
	GCT_VECTOR4D = 41,
	GCT_DOUBLE = 42,
	GCT_COLOR =43,
	GCT_UNKNOWN = 99
};

/** Enumeration of GPU vendors. */
enum GPUVendor
{
	GPU_UNKNOWN = 0,
	GPU_NVIDIA = 1,
	GPU_ATI = 2, 
	GPU_INTEL = 3,
	GPU_S3 = 4,
	GPU_MATROX = 5,
	GPU_3DLABS = 6,
	GPU_SIS = 7,
	GPU_IMAGINATION_TECHNOLOGIES = 8,
	GPU_APPLE = 9,  // Apple Software Renderer
	GPU_NOKIA = 10,
	/// placeholder
	GPU_VENDOR_COUNT = 11
};

/// Enum describing the various actions which can be taken onthe stencil buffer

//! \brief 模板缓存操作枚举常量。
enum StencilOperation
{
	/// Leave the stencil buffer unchanged
	// 保存模板缓存值不变
	SOP_KEEP,

	/// Set the stencil value to zero
	// 设置模板缓存值清空为零
	SOP_ZERO,

	/// Set the stencil value to the reference value
	// 替换原来的模板缓存
	SOP_REPLACE,

	/// Increase the stencil value by 1, clamping at the maximum value
	// 模板缓存值增加一
	SOP_INCREMENT,

	/// Decrease the stencil value by 1, clamping at 0
	// 模板缓存值减少一
	SOP_DECREMENT,

	/// Increase the stencil value by 1, wrapping back to 0 when incrementing the maximum value
	// 模板缓存值增加一，若超过最大值则赋值为零
	SOP_INCREMENT_WRAP,

	/// Decrease the stencil value by 1, wrapping when decrementing 0
	// 模板缓存值减少一，若达到最小值则不再减少
	SOP_DECREMENT_WRAP,

	/// Invert the bits of the stencil buffer
	// 模板缓存值位取反
	SOP_INVERT

};

enum TerrainRenderType
{
	TRT_CPU,
	TRT_GPU,
};

//! \brief 状态类型
enum PassStateSet
{
	PST_ALL						= 0xFFFFFFFF,		// 全部状态
	PST_Point					= 0x1,				// 点状态
	PST_Line					= 0x2,				// 线状态
	PST_CullingMode			= 0x4,				// 裁剪状态
	PST_PolygonMode		= 0x8,				// 绘制模式状态
	PST_MaterialColor			= 0x10,				// 材质颜色状态
	PST_Fog						= 0x20,				// 雾状态
	PST_Blend					= 0x40,				// 混合状态
	PST_AlphaReject			= 0x80,				// alpha测试状态
	PST_Depth					= 0x100,				// 深度状态
	PST_TransparentSort	= 0x200,				// 透明排序状态
	PST_DepthBias				= 0x400,				// 多边形偏移状态
	PST_ColorWrite			= 0x800,				// 颜色写入
	PST_Stencil					= 0x1000,			// 蒙版状态
	PST_Program				= 0x2000,			// 着色器状态
	PST_Texture				= 0x4000,			// 纹理状态	
	PST_HashPriority			= 0x8000,			// 哈希值状态
	PST_Light					= 0x10000			//光照状态
};

// 渲染状态
enum RenderStateSet
{
	RSS_ALL							= 0xFFFFFFFF,		// 在所有场景中渲染
	RSS_Normal						= 0x1,				// 只正常场景渲染
	RSS_Stencil						= 0x2,				// 只在蒙版场景渲染
	RSS_Selection					= 0x4,				// 只在选择场景渲染
	RSS_RayCastSelection			= 0x8,				// 只在射线选择场景渲染
	RSS_VertexSelection				= 0x10,				// 只在顶点捕捉场景渲染
	RSS_Query_Result				= 0x20,				// 只在绘制查询结果场景渲染
	RSS_Height						= 0x40,				// 只在绘制高度场景渲染
	RSS_NormalAndDepth		= 0x80,				// 只在绘制法线和深度场景渲染
};

//! \brief 顶点状态类型
enum VertexDataStateSet
{
	VST_ALL							= 0xFFFFFFFF,		// 全部状态
	VST_Vertex						= 0x1,				// 顶点状态
	VST_Normal						= 0x2,				// 法线状态
	VST_VertexColor				= 0x4,				// 顶点颜色状态
	VST_VertexSecondColor	= 0x8,				// 顶点第二颜色状态
	VST_TexCoord					= 0x10,				// 纹理坐标状态
};

// 阴影类型
enum ShadowType
{
	STP_NONE			= 0,		// 没有物体产生阴影
	STP_SELECTION		= 1,		// 只有选中的物体产生阴影
	STP_ALL				= 2			// 全部物体产生阴影
};

//! \brief 插值类型
enum InterpolationMode
{	 
	IM_LINEAR = 0,
	IM_SPLINE = 1
};
//! \brief 旋转插值类型
enum RotationInterpolationMode
{	
	RIM_LINEAR	  = 0,
	RIM_SPHERICAL = 1
};

//! \brief 平移插值方法
enum TranslateInterplolationMode
{
	TIM_LINEAR = 0, //线性插值,适合平面场景飞行插值
	TIM_SPHERICAL = 1 //球面插值，适合球面场景飞行插值
};

//! \brief 管线上的顶点类型
enum PipeVertexType
{
	//! \brief 位于拐角起始处的点
	CORNER_START = 1, 
	//! \brief 位于拐角终止处的点
	CORNER_END = 2, 
	//! \brief 拐角中的点
	CORNER_MIDDLE = 3,
	//! \brief 忽略拐点的点
	IGNORE_CORNER = 4,
};

//! \brief 管点中的流向
enum PipeNodeFlowType
{
	//! \brief 流入管点
	INFLOW = 0, 
	//! \brief 流出管点
	OUTFLOW = 1, 
};

enum UGCamreaMoveDirection
{
	CMD_NONE			= 0x0,	// 没有移动
	CMD_FORWARD	= 0x1,	// 向前移动
	CMD_BACK			= 0x2,	// 向后移动
	CMD_LEFT			= 0x4,	// 向左移动
	CMD_RIGHT			= 0x8,	// 向右移动	
	CMD_UP				= 0x10, // 放大
	CMD_DOWN			= 0x20, //缩小

};

//! \brief 裁剪面裁剪模式
enum PlaneClipMode
{
	CLIP_NOTHING = 0,   //不裁剪
	CLIP_BEHIND_ANY_PLANE = 1,  //裁剪掉位于任意裁剪面后面的部分
	CLIP_BEHIND_ALL_PLANE = 2, //裁剪掉位于所有裁剪面后面的部分
	ONLY_KEEP_LINE = 3, // 只保留裁线，裁剪掉其他部分
};

//! \brief LOD切换模式
enum UGRangeMode
{
	DISTANCE_FROM_EYE_POINT, // 根据到相机的距离切换
	PIXEL_SIZE_ON_SCREEN	 // 根据屏幕像素大小切换
};

//! \brief 多视口模式
enum MultiViewportMode
{
	MVM_NONE					= 0, //非多视口, 也就是只有一个视口
	MVM_HORIZONTAL		= 1, //水平跨越多视口
	MVM_VERTICAL			    = 2, //垂直跨越多视口
	MVM_QUAD					= 3, //四视口
	MVM_TRIPLE				    = 4, //三分屏 (左一右二)
	MVM_VERTICALTRISECTION		    = 5 //三分屏 (水平方向三等分)
};

//! \brief 分隔条方向类型
enum SplitterOrientation
{
	SO_VERTICAL,		//垂直方向拖动的分隔条
	SO_HORIZONTAL	//水平方向拖动的分隔条
};

//! \brief osgb加载方式
enum  UGProcessFileType
{
	PRF_Normal,
	PRF_Add
};

//! \brief 线框模式
enum UGWireFrameType
{
	WF_Triangle,
	WF_Quad,
	WF_Sketch
};

//! \brief 三维普通类
class BASE3D_API UGCommon3D
{
public:
	//! \brief 将Defualt字符集的字符串转换为UTF8的字符串。
	//! \param str Defualt字符集的字符串[in]。
	//! \return 返回UTF8的字符串。
	static UGMBString UGToKmlString(UGString str);
	//! \brief 将UTF8字符集的字符串转换为Defualt的字符串。
	//! \param str UTF8字符集的字符串[in]。
	//! \return 返回Defualt的字符串。
	static UGString KmlToUGString(UGMBString str);
	//! \brief TTF库的路径名称
// 	static UGString ms_strTTFLibPath;
// 	//! \brief TTF字体高度
 	static UGushort ms_uTTFHeight;
	//! \brief 场景资源文件夹所在目录，默认为空。解决当客户启用UAC控制后，客户端无法访问粒子以及海洋大文件资源的问题
	// 多场景下应该也是能够通用的，毕竟没有对资源进行修改并保存的功能。
	static UGString ms_strSceneResFoloderPath;

	//三维特效许可是否可用
	static UGbool ms_bIsSpace3DEffectVailed;

};

//! \brief 碰撞结果类型
enum UGCollisionResult
{
	CR_NO = 0, //未发生碰撞
	CR_NORMAL = 1,//常规碰撞
	CR_AUTOPITCHING = 2,//需自动俯仰
	CR_ZOOM_PITCHING = 3,//需放大调整视角
	CR_ZOOM_OUTEXIT = 4 //需缩小回退
};

//! \brief 多机协同渲染场景的面索引
enum MultiComputerRenderIndex
{
	MC_FRONT = 0, //前面
	MC_LEFT = 1,//左面
	MC_RIGHT = 2,//右面
	MC_BACK = 3,//后面
	MC_TOP = 4, //顶面
	MC_BOTTOM = 5 //底面
};

//! \brief 切分主题
enum TilingScheme
{
	//! \brief 墨卡托切分，1x1。
	MercatorTilingScheme,
	//! \brief 地理切分，1x2。
	GeographicTilingScheme,
};

//! \brief 点云W位字段类型
enum UGPointCloudCategoryField
{
	PCC_Intensity,
	PCC_PosZ,
	PCC_Classify,
};

//! \brief 金字塔刨分类型
enum UGPyramidSplitType
{
	PS_QuadTree,
	PS_Octree
};

}

#endif

