#include "ZombieAnimRig_TombRaiser.h"
#include "ZombieTombRaiser.h"

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


