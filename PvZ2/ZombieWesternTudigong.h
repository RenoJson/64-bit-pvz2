#pragma once
#include "ZombieBasic.h"
#include <Reflection/ReflectionBuilder.h>

class ZombieWesternTudigong : public ZombieBasic
{
public:
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieWesternTudigong, 0xC3AB1C);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombieBasic);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieWesternTudigong);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieWesternTudigong, 0xB532A8);
};
