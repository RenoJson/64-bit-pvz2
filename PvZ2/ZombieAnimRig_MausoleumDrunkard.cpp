#include "ZombieAnimRig_MausoleumDrunkard.h"

void* ZombieAnimRig_MausoleumDrunkard::vftable = __null;
Sexy::RtClass* ZombieAnimRig_MausoleumDrunkard::s_rtClass = __null;;


std::vector<SexyString>* hkInitMausoleumDrunkardLowerArmList() {

    static std::vector<SexyString> MausoleumDrunkardLowerArmList = {
       "zombie_digger_arm_outer_lower",
       "zombie_digger_hand_outer",
       "zombie_wine_jar"
    };
    return &MausoleumDrunkardLowerArmList;
}
std::vector<SexyString>* hkInitMausoleumDrunkardUpperArmList() {

    static std::vector<SexyString> MausoleumDrunkardUpperArmList = {
        "zombie_digger_arm_outer_upper",
        "zombie_digger_arm_outer_upper2"
    };
    return &MausoleumDrunkardUpperArmList;
}
std::vector<SexyString>* hkInitMausoleumDrunkardHeadList() {

    static std::vector<SexyString> MausoleumDrunkardHeadList = {
       "zombie_skull",
       "zombie_jaw"
    };
    return &MausoleumDrunkardHeadList;
}
SexyString hkIdleAnim(ZombieAnimRig_MausoleumDrunkard* thisptr) {
    return "idle";
}
SexyString hkWalkAnim(ZombieAnimRig_MausoleumDrunkard* thisptr) {
    return "walk";
}
SexyString hkEatAnim(ZombieAnimRig_MausoleumDrunkard* thisptr) {
    return "eat";

}
SexyString hkDieAnim(ZombieAnimRig_MausoleumDrunkard* thisptr) {
    int roll = rand() % 100;
    if (roll < 50) {
        return "die";
    }
    else if (roll < 80) {
        return "die2";
    }
    else {
        return "die3";
    }
}

void ZombieAnimRig_MausoleumDrunkard::modInit() {
    LOGI("ZombieAnimRig_MausoleumDrunkard init");

    vftable = CopyVFTable(getActualOffset(0x23ABF70), 67);

    PatchVFTable(vftable, (std::vector<SexyString>*)ZombieAnimRig_MausoleumDrunkard::StaticGetType, 0);
    PatchVFTable(vftable, (std::vector<SexyString>*)hkInitMausoleumDrunkardHeadList, 55);
    PatchVFTable(vftable, (std::vector<SexyString>*)hkInitMausoleumDrunkardLowerArmList, 56);
    PatchVFTable(vftable, (std::vector<SexyString>*)hkInitMausoleumDrunkardUpperArmList, 57);
    PatchVFTable(vftable, (std::vector<SexyString>*)hkIdleAnim, 58);
    PatchVFTable(vftable, (std::vector<SexyString>*)hkWalkAnim, 59);
    PatchVFTable(vftable, (std::vector<SexyString>*)hkEatAnim, 61);
    PatchVFTable(vftable, (std::vector<SexyString>*)hkDieAnim, 63);

    ZombieAnimRig_MausoleumDrunkard::StaticGetType();

    LOGI("ZombieAnimRig_MausoleumDrunkard finish init");
}
