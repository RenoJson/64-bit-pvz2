#pragma once
#include "Zombie.h"
#include <Reflection/ReflectionBuilder.h>

class ZombieModernAllStar : public Zombie
{
public:

};

class ZombieModernAllStarProps: public ZombiePropertySheet
{
public:
	float SmashDamage = 1500.0f;
	float RunningSpeedScale = 0.5f;
	float KickImpMinX = 3.0f;
};

class ZombieModernBerserkerProps : public ZombieModernAllStarProps
{
public:
	float ChargeCooldown = 6.0f;
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieModernBerserkerProps, 0xC136A4);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombieModernAllStarProps);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieModernBerserkerProps, SmashDamage);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieModernBerserkerProps, RunningSpeedScale);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieModernBerserkerProps, KickImpMinX);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieModernBerserkerProps, ChargeCooldown);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieModernBerserkerProps);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieModernBerserkerProps, 0xBB8544);
};

class ZombieModernBerserker : public ZombieModernAllStar
{
public:
	DECLARE_STATIC_RT_CLASS_MEMBERS(ZombieModernBerserker)

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieModernBerserker, 0xC3AB1C);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombieModernAllStar);
	ZombieModernBerserker::buildEventCallbacks(builder, rclass);
	RT_CLASS_BUILD_SYMBOLS_END();
	RT_CLASS_REGISTER_CLASS_AND_STATES_FUNCTION(ZombieModernBerserker)
	RT_CLASS_GET_CLASS_FUNCTION(ZombieModernBerserker, 0xBB7B54);
	static void buildEventCallbacks(Reflection::CRefManualSymbolBuilder* builder, Reflection::RClass* rtClass);
	static void buildStates();
	static void ChargeOnEnter(ZombieModernBerserker* zombie);
	static void ChargeOnLoop(ZombieModernBerserker* zombie);
	static void ChargeOnExit(ZombieModernBerserker* zombie);
};
