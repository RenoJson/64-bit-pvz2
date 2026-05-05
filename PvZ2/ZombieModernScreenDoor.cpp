#include "ZombieModernScreenDoor.h"
#include "ZombieModernScreenDoorProps.h"
#include "DamageInfo.h"
#include "ZombieAnimRig_ModernScreenDoor.h"
#include "ZombieStateHelper.h"
#include "ZombieState.h"
#include "StateMachineBuilder.h"
#include "Projectile.h"
DECLARE_DELEGATES_SETUP(ZombieModernScreenDoor)

static Sexy::DelegateBase lostDoorCompletedDelegate; 

void* ZombieModernScreenDoor::vftable = nullptr;
Sexy::RtClass* ZombieModernScreenDoor::s_rtClass = nullptr;;
typedef void (*zombieEnterState)(ZombieModernScreenDoor*, int, int);

typedef Zombie* (*update)(ZombieModernScreenDoor*);
typedef bool (*checkZombieHasCondition)(Zombie*, int);
typedef Zombie* (*removeCondition)(Zombie*, int);
typedef bool (*isDeadOrDying)(ZombieModernScreenDoor*); 
typedef uint64_t(*VirtualGetDamageFlagsFunc)(Projectile*);
typedef bool (*VirtualIsValidTargetFunc)(Zombie*, uint64_t);
typedef bool (*VirtualDoImpactFunc)(Projectile*, Zombie*);
void* ScreenDoorTakeDamage(ZombieModernScreenDoor* thisPtr, DamageInfo* damageInfo)
{
    DamageInfo newDmgInfo = *damageInfo;
    if ((newDmgInfo.m_flags & DamageTypeFlags::damage_shooter) == 0)
    {
        newDmgInfo.m_flags |= DamageTypeFlags::damage_bypass_shield;
    }
    typedef void* (*funcC43B90)(ZombieModernScreenDoor*, DamageInfo*);
    static auto* ZTakeDmg = ((funcC43B90)getActualOffset(0xC43B90));
    return ZTakeDmg(thisPtr, &newDmgInfo);
}
float DoorIsFacingOrNot(Zombie* zombie) {
    if (!zombie->m_facing) {
        return 1.0f;
    }
    return -1.0f;
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

    typedef bool (*HasArmorFunc)(Zombie*, const SexyString&);
    HasArmorFunc hasArmor = (HasArmorFunc)getActualOffset(0xC3F2E4);
    if (hasArmor(thisPtr, "ScreenDoor") == true)
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

        bool NotCoward = (proj->m_velocity.x * proj->m_velocityScale.x > 0.0f != DoorIsFacingOrNot(thisPtr) < 0.0f);
        if (!NotCoward)
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
void DoorOnArmorDestroyed(ZombieModernScreenDoor* zombie, int a2, SexyString* armorName)
{
    isDeadOrDying isDeadFunc = (isDeadOrDying)getActualOffset(0xC3E204);
    if (*armorName == "ScreenDoor" && !isDeadFunc(zombie)) {
        ((zombieEnterState)getActualOffset(0xC3D428))(zombie, 16, 0);
    }
}

typedef void* (*GooPeaApplyPoisonFunc)(void* proj, Zombie* zombie);
GooPeaApplyPoisonFunc oGooPeaApply = nullptr;

void hkGooPeaApplyPoison(void* proj, Zombie* zombie)
{
    typedef bool (*HasArmorFunc)(Zombie*, const SexyString&);
    HasArmorFunc hasArmor = (HasArmorFunc)getActualOffset(0xC3F2E4);
    if (hasArmor(zombie, "ScreenDoor") == true || hasArmor(zombie, "Newspaper"))
    {
        return;
    }
    oGooPeaApply(proj, zombie);
}
void ScreenDoorOnCreate(ZombieModernScreenDoor* zombie) {
    auto rig = reinterpret_cast<ZombieAnimRig_ModernScreenDoor*>(zombie->m_animRig.Get());
    rig->m_hasDoor = true;
}
float ScreenDoorGetArmDropFraction(ZombieModernScreenDoor* zombie)
{
    SexyString armorName = "ScreenDoor";
    auto props = reinterpret_cast<ZombiePropertySheet*>(zombie->m_propertySheet.Get());
    typedef bool (*HasArmorFunc)(Zombie*, SexyString*);
    bool hasDoor = ((HasArmorFunc)getActualOffset(0xC3F2E4))(zombie, &armorName);
    if (hasDoor == false)
    {
        return props->ArmDropFraction;
    }
    else
    {
        return -1.0f;
    }
}
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
    auto rig = reinterpret_cast<ZombieAnimRig_ModernScreenDoor*>(zombie->m_animRig.Get());
    ZombieModernScreenDoor* boxZombie = static_cast<ZombieModernScreenDoor*>(zombie);
    if (boxZombie) {
        rig->m_hasDoor = false;
        ((zombieEnterState)getActualOffset(0xC3D428))(boxZombie, 1, 0);
    }
}
void ZombieModernScreenDoor::ModInit() {
    LOGI("ZombieDoor mod init");
    PVZ2HookFunction(0xF6FAC0, (void*)hkGooPeaApplyPoison, (void**)&oGooPeaApply);
    vftable = CreateChildVFTable(204 + 9, getActualOffset(0x241D430), 204);
    PatchVFTable(vftable, (void*)ZombieModernScreenDoor::StaticGetType, 0);

    PatchVFTable(vftable, (void*)ScreenDoorTakeDamage, 35);
    PatchVFTable(vftable, (void*)ScreenDoorBlockProjectile, 43);
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
