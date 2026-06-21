#include "ZombieMausoleumVendorProps.h"

void* ZombieMausoleumVendorProps::vftable = __null;
Sexy::RtClass* ZombieMausoleumVendorProps::s_rtClass = __null;


void ZombieMausoleumVendorProps::modInit() {
	LOGI("ZombieMausoleumVendorProps init");

	vftable = CopyVFTable(getActualOffset(0x24328D0), 14);

	PatchVFTable(vftable, (void*)ZombieMausoleumVendorProps::StaticGetType, 0);

	ZombieMausoleumVendorProps::StaticGetType();

	LOGI("ZombieMausoleumVendorProps finish init");
}