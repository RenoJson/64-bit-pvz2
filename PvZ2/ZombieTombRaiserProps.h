#pragma once
#include "Zombie.h"
#include <SexyTypes.h>
#include "Reflection/CRefManualSymbolBuilder.h"
#include <Reflection/ReflectionBuilder.h>
class ZombieTombRaiserProps : public ZombiePropertySheet
{
public:
	float TimeBetweenCast = 0.0f;
	float TimeBetweenRaisings = 6.0f;
	int NumberOfTombsToSpawn = 2;
	Sexy::RtWeakPtr<Sexy::RtObject> Projectile;
};
class ZombieZCorpEnergyDrinkerProps : public ZombieTombRaiserProps {
public:
	float ChanceToApplyGoodCondition = 50.0f;
	std::vector<SexyString> GoodConditionToApply;
	float ChanceToApplyBadCondition = 50.0f;
	std::vector<SexyString> BadConditionToApply;
	float ConditionLifeTime = -1.0f;
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieZCorpEnergyDrinkerProps, 0xC136A4);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombieTombRaiserProps);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieZCorpEnergyDrinkerProps, ChanceToApplyGoodCondition);
	RT_CLASS_REGISTER_STRING_VECTOR_PROPERTY(ZombieZCorpEnergyDrinkerProps, GoodConditionToApply);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieZCorpEnergyDrinkerProps, ChanceToApplyBadCondition);
	RT_CLASS_REGISTER_STRING_VECTOR_PROPERTY(ZombieZCorpEnergyDrinkerProps, BadConditionToApply);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieZCorpEnergyDrinkerProps, ConditionLifeTime);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieZCorpEnergyDrinkerProps);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieZCorpEnergyDrinkerProps, 0xB21614);
};