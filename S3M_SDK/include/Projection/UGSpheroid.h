 //=====================================================================================
// 项目名：			SuperMap Universal GIS class (UGC) Library 0.9
// 作  者：			张立立
// 单  位：			北京超图软件股份有限公司
// 最后修改时间：	以文件日期为准
//-------------------------------------------------------------------------------------
//版权声明:         版权所有 2000-2010 北京超图软件股份有限公司，保留所有权利 
//	                Copyright (c) 2000-2010 SuperMap Software Co., Ltd. 
//	                All rights reserved

// 重要声明：		1. 
//					2. 
//					
//-------------------------------------------------------------------------------------
// 文件名：			UGSpheroid.h 
// 类  名：			UGSpheroid
// 父  类：			
// 子  类：			

// 功能说明：		地球椭球体定义
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

#if !defined(UGC_SMSPHEROID_H__82E32637_BEBE_4772_A8A7_BAA2E786E35F__INCLUDED_)
#define UGC_SMSPHEROID_H__82E32637_BEBE_4772_A8A7_BAA2E786E35F__INCLUDED_


//// #include "SmImport.h"
#include "Toolkit/UGThread.h"
#include "Stream/ugdefs.h"

#include "Projection/UGPjCon.h"

namespace UGC{


class PROJECTION_API UGSpheroid  
{
	friend class UGPrjFactory;
public:
	//! \brief 默认构造函数
	UGSpheroid();
	//! \brief 根据椭球体类型构造椭球体对象
	UGSpheroid(EmSpheroidType nTypeID );
	
	//! \brief 根据长轴长度、扁率和椭球体名称构造椭球体对象
	UGSpheroid(UGdouble dAxis, UGdouble dFlatten, 
		UGString strName);
	UGSpheroid(const UGSpheroid& Spheroid);

#ifdef SYMBIAN60
	~UGSpheroid();
#else
	virtual ~UGSpheroid();
#endif

	UGSpheroid& operator=(const UGSpheroid& Spheroid);
	UGbool operator==(const UGSpheroid& Spheroid) const;
	UGbool operator != (const UGSpheroid& Spheroid) const;

	//! \brief 获取椭球体类型
	EmSpheroidType GetTypeID() const;
	//! \brief 设置椭球体类型
	void SetTypeID(EmSpheroidType nTypeID);

	//! \brief 获取椭球体长轴长度
	UGdouble GetAxis() const;
	//! \brief 设置椭球体长轴长度
	UGbool SetAxis(UGdouble dAxis);
	//! \brief 获取椭球体名称
	UGString GetName() const;
	//! \brief 设置椭球体名称
	void SetName(const UGString& strName);
	//! \brief 获取椭球体扁率
	UGdouble GetFlatten() const;
	//! \brief 设置椭球体扁率
	UGbool SetFlatten(UGdouble dFlatten);

	//! \brief 输出为XML字符串
	UGString ToXML(UGint nVersion = 0)const;
	//! \brief 从XML字符串读取椭球体信息
	UGbool FromXML(const UGString& strPrjXml, UGint nVersion = 0);

	//! \brief 从配置文件读取椭球体信息 dongfei 2007-04-04	
	UGbool FromConfigure(EmSpheroidType nTypeID,UGint nVersion = 0);

	UGbool IsValid(){return m_bValidSpheroid;}
	UGString GetSpheroidTypeString() const ;//for prj4
protected:
	void SetIsValid(UGbool bValid){m_bValidSpheroid = bValid;}

protected:	
	EmSpheroidType m_eTypeID;   // 椭球参数类型
	UGdouble  m_dAxis;   // 地球长半轴(赤道半径)
	UGdouble  m_dFlatten;// 扁率
	UGString m_strName;// 名字
	UGbool m_bValidSpheroid; //椭球体是否合法
	UGMutex	 m_mutex;
};

}//namespace UGC



#endif // !defined(UGC_SMSPHEROID_H__82E32637_BEBE_4772_A8A7_BAA2E786E35F__INCLUDED_)

