/* 
	\file UGQuaternion4d.h
	\brief 四元数类文件
	\author malq
	\attention
	Copyright(c) 2000-2010  SuperMap GIS Technologies,Inc.<br>
	All Rights Reserved
	<b>更新说明：。</b>
	<em>1、第一条说明。</em>
	<em>2、第二条说明。</em>
*/

#if !defined(AFX_UGQUATERNION4D_H__828A96C2_95EA_4DCC_9468_666159FA635E__INCLUDED_)
#define AFX_UGQUATERNION4D_H__828A96C2_95EA_4DCC_9468_666159FA635E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Base3D/UGPrerequisites3D.h"

namespace  UGC 
{

//! \brief 四元数类
//! \remarks 四元数主要负责空间旋转
class BASE3D_API UGQuaternion4d  
{

public:
	//! \brief 构造函数
	UGQuaternion4d();
	//! \brief 构造函数
	UGQuaternion4d(UGdouble x, UGdouble y, UGdouble z, UGdouble w);
	UGQuaternion4d(const UGVector3d& xaxis, const UGVector3d& yaxis, const UGVector3d& zaxis);
	//! \brief 析构函数
	virtual ~UGQuaternion4d();
	//! \brief 重载元算符四元数相乘 
	UGQuaternion4d operator*(const UGQuaternion4d& obj);
	//! \brief 重载元算符四元数相减
	UGQuaternion4d operator-(const UGQuaternion4d& obj)const;
	UGQuaternion4d operator+(const UGQuaternion4d& obj)const;
	UGQuaternion4d operator-()const;
	UGQuaternion4d operator*(UGdouble dScalar)const;
	//! \brief 重载元算符四元数和向量相乘
	UGVector3d operator* (const UGVector3d& v) const;

	//! \brief 由旋转轴向量转为四元数
	void FromAxes (const UGVector3d* akAxis);
	//! \brief 四元数转为3个正交的坐标轴
	void ToAxes (UGVector3d* akAxis);
	//! \brief 由三维旋转轴向量转为四元数
	void FromAxes (const UGVector3d& xAxis, const UGVector3d& yAxis, const UGVector3d& zAxis);
	//! \brief 由旋转矩阵转为四元树
	void FromRotationMatrix (const UGMatrix3d& kRot);
	void FromRotationMatrix (const UGMatrix4d& kRot);
	//! \brief 标准化四元数
	void Normalize();
	//! \brief 四元数长度
	UGdouble Length();
	//! \brief 四元数转为轴和角 
	void ToAngleAxis (UGdouble& rfAngle, UGVector3d& rkAxis);
	//! \brief 四元数点积 
	static UGdouble Dot(const UGQuaternion4d& a, const UGQuaternion4d& b);
	//! \brief 四元数转为欧拉角 
	static UGVector3d QuaternionToEuler(const UGQuaternion4d& q);
	//! \brief 欧拉角转为四元数
	static UGQuaternion4d EulerToQuaternion(UGdouble yaw, UGdouble pitch, UGdouble roll);

	 /** 两个四元数之间球面线性插值(Spherical linear interpolation), 返回插值结果.
			Slerp (A, B,0.0 ) = A
			Slerp (A, B, 1.0 ) = B
			@return 插值四元数
			@remarks
			四元数可以实现球面均匀插值, 并且可以实现最短路径插值 (除非 bShortestPath=FALSE).
			但是四元数插值不满足交换律 即：
			Slerp ( 0.75f, A, B ) != Slerp ( 0.25f, B, A );
			因此注意你的代码依赖于A,B的顺序，这在动画中尤为重要
			add by 单士刚 2015-01-30
		*/	
	static UGQuaternion4d Slerp(const UGQuaternion4d& q0, const UGQuaternion4d& q1, UGdouble t, UGbool bShortestPath = TRUE);
	//! \brief 四元数线性插值
	static UGQuaternion4d Nlerp(const UGQuaternion4d& rkP,const UGQuaternion4d& rkQ,UGdouble fT, UGbool shortestPath);

	// 四元数球面样条插值 sphercial and quadrangle
	static UGQuaternion4d Squad (const UGQuaternion4d& rkP,const UGQuaternion4d& rkQ, 
		const UGQuaternion4d& rkA,	const UGQuaternion4d& rkB, UGdouble t);


	//! \brief 从轴角转为四元数
	void FromAngleAxis (const UGdouble& rfAngle,
								const UGVector3d& rkAxis);
	//! \brief 由四元数专为旋转矩阵
	void ToRotationMatrix (UGMatrix3d& kRot) const;
	//! \brief 由四元数专为旋转矩阵
	void ToRotationMatrix (UGMatrix4d& kRot) const;	

	//! \brief 求四元数的逆变换
	UGQuaternion4d Inverse () const;

	//! \brief 四元数的指数
	UGQuaternion4d Exp () const;
	//! \brief 四元数的对数
	UGQuaternion4d Log () const;

	//! \brief 求四元数的单元逆变换，即为(w,-x,-y,-z)
	UGQuaternion4d UnitInverse () const;

	//! \brief 重载的等值判断运算符
	UGbool operator== (const UGQuaternion4d& rhs) const;
	//! \brief 重载的相乘运算符
	UGQuaternion4d operator* (const UGQuaternion4d& rkQ) const;
	//! \brief 四元数成员  
	UGdouble x;
	//! \brief 四元数成员
	UGdouble y;
	//! \brief 四元数成员
	UGdouble z;
	//! \brief 四元数x成员
	UGdouble w;



	static const UGQuaternion4d ZERO;
    static const UGQuaternion4d IDENTITY;	
	static const UGdouble msEpsilon; /// Cutoff for sine near zero

};
}
#endif // !defined(AFX_UGQUATERNION4D_H__828A96C2_95EA_4DCC_9468_666159FA635E__INCLUDED_)

