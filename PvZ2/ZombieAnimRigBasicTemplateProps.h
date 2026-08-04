#pragma once
#include "PvZ2/PropertySheetBase.h"
#include <Reflection/ReflectionBuilder.h>

class ZombieAnimRigTemplateConfig : public PropertySheetBase
{
public:
	std::vector<SexyString> LowerArmLayers;
	std::vector<SexyString> UpperArmLayers;
	std::vector<SexyString> HeadLayers;
	SexyString ParticleHeadSpriteName;
	SexyString ParticleArmSpriteName;
	SexyString IdleAnimName;
	SexyString WalkAnimName;
	SexyString EatAnimName;
	SexyString DieAnimName;

	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieAnimRigTemplateConfig, 0x8AE3A8);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(PropertySheetBase);
	RT_CLASS_REGISTER_STRING_VECTOR_PROPERTY(ZombieAnimRigTemplateConfig, LowerArmLayers);
	RT_CLASS_REGISTER_STRING_VECTOR_PROPERTY(ZombieAnimRigTemplateConfig, UpperArmLayers);
	RT_CLASS_REGISTER_STRING_VECTOR_PROPERTY(ZombieAnimRigTemplateConfig, HeadLayers);
	RT_CLASS_REGISTER_STRING_PROPERTY(ZombieAnimRigTemplateConfig, ParticleHeadSpriteName);
	RT_CLASS_REGISTER_STRING_PROPERTY(ZombieAnimRigTemplateConfig, ParticleArmSpriteName);
	RT_CLASS_REGISTER_STRING_PROPERTY(ZombieAnimRigTemplateConfig, IdleAnimName);
	RT_CLASS_REGISTER_STRING_PROPERTY(ZombieAnimRigTemplateConfig, WalkAnimName);
	RT_CLASS_REGISTER_STRING_PROPERTY(ZombieAnimRigTemplateConfig, EatAnimName);
	RT_CLASS_REGISTER_STRING_PROPERTY(ZombieAnimRigTemplateConfig, DieAnimName);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieAnimRigTemplateConfig);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieAnimRigTemplateConfig, 0xD1FB60);
};
