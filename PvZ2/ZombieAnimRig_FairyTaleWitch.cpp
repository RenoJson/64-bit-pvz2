#include "ZombieAnimRig_FairyTaleWitch.h"

void* ZombieAnimRig_FairyTaleWitch::vftable = __null;
Sexy::RtClass* ZombieAnimRig_FairyTaleWitch::s_rtClass = __null;;


void* hkInitFairyTaleWitchLowerArmList() {

    static std::vector<SexyString> FairyTaleWitchLowerArmList = {
       "zombie_egyptflag_hand_outer",
       "zombie_egyptflag_arm_outer_lower",
       "witch_book"
    };
    return &FairyTaleWitchLowerArmList;
}
void* hkInitFairyTaleWitchUpperArmList() {

    static std::vector<SexyString> FairyTaleWitchUpperArmList = {
        "zombie_arm_outer_upper",
        "zombie_arm_outer_upper_bone"
    };
    return &FairyTaleWitchUpperArmList;

}
void* hkInitFairyTaleWitchHeadList() {

    static std::vector<SexyString> FairyTaleWitchHeadList = {
       "zombie_egyptflag_skull",
       "zombie_egyptflag_jaw"
    };
    return &FairyTaleWitchHeadList;
}

void ZombieAnimRig_FairyTaleWitch::modInit() {
    LOGI("ZombieAnimRig_FairyTaleWitch init");

    vftable = CopyVFTable(getActualOffset(0x23D9F58), 85);

    PatchVFTable(vftable, (void*)ZombieAnimRig_FairyTaleWitch::StaticGetType, 0);
    PatchVFTable(vftable, (void*)hkInitFairyTaleWitchHeadList, 55);
    PatchVFTable(vftable, (void*)hkInitFairyTaleWitchLowerArmList, 56);
    PatchVFTable(vftable, (void*)hkInitFairyTaleWitchUpperArmList, 57);

    ZombieAnimRig_FairyTaleWitch::StaticGetType();

    LOGI("ZombieAnimRig_FairyTaleWitch finish init");
}