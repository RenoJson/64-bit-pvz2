#include "ZombieAnimRig_ModernAllStar.h"

void* ZombieAnimRig_JourneyToTheWestAllStar::vftable = __null;
Sexy::RtClass* ZombieAnimRig_JourneyToTheWestAllStar::s_rtClass = __null;;

typedef void* (*initAllStarLowerArmList)();
initAllStarLowerArmList oInitAllStarLowerArmList = NULL;

void* hkInitAllStarLowerArmList() {

    static std::vector<SexyString> allstarLowerArmList = {
       "zombie_arm_outer_lower",
       "zombie_hand_outer_01",
       "zombie_hand_outer_02",
       "zombie_hand_outer_03"
    };
    return &allstarLowerArmList;
}

typedef void* (*initAllStarUpperArmList)();
initAllStarUpperArmList oInitAllStarUpperArmList = NULL;

void* hkInitAllStarUpperArmList() {

    static std::vector<SexyString> allstarUpperArmList = {
        "zombie_arm_outer_upper",
        "zombie_arm_outer_upper_bone"
    };
    return &allstarUpperArmList;
}

void* hkInitMarshalStarHeadList() {

    static std::vector<SexyString> marshalHeadList = {
        "zombie_skull",
        "zombie_jaw",
        "allstar_head_helmet",
        "allstar_head_helmet_back"
    };
    return &marshalHeadList;
}

void ZombieAnimRig_ModernAllStar::modInit() {
	LOGI("ZombieAnimRig_ModernAllStar init");
    PVZ2HookFunction(0xBBA94C, (void*)hkInitAllStarLowerArmList, (void**)&oInitAllStarLowerArmList);
    PVZ2HookFunction(0xBBAD60, (void*)hkInitAllStarUpperArmList, (void**)&oInitAllStarUpperArmList);
	LOGI("ZombieAnimRig_ModernAllStar finish init");
}

void ZombieAnimRig_JourneyToTheWestAllStar::modInit() {
	LOGI("ZombieAnimRig_JourneyToTheWestAllStar init");

	vftable = CopyVFTable(getActualOffset(0x240AD78), 67);

	PatchVFTable(vftable, (void*)ZombieAnimRig_JourneyToTheWestAllStar::StaticGetType, 0);

    PatchVFTable(vftable, (void*)hkInitMarshalStarHeadList, 55);

	ZombieAnimRig_JourneyToTheWestAllStar::StaticGetType();

	LOGI("ZombieAnimRig_JourneyToTheWestAllStar finish init");
}