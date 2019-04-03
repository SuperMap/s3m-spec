//////////////////////////////////////////////////////////////////////////
//                    _   _   ____   ____                    
//   Project         | | | | / ___) / ___)                   
//                   | | | || | __ | |                       
//                   | |_| || |_\ || \___                    
//                    \____/ \____| \ ___)     6.0           
//                                                           
//!  \file UGByteArray.h
//!  \brief UGByteArray 的接口定义。
//!  \details 主要用来操作内存字节流，解决跨平台的大小端问题，具有和兼容MemoryStream的功能。
//!           同时内部有一个 Position 自动控制 当前操作的内存字节流的位置
//!  \author 崔文柱
//!  \attention 
//!   Copyright (c) 1996-2008 SuperMap GIS Technologies,Inc.  <br> 
//!   All Rights Reserved.
//!  \version 6.0
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_UGBYTEARRAY_H__ABD143F6_543A_49A1_8D21_E3E5AD753F3E__INCLUDED_)
#define AFX_UGBYTEARRAY_H__ABD143F6_543A_49A1_8D21_E3E5AD753F3E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Stream/ugdefs.h"

namespace UGC {

class STREAM_API UGByteArray 
{
public:
	//! \brief 默认构造函数
	UGByteArray();
	//! \brief 默认析构函数
	~UGByteArray();
public:
	
	//! \brief 在 UGByteArray 最后添加一个对象
	//! \param nValue 要添加的对象
	//! \return 返回Position
	//! \remarks 如果原来 Position在其他位置，会自动移到最后的位置。
	UGSizeT Add(UGbyte nValue);
	inline UGSizeT Add(UGachar chValue)
	{
		return Add(UGbyte(chValue));
	}
	UGSizeT Add(UGshort nValue);
	inline UGSizeT Add(UGushort nValue)
	{
		return Add(UGshort(nValue));
	}
	#if _MSC_VER > 1200
	inline UGSizeT Add(UGwchar chvalue)
	{
		return Add(UGshort(chvalue));
	}
	#endif
	UGSizeT Add(UGint nValue);
	inline UGSizeT Add(UGuint nValue)
	{
		return Add(UGint(nValue));
	}
	UGSizeT Add(UGlong nValue);
	inline UGSizeT Add(UGulong nValue)
	{
		return Add(UGlong(nValue));
	}
	UGSizeT Add(UGfloat dValue);
	UGSizeT Add(UGdouble dValue);
	inline UGSizeT Add(const UGString& strValue)
	{
		UGASSERT(strValue.Cstr()!=NULL);
#ifdef _UGUNICODE
		if(m_eCharset == UGString::Unicode || m_eCharset == UGString::UCS2LE)
		{
			UGint nLength=0;
			nLength=strValue.GetLength();
			UGASSERT(nLength>0);
			UGint nByteLength = nLength * sizeof(UGchar);
			Add(nByteLength);
			return Add((UGushort*)strValue.Cstr(),nLength);
		}
		else
		{
			UGMBString strMB;
			strMB.SetCharset(m_eCharset);
			strValue.ToMBString(strMB);
			UGint nLength=0;
			nLength=strMB.GetLength();
			UGASSERT(nLength>0);
			Add(nLength);
			return Add(strMB.Cstr(),nLength);
		}
#else
		Add(strValue.GetLength());
		return Add(strValue.Cstr(),strValue.GetLength());
#endif
	}

	inline UGSizeT Add(const UGPoint& pntValue)
	{
		return Add((UGint *)&pntValue,2);
	}

	inline UGSizeT Add(const UGPoint2D& pnt2Value)
	{
		return Add((UGdouble *)&pnt2Value,2);
	}

	inline UGSizeT Add(const UGPoint3D& pnt3Value)
	{
		return Add((UGdouble *)&pnt3Value,3);	
	}

	inline UGSizeT Add(const UGRect& rcValue)
	{
		return Add((UGint *)&rcValue,4);
	}

	inline UGSizeT Add(const UGRect2D& rc2Value)
	{
		return Add((UGdouble *)&rc2Value,4);
	}

	inline UGSizeT Add(const UGSize& szValue)
	{
		return Add((UGint *)&szValue,2);
	}

	inline UGSizeT Add(const UGSize2D& sz2Value)
	{
		return Add((UGdouble *)&sz2Value,2);
	}
	UGSizeT Add(const UGbyte* pBytes, UGSizeT nCount);
	inline UGSizeT Add(const UGachar* pChars, UGSizeT nCount)
	{
		return Add((UGbyte*)pChars,nCount);
	}
	UGSizeT Add(const UGshort *pShorts, UGSizeT nCount);
	inline UGSizeT Add(const UGushort *pUshorts, UGSizeT nCount)
	{
		return Add((UGshort *)pUshorts,nCount);
	}
	#if _MSC_VER > 1200
	inline UGSizeT Add(const UGwchar *pValues, UGSizeT nCount)
	{
		return Add((UGshort *)pValues, nCount);
	}
	#endif
	UGSizeT Add(const UGint *pInts, UGSizeT nCount);
	inline UGSizeT Add(const UGuint *pUints, UGSizeT nCount)
	{
		return Add((UGint*)pUints,nCount);
	}

	UGSizeT Add(const UGlong *pLongs, UGSizeT nCount);
	inline UGSizeT Add(const UGulong *pUlongs, UGSizeT nCount)
	{
		return Add((UGlong*)pUlongs,nCount);
	}
	UGSizeT Add(const UGfloat* pFloats, UGSizeT nCount);
	UGSizeT Add(const UGdouble* pDoubles, UGSizeT nCount);
	UGSizeT Add(const UGPoint* pPoints, UGSizeT nCount);
	UGSizeT Add(const UGPoint2D* pPoint2Ds, UGSizeT nCount);
	UGSizeT Add(const UGPoint3D* pPoint3Ds, UGSizeT nCount); 	
	
	
	
	//! \brief 在Position 位置设置一个对象
	//! \param nValue 要添加的对象
	//! \return 返回Position
	//! \remarks Position会自动移到设置数值的后面。
	//!          如果原来Position后有内容，会被覆盖。如果空间不够会自动增长，
	//!          如果设置自动增长量,Capacity 按照自动增长量增长，
	//!          如果没有设置自动增长量，Capacity会以*2方式相应的大小，
	//!          建议大家设置自动增长量
	//! \attention 如果字节流长度不够，这里面会申请内存，如果失败，可能崩溃，
	//!            无法确定指针的位置，注意要提前保留指针位置
	UGSizeT Set(UGbyte nValue);
	inline UGSizeT Set(UGachar chValue)
	{
		return Set(UGbyte(chValue));
	}
	UGSizeT Set(UGshort nValue);
	inline UGSizeT Set(UGushort nValue)
	{
		return Set(UGshort(nValue));
	}
	#if _MSC_VER > 1200
	inline UGSizeT Set(UGwchar nWChar)
	{
		return Set(UGshort(nWChar));
	}
	#endif
	UGSizeT Set(UGint nValue);
	inline UGSizeT Set(UGuint nValue)
	{
		return Set(UGint(nValue));
	}
	UGSizeT Set(UGlong nValue);
	inline UGSizeT Set(UGulong nValue)
	{
		return Set(UGlong(nValue));
	}
	UGSizeT Set(UGfloat dValue);
	UGSizeT Set(UGdouble dValue);
	
	inline UGSizeT Set(const UGString& strValue)
	{
		UGASSERT(strValue.Cstr()!=NULL);
#ifdef _UGUNICODE
		if(m_eCharset == UGString::Unicode || m_eCharset == UGString::UCS2LE)
		{
			UGint nLength=0;
			nLength=strValue.GetLength();
			UGASSERT(nLength>0);
			UGint nByteLength = nLength * sizeof(UGchar);
			Add(nByteLength);
			return Set((UGushort*)strValue.Cstr(),nLength);
		}
		else
		{
			UGMBString strMB;
			strMB.SetCharset(m_eCharset);
			strValue.ToMBString(strMB);
			UGint nLength=0;
			nLength=strMB.GetLength();
			UGASSERT(nLength>0);
			Set(nLength);
			return Set(strMB.Cstr(),nLength);
		}
#else
		UGint nLength=0;
		nLength=strValue.GetLength();
		UGASSERT(nLength>0);

		Set(nLength);
		return Set(strValue.Cstr(),nLength);
#endif
	}

	inline UGSizeT Set(const UGPoint& pntValue)
	{
		return Set((UGint*)&pntValue,2);
	}

	inline UGSizeT Set(const UGPoint2D& pnt2Value)
	{
		return Set((UGdouble*)&pnt2Value,2);
	}

	inline UGSizeT Set(const UGPoint3D& pnt3Value)
	{
		return Set((UGdouble*)&pnt3Value,3);
	}

	inline UGSizeT Set(const UGRect& rcValue)
	{
		return Set((UGint*)&rcValue,4);
	}

	inline UGSizeT Set(const UGRect2D& rc2Value)
	{
		return Set((UGdouble *)&rc2Value,4);
	}

	inline UGSizeT Set(const UGSize& szValue)
	{
		return Set((UGint*)&szValue,2);
	}

	inline UGSizeT Set(const UGSize2D& sz2Value)
	{
		return Set((UGdouble*)&sz2Value,2);
	}

	
	UGSizeT Set(const UGbyte* pBytes, UGSizeT nCount);
    inline UGSizeT Set(const UGachar* pChars, UGSizeT nCount)
	{
		return Set((UGbyte*)pChars,nCount);
	}
	UGSizeT Set(const UGshort *pShorts, UGSizeT nCount);
	inline UGSizeT Set(const UGushort *pUshorts, UGSizeT nCount)
	{
		return Set((UGshort*)pUshorts,nCount);
	}
	#if _MSC_VER > 1200
	inline UGSizeT Set(const UGwchar *pValues, UGSizeT nCount)
	{
		return Set((UGshort*)pValues, nCount);
	}
	#endif
	UGSizeT Set(const UGint *pInts, UGSizeT nCount);
	inline UGSizeT Set(const UGuint *pUints, UGSizeT nCount)
	{
		return Set((UGint *)pUints,nCount);
	}
	UGSizeT Set(const UGlong *pLongs, UGSizeT nCount);
	inline UGSizeT Set(const UGulong *pUlongs, UGSizeT nCount)
	{
		return Set((const UGlong *)pUlongs,nCount);
	}
	UGSizeT Set(const UGfloat* pFloats, UGSizeT nCount);
	UGSizeT Set(const UGdouble* pDoubles, UGSizeT nCount);
	UGSizeT Set(const UGPoint* pPoints, UGSizeT nCount);
	UGSizeT Set(const UGPoint2D* pPoint2Ds, UGSizeT nCount);
	UGSizeT Set(const UGPoint3D* pPoint3Ds, UGSizeT nCount); 	
    
	
	
	//! \brief 在Position 位置插入一个对象，同时原来Position的内容后移，
	//!        有效对象的Size会自动变大
	//! \param nValue 要添加的对象
	//! \return 返回Position
	//! \remarks Position会自动移到插入对象的后面，
	//! \attention 这里面会申请并移动内存，如果失败，可能崩溃，
	//!            无法确定指针的位置，注意要提前保留指针位置
	UGSizeT Insert(UGbyte nValue);
	inline UGSizeT Insert(UGachar chValue)
	{
		return Insert(UGbyte(chValue));
	}
	UGSizeT Insert(UGshort nValue);
	inline UGSizeT Insert(UGushort nValue)
	{
		return Insert(UGshort(nValue));
	}
	#if _MSC_VER > 1200
	inline UGSizeT Insert(UGwchar nValue)
	{
		return Insert(UGshort(nValue));
	}
	#endif
	UGSizeT Insert(UGint nValue);
	inline UGSizeT Insert(UGuint nValue)
	{
		return Insert(UGint(nValue));
	}
	UGSizeT Insert(UGlong nValue);
	inline UGSizeT Insert(UGulong nValue)
	{
		return Insert(UGlong(nValue));
	}
	UGSizeT Insert(UGfloat dValue);
	UGSizeT Insert(UGdouble dValue);
	
	inline UGSizeT Insert(const UGString& strValue)
	{
#ifdef _UGUNICODE
		if(m_eCharset == UGString::Unicode || m_eCharset == UGString::UCS2LE)
		{
			UGint nLength=0;
			nLength=strValue.GetLength();
			UGint nByteLength = nLength * sizeof(UGchar);
			Insert(nByteLength);
			return Insert((UGushort*)strValue.Cstr(),nLength);
		}
		else
		{
			UGMBString strMB;
			strMB.SetCapacity(m_eCharset);
			strValue.ToMBString(strMB);
			UGint len = strMB.GetLength();
			Insert(len);
			return Insert(strMB.Cstr(),len);
		}
#else
		UGint nLength=0;
		nLength=strValue.GetLength();
		Insert(nLength);
		return Insert((const UGbyte *)strValue.Cstr(),nLength);
#endif
	}

	inline UGSizeT Insert(const UGPoint& pntValue)
	{
		return Insert((UGint *)&pntValue,2);
	}

	inline UGSizeT Insert(const UGPoint2D& pnt2Value)
	{
		return Insert((UGdouble *)&pnt2Value,2);
	}

	inline UGSizeT Insert(const UGPoint3D& pnt3Value)
	{
		return Insert((UGdouble *)&pnt3Value,3);
	}

	inline UGSizeT Insert(const UGRect& rcValue)
	{
		return Insert((UGint*)&rcValue,4);	
	}

	inline UGSizeT Insert(const UGRect2D& rc2Value)
	{
		return Insert((UGdouble *)&rc2Value,4);
	}
	inline UGSizeT Insert(const UGSize& szValue)
	{
		return Insert((UGint *)&szValue,2);	
	}

	inline UGSizeT Insert(const UGSize2D& sz2Value)
	{
		return Insert((UGdouble *)&sz2Value,2);
	}

	UGSizeT Insert(const UGbyte* pBytes, UGSizeT nCount);
    inline UGSizeT Insert(const UGachar* pChars, UGSizeT nCount)
	{
		return 	Insert((UGbyte *)pChars,nCount);
	}

	UGSizeT Insert(const UGshort *pShorts, UGSizeT nCount);
	inline UGSizeT Insert(const UGushort *pUshorts, UGSizeT nCount)
	{
		return Insert((UGshort*)pUshorts,nCount);
	}
#if _MSC_VER > 1200
	inline UGSizeT Insert(const UGwchar* pValues, UGSizeT nCount)
	{
		return Insert((UGshort*)pValues, nCount);
	}
#endif

	UGSizeT Insert(const UGint *pInts, UGSizeT nCount);
	inline UGSizeT Insert(const UGuint *pUints, UGSizeT nCount)
	{
		return Insert((UGint*)pUints,nCount);
	}	

	UGSizeT Insert(const UGlong *pLongs, UGSizeT nCount);
	inline UGSizeT Insert(const UGulong *pUlongs, UGSizeT nCount)
	{
		return Insert((UGlong*)pUlongs,nCount);
	}
	UGSizeT Insert(const UGfloat* pFloats, UGSizeT nCount);
	UGSizeT Insert(const UGdouble* pDoubles, UGSizeT nCount);
	UGSizeT Insert(const UGPoint* pPoints, UGSizeT nCount);
	UGSizeT Insert(const UGPoint2D* pPoint2Ds, UGSizeT nCount);
	UGSizeT Insert(const UGPoint3D* pPoint3Ds, UGSizeT nCount); 	
	
	
	
	//! \brief 在Position 位置获取一个UGByte对象
	//! \param nValue 获取的UGByte对象
	//! \return 返回Position
	//! \remarks Position会自动移到获取数值的后面。
	//! \attention 如果失败,无法确定指针的位置，注意要提前保留指针位置
	UGSizeT Get(UGbyte& nValue);
	UGSizeT Get(UGachar& chValue);
	UGSizeT Get(UGshort& nValue);
	#if _MSC_VER > 1200
	inline UGSizeT Get(UGwchar& nValue)
	{
		return Get((UGshort&)nValue);
	}
	#endif
	UGSizeT Get(UGushort& nValue);
	UGSizeT Get(UGint& nValue);
	UGSizeT Get(UGuint& nValue);
	UGSizeT Get(UGlong& nValue);
	UGSizeT Get(UGulong& nValue);
	UGSizeT Get(UGfloat& dValue);
	UGSizeT Get(UGdouble& dValue);
	UGSizeT Get(UGString& strValue);

	inline UGSizeT Get(UGPoint& pntValue)
	{
		return Get((UGint *)&pntValue,2);
	}

	inline UGSizeT Get(UGPoint2D& pnt2Value)
	{
		return Get((UGdouble *)&pnt2Value,2);
	}

	inline UGSizeT Get(UGPoint3D& pnt3Value)
	{
		return Get((UGdouble *)&pnt3Value,3);
	}
	inline UGSizeT Get(UGRect& rcValue)
	{
		return Get((UGint *)&rcValue,4);	
	}
	inline UGSizeT Get(UGRect2D& rc2Value)
	{
		return Get((UGdouble *)&rc2Value,4);
	}
	inline UGSizeT Get(UGSize& szValue)
	{
		return Get((UGint *)&szValue,2);
	}
	inline UGSizeT Get(UGSize2D& sz2Value)
	{
		return Get((UGdouble *)&sz2Value,2);
	}

	UGSizeT Get(UGbyte* pBytes, UGSizeT nCount);
    inline UGSizeT Get(UGachar* pChars, UGSizeT nCount)
	{
		return Get((UGbyte *)pChars,nCount);
	}
	UGSizeT Get(UGshort *pShorts, UGSizeT nCount);
	inline UGSizeT Get(UGushort *pUshorts, UGSizeT nCount)
	{
		return Get((UGshort *)pUshorts,nCount);
	}
	#if _MSC_VER > 1200
	inline UGSizeT Get(UGwchar *pValues, UGSizeT nCount)
	{
		return Get((UGshort *)pValues, nCount);
	}
	#endif
	UGSizeT Get(UGint *pInts, UGSizeT nCount);
	inline UGSizeT Get(UGuint *pUints, UGSizeT nCount)
	{
		return Get((UGint *)pUints,nCount);
	}

	UGSizeT Get(UGlong *pLongs, UGSizeT nCount);
	inline UGSizeT Get(UGulong *pUlongs, UGSizeT nCount)
	{
		return Get((UGlong*)pUlongs,nCount);
	}

	UGSizeT Get(UGfloat* pFloats, UGSizeT nCount);
	UGSizeT Get(UGdouble* pDoubles, UGSizeT nCount);
	UGSizeT Get(UGPoint* pPoints, UGSizeT nCount);
	UGSizeT Get(UGPoint2D* pPoint2Ds, UGSizeT nCount);
	UGSizeT Get(UGPoint3D* pPoint3Ds, UGSizeT nCount); 	
   

	//! \brief 在Position 位置设置一个对象
	//! \param nValue 要添加的对象
	//! \return 返回Position
	//! \remarks Position会自动移到设置数值的后面。
	//!          如果原来Position后有内容，会被覆盖。如果空间不够会自动增长，
	//!          如果设置自动增长量,Capacity 按照自动增长量增长，
	//!          如果没有设置自动增长量，Capacity会以*2方式相应的大小，
	//!          建议大家设置自动增长量
	//! \attention 如果字节流长度不够，这里面会申请内存，如果失败，可能崩溃，
	//!            无法确定指针的位置，注意要提前保留指针位置

	inline UGByteArray& operator<<(UGbyte nValue)
	{
		Set(nValue);
		return *this;
	}
	inline UGByteArray& operator<<(UGachar chValue)
	{
		Set(chValue);
		return *this;
	}
	inline UGByteArray& operator<<(UGshort nValue)
	{
		Set(nValue);
		return *this;
	}
	#if _MSC_VER > 1200
	inline UGByteArray& operator<<(UGwchar nValue)
	{
		Set(nValue);
		return *this;
	}
	#endif
	inline UGByteArray& operator<<(UGushort nValue)
	{
		Set(nValue);
		return *this;
	}
	inline UGByteArray& operator<<(UGuint nValue)
	{
		Set(nValue);
		return *this;
	}

	inline UGByteArray& operator<<(UGint nValue)
	{
		Set(nValue);
		return *this;
	}
	inline UGByteArray& operator<<(UGlong nValue)
	{
		Set(nValue);
		return *this;
	}
	inline UGByteArray& operator<<(UGulong nValue)
	{
		Set(nValue);
		return *this;
	}
	inline UGByteArray& operator<<(UGfloat dValue)
	{
		Set(dValue);
		return *this;
	}
	inline UGByteArray& operator<<(UGdouble dValue)
	{
		Set(dValue);
		return *this;
	}
	inline UGByteArray& operator<<(const UGString &strValue)
	{
		Set(strValue);	
		return *this;
	}
	inline UGByteArray& operator<<(const UGPoint& pntValue)
	{
		Set(pntValue);
		return *this;
	}
	inline UGByteArray& operator<<(const UGPoint2D& pnt2Value)
	{
		Set(pnt2Value);
		return *this;
	}
	inline UGByteArray& operator<<(const UGPoint3D& pnt3Value)
	{
		Set(pnt3Value);
		return *this;
	}
	inline UGByteArray& operator<<(const UGRect& rcValue)
	{
		Set(rcValue);
		return *this;
	}
	inline UGByteArray& operator<<(const UGRect2D& rc2Value)
	{
		Set(rc2Value);
		return *this;
	}
	inline UGByteArray& operator<<(const UGSize &szValue)
	{
		Set(szValue);
		return *this;
	}
	inline UGByteArray& operator<<(const UGSize2D& sz2Value)
	{
		Set(sz2Value);
		return *this;
	}

	inline UGByteArray& operator>>(UGbyte&  nValue)
	{
		Get(nValue);
		return *this;
	}

	inline UGByteArray& operator>>(UGachar& chValue)
	{
		Get(chValue);
		return *this;
	}

	inline UGByteArray& operator>>(UGshort& nValue)
	{
		Get(nValue);
		return *this;
	}
	#if _MSC_VER > 1200
	inline UGByteArray& operator>>(UGwchar& nValue)
	{
		Get(nValue);
		return *this;
	}
	#endif

	inline UGByteArray& operator>>(UGushort& nValue)
	{
		Get(nValue);
		return *this;
	}

	inline UGByteArray& operator>>(UGuint& nValue)
	{
		Get(nValue);
		return *this;
	}

	inline UGByteArray& operator>>(UGint& nValue)
	{
		Get(nValue);
		return *this;
	}

	inline UGByteArray& operator>>(UGlong& nValue)
	{
		Get(nValue);
		return *this;
	}

	inline UGByteArray& operator>>(UGulong& nvalue)
	{
		Get(nvalue);
		return *this;
	}

	inline UGByteArray& operator>>(UGfloat& dValue)
	{
		Get(dValue);
		return *this;
	}

	inline UGByteArray& operator>>(UGdouble& dValude)
	{
		Get(dValude);
		return *this;
	}

	inline UGByteArray& operator>>(UGString &strValue)
	{
		Get(strValue);
		return *this;
	}
	inline UGByteArray& operator>>(UGPoint& pntValue)
	{
		Get(pntValue);
		return *this;
	}
	inline UGByteArray& operator>>(UGPoint2D& pnt2Value)
	{
		Get(pnt2Value);
		return *this;
	}
	inline UGByteArray& operator>>(UGPoint3D& pnt3Value)
	{
		Get(pnt3Value);
		return *this;
	}
	inline UGByteArray& operator>>(UGRect& rcValue)
	{
		Get(rcValue);
		return *this;
	}
	inline UGByteArray& operator>>(UGRect2D& rc2Value)
	{
		Get(rc2Value);
		return *this;
	}
	inline UGByteArray& operator>>(UGSize &szValue)
	{
		Get(szValue);
		return *this;
	}
	inline UGByteArray& operator>>(UGSize2D& sz2Value)
	{
		Get(sz2Value);
		return *this;
	}

	//! \brief 获取当前Position
	//! \return 返回Position
	inline UGSizeT GetPosition() const
	{
		return m_nPos;
	}
	
	//! \brief 设置 Position
	//! \param nPos 设置的位置
	//! \return 返回实际设置的Position
	//! \remarks 如果 nPos<0 Position=0 如果 nPos>字节流大小，Position 会在最后的位置
	inline UGSizeT SetPosition(UGSizeT nPos)
	{
		m_nPos=nPos;
		if(m_nPos>m_array.GetSize())
			m_nPos=m_array.GetSize();
		if(m_nPos<0)
			m_nPos=0;
		return m_nPos;

	}
	
	//! \brief 拷贝UGByteArray
	//! \param aryByte 被拷贝的 UGByteArray
	//! \remarks 拷贝后的Position和aryByte的Position位置相同
	inline void Copy(const UGByteArray& aryByte)
	{
		m_array.Copy(aryByte.m_array);
		m_nPos=aryByte.GetPosition();
		m_bSwap = aryByte.SwapBytes();
	}
	
	//! \brief 从 Position 开始 移除nCount个元素,同时有效对象的Size变小
	//! \param nCount 要移除的元素个数
	//! \return 返回实际移除元素的个数
	//! \remarks 指定位置必须在字节流范围之内，如果指定的个数过大,会移除Position后的所有有效对象
	inline UGSizeT Remove(UGSizeT nCount)
	{
		return m_array.RemoveAt(m_nPos,nCount);
	}
	
	//! \brief 移除所有元素
	//! \remarks Position 置零
	inline void RemoveAll()
	{
		m_nPos=0;
		m_array.RemoveAll();
	}
	
	//! \brief 调整UGByteArray的有效数据的内存空间为 nSize,
	//! \remarks 如果原有有效数据的空间不够, 内存空间会增加, 如果指定自动增长量，
	//!          同时按照自动增长量的大小来增长Capacity 
	//!          如果原有空间比指定的大, 不会释放空间,
	//!          但 在指定的空间之后的元素就无效了
	//!          如果Position大于nSize ,Position为nSize;
	//! \param nSize 指定的新的元素个数
    inline void SetSize(UGSizeT nSize)
	{
		if(m_nPos>nSize)
			m_nPos=nSize;
		m_array.SetSize(nSize);
	}

	//! \brief 得到有用的元素的个数
	//! \return 有用的元素的个数
	inline UGSizeT GetSize() const
	{
		return m_array.GetSize();
	}
	
	//! \brief 设置自动增长量
	//! \param nGrowSize 自动增长量
	//! \remarks 如果不调用此函数，默认的Capacity自动增长量是乘以2，调用此函数，如果
	//           add的时候，超过数组的容量，会按照自动增长量的大小增加
	inline void SetGrowSize(OgdcInt nGrowSize = 10)
	{
		m_array.SetGrowSize(nGrowSize);
	}
	
	//! \brief 获取自动增长量
	//! \return 获取的自动增长量
	inline UGSizeT GetGrowSize() const
	{
		return m_array.GetGrowSize();
	}

	//! \brief 确保UGByteArray有足够的内存空间存储 指定的元素个数
	//! \param nCapacity 指定可容纳的元素个数,必须大于等于0
	//! \remarks 如果原有空间不够, 内存空间会增加
	//!          如果原有空间比指定的大, 也不会释放空间，Capacity的大小也不会有所变化。
	//!          如果 当前Position 大于有用元素的个数，Position在有用元素的最后。
	inline void SetCapacity(UGSizeT nCapacity)
	{
		m_array.SetCapacity(nCapacity);
	}

	
	//! \brief 得到目前OgdcArray中所有空间(包括已经使用的和保留的)所能容纳的元素的个数
	//! \return 返回数组所有空间可容纳的元素个数
	inline UGSizeT GetCapacity() const
	{
		return m_array.GetCapacity();
	}
	
	//! \brief 得到数组内部指针(地址连续)
	//! \return 返回数组内部指针(地址连续)
	inline const UGbyte* GetData() const
	{
		return m_array.GetData();
	};

	// 按引擎的需求，重新开放该接口 by jiangzb 2011-11-7
	// deleted by zengzm 2006-3-9 改变是否交换字节顺序的东东太危险，还是内部自己判断，暂时封存
	inline void SetSwapBytes(UGbool s) {m_bSwap = s; }

	//! \brief 得到是否需要进行字节顺序的交换。
	//! \remarks 我们的数据都采用LittleEndian方式存储，在BigEndian的CPU中需要进行字节顺序交换。
	inline UGbool SwapBytes() const
	{
		return m_bSwap;
	}

	void SetCharset(UGString::Charset eCharset)
	{
		m_eCharset = eCharset;
	}
	UGString::Charset GetCharset()
	{
		return m_eCharset;
	}
private:
	UGArray<UGbyte> m_array;
	UGSizeT m_nPos;

	UGbool m_bSwap;

	// 考虑到Stream中要操作字符串，就需要考虑字符串的编码问题，增加该变量来控制对Stream
	UGString::Charset m_eCharset;

};

}

#endif // !defined(AFX_UGBYTEARRAY_H__ABD143F6_543A_49A1_8D21_E3E5AD753F3E__INCLUDED_)

/** UGByteArray 的主要功能使用说明:

    //范例程序，添加和获取数据，

	//比如现在我们有两个数据
	//	UGint nAdd[nCount]={2,-56,-45,25,-25};
	//  UGString strAdd="123456";
	//现在想添加到我们的字节流中，下面有几种方式
	UGByteArray mba;
	//首先想到的是根据实际要存储的大小 设置他的容量，应该等于或者稍微大于你要存储
	//的东西的大小 这样在添加的时候就不会在设置他的容量了，提高效率
	mba.SetCapacity(100);
	//第二想到的是设置他的容量自动增长量，万一我们添加的时候超过他的容量，
	//就会按照自动增长量来增长。自己可以控制，要不然的话，可就不知道要曾到多大咯。
    mba.SetGrowSize(100);
	//下面是我要添加的数据
	const UGint nCount=5;
	UGint nAdd[nCount]={2,-56,-45,25,-25};
	UGString strAdd="123456";
	//第一种方式添加数据，在数据的末尾添加数据，如果mba.GetSize为0，就相当于在Position=0的位置添加
	//同时Position也是根据添加数据自动移动的哦。
	//为什么,要UGint数组的大小呢，为取数据的时候考虑吗，万一取数据的时候不知道
	//你要取的数组的大小，可以先取大小，在取数组吗？
	mba.Add(nCount);
    mba.Add(&nAdd[0],nCount);
	mba.Add(strAdd);
    
	//另外一种添加方式,知道下面这点干什么使用的吗？
	
    UGint nSize=sizeof(UGint)+5*sizeof(UGint)+sizeof(UGint)+strAdd.GetLength();
	if(nSize>mba.GetSize())
		mba.SetSize(nSize);
	//上面为什么提前一次设置他的有效数据的大小呢，因为下面这种方式是
	//从你设置的Position开始覆盖以后的数据，如果遇到有效数据的空间不够
	//的话，每一次Set的时候都会SetSize他的有效的数据空间，如果添加很多次数据的话
	//效率很低，索性一次设置好哦。
	//sizeof(UGint)+5*sizeof(UGint) 肯定看的懂
	//sizeof(UGint)+strAdd.GetLength() 这和String的存储方式有关，他先存一个UGint
	//，也就是strAdd的长度，在存他的字符串，
	//至于其他类型吗应该想得到。
    mba.SetPosition(0);
	mba.Set(nCount);
	mba.Set(&nAdd[0],nCount);
	mba.Set(strAdd);

	//另外一种添加方式

	if(nSize>mba.GetSize())
		mba.SetSize(nSize);

	mba.SetPosition(0);
   	mba<<nCount;
	for(UGint i=0;i<nCount;i++)
	     mba<<nAdd[i];
	mba<<strAdd;
    
    
	
	//下面是获取数据，记得设置你要取的位置哦。
    mba.SetPosition(0);
	UGint nGetCount;
	UGString strGet;
	mba.Get(nGetCount);
	//记得提前分配内存哦。
	UGint* pGet =new UGint[nGetCount];
	mba.Get(pGet,nGetCount);
	mba.Get(strGet);
	delete [] pGet;

	//另外一种获取方式
	mba.SetPosition(0);
	UGint nGetCounts;
	UGString strGets;
	mba>>nGetCounts;
	UGint* pGets =new UGint[nGetCounts];
	for(UGint j=0;j<nGetCounts;j++)
		mba>>pGets[i];
	mba>>strGets;
     delete [] pGets;



  //下面是完整的程序
    
	UGByteArray mba;
	mba.SetCapacity(100);
	mba.SetGrowSize(100);

	const UGint nCount=5;
	UGint nAdd[nCount]={2,-56,-45,25,-25};
	UGString strAdd="123456";
	
    UGint nSize=sizeof(UGint)+5*sizeof(UGint)+sizeof(UGint)+strAdd.GetLength();
	if(nSize>mba.GetSize())
		mba.SetSize(nSize);

    mba.SetPosition(0);
	mba.Set(nCount);
	mba.Set(&nAdd[0],nCount);
	mba.Set(strAdd);
	
	mba.SetPosition(0);
	UGint nGetCount;
	UGString strGet;
	mba.Get(nGetCount);
	UGint* pGet =new UGint[nGetCount];
	mba.Get(pGet,nGetCount);
	mba.Get(strGet);
	delete [] pGet;
	 
	   
		 **/

