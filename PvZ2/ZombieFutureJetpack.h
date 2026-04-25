#pragma once
#include "PvZ2/Zombie.h"
#include "Reflection/CRefManualSymbolBuilder.h"
#include <Reflection/ReflectionBuilder.h>

class ZombieFutureJetpack : public Zombie
{
public:
	bool m_jumpMovement;
};


class ZombieFutureJetpackVeteran : public ZombieFutureJetpack
{
public:
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieFutureJetpackVeteran, 0xC3AB1C);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombieFutureJetpack);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieFutureJetpackVeteran);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieFutureJetpackVeteran, 0xB43240);
};

class ZombieFutureJetpackProps : public ZombiePropertySheet
{
public:
	bool DiscoMode = false;
};
class ZombieAnimRig_FutureJetpack : public ZombieAnimRig
{
public:
	bool m_discoMode;
};

class ZombieFutureJetpackVeteranProps : public ZombieFutureJetpackProps
{
public:
	float SpeedScaleWhenFlyOverPlant = 4.0f;
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieFutureJetpackVeteranProps, 0xC136A4);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombieFutureJetpackProps);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieFutureJetpackVeteranProps, SpeedScaleWhenFlyOverPlant);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieFutureJetpackVeteranProps);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieFutureJetpackVeteranProps, 0xDB00C4);
};