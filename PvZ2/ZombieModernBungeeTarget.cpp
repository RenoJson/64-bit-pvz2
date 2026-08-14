#include "ZombieModernBungeeTarget.h"
#include "ZombieModernBungee.h"
#include "AddZombieType.h"
#include "ZombieState.h"
#include "StateMachineBuilder.h"
#include "ZombieHelper.h"
#include "ZombieAnimRig_Basic.h"
#include "ZombieStateHelper.h"

void* ZombieModernBungeeTarget::vftable = nullptr;
Sexy::RtClass* ZombieModernBungeeTarget::s_rtClass = nullptr;

DECLARE_DELEGATES_SETUP(ZombieModernBungeeTarget)

static Sexy::DelegateBase fallingCompletedDelegate;

static Sexy::DelegateBase markingCompletedDelegate;

int64_t BungeeTargetThreatAlert() {
    return 0;
}

bool BungeeTargetCanBeTargetedByPlant() {
    return false;
}

void BungeeTargetOnInitialize(ZombieModernBungeeTarget* zombie) {
    zombie->m_targeted = false;
    ZombieIsFlying(zombie, true);
}

void ZombieModernBungeeTarget::MarkOnEnter(ZombieModernBungeeTarget* zombie)
{
	RegisterEventAfterAnim(zombie, "bz_idle", "onFallingEnd");
}

void ZombieModernBungeeTarget::MarkOnLoop(ZombieModernBungeeTarget* zombie)
{

}

void ZombieModernBungeeTarget::MarkOnExit(ZombieModernBungeeTarget* zombie)
{

}

void onFallingEndCallback(Zombie* zombie) {
    auto target = static_cast<ZombieModernBungeeTarget*>(zombie);
    if (target && !ZombieIsDeadOrDying(zombie) && target->m_entityState.m_id != 3) {
        int randomVariant = (rand() % 3) + 1;
        SexyString soundEvent = "Zombie_Bungee_Bullet_Impact" + std::to_string(randomVariant);
        ZombiePlaySoundEvent(zombie, soundEvent, 0.0f);
        RegisterEventAfterAnim(zombie, "bz_hit", "onMarkingEnd");
    }
}

void onMarkedCallback(Zombie* zombie) {
    auto target = static_cast<ZombieModernBungeeTarget*>(zombie);
    if (target && !ZombieIsDeadOrDying(zombie) && target->m_entityState.m_id != 3) {
        target->m_targeted = true;
    }
}

void ZombieModernBungeeTarget::ModInit() {
    LOGI("ZombieBungeeTarget mod init");

    vftable = CreateChildVFTable(204 + 15, getActualOffset(0x241D430), 204);
    PatchVFTable(vftable, (void*)ZombieModernBungeeTarget::StaticGetType, 0);

    PatchVFTable(vftable, (void*)BungeeTargetThreatAlert, 75);
    PatchVFTable(vftable, (void*)BungeeTargetCanBeTargetedByPlant, 93);
    PatchVFTable(vftable, (void*)BungeeTargetOnInitialize, 169);

    PatchVFTable(vftable, (void*)ZombieModernBungeeTarget::MarkOnEnter, 204);
    PatchVFTable(vftable, (void*)ZombieModernBungeeTarget::MarkOnLoop, 205);
    PatchVFTable(vftable, (void*)ZombieModernBungeeTarget::MarkOnExit, 206);

    ZombieModernBungeeTarget::StaticGetType();
    LOGI("ZombieBungeeTarget finish init");
}

void ZombieModernBungeeTarget::buildEventCallbacks(Reflection::CRefManualSymbolBuilder* builder, Reflection::RClass* rtClass)
{
    IF_CALLBACK_NOTSETUP(ZombieModernBungeeTarget) {
        SetupLiteralDelegate(&fallingCompletedDelegate, onFallingEndCallback);
        SetupLiteralDelegate(&markingCompletedDelegate, onMarkedCallback);
        ZombieModernBungeeTarget_delegatesSetup = true;
        LOGI("SO TRUE");
    }
    RegisterEventCallback(builder, rtClass, "onFallingEnd", fallingCompletedDelegate);
    RegisterEventCallback(builder, rtClass, "onMarkingEnd", markingCompletedDelegate);
    LOGI("Reg event complete");
}


void ZombieModernBungeeTarget::buildStates()
{
    StateMachineTableBuilder* stateMachine = CallGetStateMachine(ZombieModernBungeeTarget::StaticGetType());
    RegisterStateByOffsets(stateMachine,
        16,
        (uintptr_t)ZombieModernBungeeTarget::MarkOnEnter,
        (uintptr_t)ZombieModernBungeeTarget::MarkOnLoop,
        (uintptr_t)ZombieModernBungeeTarget::MarkOnExit,
        "ZS_BungeeTarget_Marking");
    LOGI("Reg state complete");
}

