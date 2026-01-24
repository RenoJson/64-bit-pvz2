#pragma once
#include "ZombieAnimRig.h"
#include <Reflection/ReflectionBuilder.h>

class ZombieAnimRig_Gargantuar : public ZombieAnimRig
{
public:
	static void modInit();
};

class ZombieAnimRig_FairyTaleGargantuar : public ZombieAnimRig_Gargantuar
{
public:
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieAnimRig_FairyTaleGargantuar, 0x9D9C4C);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombieAnimRig_Gargantuar);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieAnimRig_FairyTaleGargantuar);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieAnimRig_FairyTaleGargantuar, 0xB54B74);
};