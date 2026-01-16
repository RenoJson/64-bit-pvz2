#pragma once
#include "ZombieGargantuar.h"
#include <Reflection/ReflectionBuilder.h>

class ZombieFairyTaleGargantuar : public ZombieGargantuar
{
public:
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieFairyTaleGargantuar, 0xC3AB1C);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombieGargantuar);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieFairyTaleGargantuar);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieFairyTaleGargantuar, 0xB53840);
};