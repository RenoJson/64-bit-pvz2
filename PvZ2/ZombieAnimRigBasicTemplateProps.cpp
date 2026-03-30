#include "ZombieAnimRigBasicTemplateProps.h"

void* ZombieAnimRigTemplateConfig::vftable = __null;
Sexy::RtClass* ZombieAnimRigTemplateConfig::s_rtClass = __null;


void ZombieAnimRigTemplateConfig::modInit() {
	LOGI("ZombieAnimRigTemplateConfig init");

	vftable = CopyVFTable(getActualOffset(0x2428660), 14);

	PatchVFTable(vftable, (void*)ZombieAnimRigTemplateConfig::StaticGetType, 0);

	ZombieAnimRigTemplateConfig::StaticGetType();

	LOGI("ZombieAnimRigTemplateConfig finish init");
}