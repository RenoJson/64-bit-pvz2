//#pragma once
//#include "Zombie.h"
//#include <Reflection/ReflectionBuilder.h>
//
//class ZombieModernBungeeTarget : public Zombie
//{
//public:
//	DECLARE_STATIC_RT_CLASS_MEMBERS(ZombieModernBungeeTarget)
//
//	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieModernBungeeTarget, 0xC3AB1C);
//	RT_CLASS_CONSTRUCT_FUNCTION_END();
//
//	RT_CLASS_BUILD_SYMBOLS_BEGIN(Zombie);
//	ZombieModernBungeeTarget::buildEventCallbacks(builder, rclass);
//	RT_CLASS_BUILD_SYMBOLS_END();
//	RT_CLASS_REGISTER_CLASS_AND_STATES_FUNCTION(ZombieModernBungeeTarget)
//		RT_CLASS_GET_CLASS_FUNCTION(ZombieModernBungeeTarget, 0xC36FE4);
//	static void buildEventCallbacks(Reflection::CRefManualSymbolBuilder* builder, Reflection::RClass* rtClass);
//	static void buildStates();
//	static void MarkOnEnter(ZombieModernBungeeTarget* zombie);
//	static void MarkOnLoop(ZombieModernBungeeTarget* zombie);
//	static void MarkOnExit(ZombieModernBungeeTarget* zombie);
//};