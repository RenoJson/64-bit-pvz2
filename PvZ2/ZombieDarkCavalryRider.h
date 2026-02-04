#pragma once
#include "ZombieImp.h"
#include <Reflection/ReflectionBuilder.h>

class ZombieDarkCavalryRider : public ZombieImp
{
public:

	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieDarkCavalryRider, 0xC3AB1C);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombieImp);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieDarkCavalryRider);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieDarkCavalryRider, 0xB40DC8);
};

