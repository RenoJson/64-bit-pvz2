
#include "ZombieModernPoleVaulter.h"
#include "ZombieStateHelper.h"
#include "ZombieModernPoleVaulterProps.h"
#include "ZombieState.h"
#include "StateMachineBuilder.h"
#include "ZombieAnimRig_ModernPoleVaulter.h"
#include "Plant.h"

void* ZombieModernPoleVaulter::vftable = nullptr; Sexy::RtClass* ZombieModernPoleVaulter::s_rtClass = nullptr;;
typedef void (*zombieEnterState)(ZombieModernPoleVaulter*, int, int);
typedef Zombie* (*zombieAllowMovement)(Zombie*, int);
typedef void (*LoopWalk)(ZombieModernPoleVaulter*);
typedef Plant* (*getTarg)(ZombieModernPoleVaulter*);
typedef bool (*isDeadOrDying)(ZombieModernPoleVaulter*);
typedef void (*setSpeed)(ZombieAnimRig_ModernPoleVaulter*, float);
DECLARE_DELEGATES_SETUP(ZombieModernPoleVaulter)

static Sexy::DelegateBase jumpingCompletedDelegate;

static Sexy::DelegateBase bonkingCompletedDelegate;

float PoleGetWalkSpeed(ZombieModernPoleVaulter* zombie)
{
	auto rig = reinterpret_cast<ZombieAnimRig_ModernPoleVaulter*>(zombie->m_animRig.Get());
	auto props = reinterpret_cast<ZombieModernPoleVaulterProps*>(zombie->m_propertySheet.Get());
	if (rig->m_hasPole == true) {
		return props->RunningSpeed;
	}
	else {
		return zombie->m_walkSpeed;
	}
}
void PoleOnSpawn(ZombieModernPoleVaulter* zombie)
{
	auto rig = reinterpret_cast<ZombieAnimRig_ModernPoleVaulter*>(zombie->m_animRig.Get());
	rig->m_hasPole = true;
	typedef void (*zombieFun49)(ZombieModernPoleVaulter*);
	((zombieFun49)getActualOffset(0xC3D1F0))(zombie); 
	((setSpeed)getActualOffset(0x8DDAA4))(rig, PoleGetWalkSpeed(zombie));
}


void PoleWalkOnLoop(ZombieModernPoleVaulter* zombie)
{
	isDeadOrDying isDeadFunc = (isDeadOrDying)getActualOffset(0xC3E204);
	if (isDeadFunc(zombie)) {
	
		((LoopWalk)getActualOffset(0xC506B4))(zombie);
		return;
	}

	auto rig = reinterpret_cast<ZombieAnimRig_ModernPoleVaulter*>(zombie->m_animRig.Get());
	if (rig->m_hasPole == true) {
		((setSpeed)getActualOffset(0x8DDAA4))(rig, PoleGetWalkSpeed(zombie));
	}
	if (rig == nullptr || !rig->m_hasPole) {
		((LoopWalk)getActualOffset(0xC506B4))(zombie);
		return;
	}

	getTarg getTarget = (getTarg)getActualOffset(0xC41910);
	Plant* target = getTarget(zombie);

	if (target != nullptr)
	{
		if (target->IsType(PlantGroup::StaticGetType()))
		{
			auto* plantGroup = reinterpret_cast<PlantGroup*>(target);
			bool hasTallPlant = false;

			for (auto& weakPlant : plantGroup->m_plants.m_plants)
			{
				Plant* p = weakPlant.Get();

				if (p != nullptr)
				{
					auto* pProps = reinterpret_cast<PlantPropertySheet*>(p->m_propertySheet.Get());

					if (pProps != nullptr && pProps->Height == BoardEntityHeight::tall)
					{
						hasTallPlant = true;
						break;
					}
				}
			}

			if (hasTallPlant) {
				((zombieEnterState)getActualOffset(0xC3D428))(zombie, 17, 0);
			}
			else {
				((zombieEnterState)getActualOffset(0xC3D428))(zombie, 16, 0);
			}
		}
	}
	else
	{
		((LoopWalk)getActualOffset(0xC506B4))(zombie);
	}
}

void ZombieModernPoleVaulter::JumpOnEnter(ZombieModernPoleVaulter* zombie)
{
	LOGI("Entering jumping");
	((zombieAllowMovement)getActualOffset(0xC51F94))(zombie, 1);
	LOGI("Entering jumping 1");
	return RegisterEventAfterAnim(zombie, "jump", "onJumpingCompleted");
}
void ZombieModernPoleVaulter::JumpOnLoop(ZombieModernPoleVaulter* zombie)
{

}
void ZombieModernPoleVaulter::JumpOnExit(ZombieModernPoleVaulter* zombie)
{

}

void ZombieModernPoleVaulter::BonkOnEnter(ZombieModernPoleVaulter* zombie)
{
	LOGI("Entering bonking");
	((zombieAllowMovement)getActualOffset(0xC51F94))(zombie, 1);
	LOGI("Entering bonking 1");
	return RegisterEventAfterAnim(zombie, "jump_tallnut", "onBonkingCompleted");
}

void ZombieModernPoleVaulter::BonkOnLoop(ZombieModernPoleVaulter* zombie)
{

}

void ZombieModernPoleVaulter::BonkOnExit(ZombieModernPoleVaulter* zombie)
{

}
void JumpingCompletedCallback(Zombie* zombie) {
	LOGI("jumping finished");
	auto rig = reinterpret_cast<ZombieAnimRig_ModernPoleVaulter*>(zombie->m_animRig.Get());
	ZombieModernPoleVaulter* poleZombie = static_cast<ZombieModernPoleVaulter*>(zombie);
	if (poleZombie) {
		rig->m_hasPole = false;
		((zombieEnterState)getActualOffset(0xC3D428))(poleZombie, 1, 0);
		((setSpeed)getActualOffset(0x8DDAA4))(rig, PoleGetWalkSpeed(poleZombie));
	}
}
void BonkingCompletedCallback(Zombie* zombie) {
	LOGI("bonking finished");
	auto rig = reinterpret_cast<ZombieAnimRig_ModernPoleVaulter*>(zombie->m_animRig.Get());
	ZombieModernPoleVaulter* poleZombie = static_cast<ZombieModernPoleVaulter*>(zombie);
	if (poleZombie) {
		rig->m_hasPole = false;
		((zombieEnterState)getActualOffset(0xC3D428))(poleZombie, 1, 0);
		((setSpeed)getActualOffset(0x8DDAA4))(rig, PoleGetWalkSpeed(poleZombie));
	}
}
void ZombieModernPoleVaulter::ModInit() {
	LOGI("ZombiePole mod init");

	vftable = CreateChildVFTable(204 + 9, getActualOffset(0x241D430), 204);
	PatchVFTable(vftable, (void*)ZombieModernPoleVaulter::StaticGetType, 0);

	PatchVFTable(vftable, (void*)PoleOnSpawn, 49);
	PatchVFTable(vftable, (void*)PoleGetWalkSpeed, 118);
	PatchVFTable(vftable, (void*)PoleWalkOnLoop, 124);

	PatchVFTable(vftable, (void*)ZombieModernPoleVaulter::JumpOnEnter, 204);
	PatchVFTable(vftable, (void*)ZombieModernPoleVaulter::JumpOnLoop, 205);
	PatchVFTable(vftable, (void*)ZombieModernPoleVaulter::JumpOnExit, 206);

	PatchVFTable(vftable, (void*)ZombieModernPoleVaulter::BonkOnEnter, 207);
	PatchVFTable(vftable, (void*)ZombieModernPoleVaulter::BonkOnLoop, 208);
	PatchVFTable(vftable, (void*)ZombieModernPoleVaulter::BonkOnExit, 209);

	ZombieModernPoleVaulter::StaticGetType();
	LOGI("ZombiePole finish init");
}

void ZombieModernPoleVaulter::buildEventCallbacks(Reflection::CRefManualSymbolBuilder* builder, Reflection::RClass* rtClass)
{
	IF_CALLBACK_NOTSETUP(ZombieModernPoleVaulter) {
		SetupLiteralDelegate(&jumpingCompletedDelegate, JumpingCompletedCallback);
		SetupLiteralDelegate(&bonkingCompletedDelegate, BonkingCompletedCallback);
		ZombieModernPoleVaulter_delegatesSetup = true;
		LOGI("SO TRUE");
	}
	RegisterEventCallback(builder, rtClass, "onJumpingCompleted", jumpingCompletedDelegate);
	RegisterEventCallback(builder, rtClass, "onBonkingCompleted", bonkingCompletedDelegate);
	LOGI("Reg event complete");
}


void ZombieModernPoleVaulter::buildStates()
{
	StateMachineTableBuilder* stateMachine = CallGetStateMachine(ZombieModernPoleVaulter::StaticGetType());
	RegisterStateByOffsets(stateMachine,
		16,
		(uintptr_t)ZombieModernPoleVaulter::JumpOnEnter,
		(uintptr_t)ZombieModernPoleVaulter::JumpOnLoop,
		(uintptr_t)ZombieModernPoleVaulter::JumpOnExit,
		"ZS_PoleVaulter_Jumping");
	RegisterStateByOffsets(stateMachine,
		17,
		(uintptr_t)ZombieModernPoleVaulter::BonkOnEnter,
		(uintptr_t)ZombieModernPoleVaulter::BonkOnLoop,
		(uintptr_t)ZombieModernPoleVaulter::BonkOnExit,
		"ZS_PoleVaulter_Bonking");
	LOGI("Reg state complete");
}
