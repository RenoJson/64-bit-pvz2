#include "ZombieMausoleumDrunkard.h"
#include "ZombieStateHelper.h"
#include "TimeMgr.h"
#include "DamageInfo.h"
#include <cmath>
#include "ZombieAnimRig_MausoleumDrunkard.h"
#include "StateMachineBuilder.h"
#include "ZombieHelper.h"
#include "Board.h"
#include "Plant.h"

void* ZombieMausoleumDrunkard::vftable = nullptr;
Sexy::RtClass* ZombieMausoleumDrunkard::s_rtClass = nullptr;

void* ZombieMausoleumDrunkardProps::vftable = nullptr;
Sexy::RtClass* ZombieMausoleumDrunkardProps::s_rtClass = nullptr;

DECLARE_DELEGATES_SETUP(ZombieMausoleumDrunkard)

static Sexy::DelegateBase drunkCompletedDelegate;

static Sexy::DelegateBase pukeCompletedDelegate;

void DrunkardOnSpawn(ZombieMausoleumDrunkard* zombie)
{
    ZombieOnSpawn(zombie);
    SexyVector3 zombiePos = { 780.0f, zombie->m_position.y, 0.0f };
    ZombieUpdatePosition(zombie, &zombiePos);
}

void DrunkardWalkOnLoop(ZombieMausoleumDrunkard* self) {
	auto props = reinterpret_cast<ZombieMausoleumDrunkardProps*>(self->m_propertySheet.Get());
	if (props->DrunkInterval <= 0.0f || self->m_elapsedTimeInState <= props->DrunkInterval) {
		CallFunc<void, ZombieMausoleumDrunkard*>(0xC506B4, self);
	}
	else {
		ZombieEnterState(self, 16, 0);
	}
}
void DrunkardEatOnLoop(ZombieMausoleumDrunkard* self) {
    auto props = reinterpret_cast<ZombieMausoleumDrunkardProps*>(self->m_propertySheet.Get());
    if (props->DrunkInterval <= 0.0f || self->m_elapsedTimeInState <= props->DrunkInterval) {
        CallFunc<void, ZombieMausoleumDrunkard*>(0xC5082C, self);
    }
    else {
        ZombieEnterState(self, 16, 0);
    }
}

void DrunkardOnCreate(ZombieMausoleumDrunkard* zombie) {
    zombie->m_lastPathGridX = 0;
    zombie->m_lastPathGridY = 0;
}
void DrunkardActionFrame(ZombieMausoleumDrunkard* zombie, SexyString* currentAnim, SexyString* actionName, SexyString* param, float nextFrameTime)
{
    if (*actionName == "spray")
    {
        auto* props = reinterpret_cast<ZombieMausoleumDrunkardProps*>(zombie->m_propertySheet.Get());

        int zX = static_cast<int>((zombie->m_position.x - 200.0f) / 64.0f);
        int zY = static_cast<int>((zombie->m_position.y - 160.0f) / 76.0f);

        Rect SprayRect;
        SprayRect.mX = zX + props->PukeOffsetGridX;
        if (ZombieHasCondition(zombie, zombie_condition_hypnotized)) {
            SprayRect.mX = zX - props->PukeOffsetGridX;
        }
        SprayRect.mY = zY;
        SprayRect.mWidth = props->PukeWidth;
        SprayRect.mHeight = 1;

        std::vector<BoardEntity*> entityList;

        GetEntitiesInRectGrid(&entityList, 63, &SprayRect);

        for (BoardEntity* ptr : entityList) {
            if (ptr == nullptr) continue;

            else if (ptr->IsType(Zombie::StaticGetType()) && zombie->m_teamFlags == 1) {
                ZombieSetCondition((Zombie*)ptr, zombie_condition_stun, 0, props->ConditionLifetime, 0.0f);
            }
            else if (ptr->IsType(Zombie::StaticGetType()) && zombie->m_teamFlags == 2) {
                ZombieSetCondition((Zombie*)ptr, zombie_condition_potiontoughness2, 0, props->ConditionLifetime, 0.0f);
            }
        }
    }
}
SexyString GetDrunkardShockEffectName()
{
    return "POPANIM_EFFECTS_ZOMBIE_FAT_SHOCK";
}
SexyString GetDrunkardAshEffectName()
{
    return "POPANIM_EFFECTS_ZOMBIE_FAT_ASH";
}

void ZombieMausoleumDrunkard::DrunkOnEnter(ZombieMausoleumDrunkard* zombie)
{
	RegisterEventAfterAnim(zombie, "drink", "onDrinkingCompleted");
}

void ZombieMausoleumDrunkard::DrunkOnLoop(ZombieMausoleumDrunkard* zombie)
{

}

void ZombieMausoleumDrunkard::DrunkOnExit(ZombieMausoleumDrunkard* zombie)
{

}

void ZombieMausoleumDrunkard::ChangeLaneOnEnter(ZombieMausoleumDrunkard* zombie)
{
    int currentZRow = static_cast<int>((zombie->m_position.y - 160.0f) / 76.0f);
    zombie->m_lastPathGridX = currentZRow;

    int targetZRow = currentZRow;

    if (currentZRow <= 0)
    {
        targetZRow = currentZRow + 1;
    }
    else if (currentZRow >= 4)
    {
        targetZRow = currentZRow - 1;
    }
    else
    {
        int roll = rand() % 2;
        if (roll == 0) {
            targetZRow = currentZRow - 1;
        }
        else {
            targetZRow = currentZRow + 1;
        }
    }
    zombie->m_lastPathGridY = targetZRow;
    RegisterEventOnWalkLoop(zombie, "onWalkAnimationCycle");
}

void ZombieMausoleumDrunkard::ChangeLaneOnLoop(ZombieMausoleumDrunkard* zombie)
{
    auto props = reinterpret_cast<ZombieMausoleumDrunkardProps*>(zombie->m_propertySheet.Get());

    float pX = zombie->m_position.x;
    float pY = zombie->m_position.y;
    float pZ = zombie->m_position.z;

    float baseSpeed = props->Speed;
    float speedScale = zombie->m_conditionTracker.m_speedScale;
    float stepDist = baseSpeed * speedScale * 64.0f * TimeMgr::GetInstance()->m_unkTime;

    float startY = 220.0f + (zombie->m_lastPathGridX * 76.0f);
    float targetY = 220.0f + (zombie->m_lastPathGridY * 76.0f);

    bool reachedCenter = false;

    if (pY < targetY) {
        pY += stepDist;
        if (pY >= targetY) reachedCenter = true;
    }
    else if (pY > targetY) {
        pY -= stepDist;
        if (pY <= targetY) reachedCenter = true;
    }
    else {
        reachedCenter = true;
    }

    if (reachedCenter)
    {
        pY = targetY;
        pZ = 0.0f; 
        zombie->m_lastPathGridY = static_cast<int>((pY - 160.0f) / 76.0f);

        SexyVector3 newPos = { pX, pY, pZ };
        ZombieUpdatePosition(zombie, &newPos);

        ZombieEnterState(zombie, 1, 0); 
        return;
    }

    float totalDist = std::abs(targetY - startY);
    if (totalDist == 0.0f) totalDist = 76.0f;

    float currentDistWalked = std::abs(pY - startY);
    float progress = currentDistWalked / totalDist;

    float stumbleHeight = 18.0f;
    pZ = stumbleHeight * std::abs(std::sin(progress * 2.0f * 3.14159f));

    SexyVector3 newPos = { pX, pY, pZ };
    ZombieUpdatePosition(zombie, &newPos);
}

void ZombieMausoleumDrunkard::ChangeLaneOnExit(ZombieMausoleumDrunkard* zombie)
{

}
void DrunkCompletedCallback(Zombie* zombie) {
    ZombieMausoleumDrunkard* doorZombie = static_cast<ZombieMausoleumDrunkard*>(zombie);
    if (doorZombie && !ZombieIsDeadOrDying(zombie)) {
        RegisterEventAfterAnim(zombie, "spray", "onPukingCompleted");
    }
}
void PukingCompletedCallback(Zombie* zombie) {
    ZombieMausoleumDrunkard* doorZombie = static_cast<ZombieMausoleumDrunkard*>(zombie);
    if (doorZombie && !ZombieIsDeadOrDying(zombie)) {
        ZombieEnterState(doorZombie, 17, 0);
    }
}
void ZombieMausoleumDrunkard::modInit() {
    LOGI("ZombieDrunkard mod init");

    vftable = CreateChildVFTable(204 + 9, getActualOffset(0x241D430), 204);
    PatchVFTable(vftable, (void*)ZombieMausoleumDrunkard::StaticGetType, 0);
    PatchVFTable(vftable, (void*)DrunkardWalkOnLoop, 124);
    PatchVFTable(vftable, (void*)DrunkardEatOnLoop, 127);
    PatchVFTable(vftable, (void*)DrunkardOnCreate, 169);
    PatchVFTable(vftable, (void*)DrunkardActionFrame, 170);
    PatchVFTable(vftable, (void*)GetDrunkardShockEffectName, 189);
    PatchVFTable(vftable, (void*)GetDrunkardAshEffectName, 190);

    PatchVFTable(vftable, (void*)ZombieMausoleumDrunkard::DrunkOnEnter, 204);
    PatchVFTable(vftable, (void*)ZombieMausoleumDrunkard::DrunkOnLoop, 205);
    PatchVFTable(vftable, (void*)ZombieMausoleumDrunkard::DrunkOnExit, 206);

    PatchVFTable(vftable, (void*)ZombieMausoleumDrunkard::ChangeLaneOnEnter, 207);
    PatchVFTable(vftable, (void*)ZombieMausoleumDrunkard::ChangeLaneOnLoop, 208);
    PatchVFTable(vftable, (void*)ZombieMausoleumDrunkard::ChangeLaneOnExit, 209);

    ZombieMausoleumDrunkard::StaticGetType();
    LOGI("ZombieDrunkard finish init");
}

void ZombieMausoleumDrunkardProps::modInit() {
    LOGI("ZombieDrunkardProps init");

    vftable = CopyVFTable(getActualOffset(0x24328D0), 14);

    PatchVFTable(vftable, (void*)ZombieMausoleumDrunkardProps::StaticGetType, 0);

    ZombieMausoleumDrunkardProps::StaticGetType();

    LOGI("ZombieDrunkardProps finish init");
}

void ZombieMausoleumDrunkard::buildEventCallbacks(Reflection::CRefManualSymbolBuilder* builder, Reflection::RClass* rtClass)
{
    IF_CALLBACK_NOTSETUP(ZombieMausoleumDrunkard) {
        SetupLiteralDelegate(&drunkCompletedDelegate, DrunkCompletedCallback);
        SetupLiteralDelegate(&pukeCompletedDelegate, PukingCompletedCallback);
        ZombieMausoleumDrunkard_delegatesSetup = true;
        LOGI("SO TRUE");
    }
    RegisterEventCallback(builder, rtClass, "onDrinkingCompleted", drunkCompletedDelegate);
    RegisterEventCallback(builder, rtClass, "onPukingCompleted", pukeCompletedDelegate);
    LOGI("Reg event complete");
}

void ZombieMausoleumDrunkard::buildStates()
{
    StateMachineTableBuilder* stateMachine = CallGetStateMachine(ZombieMausoleumDrunkard::StaticGetType());
    RegisterStateByOffsets(stateMachine,
        16,
        (uintptr_t)ZombieMausoleumDrunkard::DrunkOnEnter,
        (uintptr_t)ZombieMausoleumDrunkard::DrunkOnLoop,
        (uintptr_t)ZombieMausoleumDrunkard::DrunkOnExit,
        "ZS_Drunkard_Drinking");
    RegisterStateByOffsets(stateMachine,
        17,
        (uintptr_t)ZombieMausoleumDrunkard::ChangeLaneOnEnter,
        (uintptr_t)ZombieMausoleumDrunkard::ChangeLaneOnLoop,
        (uintptr_t)ZombieMausoleumDrunkard::ChangeLaneOnExit,
        "ZS_Drunkard_ChangeLane");
    LOGI("Reg state complete");
}

