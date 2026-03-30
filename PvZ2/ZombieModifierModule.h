#pragma once
#include "LevelModule.h"
#include "Reflection/CRefManualSymbolBuilder.h"
#include <Reflection/ReflectionBuilder.h>

class ZombieModifierModule : public LevelModule {
public:

};
class ZombieModifierProperties : public LevelModuleProperties {
public:
	bool Speedy = false;
	SexyString ModifierType = "stat";
	std::vector<SexyString> ConditionToApply;
	float ConditionLifeTime = -1.0f;
	float SpeedScale = 1.0f;
	float DPSScale = 1.0f;
	float DamageScale = 1.0f;
	float HitpointsScale = 1.0f;
	static Reflection::CRefManualSymbolBuilder::BuildSymbolsFunc oZombieModifierPropertiesBuildSymbols;
	static void modInit();

	static void* construct()
	{
		auto* props = new ZombieModifierProperties();

		typedef void* (*ctorWithThisPtr)(void*);
		ctorWithThisPtr baseCtor = (ctorWithThisPtr)getActualOffset(0x5F5AD8);
		baseCtor(props);

		*reinterpret_cast<uintptr_t*>(props) = getActualOffset(0x24D44C0);
		return props;
	}

	static void buildSymbols(Reflection::CRefManualSymbolBuilder* builder, Reflection::RClass* rclass)
	{
		oZombieModifierPropertiesBuildSymbols(builder, rclass);
		RT_CLASS_REGISTER_STRING_PROPERTY(ZombieModifierProperties, ModifierType);
		RT_CLASS_REGISTER_STRING_VECTOR_PROPERTY(ZombieModifierProperties, ConditionToApply);
		RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieModifierProperties, SpeedScale);
		RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieModifierProperties, DPSScale);
		RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieModifierProperties, DamageScale);
		RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieModifierProperties, HitpointsScale);
	};
};
