#include "MeteorFlowerProps.h"

void* MeteorFlowerProps::vftable = __null;
Sexy::RtClass* MeteorFlowerProps::s_rtClass = __null;


void MeteorFlowerProps::modInit() {
	LOGI("MeteorFlowerProps init");

	vftable = copyVFTable(getActualOffset(0x1C5FF50), 14);

	PatchVFTable(vftable, (void*)MeteorFlowerProps::StaticGetType, RTOBJECT_GETTYPE_INDEX);

	MeteorFlowerProps::StaticGetType();

	LOGI("MeteorFlowerProps finish init");
}