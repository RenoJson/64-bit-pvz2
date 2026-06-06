#pragma once
#include "ZombieAnimRig.h"
#include <Reflection/ReflectionBuilder.h>

class ZombieAnimRig_ModernDolphinRider : public ZombieAnimRig
{
public:
	bool m_isRidingDolphin;
	bool m_isCarryingDolphin;
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieAnimRig_ModernDolphinRider, 0x9D9C4C);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombieAnimRig);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieAnimRig_ModernDolphinRider, m_isRidingDolphin);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieAnimRig_ModernDolphinRider, m_isCarryingDolphin);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieAnimRig_ModernDolphinRider);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieAnimRig_ModernDolphinRider, 0x8DBB70);
};