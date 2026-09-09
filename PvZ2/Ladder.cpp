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


void* ZombieLadder::vftable = nullptr;
void* ZombieLadder::vftable1 = nullptr;
Sexy::RtClass* ZombieLadder::s_rtClass = nullptr;;

void* ZombieLadderProps::vftable = __null;
Sexy::RtClass* ZombieLadderProps::s_rtClass = __null;

void* ZombieAnimRig_Ladder::vftable = __null;
Sexy::RtClass* ZombieAnimRig_Ladder::s_rtClass = __null;;

bool LadderrCanBeTargeted(ZombieLadder* ladder, char a2) {
    if (ladder->m_teamFlags == 0) {
        return false;
    }
    else {
        return CallFunc<bool>(0xC4D594, ladder, a2);
    }
}

void* LadderTakeDamage(ZombieLadder* ladder, DamageInfo* dmgInfo) {
    DamageInfo damageInfo = *dmgInfo;
    auto* props = reinterpret_cast<ZombieLadderProps*>(ladder->m_propertySheet.Get());

    bool isIncludeList = (props->PlantsWhichCanTargetLadder.ListType == includelist);
    bool blockDamage = isIncludeList;
    if (damageInfo.m_attacker != nullptr)
    {
        if (damageInfo.m_attacker->IsType(Plant::StaticGetType()))
        {
            Plant* plant = static_cast<Plant*>(damageInfo.m_attacker);

            if (plant->m_type.IsValid())
            {
                auto* plantType = reinterpret_cast<PlantType*>(plant->m_type.Get());
                bool isPlantInList = false;
                for (const SexyString& listedPlant : props->PlantsWhichCanTargetLadder.List)
                {
                    if (plantType->TypeName == listedPlant)
                    {
                        isPlantInList = true;
                        break;
                    }
                }

                if (isIncludeList && isPlantInList) {
                    blockDamage = false;
                }
                else if (!isIncludeList && isPlantInList) {
                    blockDamage = true;
                }
            }
        }
    }
    if (blockDamage) {
        damageInfo.m_damage = 0.0f;
    }

    typedef void* (*funcC43B90)(ZombieLadder*, DamageInfo*);
    static auto* ZTakeDmg = ((funcC43B90)getActualOffset(0xC43B90));

    void* result = ZTakeDmg(ladder, &damageInfo);
    return result;
}

inline int GetRowFromY(float y) {
    return static_cast<int>((y - 160.0f) / 76.0f);
}


void ProcessClimbingZombies(ZombieLadder* ladder) {
    float climbStartX = 20.0f;
    float climbHeight = 70.0f;

    float entryMaxX = 20.0f;
    float entryMinX = 15.0f;

    auto props = reinterpret_cast<ZombieLadderProps*>(ladder->m_propertySheet.Get());
    bool isPlantDead = !ladder->m_attachedPlant.IsValid() || reinterpret_cast<Plant*>(ladder->m_attachedPlant.Get())->m_isDead;
    bool isLadderDead = ZombieIsDeadOrDying(ladder) || isPlantDead;

    float deltaX = 0.0f;
    float deltaY = 0.0f;

    if (!isPlantDead) {
        Plant* attachedPlant = reinterpret_cast<Plant*>(ladder->m_attachedPlant.Get());
        if (attachedPlant != nullptr) {
            float targetLadderX = attachedPlant->m_position.x + 30.0f;
            float targetLadderY = attachedPlant->m_position.y;

            deltaX = targetLadderX - ladder->m_position.x;
            deltaY = targetLadderY - ladder->m_position.y;

            if (deltaX != 0.0f || deltaY != 0.0f) {
                ladder->m_position.x = targetLadderX;
                ladder->m_position.y = targetLadderY;

                if (deltaX != 0.0f) {
                    for (auto it = ladder->m_climbingZombies.begin(); it != ladder->m_climbingZombies.end(); ++it) {
                        if (it->IsValid()) {
                            Zombie* z = reinterpret_cast<Zombie*>(it->Get());
                            if (!ZombieIsDeadOrDying(z)) {
                                z->m_position.z = 0.0f;
                                z->m_realObjectFlags &= ~2;

                                ZombieAllowMovement(z, true);
                            }
                        }
                    }
                    ladder->m_climbingZombies.clear();
                }
                else {
                    for (auto it = ladder->m_climbingZombies.begin(); it != ladder->m_climbingZombies.end(); ++it) {
                        if (it->IsValid()) {
                            Zombie* z = reinterpret_cast<Zombie*>(it->Get());
                            if (!ZombieIsDeadOrDying(z)) {
                                SexyVector3 newSyncPos;
                                newSyncPos.x = z->m_position.x;
                                newSyncPos.y = targetLadderY;
                                newSyncPos.z = z->m_position.z;

                                ZombieUpdatePosition(z, &newSyncPos);
                            }
                        }
                    }
                }
            }
        }
    }

    float ladderX = ladder->m_position.x;
    float ladderY = ladder->m_position.y;

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
            if (z == ladder) continue;
            if ((z->m_zombieFlags & zombieisairborne) != 0) continue;
            if (z->m_teamFlags != 2) continue;

            float distY = std::abs(z->m_position.y - ladderY);
            if (distY > 20.0f) continue;

            if (CallFunc<bool>(0x1366D8C, props->UnclimbableZombies, z)
                || ZombieHasCondition(z, zombie_condition_freeze)
                || z->IsInGridItem()) continue;

            float distX = z->m_position.x - ladderX;
            bool isAtLadderBase = (distX <= entryMaxX && distX >= entryMinX);

            if (isAtLadderBase) {
                ZombieAllowMovement(z, false);
                z->m_realObjectFlags |= 2;
                z->m_zombieFlags |= (1 << 27);
                RtWeakPtr<RtObject> weakZ;
                weakZ.FromOther(&z->m_thisPtr);
                ladder->m_climbingZombies.push_back(weakZ);
            }
        }
    }

    bool hasNextLadder = false;

    if (!isLadderDead) {
        int currentGridX = static_cast<int>((ladderX - 200.0f) / 64.0f);
        int currentGridY = static_cast<int>((ladderY - 160.0f) / 76.0f);

        Rect checkRect;
        checkRect.mX = currentGridX - 1;
        checkRect.mY = currentGridY;
        checkRect.mWidth = 1;
        checkRect.mHeight = 1;

        std::vector<BoardEntity*> checkEntities;
        GetEntitiesInRectGrid(&checkEntities, 63, &checkRect);

        for (BoardEntity* checkEnt : checkEntities) {
            if (checkEnt != nullptr && checkEnt->IsType(ZombieLadder::StaticGetType())) {
                ZombieLadder* potentialLadder = reinterpret_cast<ZombieLadder*>(checkEnt);
                if (potentialLadder != ladder && !ZombieIsDeadOrDying(potentialLadder)) {
                    hasNextLadder = true;
                    break;
                }
            }
        }
    }
    float dynamicClimbEndX = hasNextLadder ? -45.0f : -80.0f;

    for (auto it = ladder->m_climbingZombies.begin(); it != ladder->m_climbingZombies.end(); ) {
        if (!it->IsValid()) {
            it = ladder->m_climbingZombies.erase(it);
            continue;
        }

        Zombie* z = reinterpret_cast<Zombie*>(it->Get());

        bool isZombieDead = ZombieIsDeadOrDying(z);

        float distX = z->m_position.x - ladderX;

        bool isInClimbZone = (distX <= climbStartX && distX >= dynamicClimbEndX);
        bool justFinishedClimbing = (distX < dynamicClimbEndX && distX >= (dynamicClimbEndX - 15.0f));

        bool isKnockedOut = !isInClimbZone && !justFinishedClimbing;

        bool isForcedToDrop = isLadderDead || isKnockedOut || isZombieDead;

        if (justFinishedClimbing || isForcedToDrop) {

            z->m_position.z = 0.0f;
            z->m_realObjectFlags &= ~2;
            z->m_zombieFlags &= ~(1 << 27);
			ZombieAllowMovement(z, true);
            it = ladder->m_climbingZombies.erase(it);
        }
        else {
            ZombieAllowMovement(z, false);
            z->m_realObjectFlags |= 2;
            z->m_zombieFlags |= (1 << 27);
            float baseClimbSpeed = props->Speed * 50.0f;
            float speedScale = z->m_conditionTracker.m_speedScale;
            float facing = ZombieFacing(z);
            float timeMoving = TimeMgr::GetInstance()->m_unkTime;

            float moveDeltaX = facing * baseClimbSpeed * speedScale * timeMoving;
            float nextX = z->m_position.x - moveDeltaX;
            float nextDistX = nextX - ladderX;

            float climbUpDist = 40.0f;
            float peakX = climbStartX - climbUpDist;
            float nextExpectedZ = 0.0f;

            if (nextDistX >= climbStartX) {
                nextExpectedZ = 0.0f;
            }
            else if (nextDistX > peakX) {
                float upProgress = (climbStartX - nextDistX) / climbUpDist;
                upProgress = std::max(0.0f, std::min(upProgress, 1.0f));
                nextExpectedZ = upProgress * climbHeight;
            }
            else {
                float descentDist = peakX - dynamicClimbEndX;
                if (descentDist <= 0.0f) descentDist = 1.0f;
                float downProgress = (peakX - nextDistX) / descentDist;

                if (downProgress >= 1.0f) {
                    nextExpectedZ = 0.0f;
                }
                else {
                    downProgress = std::max(0.0f, std::min(downProgress, 1.0f));
                    nextExpectedZ = climbHeight * (1.0f - downProgress);
                }
            }

            SexyVector3 newPos;
            newPos.x = nextX;
            newPos.y = ladderY + (z->m_position.y - ladderY);
            newPos.z = nextExpectedZ;

            ZombieUpdatePosition(z, &newPos);
            ++it;
        }
    }
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

bool LadderrBlockProjectile(ZombieLadder* zombie, Projectile* proj)
{
    return !LadderrCanBeTargetedByPlant(zombie, &proj->m_instigatorType);
}

bool LadderrCanBeTossedByPlant() {
    return false;
}

bool LadderrCanTargetAtHeight() {
    return false;
}
void LadderrUpdate(ZombieLadder* zombie) {

    ProcessClimbingZombies(zombie);

    bool isPlantDead = !zombie->m_attachedPlant.IsValid() || reinterpret_cast<Plant*>(zombie->m_attachedPlant.Get())->m_isDead;
    if (isPlantDead) {
        ZombieEnterState(zombie, 4, 0);
    }

    float ladderX = zombie->m_position.x;
    float ladderY = zombie->m_position.y;
    int currentGridX = static_cast<int>((ladderX - 200.0f) / 64.0f);
    int currentGridY = static_cast<int>((ladderY - 160.0f) / 76.0f);

    Rect checkRect;
    checkRect.mX = 0;
    checkRect.mY = currentGridY - 1;
    checkRect.mWidth = 9;
    checkRect.mHeight = 3;

    std::vector<BoardEntity*> checkEntities;
    GetEntitiesInRectGrid(&checkEntities, 63, &checkRect);

    bool foundThreat = false;

    for (BoardEntity* entity : checkEntities) {
        if (entity == nullptr) continue;

        if (entity->IsType(PlantGroup::StaticGetType())) {

            PlantGroup* plantGroup = static_cast<PlantGroup*>(entity);
            size_t plantCount = plantGroup->m_plants.m_plants.size();

            if (plantCount > 20) continue;

            for (size_t j = 0; j < plantCount; ++j) {
                auto& weakPlant = plantGroup->m_plants.m_plants[j];
                if (weakPlant.IsValid()) {
                    Plant* plant = reinterpret_cast<Plant*>(weakPlant.Get());
                    if (plant != nullptr && !plant->m_isDead) {
                        if (LadderrCanBeTargetedByPlant(zombie, &plant->m_type)) {
                            foundThreat = true;
                            break;
                        }
                    }
                }
            }
        }
        else if (entity->IsType(Plant::StaticGetType())) {

            Plant* plant = static_cast<Plant*>(entity);

            if (!plant->m_isDead) {
                if (LadderrCanBeTargetedByPlant(zombie, &plant->m_type)) {
                    foundThreat = true;
                }
            }
        }
        else {
            continue;
        }

        if (foundThreat) {
            break;
        }
    }

    if (foundThreat) {
        zombie->m_teamFlags = 2;
    }
    else {
        zombie->m_teamFlags = 0;
    }

    CallFunc<void>(0xC3D7A0, zombie);
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

int LadderrCalcRenderOrder(void* renderableThis) {

    auto zombie = reinterpret_cast<ZombieLadder*>((uintptr_t)renderableThis - 16);

    return 0 + 406990 + (16000 * GetRowFromY(zombie->m_position.y));
}

void ZombieLadder::ModInit() {
    LOGI("ZombieLadder mod init");
    PVZ2HookFunction(0xC4146C, (void*)ZombieCanTargetEntitiesAtHeight, nullptr);
    vftable = CreateChildVFTable(204 + 9, getActualOffset(0x241D430), 204);
    vftable1 = CopyVFTable(getActualOffset(0x241DAA0), 4);
    PatchVFTable(vftable, (void*)ZombieLadder::StaticGetType, 0);
    PatchVFTable(vftable, (void*)LadderrCanBeTargeted, 21);
    PatchVFTable(vftable, (void*)LadderTakeDamage, 35);
    PatchVFTable(vftable, (void*)LadderrBlockProjectile, 43);
    PatchVFTable(vftable1, (void*)LadderrCalcRenderOrder, 3);
    PatchVFTable(vftable, (void*)LadderrOnGetCondition, 71);
    PatchVFTable(vftable, (void*)LadderrThreatAlert, 75);
    PatchVFTable(vftable, (void*)LadderrCanBeTargetedByPlant, 93);
    PatchVFTable(vftable, (void*)LadderrCanBeTossedByPlant, 97);
    PatchVFTable(vftable, (void*)LadderrCanTargetAtHeight, 103);
    PatchVFTable(vftable, (void*)LadderrUpdate, 29);
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

