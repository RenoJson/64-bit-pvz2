#pragma once
#include "Zombie.h"
#include <SexyTypes.h>
#include "Reflection/CRefManualSymbolBuilder.h"
#include <Reflection/ReflectionBuilder.h>
using namespace Sexy;
class SpellBolt {
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

class ZombieZCorpEnergyDrinker : public ZombieTombRaiser {
public:
	bool m_gotCondition = false;
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieZCorpEnergyDrinker, 0xC3AB1C);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombieTombRaiser);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieZCorpEnergyDrinker, m_gotCondition);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieZCorpEnergyDrinker);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieZCorpEnergyDrinker, 0xBBAEE4);
};