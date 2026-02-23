#pragma once
#include "Zombie.h"
#include <Reflection/ReflectionBuilder.h>

class ZombieJourneyToTheWestPiggy : public Zombie
{
public:
	float m_speedModifier;
	DECLARE_STATIC_RT_CLASS_MEMBERS(ZombieJourneyToTheWestPiggy)

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieJourneyToTheWestPiggy, 0xC3AB1C);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(Zombie);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieJourneyToTheWestPiggy, m_speedModifier);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieJourneyToTheWestPiggy);

	RT_CLASS_GET_CLASS_FUNCTION(ZombieJourneyToTheWestPiggy, 0xC36FE4);
};
