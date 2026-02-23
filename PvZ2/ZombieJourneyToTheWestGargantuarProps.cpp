#include "ZombieDinoBullyVeteranProps.h"

void* ZombieJourneyToTheWestGargantuarProps::vftable = __null;
Sexy::RtClass* ZombieJourneyToTheWestGargantuarProps::s_rtClass = __null;


void ZombieJourneyToTheWestGargantuarProps::modInit() {
	LOGI("ZombieJourneyToTheWestGargantuarProps init");

	vftable = CopyVFTable(getActualOffset(0x23E53D8), 14);

	PatchVFTable(vftable, (void*)ZombieJourneyToTheWestGargantuarProps::StaticGetType, 0);

	ZombieJourneyToTheWestGargantuarProps::StaticGetType();

	LOGI("ZombieJourneyToTheWestGargantuarProps finish init");
}