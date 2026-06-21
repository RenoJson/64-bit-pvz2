#pragma once
#include "Zombie.h"
#include <Reflection/ReflectionBuilder.h>

class ZombieMausoleumCavalry : public Zombie
{
public:
	int m_lastPathGridX;
	int m_lastPathGridY;
	int m_currentDirection;
	DECLARE_STATIC_RT_CLASS_MEMBERS(ZombieMausoleumCavalry)

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieMausoleumCavalry, 0xC3AB1C);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(Zombie);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieMausoleumCavalry, m_lastPathGridX);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieMausoleumCavalry, m_lastPathGridY);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieMausoleumCavalry, m_currentDirection);
	RT_CLASS_BUILD_SYMBOLS_END();
	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieMausoleumCavalry)
	RT_CLASS_GET_CLASS_FUNCTION(ZombieMausoleumCavalry, 0xC36FE4);
};
