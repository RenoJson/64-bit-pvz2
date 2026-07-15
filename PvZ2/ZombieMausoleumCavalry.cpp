#pragma once
#include"Zombie.h",
#include "ZombieHelper.h"
#include "GridItem.h"
#include "TimeMgr.h"
#include <cmath>
#include "Board.h"
#include "ZombieMausoleumCavalry.h"
#include "ZombieMausoleumCavalryProps.h"
#include "GridItemMausoleumLawnTile.h"

void* ZombieMausoleumCavalry::vftable = __null;
Sexy::RtClass* ZombieMausoleumCavalry::s_rtClass = __null;;

void MausoleumCavalryLoopWalk(ZombieMausoleumCavalry* zombie)
{
    auto props = reinterpret_cast<ZombieMausoleumCavalryProps*>(zombie->m_propertySheet.Get());

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

    bool isCharmed = ZombieHasCondition(zombie, zombie_condition_hypnotized);
    int homeDir = isCharmed ? 3 : 0; 
    int backDir = isCharmed ? 0 : 3;

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

                            if (checkDir == homeDir) {
                                routeLeadsToHouse = true;
                            }
                            else if (checkDir == backDir && zombie->m_currentDirection != backDir) {
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
                                    bool foundHome = false;
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
                                        if (d == homeDir) foundHome = true;
                                        if (d == simDir) canGoStraight = true;
                                        if (d != oppSimDir) turnDir = d;
                                    }

                                    if (foundHome) {
                                        routeLeadsToHouse = true;
                                        break;
                                    }

                                    if (!canGoStraight) {
                                        if (turnDir == -1 || turnDir == backDir) break;
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
                        nextDirection = homeDir;
                    }

                    if (gY >= 4 && nextDirection == 1)      nextDirection = homeDir;
                    else if (gY <= 0 && nextDirection == 2) nextDirection = homeDir;
                    else if (!isCharmed && gX >= 8 && nextDirection == 3) nextDirection = 0;
                    else if (isCharmed && gX <= 0 && nextDirection == 0) nextDirection = 3;

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
        CallFunc<void, Zombie*>(0xC506B4, zombie);
    }
    else
    {
        zombie->m_currentDirection = homeDir;
        ZombieAllowMovement(zombie, true);
        ZombieSetSpeedScale(zombie, 1.0f);
        CallFunc<void, Zombie*>(0xC506B4, zombie);
    }
}

void CavalryOnCreate(ZombieMausoleumCavalry* zombie) {
    zombie->m_currentDirection = 0;
    zombie->m_lastPathGridX = 0;
    zombie->m_lastPathGridY = 0;
}
void ZombieMausoleumCavalry::ModInit() {
    LOGI("ZombieMausoleumCavalry init");

    vftable = CopyVFTable(getActualOffset(0x241D430), 204);

    PatchVFTable(vftable, (void*)ZombieMausoleumCavalry::StaticGetType, 0);

    PatchVFTable(vftable, (void*)MausoleumCavalryLoopWalk, 124);

    PatchVFTable(vftable, (void*)CavalryOnCreate, 169);

    ZombieMausoleumCavalry::StaticGetType();

    LOGI("ZombieMausoleumCavalry finish init");
}