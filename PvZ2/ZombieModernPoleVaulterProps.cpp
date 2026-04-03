#include "ZombieModernPoleVaulterProps.h"

void* ZombieModernPoleVaulterProps::vftable = __null;
Sexy::RtClass* ZombieModernPoleVaulterProps::s_rtClass = __null;


void ZombieModernPoleVaulterProps::modInit() {
	LOGI("ZombieModernPoleVaulterProps init");

	vftable = CopyVFTable(getActualOffset(0x24328D0), 14);

	PatchVFTable(vftable, (void*)ZombieModernPoleVaulterProps::StaticGetType, 0);

	ZombieModernPoleVaulterProps::StaticGetType();

	LOGI("ZombieModernPoleVaulterProps finish init");
}