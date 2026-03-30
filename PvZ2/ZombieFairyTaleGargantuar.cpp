#include "ZombieFairyTaleGargantuar.h"
#include "ZombieGargantuarProps.h"
#include "AddZombieType.h"
#include "DamageInfo.h"
#include "ZombieAnimRig_Gargantuar.h"

void* ZombieFairyTaleGargantuar::vftable = __null;
Sexy::RtClass* ZombieFairyTaleGargantuar::s_rtClass = __null;;


typedef bool (*checkZombieHasCondition)(ZombieGargantuar*, int);
checkZombieHasCondition hasCondition = (checkZombieHasCondition)getActualOffset(0xC3E44C);
typedef void (*setConditionZ)(Zombie*, int, int, int, int);
setConditionZ setCondition = (setConditionZ)getActualOffset(0xC40CC0);

void* FGargTakeDamage(ZombieFairyTaleGargantuar* thisPtr, DamageInfo* damageInfo)
{
	auto* props = reinterpret_cast<ZombieFairyTaleGargantuarProps*>(thisPtr->m_propertySheet.Get());
	DamageInfo newDmgInfo = *damageInfo;
	float reductionMultiplier = 1.0f - (props->DamageReduction / 100.0f);
	newDmgInfo.m_damage = damageInfo->m_damage * reductionMultiplier;
	typedef void* (*funcC43B90)(ZombieFairyTaleGargantuar*, DamageInfo*);
	static auto* ZTakeDmg = ((funcC43B90)getActualOffset(0xC43B90));
	return ZTakeDmg(thisPtr, &newDmgInfo);
}

void HideImpLayer(ZombieFairyTaleGargantuar* self, ZombieAnimRig_FairyTaleGargantuar* animRig) {

	auto* props = reinterpret_cast<ZombieFairyTaleGargantuarProps*>(self->m_propertySheet.Get());

	std::vector<SexyString> HardcodedLayer = {};

	typedef int64_t(*setLayerVisibleFunc)(ZombieAnimRig_FairyTaleGargantuar*, SexyString*, bool);
	setLayerVisibleFunc setLayerVisible = (setLayerVisibleFunc)getActualOffset(0x9DB8D0);
	bool hasCustomLayers = false;
	if (!props->HealthThresholdToImpAmmoLayers.empty()) {
		hasCustomLayers = true;

		size_t pairCount = props->HealthThresholdToImpAmmoLayers.size();
		for (size_t i = 0; i < pairCount; i++) {
			const auto& layersToHide = props->HealthThresholdToImpAmmoLayers[i].ProjectileLayersToHide;
			size_t layerCount = layersToHide.size();
			for (size_t j = 0; j < layerCount; j++) {
				SexyString layerName(layersToHide[j]);
				setLayerVisible(animRig, &layerName, false);
			}
		}
	}
	if (!hasCustomLayers) {
		for (size_t i = 0; i < HardcodedLayer.size(); i++) {
			SexyString layerName(HardcodedLayer[i]);
			setLayerVisible(animRig, &layerName, false);
		}
	}
}

void overrideFOnSpawn(ZombieFairyTaleGargantuar* zombie) {
	auto* props = reinterpret_cast<ZombieFairyTaleGargantuarProps*>(zombie->m_propertySheet.Get());
	typedef void (*zombieFun49)(ZombieFairyTaleGargantuar*);
	((zombieFun49)getActualOffset(0xC3D1F0))(zombie);
	if (props->CarryImp == false) {
		auto* rig = reinterpret_cast<ZombieAnimRig_FairyTaleGargantuar*>(zombie->m_animRig.Get());
		HideImpLayer(zombie, rig);
	}
}
void overrideFIdleOnLoop(ZombieFairyTaleGargantuar* zombie) {
	auto* props = reinterpret_cast<ZombieFairyTaleGargantuarProps*>(zombie->m_propertySheet.Get());
	if (props->CarryImp == false) {
		auto* rig = reinterpret_cast<ZombieAnimRig_FairyTaleGargantuar*>(zombie->m_animRig.Get());
		HideImpLayer(zombie, rig);
	}
	typedef void (*zombieFun121)(ZombieFairyTaleGargantuar*);
	((zombieFun121)getActualOffset(0xC502CC))(zombie);
}
void SpawnImpAfterDie(ZombieFairyTaleGargantuar* self) {
	auto* props = reinterpret_cast<ZombieFairyTaleGargantuarProps*>(self->m_propertySheet.Get());
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

void hkFairyTaleGargantuarActionFrame(ZombieFairyTaleGargantuar* gargantuar, int64_t unk1, SexyString* actionName, int64_t unk2, SexyString* currentAnim)
{
	auto* props = reinterpret_cast<ZombieFairyTaleGargantuarProps*>(gargantuar->m_propertySheet.Get());
	typedef void* (*gargAF)(ZombieFairyTaleGargantuar*, int64_t, SexyString*, int64_t, SexyString*);
	gargAF gargActionFrame = (gargAF)getActualOffset(0xB30CC0);
	bool isShrinking = hasCondition(gargantuar, zombie_condition_shrinking);
	bool isShrunken = hasCondition(gargantuar, zombie_condition_shrunken);
	if (*actionName == "drop" && !isShrinking && !isShrunken && props->CarryImp == true)
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

	PatchVFTable(vftable, (void*)FGargTakeDamage, 35);

	PatchVFTable(vftable, (void*)overrideFOnSpawn, 49);

	PatchVFTable(vftable, (void*)overrideFIdleOnLoop, 121);

	PatchVFTable(vftable, (void*)hkFairyTaleGargantuarActionFrame, 170);

	ZombieFairyTaleGargantuar::StaticGetType();

	LOGI("ZombieFairyTaleGargantuar finish init");
}