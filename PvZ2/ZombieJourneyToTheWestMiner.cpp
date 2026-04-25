#include "ZombieDinoBasic.h"
#include "ZombieStateHelper.h"
#include "ZombieDinoBullyVeteranProps.h"
#include "ZombieState.h"
#include "StateMachineBuilder.h"
#include "ZombieAnimRig_Basic.h"
#include "Plant.h"
#include "DamageInfo.h"
#include "TimeMgr.h"
#include "Board.h"
#include "ZombieAnimRig_Basic.h"

void* ZombieModernMiner::vftable = nullptr;
Sexy::RtClass* ZombieModernMiner::s_rtClass = nullptr;;
typedef void (*zombieEnterState)(ZombieModernMiner*, int, int);
typedef Zombie* (*zombieAllowMovement)(Zombie*, int);
typedef Zombie* (*zombieFlippedAnim)(Zombie*, int);
typedef void (*LoopWalk)(ZombieModernMiner*);
typedef bool (*isDeadOrDying)(ZombieModernMiner*);
typedef void (*LoopEat)(ZombieModernMiner*);
typedef uintptr_t (*RenderShadow)(ZombieModernMiner*, Sexy::Graphics*);
typedef BoardEntity* (*getTarg)(ZombieModernMiner*);
typedef void* (*funcC43B90)(ZombieModernMiner*, DamageInfo*);
typedef void (*ActionFrame)(ZombieModernMiner*, int64_t, SexyString*, int64_t, SexyString*);
typedef void (*setSpeedScale)(ZombieModernMiner*, float);
typedef int64_t(*threatAlert)(ZombieModernMiner*);
int LastVetBullyState = 18;
DECLARE_DELEGATES_SETUP(ZombieModernMiner)

static Sexy::DelegateBase diveInCompletedDelegate;

static Sexy::DelegateBase diggingCompletedDelegate;

static Sexy::DelegateBase diveOutCompletedDelegate;

static Sexy::DelegateBase lostStickCompletedDelegate;

static Sexy::DelegateBase lostStickDiggingCompletedDelegate;
uintptr_t TudigongRenderShadow(ZombieModernMiner* zombie, Sexy::Graphics* graphics)
{
    if (zombie->m_isDigged == true)
    {
        return 0;
    }
    return ((RenderShadow)getActualOffset(0xC4F814))(zombie, graphics);
}
bool TudigongShouldIgnoreCollision(ZombieModernMiner* zombie, Projectile* proj)
{
    int myTeam = zombie->m_teamFlags;     
    int otherTeam = proj->m_teamFlags; 
    if (zombie->m_isDigged == true) {
        return true;
    }
    else {
        if ((otherTeam & 2) != 0 && (myTeam & 1) != 0)
        {
            return false; 
        }
        else
        {
            bool isOtherNotPlant = ((otherTeam & 1) == 0);
            bool isMeNotZombie = ((myTeam & 2) == 0);

            return isOtherNotPlant || isMeNotZombie;
        }
    }
}
void TudigongOnSpawn(ZombieModernMiner* zombie)
{
	auto rig = reinterpret_cast<ZombieAnimRig_ModernMiner*>(zombie->m_animRig.Get());
	auto props = reinterpret_cast<ZombieModernMinerProps*>(zombie->m_propertySheet.Get());
    for (auto& weakArmor : zombie->m_armor)
    {
        Armor* armor = weakArmor.Get();
        if (armor != nullptr)
        {
            auto* armorProps = reinterpret_cast<ArmorPropertySheet*>(armor->m_propertySheetPtr.Get());

            if (armorProps != nullptr && armorProps->ArmorType == props->ArmorTypeToUseForDigging)
            {
                rig->m_hasStick = true;
                break;
            }
        }
    }
    rig->m_digging = false;
    zombie->m_isDigged = false;
    zombie->m_diggedDone = false;
	typedef void (*zombieFun49)(ZombieModernMiner*);
	((zombieFun49)getActualOffset(0xC3D1F0))(zombie);
}
int64_t TudigongThreatAlert(ZombieModernMiner* zombie) {
    auto rig = reinterpret_cast<ZombieAnimRig_ModernMiner*>(zombie->m_animRig.Get());
    if (zombie->m_isDigged == true) {
        return 0;
    }
    else if (zombie->m_diggedDone == true) {
        return 0;
    }
    else {
        return ((threatAlert)getActualOffset(0xC493B4))(zombie);
    }
}
void TudigongOnArmorDestroyed(ZombieModernMiner* zombie, int a2, SexyString* armorName)
{
    auto props = reinterpret_cast<ZombieModernMinerProps*>(zombie->m_propertySheet.Get());
    isDeadOrDying isDeadFunc = (isDeadOrDying)getActualOffset(0xC3E204);
    if (*armorName == props->ArmorTypeToUseForDigging && !isDeadFunc(zombie)) {
        if (zombie->m_isDigged == false) {
            ((zombieEnterState)getActualOffset(0xC3D428))(zombie, 22, 0);
        }
        else if (zombie->m_isDigged == true) {
            ((zombieEnterState)getActualOffset(0xC3D428))(zombie, 23, 0);
        }
    }
}
void TudigongWalkOnLoop(ZombieModernMiner* zombie)
{
    isDeadOrDying isDeadFunc = (isDeadOrDying)getActualOffset(0xC3E204);
    if (isDeadFunc(zombie)) {
        ((setSpeedScale)getActualOffset(0xC484C0))(zombie, 1);
        ((LoopWalk)getActualOffset(0xC506B4))(zombie);
        return;
    }
    auto props = reinterpret_cast<ZombieModernMinerProps*>(zombie->m_propertySheet.Get());
    bool hasArmorToDig = false;
    for (auto& weakArmor : zombie->m_armor)
    {
        Armor* armor = weakArmor.Get();
        if (armor != nullptr && !armor->m_destroyed)
        {
            auto* armorProps = reinterpret_cast<ArmorPropertySheet*>(armor->m_propertySheetPtr.Get());

            if (armorProps != nullptr && armorProps->ArmorType == props->ArmorTypeToUseForDigging)
            {
                hasArmorToDig = true;
                break;
            }
        }
    }
    if (hasArmorToDig)
    {
        if (zombie->m_position.x <= 700.0f)
        {
            if (!zombie->m_isDigged && !zombie->m_diggedDone && ((zombie->m_teamFlags) & 2) != 0)
            {
                zombie->m_isDigged = true;
                ((zombieEnterState)getActualOffset(0xC3D428))(zombie, 19, 0);
                return; 
            }
        }
    }
    auto rig = reinterpret_cast<ZombieAnimRig_ModernMiner*>(zombie->m_animRig.Get());
    getTarg getTarget = (getTarg)getActualOffset(0xC41910);
    BoardEntity* target = getTarget(zombie);
    if (target != nullptr) {
        if (props->Smashable == true && rig->m_hasStick == true) {
            ((zombieEnterState)getActualOffset(0xC3D428))(zombie, 18, 0);
        }
        else {
            ((zombieEnterState)getActualOffset(0xC3D428))(zombie, 2, 0);
        }
    }
}
GameSubsystem* TudigongSurrender(ZombieModernMiner* zombie) {
    Board* board = Board::GetBoard();
    Sexy::RtClass* surrSub = ZombieSurrenderSubsystem::StaticGetType();
    typedef GameSubsystem* (*getSubsystem)(Board*, Sexy::RtClass*);
    return ((getSubsystem)getActualOffset(0xAAB864))(board, surrSub);
}

void TudigongActionFrame(ZombieModernMiner* zombie, int64_t unk1, SexyString* actionName, int64_t unk2, SexyString* currentAnim)
{
    auto props = reinterpret_cast<ZombieModernMinerProps*>(zombie->m_propertySheet.Get());
    if (props->Smashable == true)
    {
        ((ActionFrame)getActualOffset(0xB0C858))(zombie, unk1, actionName, unk2, currentAnim);
    }
}
SexyString GetTudigongShockEffectName()
{
    return "POPANIM_EFFECTS_ZOMBIE_FAT_SHOCK";
}
SexyString GetTudigongAshEffectName()
{
    return "POPANIM_EFFECTS_ZOMBIE_FAT_ASH";
}
void ZombieModernMiner::DiveInOnEnter(ZombieModernMiner* zombie)
{
    RegisterEventAfterAnim(zombie, "special_in", "onDiveInCompleted");
}

void ZombieModernMiner::DiveInOnLoop(ZombieModernMiner* zombie)
{

}

void ZombieModernMiner::DiveInOnExit(ZombieModernMiner* zombie)
{
    auto props = reinterpret_cast<ZombieModernMinerProps*>(zombie->m_propertySheet.Get());
    ((setSpeedScale)getActualOffset(0xC484C0))(zombie, props->DiggingSpeed);
}

void ZombieModernMiner::DiggingOnEnter(ZombieModernMiner* zombie)
{
    auto rig = reinterpret_cast<ZombieAnimRig_ModernMiner*>(zombie->m_animRig.Get());
    rig->m_digging = true;
    ((zombieAllowMovement)getActualOffset(0xC51F94))(zombie, 1);
    return RegisterEventOnWalkLoop(zombie, "onDiggingContinued");
}

void ZombieModernMiner::DiggingOnLoop(ZombieModernMiner* zombie)
{
    if (zombie->m_position.x <= 232.0f)
    {
       zombie->m_diggedDone = true; 
       ((setSpeedScale)getActualOffset(0xC484C0))(zombie, 1);
       ((zombieFlippedAnim)getActualOffset(0xC41290))(zombie, 1);
       ((zombieEnterState)getActualOffset(0xC3D428))(zombie, 21, 0);
    }
}

void ZombieModernMiner::DiggingOnExit(ZombieModernMiner* zombie)
{

}

void ZombieModernMiner::DiveOutOnEnter(ZombieModernMiner* zombie)
{
    auto rig = reinterpret_cast<ZombieAnimRig_ModernMiner*>(zombie->m_animRig.Get());
    rig->m_digging = false;
    RegisterEventAfterAnim(zombie, "special_out", "onDiveOutCompleted");
}

void ZombieModernMiner::DiveOutOnLoop(ZombieModernMiner* zombie)
{
}

void ZombieModernMiner::DiveOutOnExit(ZombieModernMiner* zombie)
{
}

void ZombieModernMiner::LostStickOnEnter(ZombieModernMiner* zombie)
{
    RegisterEventAfterAnim(zombie, "drop_tool", "onLostStickCompleted");
}

void ZombieModernMiner::LostStickOnLoop(ZombieModernMiner* zombie)
{
}

void ZombieModernMiner::LostStickOnExit(ZombieModernMiner* zombie)
{
}

void ZombieModernMiner::LostStickDiggingOnEnter(ZombieModernMiner* zombie)
{
    auto rig = reinterpret_cast<ZombieAnimRig_ModernMiner*>(zombie->m_animRig.Get());
    rig->m_digging = false;
    ((setSpeedScale)getActualOffset(0xC484C0))(zombie, 1);
    RegisterEventAfterAnim(zombie, "specoal_out_2", "onLostStickDiggingCompleted");
}

void ZombieModernMiner::LostStickDiggingOnLoop(ZombieModernMiner* zombie)
{
}

void ZombieModernMiner::LostStickDiggingOnExit(ZombieModernMiner* zombie)
{
   
}

void DiveInCompletedCallback(Zombie* zombie) {
    ZombieModernMiner* diggerZombie = static_cast<ZombieModernMiner*>(zombie);
    if (diggerZombie) {
        ((zombieEnterState)getActualOffset(0xC3D428))(diggerZombie, 20, 0);
    }
}
void DiggingCompletedCallback(Zombie* zombie) {
  
}
void DiveOutCompletedCallback(Zombie* zombie) {
    ZombieModernMiner* diggerZombie = static_cast<ZombieModernMiner*>(zombie);
    if (diggerZombie) {
        auto rig = reinterpret_cast<ZombieAnimRig_ModernMiner*>(zombie->m_animRig.Get());
        diggerZombie->m_isDigged = false;
        diggerZombie->m_diggedDone = true;
        ((zombieEnterState)getActualOffset(0xC3D428))(diggerZombie, 1, 0);
    }
}
void LostStickCompletedCallback(Zombie* zombie) {
    ZombieModernMiner* diggerZombie = static_cast<ZombieModernMiner*>(zombie);
    if (diggerZombie) {
        auto rig = reinterpret_cast<ZombieAnimRig_ModernMiner*>(zombie->m_animRig.Get());
        rig->m_hasStick = false;
        ((zombieEnterState)getActualOffset(0xC3D428))(diggerZombie, 1, 0);
    }
}
void LostStickDiggingCompletedCallback(Zombie* zombie) {
    ZombieModernMiner* diggerZombie = static_cast<ZombieModernMiner*>(zombie);
    if (diggerZombie) {
        auto rig = reinterpret_cast<ZombieAnimRig_ModernMiner*>(zombie->m_animRig.Get());
        rig->m_hasStick = false;
        diggerZombie->m_isDigged = false;
        diggerZombie->m_diggedDone = false;
        ((zombieEnterState)getActualOffset(0xC3D428))(diggerZombie, 1, 0);
    }
}
void ZombieModernMiner::modInit() {
    LOGI("ZombieMiner mod init");

    vftable = CreateChildVFTable(215 + 18, getActualOffset(0x23E4CC8), 215);
    PatchVFTable(vftable, (void*)ZombieModernMiner::StaticGetType, 0);

    PatchVFTable(vftable, (void*)TudigongRenderShadow, 28);
    PatchVFTable(vftable, (void*)TudigongShouldIgnoreCollision, 43);
    PatchVFTable(vftable, (void*)TudigongOnSpawn, 49);
    PatchVFTable(vftable, (void*)TudigongThreatAlert, 75);
    PatchVFTable(vftable, (void*)TudigongOnArmorDestroyed, 115);
    PatchVFTable(vftable, (void*)TudigongWalkOnLoop, 124);
    PatchVFTable(vftable, (void*)TudigongSurrender, 169);
    PatchVFTable(vftable, (void*)TudigongActionFrame, 170);
    PatchVFTable(vftable, (void*)GetTudigongShockEffectName, 189);
    PatchVFTable(vftable, (void*)GetTudigongAshEffectName, 190);

    PatchVFTable(vftable, (void*)ZombieModernMiner::DiveInOnEnter, 215);
    PatchVFTable(vftable, (void*)ZombieModernMiner::DiveInOnLoop, 216);
    PatchVFTable(vftable, (void*)ZombieModernMiner::DiveInOnExit, 217);

    PatchVFTable(vftable, (void*)ZombieModernMiner::DiggingOnEnter, 218);
    PatchVFTable(vftable, (void*)ZombieModernMiner::DiggingOnLoop, 219);
    PatchVFTable(vftable, (void*)ZombieModernMiner::DiggingOnExit, 220);

    PatchVFTable(vftable, (void*)ZombieModernMiner::DiveOutOnEnter, 221);
    PatchVFTable(vftable, (void*)ZombieModernMiner::DiveOutOnLoop, 222);
    PatchVFTable(vftable, (void*)ZombieModernMiner::DiveOutOnExit, 223);

    PatchVFTable(vftable, (void*)ZombieModernMiner::LostStickOnEnter, 224);
    PatchVFTable(vftable, (void*)ZombieModernMiner::LostStickOnLoop, 225);
    PatchVFTable(vftable, (void*)ZombieModernMiner::LostStickOnExit, 226);

    PatchVFTable(vftable, (void*)ZombieModernMiner::LostStickDiggingOnEnter, 227);
    PatchVFTable(vftable, (void*)ZombieModernMiner::LostStickDiggingOnLoop, 228);
    PatchVFTable(vftable, (void*)ZombieModernMiner::LostStickDiggingOnExit, 229);

    ZombieModernMiner::StaticGetType();
    LOGI("ZombieMiner finish init");
}

void ZombieModernMiner::buildEventCallbacks(Reflection::CRefManualSymbolBuilder* builder, Reflection::RClass* rtClass)
{
    IF_CALLBACK_NOTSETUP(ZombieModernMiner) {
        SetupLiteralDelegate(&diveInCompletedDelegate, DiveInCompletedCallback);
        SetupLiteralDelegate(&diggingCompletedDelegate, DiggingCompletedCallback);
        SetupLiteralDelegate(&diveOutCompletedDelegate, DiveOutCompletedCallback);
        SetupLiteralDelegate(&lostStickCompletedDelegate, LostStickCompletedCallback);
        SetupLiteralDelegate(&lostStickDiggingCompletedDelegate, LostStickDiggingCompletedCallback);
        ZombieModernMiner_delegatesSetup = true;
        LOGI("SO TRUE");
    }
    RegisterEventCallback(builder, rtClass, "onDiveInCompleted", diveInCompletedDelegate);
    RegisterEventCallback(builder, rtClass, "onDiggingContinued", diggingCompletedDelegate);
    RegisterEventCallback(builder, rtClass, "onDiveOutCompleted", diveOutCompletedDelegate);
    RegisterEventCallback(builder, rtClass, "onLostStickCompleted", lostStickCompletedDelegate);
    RegisterEventCallback(builder, rtClass, "onLostStickDiggingCompleted", lostStickDiggingCompletedDelegate);
    LOGI("Reg event complete");
}


void ZombieModernMiner::buildStates()
{
    StateMachineTableBuilder* stateMachine = CallGetStateMachine(ZombieModernMiner::StaticGetType());
    RegisterStateByOffsets(stateMachine,
        19,
        (uintptr_t)ZombieModernMiner::DiveInOnEnter,
        (uintptr_t)ZombieModernMiner::DiveInOnLoop,
        (uintptr_t)ZombieModernMiner::DiveInOnExit,
        "ZS_Miner_DiveIn");
    RegisterStateByOffsets(stateMachine,
        20,
        (uintptr_t)ZombieModernMiner::DiggingOnEnter,
        (uintptr_t)ZombieModernMiner::DiggingOnLoop,
        (uintptr_t)ZombieModernMiner::DiggingOnExit,
        "ZS_Miner_Digging");
    RegisterStateByOffsets(stateMachine,
        21,
        (uintptr_t)ZombieModernMiner::DiveOutOnEnter,
        (uintptr_t)ZombieModernMiner::DiveOutOnLoop,
        (uintptr_t)ZombieModernMiner::DiveOutOnExit,
        "ZS_Miner_DiveOut");
    RegisterStateByOffsets(stateMachine,
        22,
        (uintptr_t)ZombieModernMiner::LostStickOnEnter,
        (uintptr_t)ZombieModernMiner::LostStickOnLoop,
        (uintptr_t)ZombieModernMiner::LostStickOnExit,
        "ZS_Miner_LostStick");
    RegisterStateByOffsets(stateMachine,
        23,
        (uintptr_t)ZombieModernMiner::LostStickDiggingOnEnter,
        (uintptr_t)ZombieModernMiner::LostStickDiggingOnLoop,
        (uintptr_t)ZombieModernMiner::LostStickDiggingOnExit,
        "ZS_Miner_LostStickDigging");
    LOGI("Reg state complete");
}
