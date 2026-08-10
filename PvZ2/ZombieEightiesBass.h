#pragma once
#include "Zombie.h"
#include "GridItem.h"
#include <Reflection/ReflectionBuilder.h>

class ZombieEightiesBass : public Zombie
{
public:
	bool m_isJamming = false;
	RtWeakPtr<Sexy::RtObject> m_speaker;
	DECLARE_STATIC_RT_CLASS_MEMBERS(ZombieEightiesBass)

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieEightiesBass, 0xC3AB1C);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(Zombie);
	ZombieEightiesBass::buildEventCallbacks(builder, rclass);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieEightiesBass, m_isJamming);
	RT_CLASS_BUILD_SYMBOLS_END();
	RT_CLASS_REGISTER_CLASS_AND_STATES_FUNCTION(ZombieEightiesBass)
	RT_CLASS_GET_CLASS_FUNCTION(ZombieEightiesBass, 0xC36FE4);
	static void buildEventCallbacks(Reflection::CRefManualSymbolBuilder* builder, Reflection::RClass* rtClass);
	static void buildStates();
	static void GuitarIdleOnEnter(ZombieEightiesBass* zombie);
	static void GuitarIdleOnLoop(ZombieEightiesBass* zombie);
	static void GuitarIdleOnExit(ZombieEightiesBass* zombie);
	static void GuitarBreakOnEnter(ZombieEightiesBass* zombie);
	static void GuitarBreakOnLoop(ZombieEightiesBass* zombie);
	static void GuitarBreakOnExit(ZombieEightiesBass* zombie);
	static void GrandDebutOnEnter(ZombieEightiesBass* zombie);
	static void GrandDebutOnLoop(ZombieEightiesBass* zombie);
	static void GrandDebutOnExit(ZombieEightiesBass* zombie);
};
