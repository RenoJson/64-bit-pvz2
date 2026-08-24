#pragma once
#include "PvZ2/Zombie.h"
#include <Reflection/ReflectionBuilder.h>

class ZombieLadder : public Zombie
{
public:
	RtWeakPtr<RtObject> m_attachedPlant;
	std::vector<RtWeakPtr<RtObject>> m_climbingZombies;
	static void* vftable;
	static void* vftable1;
	static Sexy::RtClass* s_rtClass; 
	static void ModInit();

	static void* Construct() {
		auto* instance = new ZombieLadder(); 
		typedef void* (*ctorWithThisPtr)(void*); 
		ctorWithThisPtr baseCtor = (ctorWithThisPtr)getActualOffset(0xC3AB1C);
		baseCtor(instance);;
		SetVFTable(instance, (uintptr_t)vftable);
		instance->renderableVftable = (void**)vftable1;
		return instance;
	};

	RT_CLASS_BUILD_SYMBOLS_BEGIN(Zombie);
	RT_CLASS_BUILD_SYMBOLS_END();
	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieLadder)
	RT_CLASS_GET_CLASS_FUNCTION(ZombieLadder, 0xC36FE4);
};

class ZombieLadderProps : public ZombiePropertySheet
{
public:
	ClassRestrictionSet UnclimbableZombies;
	PlantRestrictionSet PlantsWhichCanTargetLadder;

	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieLadderProps, 0xC136A4);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombiePropertySheet);
	RT_CLASS_REGISTER_CLASS_PROPERTY(ZombieLadderProps, ClassRestrictionSet, UnclimbableZombies);
	RT_CLASS_REGISTER_CLASS_PROPERTY(ZombieLadderProps, PlantRestrictionSet, PlantsWhichCanTargetLadder);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieLadderProps);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieLadderProps, 0xDA5B00);
};

class ZombieAnimRig_Ladder : public ZombieAnimRig
{
public:
	int m_damageIndex;
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieAnimRig_Ladder, 0x9D9C4C);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombieAnimRig);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieAnimRig_Ladder);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieAnimRig_Ladder, 0x8DBB70);
};
