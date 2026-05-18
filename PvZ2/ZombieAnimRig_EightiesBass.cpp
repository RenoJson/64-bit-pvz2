#include "ZombieAnimRig_EightiesBass.h"

void* ZombieAnimRig_EightiesBass::vftable = __null;
Sexy::RtClass* ZombieAnimRig_EightiesBass::s_rtClass = __null;;


void* hkInitEightiesBassLowerArmList() {

    static std::vector<SexyString> EightiesBassLowerArmList = {
       "zombie_arm_outer_lower",
       "zombie_hand_outer_01",
       "zombie_hand_outer_02",
       "zombie_hand_outer_03",
       "zombie_hand_outer_04"
    };
    return &EightiesBassLowerArmList;
}
void* hkInitEightiesBassUpperArmList() {

    static std::vector<SexyString> EightiesBassUpperArmList = {
        "zombie_arm_outer_upper",
        "zombie_arm_outer_upper_bone"
    };
    return &EightiesBassUpperArmList;

}
void* hkInitEightiesBassHeadList() {

    static std::vector<SexyString> EightiesBassHeadList = {
       "zombie_skull",
       "zombie_jaw"
    };
    return &EightiesBassHeadList;
}
SexyString hkIdleAnim(ZombieAnimRig_EightiesBass* thisptr) {
	return "attack";
}
SexyString hkDieAnim(ZombieAnimRig_EightiesBass* thisptr) {
    if (thisptr->m_hasGuitar == true) {
        return "die1";
    }
    else {
        return "die";
    }
}

void ZombieAnimRig_EightiesBass::modInit() {
    LOGI("ZombieAnimRig_EightiesBass init");

    vftable = CopyVFTable(getActualOffset(0x23ABF70), 67);

    PatchVFTable(vftable, (void*)ZombieAnimRig_EightiesBass::StaticGetType, 0);
    PatchVFTable(vftable, (void*)hkInitEightiesBassHeadList, 55);
    PatchVFTable(vftable, (void*)hkInitEightiesBassLowerArmList, 56);
    PatchVFTable(vftable, (void*)hkInitEightiesBassUpperArmList, 57);
    PatchVFTable(vftable, (void*)hkIdleAnim, 58);
    PatchVFTable(vftable, (void*)hkDieAnim, 63);

    ZombieAnimRig_EightiesBass::StaticGetType();

    LOGI("ZombieAnimRig_EightiesBass finish init");
}
