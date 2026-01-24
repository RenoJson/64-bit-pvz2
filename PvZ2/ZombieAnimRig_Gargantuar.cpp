#include "ZombieAnimRig_Gargantuar.h"

void* ZombieAnimRig_FairyTaleGargantuar::vftable = __null;
Sexy::RtClass* ZombieAnimRig_FairyTaleGargantuar::s_rtClass = __null;;

typedef void* (*initGargantuarHeadList)();
initGargantuarHeadList oInitGargantuarHeadList = NULL;

void* hkInitGargantuarHeadList() {

    static std::vector<SexyString> gargantuarHeadList = {
        "Zombie_gargantuar_head",
        "Zombie_gargantuar_jaw"
    };
    return &gargantuarHeadList;
}

typedef void* (*initZcorpImpHeadList)();
initZcorpImpHeadList oInitZcorpImpHeadList = NULL;

void* hkIniZcorpImpHeadList() {

    static std::vector<SexyString> zcorpImpHeadList = {
        "Zombie_Corporate_Imp_Skull",
        "Zombie_Corporate_Imp_Jaw"
    };
    return &zcorpImpHeadList;
}

void* hkInitFairyTaleGargHeadList() {

    static std::vector<SexyString> ftfGargImpHeadList = {
        "Zombie_gargantuar_head",
        "Zombie_gargantuar_jaw",
        "Zombie_gargantuar_feather_front",
        "Zombie_gargantuar_feather_back"
    };
    return &ftfGargImpHeadList;
}

void ZombieAnimRig_Gargantuar::modInit() {
    LOGI("ZombieAnimRig_Gargantuar init");
    PVZ2HookFunction(0xB54F18, (void*)hkInitGargantuarHeadList, (void**)&oInitGargantuarHeadList);
    PVZ2HookFunction(0x129D9B4, (void*)hkIniZcorpImpHeadList, (void**)&oInitZcorpImpHeadList);
    LOGI("ZombieAnimRig_Gargantuar finish init");
}

void ZombieAnimRig_FairyTaleGargantuar::modInit() {
    LOGI("ZombieAnimRig_FairyTaleGargantuar init");

    vftable = CopyVFTable(getActualOffset(0x23EC798), 67);

    PatchVFTable(vftable, (void*)ZombieAnimRig_FairyTaleGargantuar::StaticGetType, 0);

    PatchVFTable(vftable, (void*)hkInitFairyTaleGargHeadList, 55);

    ZombieAnimRig_FairyTaleGargantuar::StaticGetType();
    LOGI("ZombieAnimRig_FairyTaleGargantuar finish init");
}