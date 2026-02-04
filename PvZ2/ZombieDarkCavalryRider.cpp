#include "ZombieDarkCavalryRider.h"


void* ZombieDarkCavalryRider::vftable = __null;
Sexy::RtClass* ZombieDarkCavalryRider::s_rtClass = __null;;

SexyString GetRiderShockEffectName()
{
	return "POPANIM_EFFECTS_ZOMBIE_SHOCK";
}
SexyString GetRiderAshEffectName()
{
	return "POPANIM_EFFECTS_ZOMBIE_ASH";
}
bool isRiderShrunkenToObvillion() {
	return false;
}
void ZombieDarkCavalryRider::modInit() {
	LOGI("ZombieDarkCavalryRider init");

	vftable = CopyVFTable(getActualOffset(0x23D3480), 227);

	PatchVFTable(vftable, (void*)ZombieDarkCavalryRider::StaticGetType, 0);

	PatchVFTable(vftable, (void*)GetRiderShockEffectName, 189);

	PatchVFTable(vftable, (void*)GetRiderAshEffectName, 190);

	PatchVFTable(vftable, (void*)isRiderShrunkenToObvillion, 200);

	ZombieDarkCavalryRider::StaticGetType();

	LOGI("ZombieDarkCavalryRider finish init");
}