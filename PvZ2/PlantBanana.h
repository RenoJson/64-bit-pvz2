#pragma once
#include "PlantFramework.h"
class PlantBanana : public PlantFramework
{
	bool m_onlyTargetZombies;
	bool m_fireIgnoreBoardState;
	char _pad_to_16[6];
};
static_assert(sizeof(PlantBanana) == 0x10);