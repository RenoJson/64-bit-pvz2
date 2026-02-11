#pragma once
#include "PvZ2/Zombie.h"
#include <Reflection/ReflectionBuilder.h>
#include "StringRestrictionSet.h"
class ZombieBugProps : public ZombiePropertySheet
{
public:
	float DamageFraction;
	PlantRestrictionSet PlantWhichAlsoKillBasic;
	float DamageAmountWhichAlsoKillBasic;
	char pad[24];
};
static_assert(offsetof(ZombieBugProps, DamageFraction) == 568);
static_assert(offsetof(ZombieBugProps, PlantWhichAlsoKillBasic) == 576);
static_assert(offsetof(ZombieBugProps, DamageAmountWhichAlsoKillBasic) == 616);
static_assert(sizeof(ZombieBugProps) == 648);