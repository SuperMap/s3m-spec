//////////////////////////////////////////////////////////////////////////
//                    _   _   ____   ____                    
//   Project         | | | | / ___) / ___)                   
//                   | | | || | __ | |                       
//                   | |_| || |_\ || \___                    
//                    \____/ \____| \ ___)     6.0           
//                                                           
//!  \file UGBoundingSphere.h
//!  \brief 
//!  \details 
//!  \author 
//!  \attention 
//!   Copyright (c) 1996-2010 SuperMap Software Co., Ltd. <br> 
//!   All Rights Reserved.
//!  \version 6.0
//////////////////////////////////////////////////////////////////////////


#if !defined(AFX_UGBOUNDINGSPHERE_H__29C05AF1_76E7_49B0_A62C_239A57A92F80__INCLUDED_)
#define AFX_UGBOUNDINGSPHERE_H__29C05AF1_76E7_49B0_A62C_239A57A92F80__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Base3D/UGPrerequisites3D.h"
#include "Base3D/UGVector3d.h"
#include "Base3D/UGBoundingBox.h"

namespace UGC
{
	class BASE3D_API UGBoundingSphere
	{
	public:
		UGBoundingSphere(void);
		UGBoundingSphere(const UGVector3d& center, UGdouble radius);
		UGBoundingSphere(const UGBoundingSphere& bs);
		UGBoundingSphere(const UGBoundingBox& box);
		UGBoundingSphere( UGdouble south, UGdouble north, UGdouble west, UGdouble east
			, UGdouble dGlobalRadius1, UGdouble dGlobalRadius2);
		virtual ~UGBoundingSphere(void);

		//! \brief 判断包围球是否有效
		inline UGbool valid () const
		{
			return m_radius > 0.0;
		}

		//! \brief 获取包围盒
		inline UGBoundingBox GetBoundingBox() const
		{
			UGVector3d vMinVec(m_center.x - m_radius, m_center.y - m_radius, m_center.z - m_radius);
			UGVector3d vMaxVec(m_center.x + m_radius, m_center.y + m_radius, m_center.z + m_radius);
			return UGBoundingBox(vMinVec, vMaxVec);
		}

		inline void init()
		{
			m_center = UGVector3d(0.0,0.0,0.0);
			m_radius = -1;
		}

		inline UGVector3d GetCenter() const
		{
			return m_center;
		}

		inline UGdouble GetRadius() const
		{
			return m_radius;
		}

		//! \brief 根据给定的三维点扩展包围球，半径、球心都可能改变
		//! \param 待扩展的点
		void ExpandBy(const UGVector3d& vec);

		//！ 根据给定的三维点扩展包围球，半径改变,球心保持不变
		//! \param 带扩展的点
		void ExpandRadiusBy(const UGVector3d& vec);

		//！ \brief 给据给定的包围球来扩展，球心、半径都改变
		//!  \param 待扩展的球
		void ExpandBy(const UGBoundingSphere& bs);

		//！ \brief 给据给定的包围球来扩展，半径改变,球心保持不变
		//!  \param 带扩展的球
		void ExpandRadiusBy(const UGBoundingSphere& bs);

		//! \brief 射线与球面是否相交
		//! \param vecStart 起始坐标[in]。
		//! \param vecEnd 终点坐标[in]。
		//! \return 。
		//! \remarks 。
		UGbool IntersectionWithSphere(const UGVector3d& vecStart, const UGVector3d& vecEnd) const;

	public:
		UGVector3d m_center;
		UGdouble m_radius;
	};
}
#endif // !defined(AFX_UGBOUNDINGSPHERE_H__29C05AF1_76E7_49B0_A62C_239A57A92F80__INCLUDED_)

