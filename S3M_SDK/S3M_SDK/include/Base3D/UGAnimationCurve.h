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


#if !defined(AFX_UGANIMATIONCURVE_H__E89D0670_EB32_4782_8964_7F60F1421BCF__INCLUDED_)
#define AFX_UGANIMATIONCURVE_H__E89D0670_EB32_4782_8964_7F60F1421BCF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Base3D/UGPrerequisites3D.h"
#include "Base3D/UGVector3d.h"
#include "Base3D/UGQuaternion4d.h"
#include "Stream/UGStream.h"

namespace UGC
{
class UGAnimationTrack;

//! \brief 动画关键帧类
class BASE3D_API UGAnimationKey
{
public:
	//! \brief 构造函数
	UGAnimationKey();

	//! \brief 析构函数
	virtual ~UGAnimationKey();

	//! \brief 重载拷等号。
	void operator=(const UGAnimationKey& other);

	//! \brief 从流文件中加载
	virtual void Load(UGStream& fStream);

	//! \brief 存入流文件中
	virtual void Save(UGStream& fStream);

	//! \brief 获得当前帧的时间
	UGdouble GetTime(void) const { return m_fTimePos; }

	//! \brief 设置当前帧的时间
	//! \param dTimePos 当前动画的位置[in]。
	void SetTime(UGdouble fTimePos){ m_fTimePos = fTimePos;}

	//! \brief 设置当前帧的缩放比
	//! \param vecScale 当前帧的缩放比[in]。
	void SetScale(const UGVector3d& vecScale);

	//! \brief 获得当前帧的缩放比
	const UGVector3d& GetScale(void) const;

	//! \brief 设置当前帧的旋转角度
	//! \param rot 当前帧的旋转角度[in]。
	void SetRotation(const UGQuaternion4d& rot);

	//! \brief 获得当前帧的旋转角度
	const UGQuaternion4d& GetRotation(void) const;

	//! \brief 设置当前帧的平移
	//! \param vecTrans 当前动画的位置[in]。
	void SetTranslate(const UGVector3d& vecTrans);

	//! \brief 获得前帧的平移
	const UGVector3d& GetTranslate(void) const;

	//! \brief 获得结点的大小
	UGint GetDataSize() const;

public:
	//! \brief 当前帧的旋转角度
	UGQuaternion4d m_Rotation;

	//! \brief 当前帧的平移
	UGVector3d m_vecTranslate;

	//! \brief 当前帧的缩放比
	UGVector3d m_vecScale;

	//! \brief 当前帧的时间
	UGfloat m_fTimePos;
};

class BASE3D_API  UGAnimationCurve
{
public:
	//! \brief 构造函数
	UGAnimationCurve();

	//! \brief 析构函数
	~UGAnimationCurve(void);

	//! \brief 重载等号。
	void operator=(const UGAnimationCurve& other);

	//! \brief 关键帧数。
	//! \param 
	//! \return 。
	UGint GetKeyCount(void);

	//! \brief 设置关键帧数。
	//! \param nCount [in] 关键帧数
	//! \return 。
	void SetKeyCount(UGint nCount);

	//! \brief 设置关键帧数。
	//! \param nCount [in] 关键帧数
	//! \return 。
	UGArray<UGAnimationKey*>& GetKeys();

	//! \brief 增加关键帧数。
	//! \param targetType [in] 关键帧的作用类型
	//! \return 。
	UGAnimationKey* AddKey();

	void RemoveAllKey();

	//! \brief 从流文件中加载
	void Load(UGStream& fStream);

	//! \brief 存入流文件中
	void Save(UGStream& fStream);

	//! \brief 获得结点的大小
	UGint GetDataSize() const;

	//! \brief 设置旋转的插值方式
	void SetRotationInterpolationMode(RotationInterpolationMode im);

	//! \brief 获取旋转的插值方式
	RotationInterpolationMode GetRotationInterpolationMode(void) const;

	//! \breif 设置平移插值方式
	void SetTranslateInterpolationMode(TranslateInterplolationMode tim);

	//! \brief 获取平移插值放肆
	TranslateInterplolationMode GetTranslateInterploationMode() const;

	//! \brief设置当前动画的插值方式
	void SetInterpolatonMode(InterpolationMode im);

	//! \brief获取当前动画的插值方式      
	InterpolationMode GetInterpolationMode(void) const;

private:
	//! \breif 版本号，预留
	UGdouble m_dVersion;

	//! \brief 动画信息
	UGArray<UGAnimationKey*> m_arrKeys;

	//! \brief 动画作用的结点目标
	UGString m_strTarget;

	//! \brief 旋转的插值方式
	RotationInterpolationMode m_RotationInterpolationMode;;

	//! \breif 动画的插值方式
	InterpolationMode m_InterpolationMode;

	//! \brief平移的插值方式
	TranslateInterplolationMode m_translateInterplationMode;
};
}
#endif // !defined(AFX_UGANIMATIONCURVE_H__E89D0670_EB32_4782_8964_7F60F1421BCF__INCLUDED_)

