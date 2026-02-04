#include "ZombieAnimRig_DarkRider.h"

void* ZombieAnimRig_DarkRider::vftable = __null;
Sexy::RtClass* ZombieAnimRig_DarkRider::s_rtClass = __null;;

void* hkInitDarkRiderHeadList() {

    static std::vector<SexyString> DarkRiderHeadList = {
        "zombie_skull",
        "zombie_jaw"
    };
    return &DarkRiderHeadList;
}

void* hkInitDarkRiderLowerArmList() {

    static std::vector<SexyString> DarkRiderLowerArmList = {
        "zombie_hand_outer_01",
        "zombie_hand_outer_02",
        "zombie_hand_outer_03",
        "zombie_arm_outer_lower"
    };
    return &DarkRiderLowerArmList;
}

void* hkInitDarkRiderUpperArmList() {

    static std::vector<SexyString> DarkRiderUpperArmList = {
       "zombie_arm_outer_upper",
       "zombie_arm_outer_upper_bone"
    };
    return &DarkRiderUpperArmList;
}

void ZombieAnimRig_DarkRider::modInit() {
    LOGI("ZombieAnimRig_DarkRider init");

    vftable = CopyVFTable(getActualOffset(0x23EF5D8), 90);

    PatchVFTable(vftable, (void*)ZombieAnimRig_DarkRider::StaticGetType, 0);
    PatchVFTable(vftable, (void*)hkInitDarkRiderHeadList, 55);
    PatchVFTable(vftable, (void*)hkInitDarkRiderLowerArmList, 56);
    PatchVFTable(vftable, (void*)hkInitDarkRiderUpperArmList, 57);
    ZombieAnimRig_DarkRider::StaticGetType();
    LOGI("ZombieAnimRig_DarkRider finish init");
}