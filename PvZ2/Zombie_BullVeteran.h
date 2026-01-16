#pragma once
#include "Zombie_BullProps.h"

class ZombieBullVeteranProps : public ZombieBullProps
{
public:
	float BullRunAgainDelay = 3.0f;
	float BullRammingDamage = 1500.0f;
	SexyString VetRiderType = "west_bullrider";

	static Reflection::CRefManualSymbolBuilder::BuildSymbolsFunc oZombieBullVeteranPropsBuildSymbols;
	static void modInit();

	static void* construct()
	{
		auto* props = new ZombieBullVeteranProps();

		typedef void* (*ctorWithThisPtr)(void*);
		ctorWithThisPtr baseCtor = (ctorWithThisPtr)getActualOffset(0xC136A4);
		baseCtor(props);

		*reinterpret_cast<int*>(uintptr_t(props)) = getActualOffset(0x2433048);
		return props;
	}

	static void buildSymbols(Reflection::CRefManualSymbolBuilder* builder, Reflection::RClass* rclass)
	{
		oZombieBullVeteranPropsBuildSymbols(builder, rclass);
		RT_CLASS_REGISTER_STRING_PROPERTY(ZombieBullVeteranProps, VetRiderType);
		RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieBullVeteranProps, LaunchDistance);
		RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieBullVeteranProps, LaunchHeight);
		RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieBullVeteranProps, LaunchAirTime);
		RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieBullVeteranProps, LayerListType);
		RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieBullVeteranProps, isCavalry);
	};
}; 

