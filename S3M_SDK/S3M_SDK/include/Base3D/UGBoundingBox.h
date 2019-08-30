//////////////////////////////////////////////////////////////////////////
//                    _   _   ____   ____                    
//   Project         | | | | / ___) / ___)                   
//                   | | | || | __ | |                       
//                   | |_| || |_\ || \___                    
//                    \____/ \____| \ ___)     6.0           
//                                                           
//!  \file UGBoundingBox.h
//!  \brief 包围盒类
//!  \details 封装了三维几何体的包围盒
//!  \author 
//!  \attention 
//!   Copyright (c) 1996-2010 SuperMap Software Co., Ltd. <br> 
//!   All Rights Reserved.
//!  \version 6.0
/*
         |y
         |
         2____________3
        /|          /|
      6/_|________7/ |
       | |0________|_|1
	   | /         | /
	   |/__________|/_____x
	   4           5
      /
     /z
*/
//////////////////////////////////////////////////////////////////////////


#if !defined(AFX_UGBOUNDINGBOX_H__29C05AF1_76E7_49B0_A62C_239A57A92F80__INCLUDED_)
#define AFX_UGBOUNDINGBOX_H__29C05AF1_76E7_49B0_A62C_239A57A92F80__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Base3D/UGPrerequisites3D.h"
#include "Base3D/UGVector3d.h"

namespace UGC
{
//! \brief 包围盒类
class BASE3D_API UGBoundingBox  
{
public:
	//! \brief 构造函数
	UGBoundingBox();

	//! \brief 构造函数
	//! \param vMin 包围盒的最小向量[in]。
	//! \param vMax 包围盒的最大向量[in]。
	UGBoundingBox(UGVector3d vMin,UGVector3d vMax);
	
	//! \brief 构造函数
	//! \param v0-v7 构成包围盒的八个点[in]。
	UGBoundingBox(const UGVector3d& v0, const UGVector3d& v1, const UGVector3d& v2, const UGVector3d& v3, const UGVector3d& v4, const UGVector3d& v5, const UGVector3d& v6, const UGVector3d& v7);	
	
	//! \brief 构造函数
	//! \param south 南纬[in]。
	//! \param north 北纬[in]。
	//! \param west 西经[in]。
	//! \param east 东经[in]。
	//! \param radius1 球半径1[in]。
	//! \param radius2 球半径2[in]。
	//! \remarks 由地理范围的面从半径1拉伸到半径2构成一个包围盒。
	UGBoundingBox(UGdouble south, UGdouble north, UGdouble west, UGdouble east, UGdouble radius1, UGdouble radius2);	
	
	UGBoundingBox(UGdouble south, UGdouble north, UGdouble west, UGdouble east);

	UGBoundingBox& operator = (const UGBoundingBox& boundingBox);

	//! \brief 判断两个包围盒是否相等。
	//! \param &boundingBox 另外一个包围盒[in]。
	//! \return 若相等返回为TRUE，若不相等则返回为FALSE。
	UGbool operator == (const UGBoundingBox &boundingBox);

	//! \brief 析构函数
	virtual ~UGBoundingBox();
	
	//! \brief 设置构成包围盒的最小向量。
	//! \param vMin 构成包围盒的最小向量[in]。
	void SetMin(UGVector3d vMin);	
	
	//! \brief 设置构成包围盒的最大向量。
	//! \param vMax 构成包围盒的最大向量[in]。
	void SetMax(UGVector3d vMax);
	
	//! \brief 设置构成包围盒的最小向量。
	//! \param x,y,z 构成包围盒的最小向量[in]。
	void SetMin(UGdouble x,UGdouble y,UGdouble z);
	
	//! \brief 设置构成包围盒的最大向量。
	//! \param x,y,z 构成包围盒的最大向量[in]。
	void SetMax(UGdouble x,UGdouble y,UGdouble z);

	//! \brief  得到构成包围盒的最小向量
	const UGVector3d& GetMin(void) const;
	
	//! \brief 得到构成包围盒的最大向量
	const UGVector3d& GetMax(void) const;
	
	//! \brief 设置构成包围盒的最大、最小向量。
	//! \param vMin 构成包围盒的最大最小向量[in]。
	//! \param vMax 构成包围盒的最大向量[in]。
	void SetExtents(const UGVector3d& vMin,const UGVector3d& vMax);	
	
	//! \brief 设置构成包围盒的最大、最小向量。
	//! \param (minx,miny,minz) 构成包围盒的最大最小向量[in]。
	//! \param (maxx,maxy,maxz) 构成包围盒的最大向量[in]。
	void SetExtents(UGdouble minx, UGdouble miny,UGdouble minz,
						   UGdouble maxx, UGdouble maxy, UGdouble maxz);

	//! \brief 对包围盒的顶点做变换，注意此函数只将顶点变换，变换后不调用UpdateCorners
	// 进过次变换后可能会得到倾斜的包围盒，这样的包围盒对于判断与视景体的相交特别准确
	void MultiplyMatrix(const UGMatrix4d& mat);

	//! \brief 根据输入的点扩展Box。
	//! \param vPoint 待扩展的点
	void SetExtentsByPoint(const UGVector3d& vPoint);
	
	//! \brief 设置射线的近视点。
	//! \param 近视点坐标[in]。
	void SetNearPos(UGVector3d value);
	
	//! \brief 设置射线的远视点。
	//! \param 远视点坐标[in]。
	void SetFarPos(UGVector3d value);
	
	//! \brief 设置交点。
	//! \param 交点坐标[in]。
	void SetInsectPos(UGVector3d value);
	
	//! \brief 得到射线的近视点
	UGVector3d GetNearPos();

	//! \brief 得到射线的远视点
	UGVector3d GetFarPos();
	
	//! \brief 得到射线和包围盒的交点
	UGVector3d GetInsectPos();
	
	//! \brief 得到构成包围盒的8各点。
	const UGVector3d* GetCorners()const;	
	
	//! \brief 得到包围盒的中心点。
	//! \remarks 这个中心点是构成包围盒的最小向量和最大向量的平均值。
	UGVector3d GetCenter() const;	
	
	//! \brief 对包围盒进行矩阵变换
	//! \param matrix 对包围盒进行变换的矩阵[in]。
	void Transform(UGMatrix4d& matrix);	
	
	//! \brief 将两个包围盒进行合并。
	//! \param BoundBox 要进行合并的包围盒[in]。
	void Merge(const UGBoundingBox& BoundBox);	
	
	//! \brief 设置包围盒为空
	void SetNULL(void);
	
	//! \brief 判断包围盒是否为空
	UGbool IsNULL(void)const;
	
	//! \brief 缩放包围盒。
	//! \remarks 按x、y、z方向分别缩放dScaleX、dScaleY、dScaleZ倍。
	void Scale(UGdouble dScaleX, UGdouble dScaleY, UGdouble dScaleZ);
	
	//! \brief 计算包围盒的中心点。
	//! \remarks 这个中心点是构成包围盒8个交点的平均值。
	UGVector3d CalculateCenter();
	
	//! \brief 判断射线与包围盒是否相交。
	//! \param nearPos 构成射线的近视点[in]。
	//! \param farPos 构成射线的远视点[in]。
	//! \param dist 交点与近视点的距离[out]。
	UGbool InterSectWithRadial(const UGVector3d& nearPos, const UGVector3d& farPos, UGdouble& dist) const; 	

	//! \brief 判断两个包围盒是否相交。
	UGbool IsIntersect(const UGBoundingBox& boundingBox) const;
	
	//! \brief 渲染包围盒
	void Render();	
	
	//! \brief 得到射线和包围盒的交点。
	//! \return 范围射线和包围盒的交点。
	UGVector3d GetIntersectedPoint();	

	//! \brief 射线和包围盒相交。
	//! \param rayOrg 射线的近视点[in]。
	//! \param rayDelta 射线的远视点[in]。
	//! \param returnNormal [out]。
	UGdouble RayIntersect(const UGVector3d& rayOrg, const UGVector3d& rayDelta, UGVector3d& returnNormal)const;
	
	//! \brief 判断有效性
	//! \return 包围盒是否有效
	UGbool IsVaild();

protected:
	
	//! \brief 根据最大最小向量更新包围盒的8个角点。
	void UpdateCorners(void);
	
	// 只与底面相交算法,效率为logn, 有可能有问题
	//! \brief 判断射线与包围盒每四个角点构成面是否相交。
	//! \param corner1 包围盒的角点[in]。
	//! \param corner2 包围盒的角点[in]。
	//! \param corner3 包围盒的角点[in]。
	//! \param corner4 包围盒的角点[in]。
	//! \param nearPos 射线的近视点[in]。
	//! \param farPos 射线的远视点[in]。
	//! \return 射线与平面是否相交。
	UGbool InterSectFaceWithRadial(UGint corner1, UGint corner2, UGint corner3, UGint corner4,
		                           const UGVector3d& nearPos, const UGVector3d& farPos);
	
	//! \brief 判断射线与包围盒每四个角点构成面是否相交。
	//! \param corner1 包围盒的角点[in]。
	//! \param corner2 包围盒的角点[in]。
	//! \param corner3 包围盒的角点[in]。
	//! \param corner4 包围盒的角点[in]。
	//! \param nearPos 射线的近视点[in]。
	//! \param farPos 射线的远视点[in]。
	//! \param dist  交点与近视点的距离[out]。
	//! \return 射线与平面是否相交。
	//! \remarks 可以与任意面相交, 效率为2logn。
	UGbool IntersectBoxFaces(UGint Corner1, UGint Corner2, UGint Corner3,UGint Corner4,
											const UGVector3d& nearPos, const UGVector3d& farPos, UGdouble& dist) const;	

	
	//! \brief 判断射线与包围盒每四个角点构成面是否相交。
	//! \param corner1 包围盒的角点[in]。
	//! \param corner2 包围盒的角点[in]。
	//! \param corner3 包围盒的角点[in]。
	//! \param corner4 包围盒的角点[in]。
	//! \param nearPos 射线的近视点[in]。
	//! \param farPos 射线的远视点[in]。
	//! \param t  交点与近视点的距离[out]。
	//! \param vecIntersect  交点[out]。
	//! \return 射线与平面是否相交。
	UGbool IntersectPointAndRegion(UGint Corner1, UGint Corner2, UGint Corner3,UGint Corner4,
									const UGVector3d& nearPos, const UGVector3d& farPos, 
									UGdouble& t, UGVector3d& vecIntersect);

		
	//! \brief 边的法向。
	//! \param e 边，长度为3的数组[in]。
	//! \param n 边的法向，长度为3的数组[out]。
	void Perp(UGdouble* e, UGdouble* n);	

	//! \brief 判断是否是合法实数。
	//! \param dValue 实数[in]。
	//! \return true为合法，false为非法。
	UGbool IsRealNaN(UGdouble dValue);
protected:
	//! \brief 构成包围盒的最小向量。
	UGVector3d m_vMin;
	//! \brief 构成包围盒的最大向量。
	UGVector3d m_vMax;
	//! \brief 构成包围盒8个角点。
	UGVector3d m_vCorner[8];
	//! \brief 标志包围盒是否为空。
	UGbool m_bNull;		
	//! \brief  与包围盒进行相交的射线的近视点。
	UGVector3d m_nearPos;
	//! \brief 与包围盒进行相交的射线的远视点。
	UGVector3d m_farPos;
	//! \brief 射线与包围盒进行相交的交点。
	UGVector3d m_InsectPos;
	//! \brief 暂时无用。
	UGVector3d m_vecIntersectedPoint;
};
}
#endif // !defined(AFX_UGBOUNDINGBOX_H__29C05AF1_76E7_49B0_A62C_239A57A92F80__INCLUDED_)

