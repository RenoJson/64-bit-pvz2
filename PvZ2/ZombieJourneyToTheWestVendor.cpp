#include "ZombieJourneyToTheWestVendor.h"
#include "ZombieStateHelper.h"
#include "ZombieJourneyToTheWestVendorProps.h"
#include "TimeMgr.h"
#include "ZombieState.h"
#include "AddZombieType.h"
#include "ZombieAnimRig_Vendor.h"
void* ZombieJourneyToTheWestVendor::vftable = nullptr; Sexy::RtClass* ZombieJourneyToTheWestVendor::s_rtClass = nullptr;;

typedef void (*zombieEnterState)(ZombieJourneyToTheWestVendor*, int, int);
typedef Zombie* (*zombieAllowMovement)(Zombie*, int);
DECLARE_DELEGATES_SETUP(ZombieJourneyToTheWestVendor)
static void nullsub(ZombieJourneyToTheWestVendor* zombie) {}

static Sexy::DelegateBase setPigCompletedDelegate;
static Sexy::DelegateBase walkContinueDelegate;


void vendorWalkOnLoop(ZombieJourneyToTheWestVendor* zombie) {
	typedef void (*LoopWalk)(ZombieJourneyToTheWestVendor*);
	((LoopWalk)getActualOffset(0xC506B4))(zombie);
}
void vendorPigOnEnter(ZombieJourneyToTheWestVendor* zombie) {
	LOGI("Entering setPig");
	SexyString setPigAnim = "set_pig";
	RegisterEventAfterAnim(zombie, &setPigAnim, "onSetPigCompleted");
}
void vendorCompletedCallback(Zombie* zombie) {
	LOGI("set Pig finished");
	ZombieJourneyToTheWestVendor* vendorZombie = static_cast<ZombieJourneyToTheWestVendor*>(zombie);
	if (vendorZombie) {
		TimeMgr::GetInstance()->m_curTime - zombie->m_creationTime;
		((zombieEnterState)getActualOffset(0xC3D428))(vendorZombie, 1, 0);
	}
}
float hkGetWalkSpeed(ZombieJourneyToTheWestVendor* zombie) {
	auto* props = reinterpret_cast<ZombieJourneyToTheWestVendorProps*>(zombie->m_propertySheet.Get());
	float minVal = props->SpeedRateModifier.Min;
	float maxVal = props->SpeedRateModifier.Max;
	zombie->m_speedModifier = minVal + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (maxVal - minVal)));
	return zombie->m_walkSpeed * zombie->m_speedModifier;
}


void SpawnZombiePig(ZombieJourneyToTheWestVendor* self) {
	auto* props = reinterpret_cast<ZombieJourneyToTheWestVendorProps*>(self->m_propertySheet.Get());
	SexyString name = props->PigType;
	typedef SexyVector3(*boardEntitySetPosition)(Zombie*, SexyVector3*);
	boardEntitySetPosition funBoardEntitySetPosition = (boardEntitySetPosition)getActualOffset(0x628058);
	typedef bool (*checkZombieHasCondition)(Zombie*, int);
	checkZombieHasCondition hasZCondition = (checkZombieHasCondition)getActualOffset(0xC3E44C);
	typedef void (*setConditionZ)(Zombie*, int, int, float, float);
	setConditionZ setZCondition = (setConditionZ)getActualOffset(0xC40CC0);
	Zombie* spawnedRider = AddZombie(name, -1, 6, -1);
	if (hasZCondition(self, zombie_condition_shrinking) || hasZCondition(self, zombie_condition_shrunken)) {

		setZCondition(spawnedRider, zombie_condition_shrunken, 0, 3.4028e38f, 0.0f);
	}
	
	bool isHypnotized = hasZCondition(self, zombie_condition_hypnotized);
	float newX = self->m_position.x - props->PigSpawnOffset.x;
	if (isHypnotized)
	{
		newX = self->m_position.x + props->PigSpawnOffset.x;
	}
	float newY = self->m_position.y - props->PigSpawnOffset.y;
	float newZ = self->m_position.z - props->PigSpawnOffset.z;
	SexyVector3 newCoords = SexyVector3(newX, newY, newZ);
	funBoardEntitySetPosition(spawnedRider, &newCoords);
	if (isHypnotized) {

		setZCondition(spawnedRider, zombie_condition_hypnotized, 0, 3.4028e38f, 0.0f);

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
	typedef SexyVector3(*boardEntitySetPosition)(Zombie*, SexyVector3*);
	boardEntitySetPosition funBoardEntitySetPosition = (boardEntitySetPosition)getActualOffset(0x628058);
	typedef bool (*checkZombieHasCondition)(Zombie*, int);
	checkZombieHasCondition hasZCondition = (checkZombieHasCondition)getActualOffset(0xC3E44C);
	typedef void (*setConditionZ)(Zombie*, int, int, float, float);
	setConditionZ setZCondition = (setConditionZ)getActualOffset(0xC40CC0);
	bool isHypnotized = hasZCondition(self, zombie_condition_hypnotized);
	float baseX = self->m_position.x - props->PigSpawnOffset.x;
	if (isHypnotized)
	{
		baseX = self->m_position.x + props->PigSpawnOffset.x;
	}
	float baseY = self->m_position.y - props->PigSpawnOffset.y;
	float baseZ = self->m_position.z - props->PigSpawnOffset.z;
	for (int i = 0; i < spawnCount; i++)
	{
		Zombie* spawnedRider = AddZombie(name, -1, 6, -1);
		if (hasZCondition(self, zombie_condition_shrinking) || hasZCondition(self, zombie_condition_shrunken)) {

			setZCondition(spawnedRider, zombie_condition_shrunken, 0, 3.4028e38f, 0.0f);
		}
		if (spawnedRider) 
		{
			float offsetX = (float)(rand() % 50 - 10);
			float offsetY = (float)(rand() % 30 - 10);

			SexyVector3 finalCoords = SexyVector3(baseX + offsetX, baseY + offsetY, baseZ);

			funBoardEntitySetPosition(spawnedRider, &finalCoords);
			if (isHypnotized) {

				setZCondition(spawnedRider, zombie_condition_hypnotized, 0, 3.4028e38f, 0.0f);

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
	PatchVFTable(vftable, (void*)hkGetWalkSpeed, 118);
	PatchVFTable(vftable, (void*)hkJourneyToTheWestVendorActionFrame, 170);

	PatchVFTable(vftable, (void*)vendorPigOnEnter, 207);
	PatchVFTable(vftable, (void*)nullsub, 208);
	PatchVFTable(vftable, (void*)nullsub, 209);

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
	Sexy::RtClass* rtClass = ZombieJourneyToTheWestVendor::StaticGetType();
	void* stateMachine = GetStateMachine(rtClass);
	RegisterStateByOffsets(stateMachine, 17, (uintptr_t)vendorPigOnEnter, (uintptr_t)nullsub, (uintptr_t)nullsub, "ZS_Vendor_SetPig");
	LOGI("Reg state complete");
}
