#include "PvZ2/Zombie.h"

class ZombieGargantuar : public Zombie
{
public:
	uint m_thrownImpIndex;
	int m_watchAnimHandle;
};
//static_assert(sizeof(ZombieGargantuar) == 0x310);