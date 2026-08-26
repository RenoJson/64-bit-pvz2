#pragma once
#include "Zombie.h"
#include <Reflection/ReflectionBuilder.h>

class ZombieModernPoleVaulter : public Zombie
{
public:
	DECLARE_STATIC_RT_CLASS_MEMBERS(ZombieModernPoleVaulter)

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieModernPoleVaulter, 0xC3AB1C);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(Zombie);
	ZombieModernPoleVaulter::buildEventCallbacks(builder, rclass);
	RT_CLASS_BUILD_SYMBOLS_END();
	RT_CLASS_REGISTER_CLASS_AND_STATES_FUNCTION(ZombieModernPoleVaulter)

	RT_CLASS_GET_CLASS_FUNCTION(ZombieModernPoleVaulter, 0xC36FE4);
	static void buildEventCallbacks(Reflection::CRefManualSymbolBuilder* builder, Reflection::RClass* rtClass);
	static void buildStates();
	static void JumpOnEnter(ZombieModernPoleVaulter* zombie);
	static void JumpOnLoop(ZombieModernPoleVaulter* zombie);
	static void JumpOnExit(ZombieModernPoleVaulter* zombie);
	static void BonkOnEnter(ZombieModernPoleVaulter* zombie);
	static void BonkOnLoop(ZombieModernPoleVaulter* zombie);
	static void BonkOnExit(ZombieModernPoleVaulter* zombie);
	static void FarJumpOnEnter(ZombieModernPoleVaulter* zombie);
	static void FarJumpOnLoop(ZombieModernPoleVaulter* zombie);
	static void FarJumpOnExit(ZombieModernPoleVaulter* zombie);
	static void ThrowOnEnter(ZombieModernPoleVaulter* zombie);
	static void ThrowOnLoop(ZombieModernPoleVaulter* zombie);
	static void ThrowOnExit(ZombieModernPoleVaulter* zombie);
};
