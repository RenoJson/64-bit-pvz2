#include "ZombieWesternTudigong.h"


void* ZombieWesternTudigong::vftable = __null;
Sexy::RtClass* ZombieWesternTudigong::s_rtClass = __null;;

SexyString GetTudigongShockEffectName()
{
	return "POPANIM_EFFECTS_ZOMBIE_JOURNEY_TO_THE_WEST_MINER_SHOCK";
}
SexyString GetTudigongAshEffectName()
{
	return "POPANIM_EFFECTS_ZOMBIE_JOURNEY_TO_THE_WEST_MINER_ASH";
}

void ZombieWesternTudigong::modInit() {
	LOGI("ZombieWesternTudigong init");

	vftable = CopyVFTable(getActualOffset(0x23DEFB8), 206);

	PatchVFTable(vftable, (void*)ZombieWesternTudigong::StaticGetType, 0);

	PatchVFTable(vftable, (void*)GetTudigongShockEffectName, 189);

	PatchVFTable(vftable, (void*)GetTudigongAshEffectName, 190);

	ZombieWesternTudigong::StaticGetType();

	LOGI("ZombieWesternTudigong finish init");
}