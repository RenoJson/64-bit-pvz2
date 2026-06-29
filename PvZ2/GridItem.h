#pragma once
#include "BoardEntity.h"
#include "PropertySheetBase.h"
#include "StringRestrictionSet.h"
#include "ObjectTypeDescriptor.h"
#include <Reflection/ReflectionBuilder.h>
#include "GridItemCondition.h"

class GridItemType;
class PopAnimRig;
class GridItemPropertySheet;

class GridItem : public BoardEntity
{
public:
	RT_CLASS_GET_CLASS_WRAPPER(0x8051EC);
	GridItemConditionTracker m_conditionTracker;
	Sexy::RtWeakPtr<GridItemType> m_type;
	float m_health;
	float m_healthMax;
	Sexy::Point m_gridLocation;
	Sexy::RtWeakPtr<GridItemPropertySheet> m_propertySheet;
};

static_assert(sizeof(GridItem) == 304);
static_assert(offsetof(GridItem, m_type) == 272);
static_assert(offsetof(GridItem, m_gridLocation) == 288);

class GridItemVase : public GridItem
{
public:
	RT_CLASS_GET_CLASS_WRAPPER(0xA2FC08);
	Sexy::RtWeakPtr<RtObject> m_animRig;
	Sexy::RtWeakPtr<RtObject> m_contentsType;
	int m_flags;
	float m_dropStartTime;
	float m_dropEndTime;
	bool m_allowPreGameplayInteraction;
	char pad[32];
};
static_assert(sizeof(GridItemVase) == 368);
static_assert(offsetof(GridItemVase, m_animRig) == 304);
static_assert(offsetof(GridItemVase, m_contentsType) == 312);
static_assert(offsetof(GridItemVase, m_flags) == 320);
static_assert(offsetof(GridItemVase, m_dropStartTime) == 324);
static_assert(offsetof(GridItemVase, m_dropEndTime) == 328);
static_assert(offsetof(GridItemVase, m_allowPreGameplayInteraction) == 332);

class GridItemAnimation : public GridItem
{
public:


	bool m_hasLinkedAnimRig;
	char pad7[4];
	Sexy::RtWeakPtr<PopAnimRig> m_animRig;

	virtual void Function63() {};
	virtual void Function64() {};
	virtual Sexy::SexyVector2 GetAnimPropPamOffset() {};
	virtual Sexy::SexyVector2 GetAnimPropPamScale() {};
};

static_assert(sizeof(GridItemAnimation) == 320);
static_assert(offsetof(GridItemAnimation, m_hasLinkedAnimRig) == 304);
static_assert(offsetof(GridItemAnimation, m_animRig) == 312);

class GridItemLilyPad : public GridItemAnimation
{
public:
	bool m_isDuplicate;
	char pad7[7];
	std::vector<RtWeakPtr<GridItem>> m_PFSpawnedPads;
	int m_level;
};




class GridItemGravestone : public GridItemAnimation
{
public:
	int m_state;
	int m_currDamageState;
	EntityComponent_GroundEffect m_groundEffect;
	float m_stateTime;
	float m_stateDuration;
	float m_eatenProgress;
	int m_damageStates;
};

static_assert(sizeof(GridItemGravestone) == 368); 
static_assert(offsetof(GridItemGravestone, m_state) == 320);
static_assert(offsetof(GridItemGravestone, m_currDamageState) == 324);
static_assert(offsetof(GridItemGravestone, m_groundEffect) == 328);
static_assert(offsetof(GridItemGravestone, m_stateTime) == 352);
static_assert(offsetof(GridItemGravestone, m_stateDuration) == 356);
static_assert(offsetof(GridItemGravestone, m_eatenProgress) == 360);
static_assert(offsetof(GridItemGravestone, m_damageStates) == 364);

class GridItemBreakableTarget : public GridItemAnimation
{
public:
	char pad[16];
	float m_lastTookDamage;
	bool m_playingTakeDamageAnim;
	bool m_playingDeathAnim;
	bool m_hasPlayFinalBreakEffect;
	bool m_isControlled;
	int m_damagePhase;
};
class GridItemSpeaker : public GridItemBreakableTarget
{
public:
	RT_CLASS_GET_CLASS_WRAPPER(0xD9944C);
	int m_speakerState;
	int64_t m_audioTriggeredNextSpeakerState;
	EntityComponent_GroundEffect m_groundEffect;
	float m_risingTime;
};
class GridItemSurfboard : public GridItemGravestone
{
public:

};

class GridItemPropertySheet : public PropertySheetBase
{
public:
	float Hitpoints = 1000.0f;
	float HitRectOffsetX;
	float HitRectOffsetWidth;
	BoardEntityHeight Height = BoardEntityHeight::normal;
	bool CanBeMowed;
	PlantingRestrictionSet PlantingRestrictions;
	GridItemRestrictionSet GridItemsOverride;
};

static_assert(sizeof(GridItemPropertySheet) == 152);
static_assert(offsetof(GridItemPropertySheet, Hitpoints) == 40);
static_assert(offsetof(GridItemPropertySheet, CanBeMowed) == 56);
static_assert(offsetof(GridItemPropertySheet, PlantingRestrictions) == 64);
static_assert(offsetof(GridItemPropertySheet, GridItemsOverride) == 112);

class GridItemType : public ObjectTypeDescriptor
{
public:
	SexyString GridItemClass;
	std::vector<SexyString> ResourceGroups;
	std::vector<SexyString> AudioGroups;
	Sexy::RtWeakPtr<GridItemPropertySheet> Properties;
	char m_pad[8];
};

static_assert(sizeof(GridItemType) == 120);
static_assert(offsetof(GridItemType, GridItemClass) == 32);

class GridItemAnimationProps : public GridItemPropertySheet
{
public:

	SexyString PopAnim;
	SexyString PopAnimRigClass;
	Sexy::SexyVector2 PopAnimRenderOffset;
	Sexy::SexyVector2 PopAnimRenderScale;
	SexyString LinkedPopAnim;
	Sexy::SexyVector3 LinkedOffset;
	float LinkedLayerOffset;
};

static_assert(sizeof(GridItemAnimationProps) == 256);
static_assert(offsetof(GridItemAnimationProps, PopAnim) == 152);
static_assert(offsetof(GridItemAnimationProps, LinkedOffset) == 240);

class GridItemGravestonePropertySheet : public GridItemAnimationProps
{
public:
	int DamageStateCount;
	SexyString BreakEffect;
	SexyString BreakEffectSound;
	Sexy::SexyVector2 ArtCenter;
	bool CanBeEatenByGravebuster;
	float GraveBusterEatTimeOveride;
	Sexy::Point GridExtents;
	bool IsAnimated;
	char pad_0x151[4];

};

static_assert(sizeof(GridItemGravestonePropertySheet) == 344); 
static_assert(offsetof(GridItemGravestonePropertySheet, DamageStateCount) == 256);
static_assert(offsetof(GridItemGravestonePropertySheet, BreakEffect) == 264);
static_assert(offsetof(GridItemGravestonePropertySheet, BreakEffectSound) == 288);
static_assert(offsetof(GridItemGravestonePropertySheet, ArtCenter) == 312);
static_assert(offsetof(GridItemGravestonePropertySheet, CanBeEatenByGravebuster) == 320);
static_assert(offsetof(GridItemGravestonePropertySheet, GraveBusterEatTimeOveride) == 324);
static_assert(offsetof(GridItemGravestonePropertySheet, GridExtents) == 328);
static_assert(offsetof(GridItemGravestonePropertySheet, IsAnimated) == 336);

class GridItemSurfboardProps : public GridItemGravestonePropertySheet
{
public:
	RT_CLASS_GET_CLASS_WRAPPER(0xAD46F0);
	int CanBeEatenByGravebusterAtLevel;
};
