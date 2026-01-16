#include "ZombieCavalryProps.h"

void* ZombieDarkCavalryProps::vftable = __null; 
Sexy::RtClass* ZombieDarkCavalryProps::s_rtClass = __null;


void ZombieDarkCavalryProps::modInit() {
	LOGI("ZombieDarkCavalryProps init");

	vftable = CopyVFTable(getActualOffset(0x2432FA8), 14);

	PatchVFTable(vftable, (void*)ZombieDarkCavalryProps::StaticGetType, 0);

	ZombieDarkCavalryProps::StaticGetType();

	LOGI("ZombieDarkCavalryProps finish init");
}