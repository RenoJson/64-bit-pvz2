#pragma once
#include "PvZ2/Zombie.h"
#include <Reflection/ReflectionBuilder.h>

class ZombieModernDolphinRiderProps : public ZombiePropertySheet
{
public:
	float DolphinTransitionLocation = 810.0f;
	float RidingSpeed = 0.5f;
	Rect JumpingRect;
	float SpeedWhenCarryingDolphin = 0.4f;
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieModernDolphinRiderProps, 0xC136A4);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombiePropertySheet);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieModernDolphinRiderProps, DolphinTransitionLocation);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieModernDolphinRiderProps, RidingSpeed);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieModernDolphinRiderProps, SpeedWhenCarryingDolphin);
	RT_CLASS_REGISTER_CLASS_PROPERTY(ZombieModernDolphinRiderProps, Rect, JumpingRect);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieModernDolphinRiderProps);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieModernDolphinRiderProps, 0xDA5B00);
};