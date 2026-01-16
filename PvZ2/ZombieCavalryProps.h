#pragma once
#include "Zombie_BullProps.h"

class ZombieDarkCavalryProps : public ZombieBullProps
{

public: 
	SexyString LanceType = "joustsword";
	SexyString KnightType = "dark_cavalry_rider";

	static void* vftable; 
	static Sexy::RtClass* s_rtClass; 
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieDarkCavalryProps, 0xC136A4);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombieBullProps);
	RT_CLASS_REGISTER_STRING_PROPERTY(ZombieDarkCavalryProps, KnightType);
	RT_CLASS_REGISTER_STRING_PROPERTY(ZombieDarkCavalryProps, LanceType);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieDarkCavalryProps, LaunchDistance);
	
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieDarkCavalryProps);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieDarkCavalryProps, 0xDAE0FC);
};
