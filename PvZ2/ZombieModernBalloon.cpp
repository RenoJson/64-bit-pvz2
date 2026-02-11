#include "ZombieModernBalloon.h"
#include "ZombieJourneyToTheWestBalloonProps.h"
#include "ZombieAnimRig_ModernBalloon.h";
#include "DamageInfo.h"

void* ZombieJourneyToTheWestBalloon::vftable = __null;
Sexy::RtClass* ZombieJourneyToTheWestBalloon::s_rtClass = __null;;


void* BallonTakeDamage(ZombieJourneyToTheWestBalloon* thisPtr, DamageInfo* damageInfo)
{
	auto* props = reinterpret_cast<ZombieJourneyToTheWestBalloonProps*>(thisPtr->m_propertySheet.Get());

	bool shouldTriggerCatastrophic = false;

	if (damageInfo->m_damage >= props->DamageAmountWhichAlsoKillsBasic)
	{
		shouldTriggerCatastrophic = true;
	}
	else
	{
		shouldTriggerCatastrophic = false;
	}

	if (shouldTriggerCatastrophic)
	{
		thisPtr->m_hasTakenCatastrophicDamage = true;
	}
	typedef void* (*funcC43B90)(ZombieJourneyToTheWestBalloon*, DamageInfo*);
	static auto* ZTakeDmg = ((funcC43B90)getActualOffset(0xC43B90));
	return ZTakeDmg(thisPtr, damageInfo);
}
SexyString hkJFixAnimShock(ZombieJourneyToTheWestBalloon* zombie, DamageInfo* damage) {
	auto* getProps = reinterpret_cast<ZombieJourneyToTheWestBalloonProps*>(zombie->m_propertySheet.Get());
	LOGI("Calling func");
	if (zombie->m_hasTakenCatastrophicDamage == true) {
		LOGI("m_hasBalloon is true");
		return getProps->OnAirShockAnimName;
	}
	else {
		LOGI("m_hasBalloon is false");
		return "POPANIM_EFFECTS_ZOMBIE_SHOCK";
	}
}

SexyString hkJFixAnimAsh(ZombieJourneyToTheWestBalloon* zombie, DamageInfo* damage) {
	auto* getProps = reinterpret_cast<ZombieJourneyToTheWestBalloonProps*>(zombie->m_propertySheet.Get());
	LOGI("Calling func");
	if (zombie->m_hasTakenCatastrophicDamage == true) {
		LOGI("m_hasBalloon is true");
		return getProps->OnAirAshAnimName;
	}
	else {
		LOGI("m_hasBalloon is false");
		return "POPANIM_EFFECTS_ZOMBIE_ASH";
	}
}

void ZombieJourneyToTheWestBalloon::modInit() {
	LOGI("ZombieJourneyToTheWestBalloon init");

	vftable = CopyVFTable(getActualOffset(0x23F39D8), 216);

	PatchVFTable(vftable, (void*)ZombieJourneyToTheWestBalloon::StaticGetType, 0);

	PatchVFTable(vftable, (void*)BallonTakeDamage, 35);

	PatchVFTable(vftable, (void*)hkJFixAnimShock, 189);

	PatchVFTable(vftable, (void*)hkJFixAnimAsh, 190);

	ZombieJourneyToTheWestBalloon::StaticGetType();
	LOGI("ZombieJourneyToTheWestBalloon finish init");
}


