//////////////////////////////////////////////////////////////////////////
//                    _   _   ____   ____                    
//   Project         | | | | / ___) / ___)                   
//                   | | | || | __ | |                       
//                   | |_| || |_\ || \___                    
//                    \____/ \____| \ ___)     6.0           
//                                                           
//!  \file headerfile.h
//!  \brief Class 的接口定义。
//!  \details 该文件定义了模型文件的主要接口。
//!  \author zhangshuai,sunye
//!  \attention 
//!   Copyright (c) 1996-2008 SuperMap Software Limited Company.  <br> 
//!   All Rights Reserved.
//!  \version 6.0
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_UGFILEPARSERMODE_H__392553F8_7BF9_4668_BD97_4211091FEF20__INCLUDED_)
#define AFX_UGFILEPARSERMODE_H__392553F8_7BF9_4668_BD97_4211091FEF20__INCLUDED_

#include "FileParser/UGFileParse.h"
#include "FileParser/UGFileParserTile.h"
#include "Base3D/UGBoundingBox.h"
#include "Base3D/UGMatrix4d.h"
#include "Base3D/UGNode.h"
#include "Base3D/UGAnimationCurve.h"
#include "Base3D/UGAnimation.h"
#include "Base3D/UGMaterial3D.h"
#include "Base3D/UGRenderOperationGroup.h"
#include "Base3D/UGModelNode.h"

namespace UGC 
{
	class UGMaterial;
	class UGMesh;
	class UGAnimation;
	class UGMaterial3D;
//! \brief 模型的基本信息。
class FILEPARSER_API ModelDataInfo
{
public:
	//! \brief 构造函数。
	ModelDataInfo();

	//! \brief 拷贝构造函数。
	ModelDataInfo(const ModelDataInfo& mInfo);

	//! \brief 重载拷等号。
	void operator=(const ModelDataInfo& mInfo);

	//! \brief 析构函数。
	virtual ~ModelDataInfo();
 
 	//! \brief 清除所有信息恢复默认值。
 	void Clear();
 
	//! \brief 设置模型的名称
	void SetName(const UGString& strName);

	//! \brief 获取模型的名称
	const UGString& GetName() const; 

	//! \brief 得到模型的几何数据。
	UGArray<UGMesh*>& GetMesh();

	//! \brief 获取模型的材质数据。
	UGArray<UGMaterial*>& GetMaterial();

	//! \brief 获取模型的材质数据。
	UGArray<UGMaterial3D*>& GetMaterial3D();

	//! \brief 设置包围盒
	void SetBoundingBox(const UGBoundingBox& box){m_BoundingBox = box;}

	//! \brief 获取模型的包围盒
	UGBoundingBox& GetBoundingBox(){return m_BoundingBox;}

	//! \brief 获取模型的包围盒
	void ComputeBoundingBox();

	//! \brief 模型的在局部坐标矩阵
	UGMatrix4d GetLocalMatrix();

	//! \brief 模型的在局部坐标矩阵
	//! \param matLocalMatrix 局部坐标矩阵
	void SetLocalMatrix(const UGMatrix4d matLocalMatrix);

	//! \brief 获区根结点
	UGNode& GetRootNode(void);

	const UGAnimationCurve* FindAnimationCurve(const UGString& strNodeName)const;

	//! \brief 获取动画信息
	UGbool IsAnimated();
	
	//! \brief 获得起始时间
	UGdouble GetStartTime();

	//! \brief获得结束时间
	UGdouble GetEndTime();

	//! \brief 从流文件中加载
	void Load(UGStream& fStream,const UGString& strFileName);

	//! \brief 存入流文件中
	void Save(UGStream& fStream,const UGString& strFileName);

	//! \brief 获得结点的大小
	UGint GetDataSize() const;

	//! \brief 设置是否有动画
	void SetAnimated(UGbool bAnimated);
	
	//! \brief 设置起始时间
	void SetStartTime(UGdouble dStartTime);
	
	//! \brief 设置结束时间
	void SetEndTime(UGdouble dEndTime);

	//! \brief 获取动画信息
	UGAnimation* GetAnimationState(void);

	//! \brief 设置控制动画信息
	void SetAnimationState(UGAnimation* pAnimation);

	//! \brief 获取节点动画信息
	UGNodeAnimationInfo* GetNodeAnimation(void);

	//! \brief 创建节点动画信息，如果已经有就不新建，只返回原来的
	//! \return 返回创建出的节点动画
	void CreateNodeAnimation();

	//! \brief 得到对象各方向的缩放比率
	//! \return 对象各方向的缩放比率。
	UGPoint3D GetScale() const;	

	//! \brief 得到对象各方向的旋转角度
	//! \return 对象各方向的旋转角度。
	UGPoint3D GetRotation() const;

	//! \brief 设置对象的缩放大小
	//! \param fScaleX [in] X方向缩放比率。
	//! \param fScaleY [in] Y方向缩放比率。
	//! \param fScaleZ [in] Z方向缩放比率。
	void SetScale(UGdouble fScaleX,UGdouble fScaleY,UGdouble fScaleZ);

	//! \brief 设置对象的旋转量 
	//! \param fRotateX [in] X方向的旋转角度。
	//! \param fRotateY [in] Y方向的旋转角度。
	//! \param fRotateZ [in] Z方向的旋转角度。
	void SetRotation(UGdouble fRotateX,UGdouble fRotateY,UGdouble fRotateZ);


    //!  \brief 设置三维模型对象的定位点 
    void SetPosition(const UGPoint3D pntPosition);

    //!  \brief 返回三维模型定位点
	UGPoint3D GetPosition()const;

	//! \brief 归一模型的中心，中心的底面
	UGVector3d UnitizeBaseBottom();

	//! \brief 缩放模型的顶点
	//! \param dRatioX[in] X坐标的缩放比率
	//! \param dRatioY[in] Y坐标的缩放比率
	//! \param dRatioZ[in] Z坐标的缩放比率
	//! \remarks 顶点缩放之后会重新计算包围盒
	void InflateVertices( UGdouble dRatioX, UGdouble dRatioY, UGdouble dRatioZ);

protected:
	
	//! \brief 模型数据的名称
	UGString m_strName;

	//! \brief 材质在数组中的位置与名称的对应关系,
	//! \brief 模型的材质数据。
	UGArray<UGMaterial*> m_arryMaterial;

	//! \brief 材质在数组中的位置与名称的对应关系,
	//! \brief 模型的材质数据。
	UGArray<UGMaterial3D*> m_arryMaterial3D;

	//! \brief 模型的几何数据。
	UGArray<UGMesh*> m_arryMesh;

	//! \brief 模型的包围盒
	UGBoundingBox m_BoundingBox;

	//! \brief 模型的局部坐标
	UGMatrix4d m_matLocalMatrix;

	//! \brief 根结点
	UGNode m_rootNode;

	//! \brief 动画信息
	UGbool m_bIsAnimated;

	//! \brief 动画开始时间
	UGdouble m_dStartTime;

	//! \brief 动画结束时间
	UGdouble m_dEndTime;

	//! \brief 材质对应列表,名称与在数组中的位置对
	UGDict<UGString,UGint> m_MaterialMap;

	//! \brief Mesh对应列表,名称与在数组中的位置对
	UGDict<UGString,UGint> m_MeshMap;

	//! \brief 版本号
	UGdouble m_dVersion;

	//! \brief 动画信息
	UGAnimation* m_pAnimation;

	//! \brief 三维对象的缩放系数
	UGPoint3D m_pntScale;

	//! \brief 三维对象的旋转量
	UGPoint3D m_pntRotate;

    //! \brief 三维模型插入点
	UGPoint3D m_pntPosition;

	//! \brief 节点动画信息
	UGNodeAnimationInfo* m_pNodeAnimation;
public:
	//! \brief 对应于模型信息中的动画UGString用于查找动画
	UGDict<UGString,UGAnimationCurve*> m_AnimationList;
};

class FILEPARSER_API UGModelNodeWriter
{
public:
	UGModelNodeWriter();
	~UGModelNodeWriter();

public:
	//! \brief 保存模型文件
	virtual UGbool SaveNodeFile(UGModelNode *pModelNode, UGString strFilePath);

	//! \brief 保存模型文件
	virtual UGbool SavePatchFile(UGString strPatchName, \
		std::vector<UGModelPagedPatch*>& vecPatches, UGString strFilePath);

	//! \brief 按patch所在的的FileName分组
	//! \brief strOutExt 输出文件的后缀，根据后缀判断文件格式
	static void GroupPatches(std::vector<UGModelPagedPatch*> vecPagedPatch, 
		std::map<UGString, std::vector<UGModelPagedPatch*> >& map_patches);

	//! \brief 设置顶点压缩参数
	void SetVertexCompressOptions(UGint nVertexCompressOptions);

	//! \brief 用来设置纹理压缩类型,默认为enrS3TCDXTN
	void SetTextureCompressType(UGuint nCompressType);

	//! \brief 设置纹理是否保持不变
	void SetIsChangeTexture(UGbool bChanged);

protected:

	//! \brief 填充实体包
	virtual void FillEntityPack(UGModelNode *pModelNode) = 0;

	//! \brief 填充实体包
	virtual void FillEntityPack(std::vector<UGModelPagedPatch*>& vecPatches) = 0;

	//! \brief 递归处理vecPagedPatch
	virtual void ProcessPatches(std::map<UGString, std::vector<UGModelPagedPatch*> >& map_patches, 
		UGString strParentDir, UGString strDestFloder,UGbool bIgnoreChild) = 0;

	//! \brief 检查一下是否有数据
	//! \brief 忽略LOD的模型解析叶子节点只有空壳，没有数据
	UGbool HasData(std::map<UGString, std::vector<UGModelPagedPatch*> >& map_patches);

	void CheckPatchFileNames(UGModelNode *pModelNode, const UGString strDesFile);

protected:
	//! \brief 文件扩展名
	UGString m_strExtName;

	//! \brief 压缩参数（全部解压\不压缩（0）;其他参照VertexCompressOptions各个位的意义）
	UGint m_nVertexCompressOptions;

	//! \brief 纹理压缩类型
	UGuint m_nTextureCompressType;

	//! \brief 模型保存纹理是否保持原样
	UGbool m_bIsChangeTexture;

};

class FILEPARSER_API UGFileParseModel : public UGFileParser  
{
public:

	//! \brief 构造函数。
	UGFileParseModel();	

	//! \brief 析构函数。
	virtual ~UGFileParseModel();

public:
	enum ObjRotateOption
	{
		No_Rotate = 0, //(x, y, z)
		Rotate_YZ = 1, //(x, -z, y) 右手系绕X轴旋转90度
		Rotate_ZY = 2  //(x, z, -y) 右手系绕X轴旋转-90度
	};

public:	
	ModelDataInfo* GetModelDataInfo()const;

	//! \brief 获得渲染组
	//! \param bSetNULL 渲染组设空，由外面释放内存，防止拷贝占用空间时间
	void GetRenderOperationGroup(UGRenderOperationGroup*& pGroup, UGbool bSetNULL = TRUE);

	//! \brief 是否解析成模型对象
	void SetParseModelNode(const UGbool bParseAsModelNode);

	//! \brief 获取解析结果：独立的模型对象;调用后相当于托管给外部了
	UGModelNode* GetModelNode();

	//! \brief 设置obj解析时绕X轴的旋转方式
	void SetRotateOption(UGFileParseModel::ObjRotateOption opt);

	//! \brief 是否忽略(osgb)Lod层数据
	void SetIgnoreLod(UGbool bIgnorenLod);

	//! \brief 是否调整Lod层级
	void SetAdjustLOD(UGbool bAdjust);

	//! \brief 是否解析材质。提取PagedLOD信息时不需要解析材质
	void SetParseMaterial(UGbool bParseMaterial);

	//! \brief 是否解析骨架。提取PagedLOD信息时计算Bound之后，骨架就不需要了
	void SetParseSkeleton(UGbool bParseSkeleton);

	//! \brief 用来设置压缩参数（全部解压\不压缩（0）;其他参照VertexCompressOptions各个位的意义）
	void SetVertexCompressOptions(UGint nVertexCompressOptions);

	//! \brief 用来设置纹理压缩类型,默认为enrS3TCDXTN
	void SetTextureCompressType(UGuint nCompressType);

	//! \brief 设置纹理是否保持不变
	void SetIsChangeTexture(UGbool bChanged);

	//! \brief 解析一个Tile
	virtual UGTileStuff::PatchDatas GetTilePatchDatas(const UGString& strDir, const UGString& strTile,UGRangeMode& nMode);

	//! \brief 组装树形结构
	UGbool MakeTileTree(const UGString strDir, const UGString strTileName, \
		const UGint nCurrentLODNum, UGModelPagedPatch* pPatchParent);

	////////////////////////////////////////////////////////////////////////
	//
	// 常用基本属性
	//
	////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// 读出函数
	//////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////
	// 写入函数
	//////////////////////////////////////////////////////////////////////////
	virtual UGbool InitWiter();

	virtual UGbool Save(const UGExportParams& expParams,const ModelDataInfo* pModelInfo);

	virtual UGbool Save(const UGExportParams& expParams, UGRenderOperationGroup* pGroup);

	virtual UGbool Save(UGMemoryStream& stream, UGRenderOperationGroup* pGroup);

	//! \brief 保存ModelNode
	virtual UGbool Save(const UGExportParams& expParams, UGModelNode* pModelNode);

	//! \brief 保存:ModelNode的patch打包
	virtual UGbool Save(const UGExportParams& expParams, UGString strPatchName,\
		std::vector<UGModelPagedPatch*>& vecPatches);

protected:
	ModelDataInfo* m_pModelInfo;

	//! \brief 渲染组
	UGRenderOperationGroup* m_pRenderOperationGroup;

	//! \brief 是否解析成模型
	UGbool m_bParseAsModelNode;

	//! \brief 独立的模型对象
	UGModelNode* m_pModelNode;

	ObjRotateOption m_objRotateOpt;

	//! \brief 是否忽略(osgb)Lod层数据;默认不忽略
	UGbool m_bIgnoreLod;

	//! \brief 是否调整lod层
	UGbool m_bAdjustLOD;

	//! \brief 是否解析材质
	UGbool m_bParseMaterial;

	//! \brief 是否解析顶点数据（解析骨架，计算bound之后就删除了）
	UGbool m_bParseSkeleton;

	//! \brief 压缩参数（全部解压\不压缩（0）;其他参照VertexCompressOptions各个位的意义）
	UGint m_nVertexCompressOptions;

	//! \brief 纹理压缩类型
	UGuint m_nTextureCompressType;

	//! \brief 模型保存纹理是否保持原样
	UGbool m_bIsChangeTexture;

	//! \brief 模型保存对象
	UGModelNodeWriter* m_pModelNodeWriter;
};

} //namespace UGC

#endif // !defined(AFX_UGFILEPARSERMODE_H__392553F8_7BF9_4668_BD97_4211091FEF20__INCLUDED_)

