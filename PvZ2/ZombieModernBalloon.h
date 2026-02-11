#pragma once
#include "PvZ2/Zombie.h"
#include "Reflection/CRefManualSymbolBuilder.h"
#include <Reflection/ReflectionBuilder.h>

class ZombieModernBalloon : public Zombie
{
public:
	static void modInit();
};

class ZombieBigHeadBalloon : public ZombieModernBalloon
{
public:

};

class ZombieJourneyToTheWestBalloon : public ZombieModernBalloon
{
public:
	bool m_hasTakenCatastrophicDamage = false;
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieJourneyToTheWestBalloon, 0xC3AB1C);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombieModernBalloon);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieJourneyToTheWestBalloon, m_hasTakenCatastrophicDamage);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieJourneyToTheWestBalloon);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieJourneyToTheWestBalloon, 0xBBAEE4);
};
//static_assert(sizeof(ZombieModernBalloon) == 0x308);