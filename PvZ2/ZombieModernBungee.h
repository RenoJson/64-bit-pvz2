#pragma once
#include "Zombie.h"
#include <Reflection/ReflectionBuilder.h>

class ZombieModernBungee : public Zombie
{
public:
	RtWeakPtr<Sexy::RtObject> m_target;
	RtWeakPtr<Sexy::RtObject> m_targetedPlant;
	RtWeakPtr<Sexy::RtObject> m_attachedPlant;
	bool m_hasSpawnTarget;

	static void* vftable;
	static void* vftable1;
	static Sexy::RtClass* s_rtClass; 
	static void ModInit();

	static void* Construct() {
		auto* instance = new ZombieModernBungee(); 
		typedef void* (*ctorWithThisPtr)(void*); 
		ctorWithThisPtr baseCtor = (ctorWithThisPtr)getActualOffset(0xC3AB1C); 
		baseCtor(instance);;
		SetVFTable(instance, (uintptr_t)vftable);
		instance->renderableVftable = (void**)vftable1;
		return instance;
	};

	RT_CLASS_BUILD_SYMBOLS_BEGIN(Zombie);
	ZombieModernBungee::buildEventCallbacks(builder, rclass);
	RT_CLASS_REGISTER_CLASS_RTWEAKPTR_PROPERTY(ZombieModernBungee, m_target, Sexy::RtObject);
	RT_CLASS_REGISTER_CLASS_RTWEAKPTR_PROPERTY(ZombieModernBungee, m_targetedPlant, Sexy::RtObject);
	RT_CLASS_REGISTER_CLASS_RTWEAKPTR_PROPERTY(ZombieModernBungee, m_attachedPlant, Sexy::RtObject);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieModernBungee, m_hasSpawnTarget);
	RT_CLASS_BUILD_SYMBOLS_END();
	RT_CLASS_REGISTER_CLASS_AND_STATES_FUNCTION(ZombieModernBungee)
		RT_CLASS_GET_CLASS_FUNCTION(ZombieModernBungee, 0xC36FE4);
	static void buildEventCallbacks(Reflection::CRefManualSymbolBuilder* builder, Reflection::RClass* rtClass);
	static void buildStates();
	static void HuntOnEnter(ZombieModernBungee* zombie);
	static void HuntOnLoop(ZombieModernBungee* zombie);
	static void HuntOnExit(ZombieModernBungee* zombie);
	static void FallOnEnter(ZombieModernBungee* zombie);
	static void FallOnLoop(ZombieModernBungee* zombie);
	static void FallOnExit(ZombieModernBungee* zombie);
	static void WaitingOnEnter(ZombieModernBungee* zombie);
	static void WaitingOnLoop(ZombieModernBungee* zombie);
	static void WaitingOnExit(ZombieModernBungee* zombie);
	static void GrabOnEnter(ZombieModernBungee* zombie);
	static void GrabOnLoop(ZombieModernBungee* zombie);
	static void GrabOnExit(ZombieModernBungee* zombie);
	static void EscapeOnEnter(ZombieModernBungee* zombie);
	static void EscapeOnLoop(ZombieModernBungee* zombie);
	static void EscapeOnExit(ZombieModernBungee* zombie);
};




class ZombieModernBungeeProps : public ZombiePropertySheet
{
public:
	std::vector<SexyString> BungeeTargetExcludeList;
	float PlantLiftingWaitTime = 4.0f;

	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieModernBungeeProps, 0xC136A4);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombiePropertySheet);
	RT_CLASS_REGISTER_STRING_VECTOR_PROPERTY(ZombieModernBungeeProps, BungeeTargetExcludeList);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieModernBungeeProps, PlantLiftingWaitTime);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieModernBungeeProps);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieModernBungeeProps, 0xDA5B00);
};
