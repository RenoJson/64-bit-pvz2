#include "ZombieAnimRig_Basic.h"

void* ZombieAnimRig_JourneyToTheWestGargantuar::vftable = __null;
Sexy::RtClass* ZombieAnimRig_JourneyToTheWestGargantuar::s_rtClass = __null;;

void* hkInitJTTWGargHeadList() {

    static std::vector<SexyString> jttwGargImpHeadList = {
        "Zombie_gargantuar_head",
        "Zombie_gargantuar_jaw"
    };
    return &jttwGargImpHeadList;
}
SexyString hkJTTWParticleHead() {
    return "Gargantuar_Head_Particle";
}

void ZombieAnimRig_JourneyToTheWestGargantuar::modInit() {
    LOGI("ZombieAnimRig_JourneyToTheWestGargantuar init");

    vftable = CopyVFTable(getActualOffset(0x23E49B8), 85);

    PatchVFTable(vftable, (void*)ZombieAnimRig_JourneyToTheWestGargantuar::StaticGetType, 0);

    PatchVFTable(vftable, (void*)hkJTTWParticleHead, 44);

    PatchVFTable(vftable, (void*)hkInitJTTWGargHeadList, 55);

    ZombieAnimRig_JourneyToTheWestGargantuar::StaticGetType();
    LOGI("ZombieAnimRig_JourneyToTheWestGargantuar finish init");
}