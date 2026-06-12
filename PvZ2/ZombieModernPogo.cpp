
#include "ZombieModernPogo.h"
#include "ZombieStateHelper.h"
#include "ZombieModernPogoProps.h"
#include "ZombieState.h"
#include "StateMachineBuilder.h"
#include "ZombieAnimRig_ModernPogo.h"
#include "Plant.h"
#include "DamageInfo.h"
#include "ZombieHelper.h"
#include "GridItem.h"

void* ZombieModernPogo::vftable = nullptr;
Sexy::RtClass* ZombieModernPogo::s_rtClass = nullptr;;
DECLARE_DELEGATES_SETUP(ZombieModernPogo)

static Sexy::DelegateBase jumpingCompletedDelegate;

static Sexy::DelegateBase bonkingCompletedDelegate;

static Sexy::DelegateBase lostPogoCompletedDelegate;

static Sexy::DelegateBase walkingPogoContinueDelegate;

float PogoGetWalkSpeed(ZombieModernPogo* zombie)
{
	auto rig = reinterpret_cast<ZombieAnimRig_ModernPogo*>(zombie->m_animRig.Get());
	auto props = reinterpret_cast<ZombieModernPogoProps*>(zombie->m_propertySheet.Get());
	if (rig->m_hasPogo == true) {
		return props->JumpingSpeed;
	}
	else {
		return zombie->m_walkSpeed;
	}
}
void* PogoTakeDamage(ZombieModernPogo* thisPtr, DamageInfo* damageInfo)
{
	auto* props = reinterpret_cast<ZombieModernPogoProps*>(thisPtr->m_propertySheet.Get());
	DamageInfo newDmgInfo = *damageInfo;

	float balloonHP = 0.0f;
	for (size_t i = 0; i < thisPtr->m_armor.size(); i++)
	{
		Armor* armorInstance = thisPtr->m_armor[i].Get();
		if (armorInstance != nullptr && !armorInstance->m_destroyed && armorInstance->m_health > 0)
		{
			balloonHP = armorInstance->m_health;
			break;
		}
	}
	if (newDmgInfo.m_damage >= props->DamageAmountWhichAlsoKillsBasic)
	{
		newDmgInfo.m_flags |= DamageTypeFlags::damage_bypass_shield;

		if (balloonHP > 0)
		{
			thisPtr->m_hasTakenCatastrophicDamage = true;
		}
	}
	else
	{
		if (balloonHP > 0)
		{
			if (newDmgInfo.m_damage >= balloonHP)
			{
				newDmgInfo.m_flags &= ~DamageTypeFlags::damage_bypass_shield;
				newDmgInfo.m_flags &= ~DamageTypeFlags::damage_hits_shield_and_body;
				newDmgInfo.m_flags != DamageTypeFlags::damage_hits_only_shield;
				float calculatedDamage = balloonHP;

				float effDamageScale = thisPtr->m_damageScale;
				calculatedDamage /= effDamageScale;

				bool isShrunken = CallFunc<bool, Zombie*, int>(0xC3E44C, thisPtr, zombie_condition_shrinking)
					|| CallFunc<bool, Zombie*, int>(0xC3E44C, thisPtr, zombie_condition_shrunken);

				if (isShrunken && thisPtr->m_shrunkenDamageScale > 0.001f)
				{
					calculatedDamage /= thisPtr->m_shrunkenDamageScale;
				}
				newDmgInfo.m_damage = calculatedDamage;
			}
		}
	}

	typedef void* (*funcC43B90)(ZombieModernPogo*, DamageInfo*);
	static auto* ZTakeDmg = ((funcC43B90)getActualOffset(0xC43B90));

	return ZTakeDmg(thisPtr, &newDmgInfo);
}
void PogoOnSpawn(ZombieModernPogo* zombie)
{
	zombie->m_hasTakenCatastrophicDamage = false;
	auto rig = reinterpret_cast<ZombieAnimRig_ModernPogo*>(zombie->m_animRig.Get());
	for (auto& weakArmor : zombie->m_armor)
    {
        Armor* armor = weakArmor.Get();
        if (armor != nullptr)
        {
            auto* armorProps = reinterpret_cast<ArmorPropertySheet*>(armor->m_propertySheetPtr.Get());

            if (armorProps != nullptr && armorProps->ArmorType == "Pogo")
            {
				rig->m_hasPogo = true;
                break;
            }
        }
    }
	ZombieOnSpawn(zombie);
	SetWalkSpeed(rig, PogoGetWalkSpeed(zombie));
	ZombieEnterState(zombie, 19, 0);
}
void PogoEnterWalk(ZombieModernPogo* zombie)
{
	auto rig = reinterpret_cast<ZombieAnimRig_ModernPogo*>(zombie->m_animRig.Get());
	if (rig->m_hasPogo == true) {
		ZombieEnterState(zombie, 19, 0);
	}
	else {
		ZombieEnterState(zombie, 1, 0);
	}
}
bool PogoIsBeingTossedByPlant(ZombieModernPogo* zombie, int a2) {
	if (zombie->m_entityState.m_id == 16 || zombie->m_entityState.m_id == 17) {
		return false;
	}
	else {
		return CallFunc<bool, ZombieModernPogo*, int>(0xC4D2EC, zombie, a2);
	}
}
void PogoOnArmorDestroyed(ZombieModernPogo* zombie, int a2, SexyString* armorName)
{
	if (*armorName == "Pogo" && !ZombieIsDeadOrDying(zombie)) {
		auto rig = reinterpret_cast<ZombieAnimRig_ModernPogo*>(zombie->m_animRig.Get());
		rig->m_hasPogo = false;
		ZombieEnterState(zombie, 18, 0);
	}
}
SexyString GetPogoAnimShock(ZombieModernPogo* zombie, DamageInfo* damage) {
	if (zombie->m_hasTakenCatastrophicDamage == true) {
		return "POPANIM_EFFECTS_ZOMBIE_POGO_SHOCK";
	}
	else {
		return "POPANIM_EFFECTS_ZOMBIE_SHOCK";
	}
}

SexyString GetPogoAnimAsh(ZombieModernPogo* zombie, DamageInfo* damage) {
	if (zombie->m_hasTakenCatastrophicDamage == true) {
		return "POPANIM_EFFECTS_ZOMBIE_POGO_ASH";
	}
	else {
		return "POPANIM_EFFECTS_ZOMBIE_ASH";
	}
}


void ZombieModernPogo::JumpOnEnter(ZombieModernPogo* zombie)
{
	ZombieAllowMovement(zombie, true);
	return RegisterEventAfterAnim(zombie, "jump", "onJumpingCompleted");
}
void ZombieModernPogo::JumpOnLoop(ZombieModernPogo* zombie)
{

}
void ZombieModernPogo::JumpOnExit(ZombieModernPogo* zombie)
{

}

void ZombieModernPogo::BonkOnEnter(ZombieModernPogo* zombie)
{
	auto rig = reinterpret_cast<ZombieAnimRig_ModernPogo*>(zombie->m_animRig.Get());
	rig->m_hasPogo = false;
	ZombieAllowMovement(zombie, true);
	return RegisterEventAfterAnim(zombie, "jump_tallnut", "onBonkingCompleted");
}

void ZombieModernPogo::BonkOnLoop(ZombieModernPogo* zombie)
{

}

void ZombieModernPogo::BonkOnExit(ZombieModernPogo* zombie)
{

}
void ZombieModernPogo::LostPogoOnEnter(ZombieModernPogo* zombie)
{
	return RegisterEventAfterAnim(zombie, "lose_pogo", "onLostPogoCompleted");
}
void ZombieModernPogo::LostPogoOnLoop(ZombieModernPogo* zombie)
{

}
void ZombieModernPogo::LostPogoOnExit(ZombieModernPogo* zombie)
{

}
void ZombieModernPogo::WalkPogoOnEnter(ZombieModernPogo* zombie)
{
	auto rig = reinterpret_cast<ZombieAnimRig_ModernPogo*>(zombie->m_animRig.Get());
	rig->m_hasPogo = true;
	ZombieAllowMovement(zombie, true);
	return RegisterEventOnWalkLoop(zombie, "onWalkPogoContinued");
}
void ZombieModernPogo::WalkPogoOnLoop(ZombieModernPogo* zombie)
{
	if (ZombieIsDeadOrDying(zombie)) {
		CallFunc<void, ZombieModernPogo*>(0xC506B4, zombie);
		return;
	}

	auto rig = reinterpret_cast<ZombieAnimRig_ModernPogo*>(zombie->m_animRig.Get());

	if (rig != nullptr && rig->m_hasPogo == true)
	{
		SetWalkSpeed(rig, PogoGetWalkSpeed(zombie));

		auto* props = reinterpret_cast<ZombieModernPogoProps*>(zombie->m_propertySheet.Get());
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
				ZombieEnterState(zombie, 16, 0);
			}
		}
	}
}
void ZombieModernPogo::WalkPogoOnExit(ZombieModernPogo* zombie)
{

}
void PogoJumpingCompletedCallback(Zombie* zombie) {
	auto rig = reinterpret_cast<ZombieAnimRig_ModernPogo*>(zombie->m_animRig.Get());
	ZombieModernPogo* PogoZombie = static_cast<ZombieModernPogo*>(zombie);
	if (PogoZombie) {
		rig->m_hasPogo = true;
		ZombieEnterState(PogoZombie, 19, 0);
		SetWalkSpeed(rig, PogoGetWalkSpeed(PogoZombie));
	}
}
void PogoBonkingCompletedCallback(Zombie* zombie) {
	auto rig = reinterpret_cast<ZombieAnimRig_ModernPogo*>(zombie->m_animRig.Get());
	ZombieModernPogo* PogoZombie = static_cast<ZombieModernPogo*>(zombie);
	if (PogoZombie) {
		rig->m_hasPogo = false;
		for (auto& weakArmor : zombie->m_armor)
		{
			Armor* armor = weakArmor.Get();
			if (armor != nullptr && !armor->m_destroyed)
			{
				armor->m_destroyed = true;
			}
		}
		ZombieEnterState(PogoZombie, 1, 0);
		SetWalkSpeed(rig, PogoGetWalkSpeed(PogoZombie));
	}
}
void LostPogoCompletedCallback(Zombie* zombie) {
	auto rig = reinterpret_cast<ZombieAnimRig_ModernPogo*>(zombie->m_animRig.Get());
	ZombieModernPogo* PogoZombie = static_cast<ZombieModernPogo*>(zombie);
	if (PogoZombie) {
		rig->m_hasPogo = false;
		ZombieEnterState(PogoZombie, 1, 0);
		SetWalkSpeed(rig, PogoGetWalkSpeed(PogoZombie));
	}
}
void WalkPogoCompletedCallback(Zombie* zombie) {}

void ZombieModernPogo::ModInit() {
	LOGI("ZombiePogo mod init");

	vftable = CreateChildVFTable(204 + 15, getActualOffset(0x241D430), 204);
	PatchVFTable(vftable, (void*)ZombieModernPogo::StaticGetType, 0);

	PatchVFTable(vftable, (void*)PogoTakeDamage, 35);
	PatchVFTable(vftable, (void*)PogoOnSpawn, 49);
	PatchVFTable(vftable, (void*)PogoEnterWalk, 62);
	PatchVFTable(vftable, (void*)PogoIsBeingTossedByPlant, 97);
	PatchVFTable(vftable, (void*)PogoOnArmorDestroyed, 115);
	PatchVFTable(vftable, (void*)PogoGetWalkSpeed, 118);
	PatchVFTable(vftable, (void*)GetPogoAnimShock, 189);
	PatchVFTable(vftable, (void*)GetPogoAnimAsh, 190);

	PatchVFTable(vftable, (void*)ZombieModernPogo::JumpOnEnter, 204);
	PatchVFTable(vftable, (void*)ZombieModernPogo::JumpOnLoop, 205);
	PatchVFTable(vftable, (void*)ZombieModernPogo::JumpOnExit, 206);

	PatchVFTable(vftable, (void*)ZombieModernPogo::BonkOnEnter, 207);
	PatchVFTable(vftable, (void*)ZombieModernPogo::BonkOnLoop, 208);
	PatchVFTable(vftable, (void*)ZombieModernPogo::BonkOnExit, 209);

	PatchVFTable(vftable, (void*)ZombieModernPogo::LostPogoOnEnter, 210);
	PatchVFTable(vftable, (void*)ZombieModernPogo::LostPogoOnLoop, 211);
	PatchVFTable(vftable, (void*)ZombieModernPogo::LostPogoOnExit, 212);

	PatchVFTable(vftable, (void*)ZombieModernPogo::WalkPogoOnEnter, 213);
	PatchVFTable(vftable, (void*)ZombieModernPogo::WalkPogoOnLoop, 214);
	PatchVFTable(vftable, (void*)ZombieModernPogo::WalkPogoOnExit, 215);

	ZombieModernPogo::StaticGetType();
	LOGI("ZombiePogo finish init");
}

void ZombieModernPogo::buildEventCallbacks(Reflection::CRefManualSymbolBuilder* builder, Reflection::RClass* rtClass)
{
	IF_CALLBACK_NOTSETUP(ZombieModernPogo) {
		SetupLiteralDelegate(&jumpingCompletedDelegate, PogoJumpingCompletedCallback);
		SetupLiteralDelegate(&bonkingCompletedDelegate, PogoBonkingCompletedCallback);
		SetupLiteralDelegate(&lostPogoCompletedDelegate, LostPogoCompletedCallback);
		SetupLiteralDelegate(&walkingPogoContinueDelegate, WalkPogoCompletedCallback);
		ZombieModernPogo_delegatesSetup = true;
		LOGI("SO TRUE");
	}
	RegisterEventCallback(builder, rtClass, "onJumpingCompleted", jumpingCompletedDelegate);
	RegisterEventCallback(builder, rtClass, "onBonkingCompleted", bonkingCompletedDelegate);
	RegisterEventCallback(builder, rtClass, "onLostPogoCompleted", lostPogoCompletedDelegate);
	RegisterEventCallback(builder, rtClass, "onWalkPogoContinued", walkingPogoContinueDelegate);
	LOGI("Reg event complete");
}


void ZombieModernPogo::buildStates()
{
	StateMachineTableBuilder* stateMachine = CallGetStateMachine(ZombieModernPogo::StaticGetType());
	RegisterStateByOffsets(stateMachine,
		16,
		(uintptr_t)ZombieModernPogo::JumpOnEnter,
		(uintptr_t)ZombieModernPogo::JumpOnLoop,
		(uintptr_t)ZombieModernPogo::JumpOnExit,
		"ZS_Pogo_Jumping");
	RegisterStateByOffsets(stateMachine,
		17,
		(uintptr_t)ZombieModernPogo::BonkOnEnter,
		(uintptr_t)ZombieModernPogo::BonkOnLoop,
		(uintptr_t)ZombieModernPogo::BonkOnExit,
		"ZS_Pogo_Bonking");
	RegisterStateByOffsets(stateMachine,
		18,
		(uintptr_t)ZombieModernPogo::LostPogoOnEnter,
		(uintptr_t)ZombieModernPogo::LostPogoOnLoop,
		(uintptr_t)ZombieModernPogo::LostPogoOnExit,
		"ZS_Pogo_LostPogo");
	RegisterStateByOffsets(stateMachine,
		19,
		(uintptr_t)ZombieModernPogo::WalkPogoOnEnter,
		(uintptr_t)ZombieModernPogo::WalkPogoOnLoop,
		(uintptr_t)ZombieModernPogo::WalkPogoOnExit,
		"ZS_Pogo_WalkPogo");
	LOGI("Reg state complete");
}
