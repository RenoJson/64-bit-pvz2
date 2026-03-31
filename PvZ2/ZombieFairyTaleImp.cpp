#include "ZombieFairyTaleImp.h"
#include "ZombieFairyTaleImpProps.h"
#include "AddZombieType.h"
#include "ZombieAnimRig_Imp.h"
using namespace Sexy;

void* ZombieFairyTaleImp::vftable = __null;
Sexy::RtClass* ZombieFairyTaleImp::s_rtClass = __null;;
typedef void (*zombieEnterState)(ZombieFairyTaleImp*, int, int);

void hkFairyTaleImpOnSpawn(ZombieFairyTaleImp* zombie) {
	auto* rig = reinterpret_cast<ZombieAnimRig_FairyTaleImp*>(zombie->m_animRig.Get());
	rig->m_jumpScared = false;
	zombie->m_hasSpawned = false;
	typedef void (*zombieFun49)(ZombieFairyTaleImp*);
	((zombieFun49)getActualOffset(0xC3D1F0))(zombie);
}

void hkFairyTaleImpWalkOnLoop(ZombieFairyTaleImp* zombie) {
	auto* rig = reinterpret_cast<ZombieAnimRig_FairyTaleImp*>(zombie->m_animRig.Get());
	if (rig->m_jumpScared == true) {
		auto* props = reinterpret_cast<ZombieFairyTaleImpProps*>(zombie->m_propertySheet.Get());
		typedef Zombie* (*setSpeed)(ZombieFairyTaleImp*, float);
		((setSpeed)getActualOffset(0xC484C0))(zombie, props->RunningSpeedScale);
	}
	typedef void (*zombieFun124)(ZombieFairyTaleImp*);
	((zombieFun124)getActualOffset(0xAC58E0))(zombie);
}
void hkFairyTaleImpWalkOnExit(ZombieFairyTaleImp* zombie) {
	auto* rig = reinterpret_cast<ZombieAnimRig_FairyTaleImp*>(zombie->m_animRig.Get());
	if (rig->m_jumpScared == true) {
		auto* props = reinterpret_cast<ZombieFairyTaleImpProps*>(zombie->m_propertySheet.Get());
		typedef Zombie* (*setSpeed)(ZombieFairyTaleImp*, float);
		((setSpeed)getActualOffset(0xC484C0))(zombie, props->RunningSpeedScale);
	}
	typedef void (*zombieFun125)(ZombieFairyTaleImp*);
	((zombieFun125)getActualOffset(0xAC5920))(zombie);
}

void SpawnZombie(Zombie* self) {
	auto* props = reinterpret_cast<ZombieFairyTaleImpProps*>(self->m_propertySheet.Get());
	SexyString name = props->SpawnedZombieName;
	typedef SexyVector3(*boardEntitySetPosition)(Zombie*, SexyVector3*);
	boardEntitySetPosition funBoardEntitySetPosition = (boardEntitySetPosition)getActualOffset(0x628058);
	int currentRow = (int)((self->m_position.y - 160.0f) / 76.0f);
	for (int offset = -1; offset <= 1; ++offset) {

		int targetRow = currentRow + offset;
		if (targetRow >= 0 && targetRow <= 4) {
			Zombie* spawnedRider = AddZombie(name, -1, 6, -1);
			float newX = 945.0f;
			float newY = self->m_position.y + (offset * 76.0f);
			float newZ = 0.0f;

			SexyVector3 newCoords = SexyVector3(newX, newY, newZ);
			funBoardEntitySetPosition(spawnedRider, &newCoords);
		}
	}
}

void hkFairyTaleImpActionFrame(ZombieFairyTaleImp* imp, int64_t unk1, SexyString* actionName, int64_t unk2, SexyString* currentAnim)
{
	auto* rig = reinterpret_cast<ZombieAnimRig_FairyTaleImp*>(imp->m_animRig.Get());
	auto* props = reinterpret_cast<ZombieFairyTaleImpProps*>(imp->m_propertySheet.Get());
	if (*actionName == "reinforcement" && !imp->m_hasSpawned)
	{
		SpawnZombie(imp);
		imp->m_hasSpawned = true;
	}
	else if (*actionName == "scared" && !rig->m_jumpScared) {
		rig->m_jumpScared = true;
	}
	else {
		return;
	}
}
void ZombieFairyTaleImp::modInit() {
	LOGI("ZombieFairyTaleImp init");

	vftable = CopyVFTable(getActualOffset(0x23D3480), 227);

	PatchVFTable(vftable, (void*)ZombieFairyTaleImp::StaticGetType, 0);

	PatchVFTable(vftable, (void*)hkFairyTaleImpOnSpawn, 49);

	PatchVFTable(vftable, (void*)hkFairyTaleImpWalkOnLoop, 124);

	PatchVFTable(vftable, (void*)hkFairyTaleImpWalkOnExit, 125);

	PatchVFTable(vftable, (void*)hkFairyTaleImpActionFrame, 170);

	ZombieFairyTaleImp::StaticGetType();

	LOGI("ZombieFairyTaleImp finish init");
}