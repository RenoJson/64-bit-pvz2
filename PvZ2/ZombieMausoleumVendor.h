#pragma once
#include "Zombie.h"
#include <Reflection/ReflectionBuilder.h>

class ZombieMausoleumVendor : public Zombie
{
public:
	static void buildEventCallbacks(Reflection::CRefManualSymbolBuilder* builder, Reflection::RClass* rtClass);
	static void buildStates();
	static void PigOnEnter(ZombieMausoleumVendor* zombie);
	static void PigOnLoop(ZombieMausoleumVendor* zombie);
	static void PigOnExit(ZombieMausoleumVendor* zombie);
	float m_nextPigTime;
	bool m_firstSpawned = false;
	int m_lastPathGridX;
	int m_lastPathGridY;
	int m_currentDirection;
	float m_lastGroundTrackOffset;
	static void* vftable; 
	static Sexy::RtClass* s_rtClass; 
	static void ModInit();

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieMausoleumVendor, 0xC3AB1C);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(Zombie);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieMausoleumVendor, m_nextPigTime);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieMausoleumVendor, m_firstSpawned);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieMausoleumVendor, m_lastPathGridX);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieMausoleumVendor, m_lastPathGridY);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieMausoleumVendor, m_currentDirection);
	ZombieMausoleumVendor::buildEventCallbacks(builder, rclass);
	RT_CLASS_BUILD_SYMBOLS_END();
	RT_CLASS_REGISTER_CLASS_AND_STATES_FUNCTION(ZombieMausoleumVendor)
	RT_CLASS_GET_CLASS_FUNCTION(ZombieMausoleumVendor, 0xC36FE4);

};
