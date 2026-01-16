#pragma once
#include "Sexy/RtObject.h"
#include "LevelModule.h"

class LevelModuleManager : public Sexy::RtObject
{
public:
	std::vector<Sexy::RtWeakPtr<LevelModule> > m_modules;
	std::vector<SexyString> m_loadedResourceGroups;
	char m_pad[640];
};

static_assert(sizeof(LevelModuleManager) == 696);
static_assert(offsetof(LevelModuleManager, m_modules) == 8);
static_assert(offsetof(LevelModuleManager, m_loadedResourceGroups) == 32);