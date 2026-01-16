#pragma once

#include "PropertySheetBase.h"
#include "ComponentWarmingBase.h"
class PowerPropertySheet : public PropertySheetBase {
public:
	SexyString TypeName;
};
class PowerPropsWarmingRadius : public PowerPropertySheet {
public:
	ComponentWarmingRadiusProps WarmingRadius;
    static Sexy::RtClass* StaticGetType() {
        typedef Sexy::RtClass* (*getType)();
        getType pGetType = (getType)getActualOffset(0xFC7704);
        return pGetType();
    };
};
static_assert(sizeof(PowerPropertySheet) == 0x28);
static_assert(sizeof(PowerPropsWarmingRadius) == 0x128);