#include "ZombieGargantuar.h"
#include "ZombieGargantuarProps.h"
#include "GridItem.h"
#include "ZombieHelper.h"
#include "AddZombieType.h"
#include "ZombieAnimRig_MausoleumCursedBase.h"

void* ZombieMausoleumGargantuar::vftable = __null;
Sexy::RtClass* ZombieMausoleumGargantuar::s_rtClass = __null;;

void* ZombieMausoleumGargantuarProps::vftable = __null;
Sexy::RtClass* ZombieMausoleumGargantuarProps::s_rtClass = __null;;

void MausoleumGargantuarOnCreate(ZombieMausoleumGargantuar* zombie) {
	auto props = reinterpret_cast<ZombieMausoleumGargantuarProps*>(zombie->m_propertySheet.Get());
	auto rig = reinterpret_cast<ZombieAnimRig_MausoleumGargantuar*>(zombie->m_animRig.Get());
	rig->m_hasCursed = props->CursedAtStart;
	SetAnimLayerVisible(rig, "zombie_eyes_curse", rig->m_hasCursed);
}

void hkMausoleumGargantuarActionFrame(ZombieMausoleumGargantuar* self, int64_t unk1, SexyString* actionName, int64_t unk2, SexyString* currentAnim)
{
    typedef void* (*gargAF)(ZombieMausoleumGargantuar*, int64_t, SexyString*, int64_t, SexyString*);
    gargAF gargActionFrame = (gargAF)getActualOffset(0xB30CC0);
	if (*actionName == "ghost_intro")
	{
		auto* props = reinterpret_cast<ZombieMausoleumGargantuarProps*>(self->m_propertySheet.Get());
		SexyString name = props->SpiritTypeName;
		Zombie* spirit = AddZombie(name, -1, 6, -1);
		float newX = self->m_position.x;
		float newY = self->m_position.y;
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
		return;
	}
    gargActionFrame(self, unk1, actionName, unk2, currentAnim);
}
void ZombieMausoleumGargantuar::modInit() {
    LOGI("ZombieMausoleumGargantuar init");

    vftable = CopyVFTable(getActualOffset(0x23E5478), 212);

    PatchVFTable(vftable, (void*)ZombieMausoleumGargantuar::StaticGetType, 0);

    PatchVFTable(vftable, (void*)MausoleumGargantuarOnCreate, 169);

	PatchVFTable(vftable, (void*)hkMausoleumGargantuarActionFrame, 170);

    ZombieMausoleumGargantuar::StaticGetType();

    LOGI("ZombieMausoleumGargantuar finish init");
}

void ZombieMausoleumGargantuarProps::modInit() {
	LOGI("ZombieMausoleumGargantuarProps init");

	vftable = CopyVFTable(getActualOffset(0x2432A08), 14);

	PatchVFTable(vftable, (void*)ZombieMausoleumGargantuarProps::StaticGetType, 0);

	ZombieMausoleumGargantuarProps::StaticGetType();

	LOGI("ZombieMausoleumGargantuarProps finish init");
}