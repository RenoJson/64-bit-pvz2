#include "ZombieAnimRig_ModernPogo.h"

void* ZombieAnimRig_ModernPogo::vftable = __null;
Sexy::RtClass* ZombieAnimRig_ModernPogo::s_rtClass = __null;;


void* hkInitModernPogoLowerArmList() {

    static std::vector<SexyString> ModernPogoLowerArmList = {
       "zombie_arm_outer_lower",
       "zombie_hand_outer_01",
       "zombie_hand_outer_02",
       "zombie_hand_outer_03",
       "zombie_hand_outer_04"
    };
    return &ModernPogoLowerArmList;
}
void* hkInitModernPogoUpperArmList() {

    static std::vector<SexyString> ModernPogoUpperArmList = {
        "zombie_arm_outer_upper",
        "zombie_arm_outer_upper_bone"
    };
    return &ModernPogoUpperArmList;

}
void* hkInitModernPogoHeadList() {

    static std::vector<SexyString> ModernPogoHeadList = {
       "zombie_skull",
       "zombie_jaw",
       "zombie_pogo_glasses"
    };
    return &ModernPogoHeadList;
}
SexyString hkIdleAnim(ZombieAnimRig_ModernPogo* thisptr) {
    if (thisptr->m_hasPogo == true) {
        return "idle";
    }
    else {
        return "idle_single";
    }
}
SexyString hkWalkAnim(ZombieAnimRig_ModernPogo* thisptr) {
    if (thisptr->m_hasPogo == true) {
        return "walk_pogo";
    }
    else {
        return "walk";
    }
}
SexyString hkDieAnim(ZombieAnimRig_ModernPogo* thisptr) {
    if (thisptr->m_hasPogo == true) {
        return "die_pogo";
    }
    else {
        return "die";
    }
}

void ZombieAnimRig_ModernPogo::modInit() {
    LOGI("ZombieAnimRig_ModernPogo init");

    vftable = CopyVFTable(getActualOffset(0x23ABF70), 67);

    PatchVFTable(vftable, (void*)ZombieAnimRig_ModernPogo::StaticGetType, 0);
    PatchVFTable(vftable, (void*)hkInitModernPogoHeadList, 55);
    PatchVFTable(vftable, (void*)hkInitModernPogoLowerArmList, 56);
    PatchVFTable(vftable, (void*)hkInitModernPogoUpperArmList, 57);
    PatchVFTable(vftable, (void*)hkIdleAnim, 58);
    PatchVFTable(vftable, (void*)hkWalkAnim, 59);
    PatchVFTable(vftable, (void*)hkDieAnim, 63);

    ZombieAnimRig_ModernPogo::StaticGetType();

    LOGI("ZombieAnimRig_ModernPogo finish init");
}
