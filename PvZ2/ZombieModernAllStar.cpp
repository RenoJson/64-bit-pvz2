#include "ZombieModernAllStar.h"
#include "ZombieStateHelper.h"
#include "ZombieState.h"
#include "StateMachineBuilder.h"
#include "Plant.h"
#include "DamageInfo.h"
#include "ZombieImp.h"

void* ZombieModernBerserker::vftable = nullptr;
Sexy::RtClass* ZombieModernBerserker::s_rtClass = nullptr;;
typedef void (*zombieEnterState)(ZombieModernBerserker*, int, int);
typedef void (*LoopWalk)(ZombieModernBerserker*);
typedef void (*LoopEat)(ZombieModernBerserker*);
typedef void (*ActionFrame)(ZombieModernBerserker*, int64_t, SexyString*, int64_t, SexyString*);
typedef void (*killTarg)(BoardEntity*, ZombieModernBerserker*);
typedef BoardEntity* (*getTarg)(ZombieModernBerserker*);
typedef void (*setSpeedScale)(ZombieModernBerserker*, float);
typedef BoardEntity* (*getTarg)(ZombieModernBerserker*);

DECLARE_DELEGATES_SETUP(ZombieModernBerserker)

static Sexy::DelegateBase chargeCompletedDelegate;

void BerserkerLoopWalk(ZombieModernBerserker* zombie)
{
	auto props = reinterpret_cast<ZombieModernBerserkerProps*>(zombie->m_propertySheet.Get());
	if (props->ChargeCooldown <= 0.0f || zombie->m_elapsedTimeInState <= props->ChargeCooldown) {
		((LoopWalk)getActualOffset(0xBB9430))(zombie);
	}
	else {
		((zombieEnterState)getActualOffset(0xC3D428))(zombie, 19, 0);
	}
}
void BerserkerLoopEat(ZombieModernBerserker* zombie)
{
	auto props = reinterpret_cast<ZombieModernBerserkerProps*>(zombie->m_propertySheet.Get());
	if (props->ChargeCooldown <= 0.0f || zombie->m_elapsedTimeInState <= props->ChargeCooldown) {
		((LoopEat)getActualOffset(0xC5082C))(zombie);
	}
	else {
		((zombieEnterState)getActualOffset(0xC3D428))(zombie, 19, 0);
	}
}
float BerserkerIsFacingOrNot(Zombie* zombie) {
	if (!zombie->m_facing) {
		return 1.0f;
	}
	return -1.0f;
}
void RunOnLoop(ZombieModernBerserker* zombie)
{
	auto props = reinterpret_cast<ZombieModernBerserkerProps*>(zombie->m_propertySheet.Get());

	Rect attackRect;
	attackRect.mWidth = props->AttackRect.mWidth;
	attackRect.mHeight = props->AttackRect.mHeight;
	float zX = zombie->m_position.x; 
	float zY = zombie->m_position.y;
	float zZ = zombie->m_position.z; 
	float facing = BerserkerIsFacingOrNot(zombie);

	attackRect.mX = static_cast<int>(zX - (attackRect.mWidth / 2.0f) + (facing * props->AttackRect.mX));
	attackRect.mY = static_cast<int>((zY - zZ) - attackRect.mHeight + props->AttackRect.mY);

	typedef ZombieModernSuperfanImp* (*FindSFImpFunc)(Zombie*, Rect*);
	FindSFImpFunc findSFImp = (FindSFImpFunc)getActualOffset(0xBB8EF4);
	ZombieModernSuperfanImp* target = findSFImp(zombie, &attackRect);

	if (target != nullptr && target->m_position.x >= props->KickImpMinX) {
		Rect impSearchRect;
		impSearchRect.mX = attackRect.mX - attackRect.mWidth; 
		impSearchRect.mY = attackRect.mY;                   
		impSearchRect.mWidth = attackRect.mWidth * 2;        
		impSearchRect.mHeight = attackRect.mHeight;
		ZombieModernSuperfanImp* targetToKick = findSFImp(zombie, &impSearchRect);
		if (targetToKick != nullptr) {
			void** impVTable = *(void***)targetToKick;
			typedef void (*ZombieThrowVirtual)(Zombie*, int, float, float, float, float, float);
			ZombieThrowVirtual throwImp = (ZombieThrowVirtual)impVTable[206];

			throwImp(targetToKick, 0, zX - 100.0f, zY, zZ, 1.5f, 128.0f);
		}
	}
	else
	{
		getTarg getTarget = (getTarg)getActualOffset(0xC41910);
		if (getTarget(zombie))
		{
			((zombieEnterState)getActualOffset(0xC3D428))(zombie, 17, 0);
		}
	}
}
void RunOnExit(ZombieModernBerserker* zombie)
{
	
}
void TackleOnEnter(ZombieModernBerserker* zombie)
{
	((setSpeedScale)getActualOffset(0xC484C0))(zombie, 1);
	RegisterEventAfterAnim(zombie, "tackle", "onTackleAnimationStopped");
}

void ZombieModernBerserker::ChargeOnEnter(ZombieModernBerserker* zombie)
{
	RegisterEventAfterAnim(zombie, "charge", "onChargingCompleted");
}

void ZombieModernBerserker::ChargeOnLoop(ZombieModernBerserker* zombie)
{

}

void ZombieModernBerserker::ChargeOnExit(ZombieModernBerserker* zombie)
{

}

void ChargeCompletedCallback(Zombie* zombie)
{
	ZombieModernBerserker* poleZombie = static_cast<ZombieModernBerserker*>(zombie);
	if (poleZombie) {
		((zombieEnterState)getActualOffset(0xC3D428))(poleZombie, 16, 0);
	}
}
void ZombieModernBerserker::ModInit() {
	LOGI("ZombieBerserker mod init");

	vftable = CreateChildVFTable(213 + 9, getActualOffset(0x23F2BB8), 213);
	PatchVFTable(vftable, (void*)ZombieModernBerserker::StaticGetType, 0);

	PatchVFTable(vftable, (void*)BerserkerLoopWalk, 124);
	PatchVFTable(vftable, (void*)BerserkerLoopEat, 127);
	PatchVFTable(vftable, (void*)RunOnLoop, 205);
	PatchVFTable(vftable, (void*)RunOnExit, 206);
	PatchVFTable(vftable, (void*)TackleOnEnter, 207);

	PatchVFTable(vftable, (void*)ZombieModernBerserker::ChargeOnEnter, 213);
	PatchVFTable(vftable, (void*)ZombieModernBerserker::ChargeOnLoop, 214);
	PatchVFTable(vftable, (void*)ZombieModernBerserker::ChargeOnExit, 215);

	ZombieModernBerserker::StaticGetType();
	LOGI("ZombieBerserker finish init");
}

void ZombieModernBerserker::buildEventCallbacks(Reflection::CRefManualSymbolBuilder* builder, Reflection::RClass* rtClass)
{
	IF_CALLBACK_NOTSETUP(ZombieModernBerserker) {
		SetupLiteralDelegate(&chargeCompletedDelegate, ChargeCompletedCallback);
		ZombieModernBerserker_delegatesSetup = true;
		LOGI("SO TRUE");
	}
	RegisterEventCallback(builder, rtClass, "onChargingCompleted", chargeCompletedDelegate);
	LOGI("Reg event complete");
}


void ZombieModernBerserker::buildStates()
{
	StateMachineTableBuilder* stateMachine = CallGetStateMachine(ZombieModernBerserker::StaticGetType());
	RegisterStateByOffsets(stateMachine,
		19,
		(uintptr_t)ZombieModernBerserker::ChargeOnEnter,
		(uintptr_t)ZombieModernBerserker::ChargeOnLoop,
		(uintptr_t)ZombieModernBerserker::ChargeOnExit,
		"ZS_Berserker_Charging");
	LOGI("Reg state complete");
}

