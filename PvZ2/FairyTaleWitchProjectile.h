#pragma once
#include "Projectile.h"
#include <Reflection/ReflectionBuilder.h>
using namespace Sexy;
class FrogProjectile : public Projectile {
public:
	DECLARE_STATIC_RT_CLASS_MEMBERS(FrogProjectile)

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(FrogProjectile, 0xC9CA4C);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(Projectile);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(FrogProjectile);

	RT_CLASS_GET_CLASS_FUNCTION(FrogProjectile, 0xC9B180);
};