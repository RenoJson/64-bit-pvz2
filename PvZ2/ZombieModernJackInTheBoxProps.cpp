#include "ZombieModernJackInTheBoxProps.h"

void* ZombieModernJackInTheBoxProps::vftable = __null;
Sexy::RtClass* ZombieModernJackInTheBoxProps::s_rtClass = __null;

void* TimerExplosionProps::vftable = __null;
Sexy::RtClass* TimerExplosionProps::s_rtClass = __null;


void TimerExplosionProps::modInit() {
	LOGI("TimerExplosionProps init");

	vftable = CopyVFTable(getActualOffset(0x2428660), 14);

	PatchVFTable(vftable, (void*)TimerExplosionProps::StaticGetType, 0);

	TimerExplosionProps::StaticGetType();

	LOGI("TimerExplosionProps finish init");
}

void ZombieModernJackInTheBoxProps::modInit() {
	LOGI("ZombieModernJackInTheBoxProps init");

	vftable = CopyVFTable(getActualOffset(0x24328D0), 14);

	PatchVFTable(vftable, (void*)ZombieModernJackInTheBoxProps::StaticGetType, 0);

	ZombieModernJackInTheBoxProps::StaticGetType();

	LOGI("ZombieModernJackInTheBoxProps finish init");
}
