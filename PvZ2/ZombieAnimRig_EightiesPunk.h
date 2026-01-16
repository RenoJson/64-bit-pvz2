#pragma once
#include "ZombieAnimRig.h"
#include <Reflection/ReflectionBuilder.h>

class ZombieAnimRig_EightiesPunk : public ZombieAnimRig
{
public:
	bool m_isJamming;
	char pad[7];
	static void modInit();
};