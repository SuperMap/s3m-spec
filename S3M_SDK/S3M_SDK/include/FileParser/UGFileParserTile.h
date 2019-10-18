//////////////////////////////////////////////////////////////////////////
//                    _   _   ____   ____                    
//   Project         | | | | / ___) / ___)                   
//                   | | | || | __ | |                       
//                   | |_| || |_\ || \___                    
//                    \____/ \____| \ ___)     6.0           
//                                                           
//!  \file headerfile.h
//!  \brief 
//!  \details 
//!  \author
//!  \attention 
//!   Copyright (c) 1996-2010 SuperMap Software Co., Ltd. <br> 
//!   All Rights Reserved.
//!  \version 6.0
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_UGFILEPARSERTILE_H__54B13918_6164_49A5_9255_99DD4740622F__INCLUDED_)
#define AFX_UGFILEPARSERTILE_H__54B13918_6164_49A5_9255_99DD4740622F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Base3D/UGModelNodeShells.h"

namespace UGC
{

class FILEPARSER_API UGTileStuff
{
public:
	//! \brief 下层Tile信息：切换距离和Tile名
	typedef std::pair<UGdouble, UGString>			  PagedInfo;
	typedef std::vector<PagedInfo>					  PagedInfos;

	typedef std::pair<UGModelPagedPatch*, PagedInfos> PatchData;
	typedef std::vector<PatchData>					  PatchDatas;

public:
	UGTileStuff();
	~UGTileStuff();

	//! \brief 清理内存
	void Clear();

	//! \brief 添加一个Patch，暂无子节点信息
	void AddOnePatch(UGModelPagedPatch* pPatch);

	//! \brief 添加一个Patch及子节点信息
	void AddPatchInfo(UGModelPagedPatch* pPatch, PagedInfo& info);

	//! \brief 获取所有Patch
	void GetPatches(std::vector<UGModelPagedPatch*>& vecPatches, UGbool bDelegate);

	//! \brief 获取所有PagedInfos
	PagedInfos GetPagedInfos();

	//! \brief 获取所有PagedDatas，并托管
	void DelegatePatchDatas(PatchDatas& patchdatas);

	//! \brief 设置RangeMode
	void SetRangeMode(UGRangeMode rangeMode);

	//! \brief 获取RangeMode
	UGRangeMode GetRangeMode();

	//! \brief 根据pPatch设置Tile名
	void SetTileName(UGModelPagedPatch* pPatch);

	//! \brief 获取Tile名
	UGString GetTileName();

	//! \brief 计算包围盒
	UGBoundingBox GetBoundingBox();	

	//! \brief 设置包围盒
	void SetBoundingBox(UGBoundingBox &bbox);

private:
	//! \brief 切换模式
	UGRangeMode m_rangeMode;

	//! \brief 所有的Patch
	PatchDatas m_vecPatchDatas;

	//! \brief 自身的包围盒
	UGBoundingBox m_bbox;

	//! \brief Tile名
	UGString m_strTileName;
};


//! \brief 单个模型文件解析
class FILEPARSER_API UGFileParserTile 
{
public:
	UGFileParserTile();
	~UGFileParserTile();

	//! \brief 打开文件
	virtual UGbool Open(const UGString& strDir, const UGString& strTile);

	//! \brief 关闭文件	
	virtual void Close();

	//! \brief 是否解析材质。提取PagedLOD信息时不需要解析材质
	virtual void SetParseMaterial(UGbool bParseMaterial);
	
	//! \brief 是否解析顶点。提取PagedLOD信息时计算Bound之后就不需要骨架了
	virtual void SetParseSkeleton(UGbool bParseSkeleton);

	//! \brief 设置obj解析时绕X轴的旋转方式.仅支持osgb插件的obj
	void SetRotateOption(UGint obj_opt);
	
	//! \brief 获取patch数据
	virtual void GetPatches(std::vector<UGModelPagedPatch*> &vecPatches, UGbool bDelegate);

	//! \brief 获取解析数据
	UGTileStuff* GetTileStuff();

	//! \brief 获取解析数据
	UGTileStuff::PatchDatas DelegatePatchDatas();

	//! \brief 获取本层的数据范围
	//! \brief bForce 当pagedlod信息中包围盒为空，是否强制读数据
	UGBoundingBox GetBoundingBox();

	//! \brief 获取本层的切换模式
	UGRangeMode GetRangeMode();

	//! \brief 文件的全局矩阵（目前给dae的单位用）
	UGMatrix4d GetMatrixGloble();

	//! \brief 用来设置压缩参数（全部解压\不压缩（0）;其他参照VertexCompressOptions各个位的意义）
	void SetVertexCompressOptions(UGint nVertexCompressOptions);

	//! \brief 加载时是否强制解压顶点
	void SetForceUnzip(UGbool bForceUnzip);

protected:
	//! \brief 文件夹
	UGString m_strDir;

	//! \brief 文件名
	UGString m_strTile;

	//! \brief 给文件生成一个唯一标示，骨架名使用
	UGString m_strFileUUID;

	//! \brief 是否解析材质。提取PagedLOD信息时不需要解析材质
	UGbool m_bParseMaterial;

	//! \brief 是否解析骨架。提取PagedLOD信息时计算好了Bound，骨架就不需要了
	UGbool m_bParseSkeleton;

	//! \brief 字符集
	UGString::Charset m_charset;
	
	//! \brief obj文件解析选项
	UGint m_objRotateOpt;

	//! \brief 文件的全局矩阵（目前给dae的单位用）
	UGMatrix4d m_matGloble;

	//! \brief 压缩参数（全部解压\不压缩（0）;其他参照VertexCompressOptions各个位的意义）
	UGint m_nVertexCompressOptions;

	//! \brief 解析时是否强制解压顶点
	UGbool m_bForceUnzip;

	//! \brief 一个Tile解析出来的多个Patch，为保留与下一层的对应关系，Patch不能合并
	//! \brief PagedLOD信息: Patch 及其对应的 下一层切换距离和文件名
	UGTileStuff* m_pTileStuff;
};


}

#endif // !defined(AFX_UGFILEPARSERTILE_H__54B13918_6164_49A5_9255_99DD4740622F__INCLUDED_)



