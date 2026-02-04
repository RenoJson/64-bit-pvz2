#pragma once
#include "ZombieAnimRig_Imp.h"
#include <Reflection/ReflectionBuilder.h>

class ZombieAnimRig_DarkRider : public ZombieAnimRig_Imp
{
public:
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieAnimRig_DarkRider, 0x9D9C4C);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombieAnimRig_Imp);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieAnimRig_DarkRider);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieAnimRig_DarkRider, 0xB571B8);
};
