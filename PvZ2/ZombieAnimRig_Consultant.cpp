#include "ZombieAnimRig_Consultant.h"

typedef void* (*initConsultantLowerArmList)();
initConsultantLowerArmList oInitConsultantLowerArmList = NULL;

void* hkInitConsultantLowerArmList() {

    static std::vector<SexyString> consultantLowerArmList = {
        "zombie_corporate_consultant_arm_outer_lower",
        "zombie_corporate_consultant_hand_outer_05",
        "zombie_corporate_consultant_hand_outer_04",
        "zombie_corporate_consultant_hand_outer_03",
        "zombie_corporate_consultant_hand_outer_02",
        "zombie_corporate_consultant_hand_outer_01",
    };
    return &consultantLowerArmList;
}
void ZombieAnimRig_Consultant::modInit() {
    LOGI("ZombieAnimRig_Consultant init");
    PVZ2HookFunction(0xC11864, (void*)hkInitConsultantLowerArmList, (void**)&oInitConsultantLowerArmList);

    LOGI("ZombieAnimRig_Consultant finish init");
}