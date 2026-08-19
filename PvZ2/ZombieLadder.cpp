#include "ZombieLadder.h"
#include "ZombieStateHelper.h"
#include "TimeMgr.h"
#include "DamageInfo.h"
#include "StateMachineBuilder.h"
#include "ZombieHelper.h"
#include "Board.h"
#include "Plant.h"
#include "AddGridItemType.h"

DECLARE_DELEGATES_SETUP(ZombieModernLadder)

static Sexy::DelegateBase placeLadderCompletedDelegate;

static Sexy::DelegateBase lostLadderCompletedDelegate;

void* ZombieModernLadder::vftable = nullptr;
Sexy::RtClass* ZombieModernLadder::s_rtClass = nullptr;;

void* ZombieModernLadderProps::vftable = __null;
Sexy::RtClass* ZombieModernLadderProps::s_rtClass = __null;


typedef uint64_t(*VirtualGetDamageFlagsFunc)(Projectile*);
typedef bool (*VirtualIsValidTargetFunc)(Zombie*, uint64_t);
typedef bool (*VirtualDoImpactFunc)(Projectile*, Zombie*);

Sexy::Rect LadderGetHitRect(ZombieModernLadder* zombie) {
    auto props = reinterpret_cast<ZombieModernLadderProps*>(zombie->m_propertySheet.Get());
    Rect hitRect;
    float offsetX, offsetY;

    if (ZombieHasArmor(zombie, "Ladder"))
    {
        hitRect.mWidth = props->HitRectWhenHaveLadder.mWidth;
        hitRect.mHeight = props->HitRectWhenHaveLadder.mHeight;
        offsetX = props->HitRectWhenHaveLadder.mX;
        offsetY = props->HitRectWhenHaveLadder.mY;
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
void* LadderTakeDamage(ZombieModernLadder* thisPtr, DamageInfo* damageInfo)
{
    DamageInfo newDmgInfo = *damageInfo;

    bool isBlacklisted = false;
    bool isPlantAttacker = false;

    auto* props = reinterpret_cast<ZombieModernLadderProps*>(thisPtr->m_propertySheet.Get());

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

    typedef void* (*funcC43B90)(ZombieModernLadder*, DamageInfo*);
    static auto* ZTakeDmg = ((funcC43B90)getActualOffset(0xC43B90));
    return ZTakeDmg(thisPtr, &newDmgInfo);
}
bool LadderBlockProjectile(ZombieModernLadder* thisPtr, Projectile* proj)
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

    if (ZombieHasArmor(thisPtr, "Ladder") == true)
    {
        bool isInVector = false;

        auto* props = reinterpret_cast<ZombieModernLadderProps*>(thisPtr->m_propertySheet.Get());

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
Sexy::Rect LadderGetAttackRect(ZombieModernLadder* zombie) {
    auto props = reinterpret_cast<ZombieModernLadderProps*>(zombie->m_propertySheet.Get());
    Rect attackRect;
    float offsetX, offsetY;
    if (ZombieHasArmor(zombie, "Ladder"))
    {
        attackRect.mWidth = props->AttackRectWhenHaveLadder.mWidth;
        attackRect.mHeight = props->AttackRectWhenHaveLadder.mHeight;
        offsetX = props->AttackRectWhenHaveLadder.mX;
        offsetY = props->AttackRectWhenHaveLadder.mY;
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
void LadderOnArmorDestroyed(ZombieModernLadder* zombie, int a2, SexyString* armorName)
{
    if (*armorName == "Ladder" && !ZombieIsDeadOrDying(zombie) && zombie->m_entityState.m_id != 3) {
        auto rig = reinterpret_cast<ZombieAnimRig_ModernLadder*>(zombie->m_animRig.Get());
        rig->m_hasLadder = false;
        ZombieEnterState(zombie, 17, 0);
    }
}

void LadderOnCreate(ZombieModernLadder* zombie) {
    auto rig = reinterpret_cast<ZombieAnimRig_ModernLadder*>(zombie->m_animRig.Get());
    rig->m_hasLadder = true;
}

void LadderActionFrame(ZombieModernLadder* zombie, SexyString* currentAnim, SexyString* actionName, SexyString* param, float nextFrameTime)
{
    if (*actionName == "use_ladder")
    {
        if (!zombie->m_eatTarget.IsValid()) return;

        auto pGroup = reinterpret_cast<PlantGroup*>(zombie->m_eatTarget.Get());
        if (ZombieHasArmor(zombie, "Ladder"))
        {
            GridItemLadder* ladder = (GridItemLadder*)AddGridItem("ladder", pGroup->m_gridX, pGroup->m_gridY);
            if (ladder == nullptr) return;

            for (auto& armorWeakPtr : zombie->m_armor)
            {
                if (!armorWeakPtr.IsValid()) continue;
                Armor* armor = reinterpret_cast<Armor*>(armorWeakPtr.Get());

                if (armor != nullptr && !armor->m_destroyed && armor->m_propertySheetPtr.IsValid())
                {
                    ArmorPropertySheet* props = reinterpret_cast<ArmorPropertySheet*>(armor->m_propertySheetPtr.Get());

                    if (props != nullptr && props->ArmorType == "Ladder")
                    {
                        ladder->m_currDamageState = armor->m_damageState;
                        ladder->m_health = armor->m_health;
                        ladder->m_healthMax = armor->m_maxHealth;
                        armor->m_destroyed = true;

                        break;
                    }
                }
            }
        }
    }
}

void ZombieModernLadder::PlaceLadderOnEnter(ZombieModernLadder* zombie)
{
    RegisterEventAfterAnim(zombie, "drop_ladder", "onPlaceLadderCompleted");
}

void ZombieModernLadder::PlaceLadderOnLoop(ZombieModernLadder* zombie)
{

}

void ZombieModernLadder::PlaceLadderOnExit(ZombieModernLadder* zombie)
{

}

void ZombieModernLadder::LostLadderOnEnter(ZombieModernLadder* zombie)
{
    RegisterEventAfterAnim(zombie, "lose_ladder", "onLostLadderCompleted");
}

void ZombieModernLadder::LostLadderOnLoop(ZombieModernLadder* zombie)
{

}

void ZombieModernLadder::LostLadderOnExit(ZombieModernLadder* zombie)
{

}
void LostLadderCompletedCallback(Zombie* zombie) {
    ZombieModernLadder* LadderZombie = static_cast<ZombieModernLadder*>(zombie);
    if (LadderZombie) {
        ZombieEnterState(LadderZombie, 1, 0);
    }
}

void PlaceLadderCompletedCallback(Zombie* zombie) {
    ZombieModernLadder* LadderZombie = static_cast<ZombieModernLadder*>(zombie);
    if (LadderZombie) {
        ZombieEnterState(LadderZombie, 1, 0);
    }
}

void ZombieModernLadder::ModInit() {
    LOGI("ZombieLadder mod init");
    vftable = CreateChildVFTable(204 + 9, getActualOffset(0x241D430), 204);
    PatchVFTable(vftable, (void*)ZombieModernLadder::StaticGetType, 0);

    PatchVFTable(vftable, (void*)LadderGetHitRect, 20);
    PatchVFTable(vftable, (void*)LadderTakeDamage, 35);
    PatchVFTable(vftable, (void*)LadderBlockProjectile, 43);
    PatchVFTable(vftable, (void*)LadderGetAttackRect, 88);
    PatchVFTable(vftable, (void*)LadderOnArmorDestroyed, 115);
    PatchVFTable(vftable, (void*)LadderOnCreate, 169);
    PatchVFTable(vftable, (void*)LadderActionFrame, 169);

    PatchVFTable(vftable, (void*)ZombieModernLadder::LostLadderOnEnter, 204);
    PatchVFTable(vftable, (void*)ZombieModernLadder::LostLadderOnLoop, 205);
    PatchVFTable(vftable, (void*)ZombieModernLadder::LostLadderOnExit, 206);

    ZombieModernLadder::StaticGetType();
    LOGI("ZombieLadder finish init");
}

void ZombieModernLadder::buildEventCallbacks(Reflection::CRefManualSymbolBuilder* builder, Reflection::RClass* rtClass)
{
    IF_CALLBACK_NOTSETUP(ZombieModernLadder) {
        SetupLiteralDelegate(&lostLadderCompletedDelegate, LostLadderCompletedCallback);
        SetupLiteralDelegate(&placeLadderCompletedDelegate, PlaceLadderCompletedCallback);
        ZombieModernLadder_delegatesSetup = true;
        LOGI("SO TRUE");
    }
    RegisterEventCallback(builder, rtClass, "onLostLadderCompleted", lostLadderCompletedDelegate);
    RegisterEventCallback(builder, rtClass, "onPlaceLadderCompleted", placeLadderCompletedDelegate);
    LOGI("Reg event complete");
}


void ZombieModernLadder::buildStates()
{
    StateMachineTableBuilder* stateMachine = CallGetStateMachine(ZombieModernLadder::StaticGetType());
    RegisterStateByOffsets(stateMachine,
        16,
        (uintptr_t)ZombieModernLadder::PlaceLadderOnEnter,
        (uintptr_t)ZombieModernLadder::PlaceLadderOnLoop,
        (uintptr_t)ZombieModernLadder::PlaceLadderOnExit,
        "ZS_Ladder_PlaceLadder");
    RegisterStateByOffsets(stateMachine,
        17,
        (uintptr_t)ZombieModernLadder::LostLadderOnEnter,
        (uintptr_t)ZombieModernLadder::LostLadderOnLoop,
        (uintptr_t)ZombieModernLadder::LostLadderOnExit,
        "ZS_Ladder_LostLadder");
    LOGI("Reg state complete");
}

void ZombieModernLadderProps::modInit() {
    LOGI("ZombieModernLadderProps init");

    vftable = CopyVFTable(getActualOffset(0x24328D0), 14);

    PatchVFTable(vftable, (void*)ZombieModernLadderProps::StaticGetType, 0);

    ZombieModernLadderProps::StaticGetType();

    LOGI("ZombieModernLadderProps finish init");
}

