#pragma once
#include "ZombieBull.h"
#include <Reflection/ReflectionBuilder.h>

class ZombieDarkCavalry : public ZombieBull
{
public:
	int m_watchAnimHandle;
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieDarkCavalry, 0xC3AB1C);
	RT_CLASS_CONSTRUCT_FUNCTION_END();
	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombieBull);
	ZombieDarkCavalry::buildEventCallbacks(builder, rclass);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieDarkCavalry, m_watchAnimHandle);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_AND_STATES_FUNCTION(ZombieDarkCavalry);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieDarkCavalry, 0xAE2050);
	static void buildEventCallbacks(Reflection::CRefManualSymbolBuilder* builder, Reflection::RClass* rtClass);
	static void buildStates();
	static void AttackOnEnter(ZombieDarkCavalry* zombie);
	static void AttackOnLoop(ZombieDarkCavalry* zombie);
	static void AttackOnExit(ZombieDarkCavalry* zombie);
	static void Attack1OnEnter(ZombieDarkCavalry* zombie);
	static void Attack1OnLoop(ZombieDarkCavalry* zombie);
	static void Attack1OnExit(ZombieDarkCavalry* zombie);
};