#pragma once
#include "ZombieAnimRig_Basic.h"
#include <Reflection/ReflectionBuilder.h>
#include "Zombie.h"

class ZombieAnimRig_Imp : public ZombieAnimRig_Basic
{
public:

};
class ZombieAnimRig_ModernSuperfanImp : public ZombieAnimRig_Imp
{
public:
	bool m_kicked;
	char pad[7];
};
class ZombieAnimRig_PirateBoomBarrel : public ZombieAnimRig_ModernSuperfanImp
{
public:
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieAnimRig_PirateBoomBarrel, 0x9D9C4C);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombieAnimRig_ModernSuperfanImp);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieAnimRig_PirateBoomBarrel);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieAnimRig_PirateBoomBarrel, 0xBC0CE4);
};
