#include "Zombie_BullVeteran.h";
#include "ZombieBull.h"
#include "ZombieHelper.h"
#include "DamageInfo.h"

Reflection::CRefManualSymbolBuilder::BuildSymbolsFunc ZombieBullVeteranProps::oZombieBullVeteranPropsBuildSymbols = nullptr;

void VetBullFunction204(ZombieBullVeteran* self, BoardEntity* entity) {
	if(entity && !CallFunc<bool, ZombieBullVeteran*>(0xC419D4, self)) {
		auto props = reinterpret_cast<ZombieBullVeteranProps*>(self->m_propertySheet.Get());
		DamageInfo dmgInfo;
		dmgInfo.m_damage = props->BullRammingDamage;
		dmgInfo.m_flags = DamageTypeFlags::damage_crushing; 
		void** vtable = *(void***)entity;
		typedef void (*VirtualTakeDamageFunc)(BoardEntity*, DamageInfo*);
		VirtualTakeDamageFunc takeDmg = (VirtualTakeDamageFunc)vtable[35];
		takeDmg(entity, &dmgInfo);
	}
	if (!ZombieIsDeadOrDying(self)) {
		ZombieEnterState(self, 20, 0);
	}
}
void VetBullWalkOnLoop(ZombieBullVeteran* self) {
	auto props = reinterpret_cast<ZombieBullVeteranProps*>(self->m_propertySheet.Get());
	if (props->BullRunAgainDelay <= 0.0f || self->m_elapsedTimeInState <= props->BullRunAgainDelay) {
		CallFunc<void, ZombieBullVeteran*>(0xC506B4, self);
	}
	else {
		ZombieEnterState(self, 17, 0);
	}
}
void ZombieBullVeteranProps::modInit() {
	LOGI("init bull vet props");
	PVZ2HookFunction(0xAE6AA8, (void*)VetBullWalkOnLoop, nullptr);
	PVZ2HookFunction(0xAE67BC, (void*)VetBullFunction204, nullptr);
	PVZ2HookFunction(0xDAE6CC, (void*)construct, nullptr);
	PVZ2HookFunction(0xDAE804, (void*)ZombieBullVeteranProps::buildSymbols, (void**)&ZombieBullVeteranProps::oZombieBullVeteranPropsBuildSymbols);
	LOGI("finish init bull vet");
}