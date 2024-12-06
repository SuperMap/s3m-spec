#pragma once
#if !defined(S3MB_ANIMATION)
#define S3MB_ANIMATION
#include "Quaternion.h"
#include <string>
#include <vector>
using namespace std;
namespace S3MB 
{
	class S3MB_API AnimationKey
	{
	public:
		AnimationKey();

		virtual ~AnimationKey() {}

		// 重载等号。
		void operator=(const AnimationKey& other);

		// 设置/获取当前帧的时间
		void SetTime(float fTimePos) { m_fTimePos = fTimePos; }
		double GetTime(void) const { return m_fTimePos; }

		// 设置/获取当前帧的缩放比
		// vecScale 当前帧的缩放比[in]。
		void SetScale(const Vector3d& vecScale) { m_vecScale = vecScale; };
		const Vector3d& GetScale(void) const { return m_vecScale; };

		// 设置/获取当前帧的旋转角度
		// rot 当前帧的旋转角度[in]。
		void SetRotation(const Quaternion& rot) { m_Rotation = rot; };
		const Quaternion& GetRotation(void) const { return m_Rotation; };

		// 设置/获取当前帧的平移
		// vecTrans 当前动画的位置[in]。
		void SetTranslate(const Vector3d& vecTrans) { m_vecTranslate = vecTrans; };
		const Vector3d& GetTranslate(void) const { return m_vecTranslate; };

		// 设置/获取当前帧的权重
		void SetWeights(const double& dWeights) { m_dWeights = dWeights; };
		const double& GetWeights() const { return m_dWeights; };

	private:
		// 当前帧的旋转角度
		Quaternion m_Rotation;

		// 当前帧的平移
		Vector3d m_vecTranslate;

		// 当前帧的缩放比
		Vector3d m_vecScale;

		// 当前帧的权重
		double m_dWeights;

		// 当前帧的时间
		float m_fTimePos;
	};

	class S3MB_API AnimationCurve
	{
	public:
		AnimationCurve() {};
		~AnimationCurve();

		// 设置/获取动画变换的类型
		// scale表示缩放、rotation表示旋转、translation表示平移、weights表示权重
		void SetPath(const wstring& strPath) { m_strPath = strPath; };
		const wstring& GetPath() const { return m_strPath; };

		// 设置关键帧数。
		std::vector<AnimationKey*>& GetKeys() { return m_vecKeys; };

		// 增加关键帧数。
		AnimationKey* AddKey();

	private:
		// 动画变换的类型
		wstring m_strPath;

		// 动画信息
		std::vector<AnimationKey*> m_vecKeys;
	};

	class S3MB_API Animation
	{
	public:
		Animation();
		Animation(const wstring& strAnimName, double dTimePos, double dLength, bool bEnabled = false);
		// 拷贝构造函数
		Animation(const Animation &rhs);

		// 设置/获取动画名称
		void SetAnimationName(wstring& strName) { m_strAnimationName = strName; };
		const wstring& GetAnimationName() const { return m_strAnimationName; };

		// 设置/获取动画位置
		void SetTimePosition(double dTimePos) { m_dTimePos = dTimePos; };
		double GetTimePosition(void) const { return m_dTimePos; };

		// 设置/获取动画长度
		void SetLength(double dLen) { m_dLength = dLen; };
		double GetLength() const { return m_dLength; };

		// 设置/获取起始时间
		void SetStartTime(double dStartTime) { m_dStartTime = dStartTime; };
		double GetStartTime(void) const { return m_dStartTime; };

		// 设置/获取终止时间
		void SetEndTime(double dEndTime) { m_dEndTime = dEndTime; };
		double GetEndTime(void) const { return m_dEndTime; };

		// 设置/获得当前动画是否可用的标识
		void Enabled(bool bEnabled) { m_bEnabled = bEnabled; };
		bool GetEnabled(void) const { return m_bEnabled; };

	private:
		// 动画名称
		wstring m_strAnimationName;

		// 动画位置
		double m_dTimePos;

		// 动画长度
		double m_dLength;

		// 起始时间
		double m_dStartTime;

		// 结束时间
		double m_dEndTime;

		// 是否可用
		bool m_bEnabled;
	};

}

#endif