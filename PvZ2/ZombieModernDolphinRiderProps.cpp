#include "ZombieModernDolphinRiderProps.h"

void* ZombieModernDolphinRiderProps::vftable = __null;
Sexy::RtClass* ZombieModernDolphinRiderProps::s_rtClass = __null;


void ZombieModernDolphinRiderProps::modInit() {
	LOGI("ZombieModernDolphinRiderProps init");

	vftable = CopyVFTable(getActualOffset(0x24328D0), 14);

	PatchVFTable(vftable, (void*)ZombieModernDolphinRiderProps::StaticGetType, 0);

	ZombieModernDolphinRiderProps::StaticGetType();

	LOGI("ZombieModernDolphinRiderProps finish init");
}