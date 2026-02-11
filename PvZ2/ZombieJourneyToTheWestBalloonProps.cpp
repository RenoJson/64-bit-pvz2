#include "ZombieJourneyToTheWestBalloonProps.h"

void* ZombieJourneyToTheWestBalloonProps::vftable = __null;
Sexy::RtClass* ZombieJourneyToTheWestBalloonProps::s_rtClass = __null;


void ZombieJourneyToTheWestBalloonProps::modInit() {
	LOGI("ZombieJourneyToTheWestBalloonProps init");

	vftable = CopyVFTable(getActualOffset(0x24328D0), 14);

	PatchVFTable(vftable, (void*)ZombieJourneyToTheWestBalloonProps::StaticGetType, 0);

	ZombieJourneyToTheWestBalloonProps::StaticGetType();

	LOGI("ZombieJourneyToTheWestBalloonProps finish init");
}