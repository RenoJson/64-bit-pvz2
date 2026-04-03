#pragma once
#include "Zombie.h"
#include <Reflection/ReflectionBuilder.h>

class ZombieModernPoleVaulter : public Zombie
{
public:
	static void buildEventCallbacks(Reflection::CRefManualSymbolBuilder* builder, Reflection::RClass* rtClass);
	static void buildStates();
	static void OnSpawn(ZombieModernPoleVaulter* zombie);
	static void WalkOnLoop(ZombieModernPoleVaulter* zombie);
	static void JumpOnExit(ZombieModernPoleVaulter* zombie);
	static void JumpOnEnter(ZombieModernPoleVaulter* zombie);
	static void JumpOnLoop(ZombieModernPoleVaulter* zombie);
	static void JumpOnExit(ZombieModernPoleVaulter* zombie);
	DECLARE_STATIC_RT_CLASS_MEMBERS(ZombieModernPoleVaulter)

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieModernPoleVaulter, 0xC3AB1C);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(Zombie);
	buildEventCallbacks(builder, rclass);
	RT_CLASS_BUILD_SYMBOLS_END();
	RT_CLASS_REGISTER_CLASS_AND_STATES_FUNCTION(ZombieModernPoleVaulter);

	RT_CLASS_GET_CLASS_FUNCTION(ZombieModernPoleVaulter, 0xC36FE4);

};
