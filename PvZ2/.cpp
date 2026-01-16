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
void hkTudigongActionFrame(ZombieWesternTudigong* zombie, SexyString* currentAnim, int unk1, SexyString* actionName, int unk2) 
{
	typedef void* (*gargAF)(ZombieWesternTudigong*, SexyString*, int, SexyString*, int);
	gargAF gargActionFrame = (gargAF)getActualOffset(0x7A2550);
	gargActionFrame(zombie, currentAnim, unk1, actionName, unk2);
}
void ZombieWesternTudigong::modInit() {
	LOGI("ZombieWesternTudigong init");

	vftable = copyVFTable(getActualOffset(0x1C8F50C), 206);

	patchVFTable(vftable, (void*)ZombieWesternTudigong::getRtClass, 0);

	patchVFTable(vftable, (void*)GetTudigongShockEffectName, 189);

	patchVFTable(vftable, (void*)GetTudigongAshEffectName, 190);

	patchVFTable(vftable, (void*)hkTudigongActionFrame, 170);

	ZombieWesternTudigong::getRtClass();

	LOGI("ZombieWesternTudigong finish init");
}