#include "ZombieFairyTaleGargantuar.h"
#include "ZombieGargantuarProps.h"
#include "AddZombieType.h"

void* ZombieFairyTaleGargantuar::vftable = __null;
Sexy::RtClass* ZombieFairyTaleGargantuar::s_rtClass = __null;;


typedef bool (*checkZombieHasCondition)(ZombieGargantuar*, int);
checkZombieHasCondition hasCondition = (checkZombieHasCondition)getActualOffset(0xC3E44C);
typedef void (*setConditionZ)(Zombie*, int, int, int, int);
setConditionZ setCondition = (setConditionZ)getActualOffset(0xC40CC0);

void SpawnImpAfterDie(ZombieGargantuar* self) {
	auto* props = reinterpret_cast<ZombieGargantuarProps*>(self->m_propertySheet.Get());
	SexyString name = props->ImpType;
	float impApex = props->ImpApex;
	float flightTime = props->ImpFlightTime;
	SexyVector3 spawnOffset = props->ImpSpawnOffset;

	Zombie* spawnedRider = AddZombie(name, -1, 6, -1);

	typedef SexyVector3(*boardEntitySetPosition)(Zombie*, SexyVector3*);
	boardEntitySetPosition funBoardEntitySetPosition = (boardEntitySetPosition)getActualOffset(0x628058);

	float newX = self->m_position.x - spawnOffset.x;
	float newY = self->m_position.y - spawnOffset.y;
	float newZ = self->m_position.z - spawnOffset.z;
	SexyVector3 newCoords = SexyVector3(newX, newY, newZ);
	funBoardEntitySetPosition(spawnedRider, &newCoords);
	typedef void (*ZombieThrowVirtual)(Zombie*, int, float, float, float, float, float);
	uintptr_t* vtable = *(uintptr_t**)spawnedRider;
	ZombieThrowVirtual virtualThrow = (ZombieThrowVirtual)(vtable[206]);
	bool isHypnotized = hasCondition(self, zombie_condition_hypnotized);
	if (isHypnotized) {
		setCondition(spawnedRider, zombie_condition_hypnotized, 0, 3.4028e38f, 0.0f);
		uintptr_t bullProperty = *((uintptr_t*)self + 0x24);
		*((uintptr_t*)spawnedRider + 0x24) = bullProperty;
	}
	virtualThrow(spawnedRider, 0, newX, newY, newZ, flightTime, impApex);
}

void hkFairyTaleGargantuarActionFrame(ZombieGargantuar* gargantuar, int64_t unk1, SexyString* actionName, int64_t unk2, SexyString* currentAnim)
{
	typedef void* (*gargAF)(ZombieGargantuar*, int64_t, SexyString*, int64_t,  SexyString*);
	gargAF gargActionFrame = (gargAF)getActualOffset(0xB30CC0);
	bool isShrinking = hasCondition(gargantuar, zombie_condition_shrinking);
	bool isShrunken = hasCondition(gargantuar, zombie_condition_shrunken);
	if (*actionName == "drop" && !isShrinking && !isShrunken)
	{
		SpawnImpAfterDie(gargantuar);
		return;
	}
	gargActionFrame(gargantuar, unk1, actionName, unk2, currentAnim);
}

void ZombieFairyTaleGargantuar::modInit() {
	LOGI("ZombieFairyTaleGargantuar init");

	vftable = CopyVFTable(getActualOffset(0x23E5478), 212);

	PatchVFTable(vftable, (void*)ZombieFairyTaleGargantuar::StaticGetType, 0);

	PatchVFTable(vftable, (void*)hkFairyTaleGargantuarActionFrame, 170);

	ZombieFairyTaleGargantuar::StaticGetType();

	LOGI("ZombieFairyTaleGargantuar finish init");
}