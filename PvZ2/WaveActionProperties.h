#pragma once
#include "PropertySheetBase.h"
#include <Reflection/ReflectionBuilder.h>

class WaveActionProperties : public PropertySheetBase {
public:
	std::vector<SexyString> ResourceGroupNames;
	std::vector<SexyString> NotificationEvents;
};
class ZombieSpawnerActionProps: public WaveActionProperties {
public:

};
class ZombieRainSpawnerProps : public ZombieSpawnerActionProps {
public:
	int GroupSize = 2;
	int SpiderCount = 2;
	float TimeBetweenGroups = 0.5f;
	float TimeBeforeFullSpawn = 3.0f;
	float ZombieFallTime = 2.0f;
	int ColumnStart = 5;
	int ColumnEnd = 7;
	SexyString WaveStartMessage = "[WARNING_SPIDERRAIN]";
	SexyString SpiderZombieName = "future_imp";
};
class SpiderRainZombieSpawnerProps : public ZombieRainSpawnerProps {
public:
	RT_CLASS_GET_CLASS_WRAPPER(0x91F9FC);
};
class ParachuteRainZombieSpawnerProps : public ZombieRainSpawnerProps {
public:
	RT_CLASS_GET_CLASS_WRAPPER(0x110BE64);
};