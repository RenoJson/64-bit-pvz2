#pragma once
#include "PvZ2/Zombie.h"
#include <Reflection/ReflectionBuilder.h>

class ZombieBull : public Zombie 
{
public:
    char pad_1096[1];
    bool m_hitEntity;
    bool m_walkCycled;
    char pad_align[1];
    int m_damageState;
};

static_assert(sizeof(ZombieBull) == 1104, "Size ZombieBull sai!");
static_assert(offsetof(ZombieBull, m_hitEntity) == 1097, "Offset m_hitEntity Fail");
static_assert(offsetof(ZombieBull, m_walkCycled) == 1098, "Offset m_walkCycled Fail");
static_assert(offsetof(ZombieBull, m_damageState) == 1100, "Offset m_damageState Fail");