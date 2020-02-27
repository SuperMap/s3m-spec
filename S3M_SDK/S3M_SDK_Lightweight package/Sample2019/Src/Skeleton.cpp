#include "Skeleton.h"
#include "zlib\include\zconf.h"
#include <assert.h>


double Vector3d::operator[](const unsigned i) const
{
	assert(i < 3);
	return *(&x + i);
}

double& Vector3d::operator[](const unsigned i)
{
	assert(i < 3);
	return *(&x + i);
}

Vector3d Vector3d::operator+(const Vector3d& vec) const
{
	Vector3d TempVec;
	TempVec.x = x + vec.x;
	TempVec.y = y + vec.y;
	TempVec.z = z + vec.z;
	return TempVec;
}

Vector3d Vector3d::operator-(const Vector3d& vec) const
{
	Vector3d TempVec;
	TempVec.x = x - vec.x;
	TempVec.y = y - vec.y;
	TempVec.z = z - vec.z;
	return TempVec;
}

double Vector3d::Length() const
{
	return sqrt(x*x + y * y + z * z);
}

double Vector3d::Length2() const
{
	return x*x + y * y + z * z;
}

void Vector3d::MakeFloor(const Vector3d& cmp)
{
	if (cmp.x < x) x = cmp.x;
	if (cmp.y < y) y = cmp.y;
	if (cmp.z < z) z = cmp.z;
}

void Vector3d::MakeCeil(const Vector3d& cmp)
{
	if (cmp.x > x) x = cmp.x;
	if (cmp.y > y) y = cmp.y;
	if (cmp.z > z) z = cmp.z;
}

Vector3d Vector3d::MultiplyMatrix(const Matrix4d& m) const
{
	double w = 1;
	double m11 = 0, m12 = 0, m13 = 0, m14 = 0;

	m11 = x * m[0][0] + y * m[1][0] + z * m[2][0] + w * m[3][0];
	m12 = x * m[0][1] + y * m[1][1] + z * m[2][1] + w * m[3][1];
	m13 = x * m[0][2] + y * m[1][2] + z * m[2][2] + w * m[3][2];
	m14 = x * m[0][3] + y * m[1][3] + z * m[2][3] + w * m[3][3];

	return Vector3d(m11, m12, m13);
}

void Vector3d::Normalize()
{
	double fLength = 0.0;
	fLength = sqrt(x*x + y * y + z * z);
	if (!IS0(fLength))
	{
		x /= fLength;
		y /= fLength;
		z /= fLength;
	}
}

double Vector4d::operator[](const unsigned i) const
{
	assert(i < 4);
	return *(&x + i);
}

double& Vector4d::operator[](const unsigned i)
{
	assert(i < 4);
	return *(&x + i);
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


Matrix4d Matrix4d::operator*(const Matrix4d &m2) const
{
	return Concatenate(m2);
}

Matrix4d Matrix4d::Concatenate(const Matrix4d &multiplyM2) const
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

//! \brief 重载[]，取出对应行。
//! \param iRow 行号，有效范围为零到三[in]。
//! \return 矩阵指定一行数据，返回行数据指针。
double* Matrix4d::operator[] (unsigned int iRow)
{
	assert(iRow < 4);
	return (double*)m[iRow];
}

//! \brief 取出只读矩阵行数据。
//! \param iRow 行号，有效范围为零到三[in]。
//! \return 行数据。
const double *const Matrix4d::operator [] (unsigned int iRow) const
{
	assert(iRow < 4);
	return m[iRow];
}

bool Matrix4d::operator!=(const Matrix4d &m2) const
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

bool Matrix4d::operator==(const Matrix4d &m2) const
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

BoundingBox::BoundingBox()
{
	SetMin(-0.5, -0.5, -0.5);
	SetMax(0.5, 0.5, 0.5);
	m_bNull = true;
}

BoundingBox::BoundingBox(Vector3d vMin, Vector3d vMax)
{
	SetExtents(vMin, vMax);
}

bool BoundingBox::IsRealNaN(double dValue)
{
	if (dValue > DBLMAX || dValue < -DBLMAX || IS0(dValue - DBLMAX) || IS0(dValue - DBLMIN))
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool BoundingBox::IsVaild()
{
	return (IsRealNaN(m_vMax.x) && IsRealNaN(m_vMax.y) && IsRealNaN(m_vMax.z) && \
		IsRealNaN(m_vMin.x) && IsRealNaN(m_vMin.y) && IsRealNaN(m_vMin.z));
}

void BoundingBox::Merge(const BoundingBox& BoundBox)
{
	if (BoundBox.m_bNull)
		return;
	else if (m_bNull)
		SetExtents(BoundBox.m_vMin, BoundBox.m_vMax);
	else
	{
		Vector3d vMin = m_vMin;
		Vector3d vMax = m_vMax;
		vMax.MakeCeil(BoundBox.m_vMax);
		vMin.MakeFloor(BoundBox.m_vMin);
		SetExtents(vMin, vMax);
	}
}

void BoundingBox::SetMin(Vector3d vMin)
{
	m_bNull = false;
	m_vMin = vMin;
	UpdateCorners();
}

void BoundingBox::SetMin(double x, double y, double z)
{
	m_bNull = false;
	m_vMin.x = x;
	m_vMin.y = y;
	m_vMin.z = z;
	UpdateCorners();
}

void BoundingBox::SetMax(Vector3d vMax)
{
	m_bNull = false;
	m_vMax = vMax;
	UpdateCorners();
}

void BoundingBox::SetMax(double x, double y, double z)
{
	m_bNull = false;
	m_vMax.x = x;
	m_vMax.y = y;
	m_vMax.z = z;
	UpdateCorners();
}

void BoundingBox::SetExtents(const Vector3d& vMin, const Vector3d& vMax)
{
	m_bNull = false;
	m_vMin = vMin;
	m_vMax = vMax;
	UpdateCorners();
}

void BoundingBox::SetExtents(double minx, double miny, double minz, double maxx, double maxy, double maxz)
{
	m_bNull = false;
	m_vMin.x = minx;
	m_vMin.y = miny;
	m_vMin.z = minz;

	m_vMax.x = maxx;
	m_vMax.y = maxy;
	m_vMax.z = maxz;
	UpdateCorners();
}

const Vector3d& BoundingBox::GetMin(void) const
{
	return m_vMin;
}

const Vector3d& BoundingBox::GetMax(void) const
{
	return m_vMax;
}

Vector3d BoundingBox::GetCenter() const
{
	Vector3d center;
	center.x = (m_vMin.x + m_vMax.x) / 2.0;
	center.y = (m_vMin.y + m_vMax.y) / 2.0;
	center.z = (m_vMin.z + m_vMax.z) / 2.0;

	return center;
}

void BoundingBox::Transform(Matrix4d& matrix)
{
	if (m_bNull)
		return;
	bool bFirst = true;
	Vector3d vMax, vMin, vTemp;
	int i = 0;
	for (i = 0; i < 8; i++)
	{
		vTemp = m_vCorner[i].MultiplyMatrix(matrix);
		if (bFirst || vTemp.x > vMax.x)
			vMax.x = vTemp.x;
		if (bFirst || vTemp.y > vMax.y)
			vMax.y = vTemp.y;
		if (bFirst || vTemp.z > vMax.z)
			vMax.z = vTemp.z;
		if (bFirst || vTemp.x < vMin.x)
			vMin.x = vTemp.x;
		if (bFirst || vTemp.y < vMin.y)
			vMin.y = vTemp.y;
		if (bFirst || vTemp.z < vMin.z)
			vMin.z = vTemp.z;
		bFirst = false;
	}
	SetExtents(vMin, vMax);
}

void BoundingBox::UpdateCorners(void)
{
	for (int pos = 0; pos < 8; ++pos)
	{
		m_vCorner[pos] = Vector3d(pos & 1 ? m_vMax.x : m_vMin.x, pos & 2 ? m_vMax.y : m_vMin.y, pos & 4 ? m_vMax.z : m_vMin.z);
	}
	m_bNull = false;
}

IndexPackage::IndexPackage()
	: m_nIndexesCount(0)
	, m_pIndexes(NULL)
	, m_enIndexType(IT_16BIT)
	, m_bUseIndex(true)
	, m_OperationType(OT_TRIANGLE_LIST)
{
}

IndexPackage::~IndexPackage()
{
	if (m_pIndexes != NULL)
	{
		delete[] m_pIndexes;
		m_pIndexes = NULL;
	}
	m_nIndexesCount = 0;
}

bool IndexPackage::Load(MemoryStream &stream)
{
	unsigned char nByte = 0;
	int nCount = 0;
	stream >> nCount;
	stream >> (unsigned char&)m_enIndexType;
	stream >> m_bUseIndex;
	stream >> (unsigned char&)m_OperationType;
	stream >> nByte;

	if (nCount > 0)
	{
		m_nIndexesCount = nCount;
		if (m_enIndexType == IT_32BIT || m_enIndexType == IT_32BIT_2)
		{
			m_pIndexes = new(std::nothrow) unsigned short[m_nIndexesCount * 2];
			if (m_pIndexes == NULL)
			{
				return false;
			}
			stream.Load(m_pIndexes, m_nIndexesCount * 2);
		}
		else
		{
			m_pIndexes = new(std::nothrow) unsigned short[m_nIndexesCount];
			if (m_pIndexes == NULL)
			{
				return false;
			}
			stream.Load(m_pIndexes, m_nIndexesCount);
			if (m_nIndexesCount % 2 != 0)
			{
				stream >> nByte;
				stream >> nByte;
			}
		}
	}

	int i = 0;
	stream >> nCount;

	for (i = 0; i < nCount; i++)
	{
		string strName;
		stream >> strName;
		m_strPassName.push_back(strName);
	}
	//四字节对齐
	int nPos = stream.GetReadPosition();
	if (nPos % 4 != 0)
	{
		int nReserved = 4 - nPos % 4;
		for (int j = 0; j < nReserved; j++)
		{
			stream >> nByte;
		}
	}
	return true;
}

void IndexPackage::Save(MemoryStream& stream)
{
	unsigned char nByte = 0;
	stream << m_nIndexesCount;

	stream << (unsigned char)m_enIndexType;
	stream << m_bUseIndex;
	stream << (unsigned char)m_OperationType;
	stream << nByte;

	if (m_nIndexesCount > 0 && m_pIndexes != NULL)
	{
		if (m_enIndexType == IT_32BIT || m_enIndexType == IT_32BIT_2)
		{
			stream.Save(m_pIndexes, m_nIndexesCount * 2);
		}
		else
		{
			stream.Save(m_pIndexes, m_nIndexesCount);
			if (m_nIndexesCount % 2 != 0)
			{
				stream << nByte;
				stream << nByte;
			}
		}
	}

	int nCount = m_strPassName.size();
	stream << nCount;
	for (int i = 0; i < nCount; i++)
	{
		string strName = m_strPassName[i];
		stream << strName;
	}
	//四字节对齐
	int nPos = stream.GetWritePosition();
	if (nPos % 4 != 0)
	{
		int nReserved = 4 - nPos % 4;
		for (int j = 0; j < nReserved; j++)
		{
			stream << nByte;
		}
	}
}

void IndexPackage::SetIndexNum(int nIndexNum)
{
	m_nIndexesCount = nIndexNum;
	if (m_pIndexes != NULL)
	{
		delete[] m_pIndexes;
		m_pIndexes = NULL;
	}
	if (m_enIndexType == IT_16BIT ||
		m_enIndexType == IT_16BIT_2)
	{
		m_pIndexes = new unsigned short[nIndexNum];
		memset(m_pIndexes, 0, sizeof(unsigned short)*nIndexNum);
	}
	else
	{
		m_pIndexes = (unsigned short*)new unsigned int[nIndexNum];
		memset(m_pIndexes, 0, sizeof(unsigned int)*nIndexNum);
	}
}

Geometry::Geometry() :
	m_pVertexDataPackage(NULL),
	m_strMaterialName(""),
	m_strGeoName(""),
	m_matWorldView(Matrix4d::IDENTITY),
	m_nUseIndex(0),
	m_bInstanceBatch(false),
	m_bNormalDecode(false) {}

Geometry::~Geometry()
{
	if (m_pVertexDataPackage != NULL)
	{
		delete m_pVertexDataPackage;
		m_pVertexDataPackage = NULL;
	}
	for (int i = 0; i < m_arrIndexPackage.size(); i++)
	{
		if (m_arrIndexPackage.at(i) != NULL)
		{
			delete m_arrIndexPackage.at(i);
			m_arrIndexPackage.at(i) = NULL;
		}
	}
}

void Geometry::ComputerBoundingBox()
{
	if (m_pVertexDataPackage == NULL || m_pVertexDataPackage->m_nVerticesCount < 1)
	{
		return;
	}

	Vector3d vecMax, vecMin;
	vecMax.x = -DBLMAX;
	vecMax.y = -DBLMAX;
	vecMax.z = -DBLMAX;
	vecMin.x = DBLMAX;
	vecMin.y = DBLMAX;
	vecMin.z = DBLMAX;

	int nVertexDim = m_pVertexDataPackage->m_nVertexDimension;
	int nValidVertexDim = nVertexDim > 3 ? 3 : nVertexDim;
	short* pCompressVertex = (short*)m_pVertexDataPackage->m_pVertices;
	for (int i = 0; i < m_pVertexDataPackage->m_nVerticesCount; i++)
	{
		Vector3d vertChange;
		for (int j = 0; j < nValidVertexDim; j++)
		{
			if (m_pVertexDataPackage->m_nCompressOptions & SVC_Vertex)
			{
				short encodeVal = pCompressVertex[i * nVertexDim + j];
				vertChange[j] = encodeVal * m_pVertexDataPackage->m_fVertCompressConstant + m_pVertexDataPackage->m_minVerticesValue[j];
			}
			else
			{
				vertChange[j] = m_pVertexDataPackage->m_pVertices[i * nVertexDim + j];
			}
		}

		if (vertChange.x > vecMax.x) vecMax.x = vertChange.x;
		if (vertChange.y > vecMax.y) vecMax.y = vertChange.y;
		if (vertChange.z > vecMax.z) vecMax.z = vertChange.z;
		if (vertChange.x < vecMin.x) vecMin.x = vertChange.x;
		if (vertChange.y < vecMin.y) vecMin.y = vertChange.y;
		if (vertChange.z < vecMin.z) vecMin.z = vertChange.z;
	}
	//包围盒带矩阵
	vecMin.MultiplyMatrix(m_matWorldView);
	vecMax.MultiplyMatrix(m_matWorldView);
	m_BoundingBox.SetExtents(vecMin, vecMax);
}

