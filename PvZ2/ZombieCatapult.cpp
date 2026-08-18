#include "ZombieCatapult.h"
#include "ZombieStateHelper.h"
#include "TimeMgr.h"
#include "DamageInfo.h"
#include "StateMachineBuilder.h"
#include "ZombieHelper.h"
#include "Board.h"
#include "Plant.h"
#include "Sexy/LawnApp.h"
#include "AddProjectileType.h"

void* ZombieCatapult::vftable = nullptr;
Sexy::RtClass* ZombieCatapult::s_rtClass = nullptr;

void* ZombieCatapultProps::vftable = nullptr;
Sexy::RtClass* ZombieCatapultProps::s_rtClass = nullptr;

void* ZombieAnimRig_Catapult::vftable = nullptr;
Sexy::RtClass* ZombieAnimRig_Catapult::s_rtClass = nullptr;

DECLARE_DELEGATES_SETUP(ZombieCatapult)

static Sexy::DelegateBase attackCompletedDelegate;

bool CatapultHasTarget(ZombieCatapult* zombie)
{
    int zX = static_cast<int>((zombie->m_position.x - 200.0f) / 64.0f);
    int zY = static_cast<int>((zombie->m_position.y - 160.0f) / 76.0f);

    Rect ExplodeRect;
    ExplodeRect.mX = (zX - 8 > 0) ? (zX - 8) : 0;
    ExplodeRect.mY = zY;
    ExplodeRect.mWidth = 8;
    ExplodeRect.mHeight = 1;

    std::vector<BoardEntity*> entityList;
    GetEntitiesInRectGrid(&entityList, 63, &ExplodeRect);

    for (BoardEntity* ptr : entityList) {
        if (ptr != nullptr && ptr->IsType(PlantGroup::StaticGetType()))
        {
            PlantGroup* plant = static_cast<PlantGroup*>(ptr);
            if (zombie->m_position.x - plant->m_position.x > 0.0f) {
                return true;
            }
        }
    }
    return false;
}

void InitCatapultDamageLayers(ZombieCatapult* zombie, int damageState) {
    auto rig = reinterpret_cast<ZombieAnimRig_Catapult*>(zombie->m_animRig.Get());
    SetAnimLayerVisible(rig, "zombie_catapult_barrier_damaged0", damageState == 0);
    SetAnimLayerVisible(rig, "zombie_catapult_pole_damaged0", damageState == 0);
    SetAnimLayerVisible(rig, "zombie_catapult_barrier_damaged1", damageState == 1);
    SetAnimLayerVisible(rig, "zombie_catapult_pole_damaged1", damageState == 1);
}
void CatapultUpdate(ZombieCatapult* zombie) {
    auto props = reinterpret_cast<ZombieCatapultProps*>(zombie->m_propertySheet.Get());
    int hpStage = props->Hitpoints / 2;
    if (hpStage <= 0) hpStage = 1;
    int curState = 1 - (static_cast<int>(zombie->m_hitpoints) / hpStage);
    curState = std::max(0, std::min(curState, 1));
    if (curState != zombie->m_damageIndex) {
        zombie->m_damageIndex = curState;
        InitCatapultDamageLayers(zombie, zombie->m_damageIndex);
    }
    if (zombie->m_damageIndex == 1 && !ZombieIsDeadOrDying(zombie)) {
        auto type = reinterpret_cast<ZombieType*>(zombie->m_type.Get());
        if (zombie->m_attachedEffects.GetObjectIndex("injured") == -1) {
            SexyVector3 effectOffset = { 0.0f, 0.0f, 40.0f };
            ZombieAttachEffect(zombie, 
                                "injured", 
                                type->PopAnim.c_str(), 
                                "yan", 
                                effectOffset, 
                                1, 
                                false, 
                                false, 
                                2);
        }
    }
    CallFunc<void>(0xC3D7A0, zombie);
}

bool CatapultCanTargetGroundPlant() {
    return true;
}

void CatapultWalkOnLoop(ZombieCatapult* zombie)
{
    CallFunc<void>(0xC506B4, zombie);

    if (zombie->m_position.x <= 700.0f && zombie->m_remainingAmmo > 0)
    {
        if (CatapultHasTarget(zombie))
        {
            ZombieEnterState(zombie, 16, 0);
        }
    }
}

void CatapultEatOnLoop(ZombieCatapult* zombie)
{
    BoardEntity* entity = CallVirtualFunc<BoardEntity*>(zombie, 108);
    if (entity != nullptr) {
        if (entity->IsType(PlantGroup::StaticGetType())) {
            auto pGroup = reinterpret_cast<PlantGroup*>(entity);
            auto& plantVector = pGroup->m_plants.m_plants;

            if (!plantVector.empty() && plantVector[0].IsValid()) {
                Plant* p0 = reinterpret_cast<Plant*>(plantVector[0].Get());
                auto props = reinterpret_cast<ZombieCatapultProps*>(zombie->m_propertySheet.Get());
                if (CallFunc<bool>(0x1364888, &props->PlantsWhichBreakCatapultOnCollision, p0)) {
                    DamageInfo dmg;
                    dmg.m_attacker = p0;
                    dmg.m_damage = 100000.0f;
                    dmg.m_flags = DamageTypeFlags::damage_fatal;
                    CallVirtualFunc<void>(zombie, 35, &dmg);
                    CallFunc<void>(0x12796DC, p0);
                    return;
                }
                else {
                    CallFunc<void>(0xC5082C, zombie);
                    return;
                }
            }
        }
        else {
            CallFunc<void>(0xC5082C, zombie);
            return;
        }
    }
    CallFunc<void>(0xC5082C, zombie);
}



void CatapultOnInitialize(ZombieCatapult* zombie) {
    auto props = reinterpret_cast<ZombieCatapultProps*>(zombie->m_propertySheet.Get());
    zombie->m_remainingAmmo = props->Ammo;
}
void CatapultActionFrame(ZombieCatapult* self, SexyString* currentAnim, SexyString* actionName, SexyString* param, float nextFrameTime)
{
    if (*actionName == "fire")
    {
        auto props = reinterpret_cast<ZombieCatapultProps*>(self->m_propertySheet.Get());

        int zX = static_cast<int>((self->m_position.x - 200.0f) / 64.0f);
        int zY = static_cast<int>((self->m_position.y - 160.0f) / 76.0f);

        Rect ExplodeRect;
        ExplodeRect.mX = (zX - 8 > 0) ? (zX - 8) : 0;
        ExplodeRect.mY = zY;
        ExplodeRect.mWidth = 8;
        ExplodeRect.mHeight = 1;

        std::vector<BoardEntity*> entityList;
        GetEntitiesInRectGrid(&entityList, 63, &ExplodeRect);

        PlantGroup* backmostPlant = nullptr; 
        float maxDistance = -1.0f;    

        for (BoardEntity* ptr : entityList) {
            if (ptr != nullptr && ptr->IsType(PlantGroup::StaticGetType()))
            {
                PlantGroup* plant = static_cast<PlantGroup*>(ptr);
                float distance = self->m_position.x - plant->m_position.x;

                if (distance > 0.0f && distance > maxDistance)
                {
                    maxDistance = distance;
                    backmostPlant = plant;
                }
            }
        }

        if (backmostPlant != nullptr)
        {
            auto props = reinterpret_cast<ZombieCatapultProps*>(self->m_propertySheet.Get());
            auto rig = reinterpret_cast<ZombieAnimRig_Catapult*>(self->m_animRig.Get());

            Rect projRect;
            SexyString layerName = *param + "_1";
            GetAnimRigSpriteRect(rig, layerName, &projRect);

            ZombieConditionTracker* zTracker = &self->m_conditionTracker;
            uint8_t* cond = zTracker->m_states.data();

            if (cond != nullptr && *cond != 0) {
                typedef void (*UpdateConditionsFunc)(ZombieConditionTracker*);
                ((UpdateConditionsFunc)(*(void***)zTracker)[3])(zTracker);
                *cond = 0;
            }

            float scale = zTracker->m_scale;
            auto sexyApp = SexyApp::GetInstance();

            int spriteXPos = sexyApp->ScaleRender(projRect.mX + (projRect.mWidth / 2));
            int spriteYPos = sexyApp->ScaleRender(projRect.mY + (projRect.mHeight / 2));

            SexyVector2 artCenter = props->ArtCenter;
            SexyVector3 shadowOffset = props->ShadowOffset;
            float spawnPosX = self->m_position.x + (scale * ((float)spriteXPos - artCenter.x));
            float spawnPosY = self->m_position.y - self->m_position.z;
            float spawnPosZ = scale * ((float)spriteYPos - shadowOffset.x);
            auto projectileToSpawn = props->Projectile;

            auto projectile = AddProjectile(&projectileToSpawn, self, spawnPosX, spawnPosY, spawnPosZ);
            FirePultProjectile(projectile, 
                                backmostPlant->m_position, 
                                props->ProjectileShotHeight, 
                                props->ProjectileShotAirTime);
            SexyString projLayer = *param + "_" + std::to_string(self->m_remainingAmmo);
            SetAnimLayerVisible(rig, projLayer, false);
            self->m_remainingAmmo--;
        }
    }
}

void CatapultOnTakeFatalDamage(ZombieCatapult* zombie, DamageInfo* dmgInfo) {
    RemoveAttachedEffect(&zombie->m_attachedEffects, "injured");
}

SexyString GetCatapultElectrocuteAnim() {
    return "POPANIM_EFFECTS_ZOMBIE_CATAPULT_SHOCK";
}

SexyString GetCatapultAshAnim() {
    return "POPANIM_EFFECTS_ZOMBIE_CATAPULT_ASH";
}

int64_t CatapultGetEatingDamageFlag() {
    return 0x80000;
}

bool CatapultIsImmuneToShrinking() {
    return true;
}

void ZombieCatapult::WaitingOnEnter(ZombieCatapult* zombie)
{
    auto dlgtEvent = RegisterDelegateEvent(zombie, "onIdleAnimationCycle");
    auto rig = reinterpret_cast<ZombieAnimRig_Catapult*>(zombie->m_animRig.Get());
    PlayAndContinueAnim(rig, "idle", 3, dlgtEvent);
}

void ZombieCatapult::WaitingOnLoop(ZombieCatapult* zombie)
{
    if (!CatapultHasTarget(zombie))
    {
        ZombieEnterState(zombie, 1, 0); 
        return;
    }

    auto props = reinterpret_cast<ZombieCatapultProps*>(zombie->m_propertySheet.Get());
    if (zombie->m_elapsedTimeInState >= props->FireInterval)
    {
        ZombieEnterState(zombie, 17, 0); 
    }
    BoardEntity* entity = CallVirtualFunc<BoardEntity*>(zombie, 108);
    if (entity != nullptr) {
        if (entity->IsType(PlantGroup::StaticGetType())) {
            auto pGroup = reinterpret_cast<PlantGroup*>(entity);
            auto& plantVector = pGroup->m_plants.m_plants;

            if (!plantVector.empty() && plantVector[0].IsValid()) {
                Plant* p0 = reinterpret_cast<Plant*>(plantVector[0].Get());
                auto props = reinterpret_cast<ZombieCatapultProps*>(zombie->m_propertySheet.Get());
                if (CallFunc<bool>(0x1364888, &props->PlantsWhichBreakCatapultOnCollision, p0)) {
                    DamageInfo dmg;
                    dmg.m_attacker = p0;
                    dmg.m_damage = 100000.0f;
                    dmg.m_flags = DamageTypeFlags::damage_fatal;
                    CallVirtualFunc<void>(zombie, 35, &dmg);
                    CallFunc<void>(0x12796DC, p0);
                    return;
                }
            }
        }
    }
}

void ZombieCatapult::WaitingOnExit(ZombieCatapult* zombie)
{
    
}

void ZombieCatapult::AttackOnEnter(ZombieCatapult* zombie)
{
    RegisterEventAfterAnim(zombie, "attack", "onAttackDone");
}

void ZombieCatapult::AttackOnLoop(ZombieCatapult* zombie)
{
    BoardEntity* entity = CallVirtualFunc<BoardEntity*>(zombie, 108);
    if (entity != nullptr) {
        if (entity->IsType(PlantGroup::StaticGetType())) {
            auto pGroup = reinterpret_cast<PlantGroup*>(entity);
            auto& plantVector = pGroup->m_plants.m_plants;

            if (!plantVector.empty() && plantVector[0].IsValid()) {
                Plant* p0 = reinterpret_cast<Plant*>(plantVector[0].Get());
                auto props = reinterpret_cast<ZombieCatapultProps*>(zombie->m_propertySheet.Get());
                if (CallFunc<bool>(0x1364888, &props->PlantsWhichBreakCatapultOnCollision, p0)) {
                    DamageInfo dmg;
                    dmg.m_attacker = p0;
                    dmg.m_damage = 100000.0f;
                    dmg.m_flags = DamageTypeFlags::damage_fatal;
                    CallVirtualFunc<void>(zombie, 35, &dmg);
                    CallFunc<void>(0x12796DC, p0);
                    return;
                }
            }
        }
    }
}

void ZombieCatapult::AttackOnExit(ZombieCatapult* zombie)
{

}

void OnAttackCompletedCallback(Zombie* zombie) {
    auto pultZombie = static_cast<ZombieCatapult*>(zombie);
    if (pultZombie && !ZombieIsDeadOrDying(pultZombie)) {
        if (!CatapultHasTarget(pultZombie) || pultZombie->m_remainingAmmo <= 0) {
            ZombieEnterState(pultZombie, 1, 0);
        }
        else {
            ZombieEnterState(pultZombie, 16, 0);
        }
    }
}

//Anim Rig Definition

void CatapultCreateAnimRig(ZombieAnimRig_Catapult* rig) {
    CallFunc<void>(0x8DE104, rig);
    SetAnimLayerVisible(rig, "zombie_catapult_barrier_damaged1", false);
    SetAnimLayerVisible(rig, "zombie_catapult_pole_damaged1", false);
}

SexyString GetCatapultEatAnim() {
    return "walk";
}

void ZombieCatapult::modInit() {
    LOGI("ZombieCatapult mod init");

    vftable = CreateChildVFTable(204 + 15, getActualOffset(0x241D430), 204);
    PatchVFTable(vftable, (void*)ZombieCatapult::StaticGetType, 0);

    PatchVFTable(vftable, (void*)CatapultUpdate, 29);
    PatchVFTable(vftable, (void*)CatapultCanTargetGroundPlant, 103);
    PatchVFTable(vftable, (void*)CatapultWalkOnLoop, 124);
    PatchVFTable(vftable, (void*)CatapultEatOnLoop, 127);
    PatchVFTable(vftable, (void*)CatapultOnInitialize, 169);
    PatchVFTable(vftable, (void*)CatapultActionFrame, 170);
    PatchVFTable(vftable, (void*)CatapultOnTakeFatalDamage, 185);
    PatchVFTable(vftable, (void*)GetCatapultElectrocuteAnim, 189);
    PatchVFTable(vftable, (void*)GetCatapultAshAnim, 190);
    PatchVFTable(vftable, (void*)CatapultGetEatingDamageFlag, 193);
    PatchVFTable(vftable, (void*)CatapultIsImmuneToShrinking, 199);

    PatchVFTable(vftable, (void*)ZombieCatapult::WaitingOnEnter, 204);
    PatchVFTable(vftable, (void*)ZombieCatapult::WaitingOnLoop, 205);
    PatchVFTable(vftable, (void*)ZombieCatapult::WaitingOnExit, 206);

    PatchVFTable(vftable, (void*)ZombieCatapult::AttackOnEnter, 204);
    PatchVFTable(vftable, (void*)ZombieCatapult::AttackOnLoop, 205);
    PatchVFTable(vftable, (void*)ZombieCatapult::AttackOnExit, 206);

    ZombieCatapult::StaticGetType();
    LOGI("ZombieCatapult finish init");
}

void ZombieCatapultProps::modInit() {
    LOGI("ZombieCatapultProps init");

    vftable = CopyVFTable(getActualOffset(0x24328D0), 14);

    PatchVFTable(vftable, (void*)ZombieCatapultProps::StaticGetType, 0);

    ZombieCatapultProps::StaticGetType();

    LOGI("ZombieCatapultProps finish init");
}

void ZombieAnimRig_Catapult::modInit() {
    LOGI("ZombieAnimRig_Catapult init");

    vftable = CopyVFTable(getActualOffset(0x23ABF70), 67);

    PatchVFTable(vftable, (void*)ZombieAnimRig_Catapult::StaticGetType, 0);
    PatchVFTable(vftable, (void*)CatapultCreateAnimRig, 21);
    PatchVFTable(vftable, (void*)GetCatapultEatAnim, 61);
    ZombieAnimRig_Catapult::StaticGetType();

    LOGI("ZombieAnimRig_Catapult finish init");
}

void ZombieCatapult::buildEventCallbacks(Reflection::CRefManualSymbolBuilder* builder, Reflection::RClass* rtClass)
{
    IF_CALLBACK_NOTSETUP(ZombieCatapult) {
        SetupLiteralDelegate(&attackCompletedDelegate, OnAttackCompletedCallback);
        ZombieCatapult_delegatesSetup = true;
        LOGI("SO TRUE");
    }
    RegisterEventCallback(builder, rtClass, "onAttackDone", attackCompletedDelegate);
    LOGI("Reg event complete");
}

void ZombieCatapult::buildStates()
{
    StateMachineTableBuilder* stateMachine = CallGetStateMachine(ZombieCatapult::StaticGetType());
    RegisterStateByOffsets(stateMachine,
        16,
        (uintptr_t)ZombieCatapult::WaitingOnEnter,
        (uintptr_t)ZombieCatapult::WaitingOnLoop,
        (uintptr_t)ZombieCatapult::WaitingOnExit,
        "ZS_Catapult_Waiting");
    RegisterStateByOffsets(stateMachine,
        17,
        (uintptr_t)ZombieCatapult::AttackOnEnter,
        (uintptr_t)ZombieCatapult::AttackOnLoop,
        (uintptr_t)ZombieCatapult::AttackOnExit,
        "ZS_Catapult_Attack");
    LOGI("Reg state complete");
}

