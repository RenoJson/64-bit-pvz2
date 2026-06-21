#pragma once
#include "PvZ2/Zombie.h"
#include <Reflection/ReflectionBuilder.h>

class ZombieMausoleumVendorProps : public ZombiePropertySheet
{
public:
	pvztime_t SetPigInterval = 6.0f;
	int NumberOfPigToSpawnWhenDie = 4;
	SexyString PigType = "western_piggy";
	SexyVector3 PigSpawnOffset = { 60.0f, 0.0f, 50.0f };
	float TunnelSpeed = 0.22f;
	float TunnelSpeedScale = 2.0f;

	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieMausoleumVendorProps, 0xC136A4);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombiePropertySheet);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieMausoleumVendorProps, SetPigInterval);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieMausoleumVendorProps, NumberOfPigToSpawnWhenDie);
	RT_CLASS_REGISTER_STRING_PROPERTY(ZombieMausoleumVendorProps, PigType);
	RT_CLASS_REGISTER_CLASS_PROPERTY(ZombieMausoleumVendorProps, SexyVector3, PigSpawnOffset);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieMausoleumVendorProps, TunnelSpeed);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieMausoleumVendorProps, TunnelSpeedScale);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieMausoleumVendorProps);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieMausoleumVendorProps, 0xDA5B00);
};