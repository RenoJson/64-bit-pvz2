#include "ZombieFairyTaleImpProps.h"

void* ZombieFairyTaleImpProps::vftable = __null;
Sexy::RtClass* ZombieFairyTaleImpProps::s_rtClass = __null;


void ZombieFairyTaleImpProps::modInit() {
	LOGI("ZombieFairyTaleImpProps init");

	vftable = CopyVFTable(getActualOffset(0x24328D0), 14);

	PatchVFTable(vftable, (void*)ZombieFairyTaleImpProps::StaticGetType, 0);

	ZombieFairyTaleImpProps::StaticGetType();

	LOGI("ZombieFairyTaleImpProps finish init");
}