#pragma once
#include "PvZ2/Zombie.h"
#include "Reflection/CRefManualSymbolBuilder.h"
#include <Reflection/ReflectionBuilder.h>

class ZombieFutureJetpack : public Zombie
{
public:
	bool m_jumpMovement;
};


class ZombieFutureJetpackVeteran : public ZombieFutureJetpack
{
public:
	bool m_firstBoosted;
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieFutureJetpackVeteran, 0xC3AB1C);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombieFutureJetpack);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieFutureJetpackVeteran, m_firstBoosted);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieFutureJetpackVeteran);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieFutureJetpackVeteran, 0xB43240);
};
