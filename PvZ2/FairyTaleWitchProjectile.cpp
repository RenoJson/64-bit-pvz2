#include "FairyTaleWitchProjectile.h"
#include "Plant.h"
#include "AddZombieType.h"
#include "AddPlantType.h"
#include "ZombieFairyTaleWitchFrog.h"
DEFINE_STATIC_RT_CLASS_MEMBERS(FrogProjectile)
bool overrideOnImpact(FrogProjectile* proj, Plant* plant) {
	bool isImpacted = proj->DoImpact(plant);
	if (isImpacted == true) {
		ZombieFairyTaleWitchFrog* frog = (ZombieFairyTaleWitchFrog*)AddZombie("fairy_tale_witch_frog", -1, 6, -1);
		auto type = reinterpret_cast<PlantType*>(plant->m_type.Get());
		frog->m_transformedPlant = type->TypeName;
	}
	return isImpacted;

}
void FrogProjectile::ModInit() {
	LOGI("FrogProjectile init");

	vftable = CopyVFTable(getActualOffset(0x2421040), 53);

	PatchVFTable(vftable, (void*)FrogProjectile::StaticGetType, 0);

	PatchVFTable(vftable, (void*)overrideOnImpact, 41);

	FrogProjectile::StaticGetType();

	LOGI("FrogProjectile finish init");
}