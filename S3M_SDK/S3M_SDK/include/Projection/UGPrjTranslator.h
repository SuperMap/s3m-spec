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
// 文件名：			SmPrjTranslator.h
// 类  名：			UGPrjTranslator
// 父  类：			
// 子  类：			

// 功能说明：		投影坐标系转换计算器
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
#if !defined(UGC_SMPRJTRANSLATOR_H__DCD0ED9B_E520_4376_8EAF_D9F8E7338057__INCLUDED_)
#define UGC_SMPRJTRANSLATOR_H__DCD0ED9B_E520_4376_8EAF_D9F8E7338057__INCLUDED_



#include "Projection/UGTranslateParams.h"
#include "Projection/UGPrjCoordSys.h"

// using namespace SuperMap::UGC::Utility;


namespace UGC{



class PROJECTION_API UGPrjTranslator  
{
public:
	//! \brief 默认构造函数
	UGPrjTranslator();
	UGPrjTranslator(UGPrjCoordSys* pPrjCoordSys);

#ifdef SYMBIAN60
	~UGPrjTranslator();
#else
	virtual ~UGPrjTranslator();
#endif
	
	//! \brief 得到当前转换器状态。1表示坐标系有效，0表示坐标系无效, 2 表示为自定义的投影，需要用户自行完成转换
	UGint GetTransStatus();

	//! \brief 转换器回调函数
	typedef UGbool (UGPrjTranslator::* pTransFunction) (UGdouble&, UGdouble&);

	//! \brief 正变换： 经纬度-->投影坐标系
	UGbool Forward(UGdouble& x, UGdouble& y);

	//! \brief 逆变换： 经纬度<--投影坐标系
	UGbool Inverse(UGdouble& x, UGdouble& y);

	//! \brief 设置投影坐标系
	void SetPrjCoordSys(UGPrjCoordSys* pPrjCoordSys);
	//! \brief 获取投影坐标系
	UGPrjCoordSys* GetPrjCoordSys();

	//! \brief 判断当前投影转换是否为未知的，需要由用户自行完成投影转换
	UGbool IsCustom();
	

protected:
	static pTransFunction m_For_FuncTable[PRJ_METHOD_COUNT];
	static pTransFunction m_Inv_FuncTable[PRJ_METHOD_COUNT];

	UGPrjCoordSys* m_pPrjCoordSys;//转换源坐标系


protected:
	pTransFunction m_pCurForTransFunction;//当前正变换函数 m_pSpatialRefSrc -->m_pSpatialRefDes
	pTransFunction m_pCurInvTransFunction;//当前逆变换函数 m_pSpatialRefSrc <--m_pSpatialRefDes
	
//	pTransFunction m_pFor_TransFunc1;		//第一步变换	
//	pTransFunction m_pFor_TransFunc2;       //第二步变换
//	pTransFunction m_pFor_TransFunc3;       //第三步变换

//	pTransFunction m_pInv_TransFunc1;		//第一步变换	
//	pTransFunction m_pInv_TransFunc2;       //第二步变换
//	pTransFunction m_pInv_TransFunc3;       //第三步变换

//	UGbool For_ThreeStepFunction(UGdouble& x, UGdouble& y); //三步变换函数-正
//	UGbool For_TwoStepFunction(UGdouble& x, UGdouble& y);   //两步变换函数-正

//	UGbool Inv_ThreeStepFunction(UGdouble& x, UGdouble& y); //三步变换函数-逆
//	UGbool Inv_TwoStepFunction(UGdouble& x, UGdouble& y);   //两步变换函数-逆
//	pTransFunction m_pOneStepFunction;   //一步变换函数

/*----------------------计算调度函数--------------------------------------------*/
protected:
	//根据源坐标系和目标坐标系情况，负责分配和组织相关函数进行坐标转换运算	
	UGint  Scheduler();


/*----------------------椭球几何参数计算函数------------------------------------*/
private:
	UGint   m_nConicSubType;//圆锥投影子类

	//{{====参见投影参数类
	UGdouble m_dPhiStandard1;//First standard latitude  
	UGdouble m_dPhiStandard2;//Second standard latitude
	UGdouble m_dPrimeMeridian;//中央经线
	UGdouble m_dPhiSouth;//底点纬线
//	UGdouble m_dPhiNorth;
//	UGdouble m_dPhiMiddle;
	UGdouble m_dFirstPointLongitude;
	UGdouble m_dSecondPointLongitude;
	UGdouble m_dScaleFactor;
	UGdouble m_dAzimuth;				
	UGdouble m_dFalseEasting;
	UGdouble m_dFalseNorthing;
	UGdouble m_dRectified;//纠正角
	//}}====参见投影参数类

	UGdouble m_dDeltaAxis;
	UGdouble m_dDeltaFlatten;

	UGdouble m_dAlpha;
	UGdouble m_dRho0;//最南端纬线极经
	UGdouble m_dK;

	UGint m_lTransStatus;

	UGdouble m_dPriAxis;//长径
	UGdouble m_dFlatten;//扁率
	UGdouble m_dRatio;//单位转换关系
	UGdouble m_dScale;//地图比例尺

/*-------------------------------------------------------------------------*/
	UGdouble m_dEPS2;//第一偏心率平方
	UGdouble m_dEPS; //第一偏心率
	UGdouble m_dEPSSec2;//第二偏心率平方
	UGdouble m_dEPSSec; //第二偏心率

	UGdouble m_dA1;//求经线弧长的系数===>//等距离纬度直接计算的系数
	UGdouble m_dA2;
	UGdouble m_dA3;
	UGdouble m_dA4;
	UGdouble m_dA5;

	UGdouble m_dB2;//等距离纬度直接反算的系数
	UGdouble m_dB4;
	UGdouble m_dB6;
	UGdouble m_dB8;

	UGdouble m_dC2;//等量纬度直接反算的系数
	UGdouble m_dC4;
	UGdouble m_dC6;
	UGdouble m_dC8;
	
	UGdouble m_dD2;//等面积纬度直接反算的系数
	UGdouble m_dD4;
	UGdouble m_dD6;
	UGdouble m_dD8;

	UGdouble m_dF2;//等面积纬度直接计算的系数
	UGdouble m_dF4;
	UGdouble m_dF6;
	UGdouble m_dF8;
//{{Utm & Transverse Mercator only... 2000 11 20 Teng
	//{{ test dongfei
	UGdouble m_dEN[5];
	UGdouble m_dMeridianLength0;//坐标点的经线弧长	
	UGdouble MeridianLength(UGdouble dPhi, UGdouble dSinPhi, UGdouble dCosPhi);	//不知道为什么注释掉了 开了试试
	UGdouble MeridianLengthInverse(UGdouble dValue);
	//}} test dongfei
//}}Utm & Transverse Mercator only... 2000 11 20 Teng
	UGdouble m_dR;//地球平均半径
	UGdouble m_dR2;//地球平均半径平方

	UGdouble m_dRm;//地球平均半径 = 椭球体三轴平均值 
	UGdouble m_dRf;//与椭球体表面积相等的 地球平均半径

	
	UGdouble m_dDistanceRation;//沿纬线的长度比
	UGdouble m_dDistanceRatiom;//沿经线的长度比
	UGdouble m_dAngleDistortion;//最大角度变形

	//! \brief added by qianjn for bonne prj。
	UGdouble m_dBonne_M0;
	UGdouble m_dBonne_m0;
	UGdouble m_dBonne_c0;
	UGdouble m_dBonne_c1;
	UGdouble m_dBonne_c2;
	UGdouble m_dBonne_c3;
	UGdouble m_dBonne_a0;
	UGdouble m_dBonne_a1;
	UGdouble m_dBonne_a2;
	UGdouble m_dBonne_a3;

//added by qianjn in 2009-8-31 13:44:10 增加原因：斜墨卡托投影

	UGdouble m_dObliqueMercator_A;
	UGdouble m_dObliqueMercator_B;
	UGdouble m_dObliqueMercator_H;
	UGdouble m_dObliqueMercator_lmd0;
	UGdouble m_dObliqueMercator_gmd0;
	UGdouble m_dObliqueMercator_uc;
	UGint m_dObliqueMercator_temp;

//added by qianjn in 2009-8-31 13:44:10 增加完毕


	
	UGdouble m_dLam2 ;
	UGdouble m_dCosPhip1 ;
	UGdouble m_dCosPhip2 ;
	UGdouble m_dSinPhip1 ;
	UGdouble m_dSinPhip2 ;
	UGdouble m_dCosp1Sinp2;
	UGdouble m_dCosp2Sinp1;
	UGdouble m_dCosp1Cosp2Sinlam2;
	UGdouble m_dZ02, m_dHZ0;
	UGdouble m_dCosA, m_dSinA, m_dLP, m_dLamC; //m_dLam2, 
	UGdouble m_dTanHZ0, m_dRSinHZ0, m_dRZ0;//, m_dZ02;
   
	UGbool   m_bIsValid;

	// for prj4
	UGString m_strprjParam;// +lat_0=0 +lon_0=0 +x_0=0 +y_0=0 +k=0.99984 +lat_1=0.0 +lat_2=0.0  +lon_1=0.0 +lon_2=0.0 +alpha=53.315820472200002
	UGString m_strSpheroidType;//+ellps
	
private://method
	//! \brief 经度归算到(-PI--PI)
	UGdouble AdjLongitude(UGdouble lon);
	//! \brief 求卯酉圈半径
	UGdouble N(UGdouble dPhi/* 单位：弧度*/);// ( AVERAGE_SPHERE_RADIUS / sqrt( 1 - EPS2 * sin(phi) * sin(phi) ) )
	//! \brief 经线曲率半径
	UGdouble M(UGdouble dPhi/* 单位：弧度*/);// ( R*(1-EPS2)/pow((1-EPS2*sin(phi)*sin(phi)),1.5) )
	//! \brief 反解经线曲率半径
	UGdouble AntiM(UGdouble dM);

	//! \brief 等角表象纬度函数
	UGdouble U(UGdouble dPhi);// (tan(FORTPI+phi/2)/pow(tan(FORTPI+asin(EPS*sin(phi))/2),EPS)  )

	//! \brief 计算等距离纬度直接反算的系数
	void ComputeEquidistCoefficient();

	//! \brief 经线弧长
	UGdouble Distance( UGdouble dPhi);//	A*phi+B*sin(2*phi)+C*sin(4*phi)+D*sin(6*phi)+E*sin(8*phi)
	//! \brief 底点纬度----等距离纬度反解-->用于等距投影
	UGdouble AntiDistance( UGdouble dPhi );// 公式：2.9.33

	//! \brief 等量纬度正解
	UGdouble Conformal( UGdouble dPhi );
	//! \brief 等量纬度----等量纬度反解 --->用于等角投影
	UGdouble AntiConformal( UGdouble dPhi );// 公式：2.10.11

	//! \brief 地球椭圆梯形面积公式
	UGdouble Area( UGdouble dPhi );//  (R*R*(1-EPS2) * (sin(phi)  / (2* (1-EPS2*sin(phi)*sin(phi)) ) + log( (1+EPS*sin(phi)) / (1-EPS*sin(phi)) )/(4*EPS) ) )
	//! \brief 等面积纬度----等面积纬度反解 --->用于等积投影
	UGdouble AntiArea( UGdouble dPhi ); // 公式：2.11.11


	//! \brief 普通多圆锥求纬度公式
	UGdouble PolyConicB (UGdouble dB, UGdouble dX, UGdouble dY);//(pow(Sm(b)+N(b)/tan(b)-x,2) + y*y - pow(N(b)/tan(b),2))
	UGdouble PolyConicB1(UGdouble dB, UGdouble dY);			 //(2*N(b)/tan(b) * (M(b)-Sm(b)/tan(b)+x/tan(b)))

	//! \brief 中国全图伪方位投影alpha求值公式
	UGdouble ChinaA ( UGdouble dAlpha, UGdouble dZ, UGdouble dTheta );//(a+0.011697143*Z*sin(3*(15*PI/180+a))-theta)
	UGdouble ChinaA1( UGdouble dAlpha, UGdouble dZ );				 //(1+0.035091429*Z*cos(3*(15*PI/180+a)))

	//! \brief 爱凯特正弦投影常数alpha求值公式
	UGdouble EckertA ( UGdouble dAlpha , UGdouble dPhi );	//(sin(a)+a-(PI+2)*sin(phi)/2)
	UGdouble EckertA1( UGdouble dAlpha );				//(1+cos(a))

	//! \brief 摩尔威特投影常数alpha求值公式
	UGdouble MollweideA ( UGdouble dAlpha , UGdouble dPhi );	//( sin( 2 * a ) + 2 * a - PI * sin( phi ) )
	UGdouble MollweideA1( UGdouble dAlpha );				//( 2 + 2 * cos( 2 * a ) )
	
	//for prj4
	UGString GetStringPrjParam(UGbool blon_0 );
	UGString GetStringSpheroidType();
	// prj4 都从此处调用
	UGbool prjForward(UGString strPrjName,UGdouble& x, UGdouble& y,UGbool blon_0);
	UGbool prjInverse(UGString strPrjName,UGdouble& x, UGdouble& y,UGbool blon_0);
/*----------------------水平参照系转换函数--------------------------------------*/
private:
//	UGbool TransHorizonDatum(UGdouble& x, UGdouble& y);
/*----------------------各种投影变换函数----------------------------------------*/
private:
	//! \brief 等距圆柱投影
	UGbool Equidistant_Cylindrical_Forward(UGdouble& x, UGdouble& y);
	UGbool Equidistant_Cylindrical_Inverse(UGdouble& x, UGdouble& y);
	
	//! \brief 等角圆柱投影(墨卡托投影)
	UGbool Conformal_Cylindrical_Forward(UGdouble& x, UGdouble& y);	
	UGbool Conformal_Cylindrical_Inverse(UGdouble& x, UGdouble& y);	
	
	//! \brief 等积圆柱投影
	UGbool EqualArea_Cylindrical_Forward(UGdouble& x, UGdouble& y);	
	UGbool EqualArea_Cylindrical_Inverse(UGdouble& x, UGdouble& y);	

	//! \brief 横切等角圆柱投影(UTM)
	UGbool Traverse_Conformal_Cylindrical_Forward(UGdouble& x, UGdouble& y);	
	UGbool Traverse_Conformal_Cylindrical_Inverse(UGdouble& x, UGdouble& y);	
	
	//! \brief 横切等距圆柱投影
	UGbool Traverse_Equidistant_Cylindrical_Forward(UGdouble& x, UGdouble& y);
	UGbool Traverse_Equidistant_Cylindrical_Inverse(UGdouble& x, UGdouble& y);
	
	//! \brief 横切等积圆柱投影
//	UGbool Traverse_EqualArea_Cylindrical_Forward(UGdouble& x, UGdouble& y);
//	UGbool Traverse_EqualArea_Cylindrical_Inverse(UGdouble& x, UGdouble& y);

	//! \brief 高斯克吕格投影---等角横切椭圆柱投影
	UGbool GaussKruger_Forward( UGdouble& x,UGdouble& y );//
	UGbool GaussKruger_Inverse( UGdouble& x,UGdouble& y );//

	//! \brief 等距圆锥投影
	UGbool Equidistant_Conic_Forward( UGdouble& x, UGdouble& y );
	UGbool Equidistant_Conic_Inverse( UGdouble& x, UGdouble& y );
	
	//! \brief 等角圆锥投影——兰伯特正形圆锥投影
	UGbool Conformal_Conic_Forward( UGdouble& x, UGdouble& y );//Test Ok!
	UGbool Conformal_Conic_Inverse( UGdouble& x, UGdouble& y );//Test Ok!
	
	//! \brief 等积圆锥投影（当N_LATITUDE为2时称为亚尔勃斯投影(Alberts);
	UGbool EqualArea_Conic_Forward( UGdouble& x, UGdouble& y );
	UGbool EqualArea_Conic_Inverse( UGdouble& x, UGdouble& y );

	UGbool Miller_Cylindrical_Forward(UGdouble& x, UGdouble& y);
	UGbool Miller_Cylindrical_Inverse(UGdouble& x, UGdouble& y);

	UGbool Sinusoidal_Forward(UGdouble& x, UGdouble& y);
	UGbool Sinusoidal_Inverse(UGdouble& x, UGdouble& y);

	//! \brief 摩尔威特投影——椭圆曲线等积伪圆柱投影
	UGbool Mollweide_Forward(UGdouble& x, UGdouble& y);
	UGbool Mollweide_Inverse(UGdouble& x, UGdouble& y);

	//! \brief 爱凯特投影----伪圆柱投影
	UGbool Eckert_Forward(UGdouble& x, UGdouble& y);
	UGbool Eckert_Inverse(UGdouble& x, UGdouble& y);
	//! \brief 爱凯特I投影----伪圆柱投影
	UGbool EckertI_Forward(UGdouble& x, UGdouble& y);
	UGbool EckertI_Inverse(UGdouble& x, UGdouble& y);
	//! \brief 爱凯特II投影----伪圆柱投影
	UGbool EckertII_Forward(UGdouble& x, UGdouble& y);
	UGbool EckertII_Inverse(UGdouble& x, UGdouble& y);
	//! \brief 爱凯特III投影----伪圆柱投影
	UGbool EckertIII_Forward(UGdouble& x, UGdouble& y);
	UGbool EckertIII_Inverse(UGdouble& x, UGdouble& y);
	//! \brief 爱凯特IV投影----伪圆柱投影
	UGbool EckertIV_Forward(UGdouble& x, UGdouble& y);
	UGbool EckertIV_Inverse(UGdouble& x, UGdouble& y);
	//! \brief 爱凯特V投影----伪圆柱投影
	UGbool EckertV_Forward(UGdouble& x, UGdouble& y);
	UGbool EckertV_Inverse(UGdouble& x, UGdouble& y);
	//! \brief 爱凯特VI投影----伪圆柱投影
	UGbool EckertVI_Forward(UGdouble& x, UGdouble& y);
	UGbool EckertVI_Inverse(UGdouble& x, UGdouble& y);

	//! \brief ROBINSON投影----伪圆柱投影
	UGbool Robinson_Forward(UGdouble& x, UGdouble& y);
	UGbool Robinson_Inverse(UGdouble& x, UGdouble& y);

	//! \brief 彭纳投影----等积伪圆锥投影
	UGbool Bonne_Forward(UGdouble& x, UGdouble& y);
	UGbool Bonne_Inverse(UGdouble& x, UGdouble& y);

	//! \brief 中国全图方位投影
	UGbool China_Forward(UGdouble& x, UGdouble& y);//极坐标原点的地理纬度默认为35度
	UGbool China_Inverse(UGdouble& x, UGdouble& y);
	//! \brief 普通多圆锥投影
	UGbool Normal_Polyconic_Forward(UGdouble& x, UGdouble& y); //passed!
	UGbool Normal_Polyconic_Inverse(UGdouble& x, UGdouble& y); //passed!
//===========================
	//! \brief 正射方位投影
	UGbool Orthograph_Forward(UGdouble& x, UGdouble& y);
	UGbool Orthograph_Inverse(UGdouble& x, UGdouble& y);
	//! \brief 球心投影
	UGbool Gnomonic_Forward(UGdouble& x, UGdouble& y);
	UGbool Gnomonic_Inverse(UGdouble& x, UGdouble& y);
	
	//! \brief PRJ_GALL_STEREOGRAPHIC           
	UGbool Gall_Stereographic_Forward(UGdouble& x, UGdouble& y);	//Ok!
	UGbool Gall_Stereographic_Inverse(UGdouble& x, UGdouble& y);	//Ok!

	UGbool WinkelII_Forward(UGdouble& x, UGdouble& y);
	UGbool WinkelII_Inverse(UGdouble& x, UGdouble& y);
	//! \brief 等积正弦伪圆柱投影
	UGbool WinkelI_Forward(UGdouble& x, UGdouble& y);
	UGbool WinkelI_Inverse(UGdouble& x, UGdouble& y);

	UGbool Quartic_Authalic_Forward(UGdouble& x, UGdouble& y);	//Ok!
	UGbool Quartic_Authalic_Inverse(UGdouble& x, UGdouble& y);	//Ok!

	//! \brief Loximuthal
	UGbool Loximuthal_Forward(UGdouble& x, UGdouble& y);
	UGbool Loximuthal_Inverse(UGdouble& x, UGdouble& y);

	UGbool Hotine_Forward(UGdouble& x, UGdouble& y);
	UGbool Hotine_Inverse(UGdouble& x, UGdouble& y);

	UGbool Stereographic_Forward(UGdouble& x, UGdouble& y);
	UGbool Stereographic_Inverse(UGdouble& x, UGdouble& y);

	UGbool Van_Der_Grinten_I_Forward(UGdouble& x, UGdouble& y);	//Ok!
	UGbool Van_Der_Grinten_I_Inverse(UGdouble& x, UGdouble& y);	//Ok!

	UGbool Two_Point_Equidistant_Forward(UGdouble& x, UGdouble& y);	//Ok!
	UGbool Two_Point_Equidistant_Inverse(UGdouble& x, UGdouble& y);	//Ok!

	//! \brief 等距离方位投影
	UGbool Equidistant_Azimuth_Forward(UGdouble& x, UGdouble& y);
	UGbool Equidistant_Azimuth_Inverse(UGdouble& x, UGdouble& y);

	//! \brief 等面积方位投影
	UGbool EqualArea_Azimuth_Forward(UGdouble& x, UGdouble& y);
	UGbool EqualArea_Azimuth_Inverse(UGdouble& x, UGdouble& y);
	
	//! \brief 等角方位投影
	UGbool Conformal_Azimuth_Forward(UGdouble& x, UGdouble& y);
	UGbool Conformal_Azimuth_Inverse(UGdouble& x, UGdouble& y);


	//! \brief 桑逊投影——正弦曲线等积伪圆柱投影
	UGbool Sanson_Forward(UGdouble& x, UGdouble& y);
	UGbool Sanson_Inverse(UGdouble& x, UGdouble& y);

	UGbool Hotine_Azimuth_Natorigin_Forward(UGdouble& x, UGdouble& y);
	UGbool Hotine_Azimuth_Natorigin_Inverse(UGdouble& x, UGdouble& y);


	UGbool Oblique_Mercator_Forward(UGdouble& x, UGdouble& y);
	UGbool Oblique_Mercator_Inverse(UGdouble& x, UGdouble& y);
		
	UGbool Hotine_Oblique_Mercator_Forward(UGdouble& x, UGdouble& y);
	UGbool Hotine_Oblique_Mercator_Inverse(UGdouble& x, UGdouble& y);

	//! \brief  (正球墨卡托投影)
	UGbool Sphere_Mercator_Forward(UGdouble& x, UGdouble& y);	
	UGbool Sphere_Mercator_Inverse(UGdouble& x, UGdouble& y);	

	//! \brief 彭纳投影----等积伪圆锥投影
	UGbool Bonne_South_Orientated_Forward(UGdouble& x, UGdouble& y);
	UGbool Bonne_South_Orientated_Inverse(UGdouble& x, UGdouble& y);

	UGbool Oblique_Stereographic_Forward(UGdouble& x, UGdouble& y);
	UGbool Oblique_Stereographic_Inverse(UGdouble& x, UGdouble& y);

	UGbool	Baidu_Mercator_Forward(UGdouble& x, UGdouble& y);
	UGbool	Baidu_Mercator_Inverse(UGdouble& x, UGdouble& y);

	UGbool Rectified_Skewed_Orthomorphic_Forward(UGdouble& x, UGdouble& y);
	UGbool Rectified_Skewed_Orthomorphic_Inverse(UGdouble& x, UGdouble& y);
};


}//namespace UGC



#endif // !defined(UGC_SMPRJTRANSLATOR_H__DCD0ED9B_E520_4376_8EAF_D9F8E7338057__INCLUDED_)

