#include "ZombieModernPogoProps.h"

void* ZombieModernPogoProps::vftable = __null;
Sexy::RtClass* ZombieModernPogoProps::s_rtClass = __null;


void ZombieModernPogoProps::modInit() {
	LOGI("ZombieModernPogoProps init");

	vftable = CopyVFTable(getActualOffset(0x24328D0), 14);

	PatchVFTable(vftable, (void*)ZombieModernPogoProps::StaticGetType, 0);

	ZombieModernPogoProps::StaticGetType();

	LOGI("ZombieModernPogoProps finish init");
}