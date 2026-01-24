#include "ZombieDinoBasic.h"

void* ZombieJourneyToTheWestGargantuar::vftable = __null;
Sexy::RtClass* ZombieJourneyToTheWestGargantuar::s_rtClass = __null;;

SexyString GetGargShockEffectName()
{
	return "POPANIM_EFFECTS_ZOMBIE_GARGANTUAR_SHOCK";
}
SexyString GetGargAshEffectName()
{
	return "POPANIM_EFFECTS_ZOMBIE_GARGANTUAR_ASH";
}

void ZombieJourneyToTheWestGargantuar::modInit() {
	LOGI("ZombieJourneyToTheWestGargantuar init");

	vftable = CopyVFTable(getActualOffset(0x23E4CC8), 215);

	PatchVFTable(vftable, (void*)ZombieJourneyToTheWestGargantuar::StaticGetType, 0);

	PatchVFTable(vftable, (void*)GetGargShockEffectName, 189);

	PatchVFTable(vftable, (void*)GetGargAshEffectName, 190);

	ZombieJourneyToTheWestGargantuar::StaticGetType();

	LOGI("ZombieJourneyToTheWestGargantuar finish init");
}