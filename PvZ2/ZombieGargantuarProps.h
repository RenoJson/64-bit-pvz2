#pragma once
#include "Zombie.h"
using namespace Sexy;

class ZombieGargantuarProjectilePair {
	float HealthPercentThrowImp;
	std::vector<SexyString> ProjectileLayersToHide;
};

class ZombieGargantuarProps : public ZombiePropertySheet
{
public:

    std::vector<ZombieGargantuarProjectilePair> HealthThresholdToImpAmmoLayers;
    float MinPosXThrowImp;     
    float ThrowImpDuration;    
    int32_t ImpTargetColumn;   
    float ImpApex;   
    Sexy::SexyVector3 ImpSpawnOffset;
    float ImpFlightTime;
    float SmashDuration;        
    float SmashDamage;      
    SexyString ImpType; 
    SexyString SoundOnSmash;  
    SexyString SoundOnCannon; 
};

static_assert(offsetof(ZombieGargantuarProps, HealthThresholdToImpAmmoLayers) == 568);
static_assert(offsetof(ZombieGargantuarProps, ImpSpawnOffset) == 608);
static_assert(offsetof(ZombieGargantuarProps, ImpFlightTime) == 620);