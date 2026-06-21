#include "ZombieMausoleumVendor.h"
#include "ZombieStateHelper.h"
#include "ZombieMausoleumVendorProps.h"
#include "TimeMgr.h"
#include "AddZombieType.h"
#include "ZombieAnimRig_Vendor.h"
#include "StateMachineBuilder.h"
#include "ZombieHelper.h"
#include "GridItemMausoleumLawnTile.h"

void* ZombieMausoleumVendor::vftable = nullptr;
Sexy::RtClass* ZombieMausoleumVendor::s_rtClass = nullptr;

DECLARE_DELEGATES_SETUP(ZombieMausoleumVendor)

static Sexy::DelegateBase setPigCompletedDelegate;

void vendorOnSpawn(ZombieMausoleumVendor* zombie) {
	zombie->m_firstSpawned = false;
	zombie->m_currentDirection = 0;
	zombie->m_lastPathGridX = 0;
	zombie->m_lastPathGridY = 0;
	ZombieOnSpawn(zombie);
}
void VendorOnAffectedByCondition(ZombieMausoleumVendor* zombie, int condition) {
	if (condition == zombie_condition_shrinking) {
		auto* rig = reinterpret_cast<ZombieAnimRig_Vendor*>(zombie->m_animRig.Get());
		SetAnimLayerVisible(rig, "pig_1_body", false);
		SetAnimLayerVisible(rig, "pig_1_body1", false);
		SetAnimLayerVisible(rig, "pig_1_ear", false);
		SetAnimLayerVisible(rig, "pig_1_ear1", false);
		SetAnimLayerVisible(rig, "pig_3_bottom", false);
		SetAnimLayerVisible(rig, "pig_2", false);
		SetAnimLayerVisible(rig, "pig_1_ear", false);
		SetAnimLayerVisible(rig, "pig_3_head", false);
		SetAnimLayerVisible(rig, "pig_mouth", false);
		SetAnimLayerVisible(rig, "pig_nose", false);
		SetAnimLayerVisible(rig, "pig_face", false);
		SetAnimLayerVisible(rig, "pig_body", false);
		SetAnimLayerVisible(rig, "pig_leg_2", false);
		SetAnimLayerVisible(rig, "pig_leg_4", false);
		SetAnimLayerVisible(rig, "pig_leg_1", false);
		SetAnimLayerVisible(rig, "pig_leg_3", false);
		SetAnimLayerVisible(rig, "pig_ear_right", false);
		SetAnimLayerVisible(rig, "pig_ear_left", false);
		SetAnimLayerVisible(rig, "pig_tail", false);
	}
}
void vendorWalkOnLoop(ZombieMausoleumVendor* zombie) {
		auto props = reinterpret_cast<ZombieMausoleumVendorProps*>(zombie->m_propertySheet.Get());
        bool isShrunken = (ZombieHasCondition(zombie, zombie_condition_shrinking) || ZombieHasCondition(zombie, zombie_condition_shrunken));
        if (!isShrunken) {
            if (zombie->m_firstSpawned == false) {
                if (TimeMgr::GetInstance()->m_curTime >= zombie->m_creationTime + props->SetPigInterval) {
                    zombie->m_firstSpawned = true;
                    ZombieEnterState(zombie, 16, 0);
                }
            }
            else if (TimeMgr::GetInstance()->m_curTime >= zombie->m_nextPigTime) {
                ZombieEnterState(zombie, 16, 0);
            }
        }
        CallFunc<void, ZombieMausoleumVendor*>(0xC506B4, zombie);
}

void ZombieMausoleumVendor::PigOnEnter(ZombieMausoleumVendor* zombie)
{
   /* float pX = zombie->m_position.x;
    float pY = zombie->m_position.y;
    float pZ = zombie->m_position.z;

    int gX = static_cast<int>((pX - 200.0f) / 64.0f);
    int gY = static_cast<int>((pY - 160.0f) / 76.0f);
    int currentZomRow = static_cast<int>((pY - 160.0f) / 76.0f);

    Rect feetRect;
    feetRect.mX = static_cast<int>(pX) - 10;
    feetRect.mY = static_cast<int>(pY - pZ) - 10;
    feetRect.mWidth = 20;
    feetRect.mHeight = 20;

    std::vector<BoardEntity*> entities;

    int minRow = std::max(0, currentZomRow - 1);
    int maxRow = std::min(5, currentZomRow + 1);
    GetEntitiesInRectPixel(&entities, 63, &feetRect, minRow, maxRow);

    GridItemMausoleumLawnPath* activePathTile = nullptr;

    for (BoardEntity* ptr : entities) {
        if (ptr == nullptr) continue;

        if (ptr->IsType(GridItemMausoleumLawnPath::StaticGetType()))
        {
            GridItem* tile = static_cast<GridItem*>(ptr);

            if (tile->m_gridLocation.mX == gX && tile->m_gridLocation.mY == gY) {
                activePathTile = static_cast<GridItemMausoleumLawnPath*>(tile);
                break;
            }
        }
    }

    if (activePathTile != nullptr)
    {
        ZombieAllowMovement(zombie, false);
    }
    else {
        ZombieAllowMovement(zombie, true);
    }*/
	RegisterEventAfterAnim(zombie, "set_pig", "onSetPigCompleted");
}

void ZombieMausoleumVendor::PigOnLoop(ZombieMausoleumVendor* zombie)
{

}

void ZombieMausoleumVendor::PigOnExit(ZombieMausoleumVendor* zombie)
{

}

void vendorCompletedCallback(Zombie* zombie) {
	ZombieMausoleumVendor* vendorZombie = static_cast<ZombieMausoleumVendor*>(zombie);
	if (vendorZombie) {
		auto* props = reinterpret_cast<ZombieMausoleumVendorProps*>(vendorZombie->m_propertySheet.Get());
		vendorZombie->m_nextPigTime = TimeMgr::GetInstance()->m_curTime + props->SetPigInterval;
		ZombieEnterState(vendorZombie, 1, 0);
	}
}


void SpawnZombiePig(ZombieMausoleumVendor* self) {
	if (!ZombieHasCondition(self, zombie_condition_shrinking) && !ZombieHasCondition(self, zombie_condition_shrunken))
	{
		auto* props = reinterpret_cast<ZombieMausoleumVendorProps*>(self->m_propertySheet.Get());
		SexyString name = props->PigType;
		Zombie* spawnedRider = AddZombie(name, -1, 6, -1);
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

			spawnedRider->m_teamFlags = self->m_teamFlags;
			typedef void* (*GetHypnoDataFunc)(Zombie*);
			GetHypnoDataFunc funGetHypnoData = (GetHypnoDataFunc)getActualOffset(0xC3E6DC);

			void* hypnoData = funGetHypnoData(self);
			typedef void (*ApplyHypnoDataFunc)(Zombie*, void*);
			ApplyHypnoDataFunc funApplyHypnoData = (ApplyHypnoDataFunc)getActualOffset(0xC41290);

			funApplyHypnoData(spawnedRider, hypnoData);
		}

	}
}

void SpawnZombiePigAfterDie(ZombieMausoleumVendor* self) {
	if (!ZombieHasCondition(self, zombie_condition_shrinking) && !ZombieHasCondition(self, zombie_condition_shrunken))
	{
		auto* props = reinterpret_cast<ZombieMausoleumVendorProps*>(self->m_propertySheet.Get());

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
			if (spawnedRider)
			{
				float offsetX = (float)(rand() % 50 - 10);

				SexyVector3 finalCoords = SexyVector3(baseX + offsetX, baseY, baseZ);

				ZombieSetPosition(spawnedRider, &finalCoords);
				if (ZombieHasCondition(self, zombie_condition_hypnotized)) {

					ZombieSetCondition(spawnedRider, zombie_condition_hypnotized, 0, 3.4028e38f, 0.0f);

					spawnedRider->m_teamFlags = self->m_teamFlags;
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
}

void MausoleumVendorActionFrame(ZombieMausoleumVendor* vendor, SexyString* currentAnim, SexyString* actionName, SexyString* param, float nextFrameTime)
{
	if (*actionName == "use_action")
	{
		SpawnZombiePig(vendor);
	}
	else if (*actionName == "set_allpig") {
		SpawnZombiePigAfterDie(vendor);
	}
}
void ZombieMausoleumVendor::ModInit() {
	LOGI("ZombieVendor mod init");

	vftable = CreateChildVFTable(204 + 6, getActualOffset(0x241D430), 204);
	PatchVFTable(vftable, (void*)ZombieMausoleumVendor::StaticGetType, 0);
	PatchVFTable(vftable, (void*)vendorOnSpawn, 49);
	PatchVFTable(vftable, (void*)VendorOnAffectedByCondition, 71);
	PatchVFTable(vftable, (void*)vendorWalkOnLoop, 124);
	PatchVFTable(vftable, (void*)MausoleumVendorActionFrame, 170);

	PatchVFTable(vftable, (void*)ZombieMausoleumVendor::PigOnEnter, 207);
	PatchVFTable(vftable, (void*)ZombieMausoleumVendor::PigOnLoop, 208);
	PatchVFTable(vftable, (void*)ZombieMausoleumVendor::PigOnExit, 209);

	ZombieMausoleumVendor::StaticGetType();
	LOGI("ZombieVendor finish init");
}

void ZombieMausoleumVendor::buildEventCallbacks(Reflection::CRefManualSymbolBuilder* builder, Reflection::RClass* rtClass)
{
	IF_CALLBACK_NOTSETUP(ZombieMausoleumVendor) {
		SetupLiteralDelegate(&setPigCompletedDelegate, vendorCompletedCallback);
		ZombieMausoleumVendor_delegatesSetup = true;
		LOGI("SO TRUE");
	}
	RegisterEventCallback(builder, rtClass, "onSetPigCompleted", setPigCompletedDelegate);
	LOGI("Reg event complete");
}

void ZombieMausoleumVendor::buildStates()
{
	StateMachineTableBuilder* stateMachine = CallGetStateMachine(ZombieMausoleumVendor::StaticGetType());
	RegisterStateByOffsets(stateMachine,
		16,
		(uintptr_t)ZombieMausoleumVendor::PigOnEnter,
		(uintptr_t)ZombieMausoleumVendor::PigOnLoop,
		(uintptr_t)ZombieMausoleumVendor::PigOnExit,
		"ZS_Vendor_SetPig");
	LOGI("Reg state complete");
}



