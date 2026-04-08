#include "ZombieDinoBullyVeteranProps.h"

void* ZombieJourneyToTheWestGargantuarProps::vftable = __null;
Sexy::RtClass* ZombieJourneyToTheWestGargantuarProps::s_rtClass = __null;

void* ZombieModernMinerProps::vftable = __null;
Sexy::RtClass* ZombieModernMinerProps::s_rtClass = __null;


void ZombieJourneyToTheWestGargantuarProps::modInit() {
	LOGI("ZombieJourneyToTheWestGargantuarProps init");

	vftable = CopyVFTable(getActualOffset(0x23E53D8), 14);

	PatchVFTable(vftable, (void*)ZombieJourneyToTheWestGargantuarProps::StaticGetType, 0);

	ZombieJourneyToTheWestGargantuarProps::StaticGetType();

	LOGI("ZombieJourneyToTheWestGargantuarProps finish init");
}

void ZombieModernMinerProps::modInit() {
	LOGI("ZombieModernMinerProps init");

	vftable = CopyVFTable(getActualOffset(0x23E53D8), 14);

	PatchVFTable(vftable, (void*)ZombieModernMinerProps::StaticGetType, 0);

	ZombieModernMinerProps::StaticGetType();

	LOGI("ZombieModernMinerProps finish init");
}