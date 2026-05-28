#include "ZombieAnimRig_ModernPoleVaulter.h"

void* ZombieAnimRig_ModernPoleVaulter::vftable = __null;
Sexy::RtClass* ZombieAnimRig_ModernPoleVaulter::s_rtClass = __null;;


void* hkInitModernPoleVaulterLowerArmList() {

    static std::vector<SexyString> ModernPoleVaulterLowerArmList = {
       "zombie_arm_outer_lower",
       "zombie_hand_outer_01",
       "zombie_hand_outer_02",
       "zombie_hand_outer_03",
       "zombie_hand_outer_04"
    };
    return &ModernPoleVaulterLowerArmList;
}
void* hkInitModernPoleVaulterUpperArmList() {

    static std::vector<SexyString> ModernPoleVaulterUpperArmList = {
        "zombie_arm_outer_upper",
        "zombie_arm_outer_upper_bone"
    };
    return &ModernPoleVaulterUpperArmList;

}
void* hkInitModernPoleVaulterHeadList() {

    static std::vector<SexyString> ModernPoleVaulterHeadList = {
       "zombie_skull",
       "zombie_jaw"
    };
    return &ModernPoleVaulterHeadList;
}
SexyString hkIdleAnim(ZombieAnimRig_ModernPoleVaulter* thisptr) {
    return "idle_pole";
}
SexyString hkWalkAnim(ZombieAnimRig_ModernPoleVaulter* thisptr) {
    if (thisptr->m_hasPole == true) {
        return "run";
    }
    else {
        return "walk";
    }
}
SexyString hkEatAnim(ZombieAnimRig_ModernPoleVaulter* thisptr) {
    if (thisptr->m_hasPole == true) {
        return "eat_pole";
    }
    else {
        return "eat";
    }
}
SexyString hkDieAnim(ZombieAnimRig_ModernPoleVaulter* thisptr) {
    if (thisptr->m_hasPole == true) {
        return "die_pole";
    }
    else {
        return "die";
    }
}

void ZombieAnimRig_ModernPoleVaulter::modInit() {
    LOGI("ZombieAnimRig_ModernPoleVaulter init");

    vftable = CopyVFTable(getActualOffset(0x23ABF70), 67);

    PatchVFTable(vftable, (void*)ZombieAnimRig_ModernPoleVaulter::StaticGetType, 0);
    PatchVFTable(vftable, (void*)hkInitModernPoleVaulterHeadList, 55);
    PatchVFTable(vftable, (void*)hkInitModernPoleVaulterLowerArmList, 56);
    PatchVFTable(vftable, (void*)hkInitModernPoleVaulterUpperArmList, 57);
    PatchVFTable(vftable, (void*)hkIdleAnim, 58);
    PatchVFTable(vftable, (void*)hkWalkAnim, 59);
    PatchVFTable(vftable, (void*)hkEatAnim, 61);
    PatchVFTable(vftable, (void*)hkDieAnim, 63);

    ZombieAnimRig_ModernPoleVaulter::StaticGetType();

    LOGI("ZombieAnimRig_ModernPoleVaulter finish init");
}
