#include "ZombieTombRaiser.h"
#include "ZombieAnimRig_TombRaiser.h"
#include "ZombieTombRaiserProps.h"
#include "TimeMgr.h"
#include "Sexy/LawnApp.h"
#include "StateMachineBuilder.h"
#include "ZombieHelper.h"
#include "Board.h"
#include "AddProjectileType.h"
#include "ZombieStateHelper.h"
#include "AddGridItemType.h"
#include "GridItemMausoleumLawnTile.h"

void* ZombieMysticFormation::vftable = nullptr;
Sexy::RtClass* ZombieMysticFormation::s_rtClass = nullptr;

void* TokenLayersConfig::vftable = nullptr;
Sexy::RtClass* TokenLayersConfig::s_rtClass = nullptr;

void* ZombieMysticFormationProps::vftable = nullptr;
Sexy::RtClass* ZombieMysticFormationProps::s_rtClass = nullptr;

DECLARE_DELEGATES_SETUP(ZombieMysticFormation)

static Sexy::DelegateBase throwDelegate;

static Sexy::DelegateBase spawnDoneDelegate;

bool IsPendingTokens(ZombieMysticFormation* thisPtr, int gridX, int gridY)
{
    for (const SpellBolt& bolt : thisPtr->m_pendingTokens)
    {
        if (bolt.TargetCellX == gridX && bolt.TargetCellY == gridY)
        {
            return true;
        }
    }
    return false;
}

Sexy::Point FindMausoleumGrid(ZombieMysticFormation* self)
{
    Board* board = Board::GetBoard();
    std::vector<BoardEntity*> entityList;

    GetEntitiesInRectGrid(&entityList, 63, &board->m_lawnRect);

    std::vector<Sexy::Point> validTiles;

    for (BoardEntity* ptr : entityList) {
        if (ptr != nullptr && ptr->IsType(GridItemMausoleumLawnTile::StaticGetType()))
        {
            auto* tile = reinterpret_cast<GridItemMausoleumLawnTile*>(ptr);

            int col = tile->m_gridLocation.mX;
            int row = tile->m_gridLocation.mY;

            if (CanSpawnGridItemAt(col, row, false, true) && !IsPendingTokens(self, col, row))
            {
                Sexy::Point validPoint;
                validPoint.mX = col;
                validPoint.mY = row;
                validTiles.push_back(validPoint);
            }
        }
    }

    if (!validTiles.empty()) {
        int randomIndex = std::rand() % validTiles.size();
        return validTiles[randomIndex];
    }

    Sexy::Point invalidPoint;
    invalidPoint.mX = -1;
    invalidPoint.mY = -1;
    return invalidPoint;
}

bool FindTargetGrid(ZombieMysticFormation* self, Point* outTarget)
{
    Sexy::Point tilePoint = FindMausoleumGrid(self);

    if (tilePoint.mX != -1)
    {
        *outTarget = tilePoint; 
        return true;  
    }

    Board* board = Board::GetBoard();
    int zX = static_cast<int>((self->m_position.x - 200.0f) / 64.0f);
    int zY = static_cast<int>((self->m_position.y - 160.0f) / 76.0f);

    int targetCol = zX - 2;
    if (targetCol <= 2) {
        targetCol = 2;
    }

    int maxCol = std::min(9, targetCol + 2);
    std::vector<Point> validGrids;

    for (int col = targetCol - 1; col < maxCol; ++col)
    {
        for (int row = 0; row < board->m_gridSizeY; ++row)
        {
            if (CanSpawnGridItemAt(col, row, false, true) && !IsPendingTokens(self, col, row))
            {
                Point validPoint;
                validPoint.mX = col;
                validPoint.mY = row;
                validGrids.push_back(validPoint);
            }
        }
    }

    if (validGrids.empty()) {
        return false;
    }

    auto* props = reinterpret_cast<ZombieMysticFormationProps*>(self->m_propertySheet.Get());

    if (self->m_remainingAmmo == props->NumberOfTokensToSpawn)
    {
        if (CanSpawnGridItemAt(7, zY, false, true) && !IsPendingTokens(self, 7, zY))
        {
            outTarget->mX = 7;
            outTarget->mY = zY;
            return true;
        }
    }

    int randomIndex = std::rand() % validGrids.size();
    *outTarget = validGrids[randomIndex];

    return true;
}

void MysticProjectileLayerChange(ZombieMysticFormation* zombie)
{
    auto props = reinterpret_cast<ZombieMysticFormationProps*>(zombie->m_propertySheet.Get());
    auto rig = reinterpret_cast<ZombieAnimRig_MysticFormation*>(zombie->m_animRig.Get());

    bool isGreen = static_cast<bool>(zombie->m_projectileIndex); 

    SetAnimLayerVisible(rig, props->TokenProjectileLayerProps.GreenTokenProjectileLayerName, isGreen);
    SetAnimLayerVisible(rig, props->TokenProjectileLayerProps.RedTokenProjectileLayerName, !isGreen);
}

void MysticOnSpawn(ZombieMysticFormation* zombie) {
    ZombieOnSpawn(zombie);
    zombie->m_projectileIndex = rand() % 2;
    MysticProjectileLayerChange(zombie);
    zombie->m_startThrowTime = 0;
    ZombieEnterState(zombie, 1, 0);
}
void MysticWalkOnLoop(ZombieMysticFormation* zombie) {
    if (zombie->m_position.x > 744.0f || zombie->m_teamFlags != 2 || ZombieIsDeadOrDying(zombie)) {
        CallFunc<void, ZombieMysticFormation*>(0xC506B4, zombie);
        return;
    }

    float currentTime = TimeMgr::GetInstance()->m_curTime;
    if (currentTime > zombie->m_startThrowTime)
    {
        if (FindTargetGrid(zombie, &zombie->m_throwingTarget))
        {
            auto* props = reinterpret_cast<ZombieMysticFormationProps*>(zombie->m_propertySheet.Get());
            zombie->m_remainingTokenspawnAttempts = props->NumberOfTokensToSpawn;
            ZombieEnterState(zombie, 16, 0);
            return;
        }
    }
    CallFunc<void, ZombieMysticFormation*>(0xC506B4, zombie);
}
void MysticOnCreate(ZombieMysticFormation* zombie) {
    auto props = reinterpret_cast<ZombieMysticFormationProps*>(zombie->m_propertySheet.Get());
    zombie->m_remainingAmmo = props->Ammo;
}
void MysticActionFrame(ZombieMysticFormation* zombie,
    SexyString* currentAnim,
    SexyString* actionName,
    SexyString* param,
    float nextFrameTime)
{
    if (*actionName == "token_swap") {
        zombie->m_projectileIndex = rand() % 2;
        MysticProjectileLayerChange(zombie);
    }
}
void ZombieMysticFormation::WaitingOnEnter(ZombieMysticFormation* zombie)
{
	RegisterEventOnIdleLoop(zombie, "onIdleAnimationCycle");
}

void ZombieMysticFormation::WaitingOnLoop(ZombieMysticFormation* zombie)
{
	auto props = reinterpret_cast<ZombieMysticFormationProps*>(zombie->m_propertySheet.Get());
    if (zombie->m_elapsedTimeInState > props->TimeBetweenCasts) {
        if (FindTargetGrid(zombie, &zombie->m_throwingTarget)) {
            ZombieEnterState(zombie, 17, 0);
        }
        else {
            ZombieEnterState(zombie, 1, 0);
        }
    }

}

void ZombieMysticFormation::WaitingOnExit(ZombieMysticFormation* zombie)
{

}

void ZombieMysticFormation::ThrowOnEnter(ZombieMysticFormation* zombie)
{
	RegisterEventAfterAnim(zombie, "power", "onSpawnAnimDone");
	auto dlgtEvent = RegisterDelegateEvent(zombie, "onThrow");
	auto rig = reinterpret_cast<ZombieAnimRig_MysticFormation*>(zombie->m_animRig.Get());
	ZombieAnimRig_MysticFormation::OnInitializeAnimRigDelegate(rig, dlgtEvent);
}

void ZombieMysticFormation::ThrowOnLoop(ZombieMysticFormation* zombie)
{

}

void ZombieMysticFormation::ThrowOnExit(ZombieMysticFormation* zombie)
{

}

void MysticOnSpawnDoneCallback(Zombie* zombie) {
    ZombieMysticFormation* mysticZombie = static_cast<ZombieMysticFormation*>(zombie);

    if (!mysticZombie) return;

    mysticZombie->m_remainingTokenspawnAttempts--;

    if (mysticZombie->m_remainingTokenspawnAttempts > 0) {
        if (FindTargetGrid(mysticZombie, &mysticZombie->m_throwingTarget)) {
            ZombieMysticFormation::ThrowOnEnter(mysticZombie);
            return;
        }
    }
    if (mysticZombie->m_remainingAmmo < 1) {
        mysticZombie->m_startThrowTime = FLT_MAX;
    }
    else {
        auto props = reinterpret_cast<ZombieMysticFormationProps*>(mysticZombie->m_propertySheet.Get());
        float currentTime = TimeMgr::GetInstance()->m_curTime;
        float cooldown = props->TimeBetweenRaisings;

        mysticZombie->m_startThrowTime = currentTime + cooldown;
    }
    if (!ZombieIsDeadOrDying(mysticZombie)) {
        ZombieEnterState(zombie, 1, 0); 
    }
}

void MysticOnThrowCallback(Zombie* zombie) {
    ZombieMysticFormation* mysticZombie = static_cast<ZombieMysticFormation*>(zombie);
    if (mysticZombie && !ZombieIsDeadOrDying(mysticZombie)) {
        auto props = reinterpret_cast<ZombieMysticFormationProps*>(mysticZombie->m_propertySheet.Get());
        auto rig = reinterpret_cast<ZombieAnimRig_MysticFormation*>(zombie->m_animRig.Get());
        Rect tokenRect;
        GetAnimRigSpriteRect(rig, props->TokenProjectileLayerProps.ProjectileOffsetTokenLayerName, &tokenRect);
        ZombieConditionTracker* zTracker = &zombie->m_conditionTracker;
        uint8_t* cond = zTracker->m_states.data();

        if (cond != nullptr && *cond != 0) {
            typedef void (*UpdateConditionsFunc)(ZombieConditionTracker*);
            ((UpdateConditionsFunc)(*(void***)zTracker)[3])(zTracker);
            *cond = 0;
        }

        float scale = zTracker->m_scale;
        auto sexyApp = SexyApp::GetInstance();
        int spriteXPos = sexyApp->ScaleRender(tokenRect.mX + (tokenRect.mWidth / 2));
        int spriteYPos = sexyApp->ScaleRender(tokenRect.mY + (tokenRect.mHeight / 2));
        SexyVector2 artCenter = props->ArtCenter;
        SexyVector3 shadowOffset = props->ShadowOffset;
        float spawnPosX = mysticZombie->m_position.x + (scale * ((float)spriteXPos - artCenter.x));
        float spawnPosY = mysticZombie->m_position.y - mysticZombie->m_position.z;
        float spawnPosZ = scale * ((float)spriteYPos - shadowOffset.x);
        auto* projectileToSpawn = (mysticZombie->m_projectileIndex == 1)
            ? &props->TokenProjectileLayerProps.GreenTokenProjectile
            : &props->TokenProjectileLayerProps.RedTokenProjectile;

        auto projectile = (TombraiserProjectile*)AddProjectile(projectileToSpawn, mysticZombie, spawnPosX, spawnPosY, spawnPosZ);
        float targetPosX = (mysticZombie->m_throwingTarget.mX * 64.0f) + 232.0f;
        float targetPosY = (mysticZombie->m_throwingTarget.mY * 76.0f) + 222.0f;
        projectile->m_targetGridLoc = mysticZombie->m_throwingTarget;
        SexyVector3 launchPos = { targetPosX, targetPosY, 0 };
        FirePultProjectile(projectile, launchPos, 250.0f, 1.5f);
        mysticZombie->m_remainingAmmo--;
    }
}

void TokenLayersConfig::modInit() {
    LOGI("TokenLayersConfig init");

    vftable = CopyVFTable(getActualOffset(0x2428660), 14);

    PatchVFTable(vftable, (void*)TokenLayersConfig::StaticGetType, 0);

    TokenLayersConfig::StaticGetType();

    LOGI("TokenLayersConfig finish init");
}


void ZombieMysticFormation::modInit() {

    vftable = CreateChildVFTable(204 + 6, getActualOffset(0x241D430), 204);
    PatchVFTable(vftable, (void*)ZombieMysticFormation::StaticGetType, 0);
    PatchVFTable(vftable, (void*)MysticOnSpawn, 49);
    PatchVFTable(vftable, (void*)MysticWalkOnLoop, 124);
    PatchVFTable(vftable, (void*)MysticOnCreate, 169);
    PatchVFTable(vftable, (void*)MysticActionFrame, 170);

    PatchVFTable(vftable, (void*)ZombieMysticFormation::WaitingOnEnter, 204);
    PatchVFTable(vftable, (void*)ZombieMysticFormation::WaitingOnLoop, 205);
    PatchVFTable(vftable, (void*)ZombieMysticFormation::WaitingOnExit, 206);

    PatchVFTable(vftable, (void*)ZombieMysticFormation::ThrowOnEnter, 207);
    PatchVFTable(vftable, (void*)ZombieMysticFormation::ThrowOnLoop, 208);
    PatchVFTable(vftable, (void*)ZombieMysticFormation::ThrowOnExit, 209);

    ZombieMysticFormation::StaticGetType();
}

void ZombieMysticFormationProps::modInit() {
    LOGI("ZombieArcherProps init");

    vftable = CopyVFTable(getActualOffset(0x24328D0), 14);

    PatchVFTable(vftable, (void*)ZombieMysticFormationProps::StaticGetType, 0);

    ZombieMysticFormationProps::StaticGetType();

    LOGI("ZombieArcherProps finish init");
}

void ZombieMysticFormation::buildEventCallbacks(Reflection::CRefManualSymbolBuilder* builder, Reflection::RClass* rtClass)
{
    IF_CALLBACK_NOTSETUP(ZombieMysticFormation) {
        SetupLiteralDelegate(&throwDelegate, MysticOnThrowCallback);
        SetupLiteralDelegate(&spawnDoneDelegate, MysticOnSpawnDoneCallback);
        ZombieMysticFormation_delegatesSetup = true;
    }
    RegisterEventCallback(builder, rtClass, "onThrow", throwDelegate);
    RegisterEventCallback(builder, rtClass, "onSpawnAnimDone", spawnDoneDelegate);
}

void ZombieMysticFormation::buildStates()
{
    StateMachineTableBuilder* stateMachine = CallGetStateMachine(ZombieMysticFormation::StaticGetType());
    RegisterStateByOffsets(stateMachine,
        16,
        (uintptr_t)ZombieMysticFormation::WaitingOnEnter,
        (uintptr_t)ZombieMysticFormation::WaitingOnLoop,
        (uintptr_t)ZombieMysticFormation::WaitingOnExit,
        "ZS_Mystic_Waiting");
    RegisterStateByOffsets(stateMachine,
        17,
        (uintptr_t)ZombieMysticFormation::ThrowOnEnter,
        (uintptr_t)ZombieMysticFormation::ThrowOnLoop,
        (uintptr_t)ZombieMysticFormation::ThrowOnExit,
        "ZS_Mystic_Throw");
    LOGI("Reg state complete");
}


