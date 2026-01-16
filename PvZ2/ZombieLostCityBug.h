#pragma once
#include "PvZ2/Zombie.h"
class ZombieLostCityBug : public Zombie
{
public:
    int32_t m_basicHelm_unk; // basic helm enum
    bool m_hasTakenCatastrophicDamage;
    bool m_shrunkenWhenDying;
    char _pad_align[2];
};
static_assert(sizeof(ZombieLostCityBug) == 1104);
