#pragma once
#include "ZombieAnimRig.h"
#include <Reflection/ReflectionBuilder.h>

class ZombieAnimRig_ModernScreenDoor : public ZombieAnimRig
{
public:
	bool m_hasDoor = true;
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieAnimRig_ModernScreenDoor, 0x9D9C4C);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombieAnimRig);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieAnimRig_ModernScreenDoor, m_hasDoor);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieAnimRig_ModernScreenDoor);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieAnimRig_ModernScreenDoor, 0x8DBB70);
};

class ZombieAnimRig_ModernScreenDoorAlmanac : public ZombieAnimRig_ModernScreenDoor
{
public:
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieAnimRig_ModernScreenDoorAlmanac, 0x9D9C4C);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombieAnimRig_ModernScreenDoor);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieAnimRig_ModernScreenDoorAlmanac);
	static Sexy::RtClass* StaticGetType() {
		if (s_rtClass) {
			return s_rtClass;
		}
		typedef Sexy::RtClass* (*initRtClass)();
		Sexy::RtClass* rtClass = ((initRtClass)getActualOffset(0x163A068))();
		s_rtClass = rtClass;
		Sexy::RtClass* parent = ZombieAnimRig_ModernScreenDoor::StaticGetType();
		typedef uintptr_t(*rtClassRegisterClass)(void*, const char*, Sexy::RtClass*, ParameterlessConstructorFunc);
		rtClassRegisterClass regrtclass = *(rtClassRegisterClass*)(*(uintptr_t*)rtClass + 0x40);
		uintptr_t registeredClassResult = regrtclass(rtClass, "ZombieAnimRig_ModernScreenDoorAlmanac", parent, ZombieAnimRig_ModernScreenDoorAlmanac::Construct);
		ZombieAnimRig_ModernScreenDoorAlmanac::RegisterClass();
		return s_rtClass;
	};
};