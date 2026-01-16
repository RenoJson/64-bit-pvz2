#pragma once
#include "ZombieLostCityBug.h"
#include <Reflection/ReflectionBuilder.h>

class ZombieWesternKongMingLantern : public ZombieLostCityBug
{
public:
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieWesternKongMingLantern, 0xC3AB1C);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombieWesternKongMingLantern);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieWesternKongMingLantern);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieWesternKongMingLantern, 0xB984C0);
};
class ZombieNewModernBalloon : public ZombieLostCityBug
{
public:
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieNewModernBalloon, 0xC3AB1C);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombieNewModernBalloon);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieNewModernBalloon);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieNewModernBalloon, 0xB984C0);
};
