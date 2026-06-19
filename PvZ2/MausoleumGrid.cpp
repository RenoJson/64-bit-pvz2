#include "GridItemMausoleumLawnTile.h"
#include "Sexy/LawnApp.h"

Sexy::RtWeakPtr<Image> GetImageResPtr(SexyString imageName) {
    Sexy::RtWeakPtr<Image> imagePtr;

    Sexy::ResourceManager* res = LawnApp::GetInstance()->m_resourceManager;
    Sexy::RtClass* resInfoType = ResourceInfoType__ImageRes::StaticGetType();

    uintptr_t resNode = reinterpret_cast<uintptr_t>(
        CallFunc<void*, Sexy::ResourceManager*, Sexy::RtClass*, SexyString*>(0x1644E7C, res, resInfoType, &imageName)
        );

    if (resNode != 0) {
        Sexy::RtWeakPtr<Image>* internalWeakPtr = reinterpret_cast<Sexy::RtWeakPtr<Image>*>(resNode + 32);
        imagePtr.FromOther(internalWeakPtr);
    }

    return imagePtr;
}

void TileRender(GridItemMausoleumLawnTile* tile, Graphics* g) {
    if (tile->m_imageRes == nullptr) {
        auto props = reinterpret_cast<GridItemMausoleumLawnTileProps*>(tile->m_propertySheet.Get());
        tile->m_imageRes = GetImageResPtr(props->Image);
    }
    int gridX = tile->m_gridLocation.mX;
    int gridY = tile->m_gridLocation.mY;
    float mY = 160 + (gridY * 76.0f);
    float mX = 200 + (gridX * 64.0f);
    auto sexyApp = SexyApp::GetInstance();
    float renderX = sexyApp->ScaleArt(mX);
    float renderY = sexyApp->ScaleArt(mY);
    auto imgToRender = reinterpret_cast<Image*>(tile->m_imageRes.Get());
    CallFunc<void, Graphics*, Image*, int, int>(0x154FEC4, g, imgToRender, (int)renderX, (int)renderY);
}