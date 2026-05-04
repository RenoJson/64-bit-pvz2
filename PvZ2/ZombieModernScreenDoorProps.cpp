#include "ZombieModernScreenDoorProps.h"

void* ZombieModernScreenDoorProps::vftable = __null;
Sexy::RtClass* ZombieModernScreenDoorProps::s_rtClass = __null;

void ZombieModernScreenDoorProps::modInit() {
	LOGI("ZombieModernScreenDoorProps init");

	vftable = CopyVFTable(getActualOffset(0x24328D0), 14);

	PatchVFTable(vftable, (void*)ZombieModernScreenDoorProps::StaticGetType, 0);

	ZombieModernScreenDoorProps::StaticGetType();

	LOGI("ZombieModernScreenDoorProps finish init");
}