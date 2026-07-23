#include "Zombie.h"

void Zombie::SetStatusesFromDamageInfo(DamageInfo* dmgInfo)
{
	CallFunc<void, Zombie*, DamageInfo*>(0xC431BC, this, dmgInfo);
}

bool Zombie::IsInGridItem()
{
	bool conds = this->m_conditionTracker.m_conditionFlags[zombie_condition_icecubed] 
		|| this->m_conditionTracker.m_conditionFlags[zombie_condition_present_boxed] 
		|| this->m_conditionTracker.m_conditionFlags[zombie_condition_stoneblocked];
	return conds;
}
