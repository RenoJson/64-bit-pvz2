#include "ZombieAnimRig_MausoleumCursedBase.h"

void* ZombieAnimRig_MausoleumGargantuar::vftable = __null;
Sexy::RtClass* ZombieAnimRig_MausoleumGargantuar::s_rtClass = __null;;


SexyString ParticleHead(ZombieAnimRig_MausoleumGargantuar* thisPtr) {
    if (thisPtr->m_hasCursed == true) {
        return "Gargantuar_Head_Particle_Cursed";
    }
    else {
        return "Gargantuar_Head_Particle";
    }
}
std::vector<SexyString>* MausoleumGargantuarHeadList() {

    static std::vector<SexyString> gargantuarHeadList = {
        "Zombie_gargantuar_head",
        "Zombie_gargantuar_jaw",
        "Zombie_gargantuar_head_back"
    };
    return &gargantuarHeadList;
}
SexyString hkDieAnim7(ZombieAnimRig_MausoleumBasic* thisPtr) {
    if (thisPtr->m_hasCursed == true) {
        return "die_reincarnation";
    }
    else {
        return "die";
    }
}


void ZombieAnimRig_MausoleumGargantuar::modInit() {
    LOGI("ZombieAnimRig_MausoleumGargantuar init");

    vftable = CopyVFTable(getActualOffset(0x24BDA80), 85);

    PatchVFTable(vftable, (void*)ZombieAnimRig_MausoleumGargantuar::StaticGetType, 0);
    PatchVFTable(vftable, (void*)ParticleHead, 44);
    PatchVFTable(vftable, (std::vector<SexyString>*)MausoleumGargantuarHeadList, 55);

    PatchVFTable(vftable, (std::vector<SexyString>*)hkDieAnim7, 63);

    ZombieAnimRig_MausoleumGargantuar::StaticGetType();
    LOGI("ZombieAnimRig_MausoleumGargantuar finish init");
}