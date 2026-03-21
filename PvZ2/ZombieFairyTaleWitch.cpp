#include "ZombieWithAction.h"

DEFINE_STATIC_RT_CLASS_MEMBERS(ZombieFairyTaleWitch)

SexyString GetWitchShockEffectName()
{
	return "POPANIM_EFFECTS_ZOMBIE_SHOCK";
}
SexyString GetWitchAshEffectName()
{
	return "POPANIM_EFFECTS_ZOMBIE_ASH";
}
void ZombieFairyTaleWitch::modInit() {
	LOGI("ZombieFairyTaleWitch init");

	vftable = CopyVFTable(getActualOffset(0x23D9848), 215);

	PatchVFTable(vftable, (void*)ZombieFairyTaleWitch::StaticGetType, 0);

	PatchVFTable(vftable, (void*)GetWitchShockEffectName, 189);

	PatchVFTable(vftable, (void*)GetWitchAshEffectName, 190);

	ZombieFairyTaleWitch::StaticGetType();

	LOGI("ZombieFairyTaleWitch finish init");
}
