#include "Zombie_BullVeteran.h";

Reflection::CRefManualSymbolBuilder::BuildSymbolsFunc ZombieBullVeteranProps::oZombieBullVeteranPropsBuildSymbols = nullptr;

void ZombieBullVeteranProps::modInit() {
	LOGI("init bull vet props");
	PVZ2HookFunction(0xDAE6CC, (void*)construct, nullptr);
	PVZ2HookFunction(0xDAE804, (void*)ZombieBullVeteranProps::buildSymbols, (void**)&ZombieBullVeteranProps::oZombieBullVeteranPropsBuildSymbols);
	LOGI("finish init bull vet");
}