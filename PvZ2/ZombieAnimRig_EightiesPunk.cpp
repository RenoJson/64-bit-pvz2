#include "ZombieAnimRig_EightiesPunk.h"

typedef void* (*initPunkUpperArmList)();
initPunkUpperArmList oInitPunkUpperArmList = NULL;

void* hkInitPunkUpperArmList() {

    static std::vector<SexyString> punkUpperArmList = {
        "zombie_arm_outer_upper",
        "zombie_arm_outer_upper_bone"
    };
    return &punkUpperArmList;
}
void ZombieAnimRig_EightiesPunk::modInit() {
    LOGI("ZombieAnimRig_EightiesPunk init");
    PVZ2HookFunction(0xB39194, (void*)hkInitPunkUpperArmList, (void**)&oInitPunkUpperArmList);

    LOGI("ZombieAnimRig_EightiesPunk finish init");
}