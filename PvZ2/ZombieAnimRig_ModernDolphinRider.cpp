#include "ZombieAnimRig_ModernDolphinRider.h"

void* ZombieAnimRig_ModernDolphinRider::vftable = __null;
Sexy::RtClass* ZombieAnimRig_ModernDolphinRider::s_rtClass = __null;;


void* hkInitModernDolphinRiderLowerArmList() {

    static std::vector<SexyString> ModernDolphinRiderLowerArmList = {
       "zombie_arm_outer_lower",
       "zombie_arm_outer_lower_swim",
       "zombie_arm_outer_dolphin_lower",
       "zombie_hand_outer",
       "zombie_hand_outer_01",
       "zombie_hand_outer_02",
       "zombie_hand_outer_03",
       "zombie_hand_outer_swim"
    };
    return &ModernDolphinRiderLowerArmList;
}
void* hkInitModernDolphinRiderUpperArmList() {

    static std::vector<SexyString> ModernDolphinRiderUpperArmList = {
        "zombie_arm_outer_upper",
        "zombie_arm_outer_upper_bone"
    };
    return &ModernDolphinRiderUpperArmList;

}
void* hkInitModernDolphinRiderHeadList() {

    static std::vector<SexyString> ModernDolphinRiderHeadList = {
       "zombie_skull",
       "zombie_jaw"
    };
    return &ModernDolphinRiderHeadList;
}
SexyString hkIdleAnim(ZombieAnimRig_ModernDolphinRider* thisptr) {
    return "idle";
}
SexyString hkWalkAnim(ZombieAnimRig_ModernDolphinRider* thisptr) {
    if (thisptr->m_isCarryingDolphin == true && thisptr->m_isRidingDolphin == false) {
        return "walk";
    }
    else if(thisptr->m_isCarryingDolphin == false && thisptr->m_isRidingDolphin == true) {
        return "swim";
	}
    else {
        return "walk_single";
    }
}
SexyString hkEatAnim(ZombieAnimRig_ModernDolphinRider* thisptr) {
    if (thisptr->m_isCarryingDolphin == true) {
        return "eat";
    }
    else {
        return "eat_single";
    }
}
SexyString hkDieAnim(ZombieAnimRig_ModernDolphinRider* thisptr) {
    if (thisptr->m_isCarryingDolphin == true && thisptr->m_isRidingDolphin == false) {
        return "die";
    }
    else if (thisptr->m_isCarryingDolphin == false && thisptr->m_isRidingDolphin == true) {
        return "swim_die";
    }
    else {
        return "die_single";
    }
}

void ZombieAnimRig_ModernDolphinRider::modInit() {
    LOGI("ZombieAnimRig_ModernDolphinRider init");

    vftable = CopyVFTable(getActualOffset(0x23ABF70), 67);

    PatchVFTable(vftable, (void*)ZombieAnimRig_ModernDolphinRider::StaticGetType, 0);
    PatchVFTable(vftable, (void*)hkInitModernDolphinRiderHeadList, 55);
    PatchVFTable(vftable, (void*)hkInitModernDolphinRiderLowerArmList, 56);
    PatchVFTable(vftable, (void*)hkInitModernDolphinRiderUpperArmList, 57);
    PatchVFTable(vftable, (void*)hkIdleAnim, 58);
    PatchVFTable(vftable, (void*)hkWalkAnim, 59);
    PatchVFTable(vftable, (void*)hkEatAnim, 61);
    PatchVFTable(vftable, (void*)hkDieAnim, 63);

    ZombieAnimRig_ModernDolphinRider::StaticGetType();

    LOGI("ZombieAnimRig_ModernDolphinRider finish init");
}
