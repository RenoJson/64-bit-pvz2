#pragma once
#include "Zombie.h"
#include <Reflection/ReflectionBuilder.h>

class ZombieModernDolphinRider : public Zombie
{
public:
	bool m_isRidingDolphin;
	bool m_isCarryingDolphin;
	DECLARE_STATIC_RT_CLASS_MEMBERS(ZombieModernDolphinRider)

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieModernDolphinRider, 0xC3AB1C);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(Zombie);
	ZombieModernDolphinRider::buildEventCallbacks(builder, rclass);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieModernDolphinRider, m_isRidingDolphin);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieModernDolphinRider, m_isCarryingDolphin);
	RT_CLASS_BUILD_SYMBOLS_END();
	RT_CLASS_REGISTER_CLASS_AND_STATES_FUNCTION(ZombieModernDolphinRider)

	RT_CLASS_GET_CLASS_FUNCTION(ZombieModernDolphinRider, 0xC36FE4);
	static void buildEventCallbacks(Reflection::CRefManualSymbolBuilder* builder, Reflection::RClass* rtClass);
	static void buildStates();
	static void JumpOnEnter(ZombieModernDolphinRider* zombie);
	static void JumpOnLoop(ZombieModernDolphinRider* zombie);
	static void JumpOnExit(ZombieModernDolphinRider* zombie);
	static void BonkOnEnter(ZombieModernDolphinRider* zombie);
	static void BonkOnLoop(ZombieModernDolphinRider* zombie);
	static void BonkOnExit(ZombieModernDolphinRider* zombie);
	static void PreWalkBeforeRidingOnEnter(ZombieModernDolphinRider* zombie);
	static void PreWalkBeforeRidingOnLoop(ZombieModernDolphinRider* zombie);
	static void PreWalkBeforeRidingOnExit(ZombieModernDolphinRider* zombie);
	static void TransitionOnEnter(ZombieModernDolphinRider* zombie);
	static void TransitionOnLoop(ZombieModernDolphinRider* zombie);
	static void TransitionOnExit(ZombieModernDolphinRider* zombie);
	static void RideDolphinOnEnter(ZombieModernDolphinRider* zombie);
	static void RideDolphinOnLoop(ZombieModernDolphinRider* zombie);
	static void RideDolphinOnExit(ZombieModernDolphinRider* zombie);
	static void RideDolphinToWalkOnEnter(ZombieModernDolphinRider* zombie);
	static void RideDolphinToWalkOnLoop(ZombieModernDolphinRider* zombie);
	static void RideDolphinToWalkOnExit(ZombieModernDolphinRider* zombie);
};
