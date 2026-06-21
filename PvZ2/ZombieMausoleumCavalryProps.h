#pragma once
#include "PvZ2/Zombie.h"
#include <Reflection/ReflectionBuilder.h>

class ZombieMausoleumCavalryProps : public ZombiePropertySheet
{
public:
	float TunnelSpeed = 0.45f;
	float TunnelSpeedScale = 2.0f;
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieMausoleumCavalryProps, 0xC136A4);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombiePropertySheet);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieMausoleumCavalryProps, TunnelSpeed);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieMausoleumCavalryProps, TunnelSpeedScale);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieMausoleumCavalryProps);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieMausoleumCavalryProps, 0xDA5B00);
};