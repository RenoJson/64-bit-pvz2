#pragma once
#include "PlantPropertySheet.h"
class BananaProps : public PlantPropertySheet
{
public:
    char _pad_fix_offset[4];
    int NumPlantFoodRockets;

    float FireTime;

    float PlantFoodFireTime;

    SexyVector2 TargetOffset;
};
static_assert(sizeof(BananaProps) == 0x158);
