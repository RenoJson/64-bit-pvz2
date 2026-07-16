#pragma once
#include "PvZ2/ZombieBasic.h"
#include <Sexy/RtReflectionDelegateBase.h>
#include <Reflection/ReflectionBuilder.h>

using namespace Sexy;

class ZombieDinoBasic : public ZombieBasic
{
public:
    RtReflectionDelegateBase m_onPterodactylGrab;
    RtReflectionDelegateBase m_onPterodactylDrop;
};

static_assert(sizeof(ZombieDinoBasic) == 1280);
//static_assert(offsetof(ZombieDinoBasic, m_onPterodactylGrab) == 1104);
//static_assert(offsetof(ZombieDinoBasic, m_onPterodactylDrop) == 1192);

class ZombieDinoBully : public ZombieDinoBasic
{
public:

};

class ZombieDinoBullyVeteran : public ZombieDinoBully
{
public:
	ZombieAnimRig* m_someArgThatCallZombieAnimRigFunction38;
};

class ZombieJourneyToTheWestGargantuar : public ZombieDinoBullyVeteran
{
public:
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieJourneyToTheWestGargantuar, 0xC3AB1C);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombieDinoBullyVeteran);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieJourneyToTheWestGargantuar);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieJourneyToTheWestGargantuar, 0xB0BB10);
};
class ZombieModernMiner : public ZombieDinoBullyVeteran
{
public:
	bool m_isDigged;
	bool m_diggedDone;
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieModernMiner, 0xC3AB1C);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombieDinoBullyVeteran);
	ZombieModernMiner::buildEventCallbacks(builder, rclass);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieModernMiner, m_isDigged);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieModernMiner, m_diggedDone);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_AND_STATES_FUNCTION(ZombieModernMiner);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieModernMiner, 0xB0BB10);
	static void buildEventCallbacks(Reflection::CRefManualSymbolBuilder* builder, Reflection::RClass* rtClass);
	static void buildStates();
	static void DiveInOnEnter(ZombieModernMiner* zombie);
	static void DiveInOnLoop(ZombieModernMiner* zombie);
	static void DiveInOnExit(ZombieModernMiner* zombie);
	static void DiggingOnEnter(ZombieModernMiner* zombie);
	static void DiggingOnLoop(ZombieModernMiner* zombie);
	static void DiggingOnExit(ZombieModernMiner* zombie);
	static void DiveOutOnEnter(ZombieModernMiner* zombie);
	static void DiveOutOnLoop(ZombieModernMiner* zombie);
	static void DiveOutOnExit(ZombieModernMiner* zombie);
	static void LostStickOnEnter(ZombieModernMiner* zombie);
	static void LostStickOnLoop(ZombieModernMiner* zombie);
	static void LostStickOnExit(ZombieModernMiner* zombie);
	static void LostStickDiggingOnEnter(ZombieModernMiner* zombie);
	static void LostStickDiggingOnLoop(ZombieModernMiner* zombie);
	static void LostStickDiggingOnExit(ZombieModernMiner* zombie);
};