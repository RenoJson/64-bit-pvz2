#pragma once
#include "ZombieAnimRig.h"
#include <Reflection/ReflectionBuilder.h>
#include "Zombie.h"

class ZombieAnimRig_Basic : public ZombieAnimRig
{
public:
    ZombieFlagType m_flagType;
    HelmType m_helmType;
    int32_t m_helmDamageIndex;
    float m_helmFlashStart;
};
static_assert(sizeof(ZombieAnimRig_Basic) == 680);
static_assert(offsetof(ZombieAnimRig_Basic, m_flagType) == 664);
static_assert(offsetof(ZombieAnimRig_Basic, m_helmType) == 668);
static_assert(offsetof(ZombieAnimRig_Basic, m_helmDamageIndex) == 672);
static_assert(offsetof(ZombieAnimRig_Basic, m_helmFlashStart) == 676);

class ZombieAnimRig_DinoBasic : public ZombieAnimRig_Basic
{
public:

};

class ZombieAnimRig_BeachBasic : public ZombieAnimRig_Basic
{
public:

};

class ZombieAnimRig_DinoBully : public ZombieAnimRig_DinoBasic
{
public:

};

class ZombieAnimRig_JourneyToTheWestGargantuar : public ZombieAnimRig_DinoBully
{
public:
    static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieAnimRig_JourneyToTheWestGargantuar, 0x9D9C4C);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombieAnimRig_DinoBully);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieAnimRig_JourneyToTheWestGargantuar);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieAnimRig_JourneyToTheWestGargantuar, 0xB0AE78);
};