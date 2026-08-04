#include "ZombieAnimRig_Basic.h"

void* ZombieAnimRig_BasicTemplate::vftable = __null;
Sexy::RtClass* ZombieAnimRig_BasicTemplate::s_rtClass = __null;;

SexyString ParticleHead(ZombieAnimRig_BasicTemplate* thisPtr) {
    return thisPtr->m_ParticleHeadSpriteName;
}
SexyString ParticleArm(ZombieAnimRig_BasicTemplate* thisPtr) {
    return thisPtr->m_ParticleArmSpriteName;
}
void* HeadList(ZombieAnimRig_BasicTemplate* thisPtr) {
    return &thisPtr->m_HeadLayers;
}
void* LowerArmList(ZombieAnimRig_BasicTemplate* thisPtr) {
    return &thisPtr->m_LowerArmLayers;
}
void* UpperArmList(ZombieAnimRig_BasicTemplate* thisPtr) {
    return &thisPtr->m_UpperArmLayers;
}
SexyString Idle(ZombieAnimRig_BasicTemplate* thisPtr) {
    return thisPtr->m_IdleAnimName;
}
SexyString Walk(ZombieAnimRig_BasicTemplate* thisPtr) {
    if (thisPtr->m_dancing == true) {
        return "dance_start";
    }
    else {
        return thisPtr->m_WalkAnimName;
    }
}
SexyString Eat(ZombieAnimRig_BasicTemplate* thisPtr) {
    return thisPtr->m_EatAnimName;
}
SexyString Die(ZombieAnimRig_BasicTemplate* thisPtr) {
    return thisPtr->m_DieAnimName;
}
void ZombieAnimRig_BasicTemplate::modInit() {
    LOGI("ZombieAnimRig_BasicTemplate init");

    vftable = CopyVFTable(getActualOffset(0x2488950), 85);

    PatchVFTable(vftable, (void*)ZombieAnimRig_BasicTemplate::StaticGetType, 0);

    PatchVFTable(vftable, (void*)ParticleHead, 44);

    PatchVFTable(vftable, (void*)ParticleArm, 49);

    PatchVFTable(vftable, (void*)HeadList, 55);

    PatchVFTable(vftable, (void*)LowerArmList, 56);

    PatchVFTable(vftable, (void*)UpperArmList, 57);

    PatchVFTable(vftable, (void*)Idle, 58);

    PatchVFTable(vftable, (void*)Walk, 59);

    PatchVFTable(vftable, (void*)Eat, 61);

    PatchVFTable(vftable, (void*)Die, 63);

    ZombieAnimRig_BasicTemplate::StaticGetType();
    LOGI("ZombieAnimRig_BasicTemplate finish init");
}