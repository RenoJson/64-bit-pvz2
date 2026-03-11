#pragma once
#include <Sexy/DelegateBase.h>
enum ZState
{
    ZS_Idle = 0,
    ZS_Walk = 1,
    ZS_Eat = 2,
    ZS_BleedingOut = 3,
    ZS_Die = 4,
    ZS_Electrocute = 5,
    ZS_Ash = 6,
    ZS_MowedDown = 7,
    ZS_FlickedOff = 8,
    ZS_RiseFromGround = 9,
    ZS_RiseFromStorm = 10,
    ZS_StormEntrance = 11,
    ZS_RiseFromPod = 12,
    ZS_Winning = 13,
    ZS_Plantify = 14,
    ZS_Grabbed = 15,
};
class ZombieState
{
public:

	uint64_t m_id;
	Sexy::DelegateBase m_onEnter;
	Sexy::DelegateBase m_onLoop;
	Sexy::DelegateBase m_onExit;
	SexyString m_name;
};
static_assert(sizeof(ZombieState) == 176);
static_assert(offsetof(ZombieState, m_id) == 0);
static_assert(offsetof(ZombieState, m_onEnter) == 8);
static_assert(offsetof(ZombieState, m_onLoop) == 56);
static_assert(offsetof(ZombieState, m_onExit) == 104);
static_assert(offsetof(ZombieState, m_name) == 152);
