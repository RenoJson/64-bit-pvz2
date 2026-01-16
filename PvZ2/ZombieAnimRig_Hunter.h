#pragma once
#include "ZombieAnimRig.h"
#include <Reflection/ReflectionBuilder.h>

class ZombieAnimRig_Hunter : public ZombieAnimRig
{
public:
	static void modInit();
};

class ZombieAnimRig_JourneyToTheWestTrident : public ZombieAnimRig_Hunter
{
public:
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieAnimRig_JourneyToTheWestTrident, 0x9D9C4C);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombieAnimRig_Hunter);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieAnimRig_JourneyToTheWestTrident);
	static Sexy::RtClass* StaticGetType() {
		if (s_rtClass) 
			return s_rtClass; 
		typedef Sexy::RtClass* (*initRtClass)(); 
		Sexy::RtClass* rtClass = ((initRtClass)getActualOffset(0x163A068))(); 
		s_rtClass = rtClass; 
		rtClassGetClassFunc parentGetType = (rtClassGetClassFunc)getActualOffset(0xB8CB8C);
		uintptr_t* parent = (uintptr_t*)parentGetType(); 
		typedef uintptr_t(*rtClassRegisterClass)(void*, const char*, uintptr_t, ParameterlessConstructorFunc); 
		rtClassRegisterClass regrtclass = *(rtClassRegisterClass*)(*(uintptr_t*)rtClass + 0x40); 
		regrtclass(rtClass, "ZombieAnimRig_JourneyToTheWestTrident", (uintptr_t)parent, ZombieAnimRig_JourneyToTheWestTrident::Construct); 
		return s_rtClass;
	};
};