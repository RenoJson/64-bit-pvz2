#include "ZombieAnimRig_MausoleumAssasin.h"

void* ZombieAnimRig_MausoleumAssasin::vftable = __null;
Sexy::RtClass* ZombieAnimRig_MausoleumAssasin::s_rtClass = __null;;

void CreateAssasinAnimRig(ZombieAnimRig_MausoleumAssasin* thisPtr)
{
    thisPtr->m_hasDagger = false;
    thisPtr->m_hasScroll = true;
    CallFunc<void, ZombieAnimRig_MausoleumAssasin*>(0x8DE104, thisPtr);
}

void* hkInitMausoleumAssasinLowerArmList() {

    static std::vector<SexyString> MausoleumAssasinLowerArmList = {
       "zombie_arm_outer_lower",
       "zombie_arm_outer_lower_01",
       "zombie_hand_outer_dao",
       "zombie_arm_outer_lower02",
       "zombie_hand_outer_01",
       "zombie_hand_outer_02",
       "zombie_hand_outer_03"
    };
    return &MausoleumAssasinLowerArmList;
}
void* hkInitMausoleumAssasinUpperArmList() {

    static std::vector<SexyString> MausoleumAssasinUpperArmList = {
        "zombie_arm_outer_upper",
        "zombie_arm_outer_upper_bone"
    };
    return &MausoleumAssasinUpperArmList;
}
void* hkInitMausoleumAssasinHeadList() {

    static std::vector<SexyString> MausoleumAssasinHeadList = {
       "zombie_skull",
       "zombie_jaw",
       "_zombie_glasses"
    };
    return &MausoleumAssasinHeadList;
}
SexyString hkIdleAnim(ZombieAnimRig_MausoleumAssasin* thisptr) {
    return "idle";
}
SexyString hkWalkAnim(ZombieAnimRig_MausoleumAssasin* thisptr) {
    if (thisptr->m_hasScroll == true) {
        return "walk";
    }
    else {
        return "walk02";
    }
}
SexyString hkEatAnim(ZombieAnimRig_MausoleumAssasin* thisptr) {
    if (thisptr->m_hasScroll == true) {
        return "eat";
    }
    else {
        return "eat02";
    }
}
SexyString hkDieAnim(ZombieAnimRig_MausoleumAssasin* thisptr) {
     return "die";
}

void ZombieAnimRig_MausoleumAssasin::modInit() {
    LOGI("ZombieAnimRig_MausoleumAssasin init");

    vftable = CopyVFTable(getActualOffset(0x23ABF70), 67);

    PatchVFTable(vftable, (void*)ZombieAnimRig_MausoleumAssasin::StaticGetType, 0);
    PatchVFTable(vftable, (void*)CreateAssasinAnimRig, 21);
    PatchVFTable(vftable, (void*)hkInitMausoleumAssasinHeadList, 55);
    PatchVFTable(vftable, (void*)hkInitMausoleumAssasinLowerArmList, 56);
    PatchVFTable(vftable, (void*)hkInitMausoleumAssasinUpperArmList, 57);
    PatchVFTable(vftable, (void*)hkIdleAnim, 58);
    PatchVFTable(vftable, (void*)hkWalkAnim, 59);
    PatchVFTable(vftable, (void*)hkEatAnim, 61);
    PatchVFTable(vftable, (void*)hkDieAnim, 63);

    ZombieAnimRig_MausoleumAssasin::StaticGetType();

    LOGI("ZombieAnimRig_MausoleumAssasin finish init");
}
