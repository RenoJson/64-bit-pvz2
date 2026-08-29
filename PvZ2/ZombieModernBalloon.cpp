#include "ZombieModernBalloon.h"
#include "ZombieJourneyToTheWestBalloonProps.h"
#include "ZombieAnimRig_ModernBalloon.h";
#include "DamageInfo.h"
#include "ZombieHelper.h"

void* ZombieJourneyToTheWestBalloon::vftable = __null;
Sexy::RtClass* ZombieJourneyToTheWestBalloon::s_rtClass = __null;;


void BallonOnArmorDestroyed(ZombieJourneyToTheWestBalloon* zombie, int a2, SexyString* armorName)
{

}
void BallonOnHealthChanged(ZombieJourneyToTheWestBalloon* zombie) {
    if (!ZombieIsDeadOrDying(zombie) && zombie->m_entityState.m_id != 3) {
        CallFunc<void>(0xBBBAF0, zombie);
    }
}

SexyString hkJFixAnimShock(ZombieJourneyToTheWestBalloon* zombie) {
    auto* getProps = reinterpret_cast<ZombieJourneyToTheWestBalloonProps*>(zombie->m_propertySheet.Get());
    auto rig = reinterpret_cast<ZombieAnimRig_ModernBalloon*>(zombie->m_animRig.Get());
    if (rig->m_hasBalloon == true) {
		return getProps->OnAirShockAnimName;
	}
	else {
		return getProps->ShockAnimName;
	}
}

SexyString hkJFixAnimAsh(ZombieJourneyToTheWestBalloon* zombie) {
	auto* getProps = reinterpret_cast<ZombieJourneyToTheWestBalloonProps*>(zombie->m_propertySheet.Get()); 
    auto rig = reinterpret_cast<ZombieAnimRig_ModernBalloon*>(zombie->m_animRig.Get());
    if (rig->m_hasBalloon == true) {
		return getProps->OnAirAshAnimName;
	}
	else {
		return getProps->AshAnimName;
	}
}

void ZombieJourneyToTheWestBalloon::modInit() {
	LOGI("ZombieJourneyToTheWestBalloon init");

	vftable = CopyVFTable(getActualOffset(0x23F39D8), 216);

	PatchVFTable(vftable, (void*)ZombieJourneyToTheWestBalloon::StaticGetType, 0);

    PatchVFTable(vftable, (void*)BallonOnArmorDestroyed, 115);

    PatchVFTable(vftable, (void*)BallonOnHealthChanged, 184);

	PatchVFTable(vftable, (void*)hkJFixAnimShock, 189);

	PatchVFTable(vftable, (void*)hkJFixAnimAsh, 190);

	ZombieJourneyToTheWestBalloon::StaticGetType();
	LOGI("ZombieJourneyToTheWestBalloon finish init");
}


