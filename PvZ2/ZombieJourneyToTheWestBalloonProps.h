#pragma once
#include "PvZ2/Zombie.h"
#include <Reflection/ReflectionBuilder.h>

class ZombieJourneyToTheWestBalloonProps : public ZombiePropertySheet
{
public:
	SexyString OnAirShockAnimName = "POPANIM_EFFECTS_ZOMBIE_JOURNEY_TO_THE_WEST_BALLOON_SHOCK";
	SexyString OnAirAshAnimName = "POPANIM_EFFECTS_ZOMBIE_JOURNEY_TO_THE_WEST_BALLOON_ASH";
	SexyString ShockAnimName = "POPANIM_EFFECTS_ZOMBIE_SHOCK";
	SexyString AshAnimName = "POPANIM_EFFECTS_ZOMBIE_ASH";
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieJourneyToTheWestBalloonProps, 0xC136A4);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombiePropertySheet);
	RT_CLASS_REGISTER_STRING_PROPERTY(ZombieJourneyToTheWestBalloonProps, OnAirShockAnimName);
	RT_CLASS_REGISTER_STRING_PROPERTY(ZombieJourneyToTheWestBalloonProps, OnAirAshAnimName);
	RT_CLASS_REGISTER_STRING_PROPERTY(ZombieJourneyToTheWestBalloonProps, ShockAnimName);
	RT_CLASS_REGISTER_STRING_PROPERTY(ZombieJourneyToTheWestBalloonProps, AshAnimName);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieJourneyToTheWestBalloonProps);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieJourneyToTheWestBalloonProps, 0xDA5B00);
};
