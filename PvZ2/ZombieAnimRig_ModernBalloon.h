#pragma once
#include "ZombieAnimRig.h"

class ZombieAnimRig_ModernBalloon : public ZombieAnimRig
{
public:
	bool m_hasBalloon = true;
	static void modInit();
};
static_assert(sizeof(ZombieAnimRig_ModernBalloon) == 672);
static_assert(offsetof(ZombieAnimRig_ModernBalloon, m_hasBalloon) == 664);
