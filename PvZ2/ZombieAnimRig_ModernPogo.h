#pragma once
#include "ZombieAnimRig.h"
#include <Reflection/ReflectionBuilder.h>

class ZombieAnimRig_ModernPogo : public ZombieAnimRig
{
public:
	bool m_hasPogo = true;
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieAnimRig_ModernPogo, 0x9D9C4C);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombieAnimRig);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieAnimRig_ModernPogo, m_hasPogo);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieAnimRig_ModernPogo);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieAnimRig_ModernPogo, 0x8DBB70);
};