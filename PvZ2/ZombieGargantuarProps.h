#pragma once
#include "Zombie.h"
#include <Reflection/ReflectionBuilder.h>

using namespace Sexy;

class ZombieGargantuarProjectilePair {
public:
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

class ZombieFairyTaleGargantuarProps : public ZombieGargantuarProps
{
public:
    float DamageReduction = 50.0f;
    bool CarryImp = true;
    static void* vftable;
    static Sexy::RtClass* s_rtClass;
    static void modInit();

    RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieFairyTaleGargantuarProps, 0xC136A4);
    RT_CLASS_CONSTRUCT_FUNCTION_END();

    RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombieGargantuarProps);
    RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieFairyTaleGargantuarProps, DamageReduction);
    RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieFairyTaleGargantuarProps, CarryImp);
    RT_CLASS_BUILD_SYMBOLS_END();

    RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieFairyTaleGargantuarProps);
    RT_CLASS_GET_CLASS_FUNCTION(ZombieFairyTaleGargantuarProps, 0xDAA338);
};