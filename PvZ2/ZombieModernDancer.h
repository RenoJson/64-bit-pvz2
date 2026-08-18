#pragma once
#include "Zombie.h"
#include <Reflection/ReflectionBuilder.h>

class ZombieModernDancer : public Zombie
{
public:
	RtWeakPtr<Sexy::RtObject> m_mainDancer;
	std::vector<RtWeakPtr<Sexy::RtObject>> m_backupDancer;
	int m_danceCount;
	bool m_isMainDancer;
	bool m_entrySummon;
	DECLARE_STATIC_RT_CLASS_MEMBERS(ZombieModernDancer)

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieModernDancer, 0xC3AB1C);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(Zombie);
	ZombieModernDancer::buildEventCallbacks(builder, rclass);
	RT_CLASS_REGISTER_CLASS_RTWEAKPTR_PROPERTY(ZombieModernDancer, m_mainDancer, Sexy::RtObject);
	RT_CLASS_REGISTER_RTWEAKPTR_VECTOR_PROPERTY(ZombieModernDancer, m_backupDancer, Sexy::RtObject);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieModernDancer, m_danceCount);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieModernDancer, m_isMainDancer);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieModernDancer, m_entrySummon);
	RT_CLASS_BUILD_SYMBOLS_END();
	RT_CLASS_REGISTER_CLASS_AND_STATES_FUNCTION(ZombieModernDancer)
		RT_CLASS_GET_CLASS_FUNCTION(ZombieModernDancer, 0xC36FE4);
	static void buildEventCallbacks(Reflection::CRefManualSymbolBuilder* builder, Reflection::RClass* rtClass);
	static void buildStates();
	static void IntroOnEnter(ZombieModernDancer* zombie);
	static void IntroOnLoop(ZombieModernDancer* zombie);
	static void IntroOnExit(ZombieModernDancer* zombie);
	static void DanceOnEnter(ZombieModernDancer* zombie);
	static void DanceOnLoop(ZombieModernDancer* zombie);
	static void DanceOnExit(ZombieModernDancer* zombie);
	static void SummonOnEnter(ZombieModernDancer* zombie);
	static void SummonOnLoop(ZombieModernDancer* zombie);
	static void SummonOnExit(ZombieModernDancer* zombie);
	static void WaitingOnEnter(ZombieModernDancer* zombie);
	static void WaitingOnLoop(ZombieModernDancer* zombie);
	static void WaitingOnExit(ZombieModernDancer* zombie);
};

class ZombieSpawnWeight : public PropertySheetBase {
public:
	SexyString ZombieTypeName;
	float Weight;

	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieSpawnWeight, 0x8AE3A8);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(PropertySheetBase);
	RT_CLASS_REGISTER_STRING_PROPERTY(ZombieSpawnWeight, ZombieTypeName);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieSpawnWeight, Weight);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieSpawnWeight);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieSpawnWeight, 0xD1FB60);
};

class ZombieModernDancerProps : public ZombiePropertySheet
{
public:
	int DanceCount = 3;
	float ActionInterval = 3;
	std::vector<ZombieSpawnWeight> DancerSpawnList;
	bool IsMainDancer = false;

	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieModernDancerProps, 0xC136A4);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombiePropertySheet);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieModernDancerProps, DanceCount);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieModernDancerProps, ActionInterval);
	RT_CLASS_REGISTER_CLASS_VECTOR_PROPERTY(ZombieModernDancerProps, DancerSpawnList, ZombieSpawnWeight);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieModernDancerProps, IsMainDancer);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieModernDancerProps);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieModernDancerProps, 0xDA5B00);
};
