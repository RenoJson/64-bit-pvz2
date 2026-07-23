#include "ZombiePharaoh.h"
#include "DamageInfo.h"
#include "ZombieHelper.h"
#include "MessageRouter.h"
#include "Messages.h"
#include "ZombieStateHelper.h"
#include "Board.h"


#define VFUNC_CAN_DROP_ARM        81
#define VFUNC_HAS_HEAD_DROP       82
#define VFUNC_DUMMY_TRUE          83
#define VFUNC_ENTER_BLEEDING      179
#define VFUNC_ON_TAKE_DAMAGE      184
#define VFUNC_GET_ARM_THRESHOLD   194
#define VFUNC_GET_HEAD_THRESHOLD   195


float hkGetHeadDrop(Zombie* thisPtr) {
    auto props = reinterpret_cast<ZombiePropertySheet*>(thisPtr->m_propertySheet.Get()); 
    if (props->SkipHeadDropState)
    {
        return -1.0f;
    }
    else {
        return props->HeadDropFraction;
    }
}
bool isHeadDrop(Zombie* thisPtr) {
    return ((thisPtr->m_zombieFlags & 4) == 0) && hkGetHeadDrop(thisPtr) >= 0.0f;
}
void hkZombieTakeRealDamage(Zombie* thisPtr, DamageInfo* damageInfo)
{

    if (damageInfo->m_damage <= 0.0f)
    {
        return;
    }

    float oldHp = thisPtr->m_hitpoints;
    thisPtr->m_hitpoints -= (damageInfo->m_damage >= thisPtr->m_hitpoints) ? thisPtr->m_hitpoints : damageInfo->m_damage;
    if ((damageInfo->m_flags & damage_no_sound) == 0)
    {
        auto props = reinterpret_cast<ZombiePropertySheet*>(thisPtr->m_propertySheet.Get());
        ZombiePlaySoundEvent(thisPtr, props->SoundOnTakeDamage, 0.0f);
    }

    CallVirtualFunc<void>(thisPtr, VFUNC_ON_TAKE_DAMAGE, damageInfo);

    if (CallVirtualFunc<bool>(thisPtr, VFUNC_CAN_DROP_ARM))
    {
        float armDropThreshold = CallVirtualFunc<float>(thisPtr, VFUNC_GET_ARM_THRESHOLD) * thisPtr->m_maxHitpoints;
        if (armDropThreshold >= 0.0f && thisPtr->m_hitpoints < armDropThreshold)
        {
            CallFunc<void>(0xC47944, thisPtr);
        }
    }

    if (CallVirtualFunc<bool>(thisPtr, VFUNC_HAS_HEAD_DROP))
    {
        float headDropThreshold = CallVirtualFunc<float>(thisPtr, VFUNC_GET_HEAD_THRESHOLD) * thisPtr->m_maxHitpoints;
        if (headDropThreshold >= 0.0f && thisPtr->m_hitpoints < headDropThreshold)
        {
            if ((thisPtr->m_zombieFlags & 0x200) == 0)
            {
                thisPtr->m_zombieFlags |= 0x200;
                if (CallVirtualFunc<bool>(thisPtr, VFUNC_DUMMY_TRUE))
                {
                    CallVirtualFunc<void>(thisPtr, VFUNC_ENTER_BLEEDING);
                    CallFunc<void>(0xC47BF8, thisPtr, damageInfo);
                    CallFunc<void>(0xC47E24, thisPtr, damageInfo->m_attacker);
                }
            }
        }
    }
    if (thisPtr->m_hitpoints <= 0.0f)
    {
        CallFunc<void>(0xC48338, thisPtr, damageInfo);
    }
}

void hkTakeDamage(Zombie* thisPtr, DamageInfo* damageInfo)
{
    for (size_t i = 0; i < thisPtr->m_armor.size(); i++)
    {
        Armor* armor = thisPtr->m_armor[i].Get();

        if (armor != nullptr && !armor->m_destroyed && armor->m_health > 0)
        {
            auto* armorProps = reinterpret_cast<ArmorPropertySheet*>(armor->m_propertySheetPtr.Get());

            if (armorProps != nullptr)
            {
                bool hasAbsorbOverflow = false;

                for (size_t j = 0; j < armorProps->ArmorFlags.size(); j++)
                {
                    if (armorProps->ArmorFlags[j] == ArmorTypeFlags::absorboverflow)
                    {
                        hasAbsorbOverflow = true;
                        break;
                    }
                }

                if (hasAbsorbOverflow)
                {
                    if (damageInfo->m_damage >= armor->m_health)
                    {
                        damageInfo->m_flags &= ~DamageTypeFlags::damage_bypass_shield;
                        damageInfo->m_flags &= ~DamageTypeFlags::damage_hits_shield_and_body;

                        damageInfo->m_flags &= ~DamageTypeFlags::damage_hits_only_shield;

                        float calculatedDamage = armor->m_health;

                        if (thisPtr->m_damageScale > 0.001f)
                        {
                            calculatedDamage /= thisPtr->m_damageScale;
                        }

                        bool isShrunken = CallFunc<bool, Zombie*, int>(0xC3E44C, thisPtr, zombie_condition_shrinking)
                            || CallFunc<bool, Zombie*, int>(0xC3E44C, thisPtr, zombie_condition_shrunken);

                        if (isShrunken && thisPtr->m_shrunkenDamageScale > 0.001f)
                        {
                            calculatedDamage /= thisPtr->m_shrunkenDamageScale;
                        }
                        damageInfo->m_damage = calculatedDamage;
                    }
                }
            }
            break;
        }
    }
    int m_stateId = thisPtr->m_entityState.m_id;
    BoardEntity* attacker = nullptr;

    if ((m_stateId >= 4 && m_stateId <= 8) || m_stateId == 10 || m_stateId == 14) {
        return;
    }

    if ((m_stateId != ZS_RiseFromStorm && m_stateId != ZS_StormEntrance) || thisPtr->m_hitpoints > 0)
    {
        if ((thisPtr->m_zombieFlags & 0x3000) == 0)
        {
            ZombieConditionTracker* zTracker = &thisPtr->m_conditionTracker;
            if (!(zTracker->m_conditionFlags[31] || zTracker->m_conditionFlags[34] || zTracker->m_conditionFlags[60]))
            {
                auto newdamageInfo = CallVirtualFunc<DamageInfo>(thisPtr, 55, damageInfo);

                newdamageInfo.m_damage = ceilf(newdamageInfo.m_damage * 100.0f) / 100.0f;

                if (newdamageInfo.m_damage > 0.0f && (newdamageInfo.m_flags & damage_fire) != 0)
                {
                    ZombieRemoveCondition(thisPtr, zombie_condition_freeze);
                    ZombieRemoveCondition(thisPtr, zombie_condition_chill);
                }

                thisPtr->SetStatusesFromDamageInfo(damageInfo);

                if ((newdamageInfo.m_flags & damage_magnet) == 0)
                {
                    thisPtr->m_zombieFlags |= 0x100000u;
                }

                thisPtr->m_conditionTracker.UpdateIfB1();

                float damage = newdamageInfo.m_damage * (thisPtr->m_damageScale * thisPtr->m_conditionTracker.m_damageScale);
                newdamageInfo.m_damage = damage;

                if (newdamageInfo.m_flags & damage_fire)
                {
                    auto props = reinterpret_cast<ZombiePropertySheet*>(thisPtr->m_propertySheet.Get());
                    newdamageInfo.m_damage = props->FireDamageMultiplier * newdamageInfo.m_damage;
                }

                if (damage > 0.0f && ((thisPtr->m_zombieFlags & 0x20) == 0) && (newdamageInfo.m_flags & damage_no_flash) == 0)
                {
                    ZombieSetCondition(thisPtr, zombie_condition_damageflash, 0.0f, 0.25f, 0.0f);
                }

                if ((newdamageInfo.m_flags & damage_eating) || (newdamageInfo.m_flags & damage_mech))
                {
                    attacker = newdamageInfo.m_attacker;
                }

                if (attacker != nullptr && (attacker->m_teamFlags & thisPtr->m_teamFlags) == 0)
                {
                    CallFunc<void, Zombie*, float, BoardEntity*>(0xC44E60, thisPtr, newdamageInfo.m_damage, attacker);
                }
                if (newdamageInfo.m_flags & damage_fatal)
                {
                    bool hasArmor = false;

                    for (auto& armorPtr : thisPtr->m_armor)
                    {
                        auto armor = armorPtr.Get();
                        if (!armor->m_destroyed)
                            hasArmor = true;

                        armor->Function1(damageInfo);
                    }

                    DamageInfo armorDamage;
                    armorDamage.Copy(&newdamageInfo);
                    armorDamage.m_damage = thisPtr->m_helmHitpoints;
                    CallVirtualFunc<DamageInfo*>(thisPtr, 188, &newdamageInfo);

                    DamageInfo zombieDamage;
                    zombieDamage.Copy(&newdamageInfo);
                    zombieDamage.m_damage = std::max(thisPtr->m_maxHitpoints, thisPtr->m_hitpoints);

                    hkZombieTakeRealDamage(thisPtr, &zombieDamage);
                }
                else
                {
                    DamageInfo unkDamage;
                    unkDamage.Copy(&newdamageInfo);

                    if (thisPtr->m_helm && (newdamageInfo.m_flags & damage_bypass_shield) == 0)
                    {
                        unkDamage = CallVirtualFunc<DamageInfo>(thisPtr, 188, &newdamageInfo);
                        auto props = reinterpret_cast<ZombiePropertySheet*>(thisPtr->m_propertySheet.Get());

                        if (props->HelmAbsorbsOverflowDamage)
                            return;

                        if ((newdamageInfo.m_flags & damage_hits_shield_and_body) == 0 && unkDamage.m_damage <= 0.0f)
                        {
                            return;
                        }
                    }

                    bool hasArmor = false;

                    for (auto& armorPtr : thisPtr->m_armor)
                    {
                        auto armor = armorPtr.Get();
                        if (!armor->m_destroyed)
                            hasArmor = true;

                        auto armorDamage = armor->Function1(&unkDamage);
                        unkDamage.Copy(&armorDamage);
                    }

                    if (newdamageInfo.m_flags & damage_hits_shield_and_body)
                    {
                        unkDamage.Copy(&newdamageInfo);
                    }

                    if ((newdamageInfo.m_flags & damage_hits_only_shield) == 0)
                    {
                        hkZombieTakeRealDamage(thisPtr, &unkDamage);
                    }
                }

                bool zombieCanTakeDmg = (thisPtr->m_zombieFlags & 0x100000) != 0;
                auto newState = thisPtr->m_entityState.m_id;

                if (newState > ZS_Plantify || ((m_stateId >= 4 && m_stateId <= 8) || m_stateId == 10 || m_stateId == 14))
                {
                    if (!zombieCanTakeDmg)
                    {
                        zombieCanTakeDmg = ((newState != ZS_RiseFromStorm && newState != ZS_StormEntrance) || thisPtr->m_hitpoints > 0);
                    }
                }
                if (zombieCanTakeDmg)
                {
                    DamageInfo broadcasteddamageInfo;
                    broadcasteddamageInfo.Copy(&newdamageInfo);

                    MessageRouter::GetInstance()->ExecuteMessage((void*)getActualOffset((uintptr_t)MessageId::EntityTakeDamage), thisPtr, &broadcasteddamageInfo);
                }
            }
        }
    }
}
bool IsReadyToDie(Zombie* thisPtr) {
    return thisPtr->m_elapsedTimeInState >= 5.0f;
}
bool hkIsDeadOrDying(Zombie* thisPtr)
{
    int stateId = thisPtr->m_entityState.m_id;

    if ((stateId >= 4 && stateId <= 8) || stateId == 14)
    {
        return true;
    }
    if (stateId == 3) {
        if (thisPtr->m_position.x <= 264.0f) {
            return true;
        }
    }

    if (stateId == 10 || stateId == 11)
    {
        if (thisPtr->m_hitpoints <= 0.0f)
        {
            return true;
        }
    }

    return false;
}

bool hkZombieCheckConditionsFlag(Zombie* zombie, int flag) {
    if ((flag & 1) == 0 || !hkIsDeadOrDying(zombie)) {
        if ((flag & 0x10) == 0 || (zombie->m_zombieFlags & zombiegrabbedbyptero) != 0 || zombie->IsInGridItem()){
            if ((flag & 0x20) == 0 || (zombie->m_zombieFlags & zombiegrabbedbyptero) == 0 || !zombie->IsInGridItem()) {
                auto board = Board::GetBoard();
                auto boardProps = CallFunc<BoardPropertySheet*>(0xAA1EF4, board);
                if ((flag & 0x200) == 0 || (zombie->m_position.x <= boardProps->PlantTargetingXThreshold)) {
                    if ((flag & 0x100) == 0 || (zombie->m_position.x > boardProps->PlantTargetingXThreshold)) {
                        if ((flag & 0x10000) == 0 || (zombie->m_zombieFlags & 0x2000000) == 0) {
                            return false;
                        }
                    }
                }
            }
        }
    }
    return true;
}

void ZombiePharaoh::ModInit() {
    LOGI("ZombiePharaoh init");
    PVZ2HookFunction(0xC56A54, (void*)hkZombieCheckConditionsFlag, nullptr);
    PVZ2HookFunction(0xC3E204, (void*)hkIsDeadOrDying, nullptr);
    PVZ2HookFunction(0xC4CED8, (void*)IsReadyToDie, nullptr);
    PVZ2HookFunction(0xC490AC, (void*)isHeadDrop, nullptr);
    PVZ2HookFunction(0xC41014, (void*)hkGetHeadDrop, nullptr);
    PVZ2HookFunction(0xC450BC, (void*)hkZombieTakeRealDamage, nullptr);
    PVZ2HookFunction(0xC43B90, (void*)hkTakeDamage, nullptr);
    LOGI("ZombiePharaoh finish init");
}