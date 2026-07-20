#pragma once
#include <Reflection/ReflectionBuilder.h>
#include "Projectile.h"

class TombraiserProjectileProps : public ProjectilePropertySheet
{
public:
	SexyString GridItemType = "gravestone_egypt";
	SexyString PopAnimEffect = "POPANIM_EFFECTS_ZOMBIE_EGYPT_TOMBRAISER_BONE_HIT";
	SexyString AnimationName = "animation";
	SexyString EffectEventName = "Play_Zomb_Egypt_TombRaiser_Grave_Rise";

	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;


	RT_CLASS_CONSTRUCT_FUNCTION(TombraiserProjectileProps, 0x5FDE7C);

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ProjectilePropertySheet);
	RT_CLASS_REGISTER_STRING_PROPERTY(TombraiserProjectileProps, GridItemType);
	RT_CLASS_REGISTER_STRING_PROPERTY(TombraiserProjectileProps, PopAnimEffect);
	RT_CLASS_REGISTER_STRING_PROPERTY(TombraiserProjectileProps, AnimationName);
	RT_CLASS_REGISTER_STRING_PROPERTY(TombraiserProjectileProps, EffectEventName);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(TombraiserProjectileProps);

	RT_CLASS_GET_CLASS_FUNCTION(TombraiserProjectileProps, 0x67AEC0);
};