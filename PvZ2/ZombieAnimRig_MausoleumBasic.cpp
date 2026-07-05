#include "ZombieAnimRig_Basic.h"
#include "ZombieHelper.h"

void* ZombieAnimRig_MausoleumBasic::vftable = __null;
Sexy::RtClass* ZombieAnimRig_MausoleumBasic::s_rtClass = __null;;


SexyString ParticleHead(ZombieAnimRig_MausoleumBasic* thisPtr) {
    if (thisPtr->m_hasCursed == true) {
        return "particle_head_cursed";
    }
    else {
        return "particle_head";
    }
}

std::vector<SexyString>* hkInitMausoleumBasicHeadList() {

    static std::vector<SexyString> MausoleumBasicHeadList = {
       "zombie_skull",
       "zombie_jaw",
       "zombie_eyes_curse"
    };
    return &MausoleumBasicHeadList;
}
SexyString BasicCursedDie(ZombieAnimRig_MausoleumBasic* thisPtr) {
    if (thisPtr->m_hasCursed == true) {
        return "die_reincarnation";
    }
    else {
        return "die";
    }
}

void ZombieAnimRig_MausoleumBasic::modInit() {
    LOGI("ZombieAnimRig_MausoleumBasicCursed init");

    vftable = CopyVFTable(getActualOffset(0x24BDA80), 85);

    PatchVFTable(vftable, (void*)ZombieAnimRig_MausoleumBasic::StaticGetType, 0);
    PatchVFTable(vftable, (void*)ParticleHead, 44);

    PatchVFTable(vftable, (void*)hkInitMausoleumBasicHeadList, 55);

    PatchVFTable(vftable, (void*)BasicCursedDie, 63);

    ZombieAnimRig_MausoleumBasic::StaticGetType();
    LOGI("ZombieAnimRig_MausoleumBasicCursed finish init");
}
