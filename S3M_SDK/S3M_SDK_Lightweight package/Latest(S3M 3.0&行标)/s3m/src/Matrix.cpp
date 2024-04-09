#include "Matrix.h"
#include "S3MBCommon.h"
#include <assert.h>
#include "Vector.h"
#include <math.h>

namespace S3MB
{
	Matrix3d::Matrix3d()
	{
		// 暂初始化为单位阵
		*this = Matrix3d::IDENTITY;
	}

	Matrix3d::Matrix3d(
		double m00, double m01, double m02,
		double m10, double m11, double m12,
		double m20, double m21, double m22)
	{
		m[0][0] = m00;
		m[0][1] = m01;
		m[0][2] = m02;
		m[1][0] = m10;
		m[1][1] = m11;
		m[1][2] = m12;
		m[2][0] = m20;
		m[2][1] = m21;
		m[2][2] = m22;
	}

	const Matrix3d Matrix3d::ZERO(
		0, 0, 0,
		0, 0, 0,
		0, 0, 0);

	const Matrix3d Matrix3d::IDENTITY(
		1, 0, 0,
		0, 1, 0,
		0, 0, 1);

	double* Matrix3d::operator[] (int iRow) const
	{
		return (double*)m[iRow];
	}

	Matrix3d Matrix3d::operator* (const Matrix3d& matrix) const
	{
		Matrix3d prod;
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				prod.m[i][j] =
					m[i][0] * matrix.m[0][j] +
					m[i][1] * matrix.m[1][j] +
					m[i][2] * matrix.m[2][j];
			}
		}
		return prod;
	}

	Vector3d Matrix3d::GetColumn(int iCol) const
	{
		assert(0 <= iCol && iCol < 3);
		return Vector3d(m[0][iCol], m[1][iCol],
			m[2][iCol]);
	}

	void Matrix3d::SetColumn(int iCol, const Vector3d& vec)
	{
		assert(0 <= iCol && iCol < 3);
		m[0][iCol] = vec.x;
		m[1][iCol] = vec.y;
		m[2][iCol] = vec.z;
	}

	void Matrix3d::FromAxes(const Vector3d& xVAxis, const Vector3d& yVAxis, const Vector3d& zVAxis)
	{
		SetColumn(2, zVAxis);
		SetColumn(1, yVAxis);
		SetColumn(0, xVAxis);
	}

	void Matrix3d::EigenDecomposition(Matrix3d& matUnitary, Matrix3d& matDiagonal, double dTolerance) const
	{
		int nMaxSweeps = 10;
		int nCount = 0;
		int nSweep = 0;

		matUnitary = Matrix3d::IDENTITY;
		matDiagonal = *this;

		double dEpsilon = dTolerance * matDiagonal.FrobeniusNorm();
		Matrix3d mat, matTranspose;
		while (nSweep < nMaxSweeps && matDiagonal.OffDiagonalFrobeniusNorm() > dEpsilon)
		{
			matDiagonal.SchurDecomposition(mat, 1e-15);
			matTranspose = mat.Transpose();
			matDiagonal = matDiagonal * mat;
			matDiagonal = matTranspose * matDiagonal;
			matUnitary = matUnitary * mat;

			if (++nCount > 2)
			{
				++nSweep;
				nCount = 0;
			}
		}
	}

	Matrix3d Matrix3d::Transpose() const
	{
		Matrix3d transpose;
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
				transpose[i][j] = m[j][i];
		}
		return transpose;
	}

	double Matrix3d::FrobeniusNorm() const
	{
		double dNorm = 0.0;
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				dNorm += m[i][j] * m[i][j];
			}
		}
		return sqrt(dNorm);
	}

	double Matrix3d::OffDiagonalFrobeniusNorm() const
	{
		double dNorm = 0.0;
		int rowVal[3] = { 1, 0, 0 };
		int colVal[3] = { 2, 2, 1 };
		int nRow = 0, nCol = 0;
		for (int i = 0; i < 3; i++)
		{
			nRow = rowVal[i];
			nCol = colVal[i];
			dNorm += 2.0 * m[nRow][nCol] * m[nRow][nCol];
		}
		return sqrt(dNorm);
	}

	void Matrix3d::SchurDecomposition(Matrix3d& matResult, double dTolerance)
	{
		double dMaxDiagonal = 0.0, dTemp = 0.0;
		int nRotAxis = 1;
		int rowVal[3] = { 1, 0, 0 };
		int colVal[3] = { 2, 2, 1 };
		int nRow = 0, nCol = 0;

		for (int i = 0; i < 3; i++)
		{
			nRow = rowVal[i];
			nCol = colVal[i];
            dTemp = fabs(m[nRow][nCol]);
			if (dTemp > dMaxDiagonal)
			{
				nRotAxis = i;
				dMaxDiagonal = dTemp;
			}
		}

		double dC = 1.0;
		double dS = 0.0;

		int nP = rowVal[nRotAxis];
		int nQ = colVal[nRotAxis];

        if (fabs(m[nP][nQ]) > dTolerance)
		{
			double dQQ = m[nQ][nQ];
			double dPP = m[nP][nP];
			double dPQ = m[nP][nQ];

			double dTau = (dQQ - dPP) / 2.0 / dPQ;
			double dT = 0.0;

			if (dTau < 0.0)
			{
				dT = -1.0 / (-dTau + sqrt(1.0 + dTau * dTau));
			}
			else
			{
				dT = 1.0 / (dTau + sqrt(1.0 + dTau * dTau));
			}

			dC = 1.0 / sqrt(1.0 + dT * dT);
			dS = dT * dC;
		}

		matResult = Matrix3d::IDENTITY;
		matResult[nQ][nQ] = dC;
		matResult[nP][nP] = dC;
		matResult[nP][nQ] = dS;
		matResult[nQ][nP] = -dS;
	}

	Matrix4d::Matrix4d()
	{
		*this = Matrix4d::IDENTITY;
	}

	Matrix4d::Matrix4d(double m00, double m01, double m02, double m03, double m10, double m11, double m12, double m13, double m20, double m21, double m22, double m23, double m30, double m31, double m32, double m33)
	{
		m[0][0] = m00;
		m[0][1] = m01;
		m[0][2] = m02;
		m[0][3] = m03;
		m[1][0] = m10;
		m[1][1] = m11;
		m[1][2] = m12;
		m[1][3] = m13;
		m[2][0] = m20;
		m[2][1] = m21;
		m[2][2] = m22;
		m[2][3] = m23;
		m[3][0] = m30;
		m[3][1] = m31;
		m[3][2] = m32;
		m[3][3] = m33;
	}

	const Matrix4d Matrix4d::ZERO(
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0);

	const Matrix4d Matrix4d::IDENTITY(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);

	Matrix4d Matrix4d::operator*(double scalar)
	{
		return Matrix4d(
			scalar*m[0][0], scalar*m[0][1], scalar*m[0][2], scalar*m[0][3],
			scalar*m[1][0], scalar*m[1][1], scalar*m[1][2], scalar*m[1][3],
			scalar*m[2][0], scalar*m[2][1], scalar*m[2][2], scalar*m[2][3],
			scalar*m[3][0], scalar*m[3][1], scalar*m[3][2], scalar*m[3][3]);
	}

	Matrix4d Matrix4d::operator*(const Matrix4d& m2) const
	{
		return Concatenate(m2);
	}

	Matrix4d Matrix4d::Concatenate(const Matrix4d& multiplyM2) const
	{
		Matrix4d tempMatrix;
		tempMatrix.m[0][0] = m[0][0] * multiplyM2.m[0][0] + m[0][1] * multiplyM2.m[1][0] + m[0][2] * multiplyM2.m[2][0] + m[0][3] * multiplyM2.m[3][0];
		tempMatrix.m[0][1] = m[0][0] * multiplyM2.m[0][1] + m[0][1] * multiplyM2.m[1][1] + m[0][2] * multiplyM2.m[2][1] + m[0][3] * multiplyM2.m[3][1];
		tempMatrix.m[0][2] = m[0][0] * multiplyM2.m[0][2] + m[0][1] * multiplyM2.m[1][2] + m[0][2] * multiplyM2.m[2][2] + m[0][3] * multiplyM2.m[3][2];
		tempMatrix.m[0][3] = m[0][0] * multiplyM2.m[0][3] + m[0][1] * multiplyM2.m[1][3] + m[0][2] * multiplyM2.m[2][3] + m[0][3] * multiplyM2.m[3][3];

		tempMatrix.m[1][0] = m[1][0] * multiplyM2.m[0][0] + m[1][1] * multiplyM2.m[1][0] + m[1][2] * multiplyM2.m[2][0] + m[1][3] * multiplyM2.m[3][0];
		tempMatrix.m[1][1] = m[1][0] * multiplyM2.m[0][1] + m[1][1] * multiplyM2.m[1][1] + m[1][2] * multiplyM2.m[2][1] + m[1][3] * multiplyM2.m[3][1];
		tempMatrix.m[1][2] = m[1][0] * multiplyM2.m[0][2] + m[1][1] * multiplyM2.m[1][2] + m[1][2] * multiplyM2.m[2][2] + m[1][3] * multiplyM2.m[3][2];
		tempMatrix.m[1][3] = m[1][0] * multiplyM2.m[0][3] + m[1][1] * multiplyM2.m[1][3] + m[1][2] * multiplyM2.m[2][3] + m[1][3] * multiplyM2.m[3][3];

		tempMatrix.m[2][0] = m[2][0] * multiplyM2.m[0][0] + m[2][1] * multiplyM2.m[1][0] + m[2][2] * multiplyM2.m[2][0] + m[2][3] * multiplyM2.m[3][0];
		tempMatrix.m[2][1] = m[2][0] * multiplyM2.m[0][1] + m[2][1] * multiplyM2.m[1][1] + m[2][2] * multiplyM2.m[2][1] + m[2][3] * multiplyM2.m[3][1];
		tempMatrix.m[2][2] = m[2][0] * multiplyM2.m[0][2] + m[2][1] * multiplyM2.m[1][2] + m[2][2] * multiplyM2.m[2][2] + m[2][3] * multiplyM2.m[3][2];
		tempMatrix.m[2][3] = m[2][0] * multiplyM2.m[0][3] + m[2][1] * multiplyM2.m[1][3] + m[2][2] * multiplyM2.m[2][3] + m[2][3] * multiplyM2.m[3][3];

		tempMatrix.m[3][0] = m[3][0] * multiplyM2.m[0][0] + m[3][1] * multiplyM2.m[1][0] + m[3][2] * multiplyM2.m[2][0] + m[3][3] * multiplyM2.m[3][0];
		tempMatrix.m[3][1] = m[3][0] * multiplyM2.m[0][1] + m[3][1] * multiplyM2.m[1][1] + m[3][2] * multiplyM2.m[2][1] + m[3][3] * multiplyM2.m[3][1];
		tempMatrix.m[3][2] = m[3][0] * multiplyM2.m[0][2] + m[3][1] * multiplyM2.m[1][2] + m[3][2] * multiplyM2.m[2][2] + m[3][3] * multiplyM2.m[3][2];
		tempMatrix.m[3][3] = m[3][0] * multiplyM2.m[0][3] + m[3][1] * multiplyM2.m[1][3] + m[3][2] * multiplyM2.m[2][3] + m[3][3] * multiplyM2.m[3][3];

		return tempMatrix;
	}

	// 重载[]，取出对应行。
	// iRow 行号，有效范围为零到三[in]。
	// return 矩阵指定一行数据，返回行数据指针。
	double* Matrix4d::operator[] (unsigned int iRow)
	{
		assert(iRow < 4);
		return (double*)m[iRow];
	}

	// 取出只读矩阵行数据。
	// iRow 行号，有效范围为零到三[in]。
	const double* const Matrix4d::operator [] (unsigned int iRow) const
	{
		assert(iRow < 4);
		return m[iRow];
	}

	bool Matrix4d::operator!=(const Matrix4d& m2) const
	{
		return
			!(
				IS0(m[0][0] - m2.m[0][0]) &&
				IS0(m[0][1] - m2.m[0][1]) &&
				IS0(m[0][2] - m2.m[0][2]) &&
				IS0(m[0][3] - m2.m[0][3]) &&
				IS0(m[1][0] - m2.m[1][0]) &&
				IS0(m[1][1] - m2.m[1][1]) &&
				IS0(m[1][2] - m2.m[1][2]) &&
				IS0(m[1][3] - m2.m[1][3]) &&
				IS0(m[2][0] - m2.m[2][0]) &&
				IS0(m[2][1] - m2.m[2][1]) &&
				IS0(m[2][2] - m2.m[2][2]) &&
				IS0(m[2][3] - m2.m[2][3]) &&
				IS0(m[3][0] - m2.m[3][0]) &&
				IS0(m[3][1] - m2.m[3][1]) &&
				IS0(m[3][2] - m2.m[3][2]) &&
				IS0(m[3][3] - m2.m[3][3])
				);
	}

	bool Matrix4d::operator==(const Matrix4d& m2) const
	{
		return
			(
				IS0(m[0][0] - m2.m[0][0]) &&
				IS0(m[0][1] - m2.m[0][1]) &&
				IS0(m[0][2] - m2.m[0][2]) &&
				IS0(m[0][3] - m2.m[0][3]) &&
				IS0(m[1][0] - m2.m[1][0]) &&
				IS0(m[1][1] - m2.m[1][1]) &&
				IS0(m[1][2] - m2.m[1][2]) &&
				IS0(m[1][3] - m2.m[1][3]) &&
				IS0(m[2][0] - m2.m[2][0]) &&
				IS0(m[2][1] - m2.m[2][1]) &&
				IS0(m[2][2] - m2.m[2][2]) &&
				IS0(m[2][3] - m2.m[2][3]) &&
				IS0(m[3][0] - m2.m[3][0]) &&
				IS0(m[3][1] - m2.m[3][1]) &&
				IS0(m[3][2] - m2.m[3][2]) &&
				IS0(m[3][3] - m2.m[3][3])
				);
	}

	Matrix4d Matrix4d::Adjoint() const
	{
		// 计算矩阵的伴随矩阵
		return Matrix4d(
			// 计算4x4中任意3x3子矩阵行列式	
			MINOR(*this, 1, 2, 3, 1, 2, 3), -MINOR(*this, 0, 2, 3, 1, 2, 3), MINOR(*this, 0, 1, 3, 1, 2, 3), -MINOR(*this, 0, 1, 2, 1, 2, 3),
			// 计算4x4中任意3x3子矩阵行列式	
			-MINOR(*this, 1, 2, 3, 0, 2, 3), MINOR(*this, 0, 2, 3, 0, 2, 3), -MINOR(*this, 0, 1, 3, 0, 2, 3), MINOR(*this, 0, 1, 2, 0, 2, 3),
			// 计算4x4中任意3x3子矩阵行列式		
			MINOR(*this, 1, 2, 3, 0, 1, 3), -MINOR(*this, 0, 2, 3, 0, 1, 3), MINOR(*this, 0, 1, 3, 0, 1, 3), -MINOR(*this, 0, 1, 2, 0, 1, 3),
			// 计算4x4中任意3x3子矩阵行列式		
			-MINOR(*this, 1, 2, 3, 0, 1, 2), MINOR(*this, 0, 2, 3, 0, 1, 2), -MINOR(*this, 0, 1, 3, 0, 1, 2), MINOR(*this, 0, 1, 2, 0, 1, 2));
	}

	double Matrix4d::Determinant() const
	{
		return m[0][0] * MINOR(*this, 1, 2, 3, 1, 2, 3) - m[0][1] * MINOR(*this, 1, 2, 3, 0, 2, 3) + m[0][2] * MINOR(*this, 1, 2, 3, 0, 1, 3) - m[0][3] * MINOR(*this, 1, 2, 3, 0, 1, 2);
	}

	Matrix4d Matrix4d::Invert() const
	{
		return Adjoint() * (1.0f / Determinant());
	}

	double Matrix4d::MINOR(const Matrix4d& m, const int r0, const int r1, const int r2,
		const int c0, const int c1, const int c2) const
	{
		return m[r0][c0] * (m[r1][c1] * m[r2][c2] - m[r2][c1] * m[r1][c2]) -
			m[r0][c1] * (m[r1][c0] * m[r2][c2] - m[r2][c0] * m[r1][c2]) +
			m[r0][c2] * (m[r1][c0] * m[r2][c1] - m[r2][c0] * m[r1][c1]);
	}

	void Matrix4d::SetTrans(const Vector3d& v)
	{
		m[3][0] = v.x;
		m[3][1] = v.y;
		m[3][2] = v.z;
	}

	Vector3d Matrix4d::GetTrans() const
	{
		return Vector3d(m[3][0], m[3][1], m[3][2]);
	}

	double* Matrix4d::GetPointer()
	{ 
		return (double*)&m;
	}
}
