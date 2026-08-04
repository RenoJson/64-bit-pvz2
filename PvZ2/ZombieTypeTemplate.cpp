#include "ZombieTypeTemplate.h"

void* ZombieTypeTemplate::vftable = __null;
Sexy::RtClass* ZombieTypeTemplate::s_rtClass = __null;

void ZombieTypeTemplate::modInit() {
	LOGI("ZombieTypeTemplate init");

	vftable = CopyVFTable(getActualOffset(0x248AE20), 7);

	PatchVFTable(vftable, (void*)ZombieTypeTemplate::StaticGetType, 0);

	ZombieTypeTemplate::StaticGetType();

	LOGI("ZombieTypeTemplate finish init");
}