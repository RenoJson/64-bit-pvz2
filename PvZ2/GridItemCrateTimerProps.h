#pragma once
#include"GridItemPropertySheet.h"
#include "DamageTypeFlags.h"
#include "ZombieConditions.h"
#include "PlantConditions.h"
class GridItemAnimationProps : public GridItemPropertySheet
{
public:
	SexyString PopAnim;
	SexyString PopAnimRigClass;
	SexyVector2 PopAnimRenderOffset;
	SexyVector2 PopAnimRenderScale;
	SexyString LinkedPopAnim;
	SexyVector3 LinkedOffset;
	int32_t LinkedLayerOffset;
	char pad[0xc];

};
class GridItemCraterProps : public GridItemAnimationProps
{
public:

};
class GridItemCraterTimedProps : public GridItemCraterProps
{
public:
	char pad[0x4];
	float Lifetime = 5.0f;
};
class DamageInfoProps {
	float BaseDamage;
	std::vector<DamageTypeFlags> DamageFlags;
	struct ValueRange
	{
		float Min;
		float Max;

		ValueRange() : Min(0), Max(0) {};

		ValueRange(float Min, float Max) : Min(Min), Max(Max) {};
	};
	struct ZombieConditionEntry {
		ZombieConditions Conditions;
		ValueRange Durations;
	};
	struct PlantConditionEntry {
		PlantConditions Conditions;
		ValueRange Durations;
	};
	char _pad_tail[40];
};

static_assert(sizeof(GridItemAnimationProps) == 0x0A0);
static_assert(sizeof(GridItemCraterProps) == 0x0A0);
static_assert(sizeof(GridItemCraterTimedProps) == 0x0A8);
//static_assert(sizeof(DamageInfoProps) == 0x038);
