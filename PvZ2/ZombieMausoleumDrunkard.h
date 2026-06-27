#pragma once
#include "PvZ2/Zombie.h"
#include <Reflection/ReflectionBuilder.h>
#include "Projectile.h"

class ZombieMausoleumDrunkard : public Zombie
{
public:
	static void buildEventCallbacks(Reflection::CRefManualSymbolBuilder* builder, Reflection::RClass* rtClass);
	static void buildStates();
	static void DrunkOnEnter(ZombieMausoleumDrunkard* zombie);
	static void DrunkOnLoop(ZombieMausoleumDrunkard* zombie);
	static void DrunkOnExit(ZombieMausoleumDrunkard* zombie);
	static void ChangeLaneOnEnter(ZombieMausoleumDrunkard* zombie);
	static void ChangeLaneOnLoop(ZombieMausoleumDrunkard* zombie);
	static void ChangeLaneOnExit(ZombieMausoleumDrunkard* zombie);
	int m_lastPathGridX;
	int m_lastPathGridY;
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieMausoleumDrunkard, 0xC3AB1C);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(Zombie);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieMausoleumDrunkard, m_lastPathGridX);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieMausoleumDrunkard, m_lastPathGridY);
	ZombieMausoleumDrunkard::buildEventCallbacks(builder, rclass);
	RT_CLASS_BUILD_SYMBOLS_END();
	RT_CLASS_REGISTER_CLASS_AND_STATES_FUNCTION(ZombieMausoleumDrunkard)
		RT_CLASS_GET_CLASS_FUNCTION(ZombieMausoleumDrunkard, 0xB532A8);
};

class ZombieMausoleumDrunkardProps : public ZombiePropertySheet
{
public:
	float DrunkInterval = 7.5f;
	float ConditionLifetime = 5.0f;
	int PukeOffsetGridX = -2;
	int PukeWidth = 2;

	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieMausoleumDrunkardProps, 0xC136A4);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombiePropertySheet);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieMausoleumDrunkardProps, DrunkInterval);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieMausoleumDrunkardProps, ConditionLifetime);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieMausoleumDrunkardProps, PukeOffsetGridX);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieMausoleumDrunkardProps, PukeWidth);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieMausoleumDrunkardProps);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieMausoleumDrunkardProps, 0xDA5B00);
};