#include "ZombieBasicProps.h"

void* ZombieBasicProps::vftable = __null;
Sexy::RtClass* ZombieBasicProps::s_rtClass = __null;


void ZombieBasicProps::modInit() {
	LOGI("ZombieBasicProps init");

	vftable = CopyVFTable(getActualOffset(0x24328D0), 14);

	PatchVFTable(vftable, (void*)ZombieBasicProps::StaticGetType, 0);

	ZombieBasicProps::StaticGetType();

	LOGI("ZombieBasicProps finish init");
}

void* ZombieCowboyVeteranProps::vftable = __null;
Sexy::RtClass* ZombieCowboyVeteranProps::s_rtClass = __null;


void ZombieCowboyVeteranProps::modInit() {
	LOGI("ZombieBasicProps init");

	vftable = CopyVFTable(getActualOffset(0x24328D0), 14);

	PatchVFTable(vftable, (void*)ZombieCowboyVeteranProps::StaticGetType, 0);

	ZombieCowboyVeteranProps::StaticGetType();

	LOGI("ZombieBasicProps finish init");
}