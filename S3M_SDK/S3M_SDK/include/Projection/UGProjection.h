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
// 文件名：			UGProjection.h
// 类  名：			UGProjection
// 父  类：			
// 子  类：			

// 功能说明：		投影方法定义
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

#if !defined(UGC_SMPROJECTION_H__0EBE9968_FA20_4C93_A922_7DAD98A06FF5__INCLUDED_)
#define UGC_SMPROJECTION_H__0EBE9968_FA20_4C93_A922_7DAD98A06FF5__INCLUDED_


//// #include "SmImport.h"

#include "Toolkit/UGThread.h"
#include "Stream/ugdefs.h"

#include "Projection/UGPjCon.h"

namespace UGC{


class PROJECTION_API UGProjection  
{
	friend class UGPrjFactory;
public:
	//! \brief 默认构造函数
	UGProjection();
	//! \brief 根据投影方式类型构造投影方式对象
	UGProjection(EmPrjObjectType eTypeID);
#ifdef SYMBIAN60
	~UGProjection();
#else
	virtual ~UGProjection();
#endif
	UGProjection(const UGProjection& Projection);
	UGProjection& operator = (const UGProjection& Projection);
	UGbool operator == (const UGProjection& Projection) const;
	UGbool operator != (const UGProjection& Projection) const;
	
	//! \brief 获取投影方式类型
	EmPrjObjectType GetProjectionType() const;
	//! \brief 设置投影方式类型并重构投影方式对象
	UGbool SetTypeID(EmPrjObjectType nTypeID);

	//! \brief 获取投影方式名称
	UGString GetProjectionName() const;

	//! \brief 输出为XML字符串
	UGString ToXML(UGint nVersion = 0) const;
	//! \brief 读取XML字符串为投影方式对象
	UGbool FromXML(const UGString& strPrjXml, UGint nVersion = 0);

	//! \brief 从配置文件读取投影方式信息 dongfei 2007-04-04	
	UGbool FromConfigure( EmPrjObjectType nTypeID,UGint nVersion = 0);
	
	// 用于满足用户自定义投影类型需要，目前SuperMap支持48种投影，ArcMap支持64种投影
	// 针对暂不支持的投影推荐用户自行完成，这里用户可以自行设置一个投影值
	// 不能使用43000 -- 43047里面的值，这些为预定义的值
	UGbool Set(EmPrjObjectType nTypeID, const UGString& strName);

private:
	EmPrjObjectType m_eTypeID;
	UGString m_strName;
	UGMutex	 m_mutex;
};

}//namespace UGC



#endif // !defined(UGC_SMPROJECTION_H__0EBE9968_FA20_4C93_A922_7DAD98A06FF5__INCLUDED_)

