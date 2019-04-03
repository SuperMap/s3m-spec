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
#if !defined(AFX_UGANIMATION_H__E89D0670_EB32_4782_8964_7F60F1421BCF__INCLUDED_)
#define AFX_UGANIMATION_H__E89D0670_EB32_4782_8964_7F60F1421BCF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Base3D/UGPrerequisites3D.h"
#include "Base3D/UGAnimationCurve.h"
#include "Stream/UGStream.h"
namespace UGC
{
class UGAnimation;

//! \brief 时间位置改变回调事件回调函数。
//! \param pWnd 句柄[in]。
typedef void (UGSTDCALL *TimePositionChangedProc)(UGlong pHandle);

//! \brief 节点动画结束回调事件回调函数。
//! \param pWnd 句柄[in]。
typedef void (UGSTDCALL *NodeAnimationFinishedProc)(UGlong pHandle);

class UGNodeAnimationInfo;

//! \brief 节点动画状态监听器。
class BASE3D_API UGNodeAnimationListener
{
public:
	//! \brief UGNodeAnimationInfo的Length参数改变时调用
	virtual void TimeLengthUpdate(UGNodeAnimationInfo* pAnimation) = 0;

	//! \brief UGNodeAnimationInfo的Enable参数改变时调用
	virtual void IsEnableUpdate(UGNodeAnimationInfo* pAnimation) = 0;

	//! \brief UGNodeAnimationInfo的Loop参数改变时调用
	virtual void IsLoopUpdate(UGbool bLoop) = 0;

	//! \brief UGNodeAnimationInfo的SetKeys参数改变时调用
	virtual void IsKeyFrameUpdate(UGArray<UGAnimationKey> keyFrames) = 0;

	//! \brief UGNodeAnimationInfo的TimePosition参数改变时调用
	virtual void TimePositionUpdate(UGNodeAnimationInfo* pAnimation) = 0;
};

class BASE3D_API UGAnimation
{
public:
	UGAnimation();
	//! \brief 构造函数
	//! \param strAnimName 动画名称[in]。
	//! \param dTimePos 当前动画的位置[in]。
	//! \param dTimePos 当前动画的长度[in]。
	//! \param dTimePos 是否启动动画，初始为false不启用[in]。
	UGAnimation(const UGString& strAnimName,UGdouble dTimePos, UGdouble dLength,UGbool bEnabled = false);

	//! \brief 拷贝构造函数
	UGAnimation(const UGAnimation &rhs);

	//! \brief 析构函数
	virtual ~UGAnimation();

	//! \brief 重载==
	UGbool operator==(const UGAnimation& rhs) const;
	
	//! \brief 重载!=
	UGbool operator!=(const UGAnimation& rhs) const;

	//! \brief 从流文件中加载
	void Load(UGStream& fStream);

	//! \brief 存入流文件中
	void Save(UGStream& fStream);

	//! \brief 获得结点的大小
	UGint GetDataSize() const;

public:
	//! \brief 动画名称
	const UGString& GetAnimationName() const;

	//! \brief 获取动画位置
	UGdouble GetTimePosition(void) const;

	//! \brief 设置动画位置
	//! \param dTimePos 当前动画的位置[in]。
	void SetTimePosition(UGdouble dTimePos);

	//! \brief 获取动画长度
	UGdouble GetLength() const;

	//! \brief 设置动画长度
	void SetLength(UGdouble dLen);

	//! \brief 更新动画位置，在当前位置上加上dOffset
	//! \param dOffset 动画时间增加的值[in]。
	void Update(UGdouble dOffset);

	//! \brief 当前动画是否已结束，当loop为true，返回始终为true
	UGbool HasEnded(void) const;

	//! \brief 获得当前动画是否可用的标识
	//! \return 。
	UGbool GetEnabled(void) const;

	//! \brief 设置动画是否可用的标识
	//! \param bEnabled 为true为当前动画可用[in]。
	//! \return 。
	void SetEnabled(UGbool bEnabled);

	//! \brief 设置是否循环播放
	//! \param bLoop 为true为循环播放[in]。
	//! \return 。
	void SetLoop(UGbool bLoop);

	//! \brief 获取循环播放标识
	//! \return
	UGbool GetLoop(void) const { return m_bLoop; }

	//! \brief 设置是否自动更新时间
	void SetAutoUpdated(UGbool autoupdate);	

	//! \brief 获得是否自动更新标识
	UGbool IsAutoUpdated(void) const;

	//! \brief 起始时间
	//! \param dStartTime 设置动画起始播放的时间[in]。
	void SetStartTime(UGdouble dStartTime);

	//! \brief 获得起始动画时间
	UGdouble GetStartTime(void);

	//! \brief 终止时间
	//! \param dEndTime 设置动画结束播放的时间[in]。
	void SetEndTime(UGdouble dEndTime);

	//! \brief 获得起始动画时间
	UGdouble GetEndTime(void);
private:
	//! \brief 动画名称
	UGString m_strAnimationName;
	
	//! \brief 动画位置
	UGdouble m_dTimePos;

	//! \brief 动画长度
	UGdouble m_dLength;

	//! \brief 是否可用
	UGbool m_bEnabled;

	//! \brief 是否循环播放
	UGbool m_bLoop;

	//! \brief 是否自动Update
	UGbool m_bAutoUpdate;

	//! \brief 起始时间
	UGdouble m_dStartTime;

	//! \brief 结束时间
	UGdouble m_dEndTime;
};

//! \brief 节点动画的信息，这个节点动画是调用OGRE底层的节点动画
class BASE3D_API UGNodeAnimationInfo
{
public:
	UGNodeAnimationInfo();

	//! \brief 拷贝构造函数
	UGNodeAnimationInfo(const UGNodeAnimationInfo &rhs);

	//! \brief 析构函数
	virtual ~UGNodeAnimationInfo();

	//! \brief 重载=号。
	void operator=(const UGNodeAnimationInfo& rhs);
public:
	//! \brief 获取动画位置
	UGdouble GetTimePosition(void) const;

	//! \brief 设置节点动画位置
	//! \param dTimePos 当前动画的位置[in]。
	void SetTimePosition(UGdouble dTimePos, UGbool bNeedNotifyListener = TRUE);

	//! \brief 获取动画长度
	UGdouble GetLength() const;

	//! \brief 设置动画长度
	void SetLength(UGdouble dLen);

	//! \brief 获得当前动画是否可用的标识
	//! \return 。
	UGbool GetEnabled(void) const;

	//! \brief 设置动画是否可用的标识
	//! \param bEnabled 为true为当前动画可用[in]。
	//! \return 。
	void SetEnabled(UGbool bEnabled);

	//! \brief 设置是否循环播放
	//! \param bLoop 为true为循环播放[in]。
	//! \return 。
	void SetLoop(UGbool bLoop);

	//! \brief 获取循环播放标识
	//! \return
	UGbool GetLoop(void) const { return m_bLoop; }

	//! \brief 设置关键帧
	void SetKeys(UGArray<UGAnimationKey> keys);

	//! \brief 获取关键帧
	UGArray<UGAnimationKey>& GetKeys();

	//! \brief 获取关键帧
	UGArray<UGAnimationKey> GetAnimationKeys() const;

	//! \brief 添加动画状态的监听器
	void AddListener(UGNodeAnimationListener* listener);

	//! \brief 移除动画状态的监听器
	void RemoveListener(UGNodeAnimationListener* listener);

	//! \brief 移除所有动画状态的监听器
	void RemoveAllListener();

	//! \brief 设置节点动画时间位置改变的回调函数
	void SetTimePostionChangedFunc(TimePositionChangedProc pTimePositionChangedFunc,UGlong pHandle);

	//! \brief 设置节点动画结束时的回调函数
	void SetNodeAnimationFinishedFunc(NodeAnimationFinishedProc pAnimationFinishedFunc, UGlong pHandle);

	//! \brief 获取节点动画位置状态
	//! \param vPos 节点动画位置[out]。
	//! \param dHeading 节点动画运动的方向角，与正北夹角[out]。
	//! \param dPitch 节点动画倾斜角，与水平夹角[out]。
	void CalAnimState(UGVector3d &vPos, UGdouble &dHeading, UGdouble &dPitch) const;

	//! \brief 获取/设置当前节点动画的位置
	UGVector3d GetCurrentPos();
	void SetCurrentPos(const UGVector3d& pos);

	//! \brief 获取/设置当前节点动画的的实时缩放，用于固定模型的像素大小
	UGVector3d GetCurrentScale();
	void SetCurrentScale(const UGVector3d& vScale);

	//! \brief 获取/设置原本节点动画的缩放比例，不考虑固定像素的缩放
	UGVector3d GetOriScale();
	void SetOriScale(const UGVector3d& vScale);

	//! \brief 动画结束
	void AnimationFinished();
private:
	//! \brief 计算节点动画轨迹总长度
	//! \param arrPos 节点动画关键帧位置的数组[in]。
	UGdouble CalTrackLength(const UGArray<UGVector3d>* arrPos) const;

	//! \brief 计算相邻两个关键帧位置的heading和tilt
	//! \param startPos 节点动画第一个关键帧位置[in]。
	//! \param endPos 节点动画第二个关键帧位置[in]。
	//! \param heading 节点动画运动方向角[out]。
	//! \param pitch 节点动画运动倾斜角[in]。
	void CalHeadingAndPitch(UGVector3d startPos, UGVector3d endPos, UGdouble &heading, UGdouble &pitch) const;
private:
	//! \brief 动画位置
	UGdouble m_dTimePos;

	//! \brief 动画长度
	UGdouble m_dLength;

	//! \brief 是否可用
	UGbool m_bEnabled;

	//! \brief 是否循环播放
	UGbool m_bLoop;

	//! \brief 动画关键帧
	UGArray<UGAnimationKey> m_arrKeys;

	//! \brief 动画状态监听器
	UGArray<UGNodeAnimationListener*> m_arrListener;

	//! \brief 时间位置改变的回调函数变量。
	TimePositionChangedProc m_pTimePositionChangedFunc;

	//! \brief 节点动画结束的回调函数变量。
	NodeAnimationFinishedProc m_pNodeAnimationFinishedFunc;

	//! \brief 时间位置改变的回调函数句柄。
	UGlong m_pTimePositionChangedHandle;

	//! \brief 时间位置改变的回调函数句柄。
	UGlong m_pNodeAnimationFinishedHandle;

	//! \brief 节点动画当前的位置
	UGVector3d m_vCurrentPos;

	//! \brief 节点动画的实时缩放大小
	UGVector3d m_vCurrentScale;

	//! \brief 节点动画的原本的缩放大小
	UGVector3d m_vOriScale;
};
}
#endif // !defined(AFX_UGANIMATION_H__E89D0670_EB32_4782_8964_7F60F1421BCF__INCLUDED_)

	
	
