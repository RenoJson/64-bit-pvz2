#pragma once
#include "PvZ2/Zombie.h"
#include <Reflection/ReflectionBuilder.h>
#include "Projectile.h"

class ZombieMausoleumArcher : public Zombie
{
public:
	static void buildEventCallbacks(Reflection::CRefManualSymbolBuilder* builder, Reflection::RClass* rtClass);
	static void buildStates();
	static void WalkIntoPositionOnEnter(ZombieMausoleumArcher* zombie);
	static void WalkIntoPositionOnLoop(ZombieMausoleumArcher* zombie);
	static void WalkIntoPositionOnExit(ZombieMausoleumArcher* zombie);
	static void WaitShootingOnEnter(ZombieMausoleumArcher* zombie);
	static void WaitShootingOnLoop(ZombieMausoleumArcher* zombie);
	static void WaitShootingOnExit(ZombieMausoleumArcher* zombie);
	static void ShootOnEnter(ZombieMausoleumArcher* zombie);
	static void ShootOnLoop(ZombieMausoleumArcher* zombie);
	static void ShootOnExit(ZombieMausoleumArcher* zombie);
	static void EndShootOnEnter(ZombieMausoleumArcher* zombie);
	static void EndShootOnLoop(ZombieMausoleumArcher* zombie);
	static void EndShootOnExit(ZombieMausoleumArcher* zombie);
	static void StartShootOnEnter(ZombieMausoleumArcher* zombie);
	static void StartShootOnLoop(ZombieMausoleumArcher* zombie);
	static void StartShootOnExit(ZombieMausoleumArcher* zombie);
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieMausoleumArcher, 0xC3AB1C);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(Zombie);
	ZombieMausoleumArcher::buildEventCallbacks(builder, rclass);
	RT_CLASS_BUILD_SYMBOLS_END();
	RT_CLASS_REGISTER_CLASS_AND_STATES_FUNCTION(ZombieMausoleumArcher)
	RT_CLASS_GET_CLASS_FUNCTION(ZombieMausoleumArcher, 0xB532A8);
};

class ZombieMausoleumArcherProps : public ZombiePropertySheet
{
public:
	float FireInterval = 6.0f;
	Sexy::RtWeakPtr<ProjectilePropertySheet> Projectile;
	SexyVector3 ProjectileOffset; 
	SexyString SpiritTypeName = "mausoleum_archer_spirit";
	bool CursedAtStart = false;

	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieMausoleumArcherProps, 0xC136A4);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombiePropertySheet);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieMausoleumArcherProps, FireInterval);
	RT_CLASS_REGISTER_CLASS_RTWEAKPTR_PROPERTY(ZombieMausoleumArcherProps, Projectile, ProjectilePropertySheet);
	RT_CLASS_REGISTER_CLASS_PROPERTY(ZombieMausoleumArcherProps, SexyVector3, ProjectileOffset);
	RT_CLASS_REGISTER_STRING_PROPERTY(ZombieMausoleumArcherProps, SpiritTypeName);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieMausoleumArcherProps, CursedAtStart);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieMausoleumArcherProps);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieMausoleumArcherProps, 0xDA5B00);
};