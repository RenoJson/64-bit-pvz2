#include "ZombieEightiesBassProps.h"

void* ZombieEightiesBassProps::vftable = __null;
Sexy::RtClass* ZombieEightiesBassProps::s_rtClass = __null;


void ZombieEightiesBassProps::modInit() {
	LOGI("ZombieEightiesBassProps init");

	vftable = CopyVFTable(getActualOffset(0x24328D0), 14);

	PatchVFTable(vftable, (void*)ZombieEightiesBassProps::StaticGetType, 0);

	ZombieEightiesBassProps::StaticGetType();

	LOGI("ZombieEightiesBassProps finish init");
}