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
// 文件名：			SmDatum.h
// 类  名：			CSmDatum
// 父  类：			
// 子  类：			CSmHorizonDatum

// 功能说明：		大地参照系定义
// 调用说明：		
/*-------------------------------------版本更新----------------------------------------
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
=====================================================================================*/

#if !defined(UGC_SMDATUM_H__9C4E723C_C063_403C_9C10_F853FF033121__INCLUDED_)
#define UGC_SMDATUM_H__9C4E723C_C063_403C_9C10_F853FF033121__INCLUDED_


//// #include "SmImport.h"
//#include "Project/UGPrjExport.h"
#include "Projection/UGPjCon.h"

namespace UGC{



class PROJECTION_API UGDatum  
{
public:
	UGDatum();

#ifdef SYMBIAN60
	~UGDatum();
#else
	virtual ~UGDatum();
#endif

	//! \brief 获取大地参照系类型
	EmDatumType GetDatumType();
	//! \brief 设置大地参照系类型
	void SetDatumType(EmDatumType nType);
private:
	EmDatumType m_emDatumType;
};

}//namespace UGC


#endif // !defined(UGC_SMDATUM_H__9C4E723C_C063_403C_9C10_F853FF033121__INCLUDED_)

