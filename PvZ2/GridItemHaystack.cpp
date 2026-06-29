#include "GridItemHaystack.h"

void* GridItemHaystack::vftable = __null;
void* GridItemHaystack::vftable1 = __null;
Sexy::RtClass* GridItemHaystack::s_rtClass = __null;;

uint SodGetRenderOrder(GridItemHaystack* tile) {
    return 400009;
}
void SodGridEventRegister(GridItemHaystack* tile) {
    CallFunc<void, GridItemHaystack*>(0xDEEE9C, tile);
}
void SodCanPlantGoHere(GridItemHaystack* tile, const SexyString& plantName, std::vector<int>* list) {
    int targetID = -1;
    if (plantName == "sod") {
        targetID = 35;
    }
    else {
        auto props = reinterpret_cast<GridItemLilyPadProps*>(tile->m_propertySheet.Get());
        bool isInList = CallFunc<bool, PlantRestrictionSet*, const SexyString&>(0x1363EE8, &props->PlantsWhichCannotBePlantedOnLilypads, plantName);
        if (!isInList) {
            return;
        }
        targetID = 9;
    }
    list->push_back(targetID);
}

void GridItemHaystack::modInit() {
    LOGI("GridItemHaystack init");

    vftable = CopyVFTable(getActualOffset(0x2396530), 68);
    vftable1 = CopyVFTable(getActualOffset(0x2396760), 4);
    PatchVFTable(vftable, (void*)GridItemHaystack::StaticGetType, 0);
    PatchVFTable(vftable1, (void*)SodGetRenderOrder, 3);
    PatchVFTable(vftable, (void*)SodGridEventRegister, 7);
    PatchVFTable(vftable, (void*)SodCanPlantGoHere, 55);
    GridItemHaystack::StaticGetType();
    LOGI("GridItemHaystack finish init");
}


