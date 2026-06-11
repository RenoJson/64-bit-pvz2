#pragma once
#include "PvZ2/Zombie.h"
#include "Reflection/CRefManualSymbolBuilder.h"
#include <Reflection/ReflectionBuilder.h>
#include <pch.h>
#include <SexyTypes.h>
#include <jni.h>
using namespace Sexy;

class ZombieBullProps : public ZombiePropertySheet
{
public:
	Rect BuckRect = {-95, 10, 40, 125};
	float BullRunSpeed = 1.0f;
	float BullPawLocation = 810.0f;
	float BullEnterSpeed = 0.30f;
	float BullRunAgainDelay = 3.0f;
	float BullRammingDamage = 1500.0f;
	SexyString RiderType = "west_bullrider";
	std::vector<SexyString> RiderLayersToHide;
	float LaunchDistance = 256.0f;
	float LaunchHeight = 250.0f;
	float LaunchAirTime = 0.75f;
	SexyVector3 RiderSpawnOffset = { 60.0f, 0.0f, 50.0f };
	bool isCavalry = false;

	static Reflection::CRefManualSymbolBuilder::BuildSymbolsFunc oZombieBullPropsBuildSymbols;
	static void modInit();

	static void* construct()
	{
		auto* props = new ZombieBullProps();

		typedef void* (*ctorWithThisPtr)(void*);
		ctorWithThisPtr baseCtor = (ctorWithThisPtr)getActualOffset(0xC136A4);
		baseCtor(props);

		*reinterpret_cast<uintptr_t*>(props) = getActualOffset(0x2432FA8);
		return props;
	}

	static void buildSymbols(Reflection::CRefManualSymbolBuilder* builder, Reflection::RClass* rclass)
	{
		oZombieBullPropsBuildSymbols(builder, rclass);
		RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieBullProps, BullRunAgainDelay);
		RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieBullProps, BullRammingDamage);
		RT_CLASS_REGISTER_STRING_PROPERTY(ZombieBullProps, RiderType);
		RT_CLASS_REGISTER_STRING_VECTOR_PROPERTY(ZombieBullProps, RiderLayersToHide);
		RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieBullProps, LaunchDistance);
		RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieBullProps, LaunchHeight);
		RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieBullProps, LaunchAirTime);
		RT_CLASS_REGISTER_CLASS_PROPERTY(ZombieBullProps, SexyVector3, RiderSpawnOffset);
		RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieBullProps, isCavalry);
	};
};





