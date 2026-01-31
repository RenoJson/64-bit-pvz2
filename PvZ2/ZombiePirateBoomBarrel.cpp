#include "ZombiePirateBoomBarrel.h"


void* ZombiePirateBoomBarrel::vftable = __null;
Sexy::RtClass* ZombiePirateBoomBarrel::s_rtClass = __null;;

SexyString GetBomberShockEffectName()
{
	return "POPANIM_EFFECTS_ZOMBIE_JOURNEY_TO_THE_WEST_MINER_SHOCK";
}
SexyString GetBomberAshEffectName()
{
	return "POPANIM_EFFECTS_ZOMBIE_JOURNEY_TO_THE_WEST_MINER_ASH";
}
bool isShrunkenToObvillion() {
	return false;
}
void ZombiePirateBoomBarrel::modInit() {
	LOGI("ZombiePirateBoomBarrel init");

	vftable = CopyVFTable(getActualOffset(0x23F5628), 230);

	PatchVFTable(vftable, (void*)ZombiePirateBoomBarrel::StaticGetType, 0);

	PatchVFTable(vftable, (void*)GetBomberShockEffectName, 189);

	PatchVFTable(vftable, (void*)GetBomberAshEffectName, 190);

	PatchVFTable(vftable, (void*)isShrunkenToObvillion, 200);

	ZombiePirateBoomBarrel::StaticGetType();

	LOGI("ZombiePirateBoomBarrel finish init");
}