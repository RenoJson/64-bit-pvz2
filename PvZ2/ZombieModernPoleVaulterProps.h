#pragma once
#include "PvZ2/Zombie.h"
#include <Reflection/ReflectionBuilder.h>

class ZombieModernPoleVaulterProps : public ZombiePropertySheet
{
public:
	float RunningSpeed = 0.37f;
	bool Feastivus = false;
	Rect JumpingRect;
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieModernPoleVaulterProps, 0xC136A4);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombiePropertySheet);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieModernPoleVaulterProps, RunningSpeed);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieModernPoleVaulterProps, Feastivus);;
	RT_CLASS_REGISTER_CLASS_PROPERTY(ZombieModernPoleVaulterProps, Rect, JumpingRect);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieModernPoleVaulterProps);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieModernPoleVaulterProps, 0xDA5B00);
};