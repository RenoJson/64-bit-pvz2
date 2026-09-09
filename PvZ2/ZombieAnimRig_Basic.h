#pragma once
#include "ZombieAnimRig.h"
#include <Reflection/ReflectionBuilder.h>
#include "Zombie.h"
#include "ZombieAnimRigBasicTemplateProps.h"

class ZombieAnimRig_Basic : public ZombieAnimRig
{
public:
	RT_CLASS_GET_CLASS_WRAPPER(0x1343884);
    ZombieFlagType m_flagType;
    HelmType m_helmType;
    int m_helmDamageIndex;
    float m_helmFlashStart;
};
static_assert(sizeof(ZombieAnimRig_Basic) == 680);
static_assert(offsetof(ZombieAnimRig_Basic, m_flagType) == 664);
static_assert(offsetof(ZombieAnimRig_Basic, m_helmType) == 668);
static_assert(offsetof(ZombieAnimRig_Basic, m_helmDamageIndex) == 672);
static_assert(offsetof(ZombieAnimRig_Basic, m_helmFlashStart) == 676);

class ZombieAnimRig_Tutorial : public ZombieAnimRig_Basic
{
public:

};

class ZombieAnimRig_LostCityExcavator : public ZombieAnimRig_Basic
{
public:
	bool m_hasShovel;
};


class ZombieAnimRig_MausoleumBasic : public ZombieAnimRig_Tutorial
{
public:
	bool m_hasCursed;
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieAnimRig_MausoleumBasic, 0x9D9C4C);
	RT_CLASS_CONSTRUCT_FUNCTION_END();


	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombieAnimRig_Tutorial);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieAnimRig_MausoleumBasic, m_hasCursed);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieAnimRig_MausoleumBasic);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieAnimRig_MausoleumBasic, 0x12E3600);
};


class ZombieAnimRig_CowboyBasic : public ZombieAnimRig_Basic
{
public:
	bool m_dancing;
	char pad[7];
};
class ZombieAnimRig_BasicTemplate : public ZombieAnimRig_CowboyBasic
{
public:
	Sexy::RtWeakPtr<Sexy::RtObject> m_rigProps;
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieAnimRig_BasicTemplate, 0x9D9C4C);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombieAnimRig_CowboyBasic);
	RT_CLASS_REGISTER_CLASS_RTWEAKPTR_PROPERTY(ZombieAnimRig_BasicTemplate, m_rigProps, Sexy::RtObject)
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieAnimRig_BasicTemplate);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieAnimRig_BasicTemplate, 0x1044118);
};
class ZombieAnimRig_DinoBasic : public ZombieAnimRig_Basic
{
public:

};

class ZombieAnimRig_BeachBasic : public ZombieAnimRig_Basic
{
public:

};

class ZombieAnimRig_DinoBully : public ZombieAnimRig_DinoBasic
{
public:

};

class ZombieAnimRig_JourneyToTheWestGargantuar : public ZombieAnimRig_DinoBully
{
public:
    static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieAnimRig_JourneyToTheWestGargantuar, 0x9D9C4C);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombieAnimRig_DinoBully);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieAnimRig_JourneyToTheWestGargantuar);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieAnimRig_JourneyToTheWestGargantuar, 0xB0AE78);
};
class ZombieAnimRig_ModernMiner : public ZombieAnimRig_DinoBully
{
public:
	bool m_hasStick;
	bool m_digging;
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieAnimRig_ModernMiner, 0x9D9C4C);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombieAnimRig_DinoBully);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieAnimRig_ModernMiner, m_hasStick);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieAnimRig_ModernMiner, m_digging);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieAnimRig_ModernMiner);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieAnimRig_ModernMiner, 0xB0AE78);
};