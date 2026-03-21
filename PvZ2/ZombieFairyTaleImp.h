#pragma once
#include "ZombieImp.h"
#include <Reflection/ReflectionBuilder.h>

class ZombieFairyTaleImp : public ZombieImp
{
public:
	bool m_hasSpawned;

	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieFairyTaleImp, 0xC3AB1C);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombieImp);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieFairyTaleImp, m_hasSpawned);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieFairyTaleImp);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieFairyTaleImp, 0xB40DC8);
};


