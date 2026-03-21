#include "FairyTaleWitchProjectile.h"
#include "Plant.h"
#include "AddZombieType.h"
#include "ZombieFairyTaleWitchFrog.h"
#include "ZombieWithAction.h"
DEFINE_STATIC_RT_CLASS_MEMBERS(FrogProjectile)

bool overrideDoImpact(FrogProjectile* proj, Plant* ent) {
	typedef bool (*doImpact)(FrogProjectile*, Plant*);
	doImpact DoImpact = ((doImpact)getActualOffset(0xCA065C));
	if (ent) {
		ZombieFairyTaleWitchFrog* frog = (ZombieFairyTaleWitchFrog*)AddZombie("fairy_tale_witch_frog", -1, 6, -1);
		float currentX = proj->m_position.x - 30;
		float currentY = proj->m_position.y;
		float currentZ = proj->m_position.z;
		typedef SexyVector3(*boardEntitySetPosition)(Zombie*, SexyVector3*);
		boardEntitySetPosition funBoardEntitySetPosition = (boardEntitySetPosition)getActualOffset(0x628058);
		SexyVector3 position = SexyVector3(currentX, currentY, currentZ);
		SexyVector3 updatePos = funBoardEntitySetPosition(frog, &position);
		typedef void (*func10B013C)(Zombie*, int);
		auto* setTeamFlag = ((func10B013C)getActualOffset(0x10B013C));
		setTeamFlag(frog, 1);
	}
	return DoImpact(proj, ent) & 1;
}
bool overrideCollideWithObject(FrogProjectile* proj, Plant* ent) {
	typedef bool (*func1338AE8)(Plant*);
	func1338AE8 Func1338AE8 = ((func1338AE8)getActualOffset(0x1338AE8));
	typedef void (*killTarg)(Plant*, Projectile*);
	killTarg KillTarget = (killTarg)getActualOffset(0x1337020);
	if (ent) {
		if (ent->IsType(PlantGroup::StaticGetType()) && Func1338AE8(ent)) {
			LOGI("check cd 3");
			KillTarget(ent, proj);
			LOGI("check cd 4");
			overrideDoImpact(proj, ent);
		}
	}
	return true;
}

void FrogProjectile::ModInit() {
	LOGI("FrogProjectile init");

	vftable = CopyVFTable(getActualOffset(0x2421040), 53);

	PatchVFTable(vftable, (void*)FrogProjectile::StaticGetType, 0);

	PatchVFTable(vftable, (void*)overrideCollideWithObject, 35);

	PatchVFTable(vftable, (void*)overrideDoImpact, 41);

	FrogProjectile::StaticGetType();

	LOGI("FrogProjectile finish init");
}