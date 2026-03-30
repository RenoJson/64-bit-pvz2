#include "ZombieBasic.h"
#include "ZombieBasicProps.h"
#include "ZombieAnimRig_Gargantuar.h"
#include "ZombieAnimRig_Basic.h"

void* ZombieCowboyBasicVeteran::vftable = __null;
Sexy::RtClass* ZombieCowboyBasicVeteran::s_rtClass = __null;;

void COnSpawn(ZombieCowboyBasicVeteran* zombie) {
	auto* rig = reinterpret_cast<ZombieAnimRig_CowboyBasic*>(zombie->m_animRig.Get());
	rig->m_dancing = false;
	typedef void (*zombieFun49)(ZombieCowboyBasicVeteran*);
	((zombieFun49)getActualOffset(0xC3D1F0))(zombie);
}
float CGetWalkSpeed(ZombieCowboyBasicVeteran* zombie) {
	auto* rig = reinterpret_cast<ZombieAnimRig_CowboyBasic*>(zombie->m_animRig.Get());
	auto* props = reinterpret_cast<ZombieCowboyVeteranProps*>(zombie->m_propertySheet.Get());
	if (rig->m_dancing == true) {
		return props->DancingSpeed;
	}
	else {
		return zombie->m_walkSpeed;
	}
}
void CWalkOnLoop(ZombieCowboyBasicVeteran* zombie) {
	auto* rig = reinterpret_cast<ZombieAnimRig_CowboyBasic*>(zombie->m_animRig.Get());
	auto* props = reinterpret_cast<ZombieCowboyVeteranProps*>(zombie->m_propertySheet.Get());
	if (rig->m_dancing == true) {
		typedef void (*setSpeed)(ZombieAnimRig_CowboyBasic*, float);
		((setSpeed)getActualOffset(0x8DDAA4))(rig, CGetWalkSpeed(zombie));

		typedef Zombie* (*setSpeedScale)(ZombieCowboyBasicVeteran*, float);
		((setSpeedScale)getActualOffset(0xC484C0))(zombie, props->DancingSpeedScale);
	}
	else {
		typedef void (*setSpeed)(ZombieAnimRig_CowboyBasic*, float);
		((setSpeed)getActualOffset(0x8DDAA4))(rig, CGetWalkSpeed(zombie));

		typedef Zombie* (*setSpeedScale)(ZombieCowboyBasicVeteran*, float);
		((setSpeedScale)getActualOffset(0xC484C0))(zombie, 1);
	}
	typedef void (*zombieFun124)(ZombieCowboyBasicVeteran*);
	((zombieFun124)getActualOffset(0xC506B4))(zombie);
}
Zombie* CWalkOnExit(ZombieCowboyBasicVeteran* zombie) {
	auto* rig = reinterpret_cast<ZombieAnimRig_CowboyBasic*>(zombie->m_animRig.Get());
	auto* props = reinterpret_cast<ZombieCowboyVeteranProps*>(zombie->m_propertySheet.Get());
	if (rig->m_dancing == true) {
		typedef void (*setSpeed)(ZombieAnimRig_CowboyBasic*, float);
		((setSpeed)getActualOffset(0x8DDAA4))(rig, CGetWalkSpeed(zombie));

		typedef Zombie* (*setSpeedScale)(ZombieCowboyBasicVeteran*, float);
		((setSpeedScale)getActualOffset(0xC484C0))(zombie, props->DancingSpeedScale);
	}
	else {
		typedef void (*setSpeed)(ZombieAnimRig_CowboyBasic*, float);
		((setSpeed)getActualOffset(0x8DDAA4))(rig, CGetWalkSpeed(zombie));

		typedef Zombie* (*setSpeedScale)(ZombieCowboyBasicVeteran*, float);
		((setSpeedScale)getActualOffset(0xC484C0))(zombie, 1);
	}
	typedef Zombie* (*zombieFun125)(ZombieCowboyBasicVeteran*);
	return ((zombieFun125)getActualOffset(0xC50714))(zombie);
}
void ZombieCowboyBasicVeteran::modInit() {
	LOGI("ZombieCowboyBasicVeteran init");

	vftable = CopyVFTable(getActualOffset(0x23DEFB8), 206);

	PatchVFTable(vftable, (void*)ZombieCowboyBasicVeteran::StaticGetType, 0);

	PatchVFTable(vftable, (void*)CWalkOnLoop, 124);

	PatchVFTable(vftable, (void*)CWalkOnExit, 125);

	ZombieCowboyBasicVeteran::StaticGetType();

	LOGI("ZombieCowboyBasicVeteran finish init");
}