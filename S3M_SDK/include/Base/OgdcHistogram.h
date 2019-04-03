

//////////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)           
//                                                           
//!  \file OgdcHistogram.h
//!  \brief 直方图
//!  \attention 
//!  \author Jingefei
//!  \Copyright (c) 2007-2012 SuperMap Software Co., Ltd.  <br> 
//!  \All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////

#ifndef __OGDCHISTOGRAM_H__
#define __OGDCHISTOGRAM_H__


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ogdcdefs.h"
#include "OgdcString.h"

namespace OGDC {

	// 直方图
	class BASE_API OgdcHistogram
	{
	public:
		OgdcDouble	dHistMin;
		OgdcDouble	dHistMax;
		OgdcInt    		nBucketCount;
		OgdcInt    		nBandIndex;
		OgdcArray<OgdcInt> arrHistCount;	// 频数

	public:
		OgdcHistogram();
		OgdcHistogram(const OgdcHistogram& histogram);
		OgdcHistogram& operator=(const OgdcHistogram& histogram);
	};
}


#endif // !defined(__OGDCHISTOGRAM_H__)

