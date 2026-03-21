#pragma once
#include "ZombieAnimRig_Basic.h"
class ZombieAnimRig_Octopus : public ZombieAnimRig_Basic {
public:

};
class ZombieAnimRig_FairyTaleWitch : public ZombieAnimRig_Octopus {
public:
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieAnimRig_FairyTaleWitch, 0x9D9C4C);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombieAnimRig_Octopus);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieAnimRig_FairyTaleWitch);
	static Sexy::RtClass* StaticGetType() {
		if (s_rtClass) return s_rtClass; 
		typedef Sexy::RtClass* (*initRtClass)(); 
		Sexy::RtClass* rtClass = ((initRtClass)getActualOffset(0x163A068))(); 
		s_rtClass = rtClass; rtClassGetClassFunc parentGetType = (rtClassGetClassFunc)getActualOffset(0xACC73C); 
		uintptr_t* parent = (uintptr_t*)parentGetType(); 
		typedef uintptr_t(*rtClassRegisterClass)(void*, const char*, uintptr_t, ParameterlessConstructorFunc); 
		rtClassRegisterClass regrtclass = *(rtClassRegisterClass*)(*(uintptr_t*)rtClass + 0x40); 
		regrtclass(rtClass, "ZombieAnimRig_FairyTaleWitch", (uintptr_t)parent, ZombieAnimRig_FairyTaleWitch::Construct); 
		return s_rtClass;
	};
};