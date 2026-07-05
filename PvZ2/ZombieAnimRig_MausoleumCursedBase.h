#pragma once
#include "ZombieAnimRig_Gargantuar.h"
#include <Reflection/ReflectionBuilder.h>
#include "Zombie.h"

class ZombieAnimRig_MausoleumGargantuar : public ZombieAnimRig_Gargantuar
{
public:
	bool m_hasCursed;
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieAnimRig_MausoleumGargantuar, 0x9D9C4C);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombieAnimRig_Gargantuar);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieAnimRig_MausoleumGargantuar, m_hasCursed);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieAnimRig_MausoleumGargantuar);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieAnimRig_MausoleumGargantuar, 0xB54B74);
};