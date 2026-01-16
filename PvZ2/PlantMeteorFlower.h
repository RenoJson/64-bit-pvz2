#pragma once
#include "PlantBanana.h"
#include "ComponentWarmingBase.h"
class PlantMeteorFlower : public PlantBanana
{
public: 
	Sexy::RtWeakPtr<ComponentWarmingRadius> m_warmingRadius;
	static void* vftable; 
	static Sexy::RtClass* s_rtClass; 
	static void modInit();;
	
	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(PlantMeteorFlower, PLANTFRAMEWORK_CTOR);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(PlantMeteorFlower, PlantBanana);
	REGISTER_CLASS_RTWEAKPTR_PROPERTY(builder, rclass, PlantMeteorFlower, m_warmingRadius, ComponentWarmingRadius);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(PlantMeteorFlower);
	RT_CLASS_GET_CLASS_FUNCTION(PlantMeteorFlower, 0xA514BC);
};