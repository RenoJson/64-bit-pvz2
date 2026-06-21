#include "ZombieMausoleumPiggy.h"
#include "ZombieMausoleumPiggyProps.h"
#include "ZombieHelper.h"
#include "GridItemMausoleumLawnTile.h"
#include "TimeMgr.h"

void* ZombieMausoleumPiggy::vftable = nullptr;
Sexy::RtClass* ZombieMausoleumPiggy::s_rtClass = nullptr;;

void MausoleumPiggyLoopWalk(ZombieMausoleumPiggy* zombie)
{
    auto props = reinterpret_cast<ZombieMausoleumPiggyProps*>(zombie->m_propertySheet.Get());

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

    GridItemMausoleumLawnPath* activePathTile = nullptr;

    for (BoardEntity* ptr : entities) {
        if (ptr == nullptr) continue;

        if (ptr->IsType(GridItemMausoleumLawnPath::StaticGetType()))
        {
            GridItem* tile = static_cast<GridItem*>(ptr);

            if (tile->m_gridLocation.mX == gX && tile->m_gridLocation.mY == gY) {
                activePathTile = static_cast<GridItemMausoleumLawnPath*>(tile);
                break;
            }
        }
    }

    if (activePathTile != nullptr)
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
        float pixelCenterY = 192.0f + (gY * 76.0f);
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
                auto* pathProps = reinterpret_cast<GridItemMausoleumLawnPathProps*>(activePathTile->m_propertySheet.Get());
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

                    std::vector<int> turnChoices;
                    bool canKeepGoing = false;

                    for (int i = 0; i < numChoices; i++)
                    {
                        int checkDir = pathProps->DirectionType[i];

                        if (checkDir != oppositeDirection)
                        {
                            if (checkDir == zombie->m_currentDirection) {
                                canKeepGoing = true;
                            }
                            else {
                                turnChoices.push_back(checkDir);
                            }
                        }
                    }

                    int nextDirection = zombie->m_currentDirection;
                    bool hasTurnChoices = (turnChoices.size() > 0);

                    if (canKeepGoing && hasTurnChoices)
                    {
                        int roll = rand() % 100;

                        if (roll < 40) {
                            nextDirection = zombie->m_currentDirection;
                        }
                        else {
                            int randomIndex = rand() % turnChoices.size();
                            nextDirection = turnChoices[randomIndex];
                        }
                    }
                    else if (!canKeepGoing && hasTurnChoices)
                    {
                        int randomIndex = rand() % turnChoices.size();
                        nextDirection = turnChoices[randomIndex];
                    }
                    else if (canKeepGoing && !hasTurnChoices)
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

                    const float VISUAL_OFFSET_X = 0.0f;
                    const float VISUAL_OFFSET_Y = 32.0f;

                    if (nextDirection == 0 || nextDirection == 3) {
                        pY = pixelCenterY + VISUAL_OFFSET_Y;
                    }
                    else if (nextDirection == 1 || nextDirection == 2) {
                        pX = pixelCenterX + VISUAL_OFFSET_X;
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
        zombie->m_currentDirection = 0;
        ZombieAllowMovement(zombie, true);
        ZombieSetSpeedScale(zombie, 1.0f);
        CallFunc<void, Zombie*>(0xC506B4, zombie);
    }
}

void PiggyOnCreate(ZombieMausoleumPiggy* zombie) {
    zombie->m_currentDirection = 0;
    zombie->m_lastPathGridX = 0;
    zombie->m_lastPathGridY = 0;
}

SexyString GetPiggyShockEffectName()
{
	return "POPANIM_EFFECTS_ZOMBIE_PIG_SHOCK";
}
SexyString GetPiggyAshEffectName()
{
	return "POPANIM_EFFECTS_ZOMBIE_PIG_ASH";
}
bool PiggyIsShrunkenToObvillion() {
	return true;
}
void ZombieMausoleumPiggy::ModInit() {
	LOGI("ZombiePiggy mod init");

	vftable = CopyVFTable(getActualOffset(0x241D430), 204);
	PatchVFTable(vftable, (void*)ZombieMausoleumPiggy::StaticGetType, 0);
    PatchVFTable(vftable, (void*)MausoleumPiggyLoopWalk, 124);

    PatchVFTable(vftable, (void*)PiggyOnCreate, 169);

	PatchVFTable(vftable, (void*)GetPiggyShockEffectName, 189);
	PatchVFTable(vftable, (void*)GetPiggyAshEffectName, 190);
	PatchVFTable(vftable, (void*)PiggyIsShrunkenToObvillion, 200);
	ZombieMausoleumPiggy::StaticGetType();
	LOGI("ZombiePiggy finish init");
}

