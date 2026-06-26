#pragma once
#include "ZombieAnimRig.h"
#include <Reflection/ReflectionBuilder.h>
#include "Zombie.h"

class ZombieAnimRig_MausoleumAssasin : public ZombieAnimRig
{
public:
	bool m_hasScroll;
	bool m_hasDagger;
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieAnimRig_MausoleumAssasin, 0x9D9C4C);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombieAnimRig);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieAnimRig_MausoleumAssasin, m_hasScroll);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieAnimRig_MausoleumAssasin, m_hasDagger);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieAnimRig_MausoleumAssasin);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieAnimRig_MausoleumAssasin, 0x8DBB70);
};