#pragma once
#include "Zombie.h"
#include "ZombieAnimRigBasicTemplateProps.h"

class ZombieTypeTemplate : public ZombieType
{
public:
	Sexy::RtWeakPtr<ZombieAnimRigTemplateConfig> AnimRigProps;
	SexyVector2 HeadEffectOffsetAdjust = { 0.0f, 0.0f };
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieTypeTemplate, 0x10680BC);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombieType);
	RT_CLASS_REGISTER_CLASS_RTWEAKPTR_PROPERTY(ZombieTypeTemplate, AnimRigProps, ZombieAnimRigTemplateConfig)
	RT_CLASS_REGISTER_CLASS_PROPERTY(ZombieTypeTemplate, SexyVector2, HeadEffectOffsetAdjust)
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieTypeTemplate);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieTypeTemplate, 0x1067CBC);
};