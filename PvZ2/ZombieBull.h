#pragma once
#include "PvZ2/Zombie.h"
#include <Reflection/ReflectionBuilder.h>

class ZombieBull : public Zombie 
{
public:
    bool m_hasLaunched; // unregistered bool
    bool m_hitEntity;
    bool m_walkCycled;
    char pad_align[1];
    int m_damageState;
};

static_assert(sizeof(ZombieBull) == 1104);
static_assert(offsetof(ZombieBull, m_hasLaunched) == 1096);
static_assert(offsetof(ZombieBull, m_hitEntity) == 1097);
static_assert(offsetof(ZombieBull, m_walkCycled) == 1098);
static_assert(offsetof(ZombieBull, m_damageState) == 1100);