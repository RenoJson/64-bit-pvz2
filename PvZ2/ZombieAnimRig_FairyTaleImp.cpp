#include "ZombieAnimRig_Imp.h"

void* ZombieAnimRig_FairyTaleImp::vftable = __null;
Sexy::RtClass* ZombieAnimRig_FairyTaleImp::s_rtClass = __null;;


SexyString hkImpWalkAnim(ZombieAnimRig_FairyTaleImp* thisptr) {
    if (thisptr->m_jumpScared == true) {
        return "run";
    }
    else {
        return "walk";
    }
}

void ZombieAnimRig_FairyTaleImp::modInit() {
    LOGI("ZombieAnimRig_FairyTaleImp init");

    vftable = CopyVFTable(getActualOffset(0x23EF5D8), 90);

    PatchVFTable(vftable, (void*)ZombieAnimRig_FairyTaleImp::StaticGetType, 0);
    PatchVFTable(vftable, (void*)hkImpWalkAnim, 59);

    ZombieAnimRig_FairyTaleImp::StaticGetType();

    LOGI("ZombieAnimRig_FairyTaleImp finish init");
}