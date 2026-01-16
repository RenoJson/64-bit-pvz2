#include "ZombieDarkCavalry.h"


void* ZombieDarkCavalry::vftable = __null;
Sexy::RtClass* ZombieDarkCavalry::s_rtClass = __null;;

SexyString GetCavalryShockEffectName()
{
	return "POPANIM_EFFECTS_ZOMBIE_CAVALRY_SHOCK";
}

void ZombieDarkCavalry::modInit() {
	LOGI("ZombieDarkCavalry init");

	vftable = CopyVFTable(getActualOffset(0x23DB6C8), 220);

	PatchVFTable(vftable, (void*)ZombieDarkCavalry::StaticGetType, 0);

	PatchVFTable(vftable, (void*)GetCavalryShockEffectName, 189);

	ZombieDarkCavalry::StaticGetType();

	LOGI("ZombieDarkCavalry finish init");
}