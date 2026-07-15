#pragma once
#include "ZombieAnimRig_Basic.h"
#include <Reflection/ReflectionBuilder.h>
#include "Zombie.h"

class ZombieAnimRig_MausoleumGargantuar : public ZombieAnimRig_MausoleumBasic
{
public:
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieAnimRig_MausoleumGargantuar, 0x9D9C4C);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombieAnimRig_MausoleumBasic);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieAnimRig_MausoleumGargantuar);
	RT_CLASS_GET_CLASS_NO_PARENT_ADDRESS_CLASS_FUNCTION(ZombieAnimRig_MausoleumGargantuar, ZombieAnimRig_MausoleumBasic::StaticGetType);
};