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
static_assert(offsetof(ZombieLostCityBug, m_hasTakenCatastrophicDamage) == 1100);
static_assert(sizeof(ZombieLostCityBug) == 1104);
class ZombieAnimRig_LostCityBug : public ZombieAnimRig
{
public:
    int32_t m_basicHelm_unk; // basic helm enum
    bool m_hasTakenCatastrophicDamage;
};
