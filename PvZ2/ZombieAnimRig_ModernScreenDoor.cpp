#include "ZombieAnimRig_ModernScreenDoor.h"
#include "ZombieHelper.h"

void* ZombieAnimRig_ModernScreenDoor::vftable = __null;
Sexy::RtClass* ZombieAnimRig_ModernScreenDoor::s_rtClass = __null;;

void* ZombieAnimRig_ModernScreenDoorAlmanac::vftable = __null;
Sexy::RtClass* ZombieAnimRig_ModernScreenDoorAlmanac::s_rtClass = __null;;


void* hkInitModernScreenDoorLowerArmList(ZombieAnimRig_ModernScreenDoor* thisptr) {

    static std::vector<SexyString> ModernScreenDoorLowerArmList = {};
    static std::vector<SexyString> ModernScreenNoDoorLowerArmList = {
       "zombie_arm_outer_lower",
       "zombie_arm_outer_lower_02",
       "zombie_arm_outer_lower_03",
       "zombie_hand_outer_01",
       "zombie_hand_outer_02",
       "zombie_hand_outer_03",
       "zombie_hand_outer"
    };;
    if (thisptr->m_hasDoor == true) {
        return &ModernScreenDoorLowerArmList;
    }
    else {
        return &ModernScreenNoDoorLowerArmList;
    }
}
void* hkInitModernScreenDoorUpperArmList() {

    static std::vector<SexyString> ModernScreenDoorUpperArmList = {
        "zombie_arm_outer_upper",
        "zombie_arm_outer_upper_bone"
    };
    return &ModernScreenDoorUpperArmList;

}
void* hkInitModernScreenDoorHeadList() {

    static std::vector<SexyString> ModernScreenDoorHeadList = {
       "zombie_skull",
       "zombie_jaw"
    };
    return &ModernScreenDoorHeadList;
}
SexyString hkIdleAnim(ZombieAnimRig_ModernScreenDoor* thisptr) {
    if (thisptr->m_hasDoor == true) {
        return "idle_screendoor";
    }
    else {
        return "idle";
    }
}
SexyString hkWalkAnim(ZombieAnimRig_ModernScreenDoor* thisptr) {
    if (thisptr->m_hasDoor == true) {
        return "walk_screendoor";
    }
    else {
        return "walk";
    }
}
SexyString hkEatAnim(ZombieAnimRig_ModernScreenDoor* thisptr) {
    if (thisptr->m_hasDoor == true) {
        return "eat_screendoor";
    }
    else {
        return "eat";
    }
}
SexyString hkDieAnim(ZombieAnimRig_ModernScreenDoor* thisptr) {
    if (thisptr->m_hasDoor == true) {
        return "die_screendoor";
    }
    else {
        return "die";
    }
}

void ZombieAnimRig_ModernScreenDoor::modInit() {
    LOGI("ZombieAnimRig_ModernScreenDoor init");

    vftable = CopyVFTable(getActualOffset(0x23ABF70), 67);

    PatchVFTable(vftable, (void*)ZombieAnimRig_ModernScreenDoor::StaticGetType, 0);
    PatchVFTable(vftable, (void*)hkInitModernScreenDoorHeadList, 55);
    PatchVFTable(vftable, (void*)hkInitModernScreenDoorLowerArmList, 56);
    PatchVFTable(vftable, (void*)hkInitModernScreenDoorUpperArmList, 57);
    PatchVFTable(vftable, (void*)hkIdleAnim, 58);
    PatchVFTable(vftable, (void*)hkWalkAnim, 59);
    PatchVFTable(vftable, (void*)hkEatAnim, 61);
    PatchVFTable(vftable, (void*)hkDieAnim, 63);

    ZombieAnimRig_ModernScreenDoor::StaticGetType();

    LOGI("ZombieAnimRig_ModernScreenDoor finish init");
}

void CreateScreenDoorAnimRig(ZombieAnimRig_ModernScreenDoorAlmanac* thisPtr)
{
    CallFunc<void>(0x8DE104, thisPtr);
    SetAnimLayerVisible(thisPtr, "screen_door_damage01", false);
    SetAnimLayerVisible(thisPtr, "screen_door_damage02", false);
}

void ZombieAnimRig_ModernScreenDoorAlmanac::modInit() {
    LOGI("ZombieAnimRig_ModernScreenDoorAlmanac init");

    vftable = CopyVFTable(getActualOffset(0x23ABF70), 67);

    PatchVFTable(vftable, (void*)ZombieAnimRig_ModernScreenDoorAlmanac::StaticGetType, 0);
    PatchVFTable(vftable, (void*)CreateScreenDoorAnimRig, 21);
    PatchVFTable(vftable, (void*)hkInitModernScreenDoorHeadList, 55);
    PatchVFTable(vftable, (void*)hkInitModernScreenDoorLowerArmList, 56);
    PatchVFTable(vftable, (void*)hkInitModernScreenDoorUpperArmList, 57);
    PatchVFTable(vftable, (void*)hkIdleAnim, 58);
    PatchVFTable(vftable, (void*)hkWalkAnim, 59);
    PatchVFTable(vftable, (void*)hkEatAnim, 61);
    PatchVFTable(vftable, (void*)hkDieAnim, 63);
    ZombieAnimRig_ModernScreenDoorAlmanac::StaticGetType();

    LOGI("ZombieAnimRig_ModernScreenDoorAlmanac finish init");
}
