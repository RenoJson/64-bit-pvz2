#pragma once
#include "PvZ2/Zombie.h"
#include <Reflection/ReflectionBuilder.h>

class ZombieFairyTaleImpProps : public ZombiePropertySheet
{
public:
	SexyString SpawnedZombieName = "dark_cavalry";
	float RunningSpeed = 1.2f;

	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieFairyTaleImpProps, 0xC136A4);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombiePropertySheet);
	RT_CLASS_REGISTER_STRING_PROPERTY(ZombieFairyTaleImpProps, SpawnedZombieName);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieFairyTaleImpProps, RunningSpeed);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieFairyTaleImpProps);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieFairyTaleImpProps, 0xDA5B00);
};