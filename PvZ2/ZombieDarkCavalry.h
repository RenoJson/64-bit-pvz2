#pragma once
#include "ZombieBull.h"
#include <Reflection/ReflectionBuilder.h>

class ZombieDarkCavalry : public ZombieBull
{
public:
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieDarkCavalry, 0xC3AB1C);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombieBull);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieDarkCavalry);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieDarkCavalry, 0xAE2050);
};