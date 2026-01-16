#include "PvZ2/MeteorProjectile.h"
#include <PvZ2/GridItem.h>

void* MeteorProjectile::vftable = nullptr;
Sexy::RtClass* MeteorProjectile::s_rtClass = nullptr;

#pragma region hk Meteor Proj

// the anim and sound code can be used in ImpactPAM and ImpactSoundEvent, i don't need to recreate any of them
uintptr_t hkSpawnLava(MeteorProjectile* self)
{
    typedef int (*board1)();
    int board = ((board1)getActualOffset(0x949EFC))();

    typedef GridItem* (*func716690)(int, SexyString, int, int);
    func716690 func_716690 = (func716690)getActualOffset(0x716690);

    float posX = self->m_position.mX;
    float posY = self->m_position.mY;

    int gridPosY = (((int)posY - 540) / 76) + 4;
    int gridPosX = (int)((posX - 200.0f) / 64.0f);


    SexyString gridType = "molten_pool";

    typedef bool (*func716FA4)(int, uint, uint, int, int);
    auto* fun716FA4 = ((func716FA4)getActualOffset(0x716FA4));

    if (fun716FA4(board, gridPosX, gridPosY, 0, 1) == true)
    {
        GridItem* gridItem = func_716690(board, gridType, gridPosX, gridPosY);

        typedef void (*funcCEB640)(GridItem*, int);
        auto* setTeamFlag = ((funcCEB640)getActualOffset(0xCEB640));
        setTeamFlag(gridItem, 1);

    }
    else
    {
        LOGI("Error: Grid position invalid. Skipping grid spawn.");
    }
    typedef int (*VirtualFunc41)(void*, int);
    void** vtable = *(void***)self;
    VirtualFunc41 func = (VirtualFunc41)vtable[41];
    int result = func(self, 0);
    return result;
}
void MeteorProjectile::modInit() {
    LOGI("MeteorProjectile init");

    vftable = copyVFTable(getActualOffset(0x1CB00D8), 53);

    PatchVFTable(vftable, (void*)MeteorProjectile::StaticGetType, 0);

    PatchVFTable(vftable, (void*)hkSpawnLava, 36);

    MeteorProjectile::StaticGetType();

    LOGI("MeteorProjectile finish init");
}