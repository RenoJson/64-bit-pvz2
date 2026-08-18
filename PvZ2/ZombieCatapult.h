#pragma once
#include "PvZ2/Zombie.h"
#include <Reflection/ReflectionBuilder.h>
#include "Projectile.h"
#include "StringRestrictionSet.h"

class ZombieCatapult : public Zombie
{
public:
	static void buildEventCallbacks(Reflection::CRefManualSymbolBuilder* builder, Reflection::RClass* rtClass);
	static void buildStates();
	static void WaitingOnEnter(ZombieCatapult* zombie);
	static void WaitingOnLoop(ZombieCatapult* zombie);
	static void WaitingOnExit(ZombieCatapult* zombie);
	static void AttackOnEnter(ZombieCatapult* zombie);
	static void AttackOnLoop(ZombieCatapult* zombie);
	static void AttackOnExit(ZombieCatapult* zombie);
	int m_remainingAmmo;
	int m_damageIndex;
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieCatapult, 0xC3AB1C);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(Zombie);
	ZombieCatapult::buildEventCallbacks(builder, rclass);
	RT_CLASS_BUILD_SYMBOLS_END();
	RT_CLASS_REGISTER_CLASS_AND_STATES_FUNCTION(ZombieCatapult)
		RT_CLASS_GET_CLASS_FUNCTION(ZombieCatapult, 0xB532A8);
};

class ZombieCatapultProps : public ZombiePropertySheet
{
public:
	float FireInterval = 6.0f;
	Sexy::RtWeakPtr<ProjectilePropertySheet> Projectile;
	PlantRestrictionSet PlantsWhichBreakCatapultOnCollision;
	float ProjectileShotHeight = 500.0f; 
	float ProjectileShotAirTime = 1.5f;

	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieCatapultProps, 0xC136A4);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombiePropertySheet);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieCatapultProps, FireInterval);
	RT_CLASS_REGISTER_CLASS_RTWEAKPTR_PROPERTY(ZombieCatapultProps, Projectile, ProjectilePropertySheet);
	RT_CLASS_REGISTER_CLASS_PROPERTY(ZombieCatapultProps, PlantRestrictionSet, PlantsWhichBreakCatapultOnCollision);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieCatapultProps, ProjectileShotHeight);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieCatapultProps, ProjectileShotAirTime);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieCatapultProps);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieCatapultProps, 0xDA5B00);
};

class ZombieAnimRig_Catapult : public ZombieAnimRig
{
public:
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieAnimRig_Catapult, 0x9D9C4C);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombieAnimRig);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieAnimRig_Catapult);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieAnimRig_Catapult, 0x8DBB70);
};