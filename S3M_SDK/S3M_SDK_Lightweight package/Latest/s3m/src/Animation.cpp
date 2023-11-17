#include "Animation.h"

S3MB::AnimationKey::AnimationKey()
{
	m_vecScale = Vector3d(1, 1, 1);
	m_Rotation = Quaternion::ZERO;
	m_vecTranslate = Vector3d(0, 0, 0);
	m_dWeights = 0.0;
}

void S3MB::AnimationKey::operator=(const AnimationKey& other)
{
	if (this == &other)
	{
		return;
	}

	m_fTimePos = other.m_fTimePos;
	m_vecScale = other.m_vecScale;
	m_Rotation = other.m_Rotation;
	m_vecTranslate = other.m_vecTranslate;
	m_dWeights = other.m_dWeights;
}

S3MB::AnimationCurve::~AnimationCurve()
{
	for (int i = 0; i < m_vecKeys.size(); ++i)
	{
		delete m_vecKeys[i];
	}
	m_vecKeys.clear();
}

S3MB::AnimationKey* S3MB::AnimationCurve::AddKey()
{
	AnimationKey* key = new AnimationKey;
	m_vecKeys.push_back(key);
	return key;
}

S3MB::Animation::Animation()
	:m_strAnimationName(L"")
	, m_dTimePos(0)
	, m_dLength(0)
	, m_dStartTime(0)
	, m_dEndTime(0)
    , m_bEnabled(true){}

S3MB::Animation::Animation(const wstring& strAnimName, double dTimePos, double dLength, bool bEnabled)
	: m_strAnimationName(strAnimName)
	, m_dTimePos(dTimePos)
	, m_dLength(dLength)
	, m_dStartTime(0)
	, m_dEndTime(dLength)
    , m_bEnabled(bEnabled){}

S3MB::Animation::Animation(const Animation &rhs)
	: m_strAnimationName(rhs.m_strAnimationName)
	, m_dTimePos(rhs.m_dTimePos)
	, m_dLength(rhs.m_dLength)
	, m_dStartTime(rhs.m_dStartTime)
	, m_dEndTime(rhs.m_dEndTime)
    , m_bEnabled(rhs.m_bEnabled){}
