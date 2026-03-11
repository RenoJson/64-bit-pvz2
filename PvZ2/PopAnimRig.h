#pragma once
#include "GameObject.h"
#include "ZombieEvent.h"
#include <Sexy/PopAnim.h>
#include <Sexy/DelegateBase.h>
#include <Sexy/PopAnimListener.h>
#include <Sexy/RtReflectionDelegateBase.h>

namespace Sexy {
	class Graphics;
}

class PopAnimRig : public GameObject, public Sexy::PopAnimListener
{
public:
	float m_saturation;
	char pad_001[4];
	Sexy::PopAnim* m_popAnim;
	Sexy::RtWeakPtr<PopAnimRig> m_selfPtr;
	int m_activeAnim;
	char pad_002[4];
	SexyString m_activeAnimBaseLabel;
	int m_activeAnimPlayStyle;
	int m_activeAnimSeqEndCount;
	std::vector<int> m_animRandomDistribution;
	int m_activeAnimSelectMethod;
	int m_activeAnimLastPlayedVariation;
	Sexy::RtWeakPtr<RtObject> m_audioObject;
	bool m_manageDeletion;
	bool m_mirrored;
	bool m_disabled;
	bool m_useShaderOverrideColor;
	char m_pad2[4];
	Sexy::Color m_shaderOverrideColor;
	Sexy::Color unkColor;
	Sexy::DelegateBase m_delegateOnStop;
	Sexy::DelegateBase m_delegateOnLoop;
	Sexy::DelegateBase m_delegateOnCommand;
	ZombieEvent m_serialOnAnimStopped;
	ZombieEvent m_serialOnLoopingAnimContinued;
	ZombieEvent m_serialOnPopAnimCommand;
	bool m_registeredInAudioMgr;
	char m_pad4[11];
	int m_state;

	void TransformAndRender(Sexy::Graphics* graphics, Sexy::SexyTransform2D& trans)
	{
		typedef void (*func)(PopAnimRig*, Sexy::Graphics*, Sexy::SexyTransform2D*);
		((func)getActualOffset(0x668EE8))(this, graphics, &trans);
	}

	void Render(Sexy::Graphics* graphics) {
		typedef void (*func)(PopAnimRig*, Sexy::Graphics*);
		((func)getActualOffset(0x6686B0))(this, graphics);
	}

	Sexy::Color GetColor() {
		return this->m_popAnim->mColor;
	}

	void SetColor(Sexy::Color& color) {
		this->m_popAnim->mColor = color;
	}

	virtual int SetActiveAnim(SexyString* animName, int a3, uint unk_stateNum, int a5) {}
	virtual int SetAnimModifier(float multiplier) {}
	virtual int Function16() {}
	virtual int Function17() {}
	virtual int Function18() {}
	virtual int Function19() {}
	virtual int Function20() {}
	virtual int Function21() {}
	virtual int Function22() {}
	virtual int Function23() {}
	virtual int Function24() {}
	virtual int Function25() {}
	virtual int Function26() {}
	virtual int Function27() {}
	virtual int Function28() {}
	virtual int DamageFlash() {}
	virtual int GetUserShaderOverrideColor() {}
	virtual int Function31() {}

};

static_assert(sizeof(PopAnimRig) == 592);
static_assert(offsetof(PopAnimRig, m_saturation) == 24);
static_assert(offsetof(PopAnimRig, m_selfPtr) == 40);
static_assert(offsetof(PopAnimRig, m_activeAnim) == 48);
static_assert(offsetof(PopAnimRig, m_activeAnimSelectMethod) == 112);
static_assert(offsetof(PopAnimRig, m_audioObject) == 120);
static_assert(offsetof(PopAnimRig, m_manageDeletion) == 128);
static_assert(offsetof(PopAnimRig, m_mirrored) == 129);
static_assert(offsetof(PopAnimRig, m_shaderOverrideColor) == 136);
static_assert(offsetof(PopAnimRig, m_serialOnAnimStopped) == 312);
static_assert(offsetof(PopAnimRig, m_serialOnLoopingAnimContinued) == 400);
static_assert(offsetof(PopAnimRig, m_serialOnPopAnimCommand) == 488);
static_assert(offsetof(PopAnimRig, m_registeredInAudioMgr) == 576);
static_assert(offsetof(PopAnimRig, m_state) == 588);