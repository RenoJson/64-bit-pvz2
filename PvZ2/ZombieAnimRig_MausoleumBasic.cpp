#include "ZombieAnimRig_Basic.h"
#include "ZombieHelper.h"

void* ZombieAnimRig_MausoleumBasic::vftable = __null;
Sexy::RtClass* ZombieAnimRig_MausoleumBasic::s_rtClass = __null;;

void* ZombieAnimRig_MausoleumBasicCursed::vftable = __null;
Sexy::RtClass* ZombieAnimRig_MausoleumBasicCursed::s_rtClass = __null;;

void CreateBasicAnimRig(ZombieAnimRig_MausoleumBasic* thisPtr)
{
    CallFunc<void, ZombieAnimRig_MausoleumBasic*>(0x8DE104, thisPtr);
    SetAnimLayerVisible(thisPtr, "zombie_eyes_curse", false);
}

std::vector<SexyString>* hkInitMausoleumBasicHeadList() {

    static std::vector<SexyString> MausoleumBasicHeadList = {
       "zombie_skull",
       "zombie_jaw",
       "zombie_eyes_curse"
    };
    return &MausoleumBasicHeadList;
}
SexyString BasicCursedDie(ZombieAnimRig_MausoleumBasicCursed* thisPtr) {
    if (thisPtr->m_hasCursed == true) {
        return "die_reincarnation";
    }
    else {
        return "die";
    }
}


void ZombieAnimRig_MausoleumBasicCursed::modInit() {
    LOGI("ZombieAnimRig_MausoleumBasicCursed init");

    vftable = CopyVFTable(getActualOffset(0x24BDA80), 85);

    PatchVFTable(vftable, (void*)ZombieAnimRig_MausoleumBasicCursed::StaticGetType, 0);
    PatchVFTable(vftable, (void*)hkInitMausoleumBasicHeadList, 55);

    PatchVFTable(vftable, (void*)BasicCursedDie, 63);

    ZombieAnimRig_MausoleumBasicCursed::StaticGetType();
    LOGI("ZombieAnimRig_MausoleumBasicCursed finish init");
}

void ZombieAnimRig_MausoleumBasic::modInit() {
    LOGI("ZombieAnimRig_MausoleumBasicCursed init");

    vftable = CopyVFTable(getActualOffset(0x24BDA80), 85);

    PatchVFTable(vftable, (void*)ZombieAnimRig_MausoleumBasic::StaticGetType, 0);

    PatchVFTable(vftable, (void*)CreateBasicAnimRig, 21);
    PatchVFTable(vftable, (void*)hkInitMausoleumBasicHeadList, 55);

    PatchVFTable(vftable, (void*)BasicCursedDie, 63);

    ZombieAnimRig_MausoleumBasic::StaticGetType();
    LOGI("ZombieAnimRig_MausoleumBasicCursed finish init");
}
