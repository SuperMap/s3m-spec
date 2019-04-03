//////////////////////////////////////////////////////////////////////////
//                    _   _   ____   ____                    
//   Project         | | | | / ___) / ___)                   
//                   | | | || | __ | |                       
//                   | |_| || |_\ || \___                    
//                    \____/ \____| \ ___)     6.0           
//                                                           
//!  \file headerfile.h
//!  \brief 三维文件包含。
//!  \details 三维所需要的基本文件包含。
//!  \author gaoff。
//!  \attention 
//!   Copyright (c) 1996-2010 SuperMap Software Co., Ltd. <br> 
//!   All Rights Reserved.
//!  \version 6.0
//////////////////////////////////////////////////////////////////////////

#ifndef UG3DPREREQUISITES_H
#define UG3DPREREQUISITES_H
#ifdef WIN32
#pragma warning(disable : 4786)
#endif


#include "Stream/ugdefs.h"
#include "Stream/UGFile.h"
#include "Toolkit/UGThread.h"
#include "Toolkit/UGToolkit.h"
#include "Base3D/UGCommon3D.h"

namespace UGC
{
class UGVector3d;
class UGVector4d;
class UGMatrix3d;
class UGMatrix4d;
class UGQuaternion4d;
class UGPlane;
class UGBoundingBox;
class UGColorValue3D;
class UGTerrainTile;
class UGTerrainTileServer;
class UGTerrainAccessor;
class UGTerrainManager;
class UGTerrainTileCacheEntry;

}

#endif

