#pragma once
#include "PvZ2/Zombie.h"
#include "Reflection/CRefManualSymbolBuilder.h"
#include <Reflection/ReflectionBuilder.h>
class ZombiePharaoh : public Zombie {
public:
	int m_helmDamageIndex;
};
class ZombieEgyptPharaoh : public ZombiePharaoh {
public:
	DECLARE_STATIC_RT_CLASS_MEMBERS(ZombieEgyptPharaoh)

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieEgyptPharaoh, 0xC3AB1C);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(Zombie);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieEgyptPharaoh);

	RT_CLASS_GET_CLASS_FUNCTION(ZombieEgyptPharaoh, 0xB1E4C8);
};