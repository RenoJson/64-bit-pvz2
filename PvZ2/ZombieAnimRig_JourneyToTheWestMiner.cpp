#include "ZombieAnimRig_Basic.h"

void* ZombieAnimRig_ModernMiner::vftable = __null;
Sexy::RtClass* ZombieAnimRig_ModernMiner::s_rtClass = __null;;

void* hkInitModernMinerLowerArmList() {

    static std::vector<SexyString> ModernMinerLowerArmList = {
        "zombie_digger_hand_outer",
        "zombie_digger_arm_outer_lower"
    };
    return &ModernMinerLowerArmList;
}
void* hkInitModernMinerUpperArmList() {

    static std::vector<SexyString> ModernMinerUpperArmList = {
         "zombie_digger_arm_outer_upper",
         "zombie_digger_arm_outer_upper2" 
    };
    return &ModernMinerUpperArmList;

}
void* hkInitModernMinerHeadList() {

    static std::vector<SexyString> ModernMinerHeadList = {
       "zombie_hat",
       "zombie_skull",
       "zombie_jaw",
       "zombie_hat_back"
    };
    return &ModernMinerHeadList;
}
SexyString hkWalkAnim(ZombieAnimRig_ModernMiner* thisptr) {
    if (thisptr->m_hasStick == true && thisptr->m_digging == true) {
        return "walk2";
    }
    if (thisptr->m_hasStick == true) {
        return "walk";
    }
    else {
        return "walk_no_tool";
    }
}
SexyString hkEatAnim(ZombieAnimRig_ModernMiner* thisptr) {
    if (thisptr->m_hasStick == true) {
        return "eat";
    }
    else {
        return "eat_no_tool";
    }
}
SexyString hkDieAnim(ZombieAnimRig_ModernMiner* thisptr) {
    if (thisptr->m_hasStick == true) {
        return "die";
    }
    else {
        return "die_no_tool";
    }
}

void ZombieAnimRig_ModernMiner::modInit() {
    LOGI("ZombieAnimRig_ModernMiner init");
    vftable = CopyVFTable(getActualOffset(0x23E49B8), 85);
    PatchVFTable(vftable, (void*)ZombieAnimRig_ModernMiner::StaticGetType, 0);
    PatchVFTable(vftable, (void*)hkInitModernMinerHeadList, 55);
    PatchVFTable(vftable, (void*)hkInitModernMinerLowerArmList, 56);
    PatchVFTable(vftable, (void*)hkInitModernMinerUpperArmList, 57);
    PatchVFTable(vftable, (void*)hkWalkAnim, 59);
    PatchVFTable(vftable, (void*)hkEatAnim, 61);
    PatchVFTable(vftable, (void*)hkDieAnim, 63);
    ZombieAnimRig_ModernMiner::StaticGetType();
    LOGI("ZombieAnimRig_ModernMiner finish init");
}