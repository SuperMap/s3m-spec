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
// 文件名：			UGPrimeMeridian.h
// 类  名：			UGPrimeMeridian
// 父  类：			
// 子  类：			

// 功能说明：		本初子午线定义
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

#if !defined(UGC_SMPRIMEMERIDIAN_H__4FF3AA98_8D8B_43C7_8ED4_1F619E70CCC5__INCLUDED_)
#define UGC_SMPRIMEMERIDIAN_H__4FF3AA98_8D8B_43C7_8ED4_1F619E70CCC5__INCLUDED_


//// #include "SmImport.h"
#include "Toolkit/UGThread.h"
#include "Stream/ugdefs.h"

#include "Projection/UGPjCon.h"

namespace UGC{

class PROJECTION_API UGPrimeMeridian  
{
	friend class UGPrjFactory;
public:
	//! \brief 默认构造函数
	UGPrimeMeridian();
	//! \brief 根据中央经线类型构造中央经线对象
	UGPrimeMeridian(EmPrimeMeridianType nTypeID );
	//! \brief 根据中央经线值和中央经线名称构造中央经线对象
	UGPrimeMeridian(UGdouble dValue,UGString strName = _U(""));
	UGPrimeMeridian(const UGPrimeMeridian& PrimeMeridian);

	UGPrimeMeridian& operator = (const UGPrimeMeridian& PrimeMeridian);
	UGbool operator == (const UGPrimeMeridian& PrimeMeridian) const;
	UGbool operator != (const UGPrimeMeridian& PrimeMeridian) const;
	
	//! \brief 设置中央经线类型
	//! \remarks 设置中央经线类型后 会根据此类型重新构造中央经线对象
	UGbool SetTypeID(EmPrimeMeridianType nTypeID);
	
	//! \brief 获取中央经线类型
	EmPrimeMeridianType GetTypeID() const;
	
	//! \brief 获取中央经线值
	UGdouble GetValue() const;
	//! \brief 设置中央经线值
	UGbool SetValue(UGdouble dValue);

	//! \brief 获取中央经线名称
	UGString GetName() const;
	//! \brief 设置中央经线名称
	void SetName(const UGString& strName);

#ifdef SYMBIAN60
	~UGPrimeMeridian();
#else
	virtual ~UGPrimeMeridian();
#endif
	
	//! \brief 输出为XML字符串
	UGString ToXML(UGint nVersion = 0)const;
	//! \brief 从XML字符串读取为中央经线对象
	UGbool FromXML(const UGString& strPrjXml, UGint nVersion = 0);

	//! \brief 从配置文件读取子午线体信息 dongfei 2007-04-04
	UGbool FromConfigure(EmPrimeMeridianType nTypeID,UGint nVersion = 0);

protected:
	UGString				m_strName;
	UGdouble				m_dValue;
	EmPrimeMeridianType		m_eTypeID;
	UGMutex					m_mutex;
};

}//namespace UGC


#endif // !defined(UGC_SMPRIMEMERIDIAN_H__4FF3AA98_8D8B_43C7_8ED4_1F619E70CCC5__INCLUDED_)

