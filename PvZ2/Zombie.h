#pragma once
#include "BoardEntity.h"
#include "PlantFramework.h"
#include "PropertySheetBase.h"
#include "EntityConditionTracker.h"
#include "ZombieConditions.h"
#include "ObjectTypeDescriptor.h"
#include "ZombieAnimRig.h"
#include "ArmorPropertySheet.h"


class ZombieConditionImmunity
{
public:
	float Percent;
	ZombieConditions Condition;
};

static_assert(sizeof(ZombieConditionImmunity) == 8);
static_assert(offsetof(ZombieConditionImmunity, Condition) == 4);

enum class HelmType
{
	none,
	cone,
	bucket,
	sarcophagus,
	camelsign,
	poncho,
	metalplate,
	cowboyhat,
	gravestone,
	crown,
	surfboard,
	iceblock,
	shovel,
	brick
};

enum class ZombieSize
{
	normal,
	large,
	chicken,
	imp
};

class ZombiePropertySheet : public PropertySheetBase
{
public:
	SexyString TypeName;
	float Speed = 0.16f;
	float SpeedVariance = 0.1f;
	float Hitpoints = 270.0f;
	float TimeToKillInSeconds;
	float EatDPS = 100.0f;
	float PowerUpDamageModifier = 1.0f;
	float FireDamageMultiplier = 1.0f;
	float FoeEatDamageReflectPct;
	float AlmanacScale;
	Sexy::SexyVector2 AlmanacOffset;
	int Ammo;
	ZombieSize Size;
	bool CanSpawnPlantFood = true;
	bool CanTriggerZombieWin;
	bool CanSurrender;
	bool ExplodesWhenMowed;
	bool NormalDeathWhenMowed = true;
	bool IsValidPinchTarget;
	bool NormalDeathOnExplosion;
	bool ChillInsteadOfFreeze;
	bool CanBePlantTossedStrong = true;
	bool CanBePlantTossedWeak = true;
	bool SkipHeadDropState;
	float ArmDropFraction;
	HelmType Helm;
	float HelmHitpoints;
	int HelmDamageLayerIndices = 3;
	bool HelmAbsorbsOverflowDamage;
	bool MetallicBodyParts;
	int GridHeight = 1;
	Sexy::Rect HitRect = Sexy::Rect(0, 0, 42, 125);
	Sexy::Rect AttackRect = Sexy::Rect(10, 10, 50, 125);
	int Cost = 150;
	int WavePointCost = 100;
	int Weight = 4000;
	float ScoreOverride;
	Sexy::SexyVector2 StreetSize = Sexy::SexyVector2(2.0f, 2.0f);
	Sexy::SexyVector2 StreetCriticalSize = Sexy::SexyVector2(1.0f, 1.0f);
	Sexy::SexyVector2 StreetOffset = Sexy::SexyVector2(0.0f, -1.0f);
	float ArtScale = 1.0f;
	Sexy::SexyVector2 ArtCenter = Sexy::SexyVector2(135.0f, 160.0f);
	Sexy::SexyVector3 ShadowOffset = Sexy::SexyVector3(0.0f, 0.0f, 1.2f);
	Sexy::SexyVector2 ShadowScaling = Sexy::SexyVector2(1.0f, 1.0f);
	SexyString GroundTrackName;
	Sexy::SexyVector2 ArtCenterBurnEffectOffset;
	Sexy::Point GridExtents;
	Sexy::SexyVector2 HypnoshroomEffectOffset = Sexy::SexyVector2(0.0f, 63.0f);
	std::vector<BoardEntityStat> ZombieStats;
	std::vector<Sexy::RtWeakPtr<ArmorPropertySheet>> ZombieArmorProps;
	std::vector<ZombieConditionImmunity> ConditionImmunities;
	bool IgnoreWaterLine;
	float MaxTideLoweredPercent;
	SexyString SoundOnDeath;
	SexyString SoundOnTakeDamage; 
	SexyString SoundOnWalk;
	SexyString SoundOnEat;
	SexyString SoundOnIdle;
	SexyString SoundOnAsh;
	SexyString SoundOnElectrocute;
	bool CanBeFlicked;
	bool CanBeFlickedOff;
	bool FlickIsLaneRestricted;
	bool IsSpawnedFlying;
	float ShrunkenScale;
	float unk;
};

static_assert(sizeof(ZombiePropertySheet) == 568);
static_assert(offsetof(ZombiePropertySheet, Speed) == 64);
static_assert(offsetof(ZombiePropertySheet, AlmanacScale) == 96);
static_assert(offsetof(ZombiePropertySheet, HypnoshroomEffectOffset) == 296);
static_assert(offsetof(ZombiePropertySheet, CanBeFlicked) == 552);

enum ZombieFlagType
{
	noflag = 0,
	flag_normal = 1,
	flag_veteran = 2
};

class ZombieType : public ObjectTypeDescriptor
{
public:
	SexyString ZombieClass;
	SexyString HomeWorld;
	SexyString DisplayTypeName;
	SexyString AlmanacBackdropName;
	SexyString PopAnim;
	SexyString AnimRigClass;
	std::vector<SexyString> ResourceGroups;
	std::vector<SexyString> AudioGroups;
	Sexy::RtWeakPtr<ZombiePropertySheet> Properties;
	bool Placeable = true;
	bool HastyOnStart = true;
	bool IsBasicZombie;
	bool IsFemaleZombie;
	bool HideFromAlmanac;
	ZombieFlagType FlagType = ZombieFlagType::noflag;
};

static_assert(sizeof(ZombieType) == 248);
static_assert(offsetof(ZombieType, ZombieClass) == 32);
static_assert(offsetof(ZombieType, HomeWorld) == 56);
static_assert(offsetof(ZombieType, PopAnim) == 128);
static_assert(offsetof(ZombieType, AnimRigClass) == 152);
static_assert(offsetof(ZombieType, Placeable) == 232);

class Zombie : public BoardEntity
{
public:

	pvztime_t m_creationTime;
	int m_helm;
	float m_helmHitpoints;
	char pad_0084[28];
	Sexy::RtWeakPtr<RtObject> m_type;
	Sexy::RtWeakPtr<RtObject> m_animRig;
	char pad_00B0[176];
	double m_elapsedTimeInState;
	float m_stateMachineTimeScale;
	int m_playingAnim;
	int m_zombieFlags;
	int m_facing;
	float m_hitpoints;
	float m_maxHitpoints;
	bool m_isTimeToKillActive;
	char pad_align1[3];
	float m_timeToKill;
	float m_speedScale;
	float m_dpsScale;
	float m_damageScale;
	float m_shrunkenDamageScale;
	float m_walkSpeed;
	float m_translationMultiplier;
	float m_maxHelmHitpoints;
	int m_spawnedFromWave;
	float m_nextChewSoundTime;
	float m_mowedStartTime;
	float m_flickedStartTime;
	Sexy::SexyVector3 m_flickOffStartPosition;
	Sexy::SexyVector3 m_flickOffTarget;
	float m_flickOffTimeToTarget;
	float m_flickOffFinalRotation;
	float m_rotation;
	float m_scale;
	int m_zombieRenderLayerOffset;
	Sexy::RtWeakPtr<Plant> m_ownerPlant;
	Sexy::RtWeakPtr<Sexy::RtObject> m_forcedTarget;
	EntityComponent_GroundEffect m_groundEffect;
	Sexy::SexyVector2 m_stormTargetLocation;
	int m_stormType;
	char pad_align2[4];
	std::vector<Sexy::RtWeakPtr<Armor>> m_armor;
	ZombieConditionTracker m_conditionTracker;
	char pad_HiddenAndAttached[296];
	std::vector<int> m_familyResistances;
	std::vector<Sexy::RtWeakPtr<Sexy::RtObject>> m_targetHistory;
	int m_loot;
	Sexy::Color m_highlightColor;
	bool m_isBeingDestroyed;
	bool m_wasMowedDown;
	char pad_tail[2];
	Sexy::RtWeakPtr<ZombiePropertySheet> m_propertySheet;

	virtual Sexy::FPoint GetShadowScaling() {}
	virtual void Function51() {}
	virtual void Function52() {}
	virtual int UpdateHelmDegrades() {}
	virtual int GetTotalHitpoints() {}
	virtual void Function55() {}
	virtual void Function56() {}
	virtual void SpawnFromGround() {}
	virtual void Function58() {}
	virtual void RiseFromPod() {}
	virtual void SetFlagType() {} // for flag zombies
	virtual void EnterIdle() {}
	virtual void EnterWalk() {}
	virtual void EnterGrabbed() {}
	virtual void ActivateJam() {}
	virtual void DeactivateJam() {}
	virtual void Function66() {}
	virtual void Function67() {}
	virtual int ApplyStun(float duration, PlantFramework* instigator) {}
	virtual int ApplyStunWithoutInstigator(float duration) {}
	virtual bool IsStunned() {}
	virtual void Function71() {}
	virtual void Function72() {}
	virtual void Function73() {}
	virtual void Die(int unk) {}
	virtual void ThreatAlert() {} // runs when zombie is close to house
	virtual void Function76() {}
	virtual void Function77() {}
	virtual void Function78() {}
	virtual void Function79() {}
	virtual void Function80() {}
	virtual bool HasArmDrop() {}
	virtual void Function82() {}
	virtual void Function83() {}
	virtual void Function84() {}
	virtual void Function85() {}
	virtual void Function86() {}
	virtual void Function87(int* g) {}
	virtual void Function88() {}
	virtual void Function89() {}
	virtual void Function90() {}
	virtual void Function91() {}
	virtual void Function92() {}
	virtual void Function93() {}
	virtual void Function94() {}
	virtual void Function95() {}
	virtual void Function96() {}
	virtual void Function97() {}
	virtual void Function98() {}
	virtual void Function99() {}
	virtual void Function100() {}
	virtual void Function101() {}
	virtual void Function102() {}
	virtual void Function103() {}
	virtual void Function104() {}
	virtual void Function105() {}
	virtual void Function106() {}
	virtual int GetTideEffect() {}
	virtual void* FindAttackTarget() {}
	virtual void Function109() {}
	virtual void Function110() {}
	virtual void Function111() {}
	virtual void Function112() {}
	virtual void Function113() {}
	virtual void Function114() {}
	virtual void Function115() {}
	virtual void Function116() {}
	virtual void Function117() {}
	virtual float GetWalkSpeed() {}
	virtual void Function119() {}
	virtual void IdleOnEnter(int unk) {} // weird argument
	virtual void IdleOnLoop() {}
	virtual void IdleOnExit() {}
	virtual void WalkOnEnter() {}
	virtual void WalkOnLoop() {}
	virtual void WalkOnExit() {}
	virtual void EatOnEnter() {}
	virtual void EatOnLoop() {}
	virtual void EatOnExit() {}
	virtual void BleedingOutOnEnter() {}
	virtual void BleedingOutOnLoop() {}
	virtual void BleedingOutOnExit() {}
	virtual void DieOnEnter() {}
	virtual void DieOnLoop() {}
	virtual void DieOnExit() {}
	virtual void ElectrocuteOnEnter() {}
	virtual void ElectrocuteOnLoop() {}
	virtual void ElectrocuteOnExit() {}
	virtual void AshOnEnter() {}
	virtual void AshOnLoop() {}
	virtual void AshOnExit() {}
	virtual void MowedDownOnEnter() {}
	virtual void MowedDownOnLoop() {}
	virtual void MowedDownOnExit() {}
	virtual void FlickedOffOnEnter() {}
	virtual void FlickedOffOnLoop() {}
	virtual void FlickedOffOnExit() {}
	virtual void WinningOnEnter() {}
	virtual void WinningOnLoop() {}
	virtual void WinningOnExit() {}
	virtual void PlantifyOnEnter() {}
	virtual void PlantifyOnLoop() {}
	virtual void PlantifyOnExit() {}
	virtual void GrabbedOnEnter() {}
	virtual void GrabbedOnLoop() {}
	virtual void GrabbedOnExit() {}
	virtual void RiseFromGroundOnEnter() {}
	virtual void RiseFromGroundOnLoop() {}
	virtual void RiseFromGroundOnExit() {}
	virtual void RiseFromStormOnEnter() {}
	virtual void RiseFromStormOnLoop() {}
	virtual void RiseFromStormOnExit() {}
	virtual void StormEntranceOnEnter() {}
	virtual void StormEntranceOnLoop() {}
	virtual void StormEntranceOnExit() {}
	virtual void RiseFromPodOnEnter() {}
	virtual void RiseFromPodOnLoop() {}
	virtual void RiseFromPodOnExit() {}
	virtual void Function168() {}
	virtual void Function169() {}
	virtual void OnActionCommand(SexyString* currentAnim, int unk1, SexyString* actionName, int unk2) {}
	virtual void Function171() {}
	virtual void Electrocute() {}
	virtual void Ash() {}
	virtual void Function174() {}
	virtual void Function175() {}
	virtual void Function176() {}
	virtual void Function177() {}
	virtual void Function178() {}
	virtual void Function179() {}
	virtual void KillWithFatalDamage() {}
	virtual void Function181() {}
	virtual void Function182() {}
	virtual void Function183() {}
	virtual void Function184() {}
	virtual void Function185() {}
	virtual void Function186() {}
	virtual void Function187() {}
	virtual void Function188() {}
	virtual SexyString GetElectrocuteEffectName() {}
	virtual SexyString GetAshEffectName() {}
	virtual void Function191() {}
	virtual void Function192() {}
	virtual void Function193() {}
	virtual float GetArmDropFraction() {}
	virtual void Function195() {}
	virtual int GetAttackDamage() {}
	virtual void OnDeath() {}
	virtual Sexy::SexyVector3 Function198() {} // get some offset of the zombie's position
	virtual void Function199() {}
	virtual void Function200() {}
	virtual void PlaySoundOnWalk() {}
	virtual void PlaySoundOnIdle() {}
	virtual void PlaySoundOnEat() {}
};
static_assert(offsetof(Zombie, m_creationTime) == 120);
static_assert(offsetof(Zombie, m_walkSpeed) == 408);
static_assert(offsetof(Zombie, m_helm) == 124);
static_assert(offsetof(Zombie, m_helmHitpoints) == 128);
static_assert(offsetof(Zombie, m_hitpoints) == 376);
static_assert(offsetof(Zombie, m_type) == 160);
static_assert(offsetof(Zombie, m_animRig) == 168);
static_assert(offsetof(Zombie, m_scale) == 472);
static_assert(offsetof(Zombie, m_elapsedTimeInState) == 352);
static_assert(offsetof(Zombie, m_propertySheet) == 1088);
static_assert(offsetof(Zombie, m_dpsScale) == 396);
static_assert(offsetof(Zombie, m_flickOffTarget) == 448);
static_assert(offsetof(Zombie, m_conditionTracker) == 560);
static_assert(offsetof(Zombie, m_playingAnim) == 0x16C);
static_assert(offsetof(Zombie, m_maxHelmHitpoints) == 0x1A0);

