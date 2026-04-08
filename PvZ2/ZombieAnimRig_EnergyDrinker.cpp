#include "ZombieAnimRig_TombRaiser.h"
void* ZombieAnimRig_EnergyDrinker::vftable = __null;
Sexy::RtClass* ZombieAnimRig_EnergyDrinker::s_rtClass = __null;;


void* hkInitEnergyDrinkerLowerArmList() {

    static std::vector<SexyString> EnergyDrinkerLowerArmList = {
       "zombie_arm_outer_lower",
       "zombie_hand_outer_01",
       "zombie_hand_outer_02",
       "zombie_hand_outer_03"
    };
    return &EnergyDrinkerLowerArmList;
}
void* hkInitEnergyDrinkerUpperArmList() {

    static std::vector<SexyString> EnergyDrinkerUpperArmList = {
        "zombie_arm_outer_upper",
        "zombie_arm_outer_upper_bone"
    };
    return &EnergyDrinkerUpperArmList;

}
void* hkInitEnergyDrinkerHeadList() {

    static std::vector<SexyString> EnergyDrinkerHeadList = {
       "zombie_skull",
       "zombie_jaw"
    };
    return &EnergyDrinkerHeadList;
}
SexyString hkWalkAnim(ZombieAnimRig_EnergyDrinker* thisptr) {
    if (thisptr->m_gotCondition == true) {
        return "walk_condition";
    }
    else {
        return "walk";
    }
}
SexyString hkEatAnim(ZombieAnimRig_EnergyDrinker* thisptr) {
    if (thisptr->m_gotCondition == true) {
        return "eat_condition";
    }
    else {
        return "walk";
    }
}
SexyString hkDieAnim(ZombieAnimRig_EnergyDrinker* thisptr) {
    if (thisptr->m_gotCondition == true) {
        return "die_condition";
    }
    else {
        return "die";
    }
}
void ZombieAnimRig_EnergyDrinker::modInit() {
    LOGI("ZombieAnimRig_EnergyDrinker init");

    vftable = CopyVFTable(getActualOffset(0x23ABF70), 67);

    PatchVFTable(vftable, (void*)ZombieAnimRig_EnergyDrinker::StaticGetType, 0);
    PatchVFTable(vftable, (void*)hkInitEnergyDrinkerHeadList, 55);
    PatchVFTable(vftable, (void*)hkInitEnergyDrinkerLowerArmList, 56);
    PatchVFTable(vftable, (void*)hkInitEnergyDrinkerUpperArmList, 57);
    PatchVFTable(vftable, (void*)hkWalkAnim, 59);
    PatchVFTable(vftable, (void*)hkEatAnim, 61);
    PatchVFTable(vftable, (void*)hkDieAnim, 63);

    ZombieAnimRig_EnergyDrinker::StaticGetType();

    LOGI("ZombieAnimRig_EnergyDrinker finish init");
}
