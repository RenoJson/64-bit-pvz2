#include "ZombieFutureJetpack.h"
#include "ZombieFutureJetpackProps.h"
void* ZombieFutureJetpackVeteran::vftable = nullptr;
Sexy::RtClass* ZombieFutureJetpackVeteran::s_rtClass = nullptr;;

Zombie* overrideJetOnSpawn(ZombieFutureJetpackVeteran* zombie) {
	zombie->m_firstBoosted = true;
	typedef void (*zombieFun49)(ZombieFutureJetpackVeteran*);
	((zombieFun49)getActualOffset(0xC3D1F0))(zombie);

}
float hkJetGetWalkSpeed(ZombieFutureJetpackVeteran* zombie) {
	auto* props = reinterpret_cast<ZombieFutureJetpackVeteranProps*>(zombie->m_propertySheet.Get());
	if (zombie->m_firstBoosted == true) {
		return zombie->m_walkSpeed * props->SpeedScaleWhenFlyOverPlant;
	}
	else {
		return zombie->m_walkSpeed;
	}
}
Zombie* overrideFunction208(ZombieFutureJetpackVeteran* zombie) {
	typedef Zombie* (*zombieFun208)(ZombieFutureJetpackVeteran*);
	((zombieFun208)getActualOffset(0xB449C8))(zombie);
	zombie->m_firstBoosted = false;
}
void ZombieFutureJetpackVeteran::modInit() {
	LOGI("ZombieJet mod init");

	vftable = CopyVFTable(getActualOffset(0x23EFF98), 210);
	PatchVFTable(vftable, (void*)ZombieFutureJetpackVeteran::StaticGetType, 0);

	PatchVFTable(vftable, (void*)overrideJetOnSpawn, 49);

	PatchVFTable(vftable, (void*)hkJetGetWalkSpeed, 118);

	PatchVFTable(vftable, (void*)overrideFunction208, 208);

	ZombieFutureJetpackVeteran::StaticGetType();
	LOGI("ZombieJet finish init");
}

void* ZombieFutureJetpackVeteranProps::vftable = __null;
Sexy::RtClass* ZombieFutureJetpackVeteranProps::s_rtClass = __null;


void ZombieFutureJetpackVeteranProps::modInit() {
	LOGI("ZombieFutureJetpackVeteranProps init");

	vftable = CopyVFTable(getActualOffset(0x2433408), 14);

	PatchVFTable(vftable, (void*)ZombieFutureJetpackVeteranProps::StaticGetType, 0);

	ZombieFutureJetpackVeteranProps::StaticGetType();

	LOGI("ZombieFutureJetpackVeteranProps finish init");
}
