#include "Zombie.h"

void Zombie::SetStatusesFromDamageInfo(DamageInfo* dmgInfo)
{
	CallFunc<void, Zombie*, DamageInfo*>(0xC431BC, this, dmgInfo);
}
