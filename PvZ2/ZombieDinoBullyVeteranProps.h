#pragma once
#include "PvZ2/Zombie.h"
#include <Reflection/ReflectionBuilder.h>

class ZombieDinoBullyVeteranProps : public ZombiePropertySheet
{
public:
	float SmashDuration;
	int64_t SmashDamage;
	SexyString SoundOnSmash;
	float CraterLifeTime;
};

class ZombieJourneyToTheWestGargantuarProps : public ZombieDinoBullyVeteranProps
{
public:
	float DamageReduction = 50.0f;
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieJourneyToTheWestGargantuarProps, 0xC136A4);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombieDinoBullyVeteranProps);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieJourneyToTheWestGargantuarProps, DamageReduction);
		RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieJourneyToTheWestGargantuarProps);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieJourneyToTheWestGargantuarProps, 0xB0C06C);
};