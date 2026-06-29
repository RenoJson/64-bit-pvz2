#pragma once
#include "GridItem.h"
#include <Sexy/Image.h>

class GridItemLilyPadProps : public GridItemAnimationProps {
public:
	PlantRestrictionSet PlantsWhichCannotBePlantedOnLilypads;
};

class GridItemHaystack : public GridItemAnimation {
public:
	static void* vftable;
	static void* vftable1;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	static void* Construct() {
		auto* instance = new GridItemHaystack();
		typedef void* (*ctorWithThisPtr)(void*);
		ctorWithThisPtr baseCtor = (ctorWithThisPtr)getActualOffset(0x76D2F8);
		baseCtor(instance);;
		SetVFTable(instance, (uintptr_t)vftable);
		instance->renderableVftable = (void**)vftable1;
		return instance;
	};

	RT_CLASS_BUILD_SYMBOLS_BEGIN(GridItemAnimation);
		RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(GridItemHaystack);
	RT_CLASS_GET_CLASS_FUNCTION(GridItemHaystack, 0x8051EC);
};
