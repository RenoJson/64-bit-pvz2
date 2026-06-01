#pragma once
#include "PvZ2/PlantFramework.h"
#include <Sexy/RtReflectionDelegateBase.h>
#include <Reflection/ReflectionBuilder.h>
#include "Zombie.h"
#include "Plant.h"

class PlantChomper : public PlantFramework{
public:
	float m_animStartTime;
	std::vector<RtWeakPtr<Zombie>> m_suctionZombie;
	std::vector<RtWeakPtr<Zombie>> m_burpNudgedZombies;
	bool m_zombieSuctioned;
	char pad_0029[3];
	float m_gobbledZombieCount;
};


class PlantDinoGrass : public PlantChomper
{
public:
	bool m_isRoaring;
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(PlantDinoGrass, 0x1639F68);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(PlantChomper);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(PlantDinoGrass, m_isRoaring);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(PlantDinoGrass);
	RT_CLASS_GET_CLASS_FUNCTION(PlantDinoGrass, 0xED7870);
};