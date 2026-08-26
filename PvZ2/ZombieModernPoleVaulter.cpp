
#include "ZombieModernPoleVaulter.h"
#include "ZombieStateHelper.h"
#include "ZombieHelper.h"
#include "ZombieModernPoleVaulterProps.h"
#include "ZombieState.h"
#include "StateMachineBuilder.h"
#include "ZombieAnimRig_ModernPoleVaulter.h"
#include "Plant.h"
#include "Ladder.h"
#include "GridItem.h"
#include "Sexy/LawnApp.h"
#include "AddProjectileType.h"

void* ZombieModernPoleVaulter::vftable = nullptr; 
Sexy::RtClass* ZombieModernPoleVaulter::s_rtClass = nullptr;

DECLARE_DELEGATES_SETUP(ZombieModernPoleVaulter)

static Sexy::DelegateBase jumpingCompletedDelegate;

static Sexy::DelegateBase bonkingCompletedDelegate;

static Sexy::DelegateBase farJumpingCompletedDelegate;

static Sexy::DelegateBase throwingCompletedDelegate;

float PoleGetWalkSpeed(ZombieModernPoleVaulter* zombie)
{
	auto rig = reinterpret_cast<ZombieAnimRig_ModernPoleVaulter*>(zombie->m_animRig.Get());
	auto props = reinterpret_cast<ZombieModernPoleVaulterProps*>(zombie->m_propertySheet.Get());
	if (rig->m_hasPole == true) {
		return props->RunningSpeed;
	}
	else {
		return zombie->m_walkSpeed;
	}
}
void PoleOnSpawn(ZombieModernPoleVaulter* zombie)
{
	auto rig = reinterpret_cast<ZombieAnimRig_ModernPoleVaulter*>(zombie->m_animRig.Get());
	rig->m_hasPole = true;
	ZombieOnSpawn(zombie);
	SetWalkSpeed(rig, PoleGetWalkSpeed(zombie));
}

bool PoleIsBeingTossedByPlant(ZombieModernPoleVaulter* zombie, int a2) {
	if (zombie->m_entityState.m_id == 16 || zombie->m_entityState.m_id == 17) {
		return false;
	}
	else {
		return CallFunc<bool, ZombieModernPoleVaulter*, int>(0xC4D2EC, zombie, a2);
	}
}

void PoleWalkOnLoop(ZombieModernPoleVaulter* zombie)
{
	if (ZombieIsDeadOrDying(zombie)) {
		CallFunc<void, ZombieModernPoleVaulter*>(0xC506B4, zombie);
		return;
	}

	auto rig = reinterpret_cast<ZombieAnimRig_ModernPoleVaulter*>(zombie->m_animRig.Get());

	if (rig != nullptr && rig->m_hasPole == true)
	{
		SetWalkSpeed(rig, PoleGetWalkSpeed(zombie));
		if ((zombie->m_realObjectFlags & 2) == 0) {
			int currentGridX = static_cast<int>((zombie->m_position.x - 200.0f) / 64.0f);
			int currentGridY = static_cast<int>((zombie->m_position.y - 160.0f) / 76.0f);

			Rect checkRect;
			checkRect.mX = currentGridX - 1;
			checkRect.mY = currentGridY;
			checkRect.mWidth = 2;
			checkRect.mHeight = 1;

			std::vector<BoardEntity*> checkEntities;
			GetEntitiesInRectGrid(&checkEntities, 63, &checkRect);

			for (BoardEntity* checkEnt : checkEntities) {
				if (checkEnt != nullptr && checkEnt->IsType(ZombieLadder::StaticGetType())) {
					return;
				}
			}
			auto* props = reinterpret_cast<ZombieModernPoleVaulterProps*>(zombie->m_propertySheet.Get());
			float zX = zombie->m_position.x;
			float zY = zombie->m_position.y;
			float zZ = zombie->m_position.z;

			Rect jumpRect;
			jumpRect.mWidth = props->JumpingRect.mWidth;
			jumpRect.mHeight = props->JumpingRect.mHeight;
			jumpRect.mX = static_cast<int>(zX - (jumpRect.mWidth / 2.0f) + props->JumpingRect.mX);
			jumpRect.mY = static_cast<int>((zY - zZ) - jumpRect.mHeight + props->JumpingRect.mY);
			int zRow = static_cast<int>((zY - 160.0f) / 76.0f);

			std::vector<BoardEntity*> entityList;
			GetEntitiesInRectPixel(&entityList, 38, &jumpRect, zRow, zRow);
			bool foundObstacle = false;
			BoardEntityHeight finalHeight = BoardEntityHeight::ground;

			for (BoardEntity* entity : entityList) {
				if (entity == nullptr) continue;

				BoardEntityHeight currentHeight = BoardEntityHeight::ground;
				bool isValidObstacle = false;

				if (entity->IsType(PlantGroup::StaticGetType())) {
					PlantGroup* plantGroup = reinterpret_cast<PlantGroup*>(entity);

					for (auto& weakPlant : plantGroup->m_plants.m_plants) {
						Plant* p = weakPlant.Get();
						if (p != nullptr) {
							auto* pProps = reinterpret_cast<PlantPropertySheet*>(p->m_propertySheet.Get());

							if (pProps != nullptr && pProps->Height != BoardEntityHeight::ground) {
								isValidObstacle = true;

								if (pProps->Height == BoardEntityHeight::tall) {
									currentHeight = BoardEntityHeight::tall;
									break;
								}
								else if (pProps->Height == BoardEntityHeight::normal && currentHeight != BoardEntityHeight::tall) {
									currentHeight = BoardEntityHeight::normal;
								}
								else if (pProps->Height == BoardEntityHeight::low && currentHeight == BoardEntityHeight::ground) {
									currentHeight = BoardEntityHeight::low;
								}
							}
						}
					}
				}
				else if (entity->IsType(GridItem::StaticGetType())) {

					if (entity->m_teamFlags == 1 && zombie->m_teamFlags == 2) {

						GridItem* gridItem = reinterpret_cast<GridItem*>(entity);
						auto* gridProps = reinterpret_cast<GridItemPropertySheet*>(gridItem->m_propertySheet.Get());
						if (gridProps->Height != BoardEntityHeight::ground) {
							isValidObstacle = true;
							currentHeight = gridProps->Height;
						}
					}
				}
				if (isValidObstacle) {
					foundObstacle = true;

					if (currentHeight == BoardEntityHeight::tall) {
						finalHeight = BoardEntityHeight::tall;
						break;
					}
					else if (currentHeight == BoardEntityHeight::normal && finalHeight != BoardEntityHeight::tall) {
						finalHeight = BoardEntityHeight::normal;
					}
					else if (currentHeight == BoardEntityHeight::low && finalHeight == BoardEntityHeight::ground) {
						finalHeight = BoardEntityHeight::low;
					}
				}
			}

			if (foundObstacle) {
				if (finalHeight == BoardEntityHeight::tall) {
					ZombieEnterState(zombie, 17, 0);
				}
				else if (finalHeight == BoardEntityHeight::normal || finalHeight == BoardEntityHeight::low) {
					if (props->Feastivus == true) {
						ZombieEnterState(zombie, (rand() % 2 == 0) ? 16 : 18, 0);
					}
					else {
						ZombieEnterState(zombie, 16, 0);
					}
				}
			}
			return;
		}
	}
	else {
		CallFunc<void, ZombieModernPoleVaulter*>(0xC506B4, zombie);
	}
}

void PoleActionFrame(ZombieModernPoleVaulter* self, SexyString* currentAnim, SexyString* actionName, SexyString* param, float nextFrameTime)
{
	if (*actionName == "use_action")
	{
		auto* props = reinterpret_cast<ZombieModernPoleVaulterProps*>(self->m_propertySheet.Get());

		int zX = static_cast<int>((self->m_position.x - 200.0f) / 64.0f);
		int zY = static_cast<int>((self->m_position.y - 160.0f) / 76.0f);

		Rect ExplodeRect;
		ExplodeRect.mX = (zX - 8 > 0) ? (zX - 8) : 0;
		ExplodeRect.mY = zY;
		ExplodeRect.mWidth = 8;
		ExplodeRect.mHeight = 1;

		std::vector<BoardEntity*> entityList;
		GetEntitiesInRectGrid(&entityList, 63, &ExplodeRect);

		PlantGroup* closestPlant = nullptr;
		float minDistance = 99999.0f;

		for (BoardEntity* ptr : entityList) {
			if (ptr != nullptr && ptr->IsType(PlantGroup::StaticGetType()))
			{
				PlantGroup* plant = static_cast<PlantGroup*>(ptr);
				float distance = self->m_position.x - plant->m_position.x;

				if (distance > 0.0f && distance < minDistance)
				{
					minDistance = distance;
					closestPlant = plant;
				}
			}
		}

		if (closestPlant != nullptr)
		{
			auto props = reinterpret_cast<ZombieModernPoleVaulterProps*>(self->m_propertySheet.Get());
			auto rig = reinterpret_cast<ZombieAnimRig_ModernPoleVaulter*>(self->m_animRig.Get());

			Rect projRect;
			GetAnimRigSpriteRect(rig, "Pole", &projRect);

			ZombieConditionTracker* zTracker = &self->m_conditionTracker;
			uint8_t* cond = zTracker->m_states.data();

			if (cond != nullptr && *cond != 0) {
				typedef void (*UpdateConditionsFunc)(ZombieConditionTracker*);
				((UpdateConditionsFunc)(*(void***)zTracker)[3])(zTracker);
				*cond = 0;
			}

			float scale = zTracker->m_scale;
			auto sexyApp = SexyApp::GetInstance();

			int spriteXPos = sexyApp->ScaleRender(projRect.mX + (projRect.mWidth / 2));
			int spriteYPos = sexyApp->ScaleRender(projRect.mY + (projRect.mHeight / 2));

			SexyVector2 artCenter = props->ArtCenter;
			SexyVector3 shadowOffset = props->ShadowOffset;
			float spawnPosX = self->m_position.x + (scale * ((float)spriteXPos - artCenter.x));
			float spawnPosY = self->m_position.y - self->m_position.z;
			float spawnPosZ = scale * ((float)spriteYPos - shadowOffset.x);
			auto projectileToSpawn = props->PoleProjectile;

			auto projectile = AddProjectile(&projectileToSpawn, self, spawnPosX, spawnPosY, spawnPosZ);
			FirePultProjectile(projectile,
				closestPlant->m_position,
				250.0f,
				1.5f);
		}
	}
	if (*actionName == "jumping_done")
	{
		auto rig = reinterpret_cast<ZombieAnimRig_ModernPoleVaulter*>(self->m_animRig.Get());
		rig->m_hasPole = false;
	}
}
void PoleOnArmDrop(ZombieModernPoleVaulter* zombie) {
	auto rig = reinterpret_cast<ZombieAnimRig_ModernPoleVaulter*>(zombie->m_animRig.Get());
	auto props = reinterpret_cast<ZombieModernPoleVaulterProps*>(zombie->m_propertySheet.Get());
	if (rig->m_hasPole == true && props->Throw == true) {
		ZombieEnterState(zombie, 19, 0);
	}
}
void ZombieModernPoleVaulter::JumpOnEnter(ZombieModernPoleVaulter* zombie)
{
	ZombieAllowMovement(zombie, true);
	return RegisterEventAfterAnim(zombie, "jump", "onJumpingCompleted");
}
void ZombieModernPoleVaulter::JumpOnLoop(ZombieModernPoleVaulter* zombie)
{

}
void ZombieModernPoleVaulter::JumpOnExit(ZombieModernPoleVaulter* zombie)
{

}

void ZombieModernPoleVaulter::BonkOnEnter(ZombieModernPoleVaulter* zombie)
{
	ZombieAllowMovement(zombie, true);
	return RegisterEventAfterAnim(zombie, "jump_tallnut", "onBonkingCompleted");
}

void ZombieModernPoleVaulter::BonkOnLoop(ZombieModernPoleVaulter* zombie)
{

}

void ZombieModernPoleVaulter::BonkOnExit(ZombieModernPoleVaulter* zombie)
{

}
void ZombieModernPoleVaulter::FarJumpOnEnter(ZombieModernPoleVaulter* zombie)
{
	ZombieAllowMovement(zombie, true);
	return RegisterEventAfterAnim(zombie, "jump_cobcannon", "onFarJumpingCompleted");
}
void ZombieModernPoleVaulter::FarJumpOnLoop(ZombieModernPoleVaulter* zombie)
{

}
void ZombieModernPoleVaulter::FarJumpOnExit(ZombieModernPoleVaulter* zombie)
{

}
void ZombieModernPoleVaulter::ThrowOnEnter(ZombieModernPoleVaulter* zombie)
{
	RegisterEventAfterAnim(zombie, "throw_pole", "onThrowingPoleCompleted");
}
void ZombieModernPoleVaulter::ThrowOnLoop(ZombieModernPoleVaulter* zombie)
{

}
void ZombieModernPoleVaulter::ThrowOnExit(ZombieModernPoleVaulter* zombie)
{

}

void JumpingCompletedCallback(Zombie* zombie) {
	auto rig = reinterpret_cast<ZombieAnimRig_ModernPoleVaulter*>(zombie->m_animRig.Get());
	ZombieModernPoleVaulter* poleZombie = static_cast<ZombieModernPoleVaulter*>(zombie);
	if (poleZombie && !ZombieIsDeadOrDying(poleZombie) && !ZombieIsInState(poleZombie, 3)) {
		rig->m_hasPole = false;
		ZombieEnterState(poleZombie, 1, 0);
		SetWalkSpeed(rig, PoleGetWalkSpeed(poleZombie));
	}
}
void BonkingCompletedCallback(Zombie* zombie) {
	auto rig = reinterpret_cast<ZombieAnimRig_ModernPoleVaulter*>(zombie->m_animRig.Get());
	ZombieModernPoleVaulter* poleZombie = static_cast<ZombieModernPoleVaulter*>(zombie);
	if (poleZombie && !ZombieIsDeadOrDying(poleZombie) && !ZombieIsInState(poleZombie, 3)) {
		rig->m_hasPole = false;
		ZombieEnterState(poleZombie, 1, 0);
		SetWalkSpeed(rig, PoleGetWalkSpeed(poleZombie));
	}
}
void FarJumpingCompletedCallback(Zombie* zombie) {
	auto rig = reinterpret_cast<ZombieAnimRig_ModernPoleVaulter*>(zombie->m_animRig.Get());
	ZombieModernPoleVaulter* poleZombie = static_cast<ZombieModernPoleVaulter*>(zombie);
	if (poleZombie && !ZombieIsDeadOrDying(poleZombie) && !ZombieIsInState(poleZombie, 3)) {
		rig->m_hasPole = false;
		ZombieEnterState(poleZombie, 1, 0);
		SetWalkSpeed(rig, PoleGetWalkSpeed(poleZombie));
	}
}
void ThrowingCompletedCallback(Zombie* zombie) {
	auto rig = reinterpret_cast<ZombieAnimRig_ModernPoleVaulter*>(zombie->m_animRig.Get());
	ZombieModernPoleVaulter* poleZombie = static_cast<ZombieModernPoleVaulter*>(zombie);
	if (poleZombie && !ZombieIsDeadOrDying(poleZombie) && !ZombieIsInState(poleZombie, 3)) {
		rig->m_hasPole = false;
		ZombieEnterState(poleZombie, 1, 0);
		SetWalkSpeed(rig, PoleGetWalkSpeed(poleZombie));
	}
}
void ZombieModernPoleVaulter::ModInit() {
	LOGI("ZombiePole mod init");

	vftable = CreateChildVFTable(204 + 9, getActualOffset(0x241D430), 204);
	PatchVFTable(vftable, (void*)ZombieModernPoleVaulter::StaticGetType, 0);

	PatchVFTable(vftable, (void*)PoleOnSpawn, 49);
	PatchVFTable(vftable, (void*)PoleIsBeingTossedByPlant, 97);
	PatchVFTable(vftable, (void*)PoleGetWalkSpeed, 118);
	PatchVFTable(vftable, (void*)PoleWalkOnLoop, 124);
	PatchVFTable(vftable, (void*)PoleActionFrame, 170);
	PatchVFTable(vftable, (void*)PoleOnArmDrop, 174);

	PatchVFTable(vftable, (void*)ZombieModernPoleVaulter::JumpOnEnter, 204);
	PatchVFTable(vftable, (void*)ZombieModernPoleVaulter::JumpOnLoop, 205);
	PatchVFTable(vftable, (void*)ZombieModernPoleVaulter::JumpOnExit, 206);

	PatchVFTable(vftable, (void*)ZombieModernPoleVaulter::BonkOnEnter, 207);
	PatchVFTable(vftable, (void*)ZombieModernPoleVaulter::BonkOnLoop, 208);
	PatchVFTable(vftable, (void*)ZombieModernPoleVaulter::BonkOnExit, 209);

	PatchVFTable(vftable, (void*)ZombieModernPoleVaulter::FarJumpOnEnter, 210);
	PatchVFTable(vftable, (void*)ZombieModernPoleVaulter::FarJumpOnLoop, 211);
	PatchVFTable(vftable, (void*)ZombieModernPoleVaulter::FarJumpOnExit, 212);

	ZombieModernPoleVaulter::StaticGetType();
	LOGI("ZombiePole finish init");
}

void ZombieModernPoleVaulter::buildEventCallbacks(Reflection::CRefManualSymbolBuilder* builder, Reflection::RClass* rtClass)
{
	IF_CALLBACK_NOTSETUP(ZombieModernPoleVaulter) {
		SetupLiteralDelegate(&jumpingCompletedDelegate, JumpingCompletedCallback);
		SetupLiteralDelegate(&bonkingCompletedDelegate, BonkingCompletedCallback);
		SetupLiteralDelegate(&farJumpingCompletedDelegate, FarJumpingCompletedCallback);
		SetupLiteralDelegate(&throwingCompletedDelegate, ThrowingCompletedCallback);
		ZombieModernPoleVaulter_delegatesSetup = true;
		LOGI("SO TRUE");
	}
	RegisterEventCallback(builder, rtClass, "onJumpingCompleted", jumpingCompletedDelegate);
	RegisterEventCallback(builder, rtClass, "onBonkingCompleted", bonkingCompletedDelegate);
	RegisterEventCallback(builder, rtClass, "onFarJumpingCompleted", farJumpingCompletedDelegate);
	RegisterEventCallback(builder, rtClass, "onThrowingPoleCompleted", throwingCompletedDelegate);
	LOGI("Reg event complete");
}


void ZombieModernPoleVaulter::buildStates()
{
	StateMachineTableBuilder* stateMachine = CallGetStateMachine(ZombieModernPoleVaulter::StaticGetType());
	RegisterStateByOffsets(stateMachine,
		16,
		(uintptr_t)ZombieModernPoleVaulter::JumpOnEnter,
		(uintptr_t)ZombieModernPoleVaulter::JumpOnLoop,
		(uintptr_t)ZombieModernPoleVaulter::JumpOnExit,
		"ZS_PoleVaulter_Jumping");
	RegisterStateByOffsets(stateMachine,
		17,
		(uintptr_t)ZombieModernPoleVaulter::BonkOnEnter,
		(uintptr_t)ZombieModernPoleVaulter::BonkOnLoop,
		(uintptr_t)ZombieModernPoleVaulter::BonkOnExit,
		"ZS_PoleVaulter_Bonking");
	RegisterStateByOffsets(stateMachine,
		18,
		(uintptr_t)ZombieModernPoleVaulter::FarJumpOnEnter,
		(uintptr_t)ZombieModernPoleVaulter::FarJumpOnLoop,
		(uintptr_t)ZombieModernPoleVaulter::FarJumpOnExit,
		"ZS_PoleVaulter_FarJumping");
	RegisterStateByOffsets(stateMachine,
		19,
		(uintptr_t)ZombieModernPoleVaulter::ThrowOnEnter,
		(uintptr_t)ZombieModernPoleVaulter::ThrowOnLoop,
		(uintptr_t)ZombieModernPoleVaulter::ThrowOnExit,
		"ZS_PoleVaulter_Throwing");
	LOGI("Reg state complete");
}
