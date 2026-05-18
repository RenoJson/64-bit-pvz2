#pragma once
#include "ZombieAnimRig.h"
#include <Reflection/ReflectionBuilder.h>

class ZombieAnimRig_EightiesBass : public ZombieAnimRig
{
public:
	bool m_hasGuitar = false;
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieAnimRig_EightiesBass, 0x9D9C4C);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombieAnimRig);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieAnimRig_EightiesBass, m_hasGuitar);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieAnimRig_EightiesBass);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieAnimRig_EightiesBass, 0x8DBB70);
};