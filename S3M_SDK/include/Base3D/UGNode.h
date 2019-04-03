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
//!  \author zhangshuai。
//!  \attention 
//!   Copyright (c) 1996-2010 SuperMap Software Co., Ltd. <br> 
//!   All Rights Reserved.
//!  \version 6.0
//////////////////////////////////////////////////////////////////////////


#if !defined(AFX_UGNODE_H__E89D0670_EB32_4782_8964_7F60F1421BCF__INCLUDED_)
#define AFX_UGNODE_H__E89D0670_EB32_4782_8964_7F60F1421BCF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Base3D/UGMesh.h"
#include "Base3D/UGQuaternion4d.h"

namespace UGC
{
class BASE3D_API UGNode
{
public:
	//! \brief 构造函数。
	UGNode();

	//! \brief 构造函数。
	~UGNode(void);

	//! \brief 重载等号。
	void operator=(const UGNode& other);

	//! \brief 获取结点的名称。
	UGString& GetName(void);

	//! \brief 设置结点的名称。
	//! \param strName 名称[in]。
	//! \return 。
	//! \remarks 。
	//! \attention 。
	void SetName(const UGString& strName);

	//! \brief 增加结点。
	//! \param pNode 结点指针[in]。
	void AddChild(UGNode* pNode);

	//! \brief 增加结点。
	//! \param pNode 结点指针[in]。
	//! \return 。
	//! \remarks 。
	//! \attention 。
	void SetParent(UGNode* pParent);

	//! \brief 增加结点。
	//! \param pNode 结点指针[in]。
	//! \return 。
	//! \remarks 。
	//! \attention 。
	UGint GetNumChildren(void);

	//! \brief 增加结点。
	//! \param pNode 结点指针[in]。
	//! \return 。
	//! \remarks 。
	//! \attention 。
	UGNode* GetChild(UGint nIndex);

	//! \brief 获取结点的Mesh名称。
	UGString& GetMeshName(void);

	//! \brief 设置Mesh的名称。
	//! \param strMeshName Mesh名称[in]。
	//! \return 。
	//! \remarks 。
	//! \attention 。
	void SetMeshName(const UGString& strMeshName);

	//! \brief 获取索引指定的Mesh。
	//! \param nIndex 索引[in]。
	//! \return 。
	//! \remarks 。
	//! \attention 。
	UGMesh* GetMesh(UGint nIndex);

	//! \brief 增加Mesh数据。
	//! \param pMesh 结点指针[in]。
	//! \return 。
	void AddMesh(UGMesh* pMesh);

	//! \brief 获取Mesh的个数。
	//! \return 。
	UGint GetNumMeshs(void);

	//! \brief 清空Mesh数据。
	//! \return 。
	void ClearMesh();

	//! \brief 设置相对位置
	//! \param pos 位置信息[in]。
	//! \return 
	void SetPosition(const UGVector3d& pos);

	//! \brief 获取相对位置。
	//! \return 
	const UGVector3d& GetPosition(void) const;

	//! \brief 获取旋转角度。
	//! \return 
	const UGQuaternion4d& GetOrientation() const;

	//! \brief 设置旋转角度。
	//! \param q 旋转角度[in]。
	//! \return 
	void SetOrientation( const UGQuaternion4d& q );

	//! \brief 设置缩放比。
	//! \param scale 缩放比[in]。
	//! \return 
	void SetScale(const UGVector3d& scale);

	//! \brief 获取综放比。
	//! \return 
	const UGVector3d& GetScale(void) const;

	//! \brief 从流文件中加载
	void Load(UGStream& fStream);

	//! \brief 存入流文件中
	void Save(UGStream& fStream);

	//! \brief 获得结点的大小
	UGint GetDataSize() const;
protected:
	//! \释放子结点
	void ReleaseChild(UGNode* pNode);

	//! \结点名称，作为结点的唯一标识
	UGString m_strName;

	//! \brief 子结点个数
	UGint  m_nNumChildren;

	//! \brief 子结点数组
	UGArray<UGNode*> m_arrChildren;

	//! \brief 父结点
	UGNode* m_pParent;
	
	//! \brief 相对位置
	UGVector3d m_vecPostion;

	//! \brief 缩放比
	UGVector3d m_vecScale;

	//! \brief 旋转角度
	UGQuaternion4d m_Orientation;
	
	//! \brief 当前结点的Mesh数据个数
	UGint m_nNumMesh;

	//! \brief 当前结点的Mesh数组
	UGArray<UGMesh*> m_arrMesh;

	//! \brief 当前结点的mesh的名称
	UGString m_strMeshName;
};
}
#endif // !defined(AFX_UGNODE_H__E89D0670_EB32_4782_8964_7F60F1421BCF__INCLUDED_)
