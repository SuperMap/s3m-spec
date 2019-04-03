//========================================================================//
// 项目名：			SuperMap Universal GIS class (UGC) Library 0.9
// 作  者：			张立立
// 单  位：			北京超图软件股份有限公司
// 最后修改时间：	以文件日期为准
//------------------------------------------------------------------------//
//版权声明:         版权所有 2000-2010 北京超图软件股份有限公司，保留所有权利 
//	                Copyright (c) 2000-2010 SuperMap Software Co., Ltd. 
//	                All rights reserved

// 重要声明：		1. 
//					2. 
//					
//------------------------------------------------------------------------//
// 文件名：			SmPrjCoordSys.h
// 类  名：			UGPrjCoordSys
// 父  类：			
// 子  类：			

// 功能说明：		投影坐标系定义
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
#if !defined(UGC_SMPRJCOORDSYS_H__9F382146_8F1F_4F19_AA5C_79E6842EF0E3__INCLUDED_)
#define UGC_SMPRJCOORDSYS_H__9F382146_8F1F_4F19_AA5C_79E6842EF0E3__INCLUDED_

#include "Toolkit/UGThread.h"
#include "Stream/ugdefs.h"
#include "Projection/UGGeoCoordSys.h"
#include "Projection/UGPrjParams.h"
#include "Projection/UGProjection.h"
#include "Element/OgdcCoordSys.h"

namespace UGC{

#ifdef SYMBIAN60
class PROJECTION_APIC UGPrjCoordSys : public UGSpatialRef
#else
class PROJECTION_API UGPrjCoordSys : public UGSpatialRef
#endif
{
	friend class UGPrjFactory;

public:
	enum PrjFileType
	{
		SuperMap		= 1,
		Esri			= 2,
	};

	enum PrjFileVersion
	{
		SFC60			= 1,
		UGC60			= 2,
	};

public:
	//! \brief 默认构造函数
	PROJECTION_APIF UGPrjCoordSys();
	//! \brief 通过投影坐标系类型构造投影坐标系对象
	PROJECTION_APIF UGPrjCoordSys(EmPrjCoordSysType nTypeID );
	//! \brief 拷贝构造函数
	PROJECTION_APIF UGPrjCoordSys(const UGPrjCoordSys& PrjCoordSys);
	//! \brief 通过地理坐标系、投影方式、投影参数构造投影坐标系对象
	//! \param GeoCoordSys 地理坐标系对象[in]
	//! \param Projection 投影方式对象[in]
	//! \param PrjParams 投影参数[in]
	PROJECTION_APIF UGPrjCoordSys(const UGGeoCoordSys& GeoCoordSys,
				   const UGProjection& Projection,
				   const UGPrjParams& PrjParams
				  // Utility::UGString strName = ""
				  );

	//! \brief 通过EPSGCode构建投影坐标系对象
	//! \param epsgCode 地理坐标系或者投影坐标系epsgcode[in]
	PROJECTION_APIF UGPrjCoordSys(const UGint epsgCode);

	PROJECTION_APIF UGPrjCoordSys& operator = (const UGPrjCoordSys& PrjCoordSys);
	// 下面两个函数会 hide 基类的同名(不同参数) 的函数

	PROJECTION_APIF UGbool operator == (const UGPrjCoordSys& PrjCoordSys) const;
	PROJECTION_APIF UGbool operator != (const UGPrjCoordSys& PrjCoordSys) const;

	PROJECTION_APIF UGbool operator == (const UGSpatialRef& PrjCoordSys) const;
	PROJECTION_APIF UGbool operator != (const UGSpatialRef& PrjCoordSys) const;

public:

	//! \brief 根据中央经度计算得到高斯投影和UTM的带号,中央经线取值在－180到180之间。
	//! \param dPrimeMeridian 中央经线经度值[in]。
	//! \param eTypeID 投影方式，带号只有高斯投影和UTM(PRJ_TRANSVERSE_MERCATOR)投影支持，其它投影返回－1 [in]。
	//! \return 如果设置成功返回带号；否则返回－1。
	PROJECTION_APIF UGint GetPrjZone(UGdouble dPrimeMeridian, EmPrjObjectType eTypeID = PRJ_GAUSS_KRUGER);


	PROJECTION_APIF UGbool ToOgdcCoordSys(OgdcCoordSys& coordSys) const;
	PROJECTION_APIF UGbool FromOgdcCoordSys(const OgdcCoordSys& coordSys);


	//! \brief 保存至文件 尚未实现
	PROJECTION_APIF UGbool SaveToArray(const UGString& strFileName, UGPrjCoordSys::PrjFileVersion emVersion = UGPrjCoordSys::UGC60);
	//! \brief 从文件读取 尚未实现
	PROJECTION_APIF UGbool LoadFromArray(const UGString& strFileName, UGPrjCoordSys::PrjFileType emFileType = UGPrjCoordSys::SuperMap); 	
	
	//! \brief 设置投影坐标系类型
	//! \brief 设置类型后，将会根据类型重构投影坐标系对象
	PROJECTION_APIF UGbool SetTypeID(EmPrjCoordSysType nTypeID);
	
	//! \brief 获取投影坐标系类型
	PROJECTION_APIF EmPrjCoordSysType GetTypeID() const;
	//! \brief 获取投影参数对象
	PROJECTION_APIF const UGPrjParams &GetPrjParams() const;
	//! \brief 获取地理坐标西对象
	PROJECTION_APIF const UGGeoCoordSys& GetGeoCoordSys() const;	
	//! \brief 获取投影方式对象
	PROJECTION_APIF const UGProjection &GetProjection() const;
//	UGString GetName() const;

	//! \brief 设置投影参数对象
	PROJECTION_APIF UGbool SetPrjParams(const UGPrjParams& PrjParams);
	//! \brief 设置地理坐标系对象
	PROJECTION_APIF UGbool SetGeoCoordSys(const UGGeoCoordSys& GeoCoordSys);	
	//! \brief 设置投影方式对象
	PROJECTION_APIF UGbool SetProjection(const UGProjection& Projection );
		
	//! \brief 获取参数个数
	PROJECTION_APIF UGint GetParamsCount();
	//! \brief 通过索引获取参数名称
	PROJECTION_APIF UGString GetParamName(UGint nIndex);
	//! \brief 通过索引获取参数值
	PROJECTION_APIF UGdouble GetParamValue(UGint nIndex);
	//! \brief 通过名称获取参数值
	PROJECTION_APIF UGdouble GetParamValue(const UGString& strName);
	//! \brief 通过索引设置参数值
	PROJECTION_APIF UGbool SetParamValue(UGint nIndex,UGdouble dValue);
	//! \brief 通过名称设置参数值
	PROJECTION_APIF UGbool SetParamValue(const UGString& strName,UGdouble dValue);
	
	//! \brief 正变换就是从经纬度变换至投影；逆变化则是从投影到经纬度
	PROJECTION_APIF UGbool Forward(UGPoint2D* pPoints, UGint nCount);
	PROJECTION_APIF UGbool Inverse(UGPoint2D* pPoints, UGint nCount);

	// 三维点转换只是对其中的x,y坐标作了投影变换处理，z值并不参与计算
	// 在RefTranslator中，z值表示高程，在地理坐标系不同的情况下，是参与计算的
	// 这一点需要多加小心
	PROJECTION_APIF UGbool Forward(UGPoint3D* pPoints, UGint nCount); 
	PROJECTION_APIF UGbool Inverse(UGPoint3D* pPoints, UGint nCount);


	PROJECTION_APIF UGString ToXML(UGint nVersion = 0)const;
	PROJECTION_APIF UGbool FromXML(const UGString& strPrjXml,UGint nVersion = 0);

//	UGString ToXMLEx(UGint nVersion = 0)const;

	PROJECTION_APIF UGint ToEpsgCode(); 
	PROJECTION_APIF UGbool FromEpsgCode(UGint code);

	//! \brief 从配置文件读取投影坐标系信息 dongfei 2007-04-04	
	PROJECTION_APIF UGbool FromConfigure( EmPrjCoordSysType nTypeID,UGint nVersion = 0);
	
	//! \brief 从数据流读取投影坐标系对象
	PROJECTION_APIF UGbool SetData(UGStream& prjstream,UGint nVersion=0); // Load from stream
	//! \brief 将投影坐标系对象输出到数据流
	PROJECTION_APIF UGbool GetData(UGStream& prjstream,UGint nVersion=0);   // save to stream
	
	//! \brief 设置平面坐标系单位，同时将投影坐标系类型设置为平面坐标系
	PROJECTION_APIF UGbool SetPlanar(UGint nUnits );		//平面坐标系
	//! \brief 设置地理坐标系 同时设置投影坐标系类型为地理坐标系
	PROJECTION_APIF UGbool SetEarth( UGGeoCoordSys& geogCs);//地理坐标系

	PROJECTION_APIF UGbool IsValid() const {return m_bIsValidPrj;}
	
	//! \brief 获取投影坐标系单位
	virtual PROJECTION_APIF UGint GetUnit() const;
	//! \brief 获取投影坐标系量算单位
	virtual PROJECTION_APIF UGint GetDistUnit() const;
	//! \brief 设置投影坐标系单位
	virtual PROJECTION_APIF void SetUnit(UGint nUnit);
	//! \brief 设置投影坐标系量算单位
	virtual PROJECTION_APIF void SetDistUnit(UGint nUnit);
	
	virtual PROJECTION_APIF ~UGPrjCoordSys();

	//! \brief 获取投影坐标系的EPSGCode，仅通过Prj文件
	virtual PROJECTION_APIF UGuint GetEPSGCode() const;
	//! \brief 设置投影坐标系单位
	virtual PROJECTION_APIF void SetEPSGCode(UGuint code);

	//! \brief 获取预定义投影EPSGCode和Type对应关系
	//! UGDict<UGuint, UGint>中first为EPSGCode,second表示Type
	static PROJECTION_APIF UGDict<UGuint, UGint> GetCodeType();

protected:
		PROJECTION_APIF void SetIsValid(UGbool bIsValidPrj){m_bIsValidPrj = bIsValidPrj;}
protected:
	EmPrjCoordSysType m_eTypeID;

	UGGeoCoordSys m_GeoCoordSys;
	UGPrjParams m_PrjParams;
	UGProjection m_Projection;
	UGMutex		m_mutex;
	UGbool m_bIsValidPrj;

	UGuint m_epsgCode;

	static UGString ms_PrjParamNames[];	
private:
	static UGDict<UGuint, UGint> g_PrjCodeTypeDict;
};

}//namespace UGC


#endif // !defined(UGC_SMPRJCOORDSYS_H__9F382146_8F1F_4F19_AA5C_79E6842EF0E3__INCLUDED_)

