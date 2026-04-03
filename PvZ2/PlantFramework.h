#pragma once
#include <Sexy/RtObject.h>
#include <Sexy/RtWeakPtr.h>
#include "Reflection/CRefManualSymbolBuilder.h"
#include <Reflection/ReflectionBuilder.h>

namespace Sexy {
	class Graphics;
}

class Plant;
class PlantAnimRig;
class Projectile;
class PlantAction;

class PlantFramework : public Sexy::RtObject
{
public:
	Plant* m_plant;

	virtual void Function7() {};
	virtual void Function8() {};
	virtual void Function9() {};
	virtual void RenderAnimRigs(Sexy::Graphics* g) {};
	virtual void Function11() {};
	virtual void Function12() {};
	virtual void Function13() {};
	virtual void SetupAnimRigFiringCallbacks(PlantAnimRig* rig) {};
	virtual void PlayChompSound() {};
	virtual void Die() {};
	virtual Projectile* CreateProjectile() {};
	virtual float GetLaunchIntervalMultiplier() {};
	virtual std::vector<Sexy::RtWeakPtr<void>> FindAttackTargets(char unk1, int unk2) {};
	virtual void Function20() {};
	virtual void Function21() {};
	virtual void Function22() {};
	virtual void Function23() {};
	virtual void Function24() {};
	virtual void Function25() {};
	virtual void Function26() {};
	virtual Sexy::SexyVector3 GetProjectileOffset(PlantAction* action, float scale) {};
	virtual int GetPlantfoodPlayCount() {};
	virtual void Function29() {};
	virtual void Function30() {};
	virtual void Function31() {};
	virtual void Function32() {};
	virtual void Function33() {};
	virtual void Function34() {};
	virtual void Function35() {};
	virtual void Function36() {};
	virtual bool HasPlantFoodEnded() {};
	virtual void FirePlantFood() {};
	virtual bool IsInPlantFoodState() {};
	virtual void Function40() {};
	virtual void ActivatePlantFood() {};
	virtual void TriggerPlantFoodFlash() {};
	virtual void StartExhausted(float duration) {};
	virtual void FinishExhausted() {};
	virtual bool IsExhausted() {};
	virtual void Function46() {};
	virtual void PlayCharging() {};
	virtual void PlayCycling() {};
	virtual void Function49() {};
	virtual void Function50() {};
	virtual void PlayBeingHelped() {};
	virtual bool NotDeactivated() {}; // check if not being helped nor exhausted
	virtual void Function53() {};
	virtual void PlayHelpedOff() {}; // set state to READY
	virtual bool IsBeingHelped() {};
	virtual void Function56() {};
	virtual void Function57() {};
	virtual void Function58() {};
	virtual void PlayAttackBasedOnTier() {};
	virtual void Function60() {};
	virtual void Function61() {};
	virtual void Function62() {};
	virtual void Function63() {};
	virtual void Function64() {};
	virtual void Function65() {};
	virtual void Function66() {};
	virtual float GetSinkPercentage() {};
	virtual void Function68() {};
	virtual void Function69() {};
	virtual void Function70() {};
	virtual void Function71() {};
	virtual void Function72() {};
	virtual void Function73() {};
	virtual Sexy::Rect GetActionTriggerRectGridCoords(int actionIdx) {};
	virtual Sexy::Rect GetActionTriggerRect(int actionIdx) {};
	virtual void Function76() {};
	virtual void OnDamaged() {}; // TODO: I'm not sure the fields and return value for this
	virtual void Function78() {};
	virtual SexyString GetProjectileLaunchSound() {};
	virtual int GetHeight() {};
	virtual void AttemptKill() {};
	virtual void Function82() {};
	virtual bool IsInvincible() {};
	virtual void Function84() {};
	virtual void Function85() {};
	virtual void Function86() {}; // TODO: this one is for use_action frames
	virtual void Function87() {};
	virtual void Function88() {};
	virtual void Function89() {};
	virtual void Function90() {};
	virtual void Function91() {};
	virtual void Function92() {};
	virtual void Function93() {};
	virtual void Function94() {};
	virtual void Function95() {};
	virtual void Function96() {};
	virtual void Function97() {};
	virtual void PlantfoodStartedCallback() {}; // TODO: Check parameters for these 3
	virtual void PlantfoodLoopedCallback() {};
	virtual void PlantfoodEndedCallback() {};
	virtual void Function101() {};
	virtual void Function102() {};
	virtual void Function103() {};
	virtual void Function104() {};
	virtual void Function105() {};
	virtual void Function106() {};
	virtual void Function107() {};
	virtual void Function108() {};
	virtual void Function109() {};
	virtual void Function110() {};
	virtual bool CanPlantBasedOnCellType() {};
	virtual void Function112() {};
	virtual void Function113() {};
	virtual void Function114() {};
	virtual void Function115() {};
	virtual void Function116() {};
	virtual void Function117() {};
	virtual void Function118() {};
	virtual void SetCollisionRect() {}; // Idk the return value of this one
	virtual void Function120() {};
};

static_assert(sizeof(PlantFramework) == 16);
static_assert(offsetof(PlantFramework, m_plant) == 8);

class PlantTallnut : public PlantFramework
{
public:
	Sexy::RtWeakPtr<void*> m_shield;

	RT_CLASS_GET_CLASS_WRAPPER(0x143A8DC);
};