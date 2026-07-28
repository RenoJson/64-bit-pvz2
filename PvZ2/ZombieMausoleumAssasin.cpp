#include "ZombieMausoleumAssasin.h"
#include "ZombieStateHelper.h"
#include "TimeMgr.h"
#include "DamageInfo.h"
#include "ZombieAnimRig_MausoleumAssasin.h"
#include "StateMachineBuilder.h"
#include "ZombieHelper.h"
#include "Board.h"
#include "AddZombieType.h"

void* ZombieMausoleumAssasin::vftable = nullptr;
Sexy::RtClass* ZombieMausoleumAssasin::s_rtClass = nullptr;

void* ZombieMausoleumAssasinProps::vftable = nullptr;
Sexy::RtClass* ZombieMausoleumAssasinProps::s_rtClass = nullptr;

typedef int (*playAnimWithoutCallback)(ZombieAnimRig*, const SexyString&, int, DelegateBase&);
typedef int (*playAnimWithCallback)(ZombieAnimRig*, const SexyString&, int, ZombieEvent& event);
typedef ZombieEvent* (*ConstructEvent)(ZombieEvent*, RtWeakPtr<Zombie>& owner, const SexyString& eventName);
typedef BoardEntity* (*getTarg)(ZombieMausoleumAssasin*);
typedef int64_t(*threatAlert)(ZombieMausoleumAssasin*);

DECLARE_DELEGATES_SETUP(ZombieMausoleumAssasin)

static Sexy::DelegateBase revealCompletedDelegate;

static Sexy::DelegateBase sprintStartCompletedDelegate;

static Sexy::DelegateBase sprintEndCompletedDelegate;

static Sexy::DelegateBase backstabCompletedDelegate;

bool MausoleumAssasinCanBeTargeted(ZombieMausoleumAssasin* zombie, char a2) {
    if (ZombieIsInState(zombie, 17)) {
        return false;
    }
    else {
        return CallFunc<bool, Zombie*, char>(0xC4D594, zombie, a2);
    }
}

int64_t AssasinThreatAlert(ZombieMausoleumAssasin* zombie) {
    if (zombie->m_becomeAssasin == true) {
        return 0;
    }
    else {
        return ((threatAlert)getActualOffset(0xC493B4))(zombie);
    }
}
bool AssasinIsBeingTossedByPlant(ZombieMausoleumAssasin* zombie, int a2) {
    if (zombie->m_entityState.m_id == 17) {
        return false;
    }
    else {
        return CallFunc<bool, ZombieMausoleumAssasin*, int>(0xC4D2EC, zombie, a2);
    }
}

void AssasinOnArmorDestroyed(ZombieMausoleumAssasin* zombie, int a2, SexyString* armorName)
{
    if (*armorName == "Scroll" && !ZombieIsDeadOrDying(zombie) && zombie->m_entityState.m_id != 3) {
        auto rig = reinterpret_cast<ZombieAnimRig_MausoleumAssasin*>(zombie->m_animRig.Get());
        rig->m_hasScroll = false;
        if (ZombieHasArmor(zombie, "Dao")) {
            rig->m_hasDagger = true;
        }
        ZombieEnterState(zombie, 16, 0);
    }
    if (*armorName == "Dao" && !ZombieIsDeadOrDying(zombie) && zombie->m_entityState.m_id != 3) {
        auto rig = reinterpret_cast<ZombieAnimRig_MausoleumAssasin*>(zombie->m_animRig.Get());
        rig->m_hasDagger = false;
    }
}

void AssasinWalkOnLoop(ZombieMausoleumAssasin* zombie)
{
    auto rig = reinterpret_cast<ZombieAnimRig_MausoleumAssasin*>(zombie->m_animRig.Get());
    getTarg getTarget = (getTarg)getActualOffset(0xC41910);
    BoardEntity* target = getTarget(zombie);
    if (target != nullptr) {
        if (rig->m_hasDagger == true) {
            ZombieEnterState(zombie, 18, 0);
        }
        else {
            ZombieEnterState(zombie, 2, 0);
        }
    }
}

void AssasinOnCreate(ZombieMausoleumAssasin* zombie) {
    auto props = reinterpret_cast<ZombieMausoleumAssasinProps*>(zombie->m_propertySheet.Get());
    auto rig = reinterpret_cast<ZombieAnimRig_MausoleumAssasin*>(zombie->m_animRig.Get());
    rig->m_hasCursed = props->CursedAtStart;
    SetAnimLayerVisible(rig, "zombie_eyes_curse", rig->m_hasCursed);
    zombie->m_becomeAssasin = false;
    Board* board = Board::GetBoard();
    Sexy::RtClass* surrSub = ZombieSurrenderSubsystem::StaticGetType();
    typedef GameSubsystem* (*getSubsystem)(Board*, Sexy::RtClass*);
    ((getSubsystem)getActualOffset(0xAAB864))(board, surrSub);
}

void MausoleumAssasinActionFrame(ZombieMausoleumAssasin* self, SexyString* currentAnim, SexyString* actionName, SexyString* param, float nextFrameTime)
{
    if (*actionName == "kill_plant" && !ZombieIsDeadOrDying(self))
    {
        if (ZombieHasArmor(self, "Dao")) {
            auto* props = reinterpret_cast<ZombieMausoleumAssasinProps*>(self->m_propertySheet.Get());
            getTarg getTarget = (getTarg)getActualOffset(0xC41910);
            BoardEntity* target = getTarget(self);
            if (target != nullptr) {
                DamageInfo dmg;
                dmg.m_attacker = self;
                dmg.m_damage = props->AssasinateDamage;

                void** vtable = *(void***)target;
                typedef void (*VirtualTakeDamageFunc)(BoardEntity*, DamageInfo*);
                VirtualTakeDamageFunc takeDmg = (VirtualTakeDamageFunc)vtable[35];

                takeDmg(target, &dmg);
            }
        }
    }
    if (*actionName == "ghost_intro")
    {
        auto* props = reinterpret_cast<ZombieMausoleumAssasinProps*>(self->m_propertySheet.Get());
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

void ZombieMausoleumAssasin::RevealOnEnter(ZombieMausoleumAssasin* zombie)
{
    RegisterEventAfterAnim(zombie, "pohuai", "onRevealCompleted");
}

void ZombieMausoleumAssasin::RevealOnLoop(ZombieMausoleumAssasin* zombie)
{
}

void ZombieMausoleumAssasin::RevealOnExit(ZombieMausoleumAssasin* zombie)
{
}

void ZombieMausoleumAssasin::SprintOnEnter(ZombieMausoleumAssasin* zombie)
{
    RegisterEventAfterAnim(zombie, "sprint_start", "onSprintContinued");
}

void ZombieMausoleumAssasin::SprintOnLoop(ZombieMausoleumAssasin* zombie)
{
}

void ZombieMausoleumAssasin::SprintOnExit(ZombieMausoleumAssasin* zombie)
{
}

void ZombieMausoleumAssasin::BackstabOnEnter(ZombieMausoleumAssasin* zombie)
{
    auto animRig = reinterpret_cast<ZombieAnimRig*>(zombie->m_animRig.Get());
    RtWeakPtr<Zombie> zombiePtr;
    zombiePtr.FromOther((RtWeakPtr<Zombie>*) & zombie->m_thisPtr);

    ZombieEvent zombieEvent;
    ((ConstructEvent)getActualOffset(0x6FDDDC))(&zombieEvent, zombiePtr, "onAttackEnd");

    playAnimWithCallback func = ((playAnimWithCallback)getActualOffset(0x8DCEDC));

    zombie->m_animHandle = func(animRig, "attack", 3, zombieEvent);
}

void ZombieMausoleumAssasin::BackstabOnLoop(ZombieMausoleumAssasin* zombie)
{
    auto animRig = reinterpret_cast<ZombieAnimRig*>(zombie->m_animRig.Get());
    if (IsAnimDone(animRig, zombie->m_animHandle))
    {
        ZombieEnterState(zombie, 1, 0);
    }
}

void ZombieMausoleumAssasin::BackstabOnExit(ZombieMausoleumAssasin* zombie)
{
}

void RevealCompletedCallback(Zombie* zombie) {
    ZombieMausoleumAssasin* doorZombie = static_cast<ZombieMausoleumAssasin*>(zombie);
    if (doorZombie && !ZombieIsDeadOrDying(zombie) && doorZombie->m_entityState.m_id != 3) {
        if (ZombieHasArmor(doorZombie, "Dao")) {
            ZombieEnterState(doorZombie, 17, 0);
        }
        else {
            ZombieEnterState(doorZombie, 1, 0);
        }
    }
}
void SprintStartCompletedCallback(Zombie* zombie) {
    ZombieMausoleumAssasin* doorZombie = static_cast<ZombieMausoleumAssasin*>(zombie);
    if (doorZombie && !ZombieIsDeadOrDying(zombie) && doorZombie->m_entityState.m_id != 3) {
        doorZombie->m_becomeAssasin = true;
        Sexy::SexyVector3 newPos = { 232.0f, zombie->m_position.y, 0 };
        ZombieUpdatePosition(doorZombie, &newPos);
        ZombieFlippedAnim(zombie, true);
        RegisterEventAfterAnim(zombie, "sprint_end", "onSprintCompleted");
       
    }
}
void SprintEndCompletedCallback(Zombie* zombie) {
    ZombieMausoleumAssasin* doorZombie = static_cast<ZombieMausoleumAssasin*>(zombie);
    if (doorZombie && !ZombieIsDeadOrDying(zombie) && doorZombie->m_entityState.m_id != 3) {
        ZombieEnterState(doorZombie, 1, 0);
    }
}
void BackstabCompletedCallback(Zombie* zombie) {}

void ZombieMausoleumAssasin::modInit() {
    LOGI("ZombieAssasin mod init");

    vftable = CreateChildVFTable(204 + 15, getActualOffset(0x241D430), 204);
    PatchVFTable(vftable, (void*)ZombieMausoleumAssasin::StaticGetType, 0);
    PatchVFTable(vftable, (void*)MausoleumAssasinCanBeTargeted, 21);
    PatchVFTable(vftable, (void*)AssasinThreatAlert, 75);
    PatchVFTable(vftable, (void*)AssasinIsBeingTossedByPlant, 97);
    PatchVFTable(vftable, (void*)AssasinOnArmorDestroyed, 115);
    PatchVFTable(vftable, (void*)AssasinWalkOnLoop, 124);
    PatchVFTable(vftable, (void*)MausoleumAssasinActionFrame, 170);

    PatchVFTable(vftable, (void*)ZombieMausoleumAssasin::RevealOnEnter, 204);
    PatchVFTable(vftable, (void*)ZombieMausoleumAssasin::RevealOnLoop, 205);
    PatchVFTable(vftable, (void*)ZombieMausoleumAssasin::RevealOnExit, 206);

    PatchVFTable(vftable, (void*)ZombieMausoleumAssasin::SprintOnEnter, 207);
    PatchVFTable(vftable, (void*)ZombieMausoleumAssasin::SprintOnLoop, 208);
    PatchVFTable(vftable, (void*)ZombieMausoleumAssasin::SprintOnExit, 209);

    PatchVFTable(vftable, (void*)ZombieMausoleumAssasin::BackstabOnEnter, 210);
    PatchVFTable(vftable, (void*)ZombieMausoleumAssasin::BackstabOnLoop, 211);
    PatchVFTable(vftable, (void*)ZombieMausoleumAssasin::BackstabOnExit, 212);

    ZombieMausoleumAssasin::StaticGetType();
    LOGI("ZombieAssasin finish init");
}

void ZombieMausoleumAssasinProps::modInit() {
    LOGI("ZombieAssasinProps init");

    vftable = CopyVFTable(getActualOffset(0x24328D0), 14);

    PatchVFTable(vftable, (void*)ZombieMausoleumAssasinProps::StaticGetType, 0);

    ZombieMausoleumAssasinProps::StaticGetType();

    LOGI("ZombieAssasinProps finish init");
}

void ZombieMausoleumAssasin::buildEventCallbacks(Reflection::CRefManualSymbolBuilder* builder, Reflection::RClass* rtClass)
{
    IF_CALLBACK_NOTSETUP(ZombieMausoleumAssasin) {
        SetupLiteralDelegate(&revealCompletedDelegate, RevealCompletedCallback);
        SetupLiteralDelegate(&sprintStartCompletedDelegate, SprintStartCompletedCallback);
        SetupLiteralDelegate(&sprintEndCompletedDelegate, SprintEndCompletedCallback);
        SetupLiteralDelegate(&backstabCompletedDelegate, BackstabCompletedCallback);
        ZombieMausoleumAssasin_delegatesSetup = true;
        LOGI("SO TRUE");
    }
    RegisterEventCallback(builder, rtClass, "onRevealCompleted", revealCompletedDelegate);
    RegisterEventCallback(builder, rtClass, "onSprintContinued", sprintStartCompletedDelegate);
    RegisterEventCallback(builder, rtClass, "onSprintCompleted", sprintEndCompletedDelegate);
    RegisterEventCallback(builder, rtClass, "onAttackEnd", backstabCompletedDelegate);
    LOGI("Reg event complete");
}

void ZombieMausoleumAssasin::buildStates()
{
    StateMachineTableBuilder* stateMachine = CallGetStateMachine(ZombieMausoleumAssasin::StaticGetType());
    RegisterStateByOffsets(stateMachine,
        16,
        (uintptr_t)ZombieMausoleumAssasin::RevealOnEnter,
        (uintptr_t)ZombieMausoleumAssasin::RevealOnLoop,
        (uintptr_t)ZombieMausoleumAssasin::RevealOnExit,
        "ZS_Assasin_Reveal");
    RegisterStateByOffsets(stateMachine,
        17,
        (uintptr_t)ZombieMausoleumAssasin::SprintOnEnter,
        (uintptr_t)ZombieMausoleumAssasin::SprintOnLoop,
        (uintptr_t)ZombieMausoleumAssasin::SprintOnExit,
        "ZS_Assasin_Sprint");
    RegisterStateByOffsets(stateMachine,
        18,
        (uintptr_t)ZombieMausoleumAssasin::BackstabOnEnter,
        (uintptr_t)ZombieMausoleumAssasin::BackstabOnLoop,
        (uintptr_t)ZombieMausoleumAssasin::BackstabOnExit,
        "ZS_Assasin_Backstab");
    LOGI("Reg state complete");
}