#pragma once
#include"Zombie.h",
#include "ZombieHelper.h"
#include "GridItem.h"
#include "TimeMgr.h"
#include <cmath>
#include "Board.h"
#include "ZombieMausoleumVendor.h"

void MausoleumVendorLoopWalk(ZombieMausoleumVendor* zombie)
{
    ZombieAllowMovement(zombie, false);

    float pX = zombie->m_position.x;
    float pY = zombie->m_position.y;

    int gX = static_cast<int>((pX - 200.0f) / 64.0f);
    int gY = static_cast<int>((pY - 160.0f) / 76.0f);

    GridItem* pathTile = nullptr;

    if (pathTile != nullptr)
    {
        ZombieConditionTracker* zTracker = &zombie->m_conditionTracker;
        uint8_t* cond = zTracker->m_states.data();
        if (cond != nullptr && *cond != 0) {
            typedef void (*UpdateConditionsFunc)(ZombieConditionTracker*);
            ((UpdateConditionsFunc)(*(void***)zTracker)[3])(zTracker);
            *cond = 0;
        }

        float baseSpeed = 0.5f;
        float speedScale = zTracker->m_speedScale;
        float floatingSpeed = baseSpeed * speedScale * 64.0f;
        float timeMoving = TimeMgr::GetInstance()->m_unkTime;
        float stepDist = floatingSpeed * timeMoving;

        float pixelCenterX = 232.0f + (gX * 64.0f);
        float pixelCenterY = 192.0f + (gY * 76.0f);
        float distToCenter = std::sqrt(std::pow(pX - pixelCenterX, 2) + std::pow(pY - pixelCenterY, 2));

        if (distToCenter <= stepDist)
        {
            pX = pixelCenterX;
            pY = pixelCenterY; 

            if (gX != zombie->m_lastPathGridX || gY != zombie->m_lastPathGridY)
            {
                auto* pathProps = reinterpret_cast<GridItemPropertySheet*>(pathTile->m_propertySheet.Get());
                //int numChoices = pathProps->Directions.size();
                int numChoices = 0;
                if (numChoices > 0)
                {
                    int oppositeDirection = -1;
                    switch (zombie->m_currentDirection) {
                    case 0: oppositeDirection = 3; break; 
                    case 1: oppositeDirection = 2; break; 
                    case 2: oppositeDirection = 1; break; 
                    case 3: oppositeDirection = 0; break; 
                    }

                    std::vector<int> validChoices;
                    bool canKeepGoing = false;

                    for (int i = 0; i < numChoices; i++)
                    {
                        //int checkDir = pathProps->Directions[i];
                        int checkDir = 1;
                        if (checkDir == zombie->m_currentDirection) {
                            canKeepGoing = true;
                        }

                        if (checkDir != oppositeDirection) {
                            validChoices.push_back(checkDir);
                        }
                    }

                    int nextDirection = zombie->m_currentDirection;

                    if (!canKeepGoing)
                    {
                        if (validChoices.size() > 0)
                        {
                            int randomIndex = rand() % validChoices.size();
                            nextDirection = validChoices[randomIndex];
                        }
                        else
                        {
                            nextDirection = 0;
                        }
                    }

                    if (gY >= 4 && nextDirection == 1)      nextDirection = 0;
                    else if (gY <= 0 && nextDirection == 2) nextDirection = 0;
                    else if (gX >= 8 && nextDirection == 3) nextDirection = 0;

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
        return;
    }
    else
    {
        zombie->m_currentDirection = 0;    
        ZombieAllowMovement(zombie, true);

        CallFunc<void, Zombie*>(0xC506B4, zombie);
    }
}