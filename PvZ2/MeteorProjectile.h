#pragma once
#include "Projectile.h"
class MeteorProjectile : public Projectile
{
	DECLARE_STATIC_RT_CLASS_MEMBERS(MeteorProjectile);

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(MeteorProjectile, 0x907508);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(MeteorProjectile, Projectile);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(MeteorProjectile);
	RT_CLASS_GET_CLASS_FUNCTION(MeteorProjectile, 0x9059B0);
};