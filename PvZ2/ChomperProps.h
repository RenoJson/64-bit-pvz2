#pragma once
#include "Plant.h"

class ChomperProps : public PlantPropertySheet {
public:
    int NumberOfPlantfoodTargets; 
    float ChewTimeSeconds;     
    float BurpingTimeSeconds;   
    std::vector<SexyString> ZombieChewExcludelist; 
    float ChewDamage;
    float PFChewDamage;            
    float SuctionSpeed;           
    float BurpingSpeed;           
    int GobbleCapacity;
};
class DinoGrassProps : public ChomperProps {
public:
	std::vector<float> StunDurationRange;
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(DinoGrassProps, 0x611410);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(ChomperProps); 
	RT_CLASS_REGISTER_STANDARD_VECTOR_PROPERTY(DinoGrassProps, StunDurationRange, float)
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(DinoGrassProps);
	RT_CLASS_GET_CLASS_FUNCTION(DinoGrassProps, 0x61E4B0);

};