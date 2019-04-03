//////////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)           
//                                                           
//!  \file OgdcColorTable.h
//!  \brief 颜色对照表 
//!  \attention 栅格数据支持颜色对照表方式显示
//!  \author Jingefei
//!  \Copyright (c) 2007-2012 SuperMap Software Co., Ltd.  <br> 
//!  \All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////

#ifndef __OGDCCOLORDICTTABLE_H__
#define __OGDCCOLORDICTTABLE_H__


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ogdcdefs.h"
#include "OgdcString.h"
#include "OgdcColorTable.h"

namespace OGDC {

	typedef std::map< OgdcDouble,OgdcColor,std::less<OgdcDouble> > ColorDictTable;
	typedef ColorDictTable::iterator ClrDictTableIterator;

	//!  \brief 颜色对照表类 
	class BASE_API OgdcColorDictTable
	{
	public:
		//! \brief 颜色对照表基类默认构造函数
		//! \param 
		//! \return 
		//! \remark
		OgdcColorDictTable();
		
		//! \brief 颜色对照表基类默认析构函数
		//! \param 
		//! \return 
		//! \remark
		~OgdcColorDictTable();  

		//! \brief 颜色对照表基类拷贝构造函数
		//! \param OgdcColorDictTable常对象
		//! \return 
		//! \remark
		OgdcColorDictTable(const OgdcColorDictTable& clrDictTable);

		//! \brief  重载=
		//! \param  OgdcColorDictTable常对象
		//! \return OgdcColorDictTable对象
		//! \remark
		OgdcColorDictTable& operator=(const OgdcColorDictTable& clrDictTable);

	public:
		//! \brief  增加颜色对照表项
		//! \param  dValue:高程值
		//! \param  clr:颜色值
		//! \return 如果插入成功则返回true，否则返回false
		//! \remark 如果插入的高程值已经存在，则插入失败
		OgdcBool Insert( OgdcDouble dValue, OgdcColor clr );

		//! \brief  删除一个颜色对照表项
		//! \param  dValue:需要删除的高程值对应项
		//! \return 如果删除的高程值存在，则删除成功，否则失败
		//! \remark
		OgdcBool Remove( OgdcDouble dValue );

		//! \brief  清除所有颜色对照表项
		//! \param 
		//! \return 
		//! \remark
		void Clear();

		//! \brief  根据Key值插值出颜色值
		//! \param  dValue:高程值
		//! \return 颜色值
		//! \remark 如果高程值是对照表中的某一项，则直接返回对应的颜色值，
		//! \remark 若小于对照表中的最小高程值，则返回最小高程值,
		//! \remark 若大于对照表中的最大高程值，则返回最大高程值，
		//! \remark 若在最小高程值与最大高程值之间，则找出邻近的两项做线性插值
		OgdcColor GetColor( OgdcDouble dValue ) const;

		//! \brief  编辑颜色对照表项
		//! \param  dValue:需要更改颜色的高程值，Key值
		//! \param  clrNew:新的颜色值
		//! \return 如果高程值是对照表中的某一项，则更改成功，若不是，则返回失败
		//! \remark
		OgdcBool SetColor( OgdcDouble dValue, OgdcColor clrNew );

		//! \brief  获取一个颜色对照表项
		//! \param  iIndex:索引值
		//! \param  dValue:索引对应的高程值
		//! \param  clr:索引对应的颜色值
		//! \return 若iIndex在对照表的数量范围内，则返回成功，否则返回失败
		//! \remark
		OgdcBool GetItem( OgdcInt iIndex, OgdcDouble &dValue, OgdcColor &clr ) const;

		//! \brief  获取颜色对照表项的个数
		//! \param 
		//! \return 颜色对照表中对应项的个数
		//! \remark
		OgdcInt GetCount() const;

		//! \brief  判断高程值是否已经存在
		//! \param
		//! \return
		//! \remark
		OgdcBool IsExist( OgdcDouble dValue );

		// 复制
		void Copy( const OgdcColorDictTable &clrDictTable );

		void calcKeys();
	public:
		//! \brief  对照表转颜色表
		//! \param  
		//! \return 
		//! \remark
		static OgdcBool ColorDictTableToColorTable( const OgdcColorDictTable &clrDictTable, OgdcColorTable &colorTable );
		
		//! \brief 颜色表转对照表
		//! \param 
		//! \return 
		//! \remark
		static OgdcBool ColorTableToColorDictTable( const OgdcColorTable &colorTable, OgdcColorDictTable &clrDictTable );

		// 判断颜色对照表和颜色表是否一致
		static OgdcBool IsEqual( const OgdcColorDictTable &clrDictTable, const OgdcColorTable &colorTable );

	private:
		//! \brief 获取Key值最小值项
		OgdcBool GetMinItem( OgdcDouble &dZ, OgdcColor &color );

		//! \brief 获取Key值最大值项
		OgdcBool GetMaxItem( OgdcDouble &dZ, OgdcColor &color );

		//! \brief 根据Key值找出对应的相邻两项
		void GetPreAndNextIterator( OgdcDouble dValue, ClrDictTableIterator &iterPre, ClrDictTableIterator &iterNext );

		void GetPreAndNextValueColor( OgdcDouble dValue, OgdcDouble &dPreValue, OgdcColor &clrPre, OgdcDouble &dNextValue, OgdcColor &clrNext );

	private:
		//! \brief find_if比较函数
		static OgdcBool Compare( std::pair<OgdcDouble,OgdcColor> data, OgdcDouble dValue );

	private:
		//! \brief 默认升序存储
		ColorDictTable m_clrDictTable;
		OgdcDouble * m_pKeys;
		OgdcInt m_iCount;
	};

}


#endif // !defined(__OGDCCOLORDICTTABLE_H__)

