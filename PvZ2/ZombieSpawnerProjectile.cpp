#include "ZombieSpawnerProjectile.h"
#include "Plant.h"
#include "AddZombieType.h"
#include "ZombieFairyTaleWitchFrog.h"
#include "ZombieWithAction.h"
DEFINE_STATIC_RT_CLASS_MEMBERS(ZombieSpawnerProjectile)
DEFINE_STATIC_RT_CLASS_MEMBERS(ZombieSpawnerProjectileProps)

bool hkZombieSpawnerProjectileOnCollision(ZombieSpawnerProjectile* thisPtr)
{
    auto props = reinterpret_cast<ZombieSpawnerProjectileProps*>(thisPtr->m_propertySheet.Get());

    SexyString selectedZombieName = "";
    if (!props->ZombieTypeName.empty())
    {
        float totalWeight = 0.0f;
        int numZombies = props->ZombieTypeName.size();
        int numWeights = props->Weight.size();

        int safeSize = std::min(numZombies, numWeights);

        for (int i = 0; i < safeSize; ++i)
        {
            totalWeight += props->Weight[i];
        }

        if (totalWeight <= 0.0f || safeSize == 0)
        {
            int randomIndex = std::rand() % numZombies; 
            selectedZombieName = props->ZombieTypeName[randomIndex];
        }
        else
        {
            float randomValue = (static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX)) * totalWeight;

            float currentSum = 0.0f;
            int selectedIndex = 0;

            for (int i = 0; i < safeSize; ++i)
            {
                currentSum += props->Weight[i];
                if (randomValue < currentSum)
                {
                    selectedIndex = i;
                    break; 
                }
            }

            selectedZombieName = props->ZombieTypeName[selectedIndex];
        }
    }
    else
    {
        
        return false;
    }

    Zombie* frog = AddZombie(selectedZombieName, -1, 6, -1);

    if (frog != nullptr)
    {
        frog->m_teamFlags = 2;
        float currentX = thisPtr->m_position.x;
        float currentY = thisPtr->m_position.y;
        float currentZ = thisPtr->m_position.z;

        typedef SexyVector3(*boardEntitySetPosition)(Zombie*, SexyVector3*);
        boardEntitySetPosition funBoardEntitySetPosition = (boardEntitySetPosition)getActualOffset(0x628058);

        SexyVector3 position = SexyVector3(currentX, currentY, currentZ);
        SexyVector3 updatePos = funBoardEntitySetPosition(frog, &position);

        if (props->RiseFromGround) 
        {
            typedef int64_t(*riseFromGround)(Zombie*, SexyVector3*, bool);
            ((riseFromGround)getActualOffset(0xC4CAD4))(frog, &position, true);
        }
    }

    typedef bool (*doImpact)(ZombieSpawnerProjectile*, bool);
    doImpact DoImpact = ((doImpact)getActualOffset(0xCA065C));

    return DoImpact(thisPtr, false);
}


void ZombieSpawnerProjectile::ModInit() {
	LOGI("SpawnerProjectile init");

	vftable = CopyVFTable(getActualOffset(0x2421040), 53);

	PatchVFTable(vftable, (void*)ZombieSpawnerProjectile::StaticGetType, 0);

	PatchVFTable(vftable, (void*)hkZombieSpawnerProjectileOnCollision, 36);

	ZombieSpawnerProjectile::StaticGetType();

	LOGI("SpawnerProjectile finish init");
}
void ZombieSpawnerProjectileProps::ModInit() {
	LOGI("SpawnerProjectileProps init");

	vftable = CopyVFTable(getActualOffset(0x23848F0), 14);

	PatchVFTable(vftable, (void*)ZombieSpawnerProjectileProps::StaticGetType, 0);

	ZombieSpawnerProjectileProps::StaticGetType();

	LOGI("SpawnerProjectileProps finish init");
}