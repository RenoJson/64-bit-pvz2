#include "ZombieWesternKongMingLantern.h"

void* ZombieWesternKongMingLantern::vftable = __null;
Sexy::RtClass* ZombieWesternKongMingLantern::s_rtClass = __null;;

void* ZombieNewModernBalloon::vftable = __null;
Sexy::RtClass* ZombieNewModernBalloon::s_rtClass = __null;;

SexyString GetKongMingShockEffectName()
{
	return "POPANIM_EFFECTS_ZOMBIE_JOURNEY_TO_THE_WEST_BALLOON_SHOCK";
}
SexyString GetKongMingAshEffectName()
{
	return "POPANIM_EFFECTS_ZOMBIE_JOURNEY_TO_THE_WEST_BALLOON_ASH";
}
SexyString GetBalloonShockEffectName()
{
	return "POPANIM_EFFECTS_ZOMBIE_MODERN_BALLOON_SHOCK";
}
SexyString GetBalloonAshEffectName()
{
	return "POPANIM_EFFECTS_ZOMBIE_MODERN_BALLOON_ASH";
}

void ZombieWesternKongMingLantern::modInit() {
	LOGI("ZombieWesternKongMingLantern init");

	vftable = CopyVFTable(getActualOffset(0x1CA1E40), 213);

	PatchVFTable(vftable, (void*)ZombieWesternKongMingLantern::StaticGetType, 0);

	PatchVFTable(vftable, (void*)GetKongMingShockEffectName, 189);

	PatchVFTable(vftable, (void*)GetKongMingAshEffectName, 190);

	ZombieWesternKongMingLantern::StaticGetType();

	LOGI("ZombieWesternKongMingLantern finish init");
}


void ZombieNewModernBalloon::modInit() {
	LOGI("ZombieWesternKongMingLantern init");

	vftable = CopyVFTable(getActualOffset(0x1CA1E40), 213);

	PatchVFTable(vftable, (void*)ZombieNewModernBalloon::StaticGetType, 0);

	PatchVFTable(vftable, (void*)GetBalloonShockEffectName, 189);

	PatchVFTable(vftable, (void*)GetBalloonAshEffectName, 190);

	ZombieNewModernBalloon::StaticGetType();

	LOGI("ZombieWesternKongMingLantern finish init");
}