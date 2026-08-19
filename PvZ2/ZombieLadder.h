#pragma once
#include "PvZ2/Zombie.h"
#include <Reflection/ReflectionBuilder.h>
#include "StringRestrictionSet.h"
#include "Projectile.h"
#include "GridItem.h"

class ZombieModernLadder : public Zombie
{
public:
	RtWeakPtr<RtObject> m_eatTarget;
	DECLARE_STATIC_RT_CLASS_MEMBERS(ZombieModernLadder)

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieModernLadder, 0xC3AB1C);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(Zombie);
	ZombieModernLadder::buildEventCallbacks(builder, rclass);
	RT_CLASS_BUILD_SYMBOLS_END();
	RT_CLASS_REGISTER_CLASS_AND_STATES_FUNCTION(ZombieModernLadder)

	RT_CLASS_GET_CLASS_FUNCTION(ZombieModernLadder, 0xC36FE4);
	static void buildEventCallbacks(Reflection::CRefManualSymbolBuilder* builder, Reflection::RClass* rtClass);
	static void buildStates();
	static void PlaceLadderOnEnter(ZombieModernLadder* zombie);
	static void PlaceLadderOnLoop(ZombieModernLadder* zombie);
	static void PlaceLadderOnExit(ZombieModernLadder* zombie);
	static void LostLadderOnEnter(ZombieModernLadder* zombie);
	static void LostLadderOnLoop(ZombieModernLadder* zombie);
	static void LostLadderOnExit(ZombieModernLadder* zombie);
};

class ZombieModernLadderProps : public ZombiePropertySheet
{
public:
	std::vector<Sexy::RtWeakPtr<ProjectilePropertySheet>> PierceableProjectiles;
	std::vector<SexyString> PlantBlacklist;
	Rect AttackRectWhenHaveLadder;
	Rect HitRectWhenHaveLadder;
	PlantRestrictionSet PlantsWhichPlaceLadderInsteadEating;
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieModernLadderProps, 0xC136A4);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombiePropertySheet);
	RT_CLASS_REGISTER_RTWEAKPTR_VECTOR_PROPERTY(ZombieModernLadderProps, PierceableProjectiles, ProjectilePropertySheet);
	RT_CLASS_REGISTER_STRING_VECTOR_PROPERTY(ZombieModernLadderProps, PlantBlacklist);
	RT_CLASS_REGISTER_CLASS_PROPERTY(ZombieModernLadderProps, PlantRestrictionSet, PlantsWhichPlaceLadderInsteadEating);
	RT_CLASS_REGISTER_CLASS_PROPERTY(ZombieModernLadderProps, Rect, AttackRectWhenHaveLadder);
	RT_CLASS_REGISTER_CLASS_PROPERTY(ZombieModernLadderProps, Rect, HitRectWhenHaveLadder);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieModernLadderProps);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieModernLadderProps, 0xDA5B00);
};

class ZombieAnimRig_ModernLadder : public ZombieAnimRig
{
public:
	bool m_hasLadder;
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieAnimRig_ModernLadder, 0x9D9C4C);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombieAnimRig);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieAnimRig_ModernLadder);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieAnimRig_ModernLadder, 0x8DBB70);
};

class GridItemLadder : public GridItemSurfboard {
public:

};

class GridItemLadderProps : public GridItemSurfboardProps {
public:
	std::vector<SexyString> UnclimbableZombies;
};