#pragma once
#include "PvZ2/Zombie.h"
#include <Reflection/ReflectionBuilder.h>

class ZombieBasic : public Zombie
{
public:
	int m_helmDamageIndex;
};
static_assert(offsetof(ZombieBasic, m_helmDamageIndex) == 1096);
static_assert(sizeof(ZombieBasic) == 1104);

class ZombieCowboyBasic : public Zombie
{
public:

};

class ZombieCowboyBasicVeteran : public ZombieBasic
{
public:
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieCowboyBasicVeteran, 0xC3AB1C);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombieBasic);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieCowboyBasicVeteran);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieCowboyBasicVeteran, 0xB532A8);
};

class ZombieBasicTemplate : public ZombieBasic
{
public:
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieBasicTemplate, 0xC3AB1C);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombieBasic);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieBasicTemplate);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieBasicTemplate, 0xB532A8);
};
