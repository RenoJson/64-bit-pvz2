#pragma once
#include "PvZ2/ZombieBasic.h"
#include <Reflection/ReflectionBuilder.h>

class ZombieMausoleumBasic : public ZombieBasic
{
public:
	bool m_hasCursed;
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieMausoleumBasic, 0xC3AB1C);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombieBasic);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieMausoleumBasic);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieMausoleumBasic, 0xB532A8);
};

class ZombieMausoleumSpirit : public ZombieBasic
{
public:
	static void buildEventCallbacks(Reflection::CRefManualSymbolBuilder* builder, Reflection::RClass* rtClass);
	static void buildStates();
	static void IntroOnEnter(ZombieMausoleumSpirit* zombie);
	static void IntroOnLoop(ZombieMausoleumSpirit* zombie);
	static void IntroOnExit(ZombieMausoleumSpirit* zombie);
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieMausoleumSpirit, 0xC3AB1C);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombieBasic); 
	ZombieMausoleumSpirit::buildEventCallbacks(builder, rclass);
	RT_CLASS_BUILD_SYMBOLS_END();
	RT_CLASS_REGISTER_CLASS_AND_STATES_FUNCTION(ZombieMausoleumSpirit)
	RT_CLASS_GET_CLASS_FUNCTION(ZombieMausoleumSpirit, 0xB532A8);
};

class ZombieMausoleumBasicProps : public ZombiePropertySheet
{
public:
	SexyString SpiritTypeName = "mausoleum_spirit";
	bool CursedAtStart = false;

	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieMausoleumBasicProps, 0xC136A4);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombiePropertySheet);
	RT_CLASS_REGISTER_STRING_PROPERTY(ZombieMausoleumBasicProps, SpiritTypeName);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieMausoleumBasicProps, CursedAtStart);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieMausoleumBasicProps);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieMausoleumBasicProps, 0xDA5B00);
};