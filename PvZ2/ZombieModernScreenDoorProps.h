#pragma once
#include "PvZ2/Zombie.h"
#include <Reflection/ReflectionBuilder.h>
#include "Projectile.h"

class ZombieModernScreenDoorProps : public ZombiePropertySheet
{
public:
	std::vector<Sexy::RtWeakPtr<ProjectilePropertySheet>> PierceableProjectiles;
	std::vector<SexyString> PlantBlacklist;
	Rect AttackRectWhenHaveDoor;
	Rect HitRectWhenHaveDoor;
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieModernScreenDoorProps, 0xC136A4);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombiePropertySheet);
	RT_CLASS_REGISTER_RTWEAKPTR_VECTOR_PROPERTY(ZombieModernScreenDoorProps, PierceableProjectiles, ProjectilePropertySheet);
	RT_CLASS_REGISTER_STRING_VECTOR_PROPERTY(ZombieModernScreenDoorProps, PlantBlacklist);
	RT_CLASS_REGISTER_CLASS_PROPERTY(ZombieModernScreenDoorProps, Rect, AttackRectWhenHaveDoor);
	RT_CLASS_REGISTER_CLASS_PROPERTY(ZombieModernScreenDoorProps, Rect, HitRectWhenHaveDoor);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieModernScreenDoorProps);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieModernScreenDoorProps, 0xDA5B00);
};