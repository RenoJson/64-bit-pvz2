#pragma once
#include "BoardEntity.h"

#include "Plant.h"
#include "PvZ2/Zombie.h"

enum ZSIMP {
    flying = 16,
    landing = 17,
    hithead = 18,
    falling = 19,
    gettingup = 20
};
class ZombieImp : public Zombie
{
public:
    // weird inherit from Zombie Basic in 64 bit instead of Zombie in 32 bit
    // so m_helmDamageIndex will keep the imp class right in struct
    int m_helmDamageIndex; 
    bool m_invincibleWhileInAir;
    bool m_getsUpFromLanding;
    float m_fallVelocity;
    bool m_animFailed;
    char pad[3];
};
static_assert(sizeof(ZombieImp) == 1112);
static_assert(offsetof(ZombieImp, m_invincibleWhileInAir) == 1100);
static_assert(offsetof(ZombieImp, m_getsUpFromLanding) == 1101);
static_assert(offsetof(ZombieImp, m_fallVelocity) == 1104);
static_assert(offsetof(ZombieImp, m_animFailed) == 1108);

class ZombieModernSuperfanImp : public ZombieImp
{
public:
    int64_t m_selectedTriggerColumn;
};
static_assert(sizeof(ZombieModernSuperfanImp) == 1120);
static_assert(offsetof(ZombieModernSuperfanImp, m_selectedTriggerColumn) == 1112);

class ZombieModernSuperfanImpProps : public ZombiePropertySheet
{
public:
    int MinTriggerColumn = 1;
    int MaxTriggerColumn = 4;
    float ExplosionDamage = 301;
    float ExplosionRadius = 0.004f;
    Rect ExplosionRect;

	static Reflection::CRefManualSymbolBuilder::BuildSymbolsFunc oZombieModernSuperfanImpPropsBuildSymbols;
	static void modInit();

	static void* construct()
	{
		auto* props = new ZombieModernSuperfanImpProps();

		typedef void* (*ctorWithThisPtr)(void*);
		ctorWithThisPtr baseCtor = (ctorWithThisPtr)getActualOffset(0xC136A4);
		baseCtor(props);

		*reinterpret_cast<uintptr_t*>(props) = getActualOffset(0x240B598);
		return props;
	}

	static void buildSymbols(Reflection::CRefManualSymbolBuilder* builder, Reflection::RClass* rclass)
	{
		oZombieModernSuperfanImpPropsBuildSymbols(builder, rclass);
		RT_CLASS_REGISTER_CLASS_PROPERTY(ZombieModernSuperfanImpProps, Rect, ExplosionRect);
	};
};

