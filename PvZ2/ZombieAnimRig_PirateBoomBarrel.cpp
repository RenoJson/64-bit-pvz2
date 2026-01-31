#include "ZombieAnimRig_Imp.h"

void* ZombieAnimRig_PirateBoomBarrel::vftable = __null;
Sexy::RtClass* ZombieAnimRig_PirateBoomBarrel::s_rtClass = __null;;

void* hkInitPirateBoomBarrelHeadList() {

    static std::vector<SexyString> pirateBoomBarrelHeadList = {
        "zombie_skull",
        "zombie_jaw"
    };
    return &pirateBoomBarrelHeadList;
}

void* hkInitPirateBoomBarrelLowerArmList() {

    static std::vector<SexyString> pirateBoomBarrelLowerArmList = {
        "zombie_hand_outer",
        "zombie_arm_outer_lower"
    };
    return &pirateBoomBarrelLowerArmList;
}

void ZombieAnimRig_PirateBoomBarrel::modInit() {
    LOGI("ZombieAnimRig_PirateBoomBarrel init");

    vftable = CopyVFTable(getActualOffset(0x240B638), 90);

    PatchVFTable(vftable, (void*)ZombieAnimRig_PirateBoomBarrel::StaticGetType, 0);


    PatchVFTable(vftable, (void*)hkInitPirateBoomBarrelHeadList, 55);
    PatchVFTable(vftable, (void*)hkInitPirateBoomBarrelLowerArmList, 56);

    ZombieAnimRig_PirateBoomBarrel::StaticGetType();
    LOGI("ZombieAnimRig_PirateBoomBarrel finish init");
}