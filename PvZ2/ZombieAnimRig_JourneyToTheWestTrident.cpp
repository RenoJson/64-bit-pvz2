#include "ZombieAnimRig_Hunter.h"

void* ZombieAnimRig_JourneyToTheWestTrident::vftable = __null;
Sexy::RtClass* ZombieAnimRig_JourneyToTheWestTrident::s_rtClass = __null;;

void* hkInitTridentLowerArmList() {

    static std::vector<SexyString> tridentLowerArmList = {
       "zombie_arm_outer_lower",
       "zombie_hand_outer_01",
       "zombie_hand_outer_02",
       "zombie_hand_outer_03"
    };
    return &tridentLowerArmList;
}

void ZombieAnimRig_JourneyToTheWestTrident::modInit() {
    LOGI("ZombieAnimRig_JourneyToTheWestTrident init");

    vftable = CopyVFTable(getActualOffset(0x2402218), 67);

    PatchVFTable(vftable, (void*)ZombieAnimRig_JourneyToTheWestTrident::StaticGetType, 0);

    PatchVFTable(vftable, (void*)hkInitTridentLowerArmList, 56);

    ZombieAnimRig_JourneyToTheWestTrident::StaticGetType();

    LOGI("ZombieAnimRig_JourneyToTheWestTrident finish init");
}