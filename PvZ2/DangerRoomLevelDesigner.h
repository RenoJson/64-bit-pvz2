#pragma once
#include "PropertySheetBase.h"
#include <Reflection/ReflectionBuilder.h>

class DangerRoomLevelDesigner : public PropertySheetBase
{
public:

};
static_assert(sizeof(DangerRoomLevelDesigner) == 40);

class DangerRoomSpiderRainDesigner : public DangerRoomLevelDesigner
{
public:
	int BaseSpiderRainCount = 0;
	int MaxSpiderRainCount = 0;
	float BaseSpiderRainCountPerLevel = 0.5f;
	int BaseSpiderRainZombieCount = 3;
	int MaxSpiderRainZombies = 6;
	float BaseSpiderRainZombiesCountPerLevel = 0.4f;
	int BaseSpiderRainStartWave = 5;
};
static_assert(sizeof(DangerRoomSpiderRainDesigner) == 72);
static_assert(offsetof(DangerRoomSpiderRainDesigner, BaseSpiderRainCount) == 40);
static_assert(offsetof(DangerRoomSpiderRainDesigner, MaxSpiderRainCount) == 44);
static_assert(offsetof(DangerRoomSpiderRainDesigner, BaseSpiderRainCountPerLevel) == 48);
static_assert(offsetof(DangerRoomSpiderRainDesigner, BaseSpiderRainZombieCount) == 52);
static_assert(offsetof(DangerRoomSpiderRainDesigner, MaxSpiderRainZombies) == 56);
static_assert(offsetof(DangerRoomSpiderRainDesigner, BaseSpiderRainZombiesCountPerLevel) == 60);
static_assert(offsetof(DangerRoomSpiderRainDesigner, BaseSpiderRainStartWave) == 64);

class DangerRoomFallenKnightDesigner : public DangerRoomSpiderRainDesigner 
{
public:
	SexyString WaveStartMessage = "Fallen Knight!";
	SexyString SpiderZombieName = "dark_cavalry_rider";

	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(DangerRoomFallenKnightDesigner, 0x8AE3A8);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(DangerRoomSpiderRainDesigner);
	RT_CLASS_REGISTER_STRING_PROPERTY(DangerRoomFallenKnightDesigner, WaveStartMessage);
	RT_CLASS_REGISTER_STRING_PROPERTY(DangerRoomFallenKnightDesigner, SpiderZombieName);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(DangerRoomFallenKnightDesigner);
	RT_CLASS_GET_CLASS_FUNCTION(DangerRoomFallenKnightDesigner, 0x10B92D8);
};