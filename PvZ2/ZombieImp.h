#pragma once
#include "BoardEntity.h"

#include "Plant.h"
#include "PvZ2/Zombie.h"


class ZombieImp : public Zombie
{
public:
    // weird inherit from Zombie Basic in 64 bit instead of Zombie in 32 bit
    // so m_helmDamageIndex will keep the imp class right in struct
    int m_helmDamageIndex; 
    bool m_invincibleWhileInAir;
    bool m_getsUpFromLanding;
    float m_fallVelocity;
    bool m_animFailed;
    char pad[3];
};
static_assert(sizeof(ZombieImp) == 1112);
static_assert(offsetof(ZombieImp, m_invincibleWhileInAir) == 1100);
static_assert(offsetof(ZombieImp, m_getsUpFromLanding) == 1101);
static_assert(offsetof(ZombieImp, m_fallVelocity) == 1104);
static_assert(offsetof(ZombieImp, m_animFailed) == 1108);
