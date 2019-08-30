/* 
	\file UGMatrix4d.h
	\brief 定义 4 X 4 矩阵类。
	\author malq
	\attention
	Copyright(c) 2000-2010  SuperMap GIS Technologies,Inc.<br>
	All Rights Reserved
	<b>更新说明：。</b>
	<em>1、第一条说明。</em>
	<em>2、第二条说明。</em>
*/

#if !defined(AFX_UGMATRIX4D_H__67A327BB_7D71_4996_8DBC_4884AB212BCD__INCLUDED_)
#define AFX_UGMATRIX4D_H__67A327BB_7D71_4996_8DBC_4884AB212BCD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "Base3D/UGPrerequisites3D.h"
#include "Base3D/UGMatrix3d.h"

namespace  UGC 
{

//! \brief   4X4矩阵类
//! \remarks 维护4X4矩阵
class BASE3D_API UGMatrix4d  
{

public:

	//! \brief 缺省构造函数。
	UGMatrix4d();

	//! \brief 用16个浮点数构造矩阵类的构造函数。
	//! \param m00 [in]。
	//! \param m01 [in]。
	//! \param m02 [in]。
	//! \param m03 [in]。
	//! \param m10 [in]。
	//! \param m11 [in]。
	//! \param m12 [in]。
	//! \param m13 [in]。
	//! \param m20 [in]。
	//! \param m21 [in]。
	//! \param m22 [in]。
	//! \param m23 [in]。
	//! \param m30 [in]。
	//! \param m31 [in]。
	//! \param m32 [in]。
	//! \param m33 [in]。
	UGMatrix4d(
            UGdouble m00, UGdouble m01, UGdouble m02, UGdouble m03,
            UGdouble m10, UGdouble m11, UGdouble m12, UGdouble m13,
            UGdouble m20, UGdouble m21, UGdouble m22, UGdouble m23,
            UGdouble m30, UGdouble m31, UGdouble m32, UGdouble m33);

	//! \brief 用三阶矩阵和平移参数构造四阶矩阵。
	UGMatrix4d(const UGMatrix3d& mat3, UGdouble x = 0.0, UGdouble y = 0.0, UGdouble z = 0.0);

	//! \brief 析构函数。
	virtual ~UGMatrix4d();

	//! \brief 重载*，矩阵与数值相乘。
	//! \param scalar 数值[in]。
	//! \return 运算完成后4x4矩阵。
	UGMatrix4d operator*(UGdouble scalar);

	//! \brief 重载*，完成矩阵与矩阵相乘。
	//! \param &m2 乘数矩阵[in]。
	//! \return 运算完成后矩阵。
	UGMatrix4d operator * ( const UGMatrix4d &m2 ) const;

	//! \brief 重载=，完成矩阵的赋值
	//! \brief &m2,赋值矩阵	
	UGMatrix4d& operator = (const UGMatrix4d &m2);

	//! \brief 重载*，矩阵与三维向量相乘。
	//! \param &v 三维向量[in]。
	//! \return 三维向量。
	//! \remarks 从矩阵中取出最上面3X3矩阵与三维向量进行相乘。
	UGVector3d operator * ( const UGVector3d &v ) const;

	//! \brief 矩阵相乘。
	//! \param &m2 另外一个4X4矩阵[in]。
	//! \return 完成运算后4X4矩阵。
	UGMatrix4d Concatenate(const UGMatrix4d &m2) const;
    
	//! \brief 重载[]，取出对应行。
	//! \param iRow 行号，有效范围为零到三[in]。
	//! \return 矩阵指定一行数据，返回行数据指针。
	UGdouble* operator[] (UGuint iRow);

	//! \brief 判断两个矩阵是否相等。
	//! \param &m2 另外一个矩阵[in]。
	//! \return 若相等返回为TRUE，若不相等则返回为FALSE。
	UGbool operator == (const UGMatrix4d &m2) const;

	//! \brief 判断两个矩阵是否不相等。
	//! \param &m2 另外一个矩阵[in]。
	//! \return 若不相等返回为TRUE，否则返回FALSE。
	UGbool operator != (const UGMatrix4d &m2) const;
 
	//! \brief 取出只读矩阵行数据。
	//! \param iRow 行号，有效范围为零到三[in]。
	//! \return 行数据。
	const UGdouble *const operator [] ( UGuint iRow ) const;

    //! \brief 取出矩阵某行某列的值。
	//! \param iRow 行数[in]。
	//! \param iCol 列数[in]
	UGdouble operator () (UGuint iRow, UGuint iCol);	
	//! \brief 矩阵平移。
	//! \param x X轴移动大小[in]。
	//! \param y Y轴移动大小[in]。
	//! \param z Z轴移动大小[in]。
	//! \return 平移后矩阵。
	static UGMatrix4d Translation(UGdouble x, UGdouble y, UGdouble z);

	//! \brief 构建一个缩放矩阵
	//! param x X轴缩放大小[in]。
	//! param y Y轴缩放大小[in]。
	//! param z Z轴缩放大小[in]。
	//! return 缩放矩阵。
	static UGMatrix4d Scaling(UGdouble x, UGdouble y, UGdouble z);

	//! \brief 旋转。
	//! \param rotationX 绕X轴旋转角度[in]。
	//! \param rotationY 绕Y轴旋转角度[in]。
	//! \param rotationZ 绕Z轴旋转角度[in]。
	//! \return 旋转后矩阵。
	static UGMatrix4d RotationXYZ(UGdouble rotationX, UGdouble rotationY, UGdouble rotationZ);
	
	//! \brief 绕X轴旋转。
	//! \param angle 绕X轴旋转角度[in]。
	//! \return 旋转后矩阵。		
	static UGMatrix4d RotationX(UGdouble angle);

	//! \brief 绕Y轴旋转。
	//! \param angle 绕Y轴旋转角度[in]。
	//! \return 旋转后矩阵。
	static UGMatrix4d RotationY(UGdouble angle);

	//! \brief 绕Z轴旋转。
	//! \param angle [in]。
	//! \return 旋转后矩阵。		
	static UGMatrix4d RotationZ(UGdouble angle);

	//! \brief 计算矩阵的伴随矩阵。
	//! \return 矩阵的伴随矩阵。
	//! \remarks 。	
	UGMatrix4d Adjoint() const;

	//! \brief 计算矩阵行列式值。
	//! \return 行列式值。。
	UGdouble Determinant() const;

	//! \brief 计算机矩阵逆矩阵。
	//! \return 矩阵逆矩阵。
	UGMatrix4d Invert() const;
	
	//! \brief 计算机矩阵转置矩阵。
	//! \return 矩阵转置矩阵
	UGMatrix4d Transpose() const;
	
	//! \brief 计算4x4中任意3x3子矩阵行列式。
	//! \param m 4x4矩阵[in]。
	//! \param r0 第一行行号[in]。
	//! \param r1 第二行行号[in]。
	//! \param r2 第三行行号[in]。
	//! \param c0 第一列列号[in]。
	//! \param c1 第二列列号[in]。
	//! \param c2 第三列列号[in]。
	UGdouble MINOR(const UGMatrix4d& m, const UGint r0, const UGint r1, const UGint r2, 
		const UGint c0, const UGint c1, const UGint c2) const;

	//! \brief 重载赋值运算符。
	void operator = ( const UGMatrix3d& mat3 );

	//! \brief 由三阶矩阵初始化。
	UGMatrix4d& Make(const UGMatrix3d& mat3);
	
	//! \brief 设置矩阵平移部分, [3][0], [3][1], [3][2]的值。
	//! \param v 平移向量[in]。
	void SetTrans( const UGVector3d& v);	
	
	//! \brief 获取矩阵平移部分, [3][0], [3][1], [3][2]的值。
	//! \param v 平移向量[in]。
	UGVector3d GetTrans() const;

	//! \brief 采用施密特正交方法计算出正交矩阵
	void Orthorize();
	
	//! \brief 4X4零矩阵
	//! \remarks 所有元素为零
	static const UGMatrix4d ZERO;
	//! \brief 4X4 单位矩阵
	//! \remarks 对角线元素为1，其他为0
	static const UGMatrix4d IDENTITY;

	static const UGMatrix4d CLIPSPACE2DTOIMAGESPACE;

	//! \brief生成D3D旋转矩阵
	//! \param angle 角度
	//! \param vecRotateAxis 单位化旋转轴
	static UGMatrix4d ComputeRotateMatrix(UGdouble angle, UGVector3d vecRotateAxis);
	
	//! \brief   保存为XML时用到
	//! \return  以字符串的形式返回矩阵信息
	UGString GetString()const;
	
	//! \brief 读取XML信息时用于解析矩阵
	//! \remarks strXML [in] 字符串的形式矩阵信息
	void ParserString(const UGString& strXML);

	//! \brief 向量与矩阵相乘
	//! \param matrix[16] [in]输入矩阵
	//! \param in[4] [in] 输入向量
	//! \param out[4] [out] 输出向量
	static void MultMatrixVecd(const UGdouble matrix[16], const UGdouble in[4],
									UGdouble out[4]);

	//! \brief 两个矩阵相乘
	//! \param a[16] [in] 输入矩阵
	//! \param b[16] [in] 输入矩阵
	//! \param r[16] [out] 输出结果矩阵
	static void MultMatricesd(const UGdouble a[16], const UGdouble b[16],
								   UGdouble r[16]);
	//! \brief 计算机矩阵逆矩阵
	//! \param m[16]　[in] 输入矩阵
	//! \param invOut[16] [out] 输出矩阵
	static UGint InvertMatrixd(const UGdouble m[16], UGdouble invOut[16]);

	//! \brief 获取矩阵中的数据
	void GetMatrixData(UGdouble data[4][4]) const;

	//! \brief 获取矩阵的指针
	UGdouble* GetPointer();

	//! \brief 检测矩阵是否是仿射矩阵
	//! \remarks 仿射矩阵的第三列为(0,0,0,1) 
	UGbool IsAffine() const;

	//!\brief  解析  旋转 / 缩放部分到一个3x3 的矩阵.      
	void Extract3x3Matrix(UGMatrix3d& m3x3) const;

	//! \brief 解析 x、y、z分量
	void ExtractXYZValue(UGdouble& dX, UGdouble& dY, UGdouble& dZ) const;

public:
	//! \从XML字符串读取参数设置
	//! \param strXML XML字符串  [in]
	//! \return 返回是否成功
	UGbool FromXML(UGString& strXML);

	//! \保存参数设置到XML字符串
	//! \param strXML XML字符串  [in]
	//! \return 返回是否成功
	UGbool ToXML(UGString& strXML);
protected:

	//! \brief 矩阵中数据
	//! \remarks 矩阵中数据保存在该变量中。
	UGdouble m[4][4];
};


}
#endif // !defined(AFX_UGMATRIX4D_H__67A327BB_7D71_4996_8DBC_4884AB212BCD__INCLUDED_)

