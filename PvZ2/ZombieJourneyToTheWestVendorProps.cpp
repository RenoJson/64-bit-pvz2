#include "ZombieJourneyToTheWestVendorProps.h"

void* ZombieJourneyToTheWestVendorProps::vftable = __null;
Sexy::RtClass* ZombieJourneyToTheWestVendorProps::s_rtClass = __null;


void ZombieJourneyToTheWestVendorProps::modInit() {
	LOGI("ZombieJourneyToTheWestVendorProps init");

	vftable = CopyVFTable(getActualOffset(0x24328D0), 14);

	PatchVFTable(vftable, (void*)ZombieJourneyToTheWestVendorProps::StaticGetType, 0);

	ZombieJourneyToTheWestVendorProps::StaticGetType();

	LOGI("ZombieJourneyToTheWestVendorProps finish init");
}