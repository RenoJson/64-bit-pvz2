#pragma once
#include "PvZ2/Zombie.h"
#include "Reflection/CRefManualSymbolBuilder.h"
#include <Reflection/ReflectionBuilder.h>
#include <pch.h>
#include <SexyTypes.h>
using namespace Sexy;

class ZombieZcorpRacerProps : public ZombiePropertySheet
{
public:
	Rect RacerRect = { 10, 10, 40, 80 };
	float RacerSpeed = 1.0f;
	float LaunchZombieDistance = 2.5f;
	float LaunchZombieApex = 75.0f;
	float LaunchZombieTimeToLand = 2.5f;
	float ChairCollideDamage = 5000.0f;	
	SexyString RacerType = "zcorp_racer";

	static Reflection::CRefManualSymbolBuilder::BuildSymbolsFunc oZombieZcorpRacerPropsBuildSymbols;
	static void modInit();

	static void* construct()
	{
		auto* props = new ZombieZcorpRacerProps();

		typedef void* (*ctorWithThisPtr)(void*);
		ctorWithThisPtr baseCtor = (ctorWithThisPtr)getActualOffset(0xC136A4);
		baseCtor(props);

		*reinterpret_cast<uintptr_t*>(props) = getActualOffset(0x2414AC8);
		return props;
	}

	static void buildSymbols(Reflection::CRefManualSymbolBuilder* builder, Reflection::RClass* rclass)
	{
		oZombieZcorpRacerPropsBuildSymbols(builder, rclass);
		RT_CLASS_REGISTER_STRING_PROPERTY(ZombieZcorpRacerProps, RacerType);
	};
};




