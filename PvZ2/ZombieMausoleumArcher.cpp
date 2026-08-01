#include "ZombieMausoleumArcher.h"
#include "ZombieStateHelper.h"
#include "TimeMgr.h"
#include "DamageInfo.h"
#include "ZombieAnimRig_MausoleumArcher.h"
#include "StateMachineBuilder.h"
#include "ZombieHelper.h"
#include "Board.h"
#include "Plant.h"
#include "AddProjectileType.h"
#include "AddZombieType.h"

void* ZombieMausoleumArcher::vftable = nullptr;
Sexy::RtClass* ZombieMausoleumArcher::s_rtClass = nullptr;

void* ZombieMausoleumArcherProps::vftable = nullptr;
Sexy::RtClass* ZombieMausoleumArcherProps::s_rtClass = nullptr;

typedef int (*playAnimWithoutCallback)(ZombieAnimRig*, const SexyString&, int, DelegateBase&);
typedef int (*playAnimWithCallback)(ZombieAnimRig*, const SexyString&, int, ZombieEvent& event);
typedef ZombieEvent* (*ConstructEvent)(ZombieEvent*, RtWeakPtr<Zombie>& owner, const SexyString& eventName);

DECLARE_DELEGATES_SETUP(ZombieMausoleumArcher)

static Sexy::DelegateBase waitShootCompletedDelegate;

static Sexy::DelegateBase shootCompletedDelegate;

static Sexy::DelegateBase endShootCompletedDelegate;

static Sexy::DelegateBase startShootCompletedDelegate;


void ArcherOnSpawn(ZombieMausoleumArcher* zombie) {
    ZombieOnSpawn(zombie);
    ZombieEnterState(zombie, 16, 0);
}
void MausoleumArcherOnCreate(ZombieMausoleumArcher* zombie) {
    auto props = reinterpret_cast<ZombieMausoleumArcherProps*>(zombie->m_propertySheet.Get());
    auto rig = reinterpret_cast<ZombieAnimRig_MausoleumArcher*>(zombie->m_animRig.Get());
    rig->m_hasCursed = props->CursedAtStart;
    SetAnimLayerVisible(rig, "zombie_eyes_curse", rig->m_hasCursed);
}
void ArcherOnAffectedByCondition(ZombieMausoleumArcher* zombie, int condition) {
    if (condition == zombie_condition_hypnotized) {
        ZombieEnterState(zombie, 19, 0);
    }
}
bool ArcherIsBeingTossedByPlant(ZombieMausoleumArcher* zombie, int a2) {
    if (zombie->m_entityState.m_id == 16
        || zombie->m_entityState.m_id == 17
        || zombie->m_entityState.m_id == 18
        || zombie->m_entityState.m_id == 20) {
        return false;
    }
    else {
        return CallFunc<bool, ZombieMausoleumArcher*, int>(0xC4D2EC, zombie, a2);
    }
}
void MausoleumArcherActionFrame(ZombieMausoleumArcher* self, SexyString* currentAnim, SexyString* actionName, SexyString* param, float nextFrameTime)
{
    if (*actionName == "use_action")
    {
        auto* props = reinterpret_cast<ZombieMausoleumArcherProps*>(self->m_propertySheet.Get());

        int zX = static_cast<int>((self->m_position.x - 200.0f) / 64.0f);
        int zY = static_cast<int>((self->m_position.y - 160.0f) / 76.0f);

        Rect ExplodeRect;
        ExplodeRect.mX = (zX - 8 > 0) ? (zX - 8) : 0;
        ExplodeRect.mY = zY;
        ExplodeRect.mWidth = 8;
        ExplodeRect.mHeight = 1;

        std::vector<BoardEntity*> entityList;
        GetEntitiesInRectGrid(&entityList, 63, &ExplodeRect);

        PlantGroup* closestPlant = nullptr;
        float minDistance = 99999.0f;

        for (BoardEntity* ptr : entityList) {
            if (ptr != nullptr && ptr->IsType(PlantGroup::StaticGetType()))
            {
                PlantGroup* plant = static_cast<PlantGroup*>(ptr);
                float distance = self->m_position.x - plant->m_position.x;

                if (distance > 0.0f && distance < minDistance)
                {
                    minDistance = distance;
                    closestPlant = plant;
                }
            }
        }

        if (closestPlant != nullptr)
        {
            auto proj = AddProjectile(&props->Projectile, self,
                self->m_position.x - props->ProjectileOffset.x,
                self->m_position.y - props->ProjectileOffset.y,
                self->m_position.z - props->ProjectileOffset.z);
            FirePultProjectile(proj, closestPlant->m_position, 250.0f, 2.0f);
        }
    }
    if (*actionName == "ghost_intro")
    {
        auto* props = reinterpret_cast<ZombieMausoleumArcherProps*>(self->m_propertySheet.Get());
        SexyString name = props->SpiritTypeName;
        Zombie* spirit = AddZombie(name, -1, 6, -1);
        float newX = self->m_position.x;
        float newY = self->m_position.y;
        float newZ = self->m_position.z;
        SexyVector3 newCoords = SexyVector3(newX, newY, newZ);
        ZombieSetPosition(spirit, &newCoords);
        if (ZombieHasCondition(self, zombie_condition_hypnotized)) {

            ZombieSetCondition(spirit, zombie_condition_hypnotized, 0, 3.4028e38f, 0.0f);

            spirit->m_teamFlags = self->m_teamFlags;
            typedef void* (*GetHypnoDataFunc)(Zombie*);
            GetHypnoDataFunc funGetHypnoData = (GetHypnoDataFunc)getActualOffset(0xC3E6DC);

            void* hypnoData = funGetHypnoData(self);
            typedef void (*ApplyHypnoDataFunc)(Zombie*, void*);
            ApplyHypnoDataFunc funApplyHypnoData = (ApplyHypnoDataFunc)getActualOffset(0xC41290);

            funApplyHypnoData(spirit, hypnoData);
        }
    }
}
void ZombieMausoleumArcher::WalkIntoPositionOnEnter(ZombieMausoleumArcher* zombie)
{
    ZombieAllowMovement(zombie, true);
    RegisterEventOnWalkLoop(zombie, "onWalkAnimationCycle");
}

void ZombieMausoleumArcher::WalkIntoPositionOnLoop(ZombieMausoleumArcher* zombie)
{
    if (zombie->m_position.x <= 744.0f) {
        ZombieEnterState(zombie, 20, 0);
    }
}

void ZombieMausoleumArcher::WalkIntoPositionOnExit(ZombieMausoleumArcher* zombie)
{

}

void ZombieMausoleumArcher::WaitShootingOnEnter(ZombieMausoleumArcher* zombie)
{
    auto dlgtEvent = RegisterDelegateEvent(zombie, "onWaitingContinue");
    auto rig = reinterpret_cast<ZombieAnimRig_MausoleumArcher*>(zombie->m_animRig.Get());
    PlayAndContinueAnim(rig, "waitshoot", 3, dlgtEvent);
}

void ZombieMausoleumArcher::WaitShootingOnLoop(ZombieMausoleumArcher* zombie)
{
    auto* props = reinterpret_cast<ZombieMausoleumArcherProps*>(zombie->m_propertySheet.Get());
    auto animRig = reinterpret_cast<ZombieAnimRig*>(zombie->m_animRig.Get());

    float hpPercentage = zombie->m_hitpoints / zombie->m_maxHitpoints;
    if (((zombie->m_zombieFlags & 2) != 0) && !ZombieIsDeadOrDying(zombie) && zombie->m_entityState.m_id != 3) {
        ZombieEnterState(zombie, 19, 0);
        return;
    }
    if (zombie->m_elapsedTimeInState >= props->FireInterval)
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

        bool hasTarget = false;

        for (BoardEntity* ptr : entityList) {
            if (ptr != nullptr && ptr->IsType(PlantGroup::StaticGetType()))
            {
                PlantGroup* plant = static_cast<PlantGroup*>(ptr);
                float distance = zombie->m_position.x - plant->m_position.x;

                if (distance > 0.0f) {
                    hasTarget = true;
                    break;
                }
            }
        }
        if (hasTarget) {
            ZombieEnterState(zombie, 18, 0);
            return;
        }
    }
}
void ZombieMausoleumArcher::WaitShootingOnExit(ZombieMausoleumArcher* zombie)
{

}

void ZombieMausoleumArcher::ShootOnEnter(ZombieMausoleumArcher* zombie)
{
    RegisterEventAfterAnim(zombie, "shoot", "onShootingDone");
}

void ZombieMausoleumArcher::ShootOnLoop(ZombieMausoleumArcher* zombie)
{
    if (((zombie->m_zombieFlags & 2) != 0) && !ZombieIsDeadOrDying(zombie) && zombie->m_entityState.m_id != 3) {
        ZombieEnterState(zombie, 19, 0);
    }
}

void ZombieMausoleumArcher::ShootOnExit(ZombieMausoleumArcher* zombie)
{

}

void ZombieMausoleumArcher::EndShootOnEnter(ZombieMausoleumArcher* zombie)
{
    RegisterEventAfterAnim(zombie, "endshoot", "onInjuredDone");
}

void ZombieMausoleumArcher::EndShootOnLoop(ZombieMausoleumArcher* zombie)
{

}

void ZombieMausoleumArcher::EndShootOnExit(ZombieMausoleumArcher* zombie)
{

}
void ZombieMausoleumArcher::StartShootOnEnter(ZombieMausoleumArcher* zombie)
{
    RegisterEventAfterAnim(zombie, "startshoot", "onShootingStart");
}
void ZombieMausoleumArcher::StartShootOnLoop(ZombieMausoleumArcher* zombie)
{
}
void ZombieMausoleumArcher::StartShootOnExit(ZombieMausoleumArcher* zombie)
{
}
void StartShootCompletedCallback(Zombie* zombie) {
    ZombieMausoleumArcher* doorZombie = static_cast<ZombieMausoleumArcher*>(zombie);
    if (doorZombie && !ZombieIsDeadOrDying(zombie) && zombie->m_entityState.m_id != 3) {
        ZombieEnterState(doorZombie, 17, 0);
    }
}
void EndShootingDoneCompletedCallback(Zombie* zombie) {
    ZombieMausoleumArcher* doorZombie = static_cast<ZombieMausoleumArcher*>(zombie);
    if (doorZombie && !ZombieIsDeadOrDying(zombie) && zombie->m_entityState.m_id != 3) {
        ZombieEnterState(doorZombie, 1, 0);
    }
}
void WaitingCompletedCallback(Zombie* zombie){}

void ZombieMausoleumArcher::modInit() {
    LOGI("ZombieArcher mod init");

    vftable = CreateChildVFTable(204 + 18, getActualOffset(0x241D430), 204);
    PatchVFTable(vftable, (void*)ZombieMausoleumArcher::StaticGetType, 0);
    PatchVFTable(vftable, (void*)ArcherOnSpawn, 49);
    PatchVFTable(vftable, (void*)ArcherOnAffectedByCondition, 71);
    PatchVFTable(vftable, (void*)ArcherIsBeingTossedByPlant, 97); 
    PatchVFTable(vftable, (void*)MausoleumArcherOnCreate, 169);
    PatchVFTable(vftable, (void*)MausoleumArcherActionFrame, 170);

    PatchVFTable(vftable, (void*)ZombieMausoleumArcher::WalkIntoPositionOnEnter, 204);
    PatchVFTable(vftable, (void*)ZombieMausoleumArcher::WalkIntoPositionOnLoop, 205);
    PatchVFTable(vftable, (void*)ZombieMausoleumArcher::WalkIntoPositionOnExit, 206);

    PatchVFTable(vftable, (void*)ZombieMausoleumArcher::WaitShootingOnEnter, 207);
    PatchVFTable(vftable, (void*)ZombieMausoleumArcher::WaitShootingOnLoop, 208);
    PatchVFTable(vftable, (void*)ZombieMausoleumArcher::WaitShootingOnExit, 209);

    PatchVFTable(vftable, (void*)ZombieMausoleumArcher::ShootOnEnter, 210);
    PatchVFTable(vftable, (void*)ZombieMausoleumArcher::ShootOnLoop, 211);
    PatchVFTable(vftable, (void*)ZombieMausoleumArcher::ShootOnExit, 212);

    PatchVFTable(vftable, (void*)ZombieMausoleumArcher::ShootOnEnter, 213);
    PatchVFTable(vftable, (void*)ZombieMausoleumArcher::ShootOnLoop, 214);
    PatchVFTable(vftable, (void*)ZombieMausoleumArcher::ShootOnExit, 215);

    PatchVFTable(vftable, (void*)ZombieMausoleumArcher::StartShootOnEnter, 216);
    PatchVFTable(vftable, (void*)ZombieMausoleumArcher::StartShootOnLoop, 217);
    PatchVFTable(vftable, (void*)ZombieMausoleumArcher::StartShootOnExit, 218);

    ZombieMausoleumArcher::StaticGetType();
    LOGI("ZombieArcher finish init");
}

void ZombieMausoleumArcherProps::modInit() {
    LOGI("ZombieArcherProps init");

    vftable = CopyVFTable(getActualOffset(0x24328D0), 14);

    PatchVFTable(vftable, (void*)ZombieMausoleumArcherProps::StaticGetType, 0);

    ZombieMausoleumArcherProps::StaticGetType();

    LOGI("ZombieArcherProps finish init");
}

void ZombieMausoleumArcher::buildEventCallbacks(Reflection::CRefManualSymbolBuilder* builder, Reflection::RClass* rtClass)
{
    IF_CALLBACK_NOTSETUP(ZombieMausoleumArcher) {
        SetupLiteralDelegate(&startShootCompletedDelegate, StartShootCompletedCallback);
        SetupLiteralDelegate(&waitShootCompletedDelegate, WaitingCompletedCallback);
        SetupLiteralDelegate(&shootCompletedDelegate, StartShootCompletedCallback);
        SetupLiteralDelegate(&endShootCompletedDelegate, EndShootingDoneCompletedCallback);
        ZombieMausoleumArcher_delegatesSetup = true;
        LOGI("SO TRUE");
    }
    RegisterEventCallback(builder, rtClass, "onShootingStart", startShootCompletedDelegate);
    RegisterEventCallback(builder, rtClass, "onWaitingContinue", waitShootCompletedDelegate);
    RegisterEventCallback(builder, rtClass, "onShootingDone", shootCompletedDelegate);
    RegisterEventCallback(builder, rtClass, "onInjuredDone", endShootCompletedDelegate);
    LOGI("Reg event complete");
}

void ZombieMausoleumArcher::buildStates()
{
    StateMachineTableBuilder* stateMachine = CallGetStateMachine(ZombieMausoleumArcher::StaticGetType());
    RegisterStateByOffsets(stateMachine,
        16,
        (uintptr_t)ZombieMausoleumArcher::WalkIntoPositionOnEnter,
        (uintptr_t)ZombieMausoleumArcher::WalkIntoPositionOnLoop,
        (uintptr_t)ZombieMausoleumArcher::WalkIntoPositionOnExit,
        "ZS_Archer_Prepare");
    RegisterStateByOffsets(stateMachine,
        17,
        (uintptr_t)ZombieMausoleumArcher::WaitShootingOnEnter,
        (uintptr_t)ZombieMausoleumArcher::WaitShootingOnLoop,
        (uintptr_t)ZombieMausoleumArcher::WaitShootingOnExit,
        "ZS_Archer_Wait");
    RegisterStateByOffsets(stateMachine,
        18,
        (uintptr_t)ZombieMausoleumArcher::ShootOnEnter,
        (uintptr_t)ZombieMausoleumArcher::ShootOnLoop,
        (uintptr_t)ZombieMausoleumArcher::ShootOnExit,
        "ZS_Archer_Shoot");
    RegisterStateByOffsets(stateMachine,
        19,
        (uintptr_t)ZombieMausoleumArcher::EndShootOnEnter,
        (uintptr_t)ZombieMausoleumArcher::EndShootOnLoop,
        (uintptr_t)ZombieMausoleumArcher::EndShootOnExit,
        "ZS_Archer_Injured");
    RegisterStateByOffsets(stateMachine,
        20,
        (uintptr_t)ZombieMausoleumArcher::StartShootOnEnter,
        (uintptr_t)ZombieMausoleumArcher::StartShootOnLoop,
        (uintptr_t)ZombieMausoleumArcher::StartShootOnExit,
        "ZS_Archer_StartShoot");
    LOGI("Reg state complete");
}
