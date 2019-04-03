#pragma once

#include "Base3D/UGMatrix4d.h"
#include "Base3D/UGTextureData.h"
#include "Base3D/UGMaterial3D.h"
#include "Base3D/UGRenderOperationGroup.h"
#include "Base3D/UGModelSkeletonDataPackTemplate.h"

namespace UGC
{
#define UG_MATERTIALNAME_DEFAULT	_U("matdefault")
#define UG_TEXTURENAME_DEFAULT	_U("texdefault")

class UGModelEM;

typedef UGModelSkeletonDataPackTemplate<UGVertexDataPackageExact> UGModelSkeletonExactPack;
typedef UGModelSkeletonDataPackTemplate<UGVertexDataPackage>	  UGModelSkeletonRenderPack;

class BASE3D_API UGModelEntity
{
public:
	enum METype
	{
		etUnknown		= 0,
		etSkeleton	= 1,
		etTexture		= 2,
		etMaterial	= 3
	};

public:
	UGModelEntity(){};
	~UGModelEntity(){};

public:
	virtual UGModelEntity::METype GetType() = 0;
	virtual std::vector<UGString> GetAtt() = 0;
	virtual UGString GetName() = 0;
	virtual UGbool HasBoundingBox() = 0;

	//! \brief 骨架重载之
	virtual UGBoundingBox GetBoundingBox()
	{
		return UGBoundingBox();
	}
};

class BASE3D_API UGModelTexture : public UGModelEntity
{
public:
	UGModelTexture();

	UGModelTexture(const UGModelTexture& other);

	UGModelTexture(UGString strName);

public:
	~UGModelTexture();

public:
	UGModelEntity::METype GetType()
	{
		return UGModelEntity::etTexture;
	}
	std::vector<UGString> GetAtt()
	{
		return std::vector<UGString>();
	}

	UGString GetName()
	{
		return m_strName;
	}

	UGbool HasBoundingBox()
	{
		return FALSE;
	}

	UGModelTexture& operator = (const UGModelTexture& other);

	//! \brief 写入
	UGbool Save(UGStream& stream);

	//! \brief 加载
	UGbool Load(UGStream& stream);

	//! \brief 创建mipmap
	void UpdateTextureTier();

	//! \brief 初始化默认内容
	void MakeDefault();

	//! \brief 通过TextureData解析出MipMap信息
	void MakeInfoFromTextureData();

	//! \brief 对纹理数据进行缩放
	//! \brief nMaxW nMaxH 宽高限制； =0 表示不限制
	//! \brief 返回 comp
	static UGint ScaleTextureData(UGTextureData* pTextureData, const UGint nMaxW=0, const UGint nMaxH=0);

public:
	UGString m_strName;

	UGTextureData* m_pTextureData;

	UGbool m_bMipmap;

	UGint m_nLevel;
};
typedef UGSharedPtr<UGModelTexture> UGModelTexturePtr;

class BASE3D_API UGModelMaterial : public UGMaterial3D, public UGModelEntity
{
public:
	UGModelMaterial(UGString strName=UG_MATERTIALNAME_DEFAULT);
	
	~UGModelMaterial();
public:
	UGModelEntity::METype GetType()
	{
		return UGModelEntity::etMaterial;
	}

	//! \brief 获取属性
	std::vector<UGString> GetAtt();

	UGString GetName()
	{
		return m_strName;
	}

	UGbool HasBoundingBox()
	{
		return FALSE;
	}

	//! \brief 获取材质中的纹理名
	void GetTextureNames(std::vector<UGString> &vecNames);

	//! \brief 用mapTextures里的名字替换现有的
	UGbool ResetTextureName(std::map<UGString, UGString>& mapTextures);

	//! \brief 设置纹理名，没有则新建
	//! \brief vecTexNames个数为0，则清空纹理
	void SetTextureName(std::vector<UGString>& );

	//! \brief 根据自己的内容起个名字
	UGString MakeHashName();

	//! \brief 初始化默认内容
	void MakeDefault();

	//! \brief 根据Pass的内容起个名字
	static UGString MakeHashName(UGPass* pPass);

};
typedef UGSharedPtr<UGModelMaterial> UGModelMaterialPtr;

class BASE3D_API UGModelSkeleton : public UGModelEntity
{
public:
	UGModelSkeleton();
	UGModelSkeleton(UGString strName);
	UGModelSkeleton(const UGModelSkeleton& other);

public:
	~UGModelSkeleton();

public:
	UGModelEntity::METype GetType()
	{
		return UGModelEntity::etSkeleton;
	}

	std::vector<UGString> GetAtt();

	UGString GetName()
	{
		return m_strName;
	}

	UGbool HasBoundingBox()
	{
		return TRUE;
	}

	UGModelSkeleton& operator = (const UGModelSkeleton& other);

	//! \brief 只与顶点相关，与矩阵无关
	void ComputerBoundingBox();

	//! \brief 只与顶点相关，与矩阵无关
	UGBoundingBox GetBoundingBox();

	void SetBoundingBox(UGBoundingBox bbox);

	//! \brief 写入
	UGbool Save(UGStream& stream);

	//! \brief 加载
	UGbool Load(UGStream& stream);

	//! \brief 是否使用高精度数据
	void SetExactDataTag(UGbool bExactTag = TRUE);

	//! \brief 是否使用高精度数据
	UGbool GetExactDataTag() const;

	//! \brief 原始数据的Index是否带_2
	UGbool HasIndexAtt();

	//! \brief 获取渲染数据包，如果不是低精度不带属性，则构建
	UGModelSkeletonRenderPack* GetRenderDataPack();
	//! \brief 获取渲染包的IndexPackage
	UGIndexPackage* GetRenderIndexPackage(const UGint i);
	UGArray<UGIndexPackage*> GetRenderIndexPackage();
	//! \brief 获取渲染包的顶点索引包
	UGVertexDataPackage* GetRenderVertexPack();
	//! \brief 获取渲染包的顶点个数
	UGint GetRenderVertexCount();
	//! \brief 获取渲染包的顶点指针
	UGfloat* GetRenderVertexPtr();

	//! \brief 获取分析数据包；如果不是高精度，则构建成高精度（不会改变Index）
	void GetExactDataPack(UGVertexDataPackageExact*& pVertexPack, 
		UGArray<UGIndexPackage*> &arrIndexPack);

	//! \brief 获取分析数据包；如果不是高精度，则构建成高精度（不会改变Index）
	void GetExactDataPack(UGVertexDataPackageExact*& pVertexPack, 
		UGIndexPackage*& pIndexPack);

	//! \brief 清理
	void Clear();

	//! \brief 判断存储的数据是否有效
	UGbool IsValid();

	//! \brief 设置数据包：高精度
	void SetExactDataPackRef(UGVertexDataPackageExact* pVertexPack, 
		UGArray<UGIndexPackage*> &arrIndexPack);
	//! \brief 设置数据包：高精度
	void SetExactDataPackRef(UGVertexDataPackageExact* pVertexPack, 
		UGIndexPackage* pIndexPack);

	//! \brief 设置数据包：低精度
	void SetDataPackRef(UGVertexDataPackage* pVertexPack, \
		UGArray<UGIndexPackage*> &arrIndexPack);
	//! \brief 设置数据包：低精度
	void SetDataPackRef(UGVertexDataPackage* pVertexPack, \
		UGIndexPackage* pIndexPack);

	//! \brief 重新设置IndexPack
	void SetIndexPackage(UGArray<UGIndexPackage*> &arrIndexPack);

	//! \brief 获取高精度数据包，可能为空
	UGModelSkeletonExactPack* GetExactDataPackRef();

	//! \brief 获取低精度数据包，可能为空
	UGModelSkeletonRenderPack* GetInExactDataPackRef();

	//! \brief 获取顶点和三角面个数
	void GetObjectInfo(UGint &nVertex, UGint& nTriangle);

	//! \brief 获取骨架PassName
	UGArray<UGString> GetPassNames();

	//! \brief 获取Indexpack个数：高精度、低精度、渲染包共用
	UGint GetIndexPackageCount();

	//! \brief 获取IndexPackage
	UGIndexPackage* GetIndexPackage(const UGint i=0);

	//! \brief 获取第i个IndexPack的三角形个数
	UGint GetTriangleCount(const UGint i=0);

	//! \brief 获取顶点个数
	UGint GetVertexCount();

	//! \brief 获取顶点颜色个数
	UGint GetVertexColorCount();

	//! \brief 获取顶点颜色
	UGuint* GetVertexColor();

	//! \brief 获取第i个顶点
	UGVector3d GetVertex(const UGint i);
	//! \brief 设置第i个顶点值
	void SetVertex(const UGint i, const UGVector3d& vec);

	//! \brief 设置材质名：骨架属性和Indexpack的pass名
	void SetMaterialName(UGString strMaterialName);

	//! \brief 替换材质名
	UGbool ResetMaterialName(std::map<UGString, UGModelMaterialPtr>& mapMaterial);

	//! \brief 替换材质名
	UGbool ResetMaterialName(std::map<UGString, UGString>& mapReNames);

public:
	//! \brief 获取骨架纹理坐标，兼顾高/低精度数据
	//! \brief nCount:坐标对个数
	//! \brief pTextureCoods内存仍是骨架管理
	void GetTextureCoords(UGint nTexUnit, UGfloat*& pTextureCoods, UGint& nCount, UGint& nDim);

	//! \brief 设置骨架纹理坐标，兼顾高/低精度数据
	//! \brief nCount:坐标对个数
	//! \brief pTextureCoods 托管给骨架对象，外部不能释放
	void SetTextureCoords(UGint nTexUnit, UGfloat* pTextureCoods, UGint nCount, UGint nDim);

	//! \brief 获取骨架法向量，兼顾高/低精度数据
	void GetNormalCoords(UGfloat*& pNormal, UGint& nCount);

	//! \brief 设置骨架法向量，兼顾高/低精度数据
	//! \brief nCount:坐标对个数
	//! \brief pNormal 托管给骨架对象，外部不能释放
	void SetNormalCoords(UGfloat* pNormal, UGint nCount);

private:
	//! \brief 清理
	void ClearRenderPack();

	//! \brief 清理
	void ClearExactPack();

	//! \brief 清理
	void ClearInExactPack();

	//! \brief 构造数据：不会改变Index
	void MakeExactPack();

	//! \brief 构造渲染数据包
	void MakeRenderPack();

	template<typename TVertexDataPack, typename TType>
	void ExpandByIndex(TVertexDataPack* pVertexDataPackSrc, \
		UGArray<UGIndexPackage*> &arrIndexPackSrc,\
		UGVertexDataPackage* pVertexDataPackDes,\
		UGArray<UGIndexPackage*>& arrIndexPackDes)
	{
		if(pVertexDataPackDes == NULL || arrIndexPackDes.GetSize() >0)
		{
			UGASSERT(FALSE);
			return;
		}

		TType *pVertexSrc = pVertexDataPackSrc->m_pVertices;
		UGfloat *pNormalSrc = pVertexDataPackSrc->m_pNormals;
		UGfloat *pVertexDes = NULL;
		UGfloat *pNormalDes = NULL;

		UGint nCount = 0;
		for (UGint i=0; i< arrIndexPackSrc.GetSize(); i++)
		{
			UGIndexPackage* pIndexPackageSrc = arrIndexPackSrc[i];

			UGASSERT(pIndexPackageSrc->m_enIndexType== IT_16BIT_2 || 
				pIndexPackageSrc->m_enIndexType== IT_32BIT_2);

			UGint nIndexCount = pIndexPackageSrc->m_nIndexesCount / 2;
			UGASSERT(nIndexCount % 3 == 0);
			nCount += nIndexCount;
		}

		pVertexDataPackDes->m_nVertexDimension = pVertexDataPackSrc->m_nVertexDimension;
		pVertexDataPackDes->SetVertexNum(nCount);
		pVertexDes = pVertexDataPackDes->m_pVertices;

		if(pNormalSrc != NULL)
		{
			pVertexDataPackDes->m_nNormalDimension = pVertexDataPackSrc->m_nNormalDimension;
			pVertexDataPackDes->SetNormalNum(nCount);
			pNormalDes = pVertexDataPackDes->m_pNormals;
		}
		for (UGint i=0; i<SMSCN_MAX_TEXTURE_COORD_SETS; i++)
		{
			if(pVertexDataPackSrc->m_pTexCoords[i] != NULL)
			{
				pVertexDataPackDes->m_nTexDimensions[i] = pVertexDataPackSrc->m_nTexDimensions[i];
				pVertexDataPackDes->SetTexCoordsNum(i, nCount);
			}
		}

		arrIndexPackDes.SetSize(arrIndexPackSrc.GetSize());

		for (UGint i=0; i< arrIndexPackSrc.GetSize(); i++)
		{
			UGIndexPackage* pIndexPackageSrc = arrIndexPackSrc[i];
			UGIndexPackage* pIndexPackageDes = new UGIndexPackage();
			pIndexPackageDes->m_strPassName.Append(pIndexPackageSrc->m_strPassName);

			pIndexPackageDes->m_enIndexType = nCount > 65535 ? IT_32BIT : IT_16BIT;
			pIndexPackageDes->SetIndexNum(nCount);

			//前半部分是顶点索引，后半部分是对应的属性索引
			UGint nTriIndexCount = pIndexPackageSrc->m_nIndexesCount / 2;
			UGASSERT(nTriIndexCount % 3 == 0);

			for (UGint j=0; j<nTriIndexCount; j++)
			{
				UGint idxVertex = -1, idxAtt = -1;
				if(pIndexPackageSrc->m_enIndexType == IT_32BIT_2)
				{
					idxVertex = ((UGint*)pIndexPackageSrc->m_pIndexes)[j];
					idxAtt = ((UGint*)pIndexPackageSrc->m_pIndexes)[j + nTriIndexCount];
				}
				else if(pIndexPackageSrc->m_enIndexType == IT_16BIT_2)
				{
					idxVertex = pIndexPackageSrc->m_pIndexes[j];
					idxAtt = pIndexPackageSrc->m_pIndexes[j + nTriIndexCount];
				}

				for(UGint t = 0; t < pVertexDataPackDes->m_nVertexDimension; t++)
				{
					*pVertexDes++ = pVertexSrc[idxVertex * pVertexDataPackDes->m_nVertexDimension + t];
				}

				if(pNormalSrc != NULL)
				{
					for(UGint t = 0; t < pVertexDataPackDes->m_nNormalDimension; t++)
					{
						*pNormalDes++ = pNormalSrc[idxAtt * pVertexDataPackDes->m_nNormalDimension + t];
					}
				}
				for (UGint k=0; k<SMSCN_MAX_TEXTURE_COORD_SETS; k++)
				{
					if(pVertexDataPackSrc->m_pTexCoords[k] != NULL)
					{
						UGushort uDim = pVertexDataPackSrc->m_nTexDimensions[k];
						UGfloat* pTextureSrc = pVertexDataPackSrc->m_pTexCoords[k] + uDim * idxAtt;
						UGfloat* pTextureDes = pVertexDataPackDes->m_pTexCoords[k] + uDim * j;
						for (UGint l=0; l<uDim; l++)
						{
							*pTextureDes++ = *pTextureSrc++;
						}
					}
				}

				if(pIndexPackageDes->m_enIndexType == IT_32BIT)
				{
					((UGint*)pIndexPackageDes->m_pIndexes)[j] = j;
				}
				else if(pIndexPackageDes->m_enIndexType == IT_16BIT)
				{
					pIndexPackageDes->m_pIndexes[j] = j;
				}
			}

			arrIndexPackDes.SetAt(i, pIndexPackageDes);
		}
	}

public:
	//! \brief 名字
	UGString m_strName;

	//! \brief 材质名
	UGString m_strMaterialName;

	//////////////////////////////////////////////////////////////////////////
	//为保证外部接口不变，保留这两个变量
	//! \brief 模型顶点信息：低精度
// 	UGVertexDataPackage* m_pVertexDataPackage;
// 	//! \brief 索引信息
// 	UGArray<UGIndexPackage*> m_arrIndexPackage;
	//////////////////////////////////////////////////////////////////////////

	//! \brief 位置矩阵
	UGMatrix4d m_matLocalView;

	//! \brief 在ModelNode中的引用次数
	UGint m_nRefCount;

private:	
	//! \brief 数据包标记：
	//! \brief 为TRUE，则m_pDataExactPack!=NULL
	//! \brief 为FALSE，则m_pDataInExactPack!=NULL
	UGbool m_bExactTag;

	//! \brief 模型顶点信息：渲染包。用的时候构造
	UGModelSkeletonRenderPack* m_pDataRenderPack;
	UGbool m_bRenderPackShared;

	//! \brief 模型顶点信息：低精度数据
	UGModelSkeletonRenderPack* m_pDataInExactPack;

	//! \brief 模型顶点信息：高精度数据
	UGModelSkeletonExactPack* m_pDataExactPack;

	//! \brief 包围盒
	UGBoundingBox m_BoundingBox;
};

typedef UGSharedPtr<UGModelSkeleton> UGModelSkeletonPtr;

}
