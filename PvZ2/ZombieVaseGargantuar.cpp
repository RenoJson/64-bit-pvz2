#include "ZombieGargantuar.h"
#include "GridItem.h"
#include "ZombieHelper.h"

void* ZombieVaseGargantuar::vftable = __null;
Sexy::RtClass* ZombieVaseGargantuar::s_rtClass = __null;;

typedef BoardEntity* (*getTarg)(ZombieVaseGargantuar*);

void VaseWalkOnLoop(ZombieVaseGargantuar* zombie) {
    bool foundVaseToSmash = false;
    int gX = static_cast<int>((zombie->m_position.x - 200.0f) / 64.0f);
    int gY = static_cast<int>((zombie->m_position.y - 160.0f) / 76.0f);

    if (gY > 4) gY = 4;
    if (gY < 0) gY = 0;

    Rect scanRect;
    scanRect.mX = gX; 
    scanRect.mY = gY;     
    scanRect.mWidth = 1;  
    scanRect.mHeight = 1; 

    std::vector<BoardEntity*> entityList;
    GetEntitiesInRectGrid(&entityList, 63, &scanRect);

    for (BoardEntity* ptr : entityList) {
        if (ptr == nullptr) continue;

        if (ptr->IsType(GridItemVase::StaticGetType())) {
            auto* vase = reinterpret_cast<GridItemVase*>(ptr);

            if ((vase->m_flags & 4) == 0) {
                foundVaseToSmash = true;
                break; 
            }
        }
    }

    if (foundVaseToSmash) {
        ZombieEnterState(zombie, 16, 0); 
    }
    else {
        CallFunc<void, ZombieVaseGargantuar*>(0xB4C934, zombie);
    }
}
void hkVaseGargantuarActionFrame(ZombieVaseGargantuar* zombie, int64_t unk1, SexyString* actionName, int64_t unk2, SexyString* currentAnim)
{
	typedef void* (*gargAF)(ZombieVaseGargantuar*, int64_t, SexyString*, int64_t, SexyString*);
	gargAF gargActionFrame = (gargAF)getActualOffset(0xB30CC0);

    typedef void* (*Func_VaseBreak)(GridItemVase*);
    Func_VaseBreak breakVase = (Func_VaseBreak)getActualOffset(0xA322B8);


	if (*actionName == "smash_vase")
	{
        int gX = static_cast<int>((zombie->m_position.x - 200.0f) / 64.0f);
        int gY = static_cast<int>((zombie->m_position.y - 160.0f) / 76.0f);

        if (gY > 4) gY = 4;
        if (gY < 0) gY = 0;

        Rect smashRect;
        smashRect.mX = gX;
        smashRect.mY = gY;
        smashRect.mWidth = 1;
        smashRect.mHeight = 1;

        std::vector<BoardEntity*> entityList;
        GetEntitiesInRectGrid(&entityList, 63, &smashRect);

        for (BoardEntity* ptr : entityList) {
            if (ptr == nullptr) continue;

            if (ptr->IsType(GridItemVase::StaticGetType())) {
                auto vase = reinterpret_cast<GridItemVase*>(ptr);
                if ((vase->m_flags & 4) == 0) {
                    breakVase(vase);
                }
            }
        }
		return;
	}
	gargActionFrame(zombie, unk1, actionName, unk2, currentAnim);
}
void ZombieVaseGargantuar::modInit() {
    LOGI("ZombieVaseGargantuar init");

    vftable = CopyVFTable(getActualOffset(0x23E5478), 212);

    PatchVFTable(vftable, (void*)ZombieVaseGargantuar::StaticGetType, 0);

    PatchVFTable(vftable, (void*)VaseWalkOnLoop, 124);

    PatchVFTable(vftable, (void*)hkVaseGargantuarActionFrame, 170);

    ZombieVaseGargantuar::StaticGetType();

    LOGI("ZombieVaseGargantuar finish init");
}