#pragma once
#pragma once
#include "PvZ2/Zombie.h"
#include <Reflection/ReflectionBuilder.h>

class ZombieJourneyToTheWestPiggyProps : public ZombiePropertySheet
{
public:
	ValueRange SpeedRateModifier;
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieJourneyToTheWestPiggyProps, 0xC136A4);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombiePropertySheet);
	RT_CLASS_REGISTER_CLASS_PROPERTY(ZombieJourneyToTheWestPiggyProps, ValueRange, SpeedRateModifier)
		RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieJourneyToTheWestPiggyProps);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieJourneyToTheWestPiggyProps, 0xDA5B00);
};