#include "ZombieWesternTudigongProps.h"

void* ZombieWesternTudigongProps::vftable = __null;
Sexy::RtClass* ZombieWesternTudigongProps::s_rtClass = __null;


void ZombieWesternTudigongProps::modInit() {
	LOGI("ZombieWesternTudigongProps init");

	vftable = CopyVFTable(getActualOffset(0x24328D0), 14);

	PatchVFTable(vftable, (void*)ZombieWesternTudigongProps::StaticGetType, 0);

	ZombieWesternTudigongProps::StaticGetType();

	LOGI("ZombieWesternTudigongProps finish init");
}