#include "ZombieDinoBasic.h"
#include "ZombieDinoBullyVeteranProps.h"
#include "DamageInfo.h"
#include "ZombieConditions.h"

void* ZombieJourneyToTheWestGargantuar::vftable = __null;
Sexy::RtClass* ZombieJourneyToTheWestGargantuar::s_rtClass = __null;;
void* GargTakeDamage(ZombieJourneyToTheWestGargantuar* thisPtr, DamageInfo* damageInfo)
{
    auto* props = reinterpret_cast<ZombieJourneyToTheWestGargantuarProps*>(thisPtr->m_propertySheet.Get());
    DamageInfo newDmgInfo = *damageInfo;
    float reductionMultiplier = 1.0f - (props->DamageReduction / 100.0f);
	newDmgInfo.m_damage = damageInfo->m_damage * reductionMultiplier;
    typedef void* (*funcC43B90)(ZombieJourneyToTheWestGargantuar*, DamageInfo*);
    static auto* ZTakeDmg = ((funcC43B90)getActualOffset(0xC43B90));
    return ZTakeDmg(thisPtr, &newDmgInfo);
}
SexyString GetGargShockEffectName()
{
	return "POPANIM_EFFECTS_ZOMBIE_BRONZE_SHOCK";
}
SexyString GetGargAshEffectName()
{
	return "POPANIM_EFFECTS_ZOMBIE_BRONZE_ASH";
}

void ZombieJourneyToTheWestGargantuar::modInit() {
	LOGI("ZombieJourneyToTheWestGargantuar init");

	vftable = CopyVFTable(getActualOffset(0x23E4CC8), 215);

	PatchVFTable(vftable, (void*)ZombieJourneyToTheWestGargantuar::StaticGetType, 0);

	PatchVFTable(vftable, (void*)GargTakeDamage, 35);

	PatchVFTable(vftable, (void*)GetGargShockEffectName, 189);

	PatchVFTable(vftable, (void*)GetGargAshEffectName, 190);

	ZombieJourneyToTheWestGargantuar::StaticGetType();

	LOGI("ZombieJourneyToTheWestGargantuar finish init");
}