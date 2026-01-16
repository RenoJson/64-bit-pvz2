#pragma once
#include "PvZ2/Zombie.h"

class ZombieBasic : public Zombie
{
public:

	int m_helmDamageIndex;

};
static_assert(offsetof(ZombieBasic, m_helmDamageIndex) == 1096);
static_assert(sizeof(ZombieBasic) == 1104);
