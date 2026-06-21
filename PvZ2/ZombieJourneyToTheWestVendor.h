#pragma once
#include "Zombie.h"
#include <Reflection/ReflectionBuilder.h>

class ZombieJourneyToTheWestVendor : public Zombie
{
public:
	static void buildEventCallbacks(Reflection::CRefManualSymbolBuilder* builder, Reflection::RClass* rtClass);
	static void buildStates();
	static void PigOnEnter(ZombieJourneyToTheWestVendor* zombie);
    static void PigOnLoop(ZombieJourneyToTheWestVendor* zombie);
    static void PigOnExit(ZombieJourneyToTheWestVendor* zombie);
	pvztime_t m_nextPigTime;
	float m_speedModifier;
	bool m_firstSpawned = false;
	DECLARE_STATIC_RT_CLASS_MEMBERS(ZombieJourneyToTheWestVendor)

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieJourneyToTheWestVendor, 0xC3AB1C);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(Zombie);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieJourneyToTheWestVendor, m_nextPigTime);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieJourneyToTheWestVendor, m_speedModifier);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieJourneyToTheWestVendor, m_firstSpawned);
	ZombieJourneyToTheWestVendor::buildEventCallbacks(builder, rclass);
	RT_CLASS_BUILD_SYMBOLS_END();
	RT_CLASS_REGISTER_CLASS_AND_STATES_FUNCTION(ZombieJourneyToTheWestVendor)
	
	RT_CLASS_GET_CLASS_FUNCTION(ZombieJourneyToTheWestVendor, 0xC36FE4);
	
};
