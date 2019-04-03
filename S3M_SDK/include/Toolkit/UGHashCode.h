// UGHashCode.h: interface for the UGHashCode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UGHASHCODE_H__655A005A_77A3_426D_B890_151E99E6F0F3__INCLUDED_)
#define AFX_UGHASHCODE_H__655A005A_77A3_426D_B890_151E99E6F0F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Stream//ugdefs.h"

namespace UGC {
	  //!  \brief Toolkit
class TOOLKIT_API UGHashCode  
{
public:
	  //!  \brief Toolkit
	UGHashCode();
	  //!  \brief Toolkit
	~UGHashCode();
public:
	//! \brief
	  //!  \brief Toolkit
	static UGint LONGToHashCode(UGlong nValue);
	  //!  \brief Toolkit
	//! \brief
	static UGint ShortToHashCode(UGshort nValue);
	//! \brief
	  //!  \brief Toolkit
	static UGint StringToHashCode(const UGString& strValue);
	//! \brief
	  //!  \brief Toolkit
	static UGint BoolToHashCode(UGbool bValue);
	//! \brief
	  //!  \brief Toolkit
	static UGint PointerToHashCode(void* pValue);
	//! \brief
	  //!  \brief Toolkit
	static UGint DoubleToHashCode(UGdouble dValue);
	//! \brief
	  //!  \brief Toolkit
	static UGint FloatToHashCode(UGfloat fValue);
	//! \brief
	  //!  \brief Toolkit
	static UGint ByteToHashCode(UGbyte btValue);

	//! \brief
	//!  \brief Toolkit
	static UGint PointToHashCode(UGPoint pntValue);

	//! \brief
	//!  \brief Toolkit
	static UGint Point2DToHashCode(UGPoint2D pntValue);

	//! \brief
	//!  \brief Toolkit
	static UGint Point3DToHashCode(UGPoint3D pntValue);

	//! \brief 快速把字符串转化为HashCode
	//! \remarks 由于要和SFC保持兼容,StringToHashCode中进行了Unicode的转化,非常费时
	//! 其它不需要保持兼容的地方,可以用这个函数,速度快很多
	  //!  \brief Toolkit
	static UGint FastStringToHashCode(const UGString& strValue);

	//! \brief 字符串转HashCode
	static UGlong StringToHashCode64(const UGString& strValue);
};
}
#endif // !defined(AFX_UGHASHCODE_H__655A005A_77A3_426D_B890_151E99E6F0F3__INCLUDED_)

