#pragma once
#include "BoardEntity.h"
#include "ObjectTypeDescriptor.h"

class CollectableType;

class Collectable : public BoardEntity
{
public:
	char m_pad4[8];
	Sexy::RtWeakPtr<CollectableType> m_type;
	int m_collectableFlags;
	pvztime_t m_creationTime;
	char m_pad5[176];
	pvztime_t m_stateStartTime;
	pvztime_t m_expirationStartTime;
	Sexy::RtWeakPtr<void> m_renderEffect;
	pvztime_t m_fadeOutOnCollectTime;
	float m_scale;
	Sexy::RtWeakPtr<RtObject> m_rewardSparkleEffect;
	Sexy::RtWeakPtr<RtObject> m_rewardRaysEffect;
	int m_motion;
	pvztime_t m_motionStartTime;
	Sexy::SexyVector3 m_newton_velocity;
	Sexy::SexyVector3 m_newton_acceleration;
	bool m_newton_stopOnHitGround;
	Sexy::SexyVector3 m_interp_initialPosition;
	Sexy::SexyVector3 m_interp_finalPosition;
	float m_interp_initialScale;
	float m_interp_finalScale;
	pvztime_t m_interp_duration;
	int m_interp_method;
	float m_drunk_motionAngle;
	float m_drunk_motionTurnAngle;
	float m_drunk_motionTurnAngleSpeed;
	float m_drunk_motionSpeed;
	bool m_drunk_motionPushToCenter;
};

static_assert(sizeof(Collectable) == 456);
static_assert(offsetof(Collectable, m_type) == 128);
static_assert(offsetof(Collectable, m_stateStartTime) == 320);
static_assert(offsetof(Collectable, m_drunk_motionPushToCenter) == 452);

class CollectableType : public ObjectTypeDescriptor
{
public:
	SexyString CollectableClass;
	Sexy::SexyVector2 Dimensions;
	pvztime_t ExpireDuration;
	pvztime_t FadeOutDuration;
	pvztime_t BlinkOutDuration;
	bool CollectOnExpire;
	SexyString SpawnSound;
	SexyString GroundSound;
	SexyString CollectSound;
	SexyString PopAnim;
	float Scale;
	float RewardRayScale;
	SexyString Animation;
	SexyString StandaloneImage;
	SexyString StandaloneGroup;
	Sexy::SexyVector3 RenderOffset;
	SexyString RewardDialogTitle;
	std::vector<SexyString> RewardDialogDescriptions;
	SexyString RewardDialogImage;
	bool RewardDoFancyCollect;
};

static_assert(sizeof(CollectableType) == 352);
static_assert(offsetof(CollectableType, CollectableClass) == 32);
static_assert(offsetof(CollectableType, CollectOnExpire) == 76);
static_assert(offsetof(CollectableType, Scale) == 176);
static_assert(offsetof(CollectableType, Animation) == 184);
static_assert(offsetof(CollectableType, RewardDialogTitle) == 272);