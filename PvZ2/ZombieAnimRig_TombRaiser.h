#pragma once
#include "ZombieAnimRig.h"
#include <Reflection/ReflectionBuilder.h>
#include <Sexy/RtReflectionDelegateBase.h>
class ZombieAnimRig_TombRaiser : public ZombieAnimRig
{
public:
	RtReflectionDelegateBase m_onAnimDone;
	char pad[56];
	RtReflectionDelegateBase m_onThrow;
	char pad1[56];
};
static_assert(sizeof(ZombieAnimRig_TombRaiser) == 840);
static_assert(offsetof(ZombieAnimRig_TombRaiser, m_onAnimDone) == 664);
static_assert(offsetof(ZombieAnimRig_TombRaiser, m_onThrow) == 752);
class ZombieAnimRig_EnergyDrinker : public ZombieAnimRig_TombRaiser
{
public:
	bool m_gotCondition = false;
	SexyString m_conditionWalkAnim;
	SexyString m_conditionDieAnim;
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieAnimRig_EnergyDrinker, 0x9D9C4C);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombieAnimRig_TombRaiser);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieAnimRig_EnergyDrinker, m_gotCondition);
	RT_CLASS_REGISTER_STRING_PROPERTY(ZombieAnimRig_EnergyDrinker, m_conditionWalkAnim);
	RT_CLASS_REGISTER_STRING_PROPERTY(ZombieAnimRig_EnergyDrinker, m_conditionDieAnim);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieAnimRig_EnergyDrinker);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieAnimRig_EnergyDrinker, 0xB24698);
};