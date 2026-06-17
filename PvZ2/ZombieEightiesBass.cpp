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
#include "ZombieHelper.h"
#include "AddGridItemType.h"

void* ZombieEightiesBass::vftable = nullptr;
Sexy::RtClass* ZombieEightiesBass::s_rtClass = nullptr;;
typedef Zombie* (*setEffectAnim)(Zombie*, const char*, const char*, const char*, SexyVector3*, uint, bool, bool, uint);
typedef int (*playAnimWithCallback)(ZombieAnimRig*, const SexyString&, int, ZombieEvent& event);
typedef ZombieEvent* (*ConstructEvent)(ZombieEvent*, RtWeakPtr<Zombie>& owner, const SexyString& eventName);
typedef bool (*isAnimDone)(ZombieAnimRig*, int);

typedef bool (*isTossedByPlant)(Zombie*, int);
DECLARE_DELEGATES_SETUP(ZombieEightiesBass)

static Sexy::DelegateBase riftingCompletedDelegate;

static Sexy::DelegateBase breakingGuitarCompletedDelegate;

static Sexy::DelegateBase debutingCompletedDelegate;

void* BassTakeDamage(ZombieEightiesBass* zombie, DamageInfo* damageInfo)
{
    auto* props = reinterpret_cast<ZombieEightiesBassProps*>(zombie->m_propertySheet.Get());
    DamageInfo newDmgInfo = *damageInfo;
    if(zombie->m_isInGrandDebut == true)
    {
        newDmgInfo.m_damage = 0.0f;
	}
    typedef void* (*funcC43B90)(ZombieEightiesBass*, DamageInfo*);
    static auto* ZTakeDmg = ((funcC43B90)getActualOffset(0xC43B90));

    return ZTakeDmg(zombie, &newDmgInfo);
}
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
    if(zombie->m_isRifting == true){
        setEffectAnim setAnim = (setEffectAnim)getActualOffset(0x7BF03C);
		SexyVector3 transformOffset = { 20.0f, -30.0f, 0.0f };
        if (zombie->m_attachedEffects.GetObjectIndex("rifting_idle") == -1) {
            setAnim(zombie, "rifting_idle", "POPANIM_EFFECTS_ZOMBIE_POTION_EFFECT", "idle", &transformOffset, -1, false, false, 2);
        }
	}
    return zombie;
}
Zombie* BassDeactivateJam(ZombieEightiesBass* zombie) {
    zombie->m_isJamming = false;
    if (zombie->m_isRifting == true) {
        typedef int (*removeEffectAnim)(AttachedEffectManager*, const SexyString&);
        removeEffectAnim removeAnim = (removeEffectAnim)getActualOffset(0x662360);
        removeAnim(&zombie->m_attachedEffects, "rifting_idle");
        setEffectAnim setAnim = (setEffectAnim)getActualOffset(0x7BF03C);
        SexyVector3 transformOffset = { 20.0f, -30.0f, 0.0f };
        if (zombie->m_attachedEffects.GetObjectIndex("rifting_end") == -1) {
            setAnim(zombie, "rifting_end", "POPANIM_EFFECTS_ZOMBIE_POTION_EFFECT", "over", &transformOffset, -1, true, false, 2);
        }
    }
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
bool BassIsBeingTossedByPlant(ZombieEightiesBass* zombie, int a2) {
    if(zombie->m_entityState.m_id == 16 
        || zombie->m_entityState.m_id == 17
        || zombie->m_entityState.m_id == 19) {
        return false;
	}
    else {
        return ((isTossedByPlant)getActualOffset(0xC4D2EC))(zombie, a2);
    }
}
void BassWalkOnLoop(ZombieEightiesBass* zombie)
{
    if (zombie->m_teamFlags == 0) {
		zombie->m_teamFlags = 2;
    }
    else if(zombie->m_isInGrandDebut == true) {
		zombie->m_isInGrandDebut = false;
	}
    CallFunc<void, ZombieEightiesBass*>(0xC506B4, zombie);
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

                    int oldFlags = zombie->m_teamFlags;
                    zombie->m_teamFlags = 2; 

                    DamageInfo dmg;
                    dmg.m_attacker = zombie;
                    void** plantVtable = *(void***)plant;
                    typedef DamageInfo* (*PlantTakeDamageFunc)(Plant*, DamageInfo*);
                    PlantTakeDamageFunc takeDmg = (PlantTakeDamageFunc)plantVtable[36];

                    takeDmg(plant, &dmg);

                    zombie->m_teamFlags = oldFlags; 
                }
            }
        }
    }
}

void ZombieEightiesBass::GuitarAttackOnEnter(ZombieEightiesBass* zombie)
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
    GetEntitiesInRectGrid(&entityList, 63, &scanRect);
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
        ZombiePlaySoundEvent(zombie, props->SoundOnRifting, 0.0f);
        Projectile* shockwave = AddProjectile(&props->ShockWaveProjectile,
                                                zombie,
                                                targetSpeaker->m_position.x - props->ShockWaveSpawnOffset.x,
                                                targetSpeaker->m_position.y - props->ShockWaveSpawnOffset.y,
                                                0);
        shockwave->m_teamFlags = zombie->m_teamFlags;
        ZombieEnterState(zombie, 17, 0);
    }
    else {
        ZombieEnterState(zombie, 18, 0);
    }
}

void ZombieEightiesBass::GuitarAttackOnLoop(ZombieEightiesBass* zombie)
{
   
}

void ZombieEightiesBass::GuitarAttackOnExit(ZombieEightiesBass* zombie)
{
    
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
    GetEntitiesInRectGrid(&entityList, 63, &scanRect);
    GridItemSpeaker* targetSpeaker = nullptr;
    for (BoardEntity* ptr : entityList) {
        if (ptr == nullptr) continue;

        if (ptr->IsType(GridItemSpeaker::StaticGetType())) {
            targetSpeaker = reinterpret_cast<GridItemSpeaker*>(ptr);
            break;
        }
    }

    if (targetSpeaker != nullptr) {
        if (animDone)
        {
            auto* props = reinterpret_cast<ZombieEightiesBassProps*>(zombie->m_propertySheet.Get());
            if (zombie->m_isJamming && zombie->m_elapsedTimeInState >= props->ShockWaveSpawnIntervalWhileJamming) {
                ZombieEnterState(zombie, 16, 0);
		    }
            else if (!zombie->m_isJamming && zombie->m_elapsedTimeInState >= props->ShockWaveSpawnInterval) {
                ZombieEnterState(zombie, 16, 0);
		    }
            else {
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
        ZombieEnterState(zombie, 18, 0);
    }
}
void BassOnGetCondition(ZombieEightiesBass* zombie, int conditionID)
{
    if (conditionID == zombie_condition_hypnotized)
    {
        ZombieEnterState(zombie, 18, 0);
    }
}
void ZombieEightiesBass::GuitarIdleOnExit(ZombieEightiesBass* zombie)
{

}

void ZombieEightiesBass::GuitarBreakOnEnter(ZombieEightiesBass* zombie)
{
    if (zombie->m_isRifting == true && zombie->m_isJamming == true) {
        typedef int (*removeEffectAnim)(AttachedEffectManager*, const SexyString&);
        removeEffectAnim removeAnim = (removeEffectAnim)getActualOffset(0x662360);
        removeAnim(&zombie->m_attachedEffects, "rifting_idle");
        setEffectAnim setAnim = (setEffectAnim)getActualOffset(0x7BF03C);
        SexyVector3 transformOffset = { 20.0f, -30.0f, 0.0f };
        if (zombie->m_attachedEffects.GetObjectIndex("rifting_end") == -1) {
            setAnim(zombie, "rifting_end", "POPANIM_EFFECTS_ZOMBIE_POTION_EFFECT", "over", &transformOffset, -1, true, false, 2);
        }
		zombie->m_isRifting = false;
    }
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
    zombie->m_teamFlags = 0;
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
        GridItemSpeaker* gridItem = (GridItemSpeaker*)AddGridItem(props->SpeakerType, spawnPosX, spawnPosY);
        gridItem->m_speakerState = 2;
        bassZombie->m_isInGrandDebut = false;
		bassZombie->m_teamFlags = 2;
        ZombieEnterState(bassZombie, 17, 0);
    }
}
void BreakingCompletedCallback(Zombie* zombie) {
	ZombieEightiesBass* bassZombie = static_cast<ZombieEightiesBass*>(zombie);
	if (bassZombie) {
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
    if(zombie->IsType(ZombieEightiesBass::StaticGetType())) {
        ZombieEightiesBass* bassZombie = static_cast<ZombieEightiesBass*>(zombie);
        auto* props = reinterpret_cast<ZombieEightiesBassProps*>(bassZombie->m_propertySheet.Get());
        offset.x += props->DebutOffset.x;
        offset.y += props->DebutOffset.y;
	}
    ZombieEnterState(zombie, 19, 0);
    return ZombieUpdatePosition(zombie, &offset);
}
void ZombieEightiesBass::ModInit() {
    LOGI("ZombieBass mod init");
    PVZ2HookFunction(0x110BD94, (void*)updateParachutePos, nullptr);
    vftable = CreateChildVFTable(204 + 15, getActualOffset(0x241D430), 204);
    PatchVFTable(vftable, (void*)ZombieEightiesBass::StaticGetType, 0);

    PatchVFTable(vftable, (void*)BassTakeDamage, 35);
    PatchVFTable(vftable, (void*)BassShouldIgnoreCollision, 43);
    PatchVFTable(vftable, (void*)BassActivateJam, 64);
    PatchVFTable(vftable, (void*)BassDeactivateJam, 65);
    PatchVFTable(vftable, (void*)BassGetJamStyle, 66);
    PatchVFTable(vftable, (void*)BassOnGetCondition, 71);
    PatchVFTable(vftable, (void*)BassIsBeingTossedByPlant, 97);
    PatchVFTable(vftable, (void*)BassWalkOnLoop, 124);
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
