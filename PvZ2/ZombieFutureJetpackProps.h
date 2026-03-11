#pragma once
#include "PvZ2/Zombie.h"
#include "Reflection/CRefManualSymbolBuilder.h"
#include <Reflection/ReflectionBuilder.h>

class ZombieFutureJetpackProps : public ZombiePropertySheet
{
public:
	bool DiscoMode = false;
};


class ZombieFutureJetpackVeteranProps : public ZombieFutureJetpackProps
{
public:
	float SpeedScaleWhenFlyOverPlant = 4.0f;
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieFutureJetpackVeteranProps, 0xC136A4);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombieFutureJetpackProps);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieFutureJetpackVeteranProps, SpeedScaleWhenFlyOverPlant);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieFutureJetpackVeteranProps);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieFutureJetpackVeteranProps, 0xDB00C4);
};

