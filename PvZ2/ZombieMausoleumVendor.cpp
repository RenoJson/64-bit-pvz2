#include "ZombieMausoleumVendor.h"
#include "ZombieStateHelper.h"
#include "ZombieMausoleumVendorProps.h"
#include "TimeMgr.h"
#include "AddZombieType.h"
#include "ZombieAnimRig_Vendor.h"
#include "StateMachineBuilder.h"
#include "ZombieHelper.h"
#include "GridItemMausoleumLawnTile.h"

void* ZombieMausoleumVendor::vftable = nullptr;
Sexy::RtClass* ZombieMausoleumVendor::s_rtClass = nullptr;

DECLARE_DELEGATES_SETUP(ZombieMausoleumVendor)

static Sexy::DelegateBase setPigCompletedDelegate;

void MausoleumVendorOnSpawn(ZombieMausoleumVendor* zombie) {
	zombie->m_firstSpawned = false;
	zombie->m_currentDirection = 0;
	zombie->m_lastPathGridX = 0;
	zombie->m_lastPathGridY = 0;
	ZombieOnSpawn(zombie);
}
void MausoleumVendorOnAffectedByCondition(ZombieMausoleumVendor* zombie, int condition) {
	if (condition == zombie_condition_shrinking) {
		auto* rig = reinterpret_cast<ZombieAnimRig_Vendor*>(zombie->m_animRig.Get());
		SetAnimLayerVisible(rig, "pig_1_body", false);
		SetAnimLayerVisible(rig, "pig_1_body1", false);
		SetAnimLayerVisible(rig, "pig_1_ear", false);
		SetAnimLayerVisible(rig, "pig_1_ear1", false);
		SetAnimLayerVisible(rig, "pig_3_bottom", false);
		SetAnimLayerVisible(rig, "pig_2", false);
		SetAnimLayerVisible(rig, "pig_1_ear", false);
		SetAnimLayerVisible(rig, "pig_3_head", false);
		SetAnimLayerVisible(rig, "pig_mouth", false);
		SetAnimLayerVisible(rig, "pig_nose", false);
		SetAnimLayerVisible(rig, "pig_face", false);
		SetAnimLayerVisible(rig, "pig_body", false);
		SetAnimLayerVisible(rig, "pig_leg_2", false);
		SetAnimLayerVisible(rig, "pig_leg_4", false);
		SetAnimLayerVisible(rig, "pig_leg_1", false);
		SetAnimLayerVisible(rig, "pig_leg_3", false);
		SetAnimLayerVisible(rig, "pig_ear_right", false);
		SetAnimLayerVisible(rig, "pig_ear_left", false);
		SetAnimLayerVisible(rig, "pig_tail", false);
	}
}
void MausoleumVendorWalkOnLoop(ZombieMausoleumVendor* zombie) {
    auto props = reinterpret_cast<ZombieMausoleumVendorProps*>(zombie->m_propertySheet.Get());
    float pX = zombie->m_position.x;
    float pY = zombie->m_position.y;
    float pZ = zombie->m_position.z;

    int gX = static_cast<int>((pX - 200.0f) / 64.0f);
    int gY = static_cast<int>((pY - 160.0f) / 76.0f);
    int currentZomRow = static_cast<int>((pY - 160.0f) / 76.0f);

    Rect feetRect;
    feetRect.mX = static_cast<int>(pX) - 10;
    feetRect.mY = static_cast<int>(pY - pZ) - 10;
    feetRect.mWidth = 20;
    feetRect.mHeight = 20;

    std::vector<BoardEntity*> entities;

    int minRow = std::max(0, currentZomRow - 1);
    int maxRow = std::min(5, currentZomRow + 1);
    GetEntitiesInRectPixel(&entities, 63, &feetRect, minRow, maxRow);

    GridItemMausoleumLawnPath* pathTile = nullptr;

    for (BoardEntity* ptr : entities) {
        if (ptr == nullptr) continue;

        if (ptr->IsType(GridItemMausoleumLawnPath::StaticGetType()))
        {
            GridItem* tile = static_cast<GridItem*>(ptr);

            if (tile->m_gridLocation.mX == gX && tile->m_gridLocation.mY == gY) {
                pathTile = static_cast<GridItemMausoleumLawnPath*>(tile);
                break;
            }
        }
    }

    if (pathTile != nullptr)
    {
        ZombieAllowMovement(zombie, false);
        ZombieSetSpeedScale(zombie, props->TunnelSpeedScale);

        ZombieConditionTracker* zTracker = &zombie->m_conditionTracker;
        uint8_t* cond = zTracker->m_states.data();
        if (cond != nullptr && *cond != 0) {
            typedef void (*UpdateConditionsFunc)(ZombieConditionTracker*);
            ((UpdateConditionsFunc)(*(void***)zTracker)[3])(zTracker);
            *cond = 0;
        }

        float baseSpeed = props->TunnelSpeed;
        float speedScale = zTracker->m_speedScale;
        float floatingSpeed = baseSpeed * speedScale * 64.0f;
        float timeMoving = TimeMgr::GetInstance()->m_unkTime;
        float stepDist = floatingSpeed * timeMoving;

        float pixelCenterX = 232.0f + (gX * 64.0f);
        float pixelCenterY = 220.0f + (gY * 76.0f);
        bool reachedCenter = false;
        float nextX = pX;
        float nextY = pY;

        switch (zombie->m_currentDirection) {
        case 0:
            nextX -= stepDist;
            if (pX >= pixelCenterX && nextX <= pixelCenterX) reachedCenter = true;
            break;
        case 3:
            nextX += stepDist;
            if (pX <= pixelCenterX && nextX >= pixelCenterX) reachedCenter = true;
            break;
        case 1:
            nextY += stepDist;
            if (pY <= pixelCenterY && nextY >= pixelCenterY) reachedCenter = true;
            break;
        case 2:
            nextY -= stepDist;
            if (pY >= pixelCenterY && nextY <= pixelCenterY) reachedCenter = true;
            break;
        }

        if (reachedCenter)
        {
            if (gX != zombie->m_lastPathGridX || gY != zombie->m_lastPathGridY)
            {
                auto* pathProps = reinterpret_cast<GridItemMausoleumLawnPathProps*>(pathTile->m_propertySheet.Get());
                int numChoices = pathProps->DirectionType.size();

                if (numChoices > 0)
                {
                    int oppositeDirection = -1;
                    switch (zombie->m_currentDirection) {
                    case 0: oppositeDirection = 3; break;
                    case 1: oppositeDirection = 2; break;
                    case 2: oppositeDirection = 1; break;
                    case 3: oppositeDirection = 0; break;
                    }

                    std::vector<int> validTurns;
                    bool isStraightValid = false;

                    for (int i = 0; i < numChoices; i++)
                    {
                        int checkDir = pathProps->DirectionType[i];

                        if (checkDir != oppositeDirection)
                        {
                            bool routeLeadsToHouse = false;

                            if (checkDir == 0) {
                                routeLeadsToHouse = true;
                            }
                            else if (checkDir == 3 && zombie->m_currentDirection != 3) {
                                routeLeadsToHouse = false;
                            }
                            else
                            {
                                int simX = gX;
                                int simY = gY;
                                int simDir = checkDir;

                                for (int step = 0; step < 6; step++) {
                                    if (simDir == 0) simX -= 1;
                                    else if (simDir == 1) simY += 1;
                                    else if (simDir == 2) simY -= 1;
                                    else if (simDir == 3) simX += 1;

                                    if (simX < 0 || simX > 8 || simY < 0 || simY > 4) break;

                                    float simPixelX = 232.0f + (simX * 64.0f);
                                    float simPixelY = 220.0f + (simY * 76.0f);
                                    Rect simRect;
                                    simRect.mX = static_cast<int>(simPixelX) - 10;
                                    simRect.mY = static_cast<int>(simPixelY) - 10;
                                    simRect.mWidth = 20;
                                    simRect.mHeight = 20;

                                    std::vector<BoardEntity*> simEnts;
                                    int nMinRow = std::max(0, simY - 1);
                                    int nMaxRow = std::min(5, simY + 1);
                                    GetEntitiesInRectPixel(&simEnts, 63, &simRect, nMinRow, nMaxRow);

                                    GridItemMausoleumLawnPath* simTile = nullptr;
                                    for (BoardEntity* ptr : simEnts) {
                                        if (ptr != nullptr && ptr->IsType(GridItemMausoleumLawnPath::StaticGetType())) {
                                            GridItem* nextTile = static_cast<GridItem*>(ptr);
                                            if (nextTile->m_gridLocation.mX == simX && nextTile->m_gridLocation.mY == simY) {
                                                simTile = static_cast<GridItemMausoleumLawnPath*>(nextTile);
                                                break;
                                            }
                                        }
                                    }

                                    if (simTile == nullptr) break;

                                    auto* simProps = reinterpret_cast<GridItemMausoleumLawnPathProps*>(simTile->m_propertySheet.Get());
                                    bool foundLeft = false;
                                    bool canGoStraight = false;
                                    int turnDir = -1;

                                    int oppSimDir = -1;
                                    switch (simDir) {
                                    case 0: oppSimDir = 3; break;
                                    case 1: oppSimDir = 2; break;
                                    case 2: oppSimDir = 1; break;
                                    case 3: oppSimDir = 0; break;
                                    }

                                    for (size_t j = 0; j < simProps->DirectionType.size(); j++) {
                                        int d = simProps->DirectionType[j];
                                        if (d == 0) foundLeft = true;
                                        if (d == simDir) canGoStraight = true;
                                        if (d != oppSimDir) turnDir = d;
                                    }

                                    if (foundLeft) {
                                        routeLeadsToHouse = true;
                                        break;
                                    }

                                    if (!canGoStraight) {
                                        if (turnDir == -1 || turnDir == 3) break;
                                        simDir = turnDir;
                                    }
                                }
                            }

                            if (routeLeadsToHouse) {
                                if (checkDir == zombie->m_currentDirection) {
                                    isStraightValid = true;
                                }
                                else {
                                    validTurns.push_back(checkDir);
                                }
                            }
                        }
                    }

                    int nextDirection = zombie->m_currentDirection;

                    if (!isStraightValid && !validTurns.empty())
                    {
                        int randomIndex = rand() % validTurns.size();
                        nextDirection = validTurns[randomIndex];
                    }
                    else if (isStraightValid && !validTurns.empty())
                    {
                        int roll = rand() % 100;
                        if (roll < 40) {
                            nextDirection = zombie->m_currentDirection;
                        }
                        else {
                            int randomIndex = rand() % validTurns.size();
                            nextDirection = validTurns[randomIndex];
                        }
                    }
                    else if (isStraightValid)
                    {
                        nextDirection = zombie->m_currentDirection;
                    }
                    else
                    {
                        nextDirection = 0;
                    }

                    if (gY >= 4 && nextDirection == 1)      nextDirection = 0;
                    else if (gY <= 0 && nextDirection == 2) nextDirection = 0;
                    else if (gX >= 8 && nextDirection == 3) nextDirection = 0;

                    if (nextDirection == 0 || nextDirection == 3) {
                        pY = pixelCenterY;
                    }
                    else if (nextDirection == 1 || nextDirection == 2) {
                        pX = pixelCenterX;
                    }
                    zombie->m_currentDirection = nextDirection;
                }
                zombie->m_lastPathGridX = gX;
                zombie->m_lastPathGridY = gY;
            }
        }

        switch (zombie->m_currentDirection) {
        case 0: pX -= stepDist; break;
        case 1: pY += stepDist; break;
        case 2: pY -= stepDist; break;
        case 3: pX += stepDist; break;
        }

        SexyVector3 newPos = { pX, pY, zombie->m_position.z };
        ZombieUpdatePosition(zombie, &newPos);

        bool isShrunken = (ZombieHasCondition(zombie, zombie_condition_shrinking) || ZombieHasCondition(zombie, zombie_condition_shrunken));
        if (!isShrunken) {
            if (zombie->m_firstSpawned == false) {
                if (TimeMgr::GetInstance()->m_curTime >= zombie->m_creationTime + props->SetPigInterval) {
                    zombie->m_firstSpawned = true;
                    ZombieEnterState(zombie, 16, 0);
                }
            }
            else if (TimeMgr::GetInstance()->m_curTime >= zombie->m_nextPigTime) {
                ZombieEnterState(zombie, 16, 0);
            }
        }
        CallFunc<void, ZombieMausoleumVendor*>(0xC506B4, zombie);
    }
    else
    {
        zombie->m_currentDirection = 0;
        ZombieAllowMovement(zombie, true);
        ZombieSetSpeedScale(zombie, 1.0f);

        bool isShrunken = (ZombieHasCondition(zombie, zombie_condition_shrinking) || ZombieHasCondition(zombie, zombie_condition_shrunken));
        if (!isShrunken) {
            if (zombie->m_firstSpawned == false) {
                if (TimeMgr::GetInstance()->m_curTime >= zombie->m_creationTime + props->SetPigInterval) {
                    zombie->m_firstSpawned = true;
                    ZombieEnterState(zombie, 16, 0);
                }
            }
            else if (TimeMgr::GetInstance()->m_curTime >= zombie->m_nextPigTime) {
                ZombieEnterState(zombie, 16, 0);
            }
        }
        CallFunc<void, ZombieMausoleumVendor*>(0xC506B4, zombie);
    }
}

void ZombieMausoleumVendor::PigOnEnter(ZombieMausoleumVendor* zombie)
{
   float pX = zombie->m_position.x;
    float pY = zombie->m_position.y;
    float pZ = zombie->m_position.z;

    int gX = static_cast<int>((pX - 200.0f) / 64.0f);
    int gY = static_cast<int>((pY - 160.0f) / 76.0f);
    int currentZomRow = static_cast<int>((pY - 160.0f) / 76.0f);

    Rect feetRect;
    feetRect.mX = static_cast<int>(pX) - 10;
    feetRect.mY = static_cast<int>(pY - pZ) - 10;
    feetRect.mWidth = 20;
    feetRect.mHeight = 20;

    std::vector<BoardEntity*> entities;

    int minRow = std::max(0, currentZomRow - 1);
    int maxRow = std::min(5, currentZomRow + 1);
    GetEntitiesInRectPixel(&entities, 63, &feetRect, minRow, maxRow);

    GridItemMausoleumLawnPath* pathTile = nullptr;

    for (BoardEntity* ptr : entities) {
        if (ptr == nullptr) continue;

        if (ptr->IsType(GridItemMausoleumLawnPath::StaticGetType()))
        {
            GridItem* tile = static_cast<GridItem*>(ptr);

            if (tile->m_gridLocation.mX == gX && tile->m_gridLocation.mY == gY) {
                pathTile = static_cast<GridItemMausoleumLawnPath*>(tile);
                break;
            }
        }
    }

    if (pathTile != nullptr)
    {
        ZombieAllowMovement(zombie, false);
    }
    else {
        ZombieAllowMovement(zombie, true);
    }
	RegisterEventAfterAnim(zombie, "set_pig", "onSetPigCompleted");
}

void ZombieMausoleumVendor::PigOnLoop(ZombieMausoleumVendor* zombie)
{

}

void ZombieMausoleumVendor::PigOnExit(ZombieMausoleumVendor* zombie)
{

}

void MausoleumVendorCompletedCallback(Zombie* zombie) {
	ZombieMausoleumVendor* vendorZombie = static_cast<ZombieMausoleumVendor*>(zombie);
	if (vendorZombie) {
		auto* props = reinterpret_cast<ZombieMausoleumVendorProps*>(vendorZombie->m_propertySheet.Get());
		vendorZombie->m_nextPigTime = TimeMgr::GetInstance()->m_curTime + props->SetPigInterval;
		ZombieEnterState(vendorZombie, 1, 0);
	}
}


void SpawnZombiePig1(ZombieMausoleumVendor* self) {
	if (!ZombieHasCondition(self, zombie_condition_shrinking) && !ZombieHasCondition(self, zombie_condition_shrunken))
	{
		auto* props = reinterpret_cast<ZombieMausoleumVendorProps*>(self->m_propertySheet.Get());
		SexyString name = props->PigType;
		Zombie* spawnedRider = AddZombie(name, -1, 6, -1);
		float newX = self->m_position.x - props->PigSpawnOffset.x;
		if (ZombieHasCondition(self, zombie_condition_hypnotized))
		{
			newX = self->m_position.x + props->PigSpawnOffset.x;
		}
		float newY = self->m_position.y - props->PigSpawnOffset.y;
		float newZ = self->m_position.z - props->PigSpawnOffset.z;
		SexyVector3 newCoords = SexyVector3(newX, newY, newZ);
		ZombieSetPosition(spawnedRider, &newCoords);
		if (ZombieHasCondition(self, zombie_condition_hypnotized)) {

			ZombieSetCondition(spawnedRider, zombie_condition_hypnotized, 0, 3.4028e38f, 0.0f);

			spawnedRider->m_teamFlags = self->m_teamFlags;
			typedef void* (*GetHypnoDataFunc)(Zombie*);
			GetHypnoDataFunc funGetHypnoData = (GetHypnoDataFunc)getActualOffset(0xC3E6DC);

			void* hypnoData = funGetHypnoData(self);
			typedef void (*ApplyHypnoDataFunc)(Zombie*, void*);
			ApplyHypnoDataFunc funApplyHypnoData = (ApplyHypnoDataFunc)getActualOffset(0xC41290);

			funApplyHypnoData(spawnedRider, hypnoData);
		}

	}
}

void SpawnZombiePigAfterDie1(ZombieMausoleumVendor* self) {
	if (!ZombieHasCondition(self, zombie_condition_shrinking) && !ZombieHasCondition(self, zombie_condition_shrunken))
	{
		auto* props = reinterpret_cast<ZombieMausoleumVendorProps*>(self->m_propertySheet.Get());

		int spawnCount = props->NumberOfPigToSpawnWhenDie;
		SexyString name = props->PigType;
		float baseX = self->m_position.x - props->PigSpawnOffset.x;
		if (ZombieHasCondition(self, zombie_condition_hypnotized))
		{
			baseX = self->m_position.x + props->PigSpawnOffset.x;
		}
		float baseY = self->m_position.y - props->PigSpawnOffset.y;
		float baseZ = self->m_position.z - props->PigSpawnOffset.z;
		for (int i = 0; i < spawnCount; i++)
		{
			Zombie* spawnedRider = AddZombie(name, -1, 6, -1);
			if (spawnedRider)
			{
				float offsetX = (float)(rand() % 50 - 10);

				SexyVector3 finalCoords = SexyVector3(baseX + offsetX, baseY, baseZ);

				ZombieSetPosition(spawnedRider, &finalCoords);
				if (ZombieHasCondition(self, zombie_condition_hypnotized)) {

					ZombieSetCondition(spawnedRider, zombie_condition_hypnotized, 0, 3.4028e38f, 0.0f);

					spawnedRider->m_teamFlags = self->m_teamFlags;
					typedef void* (*GetHypnoDataFunc)(Zombie*);
					GetHypnoDataFunc funGetHypnoData = (GetHypnoDataFunc)getActualOffset(0xC3E6DC);

					void* hypnoData = funGetHypnoData(self);
					typedef void (*ApplyHypnoDataFunc)(Zombie*, void*);
					ApplyHypnoDataFunc funApplyHypnoData = (ApplyHypnoDataFunc)getActualOffset(0xC41290);

					funApplyHypnoData(spawnedRider, hypnoData);
				}
			}
		}
	}
}

void MausoleumVendorActionFrame(ZombieMausoleumVendor* vendor, SexyString* currentAnim, SexyString* actionName, SexyString* param, float nextFrameTime)
{
	if (*actionName == "use_action")
	{
		SpawnZombiePig1(vendor);
	}
	else if (*actionName == "set_allpig") {
		SpawnZombiePigAfterDie1(vendor);
	}
}
void ZombieMausoleumVendor::ModInit() {
	LOGI("ZombieVendor mod init");

	vftable = CreateChildVFTable(204 + 6, getActualOffset(0x241D430), 204);
	PatchVFTable(vftable, (void*)ZombieMausoleumVendor::StaticGetType, 0);
	PatchVFTable(vftable, (void*)MausoleumVendorOnSpawn, 49);
	PatchVFTable(vftable, (void*)MausoleumVendorOnAffectedByCondition, 71);
	PatchVFTable(vftable, (void*)MausoleumVendorWalkOnLoop, 124);
	PatchVFTable(vftable, (void*)MausoleumVendorActionFrame, 170);

	PatchVFTable(vftable, (void*)ZombieMausoleumVendor::PigOnEnter, 207);
	PatchVFTable(vftable, (void*)ZombieMausoleumVendor::PigOnLoop, 208);
	PatchVFTable(vftable, (void*)ZombieMausoleumVendor::PigOnExit, 209);

	ZombieMausoleumVendor::StaticGetType();
	LOGI("ZombieVendor finish init");
}

void ZombieMausoleumVendor::buildEventCallbacks(Reflection::CRefManualSymbolBuilder* builder, Reflection::RClass* rtClass)
{
	IF_CALLBACK_NOTSETUP(ZombieMausoleumVendor) {
		SetupLiteralDelegate(&setPigCompletedDelegate, MausoleumVendorCompletedCallback);
		ZombieMausoleumVendor_delegatesSetup = true;
		LOGI("SO TRUE");
	}
	RegisterEventCallback(builder, rtClass, "onSetPigCompleted", setPigCompletedDelegate);
	LOGI("Reg event complete");
}

void ZombieMausoleumVendor::buildStates()
{
	StateMachineTableBuilder* stateMachine = CallGetStateMachine(ZombieMausoleumVendor::StaticGetType());
	RegisterStateByOffsets(stateMachine,
		16,
		(uintptr_t)ZombieMausoleumVendor::PigOnEnter,
		(uintptr_t)ZombieMausoleumVendor::PigOnLoop,
		(uintptr_t)ZombieMausoleumVendor::PigOnExit,
		"ZS_Vendor_SetPig");
	LOGI("Reg state complete");
}



