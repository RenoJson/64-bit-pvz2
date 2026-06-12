#pragma once
#include "Zombie_BullProps.h"

class ZombieDarkCavalryProps : public ZombieBullProps
{
public: 
	bool Stampede = false;
	float LanceHitpoints = 900.0f;
	Rect LanceRect;
	Rect RiderAttackRect;
	float RiderAttackDamage = 1000.0f;
	SexyString LanceType = "joustsword";

	static void* vftable; 
	static Sexy::RtClass* s_rtClass; 
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieDarkCavalryProps, 0xC136A4);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombieBullProps);
	RT_CLASS_REGISTER_STRING_PROPERTY(ZombieDarkCavalryProps, RiderType);
	RT_CLASS_REGISTER_STRING_VECTOR_PROPERTY(ZombieDarkCavalryProps, RiderLayersToHide);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieDarkCavalryProps, LaunchDistance);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieDarkCavalryProps, LaunchHeight);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieDarkCavalryProps, LaunchAirTime);
	RT_CLASS_REGISTER_CLASS_PROPERTY(ZombieDarkCavalryProps, SexyVector3, RiderSpawnOffset);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieDarkCavalryProps, isCavalry);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieDarkCavalryProps, Stampede);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieDarkCavalryProps, LanceHitpoints);
	RT_CLASS_REGISTER_CLASS_PROPERTY(ZombieDarkCavalryProps, Rect, LanceRect);
	RT_CLASS_REGISTER_CLASS_PROPERTY(ZombieDarkCavalryProps, Rect, RiderAttackRect);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieDarkCavalryProps, RiderAttackDamage);
	RT_CLASS_REGISTER_STRING_PROPERTY(ZombieDarkCavalryProps, LanceType);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieDarkCavalryProps);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieDarkCavalryProps, 0xDAE0FC);
};
