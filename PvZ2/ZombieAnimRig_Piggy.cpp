#include "ZombieAnimRig_Vendor.h"

void* ZombieAnimRig_Piggy::vftable = __null;
Sexy::RtClass* ZombieAnimRig_Piggy::s_rtClass = __null;;


void ZombieAnimRig_Piggy::modInit() {
    LOGI("ZombieAnimRig_Piggy init");

    vftable = CopyVFTable(getActualOffset(0x23ABF70), 67);

    PatchVFTable(vftable, (void*)ZombieAnimRig_Piggy::StaticGetType, 0);

    ZombieAnimRig_Vendor::StaticGetType();

    LOGI("ZombieAnimRig_Piggy finish init");
}