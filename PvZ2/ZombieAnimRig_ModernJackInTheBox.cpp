#include "ZombieAnimRig_ModernJackInTheBox.h"

void* ZombieAnimRig_ModernJackInTheBox::vftable = __null;
Sexy::RtClass* ZombieAnimRig_ModernJackInTheBox::s_rtClass = __null;;


void* hkInitModernJackInTheBoxLowerArmList() {

    static std::vector<SexyString> ModernJackInTheBoxLowerArmList = {};
    return &ModernJackInTheBoxLowerArmList;
}
void* hkInitModernJackInTheBoxUpperArmList() {

    static std::vector<SexyString> ModernJackInTheBoxUpperArmList = {
        "zombie_arm_outer_lower",
        "zombie_arm_outer_lower_bone"
    };
    return &ModernJackInTheBoxUpperArmList;

}
void* hkInitModernJackInTheBoxHeadList() {

    static std::vector<SexyString> ModernJackInTheBoxHeadList = {
       "zombie_skull",
       "zombie_skull_01",
       "zombie_jaw",
       "zombie_jaw_01",
       "zombie_jaw_02"
    };
    return &ModernJackInTheBoxHeadList;
}
SexyString hkIdleAnim(ZombieAnimRig_ModernJackInTheBox* thisptr) {
    return "idle";
}
SexyString hkWalkAnim(ZombieAnimRig_ModernJackInTheBox* thisptr) {
    if (thisptr->m_hasBox == true) {
        return "walk_box";
    }
    else {
        return "walk";
    }
}
SexyString hkEatAnim(ZombieAnimRig_ModernJackInTheBox* thisptr) {
    if (thisptr->m_hasBox == true) {
        return "eat_box";
    }
    else {
        return "eat";
    }
}
SexyString hkDieAnim(ZombieAnimRig_ModernJackInTheBox* thisptr) {
    if (thisptr->m_hasBox == true) {
        return "die_box";
    }
    else {
        return "die";
    }
}

void ZombieAnimRig_ModernJackInTheBox::modInit() {
    LOGI("ZombieAnimRig_ModernJackInTheBox init");

    vftable = CopyVFTable(getActualOffset(0x23ABF70), 67);

    PatchVFTable(vftable, (void*)ZombieAnimRig_ModernJackInTheBox::StaticGetType, 0);
    PatchVFTable(vftable, (void*)hkInitModernJackInTheBoxHeadList, 55);
    PatchVFTable(vftable, (void*)hkInitModernJackInTheBoxLowerArmList, 56);
    PatchVFTable(vftable, (void*)hkInitModernJackInTheBoxUpperArmList, 57);
    PatchVFTable(vftable, (void*)hkIdleAnim, 58);
    PatchVFTable(vftable, (void*)hkWalkAnim, 59);
    PatchVFTable(vftable, (void*)hkEatAnim, 61);
    PatchVFTable(vftable, (void*)hkDieAnim, 63);

    ZombieAnimRig_ModernJackInTheBox::StaticGetType();

    LOGI("ZombieAnimRig_ModernJackInTheBox finish init");
}
