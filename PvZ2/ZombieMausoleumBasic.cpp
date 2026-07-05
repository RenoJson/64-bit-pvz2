#include "ZombieMausoleumBasic.h"
#include "ZombieStateHelper.h"
#include "AddZombieType.h"
#include "ZombieAnimRig_Basic.h"
#include "StateMachineBuilder.h"
#include "ZombieHelper.h"

void* ZombieMausoleumSpirit::vftable = nullptr;
Sexy::RtClass* ZombieMausoleumSpirit::s_rtClass = nullptr;

void* ZombieMausoleumBasic::vftable = nullptr;
Sexy::RtClass* ZombieMausoleumBasic::s_rtClass = nullptr;

void* ZombieMausoleumBasicProps::vftable = nullptr;
Sexy::RtClass* ZombieMausoleumBasicProps::s_rtClass = nullptr;

DECLARE_DELEGATES_SETUP(ZombieMausoleumSpirit)

static Sexy::DelegateBase introCompletedDelegate;

bool MausoleumSpiritCanBeTargeted(ZombieMausoleumSpirit* zombie, char a2) {
	if (!ZombieIsInState(zombie, 2)) {
		return false;
	}
	else {
		return CallFunc<bool, Zombie*, char>(0xC4D594, zombie, a2);
	}
}

void MausoleumBasicOnCreate(ZombieMausoleumBasic* zombie) {
	auto props = reinterpret_cast<ZombieMausoleumBasicProps*>(zombie->m_propertySheet.Get());
	auto rig = reinterpret_cast<ZombieAnimRig_MausoleumBasic*>(zombie->m_animRig.Get());
	rig->m_hasCursed = props->CursedAtStart;
	SetAnimLayerVisible(rig, "zombie_eyes_curse", rig->m_hasCursed);
}

void MausoleumSpiritOnSpawn(ZombieMausoleumSpirit* zombie) {
	ZombieOnSpawn(zombie);
	ZombieEnterState(zombie, 16, 0);
}


void MausoleumBasicActionFrame(ZombieMausoleumBasic* self, SexyString* currentAnim, SexyString* actionName, SexyString* param, float nextFrameTime)
{
	if (*actionName == "ghost_intro")
	{
		auto* props = reinterpret_cast<ZombieMausoleumBasicProps*>(self->m_propertySheet.Get());
		SexyString name = props->SpiritTypeName;
		Zombie* spirit = AddZombie(name, -1, 6, -1);
		float newX = self->m_position.x;
		float newY = self->m_position.y ;
		float newZ = self->m_position.z;
		SexyVector3 newCoords = SexyVector3(newX, newY, newZ);
		ZombieSetPosition(spirit, &newCoords);
		if (ZombieHasCondition(self, zombie_condition_hypnotized)) {

			ZombieSetCondition(spirit, zombie_condition_hypnotized, 0, 3.4028e38f, 0.0f);

			spirit->m_teamFlags = self->m_teamFlags;
			typedef void* (*GetHypnoDataFunc)(Zombie*);
			GetHypnoDataFunc funGetHypnoData = (GetHypnoDataFunc)getActualOffset(0xC3E6DC);

			void* hypnoData = funGetHypnoData(self);
			typedef void (*ApplyHypnoDataFunc)(Zombie*, void*);
			ApplyHypnoDataFunc funApplyHypnoData = (ApplyHypnoDataFunc)getActualOffset(0xC41290);

			funApplyHypnoData(spirit, hypnoData);
		}
	}
}

void ZombieMausoleumSpirit::IntroOnEnter(ZombieMausoleumSpirit* zombie)
{
	RegisterEventAfterAnim(zombie, "intro", "onIntroDone");
}

void ZombieMausoleumSpirit::IntroOnLoop(ZombieMausoleumSpirit* zombie)
{

}

void ZombieMausoleumSpirit::IntroOnExit(ZombieMausoleumSpirit* zombie)
{

}

void MausoleumSpiritCompletedCallback(Zombie* zombie) {
	ZombieMausoleumSpirit* spiritZombie = static_cast<ZombieMausoleumSpirit*>(zombie);
	if (spiritZombie && !ZombieIsDeadOrDying(spiritZombie)) {
		ZombieEnterState(spiritZombie, 1, 0);
	}
}

void ZombieMausoleumSpirit::modInit() {
	LOGI("ZombieSpirit mod init");

	vftable = CreateChildVFTable(206 + 6, getActualOffset(0x23DEFB8), 206);
	PatchVFTable(vftable, (void*)ZombieMausoleumSpirit::StaticGetType, 0);
	PatchVFTable(vftable, (void*)MausoleumSpiritCanBeTargeted, 21);
	PatchVFTable(vftable, (void*)MausoleumSpiritOnSpawn, 49);

	PatchVFTable(vftable, (void*)ZombieMausoleumSpirit::IntroOnEnter, 207);
	PatchVFTable(vftable, (void*)ZombieMausoleumSpirit::IntroOnLoop, 208);
	PatchVFTable(vftable, (void*)ZombieMausoleumSpirit::IntroOnExit, 209);

	ZombieMausoleumSpirit::StaticGetType();
	LOGI("ZombieSpirit finish init");
}

void ZombieMausoleumBasic::modInit() {
	LOGI("ZombieMausoleumBasic mod init");

	vftable = CopyVFTable(getActualOffset(0x23DEFB8), 206);
	PatchVFTable(vftable, (void*)ZombieMausoleumBasic::StaticGetType, 0);
	PatchVFTable(vftable, (void*)MausoleumBasicOnCreate, 169);
	PatchVFTable(vftable, (void*)MausoleumBasicActionFrame, 170);

	ZombieMausoleumBasic::StaticGetType();
	LOGI("ZombieMausoleumBasic finish init");
}

void ZombieMausoleumBasicProps::modInit() {
	LOGI("ZombieBasicProps init");

	vftable = CopyVFTable(getActualOffset(0x24328D0), 14);

	PatchVFTable(vftable, (void*)ZombieMausoleumBasicProps::StaticGetType, 0);

	ZombieMausoleumBasicProps::StaticGetType();

	LOGI("ZombieBasicProps finish init");
}


void ZombieMausoleumSpirit::buildEventCallbacks(Reflection::CRefManualSymbolBuilder* builder, Reflection::RClass* rtClass)
{
	IF_CALLBACK_NOTSETUP(ZombieMausoleumSpirit) {
		SetupLiteralDelegate(&introCompletedDelegate, MausoleumSpiritCompletedCallback);
		ZombieMausoleumSpirit_delegatesSetup = true;
		LOGI("SO TRUE");
	}
	RegisterEventCallback(builder, rtClass, "onIntroDone", introCompletedDelegate);
	LOGI("Reg event complete");
}

void ZombieMausoleumSpirit::buildStates()
{
	StateMachineTableBuilder* stateMachine = CallGetStateMachine(ZombieMausoleumSpirit::StaticGetType());
	RegisterStateByOffsets(stateMachine,
		16,
		(uintptr_t)ZombieMausoleumSpirit::IntroOnEnter,
		(uintptr_t)ZombieMausoleumSpirit::IntroOnLoop,
		(uintptr_t)ZombieMausoleumSpirit::IntroOnExit,
		"ZS_Spirit_Intro");
	LOGI("Reg state complete");
}