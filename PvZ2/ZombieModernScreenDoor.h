#pragma once
#include "Zombie.h"
#include <Reflection/ReflectionBuilder.h>

class ZombieModernScreenDoor : public Zombie
{
public:
	DECLARE_STATIC_RT_CLASS_MEMBERS(ZombieModernScreenDoor)

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieModernScreenDoor, 0xC3AB1C);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(Zombie);
	ZombieModernScreenDoor::buildEventCallbacks(builder, rclass);
	RT_CLASS_BUILD_SYMBOLS_END();
	RT_CLASS_REGISTER_CLASS_AND_STATES_FUNCTION(ZombieModernScreenDoor)

	RT_CLASS_GET_CLASS_FUNCTION(ZombieModernScreenDoor, 0xC36FE4);
	static void buildEventCallbacks(Reflection::CRefManualSymbolBuilder* builder, Reflection::RClass* rtClass);
	static void buildStates();
	static void LostDoorOnEnter(ZombieModernScreenDoor* zombie);
	static void LostDoorOnLoop(ZombieModernScreenDoor* zombie);
	static void LostDoorOnExit(ZombieModernScreenDoor* zombie);
};
