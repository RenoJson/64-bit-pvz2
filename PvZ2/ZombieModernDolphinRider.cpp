#include "ZombieModernDolphinRider.h"
#include "ZombieStateHelper.h"
#include "ZombieModernDolphinRiderProps.h"
#include "ZombieState.h"
#include "StateMachineBuilder.h"
#include "Plant.h"
#include "DamageInfo.h"
#include "ZombieAnimRig_ModernDolphinRider.h"
#include "Board.h"

void* ZombieModernDolphinRider::vftable = nullptr;
Sexy::RtClass* ZombieModernDolphinRider::s_rtClass = nullptr;;
typedef void (*zombieEnterState)(ZombieModernDolphinRider*, int, int);
typedef Zombie* (*zombieAllowMovement)(Zombie*, int);
typedef void* (*playSoundEvent)(ZombieModernDolphinRider*, const SexyString&, float);
typedef void (*LoopWalk)(ZombieModernDolphinRider*);
typedef Plant* (*getTarg)(ZombieModernDolphinRider*);
typedef bool (*isDeadOrDying)(ZombieModernDolphinRider*);
typedef bool (*isHeadDrop)(ZombieModernDolphinRider*);
typedef bool (*isInWaterRegion)(Board*, SexyVector3*);
typedef Zombie* (*updatePos)(ZombieModernDolphinRider*, SexyVector3*);
typedef void (*setSpeed)(ZombieAnimRig*, float);

DECLARE_DELEGATES_SETUP(ZombieModernDolphinRider)
static Sexy::DelegateBase jumpingCompletedDelegate;

static Sexy::DelegateBase bonkingCompletedDelegate;

static Sexy::DelegateBase preWalkContinueDelegate;

static Sexy::DelegateBase transitionCompletedDelegate;

static Sexy::DelegateBase ridingDolphinContinueDelegate;

static Sexy::DelegateBase ridingDolphinToWalkCompletedDelegate;

float DolphinRiderGetWalkSpeed(ZombieModernDolphinRider* zombie)
{
	auto props = reinterpret_cast<ZombieModernDolphinRiderProps*>(zombie->m_propertySheet.Get());
	if (zombie->m_isCarryingDolphin == true) {
		return props->SpeedWhenCarryingDolphin;
	}
	else if (zombie->m_isRidingDolphin == true) {
		return props->RidingSpeed;
	}
	else {
		return zombie->m_walkSpeed;
	}
}
Zombie* DolphinRiderOnSpawn(ZombieModernDolphinRider* zombie)
{
	zombie->m_isRidingDolphin = false;
	zombie->m_isCarryingDolphin = true;
	auto rig = reinterpret_cast<ZombieAnimRig_ModernDolphinRider*>(zombie->m_animRig.Get());
	rig->m_isRidingDolphin = zombie->m_isRidingDolphin;
	rig->m_isCarryingDolphin = zombie->m_isCarryingDolphin;
	typedef void (*zombieFun49)(ZombieModernDolphinRider*);
	((zombieFun49)getActualOffset(0xC3D1F0))(zombie);
	zombie->m_dpsScale = 2.0f;
	SexyVector3 zombiePos = { 920.0f, zombie->m_position.y, 0.0f };
	((setSpeed)getActualOffset(0x8DDAA4))(rig, DolphinRiderGetWalkSpeed(zombie));
	((zombieEnterState)getActualOffset(0xC3D428))(zombie, 18, 0);
	return ((updatePos)getActualOffset(0x628278))(zombie, &zombiePos);
	
}
SexyString GetDolphinRiderAnimShock(ZombieModernDolphinRider* zombie) {
	if (zombie->m_isRidingDolphin == true && zombie->m_isCarryingDolphin == false) {
		return "POPANIM_ZOMBIE_ZOMBIE_PVZ1_DOLPHINRIDER_SHOCK_SWIM";
	}
	else if (zombie->m_isRidingDolphin == false && zombie->m_isCarryingDolphin == true) {
		return "POPANIM_ZOMBIE_ZOMBIE_PVZ1_DOLPHINRIDER_SHOCK";
	}
	else {
		return "POPANIM_EFFECTS_ZOMBIE_SHOCK";
	}
}

SexyString GetDolphinRiderAnimAsh(ZombieModernDolphinRider* zombie) {
	if (zombie->m_isRidingDolphin == true && zombie->m_isCarryingDolphin == false) {
		return "POPANIM_ZOMBIE_ZOMBIE_PVZ1_DOLPHINRIDER_ASH_SWIM";
	}
	else if (zombie->m_isRidingDolphin == false && zombie->m_isCarryingDolphin == true) {
		return "POPANIM_ZOMBIE_ZOMBIE_PVZ1_DOLPHINRIDER_ASH";
	}
	else {
		return "POPANIM_EFFECTS_ZOMBIE_ASH";
	}
}
bool DolphinIsBeingTossedByPlant(ZombieModernDolphinRider* zombie, int a2) {
	if (zombie->m_entityState.m_id == 16 
		|| zombie->m_entityState.m_id == 17 
		|| zombie->m_entityState.m_id == 21) {
		return false;
	}
	else {
		return CallFunc<bool, ZombieModernDolphinRider*, int>(0xC4D2EC, zombie, a2);
	}
}
bool DolphinRiderIsHeadDrop(ZombieModernDolphinRider* zombie)
{
	return zombie->m_entityState.m_id == 20 || CallFunc<bool, ZombieModernDolphinRider*>(0xC41014, zombie);
}
void ZombieModernDolphinRider::JumpOnEnter(ZombieModernDolphinRider* zombie)
{
	((zombieAllowMovement)getActualOffset(0xC51F94))(zombie, 1);
	auto rig = reinterpret_cast<ZombieAnimRig_ModernDolphinRider*>(zombie->m_animRig.Get());
	zombie->m_isRidingDolphin = false;
	zombie->m_isCarryingDolphin = false;
	rig->m_isCarryingDolphin = zombie->m_isCarryingDolphin;
	rig->m_isRidingDolphin = zombie->m_isRidingDolphin;
	return RegisterEventAfterAnim(zombie, "jump", "onJumpingCompleted");
}

void ZombieModernDolphinRider::JumpOnLoop(ZombieModernDolphinRider* zombie)
{
}

void ZombieModernDolphinRider::JumpOnExit(ZombieModernDolphinRider* zombie)
{
}

void ZombieModernDolphinRider::BonkOnEnter(ZombieModernDolphinRider* zombie)
{
	((zombieAllowMovement)getActualOffset(0xC51F94))(zombie, 1);
	auto rig = reinterpret_cast<ZombieAnimRig_ModernDolphinRider*>(zombie->m_animRig.Get());
	zombie->m_isRidingDolphin = false;
	zombie->m_isCarryingDolphin = false;
	rig->m_isCarryingDolphin = zombie->m_isCarryingDolphin;
	rig->m_isRidingDolphin = zombie->m_isRidingDolphin;
	return RegisterEventAfterAnim(zombie, "jump_tallnut", "onBonkingCompleted");
}

void ZombieModernDolphinRider::BonkOnLoop(ZombieModernDolphinRider* zombie)
{
}

void ZombieModernDolphinRider::BonkOnExit(ZombieModernDolphinRider* zombie)
{
}

void ZombieModernDolphinRider::PreWalkBeforeRidingOnEnter(ZombieModernDolphinRider* zombie)
{
	((zombieAllowMovement)getActualOffset(0xC51F94))(zombie, 1);
	((playSoundEvent)getActualOffset(0x10B0608))(zombie, "Play_Zomb_Modern_Dolphin_Appears", 0.0f);
	return RegisterEventOnWalkLoop(zombie, "onPreWalkContinued");
}

void ZombieModernDolphinRider::PreWalkBeforeRidingOnLoop(ZombieModernDolphinRider* zombie)
{
	auto props = reinterpret_cast<ZombieModernDolphinRiderProps*>(zombie->m_propertySheet.Get());
	if (zombie->m_position.x <= props->DolphinTransitionLocation) {
		Board* board = Board::GetBoard();
		SexyVector3 zombiePos = {zombie->m_position.x - 96.0f, zombie->m_position.y, zombie->m_position.z};
		if (((isInWaterRegion)getActualOffset(0xAADE34))(board, &zombiePos) == true) {
			((zombieEnterState)getActualOffset(0xC3D428))(zombie, 19, 0);
		}
		else {
			((zombieEnterState)getActualOffset(0xC3D428))(zombie, 1, 0);
		}
	}
}

void ZombieModernDolphinRider::PreWalkBeforeRidingOnExit(ZombieModernDolphinRider* zombie)
{

}

void ZombieModernDolphinRider::TransitionOnEnter(ZombieModernDolphinRider* zombie)
{
	return RegisterEventAfterAnim(zombie, "swim_transition", "onTransitionCompleted");
}

void ZombieModernDolphinRider::TransitionOnLoop(ZombieModernDolphinRider* zombie)
{

}

void ZombieModernDolphinRider::TransitionOnExit(ZombieModernDolphinRider* zombie)
{

}

void ZombieModernDolphinRider::RideDolphinOnEnter(ZombieModernDolphinRider* zombie)
{
	((zombieAllowMovement)getActualOffset(0xC51F94))(zombie, 1);
	return RegisterEventOnWalkLoop(zombie, "onRidingDolphinContinued");
}

void ZombieModernDolphinRider::RideDolphinOnLoop(ZombieModernDolphinRider* zombie)
{
	Board* board = Board::GetBoard();
	SexyVector3 zombiePos = zombie->m_position;
	if (((isInWaterRegion)getActualOffset(0xAADE34))(board, &zombiePos) == false) {
		((zombieEnterState)getActualOffset(0xC3D428))(zombie, 21, 0);
	}
	else {

		auto* props = reinterpret_cast<ZombieModernDolphinRiderProps*>(zombie->m_propertySheet.Get());
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
		typedef void (*GetEntitiesInRectPixelFunc)(std::vector<BoardEntity*>*, int, Rect*, int, int);
		GetEntitiesInRectPixelFunc getEntitiesRectPixel = (GetEntitiesInRectPixelFunc)getActualOffset(0x86F340);

		getEntitiesRectPixel(&entityList, 38, &jumpRect, zRow, zRow);
		PlantGroup* targetPlantGroup = nullptr;
		for (BoardEntity* ptr : entityList) {
			if (ptr != nullptr && ptr->IsType(PlantGroup::StaticGetType())) {
				targetPlantGroup = reinterpret_cast<PlantGroup*>(ptr);
				break;
			}
		}
		if (targetPlantGroup != nullptr)
		{
			bool hasTallPlant = false;
			for (auto& weakPlant : targetPlantGroup->m_plants.m_plants)
			{
				Plant* p = weakPlant.Get();
				if (p != nullptr)
				{
					auto* pProps = reinterpret_cast<PlantPropertySheet*>(p->m_propertySheet.Get());
					if (pProps != nullptr && pProps->Height == BoardEntityHeight::tall)
					{
						hasTallPlant = true;
						break;
					}
				}
			}

			if (hasTallPlant) {
				((zombieEnterState)getActualOffset(0xC3D428))(zombie, 17, 0);
			}
			else {
				((zombieEnterState)getActualOffset(0xC3D428))(zombie, 16, 0);
			}
			return;
		}
	}
}

void ZombieModernDolphinRider::RideDolphinOnExit(ZombieModernDolphinRider* zombie)
{
}

void ZombieModernDolphinRider::RideDolphinToWalkOnEnter(ZombieModernDolphinRider* zombie)
{
	auto rig = reinterpret_cast<ZombieAnimRig_ModernDolphinRider*>(zombie->m_animRig.Get()); 
	zombie->m_isRidingDolphin = false;
	zombie->m_isCarryingDolphin = true;
	rig->m_isCarryingDolphin = zombie->m_isCarryingDolphin;
	rig->m_isRidingDolphin = zombie->m_isRidingDolphin;
	return RegisterEventAfterAnim(zombie, "jump2", "onRidingToWalkCompleted");
}

void ZombieModernDolphinRider::RideDolphinToWalkOnLoop(ZombieModernDolphinRider* zombie)
{
}

void ZombieModernDolphinRider::RideDolphinToWalkOnExit(ZombieModernDolphinRider* zombie)
{

}

void DolphinRiderJumpingCompletedCallback(Zombie* zombie) {
	auto rig = reinterpret_cast<ZombieAnimRig_ModernDolphinRider*>(zombie->m_animRig.Get());
	ZombieModernDolphinRider* dolphinRiderZombie = static_cast<ZombieModernDolphinRider*>(zombie);
	if (dolphinRiderZombie) {
		dolphinRiderZombie->m_dpsScale = 1.0f;
		((zombieEnterState)getActualOffset(0xC3D428))(dolphinRiderZombie, 1, 0);
		((setSpeed)getActualOffset(0x8DDAA4))(rig, DolphinRiderGetWalkSpeed(dolphinRiderZombie));
	}
}
void DolphinRiderBonkingCompletedCallback(Zombie* zombie) {
	auto rig = reinterpret_cast<ZombieAnimRig_ModernDolphinRider*>(zombie->m_animRig.Get());
	ZombieModernDolphinRider* dolphinRiderZombie = static_cast<ZombieModernDolphinRider*>(zombie);
	if (dolphinRiderZombie) {
		dolphinRiderZombie->m_dpsScale = 1.0f;
		((zombieEnterState)getActualOffset(0xC3D428))(dolphinRiderZombie, 1, 0);
		((setSpeed)getActualOffset(0x8DDAA4))(rig, DolphinRiderGetWalkSpeed(dolphinRiderZombie));
	}
}

void PreWalkDolphinRiderCompletedCallback(Zombie* zombie) {}

void TransitionCompletedCallback(Zombie* zombie) {
	auto rig = reinterpret_cast<ZombieAnimRig_ModernDolphinRider*>(zombie->m_animRig.Get());
	ZombieModernDolphinRider* dolphinRiderZombie = static_cast<ZombieModernDolphinRider*>(zombie);
	if (dolphinRiderZombie) {
		dolphinRiderZombie->m_isRidingDolphin = true;
		dolphinRiderZombie->m_isCarryingDolphin = false;
		rig->m_isCarryingDolphin = dolphinRiderZombie->m_isCarryingDolphin;
		rig->m_isRidingDolphin = dolphinRiderZombie->m_isRidingDolphin;
		((zombieEnterState)getActualOffset(0xC3D428))(dolphinRiderZombie, 20, 0);
		((setSpeed)getActualOffset(0x8DDAA4))(rig, DolphinRiderGetWalkSpeed(dolphinRiderZombie));
	}
}

void RidingDolphinCompletedCallback(Zombie* zombie) {}

void RidingToWalkCompletedCallback(Zombie* zombie) {
	auto rig = reinterpret_cast<ZombieAnimRig_ModernDolphinRider*>(zombie->m_animRig.Get());
	ZombieModernDolphinRider* dolphinRiderZombie = static_cast<ZombieModernDolphinRider*>(zombie);
	if (dolphinRiderZombie) {
		((zombieEnterState)getActualOffset(0xC3D428))(dolphinRiderZombie, 1, 0);
		((setSpeed)getActualOffset(0x8DDAA4))(rig, DolphinRiderGetWalkSpeed(dolphinRiderZombie));
	}
}

void ZombieModernDolphinRider::ModInit() {
	LOGI("ZombieDolphinRider mod init");

	vftable = CreateChildVFTable(204 + 21, getActualOffset(0x241D430), 204);
	PatchVFTable(vftable, (void*)ZombieModernDolphinRider::StaticGetType, 0);

	PatchVFTable(vftable, (void*)DolphinRiderOnSpawn, 49);
	PatchVFTable(vftable, (void*)DolphinIsBeingTossedByPlant, 97);
	PatchVFTable(vftable, (void*)DolphinRiderGetWalkSpeed, 118);
	PatchVFTable(vftable, (void*)GetDolphinRiderAnimShock, 189);
	PatchVFTable(vftable, (void*)GetDolphinRiderAnimAsh, 190);
	PatchVFTable(vftable, (void*)DolphinRiderIsHeadDrop, 195);

	PatchVFTable(vftable, (void*)ZombieModernDolphinRider::JumpOnEnter, 204);
	PatchVFTable(vftable, (void*)ZombieModernDolphinRider::JumpOnLoop, 205);
	PatchVFTable(vftable, (void*)ZombieModernDolphinRider::JumpOnExit, 206);

	PatchVFTable(vftable, (void*)ZombieModernDolphinRider::BonkOnEnter, 207);
	PatchVFTable(vftable, (void*)ZombieModernDolphinRider::BonkOnLoop, 208);
	PatchVFTable(vftable, (void*)ZombieModernDolphinRider::BonkOnExit, 209);

	PatchVFTable(vftable, (void*)ZombieModernDolphinRider::PreWalkBeforeRidingOnEnter, 210);
	PatchVFTable(vftable, (void*)ZombieModernDolphinRider::PreWalkBeforeRidingOnLoop, 211);
	PatchVFTable(vftable, (void*)ZombieModernDolphinRider::PreWalkBeforeRidingOnExit, 212);

	PatchVFTable(vftable, (void*)ZombieModernDolphinRider::TransitionOnEnter, 213);
	PatchVFTable(vftable, (void*)ZombieModernDolphinRider::TransitionOnLoop, 214);
	PatchVFTable(vftable, (void*)ZombieModernDolphinRider::TransitionOnExit, 215);

	PatchVFTable(vftable, (void*)ZombieModernDolphinRider::RideDolphinOnEnter, 216);
	PatchVFTable(vftable, (void*)ZombieModernDolphinRider::RideDolphinOnLoop, 217);
	PatchVFTable(vftable, (void*)ZombieModernDolphinRider::RideDolphinOnExit, 218);

	PatchVFTable(vftable, (void*)ZombieModernDolphinRider::RideDolphinToWalkOnEnter, 219);
	PatchVFTable(vftable, (void*)ZombieModernDolphinRider::RideDolphinToWalkOnLoop, 220);
	PatchVFTable(vftable, (void*)ZombieModernDolphinRider::RideDolphinToWalkOnExit, 221);

	ZombieModernDolphinRider::StaticGetType();
	LOGI("ZombieDolphinRider finish init");
}

void ZombieModernDolphinRider::buildEventCallbacks(Reflection::CRefManualSymbolBuilder* builder, Reflection::RClass* rtClass)
{
	IF_CALLBACK_NOTSETUP(ZombieModernDolphinRider) {
		SetupLiteralDelegate(&jumpingCompletedDelegate, DolphinRiderJumpingCompletedCallback);
		SetupLiteralDelegate(&bonkingCompletedDelegate, DolphinRiderBonkingCompletedCallback);
		SetupLiteralDelegate(&preWalkContinueDelegate, PreWalkDolphinRiderCompletedCallback);
		SetupLiteralDelegate(&transitionCompletedDelegate, TransitionCompletedCallback);
		SetupLiteralDelegate(&ridingDolphinContinueDelegate, RidingDolphinCompletedCallback);
		SetupLiteralDelegate(&ridingDolphinToWalkCompletedDelegate, RidingToWalkCompletedCallback);
		ZombieModernDolphinRider_delegatesSetup = true;
		LOGI("SO TRUE");
	}
	RegisterEventCallback(builder, rtClass, "onJumpingCompleted", jumpingCompletedDelegate);
	RegisterEventCallback(builder, rtClass, "onBonkingCompleted", bonkingCompletedDelegate);
	RegisterEventCallback(builder, rtClass, "onPreWalkContinued", preWalkContinueDelegate);
	RegisterEventCallback(builder, rtClass, "onTransitionCompleted", transitionCompletedDelegate);
	RegisterEventCallback(builder, rtClass, "onRidingDolphinContinued", ridingDolphinContinueDelegate);
	RegisterEventCallback(builder, rtClass, "onRidingToWalkCompleted", ridingDolphinToWalkCompletedDelegate);
	LOGI("Reg event complete");
}


void ZombieModernDolphinRider::buildStates()
{
	StateMachineTableBuilder* stateMachine = CallGetStateMachine(ZombieModernDolphinRider::StaticGetType());
	RegisterStateByOffsets(stateMachine,
		16,
		(uintptr_t)ZombieModernDolphinRider::JumpOnEnter,
		(uintptr_t)ZombieModernDolphinRider::JumpOnLoop,
		(uintptr_t)ZombieModernDolphinRider::JumpOnExit,
		"ZS_DolphinRider_Jumping");
	RegisterStateByOffsets(stateMachine,
		17,
		(uintptr_t)ZombieModernDolphinRider::BonkOnEnter,
		(uintptr_t)ZombieModernDolphinRider::BonkOnLoop,
		(uintptr_t)ZombieModernDolphinRider::BonkOnExit,
		"ZS_DolphinRider_Bonking");
	RegisterStateByOffsets(stateMachine,
		18,
		(uintptr_t)ZombieModernDolphinRider::PreWalkBeforeRidingOnEnter,
		(uintptr_t)ZombieModernDolphinRider::PreWalkBeforeRidingOnLoop,
		(uintptr_t)ZombieModernDolphinRider::PreWalkBeforeRidingOnExit,
		"ZS_DolphinRider_PreWalkBeforeRiding");
	RegisterStateByOffsets(stateMachine,
		19,
		(uintptr_t)ZombieModernDolphinRider::TransitionOnEnter,
		(uintptr_t)ZombieModernDolphinRider::TransitionOnLoop,
		(uintptr_t)ZombieModernDolphinRider::TransitionOnExit,
		"ZS_DolphinRider_Transition");
	RegisterStateByOffsets(stateMachine,
		20,
		(uintptr_t)ZombieModernDolphinRider::RideDolphinOnEnter,
		(uintptr_t)ZombieModernDolphinRider::RideDolphinOnLoop,
		(uintptr_t)ZombieModernDolphinRider::RideDolphinOnExit,
		"ZS_DolphinRider_RideDolphin");
	RegisterStateByOffsets(stateMachine,
		21,
		(uintptr_t)ZombieModernDolphinRider::RideDolphinToWalkOnEnter,
		(uintptr_t)ZombieModernDolphinRider::RideDolphinToWalkOnLoop,
		(uintptr_t)ZombieModernDolphinRider::RideDolphinToWalkOnExit,
		"ZS_DolphinRider_RideDolphinToWalk");
	LOGI("Reg state complete");
}
