//========================================================================
// 项目名：			SuperMap Universal GIS class (UGC) Library 0.9
// 作  者：			张立立
// 单  位：			北京超图软件股份有限公司
// 最后修改时间：	以版本更新完成日期为准
//------------------------------------------------------------------------
//版权声明:         版权所有 2000-2010 北京超图软件股份有限公司，保留所有权利 
//	                Copyright (c) 2000-2010 SuperMap Software Co., Ltd. 
//	                All rights reserved
// 重要声明：		1. 
//					2. 
//					
//------------------------------------------------------------------------
// 文件名：			SmGeoCoordSys.h
// 类  名：			UGGeoCoordSys
// 父  类：			
// 子  类：			

// 功能说明：		地理坐标系定义
// 调用说明：		共有四个构造函数
//			1		UGGeoCoordSys()//生成一个WGS84地理坐标系
//          2       UGGeoCoordSys(EmGeoCoordSysType nTypeID );构造一个内置地理坐标系
//          3       拷贝构造                       
//          4       生成一个自定义的地理坐标系

//          SetTypeID(EmGeoCoordSysType nTypeID);可以通过设置类型号来设置地理坐标系的
//                                              类型


#if !defined(UGC_SMGEOCOORDSYS_H__93600E0A_9224_4165_B600_D5096E5BCCA6__INCLUDED_)
#define UGC_SMGEOCOORDSYS_H__93600E0A_9224_4165_B600_D5096E5BCCA6__INCLUDED_

#include "Toolkit/UGThread.h"

#include "Projection/UGSpatialRef.h"
#include "Projection/UGPrimeMeridian.h"
#include "Projection/UGHorizonDatum.h"

namespace UGC{

#ifdef SYMBIAN60
class PROJECTION_APIC UGGeoCoordSys  : public UGSpatialRef
#else
class PROJECTION_API UGGeoCoordSys  : public UGSpatialRef
#endif
{
	friend class UGPrjFactory;
public:
	//! \brief 默认构造函数	
	PROJECTION_APIF UGGeoCoordSys();//
	//! \brief 根据地理坐标系类型构造地理坐标系	
	PROJECTION_APIF UGGeoCoordSys(EmGeoCoordSysType nTypeID );
	//! \brief 拷贝构造函数
	PROJECTION_APIF UGGeoCoordSys(const UGGeoCoordSys& GeoCoordSys);
	//! \brief 根据水平参考系、中央经线、坐标单位以及坐标系名称构造地理坐标系	
	//! \param HorizonDatum    水平参考系[in]
	//! \param PrimeMeridian           中央经线[in]
	//! \param nUnit            坐标单位，不是坐标量算单位[in]
	//! \param strName         坐标系名称[in]
	PROJECTION_APIF UGGeoCoordSys(UGHorizonDatum& HorizonDatum,
				   UGPrimeMeridian& PrimeMeridian,
				   UGint nUnit,
				   UGString strName = _U(""));

	PROJECTION_APIF UGGeoCoordSys& operator = (const UGGeoCoordSys& GeoCoordSys);
	// 下面两个函数会 hide 基类的同名(不同参数) 的函数
	PROJECTION_APIF UGbool operator == (const UGSpatialRef& GeoCoordSys) const;
	PROJECTION_APIF UGbool operator != (const UGSpatialRef& GeoCoordSys) const;

	
	virtual PROJECTION_APIF  ~UGGeoCoordSys();
	
	//! \brief 设置地理坐标系类型
	//! \param nTypeID 地理坐标系类型
	//! \return 类型设置成功返回TRUE 否则返回FALSE
	//! \remarks 由于地理坐标类型是地理坐标系的唯一标识符 所以类型设置后，所有的参数也相应变化了
	PROJECTION_APIF UGbool SetTypeID(EmGeoCoordSysType nTypeID);	
	//! \brief 返回地理坐标系类型
	PROJECTION_APIF EmGeoCoordSysType GetTypeID() const;

	PROJECTION_APIF void SetEPSGCode(UGuint epsg);
	PROJECTION_APIF UGuint GetEPSGCode() const;

	//! \brief 获取水平参考系参数
	//! \return 水平参考系
	const PROJECTION_APIF UGHorizonDatum &GetHorizonDatum() const;
	//! \brief 设置水平参考系
	//! \param datum 水平参考系 [in]
	//! \return 设置成功返回TRUE 否则返回FALSE
	PROJECTION_APIF UGbool SetHorizonDatum(const UGHorizonDatum& datum);

	//! \brief 获取中央经线对象
	const PROJECTION_APIF  UGPrimeMeridian& GetPrimeMeridian() const;
	//! \brief 设置中央经线对象
	//! \param primeMeridian 中央经线 [in]
	//! \return 设置成功返回TRUE 否则返回FALSE
	PROJECTION_APIF UGbool SetPrimeMeridian(const UGPrimeMeridian& primeMeridian);

	//! \brief 输出为XML字符串
	//! \param nVersion 输出为XML的版本 默认为当前版本 [in]
	//! \return 输出的XML字符串
	PROJECTION_APIF UGString ToXML(UGint nVersion = 0)const;
	
	//! \brief 从XML字符串读取
	//! \param strPrjXML 读入的XML字符串[in]
	//! \param nVersion 读入XML字符串的版本
	//! \return 读入XML字符串成功返回真 否则返回假
	PROJECTION_APIF UGbool FromXML(const UGString& strPrjXml,UGint nVersion = 0);

	//modified by dongfei 2007-04-03
	/* 更改xml结构，投影坐标系中只存投影方式、地理坐标系的类型，不存其实际数据
	 * 水平参考系、子午线在地理坐标系中也是同样方式 主要是为了导出所有的投影坐标系数据
	 * 可能以后用不着
	 */
//	UGString ToXMLEx(UGint nVersion = 0)const;
	
	//! \brief 从配置文件读取地理坐标系信息 dongfei 2007-04-04	
	PROJECTION_APIF UGbool FromConfigure(EmGeoCoordSysType nTypeID,UGint nVersion = 0);

	//! \brief 根据数据流创建地理坐标系
	PROJECTION_APIF UGbool SetData(UGStream& prjstream,UGint nVersion=0); // Load from stream
	//! \brief 输出地理坐标系到数据流
	PROJECTION_APIF UGbool GetData(UGStream& prjstream,UGint nVersion=0);   // save to stream

	//! \brief 获取空间坐标系类型
	PROJECTION_APIF EmSpatialRefType GetSpatialRefType() const;
	//! \brief 设置空间坐标系类型
	virtual PROJECTION_APIF void SetSpatialRefType(EmSpatialRefType eType);
	PROJECTION_APIF UGbool IsValid(){return m_bValidGeo;}

	//! \brief 获取预定义地理坐标系EPSGCode和Type对应关系
	//! UGDict<UGuint, UGint>中first为EPSGCode,second表示Type
	static PROJECTION_APIF UGDict<UGuint, UGint> GetGeoCodeTypeDict();
	//! \brief 根据epsgCode获取地理坐标系信息
	PROJECTION_APIF UGbool FromEpsgCode(UGuint epsgCode, UGint nVersion = 0);

protected:
	PROJECTION_APIF void SetIsValid(UGbool bValid){m_bValidGeo = bValid;}

protected:
	UGHorizonDatum  m_HorizonDatum; //大地水平参照系
	UGPrimeMeridian m_PrimeMeridian;//本初子午线

//	UGString m_strName;
	EmGeoCoordSysType   m_eTypeID;
	UGMutex				m_mutex;
	
	UGbool m_bValidGeo;

	UGuint m_epsgCode;

private:
	static UGDict<UGuint, UGint> g_geoCodeTypeDict;
};

}//namespace UGC



#endif // !defined(UGC_SMGEOCOORDSYS_H__93600E0A_9224_4165_B600_D5096E5BCCA6__INCLUDED_)

