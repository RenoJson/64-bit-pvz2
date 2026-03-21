#include "ZombieGargantuarProps.h"

void* ZombieFairyTaleGargantuarProps::vftable = __null;
Sexy::RtClass* ZombieFairyTaleGargantuarProps::s_rtClass = __null;


void ZombieFairyTaleGargantuarProps::modInit() {
	LOGI("ZombieFairyTaleGargantuarProps init");

	vftable = CopyVFTable(getActualOffset(0x2432A08), 14);

	PatchVFTable(vftable, (void*)ZombieFairyTaleGargantuarProps::StaticGetType, 0);

	ZombieFairyTaleGargantuarProps::StaticGetType();

	LOGI("ZombieFairyTaleGargantuarProps finish init");
}