#pragma once
#include "ZombieAnimRig.h"
#include <Reflection/ReflectionBuilder.h>

class ZombieAnimRig_ModernAllStar : public ZombieAnimRig
{
public:
	static void modInit();
};

class ZombieAnimRig_JourneyToTheWestAllStar : public ZombieAnimRig_ModernAllStar
{
public:
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieAnimRig_JourneyToTheWestAllStar, 0x9D9C4C);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombieAnimRig_ModernAllStar);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieAnimRig_JourneyToTheWestAllStar);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieAnimRig_JourneyToTheWestAllStar, 0xBBA134);
};