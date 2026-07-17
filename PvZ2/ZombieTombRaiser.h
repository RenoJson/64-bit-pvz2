#pragma once
#include "Zombie.h"
#include <SexyTypes.h>
#include "Reflection/CRefManualSymbolBuilder.h"
#include <Reflection/ReflectionBuilder.h>
using namespace Sexy;
class SpellBolt {
public:
	int posX;
	int posY;
	float velX;
	float velY;
	float lifetime;
	int TargetCellX;
	int TargetCellY;
};
class ZombieTombRaiser : public Zombie {
public:
	int m_remainingAmmo;
	int m_remainingGravespawnAttempts;
	float m_startThrowTime;
	Point m_throwingTarget;
	char pad[4];
	std::vector<SpellBolt> m_pendingGraves;
};
static_assert(sizeof(ZombieTombRaiser) == 1144);
static_assert(offsetof(ZombieTombRaiser, m_remainingAmmo) == 1096);
static_assert(offsetof(ZombieTombRaiser, m_remainingGravespawnAttempts) == 1100);
static_assert(offsetof(ZombieTombRaiser, m_startThrowTime) == 1104);
static_assert(offsetof(ZombieTombRaiser, m_throwingTarget) == 1108);
static_assert(offsetof(ZombieTombRaiser, m_pendingGraves) == 1120);

class ZombieMysticFormation : public Zombie {
public:
	int m_remainingAmmo;
	int m_remainingTokenspawnAttempts;
	float m_startThrowTime;
	Point m_throwingTarget;
	int m_projectileIndex;
	std::vector<SpellBolt> m_pendingTokens;
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieMysticFormation, 0xC3AB1C);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(Zombie);
	ZombieMysticFormation::buildEventCallbacks(builder, rclass);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieMysticFormation, m_remainingAmmo);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieMysticFormation, m_remainingTokenspawnAttempts);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieMysticFormation, m_startThrowTime);
	RT_CLASS_REGISTER_CLASS_PROPERTY(ZombieMysticFormation, Point, m_throwingTarget);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieMysticFormation, m_projectileIndex);
	RT_CLASS_REGISTER_CLASS_VECTOR_PROPERTY(ZombieMysticFormation, SpellBolt, m_pendingTokens);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_AND_STATES_FUNCTION(ZombieMysticFormation);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieMysticFormation, 0xC36FE4);
	static void buildEventCallbacks(Reflection::CRefManualSymbolBuilder* builder, Reflection::RClass* rtClass);
	static void buildStates();
	static void WaitingOnEnter(ZombieMysticFormation* zombie);
	static void WaitingOnLoop(ZombieMysticFormation* zombie);
	static void WaitingOnExit(ZombieMysticFormation* zombie);
	static void ThrowOnEnter(ZombieMysticFormation* zombie);
	static void ThrowOnLoop(ZombieMysticFormation* zombie);
	static void ThrowOnExit(ZombieMysticFormation* zombie);
};

class ZombieZCorpEnergyDrinker : public Zombie {
public:
	bool m_gotCondition;
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieZCorpEnergyDrinker, 0xC3AB1C);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(Zombie);
	ZombieZCorpEnergyDrinker::buildEventCallbacks(builder, rclass);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieZCorpEnergyDrinker, m_gotCondition);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_AND_STATES_FUNCTION(ZombieZCorpEnergyDrinker);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieZCorpEnergyDrinker, 0xC36FE4);
	static void buildEventCallbacks(Reflection::CRefManualSymbolBuilder* builder, Reflection::RClass* rtClass);
	static void buildStates();
	static void DrinkingOnEnter(ZombieZCorpEnergyDrinker* zombie);
	static void DrinkingOnLoop(ZombieZCorpEnergyDrinker* zombie);
	static void DrinkingOnExit(ZombieZCorpEnergyDrinker* zombie);
};