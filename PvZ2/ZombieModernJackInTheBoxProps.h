#pragma once
#include "PvZ2/Zombie.h"
#include <Reflection/ReflectionBuilder.h>

class TimerExplosionProps : public PropertySheetBase
{
public:
	ValueRange HighTimer;
	ValueRange LowTimer;
	float HighTimerChance;
	float LowTimerChance;
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(TimerExplosionProps, 0x8AE3A8);
	RT_CLASS_CONSTRUCT_FUNCTION_END();
	RT_CLASS_BUILD_SYMBOLS_BEGIN(PropertySheetBase);
	RT_CLASS_REGISTER_CLASS_PROPERTY(TimerExplosionProps, ValueRange, HighTimer);
	RT_CLASS_REGISTER_CLASS_PROPERTY(TimerExplosionProps, ValueRange, LowTimer);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(TimerExplosionProps, HighTimerChance);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(TimerExplosionProps, LowTimerChance);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(TimerExplosionProps);
	RT_CLASS_GET_CLASS_FUNCTION(TimerExplosionProps, 0xD1FB60);
};

class ZombieModernJackInTheBoxProps : public ZombiePropertySheet
{
public:
	float HaveBoxSpeedScale = 2.5f;
	TimerExplosionProps BoxExplodeTimerProps;
	float MinXPosToExplode = 650.0f;
	float ExplosionDamage = 1000.0f;
	Rect ExplosionRect;
	bool CanBreakVase = false;
	bool CanRevealVase = false;
	float VaseRevealDuration = 15.0f;
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieModernJackInTheBoxProps, 0xC136A4);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombiePropertySheet);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieModernJackInTheBoxProps, HaveBoxSpeedScale);
	RT_CLASS_REGISTER_CLASS_PROPERTY(ZombieModernJackInTheBoxProps, TimerExplosionProps, BoxExplodeTimerProps);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieModernJackInTheBoxProps, MinXPosToExplode);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieModernJackInTheBoxProps, ExplosionDamage);
	RT_CLASS_REGISTER_CLASS_PROPERTY(ZombieModernJackInTheBoxProps, Rect, ExplosionRect);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieModernJackInTheBoxProps, CanBreakVase);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieModernJackInTheBoxProps, CanRevealVase);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieModernJackInTheBoxProps, VaseRevealDuration);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieModernJackInTheBoxProps);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieModernJackInTheBoxProps, 0xDA5B00);
};