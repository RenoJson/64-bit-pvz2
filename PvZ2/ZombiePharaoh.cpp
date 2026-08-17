#include "ZombiePharaoh.h"
#include "DamageInfo.h"
#include "ZombieHelper.h"
#include "MessageRouter.h"
#include "Messages.h"
#include "ZombieStateHelper.h"
#include "Board.h"
#include "ZombieModernDolphinRider.h"
#include "DamageLifetime.h"
#include "PlantIceBloom.h"
#include "ZombieImp.h"
#include "ZombieWithAction.h"


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

    bool isAlreadyHeadless = (thisPtr->m_zombieFlags & 4) != 0 || (thisPtr->m_entityState.m_id == 3);

    float expectedHp = thisPtr->m_hitpoints - damageInfo->m_damage;

    float headDropThreshold = -1.0f;
    if (CallVirtualFunc<bool>(thisPtr, VFUNC_HAS_HEAD_DROP)) {
        headDropThreshold = CallVirtualFunc<float>(thisPtr, VFUNC_GET_HEAD_THRESHOLD) * thisPtr->m_maxHitpoints;
    }

    if (damageInfo->m_attacker != nullptr && damageInfo->m_attacker->IsType(Plant::StaticGetType()))
    {
        auto plant = static_cast<Plant*>(damageInfo->m_attacker);

        if (plant->m_plantFramework->IsType(PlantIceBloom::StaticGetType()))
        {
            if (ZombieHasCondition(thisPtr, zombie_condition_iceblocked)) {
                return;
            }

            bool willDie = (expectedHp <= 0.0f);
            bool willDropHead = (headDropThreshold >= 0.0f && expectedHp < headDropThreshold);

            if (!isAlreadyHeadless && (willDie || willDropHead))
            {
                auto iceBloomProps = reinterpret_cast<IceBloomProps*>(plant->m_propertySheet.Get());
                float blockHP = iceBloomProps->IceBlockHealth;
                CallFunc<void>(0xC42320, thisPtr, &iceBloomProps->IceBlockDamagePhases, plant, blockHP);
                return;
            }
        }
    }

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
            uint64_t SKIP_ARM_MASK = damage_instantly_fatal | damage_lightning | damage_ash_death;

            bool isFatalSpecialDeath = false;
            if (headDropThreshold >= 0.0f && thisPtr->m_hitpoints < headDropThreshold)
            {
                if ((damageInfo->m_flags & SKIP_ARM_MASK) != 0)
                {
                    isFatalSpecialDeath = true;
                }
            }
            if (!isFatalSpecialDeath)
            {
                CallFunc<void>(0xC47944, thisPtr); 
            }
        }
    }

    bool isIceBlocked = ZombieHasCondition(thisPtr, zombie_condition_iceblocked);

    
    uint64_t SPECIAL_DEATH_MASK = damage_instantly_fatal | damage_lightning | damage_ash_death |
        damage_mower | damage_plantify_on_death | damage_no_bleed_on_death;

    if (CallVirtualFunc<bool>(thisPtr, VFUNC_HAS_HEAD_DROP))
    {
        if (!isAlreadyHeadless && headDropThreshold >= 0.0f && thisPtr->m_hitpoints < headDropThreshold && !isIceBlocked)
        {
            if ((damageInfo->m_flags & SPECIAL_DEATH_MASK) == 0)
            {
                if (CallVirtualFunc<bool>(thisPtr, VFUNC_DUMMY_TRUE)) 
                {
                    CallVirtualFunc<void>(thisPtr, VFUNC_ENTER_BLEEDING); 
                    CallFunc<void>(0xC47BF8, thisPtr, damageInfo); 
                    CallFunc<void>(0xC47E24, thisPtr, damageInfo->m_flags); 
                    return;
                }
            }

            if ((damageInfo->m_flags & damage_no_bleed_on_death) != 0)
            {
                CallFunc<void>(0xC47E24, thisPtr, damageInfo->m_flags);
                CallFunc<void>(0xC48338, thisPtr, damageInfo);
                return;
            }
        }
    }

    if (thisPtr->m_hitpoints <= 0.0f)
    {
        if (thisPtr->IsType(ZombieZombossMech::StaticGetType()))
        {
            thisPtr->m_hitpoints = 0.0f;
            return;
        }

        if ((thisPtr->m_zombieFlags & 0x200) != 0) 
        {
            return;
        }

        if (headDropThreshold <= 0.0f)
        {
            CallFunc<void>(0xC47BF8, thisPtr, damageInfo);
        }
        else if (isIceBlocked)
        {
            CallFunc<void>(0xC47E24, thisPtr, damageInfo->m_flags);
        }

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
                        damageInfo->m_flags |= DamageTypeFlags::damage_hits_only_shield;

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
            if (!thisPtr->IsInGridItem())
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
    return thisPtr->m_position.x <= 0.0f;
}

bool hkIsDeadOrDying(Zombie* thisPtr)
{
    int stateId = thisPtr->m_entityState.m_id;

    if ((stateId >= 4 && stateId <= 8) || stateId == 14)
    {
        return true;
    }
    if (stateId == 3 && thisPtr->m_position.x <= 232.0f) {
        return true;
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
bool hkCanBeTargetted(Zombie* thisPtr, char targetingFlags) {
    int state = thisPtr->m_entityState.m_id;
    int zombieFlags = thisPtr->m_zombieFlags;

    if ((targetingFlags & 4) == 0)
    {
        if ((state >= 4 && state <= 8) || state == 14)
        {
            return false; 
        }

        if (state == 10 || state == 11)
        {
            if (thisPtr->m_hitpoints <= 0.0f)
            {
                return false;
            }
        }
    }

    if ((zombieFlags & 0x80000) != 0) {
        return false;
    }

    if (thisPtr->IsInGridItem()) {
        return false;
    }

    if (state <= 12) 
    {
        unsigned int isRising = 0x1600;
        if (((1 << state) & isRising) != 0)
        {
            return false;
        }
    }

    if ((targetingFlags & 1) == 0 || ((zombieFlags & 0x40) != 0) || (thisPtr->m_position.z > 0.0f))
    {
        if ((targetingFlags & 2) != 0)
        {
            if (((zombieFlags & 0x40) != 0) || (thisPtr->m_position.z > 0.0f)) {
                return true; 
            }
        }
        return false;
    }

    return true;
}

float SurferIsHeadDrop(ZombieBeachSurfer* zombie)
{
    if (zombie->m_entityState.m_id == 16) {
        return -1.0f;
    }
    else {
        auto props = reinterpret_cast<ZombiePropertySheet*>(zombie->m_propertySheet.Get());
        if (props->SkipHeadDropState)
        {
            return -1.0f;
        }
        else {
            return props->HeadDropFraction;
        }
    }
}
bool IsInBleedingState(Zombie* thisPtr) {
    return thisPtr->m_entityState.m_id == 3;
}

bool hkZombieCheckConditionsFlag(Zombie* zombie, int flag) {
    
    if ((flag & 1) == 0 || !hkIsDeadOrDying(zombie))
    {
        if ((flag & 0x10) == 0 || ((zombie->m_zombieFlags & zombiegrabbedbyptero) != 0) || zombie->IsInGridItem())
        {
            if ((flag & 0x20) == 0 || (!((zombie->m_zombieFlags & zombiegrabbedbyptero) != 0) && !zombie->IsInGridItem()))
            {
                auto board = Board::GetBoard();
                auto boardProps = CallFunc<BoardPropertySheet*>(0xAA1EF4, board);
                if ((flag & 0x200) == 0 || (zombie->m_position.x <= boardProps->PlantTargetingXThreshold))
                {
                   if ((flag & 0x100) == 0 || (zombie->m_position.x > boardProps->PlantTargetingXThreshold))
                   {
                         if ((flag & 0x10000) == 0 || (zombie->m_zombieFlags & 0x2000000) == 0)
                         {
                            return false;
                         }
                   }
                }
            }
        }
    }

    return true;
}
typedef void (*Update)(Zombie*);
Update oUpdate = nullptr;
void hkUpdate(Zombie* thisPtr) {
    oUpdate(thisPtr);
    if (((thisPtr->m_zombieFlags & 4) != 0))
    {
        ZombieRemoveCondition(thisPtr, zombie_condition_butter);
        ZombieRemoveCondition(thisPtr, zombie_condition_contagiouspoison);
        ZombieRemoveCondition(thisPtr, zombie_condition_decaypoison);
        ZombieRemoveCondition(thisPtr, zombie_condition_poisoned);
    }

    if (((thisPtr->m_zombieFlags & 4) != 0) && 
        ZombieHasCondition(thisPtr, zombie_condition_warpingOut))
    {
        auto rig = reinterpret_cast<ZombieAnimRig*>(thisPtr->m_animRig.Get());
        (thisPtr->m_zombieFlags & 2) != 0;
        CallVirtualFunc<void>(rig, 46);
        ZombieEnterState(thisPtr, 4, 0);
    }

    if(ZombieIsDeadOrDying(thisPtr) && ZombieHasCondition(thisPtr, zombie_condition_bleeding))
    {
        ZombieRemoveCondition(thisPtr, zombie_condition_bleeding);
	}
}

float GetTotalArmorHealth(std::vector<Sexy::RtWeakPtr<Armor>>& armorList)
{
    float totalHealth = 0.0f;

    for (auto& armorWeakPtr : armorList)
    {
        if (armorWeakPtr.IsValid())
        {
            Armor* armor = reinterpret_cast<Armor*>(armorWeakPtr.Get());

            if (armor != nullptr && !armor->m_destroyed)
            {
                bool hasPassDamage = false;

                if (armor->m_propertySheetPtr.IsValid())
                {
                    ArmorPropertySheet* props = reinterpret_cast<ArmorPropertySheet*>(armor->m_propertySheetPtr.Get());

                    if (props != nullptr)
                    {
                        auto it = std::find(props->ArmorFlags.begin(), props->ArmorFlags.end(), ArmorTypeFlags::passdamage);

                        if (it != props->ArmorFlags.end())
                        {
                            hasPassDamage = true;
                        }
                    }
                }
                if (!hasPassDamage)
                {
                    totalHealth += std::max(0.0f, armor->m_health);
                }
            }
        }
    }
    return totalHealth;
}
int hkCalcProgressMeterHP(Zombie* zombie) {
    if (zombie->IsType(ZombieImp::StaticGetType())) {
        return -1;
    }

    if ((zombie->m_teamFlags & 2) != 0) {
        float currentBaseHP = std::max(0.0f, zombie->m_hitpoints);
        return (int)currentBaseHP + (int)GetTotalArmorHealth(zombie->m_armor);
    }
    return 0;
}
void PatchZombieSetCondition()
{
    uint32_t value = 0x7100011F; // restore damage flash for corpse by changing state cant get condition to idle
    ReplaceBytes(0xC43898, &value, 4);
}

void PatchZombieUpdate()
{
    uint32_t value = 0x52883E0A; 
    ReplaceBytes(0xC3D908, &value, 4);
    ReplaceBytes(0xC3D978, &value, 4);
}


void ZombiePharaoh::ModInit() {
    LOGI("ZombiePharaoh init");
    PVZ2HookFunction(0xC3D7A0, (void*)hkUpdate, (void**)&oUpdate);
    PVZ2HookFunction(0xC56A54, (void*)hkZombieCheckConditionsFlag, nullptr);
    PVZ2HookFunction(0xC4126C, (void*)hkCalcProgressMeterHP, nullptr);
    PVZ2HookFunction(0xC3E204, (void*)hkIsDeadOrDying, nullptr);
    PVZ2HookFunction(0xC4CED8, (void*)IsReadyToDie, nullptr);
    PVZ2HookFunction(0xC490AC, (void*)isHeadDrop, nullptr);
    PVZ2HookFunction(0xC41014, (void*)hkGetHeadDrop, nullptr);
    PVZ2HookFunction(0xC450BC, (void*)hkZombieTakeRealDamage, nullptr);
    PVZ2HookFunction(0xC43B90, (void*)hkTakeDamage, nullptr);
    PVZ2HookFunction(0xC4CEC8, (void*)IsInBleedingState, nullptr);
    PVZ2HookFunction(0xC4D594, (void*)hkCanBeTargetted, nullptr);
    PVZ2HookFunction(0xAD117C, (void*)SurferIsHeadDrop, nullptr);
    PatchZombieUpdate();
    PatchZombieSetCondition();
    LOGI("ZombiePharaoh finish init");
}