#pragma once
#include "ZombieAnimRig.h"
#include <Reflection/ReflectionBuilder.h>
#include <Sexy/RtReflectionDelegateBase.h>
class ZombieAnimRig_TombRaiser : public ZombieAnimRig
{
public:
	RtReflectionDelegateBase m_onAnimDone;
	RtReflectionDelegateBase m_onThrow;
};
static_assert(sizeof(ZombieAnimRig_TombRaiser) == 840);
static_assert(offsetof(ZombieAnimRig_TombRaiser, m_onAnimDone) == 664);
static_assert(offsetof(ZombieAnimRig_TombRaiser, m_onThrow) == 752);

class ZombieAnimRig_MysticFormation : public ZombieAnimRig
{
public:
	static uint64_t OnAnimCommand(ZombieAnimRig_MysticFormation* thisPtr, const SexyString& actionName);
	static std::vector<SexyString>* lowerArmList();
	static std::vector<SexyString>* upperArmList();
	static std::vector<SexyString>* headList();
	static bool OnInitializeAnimRigDelegate(ZombieAnimRig_MysticFormation* thisPtr, RtReflectionDelegateBase* dlgt);
	RtReflectionDelegateBase m_onThrow;
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieAnimRig_MysticFormation, 0x9D9C4C);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombieAnimRig);
	RT_CLASS_REGISTER_CLASS_PROPERTY(ZombieAnimRig_MysticFormation, RtReflectionDelegateBase, m_onThrow);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieAnimRig_MysticFormation);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieAnimRig_MysticFormation, 0x8DBB70);
};

class ZombieAnimRig_EnergyDrinker : public ZombieAnimRig
{
public:
	bool m_gotCondition;
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieAnimRig_EnergyDrinker, 0x9D9C4C);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombieAnimRig);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieAnimRig_EnergyDrinker, m_gotCondition);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieAnimRig_EnergyDrinker);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieAnimRig_EnergyDrinker, 0x8DBB70);
};