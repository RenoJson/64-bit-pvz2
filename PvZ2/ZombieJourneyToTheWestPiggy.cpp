#include "ZombieJourneyToTheWestPiggy.h"
#include "ZombieJourneyToTheWestPiggyProps.h"
void* ZombieJourneyToTheWestPiggy::vftable = nullptr; 
Sexy::RtClass* ZombieJourneyToTheWestPiggy::s_rtClass = nullptr;;

float hkPigGetWalkSpeed(ZombieJourneyToTheWestPiggy* zombie) {
	auto* props = reinterpret_cast<ZombieJourneyToTheWestPiggyProps*>(zombie->m_propertySheet.Get());
	float minVal = props->SpeedRateModifier.Min;
	float maxVal = props->SpeedRateModifier.Max;
	zombie->m_speedModifier = minVal + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (maxVal - minVal)));
	return zombie->m_walkSpeed * zombie->m_speedModifier;
}
SexyString GetPigShockEffectName()
{
	return "POPANIM_EFFECTS_ZOMBIE_PIG_SHOCK";
}
SexyString GetPigAshEffectName()
{
	return "POPANIM_EFFECTS_ZOMBIE_PIG_ASH";
}
bool PigIsShrunkenToObvillion() {
	return true;
}
void ZombieJourneyToTheWestPiggy::ModInit() {
	LOGI("ZombiePiggy mod init");

	vftable = CopyVFTable(getActualOffset(0x241D430), 204);
	PatchVFTable(vftable, (void*)ZombieJourneyToTheWestPiggy::StaticGetType, 0);
	PatchVFTable(vftable, (void*)hkPigGetWalkSpeed, 118);
	PatchVFTable(vftable, (void*)GetPigShockEffectName, 189);
	PatchVFTable(vftable, (void*)GetPigAshEffectName, 190);
	PatchVFTable(vftable, (void*)PigIsShrunkenToObvillion, 200);
	ZombieJourneyToTheWestPiggy::StaticGetType();
	LOGI("ZombiePiggy finish init");
}

