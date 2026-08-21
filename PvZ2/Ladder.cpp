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
#include <cmath> 


void* ZombieLadder::vftable = nullptr;
Sexy::RtClass* ZombieLadder::s_rtClass = nullptr;;

void* ZombieLadderProps::vftable = __null;
Sexy::RtClass* ZombieLadderProps::s_rtClass = __null;

void* ZombieAnimRig_Ladder::vftable = __null;
Sexy::RtClass* ZombieAnimRig_Ladder::s_rtClass = __null;;


inline int GetRowFromY(float y) {
    return static_cast<int>((y - 160.0f) / 76.0f);
}

bool LadderrCanBeTargeted() {
    return false;
}

void ProcessClimbingZombies(ZombieLadder* ladder) {
    std::vector<BoardEntity*> entities;

    int gX = static_cast<int>((ladder->m_position.x - 200.0f) / 64.0f);
    int gY = static_cast<int>((ladder->m_position.y - 160.0f) / 76.0f);

    Rect scanRect;
    scanRect.mX = gX - 2;
    scanRect.mY = gY - 1;
    scanRect.mWidth = 4;
    scanRect.mHeight = 2;

    GetEntitiesInRectGrid(&entities, 63, &scanRect);

    float ladderX = ladder->m_position.x;
    float ladderY = ladder->m_position.y;

    float climbStartX = 20.0f;
    float climbEndX = -90.0f;
    float totalClimbWidth = climbStartX - climbEndX;

    float climbHeight = 70.0f;

    auto props = reinterpret_cast<ZombieLadderProps*>(ladder->m_propertySheet.Get());
    bool isPlantDead = !ladder->m_attachedPlant.IsValid() || reinterpret_cast<Plant*>(ladder->m_attachedPlant.Get())->m_isDead;
    bool isLadderDead = ZombieIsDeadOrDying(ladder) || isPlantDead;

    for (BoardEntity* entity : entities) {
        if (entity == nullptr || !entity->IsType(Zombie::StaticGetType())) continue;

        Zombie* z = reinterpret_cast<Zombie*>(entity);

        if (z == ladder || ZombieIsDeadOrDying(z)) continue;

        if (CallFunc<bool>(0x1366D8C, props->UnclimbableZombies, z)
            || ZombieHasCondition(z, zombie_condition_freeze)
            || z->IsInGridItem()
            || (CallFunc<bool>(0xC4D588, z) & 1) != 0
            || (CallFunc<bool>(0xC3E430, z) & 1) != 0) continue;

        float distX = z->m_position.x - ladderX;
        float distY = ladderY - z->m_position.y;

        float progress = (climbStartX - distX) / totalClimbWidth;
        progress = std::max(0.0f, std::min(progress, 1.0f));
        float expectedDistY = std::sin(progress * 3.14159f) * climbHeight;

        float virtualBaseY = z->m_position.y + expectedDistY;
        float virtualDistY = ladderY - virtualBaseY;

        bool isWalkingOnGround = (z->m_entityState.m_id == 1 && std::abs(ladderY - z->m_position.y) <= 25.0f);

        bool isAlreadyClimbing = (z->m_entityState.m_id == 0 && std::abs(virtualDistY) <= 25.0f);

        if (!isWalkingOnGround && !isAlreadyClimbing) continue;

        bool isInClimbZone = (distX <= climbStartX && distX >= climbEndX);
        bool justFinishedClimbing = (distX < climbEndX && distX >= (climbEndX - 15.0f));
        bool isForcedToDrop = ZombieIsInState(z, 3) || isLadderDead;

        if (justFinishedClimbing || (isInClimbZone && isForcedToDrop && isAlreadyClimbing)) {

            z->m_position.y = virtualBaseY;

            ZombieAllowMovement(z, true);

            if (z->m_entityState.m_id != 1) {
                ZombieEnterState(z, 1, 0);
            }
        }
        else if (!isLadderDead && isInClimbZone) {

            ZombieAllowMovement(z, false);

            if (z->m_entityState.m_id != 0) {
                ZombieEnterState(z, 0, 0);
            }

            float baseClimbSpeed = 48.0f;
            float speedScale = z->m_conditionTracker.m_speedScale;
            float facing = ZombieFacing(z);
            float timeMoving = TimeMgr::GetInstance()->m_unkTime;

            float moveDeltaX = facing * baseClimbSpeed * speedScale * timeMoving;

            SexyVector3 newPos;
            newPos.x = z->m_position.x - moveDeltaX;

            float nextDistX = newPos.x - ladderX;
            float nextProgress = (climbStartX - nextDistX) / totalClimbWidth;
            nextProgress = std::max(0.0f, std::min(nextProgress, 1.0f));
            float nextExpectedDistY = std::sin(nextProgress * 3.14159f) * climbHeight;

            newPos.y = virtualBaseY - nextExpectedDistY;
            newPos.z = 0.0f;

            ZombieUpdatePosition(z, &newPos);
        }
    }
}

void InitLadderDamageLayers(ZombieLadder* zombie, int damageState) {
    auto rig = reinterpret_cast<ZombieAnimRig_Ladder*>(zombie->m_animRig.Get());
    SetAnimLayerVisible(rig, "ladder", damageState == 0);
    SetAnimLayerVisible(rig, "ladder_damage1", damageState == 1);
    SetAnimLayerVisible(rig, "ladder_damage2", damageState == 2);
}

void LadderrUpdate(ZombieLadder* zombie) {
    auto props = reinterpret_cast<ZombieLadderProps*>(zombie->m_propertySheet.Get());

    int hpStage = props->Hitpoints / 3;
    if (hpStage <= 0) hpStage = 2;

    int curState = (props->Hitpoints - static_cast<int>(zombie->m_hitpoints)) / hpStage;
    curState = std::max(0, std::min(curState, 2));

    if (curState != zombie->m_damageIndex) {
        auto rig = reinterpret_cast<ZombieAnimRig_Ladder*>(zombie->m_animRig.Get());
        zombie->m_damageIndex = curState;
        rig->m_damageIndex = zombie->m_damageIndex;
        InitLadderDamageLayers(zombie, zombie->m_damageIndex);
    }

    ProcessClimbingZombies(zombie);

    bool isPlantDead = !zombie->m_attachedPlant.IsValid() || reinterpret_cast<Plant*>(zombie->m_attachedPlant.Get())->m_isDead;
    if (isPlantDead) {
        ZombieEnterState(zombie, 4, 0);
    }

    CallFunc<void>(0xC3D7A0, zombie);
}

int64_t LadderrThreatAlert() {
    return 0;
}

bool LadderrCanBeTargetedByPlant(ZombieLadder* zombie, RtWeakPtr<PlantType> pType) {
   
    auto props = reinterpret_cast<ZombieLadderProps*>(zombie->m_propertySheet.Get());
    if (CallFunc<bool>(0x1364888, &props->PlantsWhichCanTargetLadder, pType)) {
       return true;
    }
    return CallFunc<bool>(0xC5677C, zombie);
}

bool LadderrCanBeTossedByPlant() {
    return false;
}

bool LadderrCanTargetPlant() {
    return false;
}


void ZombieLadder::ModInit() {
    LOGI("ZombieLadder mod init");
    vftable = CreateChildVFTable(204 + 9, getActualOffset(0x241D430), 204);
    PatchVFTable(vftable, (void*)ZombieLadder::StaticGetType, 0);
    PatchVFTable(vftable, (void*)LadderrCanBeTargeted, 21);
    PatchVFTable(vftable, (void*)LadderrUpdate, 29);
    PatchVFTable(vftable, (void*)LadderrThreatAlert, 75);
    PatchVFTable(vftable, (void*)LadderrCanBeTargetedByPlant, 93);
    PatchVFTable(vftable, (void*)LadderrCanBeTossedByPlant, 97);
    PatchVFTable(vftable, (void*)LadderrCanTargetPlant, 103);
    ZombieLadder::StaticGetType();
    LOGI("ZombieLadder finish init");
}


void ZombieLadderProps::modInit() {
    LOGI("ZombieLadderProps init");

    vftable = CopyVFTable(getActualOffset(0x24328D0), 14);

    PatchVFTable(vftable, (void*)ZombieLadderProps::StaticGetType, 0);

    ZombieLadderProps::StaticGetType();

    LOGI("ZombieLadderProps finish init");
}

SexyString GetLadderParticle(ZombieAnimRig_Ladder* thisPtr) {
    if (thisPtr->m_damageIndex == 0) {
        return "ladder";
    }
    else {
        SexyString ladderParticle = "ladder_damage" + std::to_string(thisPtr->m_damageIndex);
        return ladderParticle;
    }
}

SexyString GetIdleAnim() {
    return "ladder";
}
SexyString GetWalkAnim(ZombieAnimRig_ModernLadder* thisptr) {
    return "ladder";
}
SexyString GetEatAnim(ZombieAnimRig_ModernLadder* thisptr) {
    return "ladder";
}
SexyString GetDieAnim(ZombieAnimRig_ModernLadder* thisptr) {
    return "";
}


void ZombieAnimRig_Ladder::modInit() {
    LOGI("ZombieAnimRig_Ladder init");

    vftable = CopyVFTable(getActualOffset(0x23ABF70), 67);

    PatchVFTable(vftable, (void*)ZombieAnimRig_Ladder::StaticGetType, 0);
    PatchVFTable(vftable, (void*)GetLadderParticle, 44);
    PatchVFTable(vftable, (void*)GetIdleAnim, 58);
    PatchVFTable(vftable, (void*)GetWalkAnim, 59);
    PatchVFTable(vftable, (void*)GetEatAnim, 61);
    PatchVFTable(vftable, (void*)GetDieAnim, 63);
    ZombieAnimRig_Ladder::StaticGetType();

    LOGI("ZombieAnimRig_Ladder finish init");
}

