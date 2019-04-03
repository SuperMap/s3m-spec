//========================================================================
// 项目名：			SuperMap Universal GIS class (UGC) Library 0.9
// 作  者：			张立立
// 单  位：			北京超图软件股份有限公司
// 最后修改时间：	以文件日期为准
//------------------------------------------------------------------------
//版权声明:         版权所有 2000-2010 北京超图软件股份有限公司，保留所有权利 
//	                Copyright (c) 2000-2010 SuperMap Software Co., Ltd. 
//	                All rights reserved
// 重要声明：		1. 
//					2. 
//					
//------------------------------------------------------------------------
// 文件名：			SmRefTranslator.h
// 类  名：			UGRefTranslator
// 父  类：			
// 子  类：			

// 功能说明：		坐标系转换计算器(包括投影变换，参照系变换)
// 调用说明：		
/*-------------------------------------版本更新---------------------------
V 0.92
作    者：输入作者（或修改者）名字
完成日期：2002年10月20日
更新说明：

V 0.91
作    者：输入作者（或修改者）名字
完成日期：2001年7月20日
更新说明：

V 0.9 
原作者  ：张立立
完成日期：
========================================================================*/

#if !defined(UGC_SMREFTRANSLATOR_H__9153EB7D_C880_4A7B_8184_754D69D0FDF0__INCLUDED_)
#define UGC_SMREFTRANSLATOR_H__9153EB7D_C880_4A7B_8184_754D69D0FDF0__INCLUDED_


#include "Toolkit/UGErrorObj.h"

#include "Projection/UGPrjTranslator.h"
#include "Projection/UGGeoTranslator.h"
#include "Stream/ugdefs.h"
#include "Toolkit/UGListenerControl.h"

namespace UGC{
		class UGRefTranslator;
		class UGProj4Transform;

		// 用于投影转换的参数类
		class PROJECTION_API UGRefTranslaterEventArgs : public UGEventArgs
		{
		public:
			enum TranslaterAction
			{
				SrcForward = 0, // 同一地理坐标系下，地理坐标转换到投影坐标
				SrcInverse =1,
				GeoSrcToDes = 2, // 不同地理坐标系的转换,将坐标从sender中UGGeoTranslator对象中的GeoCoordSysSrc地理坐标系转到GeoCoordSysDes坐标
				GeoDesToSrc = 3, // 不同地理坐标系的转换,将坐标从sender中UGGeoTranslator对象中的GeoCoordSysDes地理坐标系转到GeoCoordSysSrc坐标
				DesForward = 4,
				DesInverse = 5, // 同一地理坐标系下，投影坐标转换到地理坐标
			};
		public:
			UGRefTranslaterEventArgs(UGPoint2D* point2Ds, UGint pt2dCount,UGPoint3D* point3ds, UGint pt3dCount,UGint action ):
				m_point2ds(point2Ds),m_point2dsCount(pt2dCount),m_point3ds(point3ds),m_point3dsCount(pt3dCount),m_action(action){}	


			UGPoint2D* Point2ds() const { return m_point2ds; }
			UGint Point2dsCount() const { return m_point2dsCount; }

			UGPoint3D* Point3ds() const { return m_point3ds; }
			UGint Point3dsCount() const { return m_point3dsCount; }
			UGC::UGint Action() const { return m_action; }

		private:

			UGPoint2D* m_point2ds;
			UGint m_point2dsCount;

			UGPoint3D* m_point3ds;	
			UGint m_point3dsCount;

			UGint m_action;
		};

		class  PROJECTION_API UGRefTranslaterListener : public UGListener
		{
		public:	
			// 内部使用，请不要重写此方法
			virtual void Run( void* pSender,UGEventArgs* pArgs );

			// 所有注册转换事件的用户都需要实现此方法
			// 参数里的对象请不用执行delete操作，否则会引起崩溃
			virtual void Run(UGRefTranslator* pSender, UGRefTranslaterEventArgs* pArgs) = 0;
		};
class PROJECTION_API UGRefTranslator  
{
public:
	enum EmTransType
	{
		//! \brief m_PJCoordSysSrc 投影坐标-->m_PJCoordSysDes 投影坐标
		XY1ToXY2	= 0,
		//! \brief m_PJCoordSysSrc 投影坐标-->m_PJCoordSysDes 地理经纬坐标
		XY1ToGeo2	= 1,
		//! \brief m_PJCoordSysSrc 地理经纬坐标-->m_PJCoordSysDes 投影坐标
		Geo1ToXY2	= 2,
		//! \brief m_PJCoordSysSrc 地理经纬坐标-->m_PJCoordSysDes 地理经纬坐标
		Geo1ToGeo2	= 3,
		//! \brief m_PJCoordSysSrc 投影坐标-->m_PJCoordSysSrc 地理经纬坐标
		XY1ToGeo1	= 4,
		//! \brief m_PJCoordSysSrc 地理经纬坐标-->m_PJCoordSysSrc 投影坐标
		Geo1ToXY1	= 5,
		//! \brief m_PJCoordSysDes 投影坐标-->m_PJCoordSysDes 地理经纬坐标
		XY2ToGeo2	= 6,
		//! \brief m_PJCoordSysDes 地理经纬坐标-->m_PJCoordSysDes 投影坐标
		Geo2ToXY2	= 7
		
	};
	//! \brief 默认构造函数
	UGRefTranslator();
#ifdef SYMBIAN60
	~UGRefTranslator();
#else
	virtual ~UGRefTranslator();
#endif

	//! \brief 坐标正反转换
	UGbool Translate(UGPoint2D* pPoints, UGlong nPointCount,UGbool bForward = true);
	//! \brief 坐标正反转换
	UGbool Translate(UGPoint3D* pPoints, UGlong nPointCount,UGbool bForward = true);

	

	//! \brief 设置目标参考系的地理坐标
	//! \remarks 不是设置目标参考系为某种特定地理坐标系 而是设置目标坐标系中的地理坐标系
	UGint SetGeoCoordSysDes(const UGGeoCoordSys& GeoCoordSysDes);	
	//! \brief 设置源参考系的地理坐标
	UGint SetGeoCoordSysSrc(const UGGeoCoordSys& GeoCoordSysSrc);

	//! \brief 设置目标投影坐标系
	UGint SetPrjCoordSysDes(const UGPrjCoordSys& PrjCoordSysDes);
	//! \brief 获取目标投影坐标系
	const UGPrjCoordSys& GetPrjCoordSysDes();
	
	//! \brief 设置源投影坐标系
	UGint SetPrjCoordSysSrc(const UGPrjCoordSys& PrjCoordSysSrc);	
	//! \brief 获取源投影坐标系
	const UGPrjCoordSys& GetPrjCoordSysSrc();	

	//! \brief 设置地理坐标转换方式
	void SetGeoTransMethod(EmGeoTransMethod nGeoTransMethod);
	//! \brief 获取地理坐标转换方式
	EmGeoTransMethod GetGeoTransMethod(); 
	//! \brief 获取转换器状态
	//! \return lResult = -1;// 不可能的返回值
	//!		lResult = 0;// 源坐标系为空
	//!		lResult = 1;// 源坐标系为投影坐标系，目标坐标系为空
	//!		lResult = 2;// 源坐标系不为投影坐标系，目标坐标系为空
	//!		lResult = 3;// 源坐标系或目标坐标系有一个是平面坐标系
	//!		lResult = 4;// 经纬度 ==> 经纬度,参考系转换
	//!		lResult = 5;// 经纬度 ==> 经纬度,坐标系相同
	//!		lResult = 6;// 经纬度 ==> 投影，同时变换参照系
	//!		lResult = 7; // 经纬度 ==> 投影，参照系相同
	//!		lResult = 8; // 投影 ==> 经纬度，参照系不同
	//!		lResult = 9; // 投影 ==> 经纬度，参照系相同
	//!		lResult = 10; // 投影==>投影，参照系不同
	//!		lResult = 11; // 投影==>投影，参照系相同
	//!		lResult = 12; // 投影==>投影，坐标系相同，不干
	//!		lResult = 13; 投影==>投影，参照系相同,投影方法相同，坐标单位不同
	UGint GetTranslatorSatus();
	//! \brief 是否可以转换
	UGbool IsValid();

	//! \brief 设置地理坐标系转换参数对象
	void SetGeoTransParams (const UGGeoTransParams& GeoTransParams);

	//! \brief 获取地理坐标系转换参数对象
	UGGeoTransParams&  GetGeoTransParams ();

	UGListenerControler* GetTranslateControler();

protected:
	UGint Schedule();

	// 将不同的转换方式封装起来，便于后续调用及维护
	UGbool SrcFoward(UGPoint3D* pPoint3ds,UGlong pt3dCount,UGPoint2D* pPoint2ds,UGlong pt2dCount);
	UGbool SrcInverse(UGPoint3D* pPoint3ds,UGlong pt3dCount,UGPoint2D* pPoint2ds,UGlong pt2dCount);

	UGbool DesFoward(UGPoint3D* pPoint3ds,UGlong pt3dCount,UGPoint2D* pPoint2ds,UGlong pt2dCount);
	UGbool DesInverse(UGPoint3D* pPoint3ds,UGlong pt3dCount,UGPoint2D* pPoint2ds,UGlong pt2dCount);

	UGbool GeoTranslate(UGPoint3D* pPoint3ds,UGlong pt3dCount,UGPoint2D* pPoint2ds,UGlong pt2dCount,UGbool bForward = true);

	UGbool Translate(UGPoint3D* pPoint3ds,UGlong pt3dCount,UGPoint2D* pPoint2ds,UGlong pt2dCount,UGbool bForward = true);

	UGbool China_2D_4P(UGPoint3D* pPoint3ds,UGlong pt3dCount,UGPoint2D* pPoint2ds,UGlong pt2dCount,UGbool bForward = true);

protected:
	UGint m_lStatus;
	EmGeoTransMethod m_nGeoTransMethod;

	UGPrjCoordSys  m_PrjCoordSysSrc;
	UGint m_epsgSrc;
	UGPrjCoordSys  m_PrjCoordSysDes;
	UGint m_epsgDes;
	
	UGPrjTranslator m_PrjTranslatorSrc;
	UGPrjTranslator m_PrjTranslatorDes;
	
	UGGeoTranslator m_GeoTranslator;
	UGProj4Transform* m_pProj4Transform;

	double m_dUnitRatio;// 源坐标系单位和目标坐标系单位的比率

	// 可以通过添加该侦听
	UGListenerControler* m_plistenerControler;	
};
}//namespace UGC


#endif // !defined(UGC_SMREFTRANSLATOR_H__9153EB7D_C880_4A7B_8184_754D69D0FDF0__INCLUDED_)

