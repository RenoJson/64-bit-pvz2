#pragma once

#include "ZombieImp.h"
class ZombieZcorpRacerZombie : public ZombieImp
{
public:

    RT_CLASS_GET_CLASS_WRAPPER(0xBF1124);
};
class ZombieGum : public Zombie
{
public:
    Sexy::RtWeakPtr<Zombie> m_owner;
    char pad[8];
    RT_CLASS_GET_CLASS_WRAPPER(0xB7EA10);
};