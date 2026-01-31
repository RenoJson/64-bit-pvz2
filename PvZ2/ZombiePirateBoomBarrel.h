#pragma once
#include "ZombieImp.h"
#include <Reflection/ReflectionBuilder.h>

class ZombiePirateBoomBarrel : public ZombieModernSuperfanImp
{
public:

	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombiePirateBoomBarrel, 0xC3AB1C);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombieModernSuperfanImp);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombiePirateBoomBarrel);
	RT_CLASS_GET_CLASS_FUNCTION(ZombiePirateBoomBarrel, 0xBBF9D8);
};

