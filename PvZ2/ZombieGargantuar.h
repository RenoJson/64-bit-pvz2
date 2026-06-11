#include "PvZ2/Zombie.h"

class ZombieGargantuar : public Zombie
{
public:
	uint m_thrownImpIndex;
	int m_watchAnimHandle;
};
class ZombieLostCityGargantuar : public ZombieGargantuar
{
public:

};
class ZombieLostCityTorchGargantuar : public ZombieGargantuar
{
public:
	bool m_hasTorch = true;	
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieLostCityTorchGargantuar, 0xC3AB1C);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombieGargantuar);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieLostCityTorchGargantuar);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieLostCityTorchGargantuar, 0xB53840);
};
class ZombieVaseGargantuar : public ZombieGargantuar
{
public:
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieVaseGargantuar, 0xC3AB1C);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombieGargantuar);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieVaseGargantuar);
	RT_CLASS_GET_CLASS_FUNCTION(ZombieVaseGargantuar, 0xB53840);
};
//static_assert(sizeof(ZombieGargantuar) == 0x310);