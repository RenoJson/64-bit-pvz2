#include "ZombieLadder.h"
#include "ZombieStateHelper.h"
#include "TimeMgr.h"
#include "DamageInfo.h"
#include "StateMachineBuilder.h"
#include "ZombieHelper.h"
#include "Board.h"
#include "Plant.h"
#include "AddGridItemType.h"
#include "Ladder.h"
#include "AddZombieType.h"


void* ZombieModernLadder::vftable = nullptr;
Sexy::RtClass* ZombieModernLadder::s_rtClass = nullptr;;

void* ZombieModernLadderProps::vftable = __null;
Sexy::RtClass* ZombieModernLadderProps::s_rtClass = __null;

void* ZombieAnimRig_ModernLadder::vftable = __null;
Sexy::RtClass* ZombieAnimRig_ModernLadder::s_rtClass = __null;;

DECLARE_DELEGATES_SETUP(ZombieModernLadder)

static Sexy::DelegateBase placeLadderCompletedDelegate;

static Sexy::DelegateBase lostLadderCompletedDelegate;

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
void LadderOnSpawn(ZombieModernLadder* zombie) {
    ZombieOnSpawn(zombie);
    auto rig = reinterpret_cast<ZombieAnimRig_ModernLadder*>(zombie->m_animRig.Get());
    auto props = reinterpret_cast<ZombieModernLadderProps*>(zombie->m_propertySheet.Get());
    SetWalkSpeed(rig, props->SpeedWhenHaveLadder);
}
void LadderOnArmorDestroyed(ZombieModernLadder* zombie, int a2, SexyString* armorName)
{
    if (*armorName == "Ladder" && !ZombieIsDeadOrDying(zombie) && zombie->m_entityState.m_id != 3) {
        auto rig = reinterpret_cast<ZombieAnimRig_ModernLadder*>(zombie->m_animRig.Get());
        rig->m_hasLadder = false;
        ZombieEnterState(zombie, 17, 0);
    }
}

void LadderWalkOnLoop(ZombieModernLadder* zombie)
{
    auto rig = reinterpret_cast<ZombieAnimRig_ModernLadder*>(zombie->m_animRig.Get());

    if (rig != nullptr && rig->m_hasLadder == true && zombie->m_teamFlags == 2)
    {
        if ((zombie->m_realObjectFlags & 2) == 0) {

            auto* props = reinterpret_cast<ZombieModernLadderProps*>(zombie->m_propertySheet.Get());

            float zX = zombie->m_position.x;
            float zY = zombie->m_position.y;
            float zZ = zombie->m_position.z;

            Rect attackRect;
            attackRect.mWidth = props->LadderRect.mWidth;
            attackRect.mHeight = props->LadderRect.mHeight;
            attackRect.mX = static_cast<int>(zX - (attackRect.mWidth / 2.0f) + props->LadderRect.mX);
            attackRect.mY = static_cast<int>((zY - zZ) - attackRect.mHeight + props->LadderRect.mY);

            int zRow = static_cast<int>((zY - 160.0f) / 76.0f);

            std::vector<BoardEntity*> entityList;

            GetEntitiesInRectPixel(&entityList, 38, &attackRect, zRow, zRow);

            for (BoardEntity* entity : entityList) {
                if (entity != nullptr && entity->IsType(ZombieLadder::StaticGetType())) {
                    CallFunc<void>(0xC506B4, zombie);
                    return;
                }
            }

            bool foundValidPlant = false;
            Plant* targetPlant = nullptr;

            for (BoardEntity* entity : entityList) {
                if (entity != nullptr && entity->IsType(PlantGroup::StaticGetType())) {
                    PlantGroup* plantGroup = reinterpret_cast<PlantGroup*>(entity);

                    for (auto& weakPlant : plantGroup->m_plants.m_plants) {
                        Plant* p = weakPlant.Get();
                        if (p != nullptr) {
                            if (CallFunc<bool>(0x1364888, &props->PlantsWhichPlaceLadderInsteadEating, p)) {
                                foundValidPlant = true;
                                targetPlant = p;
                                break;
                            }
                        }
                    }
                }
                if (foundValidPlant) break;
            }

            if (foundValidPlant && targetPlant != nullptr) {
                zombie->m_eatTarget.FromOther(&targetPlant->m_thisPtr);
                ZombieEnterState(zombie, 16, 0);
                return;
            }
        }
    }

    CallFunc<void>(0xC506B4, zombie);
}

void LadderEatOnLoop(ZombieModernLadder* zombie)
{
    BoardEntity* entity = CallVirtualFunc<BoardEntity*>(zombie, 108);
    auto rig = reinterpret_cast<ZombieAnimRig_ModernLadder*>(zombie->m_animRig.Get());
    if (rig->m_hasLadder) {
        if (entity != nullptr && entity->IsType(PlantGroup::StaticGetType()))
        {
            auto pGroup = reinterpret_cast<PlantGroup*>(entity);
            auto& plantVector = pGroup->m_plants.m_plants;

            if (!plantVector.empty() && plantVector[0].IsValid())
            {
                Plant* p0 = reinterpret_cast<Plant*>(plantVector[0].Get());
                auto props = reinterpret_cast<ZombieModernLadderProps*>(zombie->m_propertySheet.Get());
                if (CallFunc<bool>(0x1364888, &props->PlantsWhichPlaceLadderInsteadEating, p0))
                {
                    zombie->m_eatTarget.FromOther(&p0->m_thisPtr);
                    ZombieEnterState(zombie, 16, 0);
                    return;
                }
            }
        }
    }
    CallFunc<void>(0xC5082C, zombie);
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

        if (ZombieHasArmor(zombie, "Ladder"))
        {
            ZombieLadder* ladder = (ZombieLadder*)AddZombie("ladder", -1, 6, 1);

            ladder->m_attachedPlant.FromOther(&zombie->m_eatTarget);

            for (auto& armorWeakPtr : zombie->m_armor)
            {
                if (!armorWeakPtr.IsValid()) continue;
                Armor* armor = reinterpret_cast<Armor*>(armorWeakPtr.Get());

                if (armor != nullptr && !armor->m_destroyed && armor->m_propertySheetPtr.IsValid())
                {
                    ArmorPropertySheet* props = reinterpret_cast<ArmorPropertySheet*>(armor->m_propertySheetPtr.Get());

                    if (props != nullptr && props->ArmorType == "Ladder")
                    {
                        if (ladder->m_attachedPlant.IsValid())
                        {
                            auto attachTarg = reinterpret_cast<Plant*>(ladder->m_attachedPlant.Get());

                            SexyVector3 ladderPos = { attachTarg->m_position.x + 30.0f, attachTarg->m_position.y, 0 };
                            ZombieSetPosition(ladder, &ladderPos);
							ZombieEnterState(ladder, 1, 0);
                            armor->m_destroyed = true;
                            auto rig = reinterpret_cast<ZombieAnimRig_ModernLadder*>(zombie->m_animRig.Get());
                            rig->m_hasLadder = false;
                            break;
                        }
                    }
                }
            }
        }
    }
}

void ZombieModernLadder::PlaceLadderOnEnter(ZombieModernLadder* zombie)
{
    ZombieSetUnmovableStatusFlag(zombie, true);
    RegisterEventAfterAnim(zombie, "drop_ladder", "onPlaceLadderCompleted");
}

void ZombieModernLadder::PlaceLadderOnLoop(ZombieModernLadder* zombie)
{

}

void ZombieModernLadder::PlaceLadderOnExit(ZombieModernLadder* zombie)
{
    ZombieSetUnmovableStatusFlag(zombie, false);
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
    if (LadderZombie && !ZombieIsDeadOrDying(LadderZombie) && !ZombieIsInState(LadderZombie, 3)) {
        auto rig = reinterpret_cast<ZombieAnimRig_ModernLadder*>(LadderZombie->m_animRig.Get());
        auto props = reinterpret_cast<ZombieModernLadderProps*>(LadderZombie->m_propertySheet.Get());
        rig->m_hasLadder = false;
        SetWalkSpeed(rig, LadderZombie->m_walkSpeed);
        ZombieEnterState(LadderZombie, 1, 0);
    }
}

void PlaceLadderCompletedCallback(Zombie* zombie) {
    ZombieModernLadder* LadderZombie = static_cast<ZombieModernLadder*>(zombie);
    if (LadderZombie && !ZombieIsDeadOrDying(LadderZombie) && !ZombieIsInState(LadderZombie, 3)) {
        ZombieSetUnmovableStatusFlag(LadderZombie, false);
        auto rig = reinterpret_cast<ZombieAnimRig_ModernLadder*>(LadderZombie->m_animRig.Get());
        auto props = reinterpret_cast<ZombieModernLadderProps*>(LadderZombie->m_propertySheet.Get());
        rig->m_hasLadder = false;
        SetWalkSpeed(rig, LadderZombie->m_walkSpeed);
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
    PatchVFTable(vftable, (void*)LadderOnSpawn, 49);
    PatchVFTable(vftable, (void*)LadderOnArmorDestroyed, 115);
    PatchVFTable(vftable, (void*)LadderWalkOnLoop, 124);
    PatchVFTable(vftable, (void*)LadderEatOnLoop, 127);
    PatchVFTable(vftable, (void*)LadderOnCreate, 169);
    PatchVFTable(vftable, (void*)LadderActionFrame, 170);

    PatchVFTable(vftable, (void*)ZombieModernLadder::PlaceLadderOnEnter, 204);
    PatchVFTable(vftable, (void*)ZombieModernLadder::PlaceLadderOnLoop, 205);
    PatchVFTable(vftable, (void*)ZombieModernLadder::PlaceLadderOnExit, 206);

    PatchVFTable(vftable, (void*)ZombieModernLadder::LostLadderOnEnter, 207);
    PatchVFTable(vftable, (void*)ZombieModernLadder::LostLadderOnLoop, 208);
    PatchVFTable(vftable, (void*)ZombieModernLadder::LostLadderOnExit, 209);

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

void* hkInitModernLadderLowerArmList(ZombieAnimRig_ModernLadder* thisptr) {

    static std::vector<SexyString> ModernScreenNoLadderLowerArmList = {
       "zombie_arm_outer_lower",
       "zombie_arm_outer_lower_02",
       "zombie_arm_outer_lower_03",
       "zombie_hand_outer_01",
       "zombie_hand_outer_02",
       "zombie_hand_outer_03",
       "zombie_hand_grip",
       "zombie_hand_outer",
       "zombie_hand_outer_no_shovel"
    };
    return &ModernScreenNoLadderLowerArmList;
}
void* hkInitModernLadderUpperArmList() {

    static std::vector<SexyString> ModernLadderUpperArmList = {
        "zombie_arm_outer_upper",
        "zombie_arm_outer_upper_bone"
    };
    return &ModernLadderUpperArmList;

}
void* hkInitModernLadderHeadList() {

    static std::vector<SexyString> ModernLadderHeadList = {
       "zombie_skull",
       "zombie_jaw"
    };
    return &ModernLadderHeadList;
}
SexyString hkIdleAnim(ZombieAnimRig_ModernLadder* thisptr) {
    if (thisptr->m_hasLadder == true) {
        return "idle";
    }
    else {
        return "idle_no_ladder";
    }
}
SexyString hkWalkAnim(ZombieAnimRig_ModernLadder* thisptr) {
    if (thisptr->m_hasLadder == true) {
        return "walk";
    }
    else {
        return "walk_no_ladder";
    }
}
SexyString hkEatAnim(ZombieAnimRig_ModernLadder* thisptr) {
    if (thisptr->m_hasLadder == true) {
        return "eat_ladder";
    }
    else {
        return "eat";
    }
}
SexyString hkDieAnim(ZombieAnimRig_ModernLadder* thisptr) {
    if (thisptr->m_hasLadder == true) {
        return "die_ladder";
    }
    else {
        return "die";
    }
}

void CreateLadderAnimRig(ZombieAnimRig_ModernLadder* thisPtr)
{
    CallFunc<void>(0x8DE104, thisPtr);
    SetAnimLayerVisible(thisPtr, "ladder_damage1", false);
    SetAnimLayerVisible(thisPtr, "ladder_damage2", false);
}

void ZombieAnimRig_ModernLadder::modInit() {
    LOGI("ZombieAnimRig_ModernLadder init");

    vftable = CopyVFTable(getActualOffset(0x23ABF70), 67);

    PatchVFTable(vftable, (void*)ZombieAnimRig_ModernLadder::StaticGetType, 0);
    PatchVFTable(vftable, (void*)CreateLadderAnimRig, 21);
    PatchVFTable(vftable, (void*)hkInitModernLadderHeadList, 55);
    PatchVFTable(vftable, (void*)hkInitModernLadderLowerArmList, 56);
    PatchVFTable(vftable, (void*)hkInitModernLadderUpperArmList, 57);
    PatchVFTable(vftable, (void*)hkIdleAnim, 58);
    PatchVFTable(vftable, (void*)hkWalkAnim, 59);
    PatchVFTable(vftable, (void*)hkEatAnim, 61);
    PatchVFTable(vftable, (void*)hkDieAnim, 63);

    ZombieAnimRig_ModernLadder::StaticGetType();

    LOGI("ZombieAnimRig_ModernLadder finish init");
}
