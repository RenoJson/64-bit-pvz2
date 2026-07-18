#include "ZombieAnimRig_TombRaiser.h"
#include "ZombieTombRaiser.h"

void* ZombieAnimRig_MysticFormation::vftable = nullptr;
Sexy::RtClass* ZombieAnimRig_MysticFormation::s_rtClass = nullptr;

uint64_t ZombieAnimRig_MysticFormation::OnAnimCommand(ZombieAnimRig_MysticFormation* thisPtr, const SexyString& actionName)
{
    if (actionName == "throw")
    {
        auto owner = reinterpret_cast<ZombieMysticFormation*>(thisPtr->m_onThrow.m_ownerPtr.Get());
        if (owner != nullptr)
        {
          CallFunc<void, RtReflectionDelegateBase*>(0x5C967C, &thisPtr->m_onThrow);
          using ExecuteFuncType = uint64_t(*)(DelegateBase*);
          ExecuteFuncType executeFunc = reinterpret_cast<ExecuteFuncType>(thisPtr->m_onThrow.m_delegate.m_executeCallbackFunction);
          return executeFunc(&thisPtr->m_onThrow.m_delegate);
        }
    }
    return 0;
}

std::vector<SexyString>* ZombieAnimRig_MysticFormation::lowerArmList()
{
    static std::vector<SexyString> ModernPogoHeadList = {
       "zombie_arm_outer_lower",
       "zombie_hand_outer_01",
       "zombie_hand_outer_02",
       "zombie_hand_outer_04"
    };
    return &ModernPogoHeadList; 
}

std::vector<SexyString>* ZombieAnimRig_MysticFormation::upperArmList()
{
    static std::vector<SexyString> ModernPogoHeadList = {
        "zombie_arm_outer_upper",
        "zombie_arm_outer_upper_bone"
    };
    return &ModernPogoHeadList;
}

std::vector<SexyString>* ZombieAnimRig_MysticFormation::headList()
{
    static std::vector<SexyString> ModernPogoHeadList = {
       "zombie_skull",
       "zombie_jaw"
    };
    return &ModernPogoHeadList;
}

bool ZombieAnimRig_MysticFormation::OnInitializeAnimRigDelegate(ZombieAnimRig_MysticFormation* thisPtr, RtReflectionDelegateBase* dlgt)
{
	thisPtr->m_state = 6;
    if (dlgt != nullptr)
    {
        thisPtr->m_onThrow = *dlgt;
    }
	return true;
}

void ZombieAnimRig_MysticFormation::modInit() {
    LOGI("ZombieAnimRig_MysticFormation init");

    vftable = CreateChildVFTable(67 + 1, getActualOffset(0x23ABF70), 67);
    PatchVFTable(vftable, (void*)ZombieAnimRig_MysticFormation::StaticGetType, 0);
    PatchVFTable(vftable, (void*)ZombieAnimRig_MysticFormation::OnAnimCommand, 25);
    PatchVFTable(vftable, (void*)ZombieAnimRig_MysticFormation::headList, 55);
    PatchVFTable(vftable, (void*)ZombieAnimRig_MysticFormation::lowerArmList, 56);
    PatchVFTable(vftable, (void*)ZombieAnimRig_MysticFormation::upperArmList, 57);
    PatchVFTable(vftable, (void*)ZombieAnimRig_MysticFormation::OnInitializeAnimRigDelegate, 67);

    ZombieAnimRig_MysticFormation::StaticGetType();

    LOGI("ZombieAnimRig_MysticFormation finish init");
}

