#include "ZombieMausoleumCavalryProps.h"

void* ZombieMausoleumCavalryProps::vftable = __null;
Sexy::RtClass* ZombieMausoleumCavalryProps::s_rtClass = __null;


void ZombieMausoleumCavalryProps::modInit() {
	LOGI("ZombieMausoleumCavalryProps init");

	vftable = CopyVFTable(getActualOffset(0x24328D0), 14);

	PatchVFTable(vftable, (void*)ZombieMausoleumCavalryProps::StaticGetType, 0);

	ZombieMausoleumCavalryProps::StaticGetType();

	LOGI("ZombieMausoleumCavalryProps finish init");
}