#pragma once
#include "Zombie.h"
#include <Reflection/ReflectionBuilder.h>

class ZombieJourneyToTheWestVendor : public Zombie
{
public:
	enum ZVendor {
		ZV_SetPig = 16,
	};
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
	buildEventCallbacks(builder, rclass);
	RT_CLASS_BUILD_SYMBOLS_END();
	static void RegisterClass() {
		void* v0 = CallFunc<void*>(0x1624024); if (v0) {
			auto* builder = (Reflection::CRefManualSymbolBuilder*)CallFunc<void*, void*>(0x162413C, v0); if (builder) {
				(builder)->RegisterClassWithProperties("ZombieJourneyToTheWestVendor", ZombieJourneyToTheWestVendor::BuildSymbols, sizeof(ZombieJourneyToTheWestVendor), 0);;
			}
		} 
		buildStates();
	};
	
	RT_CLASS_GET_CLASS_FUNCTION(ZombieJourneyToTheWestVendor, 0xC36FE4);
	
};
