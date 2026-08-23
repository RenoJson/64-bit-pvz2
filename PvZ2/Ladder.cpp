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

void ProcessClimbingZombies(ZombieLadder* ladder) {
    float ladderX = ladder->m_position.x;
    float ladderY = ladder->m_position.y;

    float climbStartX = 30.0f;
    float climbEndX = -80.0f;
    float totalClimbWidth = climbStartX - climbEndX;
    float climbHeight = 70.0f;

    float entryMaxX = 30.0f;
    float entryMinX = 20.0f;

    auto props = reinterpret_cast<ZombieLadderProps*>(ladder->m_propertySheet.Get());
    bool isPlantDead = !ladder->m_attachedPlant.IsValid() || reinterpret_cast<Plant*>(ladder->m_attachedPlant.Get())->m_isDead;
    bool isLadderDead = ZombieIsDeadOrDying(ladder) || isPlantDead;

    if (!isLadderDead) {
        std::vector<BoardEntity*> entities;
        int gX = static_cast<int>((ladder->m_position.x - 200.0f) / 64.0f);
        int gY = static_cast<int>((ladder->m_position.y - 160.0f) / 76.0f);

        Rect scanRect;
        scanRect.mX = gX - 2;
        scanRect.mY = gY;
        scanRect.mWidth = 4;
        scanRect.mHeight = 1;

        GetEntitiesInRectGrid(&entities, 63, &scanRect);

        for (BoardEntity* entity : entities) {
            if (entity == nullptr || !entity->IsType(Zombie::StaticGetType())) continue;
            Zombie* z = reinterpret_cast<Zombie*>(entity);
            if (z == ladder || ZombieIsDeadOrDying(z)) continue;

            if (z->m_entityState.m_id != 1) continue;
            bool hasHoverFlag = (z->m_realObjectFlags & 2) != 0;
            if (hasHoverFlag) continue;

            float distY = std::abs(z->m_position.y - ladderY);
            if (distY > 10.0f) continue;

            if (CallFunc<bool>(0x1366D8C, props->UnclimbableZombies, z)
                || ZombieHasCondition(z, zombie_condition_freeze)
                || z->IsInGridItem()
                || (CallFunc<bool>(0xC4D588, z) & 1) != 0
                || (CallFunc<bool>(0xC3E430, z) & 1) != 0) continue;

            float distX = z->m_position.x - ladderX;

            bool isAtLadderBase = (distX <= entryMaxX && distX >= entryMinX);

            if (isAtLadderBase) {
                ZombieAllowMovement(z, false);
                z->m_realObjectFlags |= 2;

                RtWeakPtr<RtObject> weakZ;
                weakZ.FromOther(&z->m_thisPtr);
                ladder->m_climbingZombies.push_back(weakZ);
            }
        }
    }

    for (auto it = ladder->m_climbingZombies.begin(); it != ladder->m_climbingZombies.end(); ) {
        if (!it->IsValid()) {
            it = ladder->m_climbingZombies.erase(it);
            continue;
        }

        Zombie* z = reinterpret_cast<Zombie*>(it->Get());
        if (ZombieIsDeadOrDying(z)) {
            it = ladder->m_climbingZombies.erase(it);
            continue;
        }

        float distX = z->m_position.x - ladderX;

        bool isInClimbZone = (distX <= climbStartX && distX >= climbEndX);
        bool justFinishedClimbing = (distX < climbEndX && distX >= (climbEndX - 15.0f));

        bool isKnockedOut = !isInClimbZone && !justFinishedClimbing;
        bool isForcedToDrop = isLadderDead || isKnockedOut;

        if (justFinishedClimbing || isForcedToDrop) {

            z->m_position.z = 0.0f; 
            z->m_realObjectFlags &= ~2;
            ZombieAllowMovement(z, true);
            ZombieSetUnmovableStatusFlag(z, false);
            it = ladder->m_climbingZombies.erase(it);
        }
        else {
            ZombieSetUnmovableStatusFlag(z, true);
            float baseClimbSpeed = 48.0f;
            float speedScale = z->m_conditionTracker.m_speedScale;
            float facing = ZombieFacing(z);
            float timeMoving = TimeMgr::GetInstance()->m_unkTime;

            float moveDeltaX = facing * baseClimbSpeed * speedScale * timeMoving;
            float nextX = z->m_position.x - moveDeltaX;
            float nextDistX = nextX - ladderX;

            float nextProgress = (climbStartX - nextDistX) / totalClimbWidth;
            nextProgress = std::max(0.0f, std::min(nextProgress, 1.0f));

            float nextExpectedZ = std::sin(nextProgress * 3.14159f) * climbHeight;

            SexyVector3 newPos;
            newPos.x = nextX;
            newPos.y = z->m_position.y;
            newPos.z = nextExpectedZ;

            ZombieUpdatePosition(z, &newPos);

            ++it;
        }
    }
}

void LadderrUpdate(ZombieLadder* zombie) {
   
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

void LadderrOnGetCondition(ZombieLadder* zombie, int conditionID)
{
    ZombieRemoveCondition(zombie, conditionID);
}

bool LadderrCanBeTargetedByPlant(ZombieLadder* zombie, RtWeakPtr<PlantType>* pType) {

    auto props = reinterpret_cast<ZombieLadderProps*>(zombie->m_propertySheet.Get());
    bool isRestricted = CallFunc<bool>(0x1364D84, &props->PlantsWhichCanTargetLadder, pType);
    return !isRestricted;
}

bool LadderrCanBeTossedByPlant() {
    return false;
}

bool LadderrCanTargetAtHeight() {
    return false;
}

bool ZombieCanTargetEntitiesAtHeight(Zombie* thisPtr, BoardEntityHeight entityHeight) {
    if ((thisPtr->m_realObjectFlags & 2) != 0) {
        return false;
    }
    return entityHeight > BoardEntityHeight::ground;
}


bool LadderrCanBeElectrocuted() {
    return false;
}

bool LadderrCanBeAshed() {
    return false;
}


void ZombieLadder::ModInit() {
    LOGI("ZombieLadder mod init");
    PVZ2HookFunction(0xC4146C, (void*)ZombieCanTargetEntitiesAtHeight, nullptr);
    vftable = CreateChildVFTable(204 + 9, getActualOffset(0x241D430), 204);
    PatchVFTable(vftable, (void*)ZombieLadder::StaticGetType, 0);
    PatchVFTable(vftable, (void*)LadderrUpdate, 29);
    PatchVFTable(vftable, (void*)LadderrOnGetCondition, 71);
    PatchVFTable(vftable, (void*)LadderrThreatAlert, 75);
    PatchVFTable(vftable, (void*)LadderrCanBeTargetedByPlant, 93);
    PatchVFTable(vftable, (void*)LadderrCanBeTossedByPlant, 97);
    PatchVFTable(vftable, (void*)LadderrCanTargetAtHeight, 103);
    PatchVFTable(vftable, (void*)LadderrCanBeElectrocuted, 191);
    PatchVFTable(vftable, (void*)LadderrCanBeAshed, 192);
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
    return "ladder";
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

