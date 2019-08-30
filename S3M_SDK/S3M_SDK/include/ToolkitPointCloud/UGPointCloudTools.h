#pragma once
#include "Stream/ugdefs.h"
#include "Base3D/UGBoundingBox.h"
#include "Base3D/UGPointCloud.h"
#include "Projection/UGPrjCoordSys.h"
#include "Toolkit/UGProgress.h"
#include "Projection/UGTranslateParams.h"
#include "Projection/UGRefTranslator.h"

namespace UGC{

struct TOOLKITPOINTCLOUD_API PointsBlockArea
{
		UGint m_nLevel;
		UGint m_nRowBegin;
		UGint m_nRowEnd;
		UGint m_nColBegin;
		UGint m_nColEnd;
		UGint m_nHeightBegin;
		UGint m_nHeightEnd;

		PointsBlockArea()
		{
			m_nLevel = 0;
			m_nRowBegin = 0;
			m_nRowEnd = 0;
			m_nColBegin = 0;
			m_nColEnd = 0;
			m_nHeightBegin = 0;
			m_nHeightEnd = 0;
		}
};

enum TOOLKITPOINTCLOUD_API PointCloudDataType
{		
	//! \brief 无效的字段类型。
	XYZ = 0,
	XYZ_Reflectance = 1,
	XYZ_Reflectance_RGB = 2,
	XYZ_Reflectance_Normal = 3,
	XYZ_Reflectance_RGB_Normal = 4,
	XYZ_Reflectance_Normal_RGB = 5,
	XYZ_RGB = 6,
	XYZ_RGB_Reflectance = 7,
	XYZ_RGB_Normal = 8,
	XYZ_RGB_Reflectance_Normal = 9,
	XYZ_RGB_Normal_Reflectance = 10,
	XYZ_Normal = 11,
	XYZ_Normal_RGB = 12,
	XYZ_Normal_Reflectance = 13,
	XYZ_Normal_Reflectance_RGB = 14,
	XYZ_Normal_RGB_Reflectance = 15,
	UnKnown = 16,
};

enum TOOLKITPOINTCLOUD_API PointCloudSeparator
{
	//! \brief 点云数据之间分割符。
	PC_SPACE  = 0, // 空格“ ”
	PC_COMMA  = 1, // 逗号  ","
	PC_COLON  = 2, // 冒号  ":"
};

enum TOOLKITPOINTCLOUD_API PointCloudRGBMode
{
	//! \brief 颜色值表示范围。
	RGB_0_1,         //0-1
	RGB_0_255,     //0-255
};

enum TOOLKITPOINTCLOUD_API PointCloudUnit
{
	//! \brief 颜色值表示范围。
	Degree,         //0
	Meter,     //1
	Centimeter,  //2
	Millimeter,   //3
	Inch,    //4
	Foot,   //5
};

//! \brief 点云配置文件List相关信息
struct TOOLKITPOINTCLOUD_API UGPointCloudFileInfo
{
	UGPointCloudFileInfo()
	{
		m_strListFileName = _U("");
		m_pntPos = UGPoint3D(0.0,0.0,0.0);
		m_fPointsNum = 0;
		m_prjCoordSys = UGPrjCoordSys(PCS_NON_EARTH);
		m_bTransEnable = FALSE;
		m_eTransMethod = MTH_COORDINATE_FRAME;
		m_BoundingboxMerge.SetNULL();
		m_nDataInfoType = XYZ;
		m_nDataSeparator = PC_SPACE;
		m_nDataRGBMode = RGB_0_255;
		m_nDataUnit = Meter;
		m_mapBoundingBox.clear();
	};

	~UGPointCloudFileInfo()
	{
		m_BoundingboxMerge.SetNULL();
		m_mapBoundingBox.clear();
	};

	//! \brief list文件名称。
	UGString m_strListFileName;

	//! \brief 插入点位置。
	UGPoint3D m_pntPos;

	//! \brief 点云顶点数
	UGfloat m_fPointsNum;

	//! \brief 当前坐标系
	UGPrjCoordSys m_prjCoordSys;

	//! \brief 坐标系转换参数和方法是否可用
	UGbool m_bTransEnable;

	//! \brief 目标坐标系
	UGPrjCoordSys m_desPrjCoordSys;

	//! \brief 坐标系转换参数
	UGGeoTransParams m_geoTransParams;

	//! \brief 坐标系转换方法
	EmGeoTransMethod m_eTransMethod;

	//! \brief list文件所有数据的整体包围盒。
	UGBoundingBox m_BoundingboxMerge;

	//! \brief 点云文件数据格式类型
	PointCloudDataType m_nDataInfoType;

	//! \brief 点云数据之间分割符。
	PointCloudSeparator m_nDataSeparator;

	//! \brief 点云数据RGB范围。
	PointCloudRGBMode m_nDataRGBMode;

	//! \brief 点云数据单位。
	PointCloudUnit m_nDataUnit;

	//! \brief 点云文件强度值范围
	std::pair<UGdouble, UGdouble> m_instensityRange;

	//! \brief 点云文件密度范围
	std::pair<UGdouble, UGdouble> m_densityRange;

	//! \brief list文件名称及包围盒数组
	std::map<UGString, UGBoundingBox> m_mapBoundingBox;

	//! \brief 文件名与点云个数映射
	std::map<UGString,UGlong> m_mapFileNameSumPoints;

};

class TOOLKITPOINTCLOUD_API UGPointCloudTools
{
public:
	UGPointCloudTools();
	~UGPointCloudTools();
	
public:
	//! \brief 保存点云List文件
	static UGbool SaveList(UGPointCloudFileInfo& pointCloudFileInfo);

	//! \brief 解析点云List文件
	static UGbool ParserList(UGPointCloudFileInfo& pointCloudFileInfo);

	//! \brief 计算bounds范围所在的行列号
	//! \param rcBound [in] 地理范围。
	//! \param dMPTile [in] 单个瓦片的地理大小，单位度。
	//! \param nRowBottom [out] 返回瓦片行号下限。
	//! \param nRowTop [out] 返回瓦片行号上限。
	//! \param nColLeft [out] 返回瓦片最小列号。
	//! \param nColRight [out] 返回瓦片最大列号。
	//! \remarks 四叉树剖分。
	static void GetRowColRange(UGRect2D rcBound, UGdouble dMPTile, UGint& nRowBottom, UGint& nRowTop, UGint& nColLeft, UGint& nColRight);

	//! \brief 计算bounds范围所在的行列号
	//! \param rcBound [in] 地理范围。
	//! \param nLevel [in] 瓦片所在全球剖分层级。
	//! \param nRowBottom [out] 返回瓦片行号下限。
	//! \param nRowTop [out] 返回瓦片行号上限。
	//! \param nColLeft [out] 返回瓦片最小列号。
	//! \param nColRight [out] 返回瓦片最大列号。
	//! \remarks 四叉树剖分。
	static void GetRowColRange(UGRect2D rcBound, PointsBlockArea& pointArea);

	//! \brief 根据层级计算八叉树高度所在的高度号
	//! \param dMaxZ [in] 高度最大值。
	//! \param dMinZ [in] 高度最小值。
	//! \param pointsArea [int/ out] 返回高度号信息。
	//! \remarks 四叉树/八叉树剖分。
	static void GetHeightRange(UGdouble dMaxZ, UGdouble dMinZ, PointsBlockArea& pointsArea, UGbool bOctree = TRUE);

	//! \brief 根据层级计算瓦片大小，单位为米
	static UGdouble GetTileMeter(UGint nLevel);

	//! \brief 单位转换为米的比例系数
	static UGdouble GetUnitRatio2Meter(PointCloudUnit nUnit);

	//! \brief 获取当前瓦片的下一层级孩子节点所在的行列号
	//! \param nLevel [in] 当前瓦片所在全球剖分层级。
	//! \param nRow [in] 当前瓦片所在行号。
	//! \param nCol [in] 当前瓦片所在列号。
	//! \param pointsArea [out] 返回孩子节点行列号信息。
	//! \remarks 四叉树剖分。
	static void GetChildRowColRange(UGint nLevel, UGint nRow, UGint nCol, PointsBlockArea& pointsArea);

	//! \brief 计算孩子节点八叉树高度所在的层级
	//! \param nHeight [in] 高度所在索引。
	//! \param pointsArea [out] 高度索引信息。
	//! \param bOctree [in] 是否采用八叉树剖分。
	//! \remarks 四叉树/八叉树剖分。
	static void GetChildHeightRange(UGint nHeight, PointsBlockArea& pointsArea, UGbool bOctree = TRUE);

	//! \brief 从点云列表中获取指定范围的点云数据
	//! \param strListFileName [in] 点云文件list列表。
	//! \param rcBounds [in] 查询范围，地理范围。
	//! \param dMinZ [in] 查询点云高度最小值。
	//! \param dMaxZ [in] 查询点云高度最大值。
	//! \remarks 返回点云数据。
	static UGPointCloud* QueryPointsByRect(const UGString strListFileName, UGRect2D rcBounds);

	//! \brief 从源数据中获取指定范围的点云数据
	//! \param pSrcPointCloud [in] 源点云数据。
	//! \param rcBounds [in] 查询范围。
	//! \remarks 返回点云数据。
	static UGPointCloud* QueryPointsByRect(const UGPointCloud& pSrcPointCloud, UGRect2D rcBounds, UGdouble dMinZ = -OGDC_DBLMAX, UGdouble dMaxZ = OGDC_DBLMAX);

	//! \brief 点云投影变换，默认为转地理坐标系
	//! \param refTranslator; [in] 坐标系转换器。
	//! \param vecPoints [in] 点云三维坐标。
	//! \param pntPos [in] 插入点坐标。
	//! \param bInverse [in] 是否逆转换，正转换为prjCoordSys-----地理坐标系，逆转换为prjCoordSys-----笛卡尔坐标。
	//! \remarks 。
	static void PJConvertPointCloud(UGRefTranslator& refTranslator, std::vector<UGdouble>& vecPoints, UGPoint3D pntPos, UGbool bInverse = false);

	//! \brief 米转度
	static UGdouble TileMeterToDegree(UGdouble dMPTile);

	//! \brief 采用PCL+Meshlab方法计算点云法线
	//! \param vecVertices [in] 点云三维坐标。
	//! \param vecNormal [out] 点云法线。
	//! \param nK [in] K邻域搜索大小
	//! \remarks 。
	static void PCA_ComputeNormal(std::vector<UGdouble>& vecVertices, std::vector<UGfloat>& vecNormal, UGint nK);

	//! \brief 计算三维点所在瓦片的位置
	//! \param pntOriginal [in] 剖分原点。
	//! \param pntPosition [in] 三维点坐标。
	//! \param nRow [out] 返回瓦片行号。
	//! \param nCol [out] 返回瓦片列号。
	//! \remarks 地图缓存使用，主要是计算瓦片的位置。
	static void ComputePointPos(const UGPoint3D &pntOriginal, const UGPoint3D &pntPosition, UGdouble dMPTile,
		UGint &nRow, UGint &nCol);

	//! \brief 判断点是否在rect范围内
	static bool RectContainPt(const UGRect2D& rect, const UGPoint3D& ptTest);

	//! \brief 计算三维点集分辨率
	//! \param vecVertices [in] 剖分原点。
	//! \param nValueCount [out] 有效三维点个数。
	//! \remarks 返回分辨率。
	static UGdouble CalPointCloudResulotion(const std::vector<UGdouble>& vecVertices, UGint& nValueCount);

	//! \brief 球面场景插值生成栅格数据
	static UGfloat* InterpolateGlobeRasterData(UGbool IsSupportFloatTex, const UGfloat* pSrcData, const UGRect2D& rectAnalyst, const UGint& nRow, const UGint& nCol, const UGMatrix4d& texToModelPosMatrix);
	//! \brief 平面场景插值生成栅格数据
	static UGfloat* InterpolateLocalRasterData(UGbool IsSupportFloatTex, const UGfloat* pSrcData, const UGRect2D& rectAnalyst, const UGint& nRow, const UGint& nCol, const UGMatrix4d& texToModelPosMatrix);
	//! \brief 球面场景插值生成栅格影像
	static UGuint* InterpolateGlobeRasterImage(const void* pSrcData, const UGRect2D& rectAnalyst, const UGint& nRow, const UGint& nCol, const UGMatrix4d& texToModelPosMatrix);

	//! \brief 获得仿射矩阵2*3
	static UGdouble* GetAffineMatrix(UGArray<UGVector3d>& arrPoint2D, UGArray<UGVector3d>& arrPoint3D);

};
}