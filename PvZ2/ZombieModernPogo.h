#pragma once
#include "Zombie.h"
#include <Reflection/ReflectionBuilder.h>

class ZombieModernPogo : public Zombie
{
public:
	DECLARE_STATIC_RT_CLASS_MEMBERS(ZombieModernPogo)

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieModernPogo, 0xC3AB1C);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(Zombie);
	ZombieModernPogo::buildEventCallbacks(builder, rclass);
	RT_CLASS_BUILD_SYMBOLS_END();
	RT_CLASS_REGISTER_CLASS_AND_STATES_FUNCTION(ZombieModernPogo)

	RT_CLASS_GET_CLASS_FUNCTION(ZombieModernPogo, 0xC36FE4);
	static void buildEventCallbacks(Reflection::CRefManualSymbolBuilder* builder, Reflection::RClass* rtClass);
	static void buildStates();
	static void JumpOnEnter(ZombieModernPogo* zombie);
	static void JumpOnLoop(ZombieModernPogo* zombie);
	static void JumpOnExit(ZombieModernPogo* zombie);
	static void BonkOnEnter(ZombieModernPogo* zombie);
	static void BonkOnLoop(ZombieModernPogo* zombie);
	static void BonkOnExit(ZombieModernPogo* zombie);
	static void LostPogoOnEnter(ZombieModernPogo* zombie);
	static void LostPogoOnLoop(ZombieModernPogo* zombie);
	static void LostPogoOnExit(ZombieModernPogo* zombie);
	static void WalkPogoOnEnter(ZombieModernPogo* zombie);
	static void WalkPogoOnLoop(ZombieModernPogo* zombie);
	static void WalkPogoOnExit(ZombieModernPogo* zombie);
};
