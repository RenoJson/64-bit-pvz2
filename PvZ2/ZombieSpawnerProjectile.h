#pragma once
#include "Projectile.h"
#include <Reflection/ReflectionBuilder.h>
using namespace Sexy;
class ZombieSpawnerProjectile : public Projectile {
public:
	DECLARE_STATIC_RT_CLASS_MEMBERS(ZombieSpawnerProjectile)

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieSpawnerProjectile, 0xC9CA4C);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(Projectile);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieSpawnerProjectile);

	RT_CLASS_GET_CLASS_FUNCTION(ZombieSpawnerProjectile, 0xC9B180);
};

class ZombieSpawnerProjectileProps : public ProjectilePropertySheet {
public:
	SexyString ZombieTypeName = "tutorial";
	bool RiseFromGround = false;
	DECLARE_STATIC_RT_CLASS_MEMBERS(ZombieSpawnerProjectileProps)

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieSpawnerProjectileProps, 0x5FDE7C);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ProjectilePropertySheet);
	RT_CLASS_REGISTER_STRING_PROPERTY(ZombieSpawnerProjectileProps, ZombieTypeName);
	RT_CLASS_REGISTER_STRING_PROPERTY(ZombieSpawnerProjectileProps, RiseFromGround);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieSpawnerProjectileProps);

	RT_CLASS_GET_CLASS_FUNCTION(ZombieSpawnerProjectileProps, 0x67AEC0);
};

