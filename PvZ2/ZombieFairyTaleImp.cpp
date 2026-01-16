#include "ZombieFairyTaleImp.h"
#include "ZombieFairyTaleImpProps.h"
#include "AddZombieType.h"
using namespace Sexy;

void* ZombieFairyTaleImp::vftable = __null;
Sexy::RtClass* ZombieFairyTaleImp::s_rtClass = __null;;


void SpawnZombie(Zombie* self) {
	auto* props = reinterpret_cast<ZombieFairyTaleImpProps*>(self->m_propertySheet.Get());
	SexyString name = props->SpawnedZombieName;

	Zombie* spawnedRider = AddZombie(name, -1, 6, -1);

	typedef SexyVector3(*boardEntitySetPosition)(Zombie*, SexyVector3*);
	boardEntitySetPosition funBoardEntitySetPosition = (boardEntitySetPosition)getActualOffset(0x628058);

	float newX = 920;
	float newY = self->m_position.y;
	float newZ = 0;
	SexyVector3 newCoords = SexyVector3(newX, newY, newZ);
	funBoardEntitySetPosition(spawnedRider, &newCoords);
}

void hkFairyTaleImpActionFrame(ZombieFairyTaleImp* imp, int64_t unk1, SexyString* actionName, int64_t unk2, SexyString* currentAnim)
{
	if (*actionName == "reinforcement" && !imp->m_hasSpawned)
	{
		SpawnZombie(imp);
		imp->m_hasSpawned = true;
	}
	else {
		return;
	}
}

void ZombieFairyTaleImp::modInit() {
	LOGI("ZombieFairyTaleImp init");

	vftable = CopyVFTable(getActualOffset(0x23D3480), 227);

	PatchVFTable(vftable, (void*)ZombieFairyTaleImp::StaticGetType, 0);


	PatchVFTable(vftable, (void*)hkFairyTaleImpActionFrame, 170);

	ZombieFairyTaleImp::StaticGetType();

	LOGI("ZombieFairyTaleImp finish init");
}