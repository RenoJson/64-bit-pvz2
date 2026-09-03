#include "ZombieModernScreenDoor.h"
#include "ZombieModernScreenDoorProps.h"
#include "DamageInfo.h"
#include "ZombieAnimRig_ModernScreenDoor.h"
#include "ZombieStateHelper.h"
#include "ZombieState.h"
#include "StateMachineBuilder.h"
#include "Projectile.h"
#include "Plant.h"
#include "ZombieHelper.h"
DECLARE_DELEGATES_SETUP(ZombieModernScreenDoor)

static Sexy::DelegateBase lostDoorCompletedDelegate; 

void* ZombieModernScreenDoor::vftable = nullptr;
Sexy::RtClass* ZombieModernScreenDoor::s_rtClass = nullptr;;

typedef uint64_t(*VirtualGetDamageFlagsFunc)(Projectile*);
typedef bool (*VirtualIsValidTargetFunc)(Zombie*, uint64_t);
typedef bool (*VirtualDoImpactFunc)(Projectile*, Zombie*);

Sexy::Rect ScreenDoorGetHitRect(ZombieModernScreenDoor* zombie) {
    auto props = reinterpret_cast<ZombieModernScreenDoorProps*>(zombie->m_propertySheet.Get());
    Rect hitRect;
    float offsetX, offsetY;

    if (ZombieHasArmor(zombie, "ScreenDoor"))
    {
        hitRect.mWidth = props->HitRectWhenHaveDoor.mWidth;
        hitRect.mHeight = props->HitRectWhenHaveDoor.mHeight;
        offsetX = props->HitRectWhenHaveDoor.mX;
        offsetY = props->HitRectWhenHaveDoor.mY;
    }
    else
    {
        hitRect.mWidth = props->HitRect.mWidth;
        hitRect.mHeight = props->HitRect.mHeight;
        offsetX = props->HitRect.mX;
        offsetY = props->HitRect.mY;
    }
    float zX = zombie->m_position.x;
    float zY = zombie->m_position.y;
    float zZ = zombie->m_position.z;
    float facing = ZombieFacing(zombie);

    hitRect.mX = static_cast<int>(zX - (hitRect.mWidth / 2.0f) + (facing * offsetX));
    int baseY = static_cast<int>((zY - zZ) - hitRect.mHeight + offsetY);
    hitRect.mY = baseY;

    ZombieConditionTracker* zTracker = &zombie->m_conditionTracker;
    uint8_t* cond = zTracker->m_states.data();

    if (cond != nullptr && *cond != 0) {
        typedef void (*UpdateConditionsFunc)(ZombieConditionTracker*);
        ((UpdateConditionsFunc)(*(void***)zTracker)[3])(zTracker);
        *cond = 0;
    }

    float scale = zTracker->m_scale;
    float shrinkAmount = (float)hitRect.mHeight - (scale * (float)hitRect.mHeight);

    hitRect.mY = static_cast<int>((float)baseY + shrinkAmount);
    hitRect.mHeight = static_cast<int>((float)hitRect.mHeight - shrinkAmount);

    return hitRect;
}
void* ScreenDoorTakeDamage(ZombieModernScreenDoor* thisPtr, DamageInfo* damageInfo)
{
    DamageInfo newDmgInfo = *damageInfo;

    bool isBlacklisted = false;
    bool isPlantAttacker = false;

    auto* props = reinterpret_cast<ZombieModernScreenDoorProps*>(thisPtr->m_propertySheet.Get());

    if (newDmgInfo.m_attacker != nullptr
        && newDmgInfo.m_attacker != reinterpret_cast<BoardEntity*>(thisPtr))
    {
        if (newDmgInfo.m_attacker->IsType(Plant::StaticGetType()))
        {
            isPlantAttacker = true;

            Plant* plant = static_cast<Plant*>(newDmgInfo.m_attacker);
            if (plant->m_type.Get() != nullptr)
            {
                auto* plantType = reinterpret_cast<PlantType*>(plant->m_type.Get());
                for (const SexyString& blacklistedPlant : props->PlantBlacklist)
                {
                    if (plantType->TypeName == blacklistedPlant)
                    {
                        isBlacklisted = true;
                        break;
                    }
                }
            }
        }
    }

    bool isShooter = (newDmgInfo.m_flags & DamageTypeFlags::damage_shooter) != 0;

    if (isShooter)
    {

    }
    else
    {
        if (isPlantAttacker && !isBlacklisted)
        {
            newDmgInfo.m_flags |= DamageTypeFlags::damage_bypass_shield;
        }
        else
        {
            newDmgInfo.m_flags &= ~DamageTypeFlags::damage_bypass_shield;
        }
    }

    typedef void* (*funcC43B90)(ZombieModernScreenDoor*, DamageInfo*);
    static auto* ZTakeDmg = ((funcC43B90)getActualOffset(0xC43B90));
    return ZTakeDmg(thisPtr, &newDmgInfo);
}
bool ScreenDoorBlockProjectile(ZombieModernScreenDoor* thisPtr, Projectile* proj)
{
    void** projVTable = *(void***)proj;
    VirtualGetDamageFlagsFunc getDamageFlags = (VirtualGetDamageFlagsFunc)projVTable[22];
    uint64_t projDamageFlags = getDamageFlags(proj);

    void** zombieVTable = *(void***)thisPtr;
    VirtualIsValidTargetFunc isValidTarget = (VirtualIsValidTargetFunc)zombieVTable[21];

    if (!isValidTarget(thisPtr, projDamageFlags))
    {
        return true;
    }
    if (!(((proj->m_teamFlags & 2) != 0 && (thisPtr->m_teamFlags & 1) != 0) || ((proj->m_teamFlags & 1) != 0 && (thisPtr->m_teamFlags & 2) != 0)))
    {
        return true;
    }

    if (ZombieHasArmor(thisPtr, "ScreenDoor") == true)
    {
        bool isInVector = false;

        auto* props = reinterpret_cast<ZombieModernScreenDoorProps*>(thisPtr->m_propertySheet.Get());

        Sexy::RtWeakPtr<ProjectilePropertySheet> projPropsPtr = proj->m_propertySheet;

        auto it = props->PierceableProjectiles.begin();
        auto end = props->PierceableProjectiles.end();

        for (; it != end; ++it)
        {
            if (it->operator==(projPropsPtr))
            {
                isInVector = true;
                break;
            }
        }

        if (isInVector)
        {
            int* projFlags = &proj->m_damageFlags;
            int originalFlags = *projFlags;
            *projFlags = originalFlags | DamageTypeFlags::damage_bypass_shield;
            VirtualDoImpactFunc doImpact = (VirtualDoImpactFunc)projVTable[35];
            doImpact(proj, thisPtr);
            *projFlags = originalFlags;
            return true;
        }

        float projDirection = proj->m_velocity.x * proj->m_velocityScale.x;

        bool isShotInBack = (projDirection * ZombieFacing(thisPtr)) < 0.0f;

        if (isShotInBack)
        {
            int* projFlags = &proj->m_damageFlags;
            int originalFlags = *projFlags;
            *projFlags = originalFlags | DamageTypeFlags::damage_bypass_shield;
            VirtualDoImpactFunc doImpact = (VirtualDoImpactFunc)projVTable[35];
            doImpact(proj, thisPtr);
            *projFlags = originalFlags;
            return true;
        }
        return false;
    }
    else {
        return false;
    }
}
Sexy::Rect ScreenDoorGetAttackRect(ZombieModernScreenDoor* zombie) {
    auto props = reinterpret_cast<ZombieModernScreenDoorProps*>(zombie->m_propertySheet.Get());
    Rect attackRect;
    float offsetX, offsetY;
    if (ZombieHasArmor(zombie, "ScreenDoor"))
    {
        attackRect.mWidth = props->AttackRectWhenHaveDoor.mWidth;
        attackRect.mHeight = props->AttackRectWhenHaveDoor.mHeight;
        offsetX = props->AttackRectWhenHaveDoor.mX;
        offsetY = props->AttackRectWhenHaveDoor.mY;
    }
    else {
        attackRect.mWidth = props->AttackRect.mWidth;
        attackRect.mHeight = props->AttackRect.mHeight;
        offsetX = props->AttackRect.mX;
        offsetY = props->AttackRect.mY;
    }
    float zX = zombie->m_position.x;
    float zY = zombie->m_position.y;
    float zZ = zombie->m_position.z;
    float facing = ZombieFacing(zombie);

    attackRect.mX = static_cast<int>(zX - (attackRect.mWidth / 2.0f) + (facing * offsetX));
    attackRect.mY = static_cast<int>((zY - zZ) - attackRect.mHeight + offsetY);
    return attackRect;
}
void DoorOnArmorDestroyed(ZombieModernScreenDoor* zombie, int a2, SexyString* armorName)
{
    if (*armorName == "ScreenDoor" && !ZombieIsDeadOrDying(zombie) && zombie->m_entityState.m_id != 3) {
        auto rig = reinterpret_cast<ZombieAnimRig_ModernScreenDoor*>(zombie->m_animRig.Get());
        rig->m_hasDoor = false;
        ZombieEnterState(zombie, 16, 0);
    }
}

typedef void* (*GooPeaApplyPoisonFunc)(void* proj, BoardEntity* ent);
GooPeaApplyPoisonFunc oGooPeaApply = nullptr;

void hkGooPeaApplyPoison(void* proj, BoardEntity* ent)
{
    if (ent->IsType(Zombie::StaticGetType())) {
		auto zombie = static_cast<Zombie*>(ent);
        if (ZombieHasArmor(zombie, "ScreenDoor")
            || ZombieHasArmor(zombie, "Newspaper")
            || ZombieHasArmor(zombie, "Scroll")
            || ZombieHasArmor(zombie, "Ladder")
            || ZombieHasArmor(zombie, "Shovel")
            || ZombieHasArmor(zombie, "Sarcophagus")
            || ((zombie->m_zombieFlags & 4) != 0))
        {
            return;
        }
    }
    oGooPeaApply(proj, ent);
}
void ScreenDoorOnCreate(ZombieModernScreenDoor* zombie) {
    auto rig = reinterpret_cast<ZombieAnimRig_ModernScreenDoor*>(zombie->m_animRig.Get());
    rig->m_hasDoor = true;
}
float ScreenDoorGetArmDropFraction(ZombieModernScreenDoor* zombie)
{
    auto props = reinterpret_cast<ZombiePropertySheet*>(zombie->m_propertySheet.Get());
    if (ZombieHasArmor(zombie, "ScreenDoor") || zombie->m_entityState.m_id == 3)
    {
        return -1.0f; 
    }
    else
    {
        return props->ArmDropFraction;
    }
}
/*
float ScreenDoorGetArmDropFraction(ZombieModernScreenDoor* zombie)
{
    auto props = reinterpret_cast<ZombiePropertySheet*>(zombie->m_propertySheet.Get());
    if (ZombieHasArmor(zombie, "ScreenDoor"))
    {
        return -1.0f;
    }
    {
        return props->ArmDropFraction;
    }
}*/
void ZombieModernScreenDoor::LostDoorOnEnter(ZombieModernScreenDoor* zombie)
{
    RegisterEventAfterAnim(zombie, "lose_screendoor", "onLostDoorCompleted");
}

void ZombieModernScreenDoor::LostDoorOnLoop(ZombieModernScreenDoor* zombie)
{

}

void ZombieModernScreenDoor::LostDoorOnExit(ZombieModernScreenDoor* zombie)
{

}
void LostDoorCompletedCallback(Zombie* zombie) {
    ZombieModernScreenDoor* doorZombie = static_cast<ZombieModernScreenDoor*>(zombie);
    if (doorZombie && !ZombieIsDeadOrDying(doorZombie) && !ZombieIsInState(doorZombie, 3)) {
        ZombieEnterState(doorZombie, 1, 0);
    }
}
void ZombieModernScreenDoor::ModInit() {
    LOGI("ZombieDoor mod init");
    PVZ2HookFunction(0xF6FAC0, (void*)hkGooPeaApplyPoison, (void**)&oGooPeaApply);
    vftable = CreateChildVFTable(204 + 9, getActualOffset(0x241D430), 204);
    PatchVFTable(vftable, (void*)ZombieModernScreenDoor::StaticGetType, 0);

    PatchVFTable(vftable, (void*)ScreenDoorGetHitRect, 20);
    PatchVFTable(vftable, (void*)ScreenDoorTakeDamage, 35);
    PatchVFTable(vftable, (void*)ScreenDoorBlockProjectile, 43);
    PatchVFTable(vftable, (void*)ScreenDoorGetAttackRect, 88);
    PatchVFTable(vftable, (void*)DoorOnArmorDestroyed, 115);
    PatchVFTable(vftable, (void*)ScreenDoorOnCreate, 169);
    PatchVFTable(vftable, (void*)ScreenDoorGetArmDropFraction, 194);

    PatchVFTable(vftable, (void*)ZombieModernScreenDoor::LostDoorOnEnter, 204);
    PatchVFTable(vftable, (void*)ZombieModernScreenDoor::LostDoorOnLoop, 205);
    PatchVFTable(vftable, (void*)ZombieModernScreenDoor::LostDoorOnExit, 206);

    ZombieModernScreenDoor::StaticGetType();
    LOGI("ZombieDoor finish init");
}

void ZombieModernScreenDoor::buildEventCallbacks(Reflection::CRefManualSymbolBuilder* builder, Reflection::RClass* rtClass)
{
    IF_CALLBACK_NOTSETUP(ZombieModernScreenDoor) {
        SetupLiteralDelegate(&lostDoorCompletedDelegate, LostDoorCompletedCallback);
        ZombieModernScreenDoor_delegatesSetup = true;
        LOGI("SO TRUE");
    }
    RegisterEventCallback(builder, rtClass, "onLostDoorCompleted", lostDoorCompletedDelegate);
    LOGI("Reg event complete");
}


void ZombieModernScreenDoor::buildStates()
{
    StateMachineTableBuilder* stateMachine = CallGetStateMachine(ZombieModernScreenDoor::StaticGetType());
    RegisterStateByOffsets(stateMachine,
        16,
        (uintptr_t)ZombieModernScreenDoor::LostDoorOnEnter,
        (uintptr_t)ZombieModernScreenDoor::LostDoorOnLoop,
        (uintptr_t)ZombieModernScreenDoor::LostDoorOnExit,
        "ZS_ScreenDoor_LostDoor");
    LOGI("Reg state complete");
}
