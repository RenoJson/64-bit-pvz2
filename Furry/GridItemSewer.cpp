#include "GridItemSewer.h"

DEFINE_STATIC_RT_CLASS_MEMBERS(GridItemSewer);
DEFINE_STATIC_RT_CLASS_MEMBERS(GridItemSewerProps);

typedef uint64_t*(*fun)(GridItem*);
fun oGridItemUpdate = nullptr;

uint64_t* hkGridItemUpdate(GridItem* self)
{
	LOGI("passed");
	if (self->IsType(GridItemSewer::StaticGetType()))
	{
	}
	else
	{
		LOGI("nuh uh");
	}
	return oGridItemUpdate(self);
}

void GridItemSewer::ModInit()
{
	vftable = CreateChildVFTable(GRIDITEM_ANIM_VFUNCS, getActualOffset(GRIDITEM_ANIM_VFTABLE), GRIDITEM_ANIM_VFUNCS);
	PatchVFTable(vftable, (void*)StaticGetType, RTOBJECT_INDEX_GETTYPE);
	PatchVFTable(vftable, (void*)Destroy, RTOBJECT_INDEX_DTOR);
	PatchVFTable(vftable, (void*)DestroyAndDealloc, RTOBJECT_INDEX_DEALLOC);

	s_rtClass = StaticGetType();

	//PVZ2HookFunction(0x76DA44, (void*)hkGridItemUpdate, (void**)&oGridItemUpdate);
}

void GridItemSewerProps::ModInit()
{
	vftable = CreateChildVFTable(GRIDITEM_ANIMPROPS_VFUNCS, getActualOffset(GRIDITEM_ANIMPROPS_VFTABLE), GRIDITEM_ANIMPROPS_VFUNCS);
	PatchVFTable(vftable, (void*)StaticGetType, RTOBJECT_INDEX_GETTYPE);
	PatchVFTable(vftable, (void*)Destroy, RTOBJECT_INDEX_DTOR);
	PatchVFTable(vftable, (void*)DestroyAndDealloc, RTOBJECT_INDEX_DEALLOC);

	s_rtClass = StaticGetType();
}