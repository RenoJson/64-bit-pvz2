#include "ZombieModernAllStar.h"

void* ZombieModernBerserkerProps::vftable = __null;
Sexy::RtClass* ZombieModernBerserkerProps::s_rtClass = __null;


void ZombieModernBerserkerProps::modInit() {
	LOGI("ZombieModernBerserkerProps init");

	vftable = CopyVFTable(getActualOffset(0x23F2358), 14);

	PatchVFTable(vftable, (void*)ZombieModernBerserkerProps::StaticGetType, 0);

	ZombieModernBerserkerProps::StaticGetType();

	LOGI("ZombieModernBerserkerProps finish init");
}