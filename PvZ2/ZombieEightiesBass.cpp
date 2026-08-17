#include "ZombieEightiesBass.h"
#include "ZombieModernBungee.h"
#include "ZombieStateHelper.h"
#include "ZombieEightiesBassProps.h"
#include "ZombieState.h"
#include "StateMachineBuilder.h"
#include "ZombieAnimRig_EightiesBass.h"
#include "Plant.h"
#include "GridItem.h"
#include "AddProjectileType.h"
#include "DamageInfo.h"
#include "ZombieHelper.h"
#include "AddGridItemType.h"
#include "GridItemSpeakerBass.h"

void* ZombieEightiesBass::vftable = nullptr;
Sexy::RtClass* ZombieEightiesBass::s_rtClass = nullptr;;

DECLARE_DELEGATES_SETUP(ZombieEightiesBass)

static Sexy::DelegateBase riftingCompletedDelegate;

static Sexy::DelegateBase breakingGuitarCompletedDelegate;

static Sexy::DelegateBase debutingCompletedDelegate;

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

    int styleCount = props->JamStyle.size();

    if (styleCount == 0) {
        return "";
    }

    int randomIndex = std::rand() % styleCount;
    SexyString chosenStyle = props->JamStyle[randomIndex];
    return props->JamStyle[randomIndex];
}

void BassOnGetCondition(ZombieEightiesBass* zombie, int conditionID)
{
    if (conditionID == zombie_condition_hypnotized)
    {
        ZombieEnterState(zombie, 17, 0);
    }
}

void BassActionFrame(ZombieEightiesBass* zombie, int64_t unk1, SexyString* actionName, int64_t unk2, SexyString* currentAnim)
{
    if (*actionName == "kill_plant")
    {
        int gX = static_cast<int>((zombie->m_position.x - 200.0f) / 64.0f);
        int gY = static_cast<int>((zombie->m_position.y - 160.0f) / 76.0f);

        if (gY > 4) gY = 4;
        if (gY < 0) gY = 0;

        Rect scanRect;
        scanRect.mX = gX - 1;
        scanRect.mY = gY;
        scanRect.mWidth = 1;
        scanRect.mHeight = 1;

        std::vector<BoardEntity*> entityList;
        GetEntitiesInRectGrid(&entityList, 63, &scanRect);

        auto* props = reinterpret_cast<ZombieEightiesBassProps*>(zombie->m_propertySheet.Get());

        for (BoardEntity* ptr : entityList) {
            if (ptr == nullptr) continue;

            if (ptr->IsType(PlantGroup::StaticGetType()))
            {
                PlantGroup* group = static_cast<PlantGroup*>(ptr);

                for (auto& weakPlantPtr : group->m_plants.m_plants)
                {
                    Plant* plant = weakPlantPtr.Get();
                    if (plant == nullptr) continue;
                    bool isInvincible = CallFunc<bool, PlantFramework*>(0x7C8E58, plant->m_plantFramework);
                    if (plant->m_isInPlantFoodState || isInvincible) {
                        continue; 
                    }
                    DamageInfo dmg;
                    dmg.m_attacker = zombie;
                    CallVirtualFunc<DamageInfo*>(plant, 36, &dmg);
                }
            }
        }
    }
}

void BassOnHeadDrop(ZombieEightiesBass* zombie)
{
    if (zombie->m_speaker.IsValid())
    {
        auto speaker = reinterpret_cast<GridItemSpeakerBass*>(zombie->m_speaker.Get());
        if (speaker->m_speakerState == 5) {
            GridItemSpeakerBass::SpeakerSetState(speaker, 2);
        }
    }
}


void ZombieEightiesBass::GuitarIdleOnEnter(ZombieEightiesBass* zombie)
{
    auto dlgtEvent = RegisterDelegateEvent(zombie, "onWaitingToRiftEnd");
    auto rig = reinterpret_cast<ZombieAnimRig_EightiesBass*>(zombie->m_animRig.Get());
    PlayAndContinueAnim(rig, "attack", 3, dlgtEvent);
}

void ZombieEightiesBass::GuitarIdleOnLoop(ZombieEightiesBass* zombie)
{
    auto* props = reinterpret_cast<ZombieEightiesBassProps*>(zombie->m_propertySheet.Get());
    float targetCooldown = zombie->m_isJamming ?
        props->ShockWaveSpawnIntervalWhileJamming :
        props->ShockWaveSpawnInterval;
    if (zombie->m_speaker.IsValid())
    {
        auto speaker = reinterpret_cast<GridItemSpeakerBass*>(zombie->m_speaker.Get());
        if (zombie->m_elapsedTimeInState <= 0.01f)
        {
            speaker->m_nextFireTime = 0.0f;
        }
        if (speaker->m_speakerState == 5)
        {
            speaker->m_nextFireTime = zombie->m_elapsedTimeInState;
        }
        else if (zombie->m_elapsedTimeInState - speaker->m_nextFireTime >= targetCooldown)
        {
            GridItemSpeakerBass::SpeakerSetState(speaker, 5);
        }
    }
    else
    {
        ZombieEnterState(zombie, 17, 0);
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
    auto* props = reinterpret_cast<ZombieEightiesBassProps*>(zombie->m_propertySheet.Get());
    ZombiePlaySoundEvent(zombie, props->SoundOnDebut, 0.0f);
    auto animRig = reinterpret_cast<ZombieAnimRig_EightiesBass*>(zombie->m_animRig.Get());
	animRig->m_hasGuitar = true;
    ZombieSetInvincibleStatusFlag(zombie, true);
    ZombieSetNoCollisionFlag(zombie, true);
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
        float rawPosX;
        float rawPosY;
        int spawnPosX;
        int spawnPosY;

        rawPosX = bassZombie->m_position.x;
        rawPosY = bassZombie->m_position.y;

        spawnPosX = (int)(((rawPosX - 200.0f) / 64.0f)) - 1;
        spawnPosY = (int)(((rawPosY - 160.0f) / 76.0f));

        if (spawnPosX < 0) spawnPosX = 0;
        if (spawnPosX > 8) spawnPosX = 8;
        if (spawnPosY < 0) spawnPosY = 0;
        if (spawnPosY > 4) spawnPosY = 4;
        auto* props = reinterpret_cast<ZombieEightiesBassProps*>(bassZombie->m_propertySheet.Get());
        GridItemSpeakerBass* gridItem = (GridItemSpeakerBass*)AddGridItem(props->SpeakerType, spawnPosX, spawnPosY);
        gridItem->m_speakerState = 2;
        gridItem->m_teamFlags = bassZombie->m_teamFlags;
        gridItem->m_realObjectFlags &= ~1;
        bassZombie->m_speaker.FromOther(&gridItem->m_thisPtr);
        ZombieSetInvincibleStatusFlag(bassZombie, false);
        ZombieSetNoCollisionFlag(bassZombie, false);
        ZombieEnterState(bassZombie, 16, 0);
    }
}
void BreakingCompletedCallback(Zombie* zombie) {
	ZombieEightiesBass* bassZombie = static_cast<ZombieEightiesBass*>(zombie);
	if (bassZombie && !ZombieIsDeadOrDying(bassZombie)) {
        auto animRig = reinterpret_cast<ZombieAnimRig_EightiesBass*>(bassZombie->m_animRig.Get());
        animRig->m_hasGuitar = false;
        auto* props = reinterpret_cast<ZombieEightiesBassProps*>(bassZombie->m_propertySheet.Get());
        bassZombie->m_dpsScale = props->EnragedDamageScale;
        ZombieSetSpeedScale(bassZombie, props->EnragedSpeedScale);
		ZombieEnterState(bassZombie, 1, 0);
	}
}
// a4 is parachute rain module
Zombie* updateParachutePos(float mX, float mY, float mZ, void* a4, Zombie* zombie) {
	SexyVector3 offset = { zombie->m_position.x, zombie->m_position.y, 0 };
    int state;
    if(zombie->IsType(ZombieEightiesBass::StaticGetType())) {
        ZombieEightiesBass* bassZombie = static_cast<ZombieEightiesBass*>(zombie);
        auto* props = reinterpret_cast<ZombieEightiesBassProps*>(bassZombie->m_propertySheet.Get());
        offset.x += props->DebutOffset.x;
        offset.y += props->DebutOffset.y;
        state = 18;
	}
    else if (zombie->IsType(ZombieModernBungee::StaticGetType())) {
        state = 16;
    }
    else {
        state = 19;
    }
    ZombieEnterState(zombie, state, 0);
    return ZombieUpdatePosition(zombie, &offset);
}
void ZombieEightiesBass::ModInit() {
    LOGI("ZombieBass mod init");
    PVZ2HookFunction(0x110BD94, (void*)updateParachutePos, nullptr);
    vftable = CreateChildVFTable(204 + 15, getActualOffset(0x241D430), 204);
    PatchVFTable(vftable, (void*)ZombieEightiesBass::StaticGetType, 0);

    PatchVFTable(vftable, (void*)BassActivateJam, 64);
    PatchVFTable(vftable, (void*)BassDeactivateJam, 65);
    PatchVFTable(vftable, (void*)BassGetJamStyle, 66);
    PatchVFTable(vftable, (void*)BassOnGetCondition, 71);
    PatchVFTable(vftable, (void*)BassActionFrame, 170);
    PatchVFTable(vftable, (void*)BassOnHeadDrop, 171);

    PatchVFTable(vftable, (void*)ZombieEightiesBass::GuitarIdleOnEnter, 204);
    PatchVFTable(vftable, (void*)ZombieEightiesBass::GuitarIdleOnLoop, 205);
    PatchVFTable(vftable, (void*)ZombieEightiesBass::GuitarIdleOnExit, 206);

    PatchVFTable(vftable, (void*)ZombieEightiesBass::GuitarBreakOnEnter, 207);
    PatchVFTable(vftable, (void*)ZombieEightiesBass::GuitarBreakOnLoop, 208);
    PatchVFTable(vftable, (void*)ZombieEightiesBass::GuitarBreakOnExit, 209);

    PatchVFTable(vftable, (void*)ZombieEightiesBass::GrandDebutOnEnter, 210);
    PatchVFTable(vftable, (void*)ZombieEightiesBass::GrandDebutOnLoop, 211);
    PatchVFTable(vftable, (void*)ZombieEightiesBass::GrandDebutOnExit, 212);

    ZombieEightiesBass::StaticGetType();
    LOGI("ZombieBass finish init");
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
        (uintptr_t)ZombieEightiesBass::GuitarIdleOnEnter,
        (uintptr_t)ZombieEightiesBass::GuitarIdleOnLoop,
        (uintptr_t)ZombieEightiesBass::GuitarIdleOnExit,
        "ZS_Bassist_Rifting");
    RegisterStateByOffsets(stateMachine,
        17,
        (uintptr_t)ZombieEightiesBass::GuitarBreakOnEnter,
        (uintptr_t)ZombieEightiesBass::GuitarBreakOnLoop,
        (uintptr_t)ZombieEightiesBass::GuitarBreakOnExit,
        "ZS_Bassist_WreckingGuitar");
    RegisterStateByOffsets(stateMachine,
        18,
        (uintptr_t)ZombieEightiesBass::GrandDebutOnEnter,
        (uintptr_t)ZombieEightiesBass::GrandDebutOnLoop,
        (uintptr_t)ZombieEightiesBass::GrandDebutOnExit,
        "ZS_Bassist_GrandDebut");
    LOGI("Reg state complete");
}
