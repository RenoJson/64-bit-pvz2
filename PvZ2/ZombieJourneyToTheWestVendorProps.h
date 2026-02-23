#pragma once
#include "PvZ2/Zombie.h"
#include <Reflection/ReflectionBuilder.h>

class ZombieJourneyToTheWestVendorProps : public ZombiePropertySheet
{
public:
	pvztime_t SetPigInterval = 6.0f;
	int NumberOfPigToSpawnWhenDie = 4;
	SexyString PigType = "western_piggy";
	SexyVector3 PigSpawnOffset = { 60.0f, 0.0f, 50.0f };
	ValueRange SpeedRateModifier;
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieJourneyToTheWestVendorProps, 0xC136A4);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombiePropertySheet);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieJourneyToTheWestVendorProps, SetPigInterval);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieJourneyToTheWestVendorProps, NumberOfPigToSpawnWhenDie);
	RT_CLASS_REGISTER_STRING_PROPERTY(ZombieJourneyToTheWestVendorProps, PigType);
	RT_CLASS_REGISTER_CLASS_PROPERTY(ZombieJourneyToTheWestVendorProps, SexyVector3, PigSpawnOffset);
	RT_CLASS_REGISTER_CLASS_PROPERTY(ZombieJourneyToTheWestVendorProps, ValueRange, SpeedRateModifier)
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieJourneyToTheWestVendorProps);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieJourneyToTheWestVendorProps, 0xDA5B00);
};