#pragma once
#include "ZombieAnimRig.h"

class ZombieAnimRig_Bull : public ZombieAnimRig
{
public:
	bool m_running;
};
static_assert(sizeof(ZombieAnimRig_Bull) == 672);
static_assert(offsetof(ZombieAnimRig_Bull, m_running) == 664);
