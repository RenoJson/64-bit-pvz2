#pragma once
#include "PvZ2/Zombie.h"
#include "PvZ2/ZombieAnimRig.h"
#include "Reflection/CRefManualSymbolBuilder.h"
#include <Reflection/ReflectionBuilder.h>
class ZombiePharaoh : public Zombie {
public:
	int m_helmDamageIndex;
	
    RT_CLASS_GET_CLASS_WRAPPER(0xB1E4C8);
	static void ModInit();
};

class ZombieDarkKing : public Zombie {
public:
	RtWeakPtr<RtObject> m_knightingTarget;

};