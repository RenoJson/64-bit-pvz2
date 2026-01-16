#pragma once
#include <Sexy/RtObject.h>

enum RestrictionSetType
{
	excludelist = 0,
	includelist = 1
};

class StringRestrictionSet : public Sexy::RtObject
{
public:
	RestrictionSetType ListType;
	std::vector<SexyString> List;
};

static_assert(sizeof(StringRestrictionSet) == 40);
static_assert(offsetof(StringRestrictionSet, ListType) == 8);
static_assert(offsetof(StringRestrictionSet, List) == 16);

enum PlantingReason
{
	PLANTING_NOT_ON_ICEBLOCKS = 15,
	PLANTING_NOT_ON_CRATERS = 16,
	PLANTING_NOT_ON_ARCADE_MACHINE = 20,
	PLANTING_NOT_ON_HEALER_STAFF = 48
};

class GridItemRestrictionSet : public StringRestrictionSet
{};

class ZombieRestrictionSet : public StringRestrictionSet
{};

class BoardEntityRestrictionSet : public StringRestrictionSet
{};

class PlantRestrictionSet : public StringRestrictionSet
{};

class PlantingRestrictionSet : public StringRestrictionSet
{
public:
	PlantingReason BlockedPlantingReason;
};

static_assert(sizeof(PlantingRestrictionSet) == 48);
static_assert(offsetof(PlantingRestrictionSet, BlockedPlantingReason) == 40);