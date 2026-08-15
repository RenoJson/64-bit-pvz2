#pragma once
#include "Zombie.h"
#include <Reflection/ReflectionBuilder.h>

class ZombieMausoleumPiggy : public Zombie
{
public:
	int m_lastPathGridX;
	int m_lastPathGridY;
	int m_currentDirection;
	float m_lastGroundTrackOffset;
	DECLARE_STATIC_RT_CLASS_MEMBERS(ZombieMausoleumPiggy)

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieMausoleumPiggy, 0xC3AB1C);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(Zombie);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieMausoleumPiggy, m_lastPathGridX);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieMausoleumPiggy, m_lastPathGridY);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieMausoleumPiggy, m_currentDirection);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieMausoleumPiggy);

	RT_CLASS_GET_CLASS_FUNCTION(ZombieMausoleumPiggy, 0xC36FE4);
};
