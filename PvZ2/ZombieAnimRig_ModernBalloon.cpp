#include "ZombieAnimRig_ModernBalloon.h"


typedef void* (*initInitBallonLowerArmList)(uintptr_t*);
initInitBallonLowerArmList oInitBallonLowerArmList = NULL;

void* hkInitBallonLowerArmList(uintptr_t* rig) {

    if (!rig) return  oInitBallonLowerArmList(rig);

    uintptr_t vtable = *(uintptr_t*)rig;

    bool isZombieRacer = (vtable == getActualOffset(0x240AFF8));

    if (isZombieRacer) {

        static std::vector<SexyString> racerList = {
                "zombie_hand_outer_01",
                "zombie_hand_outer_02",
                "zombie_hand_outer_03",
                "zombie_arm_outer_lower"
        };
        return &racerList;
    }
    else {
        return oInitBallonLowerArmList(rig);
    }
}

void ZombieAnimRig_ModernBalloon::modInit() {
    LOGI("ZombieAnimRig_ModernBalloon init");
    PVZ2HookFunction(0x8DDDD0, (void*)hkInitBallonLowerArmList, (void**)&oInitBallonLowerArmList);
   

    LOGI("ZombieAnimRig_ModernBalloon finish init");
}