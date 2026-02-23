#pragma once
#include "Zombie.h"
#include <Reflection/ReflectionBuilder.h>

class ZombieJourneyToTheWestVendor : public Zombie
{
public:
	enum ZVendor {
		ZV_SetPig = 17,
	};
	pvztime_t m_pigTime = 6.0f;
	float m_speedModifier;
	DECLARE_STATIC_RT_CLASS_MEMBERS(ZombieJourneyToTheWestVendor)

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieJourneyToTheWestVendor, 0xC3AB1C);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(Zombie);
	ZombieJourneyToTheWestVendor::buildEventCallbacks(builder, rclass);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieJourneyToTheWestVendor, m_pigTime);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieJourneyToTheWestVendor, m_speedModifier);
	RT_CLASS_BUILD_SYMBOLS_END();
	RT_CLASS_REGISTER_CLASS_AND_STATES_FUNCTION(ZombieJourneyToTheWestVendor);
	
	RT_CLASS_GET_CLASS_FUNCTION(ZombieJourneyToTheWestVendor, 0xC36FE4);
	static void buildEventCallbacks(Reflection::CRefManualSymbolBuilder* builder, Reflection::RClass* rtClass);
	static void buildStates();
};
