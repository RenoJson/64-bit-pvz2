#pragma once
#include "Zombie.h"
#include "Projectile.h"
#include <SexyTypes.h>
#include "Reflection/CRefManualSymbolBuilder.h"
#include <Reflection/ReflectionBuilder.h>

class TokenLayersConfig : public PropertySheetBase
{
public:
	SexyString ProjectileOffsetTokenLayerName = "_token";
	SexyString GreenTokenProjectileLayerName = "_token_green";
	SexyString RedTokenProjectileLayerName = "_token_red";
	Sexy::RtWeakPtr<ProjectilePropertySheet> GreenTokenProjectile;
	Sexy::RtWeakPtr<ProjectilePropertySheet> RedTokenProjectile; 
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(TokenLayersConfig, 0x8AE3A8);
	RT_CLASS_CONSTRUCT_FUNCTION_END();
	RT_CLASS_BUILD_SYMBOLS_BEGIN(PropertySheetBase);
	RT_CLASS_REGISTER_STRING_PROPERTY(TokenLayersConfig, ProjectileOffsetTokenLayerName);
	RT_CLASS_REGISTER_STRING_PROPERTY(TokenLayersConfig, GreenTokenProjectileLayerName);
	RT_CLASS_REGISTER_STRING_PROPERTY(TokenLayersConfig, RedTokenProjectileLayerName);
	RT_CLASS_REGISTER_CLASS_RTWEAKPTR_PROPERTY(TokenLayersConfig, GreenTokenProjectile, ProjectilePropertySheet);
	RT_CLASS_REGISTER_CLASS_RTWEAKPTR_PROPERTY(TokenLayersConfig, RedTokenProjectile, ProjectilePropertySheet);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(TokenLayersConfig);
	RT_CLASS_GET_CLASS_FUNCTION(TokenLayersConfig, 0xD1FB60);
};


class ZombieTombRaiserProps : public ZombiePropertySheet
{
public:
	float TimeBetweenCasts = 0.0f;
	float TimeBetweenRaisings = 6.0f;
	int NumberOfTombsToSpawn = 2;
	Sexy::RtWeakPtr<Sexy::RtObject> Projectile;
};

class ZombieMysticFormationProps : public ZombiePropertySheet
{
public:
	float TimeBetweenCasts = 0.0f;
	float TimeBetweenRaisings = 6.0f;
	int NumberOfTokensToSpawn = 2;
	TokenLayersConfig TokenProjectileLayerProps;
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieMysticFormationProps, 0xC136A4);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombiePropertySheet);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieMysticFormationProps, TimeBetweenCasts);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieMysticFormationProps, TimeBetweenRaisings);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieMysticFormationProps, NumberOfTokensToSpawn);
	RT_CLASS_REGISTER_CLASS_PROPERTY(ZombieMysticFormationProps, TokenLayersConfig, TokenProjectileLayerProps);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieMysticFormationProps);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieMysticFormationProps, 0xDA5B00);
};

class ZombieZCorpEnergyDrinkerProps : public ZombiePropertySheet {
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

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombiePropertySheet);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieZCorpEnergyDrinkerProps, ChanceToApplyGoodCondition);
	RT_CLASS_REGISTER_STRING_VECTOR_PROPERTY(ZombieZCorpEnergyDrinkerProps, GoodConditionToApply);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieZCorpEnergyDrinkerProps, ChanceToApplyBadCondition);
	RT_CLASS_REGISTER_STRING_VECTOR_PROPERTY(ZombieZCorpEnergyDrinkerProps, BadConditionToApply);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieZCorpEnergyDrinkerProps, ConditionLifeTime);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieZCorpEnergyDrinkerProps);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieZCorpEnergyDrinkerProps, 0xDA5B00);
};