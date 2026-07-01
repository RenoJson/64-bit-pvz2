#pragma once
#include "StandaloneEffect.h"

class PopAnimRig;

class AnimationSequenceEntry
{
public:
	SexyString AnimationLabel;
	int SelectionMethod;
	pvztime_t LoopingDuration;
};

static_assert(sizeof(AnimationSequenceEntry) == 32);
static_assert(offsetof(AnimationSequenceEntry, AnimationLabel) == 0);
static_assert(offsetof(AnimationSequenceEntry, LoopingDuration) == 28);

class AnimationSequence
{
public:
	std::vector<AnimationSequenceEntry> m_animationEntries;
};

static_assert(sizeof(AnimationSequence) == 24);
static_assert(offsetof(AnimationSequence, m_animationEntries) == 0);

class Effect_PopAnim : public StandaloneEffect
{
public:
	RT_CLASS_GET_CLASS_WRAPPER(0x1230EDC);
	Sexy::RtWeakPtr<PopAnimRig> m_rig;
	AnimationSequence m_animSequence;
	int m_animSequenceCurrentIndex;
	pvztime_t m_animSequenceCurrentIndexStartTime;
	float m_xFocusFraction;
	float m_yFocusFraction;
	float m_xScale;
	float m_yScale;
	float m_orientation;
	Sexy::SexyVector2 m_translation;
	char m_pad3[36];

	void PlayAnimation(const SexyString& animName, int selectionMethod);
	void SetFromResource(const SexyString& resourceName);
	void SetFromResource(void* popAnimRes, Sexy::RtClass* rtClass = nullptr);
	int SetFocusFracAndScale(int unkValue);
	void SetPositionAndLayer(const Sexy::SexyVector3& pos, int layer);
};

static_assert(sizeof(Effect_PopAnim) == 280);
static_assert(offsetof(Effect_PopAnim, m_rig) == 176);