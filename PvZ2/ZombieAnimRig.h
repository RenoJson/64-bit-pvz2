#pragma once
#include "PopAnimRig.h"

class ZombieType;

class ZombieAnimRig : public PopAnimRig
{
public:
	SexyString m_groundTrackName;
	float m_lastGroundTrackOffset;
	float m_desiredVelocity;
	float m_animRateModifier;
	float m_needsToDieRequestedTime;
	bool m_readyToDie;
	char m_pad5[4];
	ZombieType* m_zombieType;
	char m_pad6[16];

	virtual void LoopIdleWithCallback(ZombieEvent*) {}
	virtual void LoopWalkWithCallback(ZombieEvent*) {}
	virtual void Function34() {};
	virtual void Function35() {};
	virtual void Function36() {};
	virtual void Function37() {};
	virtual void Function38() {};
	virtual void PlayAnimWithCallback(SexyString* animName, int a3, ZombieEvent* a4) {};
	virtual void DropHead() {};
	virtual void Function41() {};
	virtual void Function42() {};
	virtual void Function43() {};
	virtual void Function44() {};
	virtual void DropArm() {};
	virtual void Function46() {};
	virtual void Function47() {};
	virtual void Function48() {};
	virtual void Function49() {};
	virtual void Function50() {};
	virtual void Function51() {};
	virtual void Function52() {};
	virtual void Function53() {};
	virtual void Function54() {};
	virtual void GetHeadLayerToDrop() {};
	virtual void GetArmLayerToDrop() {};
	virtual void Function57() {};
	virtual void Function58() {};
	virtual void Function59() {};
	virtual void Function60() {};
	virtual void Function61() {};
	virtual void Function62() {};
	virtual void Function63() {};
	virtual void Function64() {};
	virtual void Function65() {};
	virtual void Function66() {};
};

static_assert(sizeof(ZombieAnimRig) == 664);
static_assert(offsetof(ZombieAnimRig, m_groundTrackName) == 592);
static_assert(offsetof(ZombieAnimRig, m_zombieType) == 640);