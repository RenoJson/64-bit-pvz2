#include "ZombieAnimRig_Vendor.h"

void* ZombieAnimRig_Vendor::vftable = __null;
Sexy::RtClass* ZombieAnimRig_Vendor::s_rtClass = __null;;


void* hkInitVendorLowerArmList() {

    static std::vector<SexyString> vendorLowerArmList = {
       "zombie_arm_outer_lower",
       "zombie_hand_outer_01",
       "zombie_hand_outer_02",
       "zombie_hand_outer_03",
       "zombie_hand_outer_04"
    };
    return &vendorLowerArmList;
}
void* hkInitVendorUpperArmList() {

    static std::vector<SexyString> vendorUpperArmList = {
        "zombie_arm_outer_upper",
        "zombie_arm_outer_upper_bone"
    };
    return &vendorUpperArmList;

}
void* hkInitVendorHeadList() {

    static std::vector<SexyString> vendorHeadList = {
       "zombie_skull",
       "zombie_jaw"
    };
    return &vendorHeadList;
}
SexyString hkWalkAnim(ZombieAnimRig_Vendor* thisptr){
    return "set_pig";
}

void ZombieAnimRig_Vendor::modInit() {
    LOGI("ZombieAnimRig_Vendor init");

    vftable = CopyVFTable(getActualOffset(0x23ABF70), 67);

    PatchVFTable(vftable, (void*)ZombieAnimRig_Vendor::StaticGetType, 0);
    PatchVFTable(vftable, (void*)hkInitVendorHeadList, 55);
    PatchVFTable(vftable, (void*)hkInitVendorLowerArmList, 56);
    PatchVFTable(vftable, (void*)hkInitVendorUpperArmList, 57);
    PatchVFTable(vftable, (void*)hkWalkAnim, 59);

    ZombieAnimRig_Vendor::StaticGetType();

    LOGI("ZombieAnimRig_Vendor finish init");
}
