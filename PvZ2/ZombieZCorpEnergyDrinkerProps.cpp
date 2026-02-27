#include "ZombieTombRaiserProps.h"

void* ZombieZCorpEnergyDrinkerProps::vftable = __null;
Sexy::RtClass* ZombieZCorpEnergyDrinkerProps::s_rtClass = __null;


void ZombieZCorpEnergyDrinkerProps::modInit() {
	LOGI("ZombieZCorpEnergyDrinkerProps init");

	vftable = CopyVFTable(getActualOffset(0x23E1138), 14);

	PatchVFTable(vftable, (void*)ZombieZCorpEnergyDrinkerProps::StaticGetType, 0);

	ZombieZCorpEnergyDrinkerProps::StaticGetType();

	LOGI("ZombieZCorpEnergyDrinkerProps finish init");
}