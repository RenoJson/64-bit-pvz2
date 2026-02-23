#include "ZombieJourneyToTheWestPiggyProps.h"

void* ZombieJourneyToTheWestPiggyProps::vftable = __null;
Sexy::RtClass* ZombieJourneyToTheWestPiggyProps::s_rtClass = __null;


void ZombieJourneyToTheWestPiggyProps::modInit() {
	LOGI("ZombieJourneyToTheWestPiggyProps init");

	vftable = CopyVFTable(getActualOffset(0x24328D0), 14);

	PatchVFTable(vftable, (void*)ZombieJourneyToTheWestPiggyProps::StaticGetType, 0);

	ZombieJourneyToTheWestPiggyProps::StaticGetType();

	LOGI("ZombieJourneyToTheWestPiggyProps finish init");
}