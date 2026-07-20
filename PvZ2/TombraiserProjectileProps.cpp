#include "TombraiserProjectileProps.h"

#include <PvZ2/Effect_PopAnim.h>
#include <PvZ2/GridItem.h>
#include <PvZ2/Board.h>
#include <PvZ2/PVZDB.h>
#include <PvZ2/AudioMgr.h>
#include <PvZ2/Projectile.h>
#include <Sexy/RtWeakPtr.h>
#include <Sexy/ResourceManager.h>
#include "ZombieTombRaiser.h"
#include "AddGridItemType.h"


DEFINE_STATIC_RT_CLASS_MEMBERS(TombraiserProjectileProps);

bool hkTombraiserProjectileOnCollision(TombraiserProjectile* thisPtr)
{
    auto props = reinterpret_cast<TombraiserProjectileProps*>(thisPtr->m_propertySheet.Get());

    int mX = thisPtr->m_targetGridLoc.mX;
    int mY = thisPtr->m_targetGridLoc.mY;

    if (CanSpawnGridItemAt(mX, mY, false, true))
    {
        auto gridItem = AddGridItem(props->GridItemType, mX, mY);
        Board* boardPtr = Board::GetBoard();
        auto type = Effect_PopAnim::StaticGetType();
        auto popAnim = CallFunc<Effect_PopAnim*, Board*, Sexy::RtClass*>(0xAA1EFC, boardPtr, type);
        popAnim->SetFromResource(props->PopAnimEffect);
        popAnim->SetFocusFracAndScale(1);

        Sexy::SexyVector3 pos = Sexy::SexyVector3((float)(mX * GRID_TILE_WIDTH + 232), (float)(mY * GRID_TILE_HEIGHT + 192), 0.0f);
        popAnim->SetPositionAndLayer(pos, gridItem->GetRenderOrder() + 2);
        popAnim->PlayAnimation(props->AnimationName, 0);

        AudioMgr::GetInstance()->PostEvent(props->EffectEventName);
    }
    typedef bool (*doImpact)(TombraiserProjectile*, bool);
    doImpact DoImpact = ((doImpact)getActualOffset(0xCA065C));
    return DoImpact(thisPtr, false);
}


void TombraiserProjectileProps::modInit()
{
    LOGI("[TombraiserProjectileProps::ModInit]");
    PVZ2HookFunction(0xB22EF8, (void*)hkTombraiserProjectileOnCollision, nullptr);
    vftable = CopyVFTable(getActualOffset(0x23848F0), 14);
    PatchVFTable(vftable, (void*)TombraiserProjectileProps::StaticGetType, 0);
    TombraiserProjectileProps::StaticGetType();
}