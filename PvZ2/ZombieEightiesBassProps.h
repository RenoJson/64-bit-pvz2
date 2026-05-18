#pragma once
#include "PvZ2/Zombie.h"
#include <Reflection/ReflectionBuilder.h>
#include "Projectile.h"

class ZombieEightiesBassProps : public ZombiePropertySheet
{
public:
	SexyString JamStyle = "jam_metal";
	Sexy::RtWeakPtr<ProjectilePropertySheet> ShockWaveProjectile;
	SexyVector2 ShockWaveSpawnOffset;
	float ShockWaveSpawnInterval = 15.0f;
	float ShockWaveSpawnIntervalWhileJamming = 5.0f;
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieEightiesBassProps, 0xC136A4);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombiePropertySheet);
	RT_CLASS_REGISTER_STRING_PROPERTY(ZombieEightiesBassProps, JamStyle);
	RT_CLASS_REGISTER_CLASS_RTWEAKPTR_PROPERTY(ZombieEightiesBassProps, ShockWaveProjectile, ProjectilePropertySheet)
	RT_CLASS_REGISTER_CLASS_PROPERTY(ZombieEightiesBassProps, SexyVector2, ShockWaveSpawnOffset);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieEightiesBassProps, ShockWaveSpawnInterval);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieEightiesBassProps, ShockWaveSpawnIntervalWhileJamming);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieEightiesBassProps);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieEightiesBassProps, 0xDA5B00);
};