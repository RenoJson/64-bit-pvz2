#include "ZombieModernBalloon.h"
#include "ZombieJourneyToTheWestBalloonProps.h"
#include "ZombieAnimRig_ModernBalloon.h";
#include "DamageInfo.h"

void* ZombieJourneyToTheWestBalloon::vftable = __null;
Sexy::RtClass* ZombieJourneyToTheWestBalloon::s_rtClass = __null;;


void* BallonTakeDamage(ZombieJourneyToTheWestBalloon* thisPtr, DamageInfo* damageInfo)
{
    auto* props = reinterpret_cast<ZombieJourneyToTheWestBalloonProps*>(thisPtr->m_propertySheet.Get());
    DamageInfo newDmgInfo = *damageInfo;
    if (newDmgInfo.m_damage >= props->DamageAmountWhichAlsoKillsBasic)
    {
        thisPtr->m_hasTakenCatastrophicDamage = true;
    }
    else
    {
        float balloonHP = 0.0f;
        for (size_t i = 0; i < thisPtr->m_armor.size(); i++)
        {
            Armor* armorInstance = thisPtr->m_armor[i].Get();
            if (armorInstance != nullptr && !armorInstance->m_destroyed && armorInstance->m_health > 0)
            {
                balloonHP = armorInstance->m_health; 
                break; 
            }
        }
        if (balloonHP > 0)
        {
            if (newDmgInfo.m_damage > balloonHP) {
                newDmgInfo.m_damage = balloonHP;
            }
            else {
                LOGI("[ZombieBalloon] -> Balloon absorbs damage completely and survives.");
            }
        }
        else
        {
            LOGI("[ZombieBalloon] No Balloon! Zombie takes full damage: %.2f", newDmgInfo.m_damage);
        }
    }

    typedef void* (*funcC43B90)(ZombieJourneyToTheWestBalloon*, DamageInfo*);
    static auto* ZTakeDmg = ((funcC43B90)getActualOffset(0xC43B90));

    return ZTakeDmg(thisPtr, &newDmgInfo);
}
SexyString hkJFixAnimShock(ZombieJourneyToTheWestBalloon* zombie, DamageInfo* damage) {
	auto* getProps = reinterpret_cast<ZombieJourneyToTheWestBalloonProps*>(zombie->m_propertySheet.Get());
	if (zombie->m_hasTakenCatastrophicDamage == true) {
		return getProps->OnAirShockAnimName;
	}
	else {
		return "POPANIM_EFFECTS_ZOMBIE_SHOCK";
	}
}

SexyString hkJFixAnimAsh(ZombieJourneyToTheWestBalloon* zombie, DamageInfo* damage) {
	auto* getProps = reinterpret_cast<ZombieJourneyToTheWestBalloonProps*>(zombie->m_propertySheet.Get());
	if (zombie->m_hasTakenCatastrophicDamage == true) {
		return getProps->OnAirAshAnimName;
	}
	else {
		return "POPANIM_EFFECTS_ZOMBIE_ASH";
	}
}

void ZombieJourneyToTheWestBalloon::modInit() {
	LOGI("ZombieJourneyToTheWestBalloon init");

	vftable = CopyVFTable(getActualOffset(0x23F39D8), 216);

	PatchVFTable(vftable, (void*)ZombieJourneyToTheWestBalloon::StaticGetType, 0);

	PatchVFTable(vftable, (void*)BallonTakeDamage, 35);

	PatchVFTable(vftable, (void*)hkJFixAnimShock, 189);

	PatchVFTable(vftable, (void*)hkJFixAnimAsh, 190);

	ZombieJourneyToTheWestBalloon::StaticGetType();
	LOGI("ZombieJourneyToTheWestBalloon finish init");
}


