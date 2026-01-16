#pragma once
#include "PlantBananaProps.h"
#include "ComponentWarmingBase.h"
class MeteorFlowerProps : public BananaProps
{
public: 
	ComponentWarmingRadiusProps WarmingRadius;
	static void* vftable; 
	static Sexy::RtClass* s_rtClass; 
	static void modInit();

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(MeteorFlowerProps, PLANTPROPS_CTOR);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(MeteorFlowerProps, BananaProps);
	REGISTER_CLASS_PROPERTY(builder, rclass, MeteorFlowerProps, ComponentWarmingRadiusProps, WarmingRadius);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(MeteorFlowerProps);
	RT_CLASS_GET_CLASS_FUNCTION(MeteorFlowerProps, 0x2BEE78);
};