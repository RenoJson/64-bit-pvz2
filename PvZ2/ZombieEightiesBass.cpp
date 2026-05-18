#include "ZombieEightiesBass.h"
#include "ZombieStateHelper.h"
#include "ZombieEightiesBassProps.h"
#include "ZombieState.h"
#include "StateMachineBuilder.h"
#include "ZombieAnimRig_EightiesBass.h"
#include "Plant.h"
#include "GridItem.h"
#include "AddProjectileType.h"
#include "DamageInfo.h"

void* ZombieEightiesBass::vftable = nullptr;
Sexy::RtClass* ZombieEightiesBass::s_rtClass = nullptr;;
typedef void (*zombieEnterState)(ZombieEightiesBass*, int, int);
typedef int (*playAnimWithCallback)(ZombieAnimRig*, const SexyString&, int, ZombieEvent& event);
typedef ZombieEvent* (*ConstructEvent)(ZombieEvent*, RtWeakPtr<Zombie>& owner, const SexyString& eventName);
typedef Zombie* (*zombieFlippedAnim)(Zombie*, int);
typedef bool (*isAnimDone)(ZombieAnimRig*, int);
typedef void (*GetEntitiesInRectFunc)(std::vector<BoardEntity*>*, int, Rect*);
DECLARE_DELEGATES_SETUP(ZombieEightiesBass)

static Sexy::DelegateBase riftingCompletedDelegate;

static Sexy::DelegateBase breakingGuitarCompletedDelegate;

static Sexy::DelegateBase debutingCompletedDelegate;

bool BassShouldIgnoreCollision(ZombieEightiesBass* zombie, Projectile* proj)
{
    int myTeam = zombie->m_teamFlags;
    int otherTeam = proj->m_teamFlags;
    if (zombie->m_isInGrandDebut == true) {
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
Zombie* BassActivateJam(ZombieEightiesBass* zombie) {
    zombie->m_isJamming = true;
    return zombie;
}
Zombie* BassDeactivateJam(ZombieEightiesBass* zombie) {
    zombie->m_isJamming = false;
    return zombie;
}
SexyString BassGetJamStyle(ZombieEightiesBass* zombie) {
    auto* props = reinterpret_cast<ZombieEightiesBassProps*>(zombie->m_propertySheet.Get());
    return props->JamStyle;
}

void BassActionFrame(ZombieEightiesBass* zombie, int64_t unk1, SexyString* actionName, int64_t unk2, SexyString* currentAnim)
{
    if (*actionName == "kill_plant")
    {
        typedef PlantGroup* (*getTarg)(ZombieEightiesBass*);
        getTarg getTarget = (getTarg)getActualOffset(0xC41910);
        PlantGroup* target = getTarget(zombie);
        if (target != nullptr && target->IsType(PlantGroup::StaticGetType())) {

            DamageInfo dmg;
            dmg.m_attacker = zombie;

            void** vtable = *(void***)target;
            typedef void (*VirtualTakeDamageFunc)(PlantGroup*, DamageInfo*);
            VirtualTakeDamageFunc takeDmg = (VirtualTakeDamageFunc)vtable[36];
            takeDmg(target, &dmg);
        }
        else {
            return;
        }
    }
}

void ZombieEightiesBass::GuitarAttackOnEnter(ZombieEightiesBass* zombie)
{
    RegisterEventAfterAnim(zombie, "attack", "onWaitingToRiftEnd");
}

void ZombieEightiesBass::GuitarAttackOnLoop(ZombieEightiesBass* zombie)
{
    int gX = static_cast<int>((zombie->m_position.x - 200.0f) / 64.0f);
    int gY = static_cast<int>((zombie->m_position.y - 160.0f) / 76.0f);

    if (gY > 4) gY = 4;
    if (gY < 0) gY = 0;

    Rect scanRect;
    scanRect.mX = gX - 1;
    scanRect.mY = gY;
    scanRect.mWidth = 2;
    scanRect.mHeight = 1;

    std::vector<BoardEntity*> entityList;
    GetEntitiesInRectFunc getEntitiesRect = (GetEntitiesInRectFunc)getActualOffset(0x86F180);
    getEntitiesRect(&entityList, 63, &scanRect);
    GridItemSpeaker* targetSpeaker = nullptr;
    for (BoardEntity* ptr : entityList) {
        if (ptr == nullptr) continue;

        if (ptr->IsType(GridItemSpeaker::StaticGetType())) {
            targetSpeaker = reinterpret_cast<GridItemSpeaker*>(ptr);
            break;
        }
    }

    if (targetSpeaker != nullptr) {
        auto* props = reinterpret_cast<ZombieEightiesBassProps*>(zombie->m_propertySheet.Get());
        Projectile* shockwave = AddProjectile(&props->ShockWaveProjectile,
            zombie,
            targetSpeaker->m_position.x - props->ShockWaveSpawnOffset.x,
            targetSpeaker->m_position.y - props->ShockWaveSpawnOffset.y,
            0);
		shockwave->m_teamFlags = zombie->m_teamFlags;
    }
    else {
        ((zombieEnterState)getActualOffset(0xC3D428))(zombie, 18, 0);
    }
}

void ZombieEightiesBass::GuitarAttackOnExit(ZombieEightiesBass* zombie)
{
    ((zombieEnterState)getActualOffset(0xC3D428))(zombie, 17, 0);
}

void ZombieEightiesBass::GuitarIdleOnEnter(ZombieEightiesBass* zombie)
{
    auto animRig = reinterpret_cast<ZombieAnimRig*>(zombie->m_animRig.Get());
    RtWeakPtr<Zombie> zombiePtr;
    zombiePtr.FromOther((RtWeakPtr<Zombie>*) & zombie->m_thisPtr);

    ZombieEvent zombieEvent;
    ((ConstructEvent)getActualOffset(0x6FDDDC))(&zombieEvent, zombiePtr, "onWaitingToRiftEnd");

    playAnimWithCallback func = ((playAnimWithCallback)getActualOffset(0x8DCEDC));

    zombie->m_animHandle = func(animRig, "attack", 3, zombieEvent);
}

void ZombieEightiesBass::GuitarIdleOnLoop(ZombieEightiesBass* zombie)
{
    auto animRig = reinterpret_cast<ZombieAnimRig*>(zombie->m_animRig.Get());
    bool animDone = ((isAnimDone)getActualOffset(0x9DCBE8))(animRig, zombie->m_animHandle);
    int gX = static_cast<int>((zombie->m_position.x - 200.0f) / 64.0f);
    int gY = static_cast<int>((zombie->m_position.y - 160.0f) / 76.0f);

    if (gY > 4) gY = 4;
    if (gY < 0) gY = 0;

    Rect scanRect;
    scanRect.mX = gX - 1;
    scanRect.mY = gY;
    scanRect.mWidth = 2;
    scanRect.mHeight = 1;

    std::vector<BoardEntity*> entityList;
    GetEntitiesInRectFunc getEntitiesRect = (GetEntitiesInRectFunc)getActualOffset(0x86F180);
    getEntitiesRect(&entityList, 63, &scanRect);
    GridItemSpeaker* targetSpeaker = nullptr;
    for (BoardEntity* ptr : entityList) {
        if (ptr == nullptr) continue;

        if (ptr->IsType(GridItemSpeaker::StaticGetType())) {
            targetSpeaker = reinterpret_cast<GridItemSpeaker*>(ptr);
            break;
        }
    }

    if (targetSpeaker != nullptr) {
        auto* props = reinterpret_cast<ZombieEightiesBassProps*>(zombie->m_propertySheet.Get());
        if (zombie->m_isJamming && zombie->m_elapsedTimeInState >= props->ShockWaveSpawnIntervalWhileJamming) {
            ((zombieEnterState)getActualOffset(0xC3D428))(zombie, 16, 0);
		}
        else if (!zombie->m_isJamming && zombie->m_elapsedTimeInState >= props->ShockWaveSpawnInterval) {
            ((zombieEnterState)getActualOffset(0xC3D428))(zombie, 16, 0);
		}
        else {
            if (animDone)
            {
                RtWeakPtr<Zombie> zombiePtr;
                zombiePtr.FromOther((RtWeakPtr<Zombie>*) & zombie->m_thisPtr);

                ZombieEvent zombieEvent;
                ((ConstructEvent)getActualOffset(0x6FDDDC))(&zombieEvent, zombiePtr, "onWaitingToRiftEnd");

                playAnimWithCallback func = ((playAnimWithCallback)getActualOffset(0x8DCEDC));

                zombie->m_animHandle = func(animRig, "attack", 3, zombieEvent);
            }
        }
    }
    else {
        ((zombieEnterState)getActualOffset(0xC3D428))(zombie, 18, 0);
    }
}

void ZombieEightiesBass::GuitarIdleOnExit(ZombieEightiesBass* zombie)
{

}

void ZombieEightiesBass::GuitarBreakOnEnter(ZombieEightiesBass* zombie)
{
	RegisterEventAfterAnim(zombie, "attack_off", "onBreakingCompleted");
}

void ZombieEightiesBass::GuitarBreakOnLoop(ZombieEightiesBass* zombie)
{

}

void ZombieEightiesBass::GuitarBreakOnExit(ZombieEightiesBass* zombie)
{

}

void ZombieEightiesBass::GrandDebutOnEnter(ZombieEightiesBass* zombie)
{
    auto animRig = reinterpret_cast<ZombieAnimRig_EightiesBass*>(zombie->m_animRig.Get());
	zombie->m_isInGrandDebut = true;
	animRig->m_hasGuitar = true;
	RegisterEventAfterAnim(zombie, "attack_on", "onDebutCompleted");
}

void ZombieEightiesBass::GrandDebutOnLoop(ZombieEightiesBass* zombie)
{
}

void ZombieEightiesBass::GrandDebutOnExit(ZombieEightiesBass* zombie)
{

}
void RiftingCompletedCallback(Zombie* zombie) {

}

void DebutCompletedCallback(Zombie* zombie) {
	ZombieEightiesBass* bassZombie = static_cast<ZombieEightiesBass*>(zombie);
	if (bassZombie) {
        typedef PlantGroup* (*getTarg)(ZombieEightiesBass*);
        getTarg getTarget = (getTarg)getActualOffset(0xC41910);
        PlantGroup* target = getTarget(bassZombie);
        float rawPosX;
        float rawPosY;
        int spawnPosX;
        int spawnPosY;

        if (target != nullptr && target->IsType(PlantGroup::StaticGetType())) {
            rawPosX = target->m_position.x;
            rawPosY = target->m_position.y;

            spawnPosX = (int)(((rawPosX - 232.0f) / 64.0f));
            spawnPosY = (int)(((rawPosY - 160.0f) / 76.0f));


            DamageInfo dmg;
            dmg.m_attacker = bassZombie;

            void** vtable = *(void***)target;
            typedef void (*VirtualTakeDamageFunc)(PlantGroup*, DamageInfo*);
            VirtualTakeDamageFunc takeDmg = (VirtualTakeDamageFunc)vtable[36];
            takeDmg(target, &dmg);
        }
        else {
            rawPosX = bassZombie->m_position.x;
            rawPosY = bassZombie->m_position.y;

            spawnPosX = (int)(((rawPosX - 232.0f) / 64.0f) + 0.5f) - 1;
            spawnPosY = (int)(((rawPosY - 160.0f) / 76.0f));
        }

        Board* board = Board::GetBoard();

        if (spawnPosX < 0) spawnPosX = 0;
        if (spawnPosX > 8) spawnPosX = 8;
        if (spawnPosY < 0) spawnPosY = 0;
        if (spawnPosY > 4) spawnPosY = 4;

        typedef GridItemSurfboard* (*funcAA230C)(Board*, SexyString, int, int);
        funcAA230C func_AA230C = (funcAA230C)getActualOffset(0xAA230C);
        GridItemSurfboard* gridItem = func_AA230C(board, "speaker", spawnPosX, spawnPosY);

		bassZombie->m_isInGrandDebut = false;
		((zombieEnterState)getActualOffset(0xC3D428))(bassZombie, 17, 0);
	}
}
void BreakingCompletedCallback(Zombie* zombie) {
	ZombieEightiesBass* bassZombie = static_cast<ZombieEightiesBass*>(zombie);
	if (bassZombie) {
        auto animRig = reinterpret_cast<ZombieAnimRig_EightiesBass*>(bassZombie->m_animRig.Get());
        animRig->m_hasGuitar = false;
		((zombieEnterState)getActualOffset(0xC3D428))(bassZombie, 1, 0);
	}
}
void ZombieEightiesBass::ModInit() {
    LOGI("ZombieMiner mod init");

    vftable = CreateChildVFTable(204 + 15, getActualOffset(0x241D430), 204);
    PatchVFTable(vftable, (void*)ZombieEightiesBass::StaticGetType, 0);

    PatchVFTable(vftable, (void*)BassShouldIgnoreCollision, 43);
    PatchVFTable(vftable, (void*)BassActivateJam, 64);
    PatchVFTable(vftable, (void*)BassDeactivateJam, 65);
    PatchVFTable(vftable, (void*)BassGetJamStyle, 66);
    PatchVFTable(vftable, (void*)BassActionFrame, 170);

    PatchVFTable(vftable, (void*)ZombieEightiesBass::GuitarAttackOnEnter, 204);
    PatchVFTable(vftable, (void*)ZombieEightiesBass::GuitarAttackOnLoop, 205);
    PatchVFTable(vftable, (void*)ZombieEightiesBass::GuitarAttackOnExit, 206);

    PatchVFTable(vftable, (void*)ZombieEightiesBass::GuitarIdleOnEnter, 207);
    PatchVFTable(vftable, (void*)ZombieEightiesBass::GuitarIdleOnLoop, 208);
    PatchVFTable(vftable, (void*)ZombieEightiesBass::GuitarIdleOnExit, 209);

    PatchVFTable(vftable, (void*)ZombieEightiesBass::GuitarBreakOnEnter, 210);
    PatchVFTable(vftable, (void*)ZombieEightiesBass::GuitarBreakOnLoop, 211);
    PatchVFTable(vftable, (void*)ZombieEightiesBass::GuitarBreakOnExit, 212);

    PatchVFTable(vftable, (void*)ZombieEightiesBass::GrandDebutOnEnter, 213);
    PatchVFTable(vftable, (void*)ZombieEightiesBass::GrandDebutOnLoop, 214);
    PatchVFTable(vftable, (void*)ZombieEightiesBass::GrandDebutOnExit, 215);

    ZombieEightiesBass::StaticGetType();
    LOGI("ZombieMiner finish init");
}

void ZombieEightiesBass::buildEventCallbacks(Reflection::CRefManualSymbolBuilder* builder, Reflection::RClass* rtClass)
{
    IF_CALLBACK_NOTSETUP(ZombieEightiesBass) {
        SetupLiteralDelegate(&riftingCompletedDelegate, RiftingCompletedCallback);
        SetupLiteralDelegate(&breakingGuitarCompletedDelegate, BreakingCompletedCallback);
        SetupLiteralDelegate(&debutingCompletedDelegate, DebutCompletedCallback);
        ZombieEightiesBass_delegatesSetup = true;
        LOGI("SO TRUE");
    }
    RegisterEventCallback(builder, rtClass, "onWaitingToRiftEnd", riftingCompletedDelegate);
    RegisterEventCallback(builder, rtClass, "onBreakingCompleted", breakingGuitarCompletedDelegate);
    RegisterEventCallback(builder, rtClass, "onDebutCompleted", debutingCompletedDelegate);
    LOGI("Reg event complete");
}


void ZombieEightiesBass::buildStates()
{
    StateMachineTableBuilder* stateMachine = CallGetStateMachine(ZombieEightiesBass::StaticGetType());
    RegisterStateByOffsets(stateMachine,
        16,
        (uintptr_t)ZombieEightiesBass::GuitarAttackOnEnter,
        (uintptr_t)ZombieEightiesBass::GuitarAttackOnLoop,
        (uintptr_t)ZombieEightiesBass::GuitarAttackOnExit,
        "ZS_Bassist_Rifting");
    RegisterStateByOffsets(stateMachine,
        17,
        (uintptr_t)ZombieEightiesBass::GuitarIdleOnEnter,
        (uintptr_t)ZombieEightiesBass::GuitarIdleOnLoop,
        (uintptr_t)ZombieEightiesBass::GuitarIdleOnExit,
        "ZS_Bassist_IdleRifting");
    RegisterStateByOffsets(stateMachine,
        18,
        (uintptr_t)ZombieEightiesBass::GuitarBreakOnEnter,
        (uintptr_t)ZombieEightiesBass::GuitarBreakOnLoop,
        (uintptr_t)ZombieEightiesBass::GuitarBreakOnExit,
        "ZS_Bassist_BreakAGuitar");
    RegisterStateByOffsets(stateMachine,
        19,
        (uintptr_t)ZombieEightiesBass::GrandDebutOnEnter,
        (uintptr_t)ZombieEightiesBass::GrandDebutOnLoop,
        (uintptr_t)ZombieEightiesBass::GrandDebutOnExit,
        "ZS_Bassist_GrandDebut");
    LOGI("Reg state complete");
}
