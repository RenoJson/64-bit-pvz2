#include "ZombieJourneyToTheWestVendor.h"
#include "ZombieStateHelper.h"
#include "ZombieJourneyToTheWestVendorProps.h"
#include "TimeMgr.h"
#include "AddZombieType.h"
#include "ZombieAnimRig_Vendor.h"
#include "StateMachineBuilder.h"
#include "ZombieHelper.h"
void* ZombieJourneyToTheWestVendor::vftable = nullptr; Sexy::RtClass* ZombieJourneyToTheWestVendor::s_rtClass = nullptr;;

DECLARE_DELEGATES_SETUP(ZombieJourneyToTheWestVendor)

static Sexy::DelegateBase setPigCompletedDelegate;

void vendorOnSpawn(ZombieJourneyToTheWestVendor* zombie) {
	zombie->m_firstSpawned = false;
	ZombieOnSpawn(zombie);
}

void vendorWalkOnLoop(ZombieJourneyToTheWestVendor* zombie) {
	auto* props = reinterpret_cast<ZombieJourneyToTheWestVendorProps*>(zombie->m_propertySheet.Get());
	if (zombie->m_firstSpawned == false) {
		if (TimeMgr::GetInstance()->m_curTime >= zombie->m_creationTime + props->SetPigInterval) {
			ZombieEnterState(zombie, 16, 0);
			zombie->m_firstSpawned = true;
		}
	}
	else if (TimeMgr::GetInstance()->m_curTime >= zombie->m_nextPigTime) {
		ZombieEnterState(zombie, 16, 0);
	}
	else {
		CallFunc<void, ZombieJourneyToTheWestVendor*>(0xC506B4, zombie);
	}
}
float hkGetWalkSpeed(ZombieJourneyToTheWestVendor* zombie) {
	auto* props = reinterpret_cast<ZombieJourneyToTheWestVendorProps*>(zombie->m_propertySheet.Get());
	float minVal = props->SpeedRateModifier.Min;
	float maxVal = props->SpeedRateModifier.Max;
	zombie->m_speedModifier = minVal + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (maxVal - minVal)));
	return zombie->m_walkSpeed * zombie->m_speedModifier;
}

void ZombieJourneyToTheWestVendor::PigOnEnter(ZombieJourneyToTheWestVendor* zombie)
{
	//LOGI("Entering setPig");
	ZombieAllowMovement(zombie, true);
	return RegisterEventAfterAnim(zombie, "set_pig", "onSetPigCompleted");
}

void ZombieJourneyToTheWestVendor::PigOnLoop(ZombieJourneyToTheWestVendor* zombie)
{

}

void ZombieJourneyToTheWestVendor::PigOnExit(ZombieJourneyToTheWestVendor* zombie)
{

}

void vendorCompletedCallback(Zombie* zombie) {
	//LOGI("set Pig finished");
	ZombieJourneyToTheWestVendor* vendorZombie = static_cast<ZombieJourneyToTheWestVendor*>(zombie);
	if (vendorZombie) {
		auto* props = reinterpret_cast<ZombieJourneyToTheWestVendorProps*>(vendorZombie->m_propertySheet.Get());
		vendorZombie->m_nextPigTime = TimeMgr::GetInstance()->m_curTime + props->SetPigInterval;
		ZombieEnterState(vendorZombie, 1, 0);
	}
}


void SpawnZombiePig(ZombieJourneyToTheWestVendor* self) {
	auto* props = reinterpret_cast<ZombieJourneyToTheWestVendorProps*>(self->m_propertySheet.Get());
	SexyString name = props->PigType;
	Zombie* spawnedRider = AddZombie(name, -1, 6, -1);
	if (ZombieHasCondition(self, zombie_condition_shrinking) || ZombieHasCondition(self, zombie_condition_shrunken)) {

		ZombieSetCondition(spawnedRider, zombie_condition_shrunken, 0, 3.4028e38f, 0.0f);
	}
	
	float newX = self->m_position.x - props->PigSpawnOffset.x;
	if (ZombieHasCondition(self, zombie_condition_hypnotized))
	{
		newX = self->m_position.x + props->PigSpawnOffset.x;
	}
	float newY = self->m_position.y - props->PigSpawnOffset.y;
	float newZ = self->m_position.z - props->PigSpawnOffset.z;
	SexyVector3 newCoords = SexyVector3(newX, newY, newZ);
	ZombieSetPosition(spawnedRider, &newCoords);
	if (ZombieHasCondition(self, zombie_condition_hypnotized)) {

		ZombieSetCondition(spawnedRider, zombie_condition_hypnotized, 0, 3.4028e38f, 0.0f);

		int valAtOffset36 = *(int*)((uintptr_t)self + 0x24);
		typedef void (*func10B013C)(Zombie*, int);
		auto* setTeamFlag = ((func10B013C)getActualOffset(0x10B013C));
		setTeamFlag(spawnedRider, valAtOffset36);
		typedef void* (*GetHypnoDataFunc)(Zombie*);
		GetHypnoDataFunc funGetHypnoData = (GetHypnoDataFunc)getActualOffset(0xC3E6DC);

		void* hypnoData = funGetHypnoData(self);
		typedef void (*ApplyHypnoDataFunc)(Zombie*, void*);
		ApplyHypnoDataFunc funApplyHypnoData = (ApplyHypnoDataFunc)getActualOffset(0xC41290);

		funApplyHypnoData(spawnedRider, hypnoData);
	}
}

void SpawnZombiePigAfterDie(ZombieJourneyToTheWestVendor* self) {
	auto* props = reinterpret_cast<ZombieJourneyToTheWestVendorProps*>(self->m_propertySheet.Get());

	int spawnCount = props->NumberOfPigToSpawnWhenDie;
	SexyString name = props->PigType;
	float baseX = self->m_position.x - props->PigSpawnOffset.x;
	if (ZombieHasCondition(self, zombie_condition_hypnotized))
	{
		baseX = self->m_position.x + props->PigSpawnOffset.x;
	}
	float baseY = self->m_position.y - props->PigSpawnOffset.y;
	float baseZ = self->m_position.z - props->PigSpawnOffset.z;
	for (int i = 0; i < spawnCount; i++)
	{
		Zombie* spawnedRider = AddZombie(name, -1, 6, -1);
		if (ZombieHasCondition(self, zombie_condition_shrinking) || ZombieHasCondition(self, zombie_condition_shrunken)) {

			ZombieSetCondition(spawnedRider, zombie_condition_shrunken, 0, 3.4028e38f, 0.0f);
		}
		if (spawnedRider) 
		{
			float offsetX = (float)(rand() % 50 - 10);

			SexyVector3 finalCoords = SexyVector3(baseX + offsetX, baseY, baseZ);

			ZombieSetPosition(spawnedRider, &finalCoords);
			if (ZombieHasCondition(self, zombie_condition_hypnotized)) {

				ZombieSetCondition(spawnedRider, zombie_condition_hypnotized, 0, 3.4028e38f, 0.0f);

				int valAtOffset36 = *(int*)((uintptr_t)self + 0x24);
				typedef void (*func10B013C)(Zombie*, int);
				auto* setTeamFlag = ((func10B013C)getActualOffset(0x10B013C));
				setTeamFlag(spawnedRider, valAtOffset36);
				typedef void* (*GetHypnoDataFunc)(Zombie*);
				GetHypnoDataFunc funGetHypnoData = (GetHypnoDataFunc)getActualOffset(0xC3E6DC);

				void* hypnoData = funGetHypnoData(self);
				typedef void (*ApplyHypnoDataFunc)(Zombie*, void*);
				ApplyHypnoDataFunc funApplyHypnoData = (ApplyHypnoDataFunc)getActualOffset(0xC41290);

				funApplyHypnoData(spawnedRider, hypnoData);
			}
		}
	}
}

void hkJourneyToTheWestVendorActionFrame(ZombieJourneyToTheWestVendor* vendor, int64_t unk1, SexyString* actionName, int64_t unk2, SexyString* currentAnim)
{
	if (*actionName == "use_action")
	{
		SpawnZombiePig(vendor);
	}
	else if (*actionName == "set_allpig") {
		SpawnZombiePigAfterDie(vendor);
	}
}
void ZombieJourneyToTheWestVendor::ModInit() {
	LOGI("ZombieVendor mod init");

	vftable = CreateChildVFTable(204 + 6, getActualOffset(0x241D430), 204);
	PatchVFTable(vftable, (void*)ZombieJourneyToTheWestVendor::StaticGetType, 0);
	PatchVFTable(vftable, (void*)vendorOnSpawn, 49);
	PatchVFTable(vftable, (void*)hkGetWalkSpeed, 118);
	PatchVFTable(vftable, (void*)vendorWalkOnLoop, 124);
	PatchVFTable(vftable, (void*)hkJourneyToTheWestVendorActionFrame, 170);

	PatchVFTable(vftable, (void*)ZombieJourneyToTheWestVendor::PigOnEnter, 207);
	PatchVFTable(vftable, (void*)ZombieJourneyToTheWestVendor::PigOnLoop, 208);
	PatchVFTable(vftable, (void*)ZombieJourneyToTheWestVendor::PigOnExit, 209);

	ZombieJourneyToTheWestVendor::StaticGetType();
	LOGI("ZombieVendor finish init");
}

void ZombieJourneyToTheWestVendor::buildEventCallbacks(Reflection::CRefManualSymbolBuilder* builder, Reflection::RClass* rtClass)
{
	IF_CALLBACK_NOTSETUP(ZombieJourneyToTheWestVendor) {
		SetupLiteralDelegate(&setPigCompletedDelegate, vendorCompletedCallback);
		ZombieJourneyToTheWestVendor_delegatesSetup = true;
		LOGI("SO TRUE");
	}
	RegisterEventCallback(builder, rtClass, "onSetPigCompleted", setPigCompletedDelegate);
	LOGI("Reg event complete");
}

void ZombieJourneyToTheWestVendor::buildStates()
{
	StateMachineTableBuilder* stateMachine = CallGetStateMachine(ZombieJourneyToTheWestVendor::StaticGetType());
	RegisterStateByOffsets(stateMachine, 
		16, 
		(uintptr_t)ZombieJourneyToTheWestVendor::PigOnEnter, 
		(uintptr_t)ZombieJourneyToTheWestVendor::PigOnLoop, 
		(uintptr_t)ZombieJourneyToTheWestVendor::PigOnExit, 
		"ZS_Vendor_SetPig");
	LOGI("Reg state complete");
}



