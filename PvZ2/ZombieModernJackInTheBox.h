#pragma once
#include "Zombie.h"
#include <Reflection/ReflectionBuilder.h>

class ZombieModernJackInTheBox : public Zombie
{
public:
	float m_finalExplosionTime;
	bool m_isExploded;
	DECLARE_STATIC_RT_CLASS_MEMBERS(ZombieModernJackInTheBox)

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieModernJackInTheBox, 0xC3AB1C);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(Zombie);
	ZombieModernJackInTheBox::buildEventCallbacks(builder, rclass);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieModernJackInTheBox, m_finalExplosionTime);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieModernJackInTheBox, m_isExploded);
	RT_CLASS_BUILD_SYMBOLS_END();
	RT_CLASS_REGISTER_CLASS_AND_STATES_FUNCTION(ZombieModernJackInTheBox)

	RT_CLASS_GET_CLASS_FUNCTION(ZombieModernJackInTheBox, 0xC36FE4);
	static void buildEventCallbacks(Reflection::CRefManualSymbolBuilder* builder, Reflection::RClass* rtClass);
	static void buildStates();
	static void LostBoxOnEnter(ZombieModernJackInTheBox* zombie);
	static void LostBoxOnLoop(ZombieModernJackInTheBox* zombie);
	static void LostBoxOnExit(ZombieModernJackInTheBox* zombie);
	static void SurpriseOnEnter(ZombieModernJackInTheBox* zombie);
	static void SurpriseOnLoop(ZombieModernJackInTheBox* zombie);
	static void SurpriseOnExit(ZombieModernJackInTheBox* zombie);
};
