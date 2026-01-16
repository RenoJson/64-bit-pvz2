#pragma once
#include "GameObject.h"
#include "PropertySheetBase.h"

class LevelModuleProperties;

class LevelModule : public GameObject
{
public:
	Sexy::RtWeakPtr<LevelModuleProperties> m_propertySheet;
};

static_assert(sizeof(LevelModule) == 24);
static_assert(offsetof(LevelModule, m_propertySheet) == 16);

class LevelModuleProperties : public PropertySheetBase
{
public:
	std::vector<SexyString> ResourceGroupNames;
	Sexy::RtWeakPtr<void> DifficultyProps;
	SexyString IconImage;
	SexyString IconText;
	char m_pad[8];
};

static_assert(sizeof(LevelModuleProperties) == 128);
static_assert(offsetof(LevelModuleProperties, ResourceGroupNames) == 40);
static_assert(offsetof(LevelModuleProperties, DifficultyProps) == 64);
static_assert(offsetof(LevelModuleProperties, IconText) == 96);