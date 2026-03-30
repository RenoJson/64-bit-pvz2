#pragma once
#include "PvZ2/Zombie.h"
#include <Reflection/ReflectionBuilder.h>
#include "ZombieAnimRigBasicTemplateProps.h"

class ZombieBasicProps : public ZombiePropertySheet
{
public:
	Sexy::RtWeakPtr<ZombieAnimRigTemplateConfig> CustomAnimRigPropertySheet;
	SexyString ShockAnimName = "POPANIM_EFFECTS_ZOMBIE_SHOCK";
	SexyString AshAnimName = "POPANIM_EFFECTS_ZOMBIE_ASH";
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieBasicProps, 0xC136A4);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombiePropertySheet);
	RT_CLASS_REGISTER_CLASS_RTWEAKPTR_PROPERTY(ZombieBasicProps, CustomAnimRigPropertySheet, ZombieAnimRigTemplateConfig)
	RT_CLASS_REGISTER_STRING_PROPERTY(ZombieBasicProps, ShockAnimName);
	RT_CLASS_REGISTER_STRING_PROPERTY(ZombieBasicProps, AshAnimName);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieBasicProps);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieBasicProps, 0xDA5B00);
};

class ZombieCowboyVeteranProps : public ZombiePropertySheet
{
public:
	float DancingSpeed;
	float DancingSpeedScale;
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieCowboyVeteranProps, 0xC136A4);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombiePropertySheet);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieCowboyVeteranProps, DancingSpeed);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieCowboyVeteranProps, DancingSpeedScale);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieCowboyVeteranProps);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieCowboyVeteranProps, 0xDA5B00);
};
