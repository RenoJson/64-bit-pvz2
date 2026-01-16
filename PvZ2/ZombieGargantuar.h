#include "PvZ2/Zombie.h"

class ZombieGargantuar : public Zombie
{
public:
	uint32_t m_thrownImpIndex;
	int32_t m_watchAnimHandle;
};
//static_assert(sizeof(ZombieGargantuar) == 0x310);