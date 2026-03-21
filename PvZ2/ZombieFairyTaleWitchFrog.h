#pragma once
#include "Zombie.h"
#include <Reflection/ReflectionBuilder.h>

class ZombieFairyTaleWitchFrog : public Zombie {
public:
	SexyString m_transformedPlant;
	DECLARE_STATIC_RT_CLASS_MEMBERS(ZombieFairyTaleWitchFrog)

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieFairyTaleWitchFrog, 0xC3AB1C);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(Zombie);
	RT_CLASS_REGISTER_STRING_PROPERTY(ZombieFairyTaleWitchFrog, m_transformedPlant);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieFairyTaleWitchFrog);

	RT_CLASS_GET_CLASS_FUNCTION(ZombieFairyTaleWitchFrog, 0xC36FE4);
};