#include "ZombieMausoleumPiggyProps.h"

void* ZombieMausoleumPiggyProps::vftable = __null;
Sexy::RtClass* ZombieMausoleumPiggyProps::s_rtClass = __null;


void ZombieMausoleumPiggyProps::modInit() {
	LOGI("ZombieMausoleumPiggyProps init");

	vftable = CopyVFTable(getActualOffset(0x24328D0), 14);

	PatchVFTable(vftable, (void*)ZombieMausoleumPiggyProps::StaticGetType, 0);

	ZombieMausoleumPiggyProps::StaticGetType();

	LOGI("ZombieMausoleumPiggyProps finish init");
}