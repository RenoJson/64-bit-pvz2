#pragma once
#include "GameObject.h"

// Base class for all Property Sheet objects
// Derive from it if you are creating new JSON props
class PropertySheetBase : public GameObject
{
public:
	// This property is only used in plants, zombies and gis. You can just skip it
	std::vector<int> ScaledProps; // BoardEntityScaledPropsEntry

	virtual void Function13() {};
};

static_assert(sizeof(PropertySheetBase) == 40);
static_assert(offsetof(PropertySheetBase, ScaledProps) == 16);