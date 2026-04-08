#pragma once
#include "RealObject.h"
#include "PropertySheetBase.h"
#include "ZombieConditions.h"
#include "BoardEntity.h"
#include <jni.h>
#include "PopAnimRig.h"

class BoardEntity;
class PlantType;
class ZombieConditionEntry;
class DamageInfo;
enum DamageTypeFlags : int64_t;

class Projectile : public ModularRealObject
{
public:
	bool m_shouldAffectTarget;
	char pad2[12];
	Sexy::RtWeakPtr<RtObject> m_propertySheet;
	Sexy::RtWeakPtr<RtObject> m_instigator;
	int m_instigatorLevel;
	int m_instigatorPowerUpFlags;
	Sexy::RtWeakPtr<PlantType> m_instigatorType;
	pvztime_t m_timeStepOverride;
	Sexy::SexyVector3 m_previousPosition;
	Sexy::SexyVector3 m_velocity;
	Sexy::SexyVector3 m_acceleration;
	Sexy::SexyVector3 m_velocityScale;
	int m_boardRow;
	int m_previousBoard;
	int m_boardRowHeight;
	int m_boardRowFiredOrDeflectedFrom;
	char pad3[4];
	Sexy::RtWeakPtr<RtObject> m_renderImage;
	int m_renderOrder;
	float m_scale;
	float m_rotation;
	float m_baseRotation;
	float m_angularVelocity;
	char pad4[4];
	bool m_mirrored;
	bool m_collideLeftToRight;
	float m_damageAmount;
	float m_splashDamageAmount;
	float m_stunDurationAmount;
	int m_damageFlags;
	std::vector<ZombieConditionEntry> m_damageConditions;
	int m_collisionFlagsOverride;
	int m_propertyFlags;
	Sexy::RtWeakPtr<RtObject> m_attachedAnimRig;
	Sexy::SexyVector2 m_attachedAnimRigOffset;
	Sexy::RtWeakPtr<BoardEntity> m_target;
	EntityComponent_GroundEffect m_groundEffect;
	std::vector<SexyString> m_plantFamilies;
	float m_resistancePiercingOverride;
	int m_plantTier;
	char pad5[4];

	void SetTarget(Sexy::RtWeakPtr<BoardEntity>* targetPtr)
	{
		this->m_target.FromOther(targetPtr);
	}
	virtual bool CollideWithObject(GameObject* object) {};
	virtual void OnCollision() {};
	virtual void Function37() {};
	virtual void Function38() {};
	virtual void Function39() {};
	virtual void UpdateVelocity(float velocity) {};
	virtual bool DoImpact(BoardEntity* target) {};
	virtual void DoSplashImpact(BoardEntity* target) {};
	virtual void Function43() {};
	virtual void DebugDrawRects(Sexy::Graphics* g) {};
	virtual void Function45() {};
	virtual void HealTarget(BoardEntity* target) {};
	virtual void AffectTarget(BoardEntity* target) {};
	virtual void Function48() {};
	virtual void Function49() {};
	virtual void GetDamageInfo(DamageInfo* outDmgInfo) {};
	virtual void FindCollisionTargets(std::vector<BoardEntity*>& targets) {};
	virtual void InitializeAttachedAnimRig(PopAnimRig* animRig, float offsetX, float offsetY) {};
};

static_assert(sizeof(Projectile) == 376);
static_assert(offsetof(Projectile, m_shouldAffectTarget) == 80);
static_assert(offsetof(Projectile, m_propertySheet) == 96);
static_assert(offsetof(Projectile, m_instigator) == 104);
static_assert(offsetof(Projectile, m_target) == 304);
static_assert(offsetof(Projectile, m_previousPosition) == 132);
static_assert(offsetof(Projectile, m_renderImage) == 200);
static_assert(offsetof(Projectile, m_mirrored) == 232);
static_assert(offsetof(Projectile, m_damageFlags) == 248);
static_assert(offsetof(Projectile, m_propertyFlags) == 284);
static_assert(offsetof(Projectile, m_attachedAnimRig) == 288);
static_assert(offsetof(Projectile, m_plantFamilies) == 336);
static_assert(offsetof(Projectile, m_plantTier) == 364);

enum class CollisionTypeFlags
{
	none,
	ground_zombies,
	off_ground_zombies,
	dying_zombies = 4,
	all_zombies = 7,
	griditems = 8,
	low_plants = 32,
	normal_plants = 64,
	tall_plants = 128,
	ground = 256,
	instigator = 512,
	plants = 240,
	everything = 1023
};

class ProjectilePropertySheet : public PropertySheetBase
{
public:
	SexyString ClassName;
	float BaseDamage;
	float HealAmount;
	char pad1[16];
	float SplashDamage;
	float StunDuration;
	float SplashRadius;
	float SplashRadiusBaseDamageCutoff;
	bool ShakeBoardOnSplash;
	bool DiesOnImpact;
	bool HasShadow;
	bool ImpactShowsAtZombieFeet;
	bool RotateToMatchVelocity;
	bool ImpactSoundForce;
	bool FollowsGround;
	ZombieConditions OverrideStunCondition;
	std::vector<Sexy::ValueRange> InitialVelocity;
	std::vector<Sexy::ValueRange> InitialAcceleration;
	std::vector<Sexy::ValueRange> InitialVelocityScale;
	Sexy::ValueRange InitialHeight;
	Sexy::ValueRange InitialRotation;
	Sexy::ValueRange InitialAngularVelocity;
	Sexy::ValueRange InitialScale;
	SexyString AttachedPAM;
	SexyString AttachedPAMAnimRigClass;
	Sexy::SexyVector2 AttachedPAMOffset;
	std::vector<SexyString> AttachedPAMAnimationToPlay;
	SexyString ShadowImage;
	SexyString RenderImage;
	Sexy::Color RenderColor;
	Sexy::FRect CollisionRect;
	SexyString ImpactSoundEvent;
	float ImpactSoundThrottleTimer;
	SexyString ImpactPAM;
	std::vector<SexyString> ImpactPAMAnimationToPlay;
	std::vector<Sexy::ValueRange> ImpactOffset;
	SexyString SpawnPAM;
	std::vector<SexyString> SpawnPAMAnimationToPlay;
	std::vector<Sexy::ValueRange> SpawnPAMOffset;
	std::vector<ZombieConditionEntry> Conditions;
	Sexy::SexyVector2 AttachedPAMEffectOffset;
	std::vector<CollisionTypeFlags> CollisionFlags;
	std::vector<DamageTypeFlags> DamageFlags;
};

static_assert(sizeof(ProjectilePropertySheet) == 640);
static_assert(offsetof(ProjectilePropertySheet, ClassName) == 40);
static_assert(offsetof(ProjectilePropertySheet, BaseDamage) == 64);
static_assert(offsetof(ProjectilePropertySheet, SplashDamage) == 88);
static_assert(offsetof(ProjectilePropertySheet, OverrideStunCondition) == 112);
static_assert(offsetof(ProjectilePropertySheet, InitialHeight) == 192);
static_assert(offsetof(ProjectilePropertySheet, AttachedPAM) == 224);
static_assert(offsetof(ProjectilePropertySheet, SpawnPAM) == 488);
static_assert(offsetof(ProjectilePropertySheet, AttachedPAMEffectOffset) == 584);
static_assert(offsetof(ProjectilePropertySheet, CollisionFlags) == 592);
static_assert(offsetof(ProjectilePropertySheet, DamageFlags) == 616);