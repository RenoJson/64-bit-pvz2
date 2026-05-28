#pragma once
#include "PvZ2/Zombie.h"
#include <Reflection/ReflectionBuilder.h>

class ZombieModernPogoProps : public ZombiePropertySheet
{
public:
	float DamageAmountWhichAlsoKillsBasic = 1000.0f;
	float JumpingSpeed = 0.37f;
	Rect JumpingRect;
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieModernPogoProps, 0xC136A4);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombiePropertySheet);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieModernPogoProps, DamageAmountWhichAlsoKillsBasic);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieModernPogoProps, JumpingSpeed);
	RT_CLASS_REGISTER_CLASS_PROPERTY(ZombieModernPogoProps, Rect, JumpingRect);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieModernPogoProps);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieModernPogoProps, 0xDA5B00);
};