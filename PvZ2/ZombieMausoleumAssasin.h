#pragma once
#include "PvZ2/Zombie.h"
#include <Reflection/ReflectionBuilder.h>

class ZombieMausoleumAssasin : public Zombie
{
public:
	int m_animHandle;
	bool m_becomeAssasin;
	static void buildEventCallbacks(Reflection::CRefManualSymbolBuilder* builder, Reflection::RClass* rtClass);
	static void buildStates();
	static void RevealOnEnter(ZombieMausoleumAssasin* zombie);
	static void RevealOnLoop(ZombieMausoleumAssasin* zombie);
	static void RevealOnExit(ZombieMausoleumAssasin* zombie);
	static void SprintOnEnter(ZombieMausoleumAssasin* zombie);
	static void SprintOnLoop(ZombieMausoleumAssasin* zombie);
	static void SprintOnExit(ZombieMausoleumAssasin* zombie);
	static void BackstabOnEnter(ZombieMausoleumAssasin* zombie);
	static void BackstabOnLoop(ZombieMausoleumAssasin* zombie);
	static void BackstabOnExit(ZombieMausoleumAssasin* zombie);
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieMausoleumAssasin, 0xC3AB1C);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(Zombie);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieMausoleumAssasin, m_animHandle);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieMausoleumAssasin, m_becomeAssasin);
	ZombieMausoleumAssasin::buildEventCallbacks(builder, rclass);
	RT_CLASS_BUILD_SYMBOLS_END();
	RT_CLASS_REGISTER_CLASS_AND_STATES_FUNCTION(ZombieMausoleumAssasin)
		RT_CLASS_GET_CLASS_FUNCTION(ZombieMausoleumAssasin, 0xB532A8);
};

class ZombieMausoleumAssasinProps : public ZombiePropertySheet
{
public:
	float AssasinateDamage = 300.0f;

	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieMausoleumAssasinProps, 0xC136A4);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombiePropertySheet);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieMausoleumAssasinProps, AssasinateDamage);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieMausoleumAssasinProps);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieMausoleumAssasinProps, 0xDA5B00);
};