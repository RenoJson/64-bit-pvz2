#pragma once
#include "Zombie.h"
#include <SexyTypes.h>
#include "Reflection/CRefManualSymbolBuilder.h"
#include <Reflection/ReflectionBuilder.h>
class ZombieTombRaiserProps : public ZombiePropertySheet
{
public:
	float TimeBetweenCast = 0.0f;
	float TimeBetweenRaisings = 6.0f;
	int NumberOfTombsToSpawn = 2;
	Sexy::RtWeakPtr<Sexy::RtObject> Projectile;
};
