#pragma once
#include "Zombie.h"
#include <Reflection/ReflectionBuilder.h>
using namespace Sexy;
class ZombieWithActions : public Zombie {
public:
	int m_queueEmptyState;
	int m_queuedPhaseAfterAction;
	int m_actionRepeatsLeft;
	char pad1[4];
	RtWeakPtr<RtObject> m_lastAction;
	RtWeakPtr<RtObject> m_currentAction;
	std::vector<RtWeakPtr<RtObject>> m_actionQueue;
	int m_currentPhase;
};
static_assert(sizeof(ZombieWithActions) == 1160);
static_assert(offsetof(ZombieWithActions, m_queueEmptyState) == 1096);
static_assert(offsetof(ZombieWithActions, m_queuedPhaseAfterAction) == 1100);
static_assert(offsetof(ZombieWithActions, m_actionRepeatsLeft) == 1104);
static_assert(offsetof(ZombieWithActions, m_lastAction) == 1112);
static_assert(offsetof(ZombieWithActions, m_currentAction) == 1120);
static_assert(offsetof(ZombieWithActions, m_actionQueue) == 1128);

class ZombieZombossMech : public ZombieWithActions {
public:
	RT_CLASS_GET_CLASS_WRAPPER(0xB5905C);
	float m_scheduledStateEndTime;
	int m_currentStageIndex;
	char pad2[4];
	char m_movementCurve[48];
	char m_effectCurves[48];
	char m_colorCurve[24];
	float m_zombossScaledTime;
	bool m_dyingAnimDone;
	bool m_exitAnimDone;
	bool m_playingStunStart;
	bool m_playingStunEnd;
	bool m_takenStunAttack;
	bool m_haveShownHalfDamagePhase;
	bool m_cheatKillZombie;
	bool m_slideWithStreetView;
	bool m_canShowDamageEffects;
	Sexy::RtWeakPtr<Sexy::RtObject> m_zombieDamageSparks;
	int m_droppedDebrisCount;
	std::vector<void*> m_lootForSpawnedZombies; //Loot
};

class ZombieWithActionsProps : public ZombiePropertySheet
{
public:
	std::vector<Sexy::RtWeakPtr<Sexy::RtObject>> Actions;
	char pad3[4];
};
class ZombieBeachOctopus: public ZombieWithActions{
public:
	float m_nextCastTime;
};

//static_assert(sizeof(ZombieBeachOctopus) == 1168);
//static_assert(offsetof(ZombieBeachOctopus, m_nextCastTime) == 1160);
class ZombieFairyTaleWitch :public ZombieBeachOctopus {
public:
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieFairyTaleWitch, 0xC3AB1C);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombieBeachOctopus);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieFairyTaleWitch);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieFairyTaleWitch, 0xACB9F4);
};