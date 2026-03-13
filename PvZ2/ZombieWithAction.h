#pragma once
#include "Zombie.h"
#include <Reflection/ReflectionBuilder.h>
using namespace Sexy;
enum ZombieActionPhase {
	Zombie_Action_Phase_None = 0,
	Zombie_Action_Phase_Entering = 1
};
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
static_assert(offsetof(ZombieWithActions, m_currentPhase) == 1152);
class ZombieBeachOctopus: public ZombieWithActions{
public:
	float m_nextCastTime;
};
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