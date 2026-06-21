#include "GridItemMausoleumLawnTile.h"
#include "Sexy/LawnApp.h"
#include "Board.h"

void* GridItemMausoleumLawnTile::vftable = __null;
void* GridItemMausoleumLawnTile::vftable1 = __null;
Sexy::RtClass* GridItemMausoleumLawnTile::s_rtClass = __null;;

void* GridItemMausoleumLawnTileProps::vftable = __null;
Sexy::RtClass* GridItemMausoleumLawnTileProps::s_rtClass = __null;;

void* GridItemMausoleumLawnPath::vftable = __null;
void* GridItemMausoleumLawnPath::vftable1 = __null;
Sexy::RtClass* GridItemMausoleumLawnPath::s_rtClass = __null;;

void* GridItemMausoleumLawnPathProps::vftable = __null;
Sexy::RtClass* GridItemMausoleumLawnPathProps::s_rtClass = __null;;

void GetImageResPtr(Sexy::RtWeakPtr<Image>*imgPtr, SexyString imageName) {

    Sexy::ResourceManager* res = LawnApp::GetInstance()->m_resourceManager;
    Sexy::RtClass* resInfoType = ResourceInfoType__ImageRes::StaticGetType();

    uintptr_t resNode = reinterpret_cast<uintptr_t>(
        CallFunc<void*, Sexy::ResourceManager*, Sexy::RtClass*, SexyString*>(0x1644E7C, res, resInfoType, &imageName)
        );

    if (resNode != 0) {
        auto internalWeakPtr = reinterpret_cast<Sexy::RtWeakPtr<Image>*>(resNode + 32);
        imgPtr->FromOther(internalWeakPtr); 
    }
}

void TileRender(GridItemMausoleumLawnTile * tile, Graphics * g) {
    int gridX = tile->m_gridLocation.mX;
    int gridY = tile->m_gridLocation.mY;
    
    CallFunc<void, Graphics*>(0x154E9AC, g); // Push State

    if (tile->m_imageRes.Get() == nullptr) {
        auto props = reinterpret_cast<GridItemMausoleumLawnTileProps*>(tile->m_propertySheet.Get());
        GetImageResPtr(&tile->m_imageRes, props->Image);
    }
    auto imgToRender = reinterpret_cast<Image*>(tile->m_imageRes.Get());
    float mY = 160 + (gridY * 76.0f);
    float mX = 200 + (gridX * 64.0f);
    auto sexyApp = SexyApp::GetInstance();
    float renderX = sexyApp->ScaleArt(mX);
    float renderY = sexyApp->ScaleArt(mY);

    CallFunc<void, Graphics*, Image*, int, int>(0x154FEC4, g, imgToRender, (int)renderX, (int)renderY);
    CallFunc<void, Graphics*>(0x154EB48, g); // Pop State
}
uint TileGetRenderOrder(GridItemMausoleumLawnTile* tile) {
    return 200000;
}
uint PathGetRenderOrder(GridItemMausoleumLawnPath* tile) {
    return 200000;
}

void PathRender(GridItemMausoleumLawnPath * path, Graphics * g) {
    int gridX = path->m_gridLocation.mX;
    int gridY = path->m_gridLocation.mY;

    CallFunc<void, Graphics*>(0x154E9AC, g);

    if (path->m_imageRes.Get() == nullptr) {
        auto props = reinterpret_cast<GridItemMausoleumLawnPathProps*>(path->m_propertySheet.Get());
        GetImageResPtr(&path->m_imageRes, props->Image);
    }

    auto imgToRender = reinterpret_cast<Image*>(path->m_imageRes.Get());
    float mY = 160 + (gridY * 76.0f);
    float mX = 200 + (gridX * 64.0f);
    auto sexyApp = SexyApp::GetInstance();
    float renderX = sexyApp->ScaleArt(mX);
    float renderY = sexyApp->ScaleArt(mY);

    CallFunc<void, Graphics*, Image*, int, int>(0x154FEC4, g, imgToRender, (int)renderX, (int)renderY);
    CallFunc<void, Graphics*>(0x154EB48, g);
}


void GridItemMausoleumLawnTile::modInit() {
    LOGI("GridItemMausoleumLawnTile init");

    vftable = CopyVFTable(getActualOffset(0x23A0BC0), 64);
    vftable1 = CopyVFTable(getActualOffset(0x23A0DD0), 4);
    PatchVFTable(vftable, (void*)GridItemMausoleumLawnTile::StaticGetType, 0);
    PatchVFTable(vftable1, (void*)TileGetRenderOrder, 3);
    PatchVFTable(vftable, (void*)TileRender, 27);
    GridItemMausoleumLawnTile::StaticGetType();
    LOGI("GridItemMausoleumLawnTile finish init");
}

void GridItemMausoleumLawnPath::modInit() {
    LOGI("GridItemMausoleumLawnPath init");

    vftable = CopyVFTable(getActualOffset(0x23A0BC0), 64);
    vftable1 = CopyVFTable(getActualOffset(0x23A0DD0), 4);
    PatchVFTable(vftable, (void*)GridItemMausoleumLawnPath::StaticGetType, 0);
    PatchVFTable(vftable1, (void*)PathGetRenderOrder, 3);
    PatchVFTable(vftable, (void*)PathRender, 27);
    GridItemMausoleumLawnPath::StaticGetType();
    LOGI("GridItemMausoleumLawnPath finish init");
}

void GridItemMausoleumLawnTileProps::modInit() {
    LOGI("GridItemMausoleumLawnTileProps init");

    vftable = CopyVFTable(getActualOffset(0x24B55A0), 14);

    PatchVFTable(vftable, (void*)GridItemMausoleumLawnTileProps::StaticGetType, 0);

    GridItemMausoleumLawnTileProps::StaticGetType();
    LOGI("GridItemMausoleumLawnTileProps finish init");
}

void GridItemMausoleumLawnPathProps::modInit() {
    LOGI("GridItemMausoleumLawnPathProps init");

    vftable = CopyVFTable(getActualOffset(0x24B55A0), 14);

    PatchVFTable(vftable, (void*)GridItemMausoleumLawnPathProps::StaticGetType, 0);

    GridItemMausoleumLawnPathProps::StaticGetType();
    LOGI("GridItemMausoleumLawnPathProps finish init");
}