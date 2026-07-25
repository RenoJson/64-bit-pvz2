#pragma once
#include "PvZ2/PlantFramework.h"
#include "Plant.h"
#include "DamageLifetime.h"

class PlantIceBloom : public PlantFramework {
public:
	RT_CLASS_GET_CLASS_WRAPPER(0xF9D7F0);
};

class IceBloomProps : public PlantPropertySheet {
public:
	DamageLifetime IceBlockDamagePhases;
	float IceBlockHealth;
};