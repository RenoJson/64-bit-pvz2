#include "ZombieAnimRig_Basic.h"

void* ZombieAnimRig_BasicTemplate::vftable = __null;
Sexy::RtClass* ZombieAnimRig_BasicTemplate::s_rtClass = __null;;

SexyString ParticleHead(ZombieAnimRig_BasicTemplate* thisPtr) {
    auto rigProps = reinterpret_cast<ZombieAnimRigTemplateConfig*>(thisPtr->m_rigProps.Get());
    if (rigProps == nullptr) return "";
    return rigProps->ParticleHeadSpriteName;
}

SexyString ParticleArm(ZombieAnimRig_BasicTemplate* thisPtr) {
    auto rigProps = reinterpret_cast<ZombieAnimRigTemplateConfig*>(thisPtr->m_rigProps.Get());
    if (rigProps == nullptr) return "";
    return rigProps->ParticleArmSpriteName;
}

SexyString Idle(ZombieAnimRig_BasicTemplate* thisPtr) {
    auto rigProps = reinterpret_cast<ZombieAnimRigTemplateConfig*>(thisPtr->m_rigProps.Get());
    if (rigProps == nullptr) return "";
    return rigProps->IdleAnimName;
}

SexyString Walk(ZombieAnimRig_BasicTemplate* thisPtr) {
    auto rigProps = reinterpret_cast<ZombieAnimRigTemplateConfig*>(thisPtr->m_rigProps.Get());
    if (rigProps == nullptr) return "";

    if (thisPtr->m_dancing) {
        return "dance_start";
    }
    return rigProps->WalkAnimName;
}

SexyString Eat(ZombieAnimRig_BasicTemplate* thisPtr) {
    auto rigProps = reinterpret_cast<ZombieAnimRigTemplateConfig*>(thisPtr->m_rigProps.Get());
    if (rigProps == nullptr) return "";
    return rigProps->EatAnimName;
}

SexyString Die(ZombieAnimRig_BasicTemplate* thisPtr) {
    auto rigProps = reinterpret_cast<ZombieAnimRigTemplateConfig*>(thisPtr->m_rigProps.Get());
    if (rigProps == nullptr) return "";
    return rigProps->DieAnimName;
}

std::vector<SexyString>* HeadList(ZombieAnimRig_BasicTemplate* thisPtr) {
    auto rigProps = reinterpret_cast<ZombieAnimRigTemplateConfig*>(thisPtr->m_rigProps.Get());
    if (rigProps == nullptr) {
        static std::vector<SexyString> emptyList; 
        return &emptyList;
    }
    return &rigProps->HeadLayers;
}

std::vector<SexyString>* LowerArmList(ZombieAnimRig_BasicTemplate* thisPtr) {
    auto rigProps = reinterpret_cast<ZombieAnimRigTemplateConfig*>(thisPtr->m_rigProps.Get());
    if (rigProps == nullptr) {
        static std::vector<SexyString> emptyList;
        return &emptyList;
    }
    return &rigProps->LowerArmLayers;
}

std::vector<SexyString>* UpperArmList(ZombieAnimRig_BasicTemplate* thisPtr) {
    auto rigProps = reinterpret_cast<ZombieAnimRigTemplateConfig*>(thisPtr->m_rigProps.Get());
    if (rigProps == nullptr) {
        static std::vector<SexyString> emptyList;
        return &emptyList;
    }
    return &rigProps->UpperArmLayers;
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