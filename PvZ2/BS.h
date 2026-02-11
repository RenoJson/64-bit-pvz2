#pragma once
#include "ZombieLostCityBug.h"
#include "ZombieBugProps.h"
#include "DamageInfo.h"

void* TakeDamage(ZombieLostCityBug* thisPtr, DamageInfo* damageInfo)
{
    auto* props = reinterpret_cast<ZombieBugProps*>(thisPtr->m_propertySheet.Get());

    bool shouldTriggerCatastrophic = false;

    if (!shouldTriggerCatastrophic)
    {
        if (damageInfo->m_damage >= props->DamageAmountWhichAlsoKillBasic)
        {
            shouldTriggerCatastrophic = true;
        }
    }
    if (shouldTriggerCatastrophic)
    {
        thisPtr->m_hasTakenCatastrophicDamage = true;

        auto* getRig = reinterpret_cast<ZombieAnimRig_LostCityBug*>(thisPtr->m_animRig.Get());
        if (getRig)
        {
            getRig->m_hasTakenCatastrophicDamage = true;
        }
    }
    thisPtr->TakeDamage(damageInfo);
}