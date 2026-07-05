#include "ZombieAnimRig_MausoleumArcher.h"

void* ZombieAnimRig_MausoleumArcher::vftable = __null;
Sexy::RtClass* ZombieAnimRig_MausoleumArcher::s_rtClass = __null;;

SexyString ParticleHead1(ZombieAnimRig_MausoleumBasic* thisPtr) {
    if (thisPtr->m_hasCursed == true) {
        return "particle_head_cursed";
    }
    else {
        return "particle_head";
    }
}
std::vector<SexyString>* hkInitMausoleumArcherLowerArmList() {

    static std::vector<SexyString> MausoleumArcherLowerArmList = {
       "zombie_arm_outer_lower",
       "zombie_hand_outer_01",
       "zombie_hand_outer_02",
       "zombie_hand_outer_03",
       "zombie_arrow"
    };
    return &MausoleumArcherLowerArmList;
}
std::vector<SexyString>* hkInitMausoleumArcherUpperArmList() {

    static std::vector<SexyString> MausoleumArcherUpperArmList = {
        "zombie_arm_outer_upper",
        "zombie_arm_outer_upper_bone"
    };
    return &MausoleumArcherUpperArmList;
}
std::vector<SexyString>* hkInitMausoleumArcherHeadList() {

    static std::vector<SexyString> MausoleumArcherHeadList = {
       "zombie_skull",
       "zombie_jaw",
       "zombie_eyes_curse"
    };
    return &MausoleumArcherHeadList;
}
SexyString hkIdleAnim(ZombieAnimRig_MausoleumArcher* thisptr) {
    return "idle";
}
SexyString hkWalkAnim(ZombieAnimRig_MausoleumArcher* thisptr) {
    return "walk";
}
SexyString hkEatAnim(ZombieAnimRig_MausoleumArcher* thisptr) {
    return "eat";
   
}
SexyString hkDieAnim1(ZombieAnimRig_MausoleumBasic* thisPtr) {
    if (thisPtr->m_hasCursed == true) {
        return "die_reincarnation";
    }
    else {
        return "die";
    }
}

void ZombieAnimRig_MausoleumArcher::modInit() {
    LOGI("ZombieAnimRig_MausoleumArcher init");

    vftable = CopyVFTable(getActualOffset(0x24BDA80), 85);

    PatchVFTable(vftable, (std::vector<SexyString>*)ZombieAnimRig_MausoleumArcher::StaticGetType, 0);
    PatchVFTable(vftable, (void*)ParticleHead1, 44);
    PatchVFTable(vftable, (std::vector<SexyString>*)hkInitMausoleumArcherHeadList, 55);
    PatchVFTable(vftable, (std::vector<SexyString>*)hkInitMausoleumArcherLowerArmList, 56);
    PatchVFTable(vftable, (std::vector<SexyString>*)hkInitMausoleumArcherUpperArmList, 57);
    PatchVFTable(vftable, (std::vector<SexyString>*)hkIdleAnim, 58);
    PatchVFTable(vftable, (std::vector<SexyString>*)hkWalkAnim, 59);
    PatchVFTable(vftable, (std::vector<SexyString>*)hkEatAnim, 61);
    PatchVFTable(vftable, (std::vector<SexyString>*)hkDieAnim1, 63);

    ZombieAnimRig_MausoleumArcher::StaticGetType();

    LOGI("ZombieAnimRig_MausoleumArcher finish init");
}
