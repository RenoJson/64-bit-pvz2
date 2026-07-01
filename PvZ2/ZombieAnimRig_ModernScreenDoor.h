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
	RT_CLASS_GET_CLASS_NO_PARENT_ADDRESS_CLASS_FUNCTION(ZombieAnimRig_ModernScreenDoorAlmanac, ZombieAnimRig_ModernScreenDoor::StaticGetType)
};