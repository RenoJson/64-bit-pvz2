#pragma once
#include "Zombie_BullProps.h"

class ZombieBullVeteranProps : public ZombieBullProps
{
public:

	static Reflection::CRefManualSymbolBuilder::BuildSymbolsFunc oZombieBullVeteranPropsBuildSymbols;
	static void modInit();

	static void* construct()
	{
		auto* props = new ZombieBullVeteranProps();

		typedef void* (*ctorWithThisPtr)(void*);
		ctorWithThisPtr baseCtor = (ctorWithThisPtr)getActualOffset(0xC136A4);
		baseCtor(props);

		*reinterpret_cast<uintptr_t*>(props) = getActualOffset(0x2433048);
		return props;
	}

	static void buildSymbols(Reflection::CRefManualSymbolBuilder* builder, Reflection::RClass* rclass)
	{
		RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieBullVeteranProps, BullRunAgainDelay);
		RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieBullVeteranProps, BullRammingDamage);
		RT_CLASS_REGISTER_STRING_PROPERTY(ZombieBullVeteranProps, RiderType);
		RT_CLASS_REGISTER_STRING_VECTOR_PROPERTY(ZombieBullVeteranProps, RiderLayersToHide);
		RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieBullVeteranProps, LaunchDistance);
		RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieBullVeteranProps, LaunchHeight);
		RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieBullVeteranProps, LaunchAirTime);
		RT_CLASS_REGISTER_CLASS_PROPERTY(ZombieBullVeteranProps, SexyVector3, RiderSpawnOffset);
		RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieBullVeteranProps, isCavalry);
	};
}; 

