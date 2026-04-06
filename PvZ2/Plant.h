#pragma once
#include "BoardEntity.h"
#include "PropertySheetBase.h"
#include "Projectile.h"
#include "ObjectTypeDescriptor.h"
#include <Reflection/ReflectionBuilder.h>

class PlantFramework;
class PlantAnimRigPropertySheet;
class PlantAnimRig;


class PowerNode
{
public:
	int64_t Type; // PowerType
	int64_t Context; // PowerContext
	SexyString Props;
};

static_assert(sizeof(PowerNode) == 40);
static_assert(offsetof(PowerNode, Type) == 0);
static_assert(offsetof(PowerNode, Context) == 8);
static_assert(offsetof(PowerNode, Props) == 16);

class PowerSet
{
public:
	std::vector<PowerNode> List;
};

static_assert(sizeof(PowerSet) == 24);
static_assert(offsetof(PowerSet, List) == 0);

enum class PlantActionType
{
	projectile,
	explode,
	sun,
	special,
	none
};

enum class SplashDamageType
{
	none,
	radius,
	large_radius,
	row,
	column,
	column_gridwide,
	square,
	enemy
};

enum class PlantActionTriggerType
{
	none,
	instant,
	column,
	row,
	rect,
	rect_grid,
	manual,
	board,
	allfront
};

enum class PlantGridPlantingType
{
	ground,
	water,
	any
};

enum class MultiPlantGridLayer
{
	power,
	armor,
	main,
	ground
};

class PlantAction
{
public:
	PlantActionType Type;
	Sexy::RtWeakPtr<Projectile> Projectile;
	SplashDamageType ExplodeType;
	float ExplodeRadius;
	PlantActionTriggerType TriggerType;
	bool DieAfterExplode;
	bool Burns;
	bool SecondaryAction;
	float InitialMinCooldownTime;
	float CooldownTimeMin;
	float CooldownTimeMax;
	int Damage;
	int SplashDamage;
	float XVelocityOverride;
	int PushbackAmount;
	float ChillDuration;
	float FreezeDuration;
	float StunDuration;
	SexyString ProjectileLaunchSound;
	Sexy::Rect RectTriggerRange;
	Sexy::SexyVector2 SpawnOffset;
};

static_assert(sizeof(PlantAction) == 120);
static_assert(offsetof(PlantAction, Type) == 0);
static_assert(offsetof(PlantAction, Projectile) == 4);
static_assert(offsetof(PlantAction, TriggerType) == 20);
static_assert(offsetof(PlantAction, ProjectileLaunchSound) == 72);

class PlantBoost
{
public:
	Sexy::RtWeakPtr<void> PlantBoostProps; // PlantBoostPropertySheet
	float Value;
	float CooldownDurationSeconds;
};

static_assert(sizeof(PlantBoost) == 16);
static_assert(offsetof(PlantBoost, PlantBoostProps) == 0);
static_assert(offsetof(PlantBoost, CooldownDurationSeconds) == 12);

class PlantPropertySheet : public PropertySheetBase
{
public:
	SexyString TypeName;
	int Cost;
	int BeghouledCost;
	float Hitpoints;
	float PacketCooldown;
	float StartingCooldown;
	float PlantFoodDurationSeconds;
	int PlantFoodPlayCount;
	float PlantFoodShineTime = 0.75f;
	SexyString PlantFoodActivationSound;
	SexyString HelpedActivationSound;
	PlantGridPlantingType PlantGridType;
	MultiPlantGridLayer MultiPlantLayer = MultiPlantGridLayer::ground;
	std::vector<PlantAction> Actions;
	PowerSet Powers;
	int GridWidth;
	int GridHeight;
	int GridItemDamage;
	float PlantingOffsetY;
	Sexy::SexyVector2 CursorPreviewOffset;
	Sexy::SexyVector2 ArtCenter;
	Sexy::SexyVector2 ZenGardenOffset;
	float ZenGardenSmallScale = 0.6f;
	float ZenGardenLargeScale = 1.2f;
	Sexy::Rect HitRectAdjust;
	BoardEntityHeight Height;
	int DangerRoomWeight = 100;
	bool IsZenGardenTreasurePlant;
	bool IsZenGardenWaterPlant;
	int ZenGardenSproutWeight;
	int ZenGardenBoostCost;
	int ZenGardenDurationOverrideSeconds = -1;
	std::vector<PlantBoost> Boosts;
	Sexy::Rect RestrictPlantingToGridArea;
	std::vector<BoardEntityStat> PlantStats;
	std::vector<SexyString> ValidStages;
	std::vector<SexyString> SuggestionAlts;
	bool CanReceiveFirstAid;
	bool IsConsumable;
	bool IsInstant;
	bool CanLiveOnWaves;
	bool CanPlantAgainToUpgrade;
	std::map<SexyString, int> IdleAnimationWeights;
	char pad1[4];
	Sexy::RtWeakPtr<PlantAnimRigPropertySheet> AnimRigProps;
	bool HidePlantingEffects;
	float MasteryBoostChance;
	float MasteryResistancePiercing;
};

static_assert(sizeof(PlantPropertySheet) == 456);
static_assert(offsetof(PlantPropertySheet, TypeName) == 40);
static_assert(offsetof(PlantPropertySheet, PlantGridType) == 144);
static_assert(offsetof(PlantPropertySheet, MultiPlantLayer) == 148);
static_assert(offsetof(PlantPropertySheet, Actions) == 152);
static_assert(offsetof(PlantPropertySheet, GridWidth) == 200);
static_assert(offsetof(PlantPropertySheet, Height) == 264);
static_assert(offsetof(PlantPropertySheet, ZenGardenSproutWeight) == 276);
static_assert(offsetof(PlantPropertySheet, RestrictPlantingToGridArea) == 312);
static_assert(offsetof(PlantPropertySheet, PlantStats) == 328);
static_assert(offsetof(PlantPropertySheet, ValidStages) == 352);
static_assert(offsetof(PlantPropertySheet, CanPlantAgainToUpgrade) == 404);
static_assert(offsetof(PlantPropertySheet, AnimRigProps) == 436);


class Plant : public BoardEntity
{
public:
	pvztime_t m_createdTime;
	char pad_007C[12];
	Sexy::ValueRange m_initialLaunchInterval;
	Sexy::ValueRange m_launchInterval;
	float m_baseScale;
	int m_state;
	int m_plantFlags;
	float m_PlantHealth;
	float m_overrideMinimumMaxHealth;
	float m_speedModifier;
	int m_lastDamageType;
	char pad_00B4[4];
	int m_row;
	int m_column;
	bool m_isOnBoard;
	bool m_isShooter;
	bool m_mirror;
	bool m_isDead;
	pvztime_t m_disappearTime;
	pvztime_t m_doSpecialTime;
	pvztime_t m_stateEndTime;
	pvztime_t m_launchTime;
	pvztime_t m_plantFoodEndTime;
	pvztime_t m_timeTillNextDamageFlash;
	float m_fadeInDuration;
	pvztime_t m_fadeInEndTime;
	float m_sproutScalingDuration;
	pvztime_t m_sproutScalingEndTime;
	char pad_00EC[20];
	int m_damageStates;
	int m_currentDamageState;
	int m_currentPlantActionIdx;
	char pad_align_ptr[4];
	Sexy::RtWeakPtr<Sexy::RtObject> m_type;
	Sexy::RtWeakPtr<Sexy::RtObject> m_animRig;
	Sexy::RtWeakPtr<Sexy::RtObject> m_plantFoodShine;
	bool m_isInPlantFoodState;
	bool m_protectedFromShovel;
	bool m_isDuplicate; 
	char pad_align_299[1];
	int m_level;
	int m_masteryLevel;
	int m_powerUpFlags; 
	Sexy::RtWeakPtr<PlantPropertySheet> m_propertySheet;
	MultiPlantGridLayer m_multiPlantGridLayer;
	MultiPlantGridLayer m_multiPlantDrawLayer;
	bool m_relocating;
	char pad_align_329[3];
	float m_relocateFromX;
	float m_relocateFromY;
	float m_relocateToX;
	float m_relocateToY;
	float m_relocateApexHeight;
	Sexy::SexyVector2 m_relocationOffsetFromDestination;
	pvztime_t m_relocateStartTime;
	pvztime_t m_relocateEndTime;
	int m_relocationType;
	char pad_0174[4];
	char m_groundEffect[24];
	char m_conditionTracker[56];
	char pad_to_attached[88];
	char m_attachedBoardEntities[32]; 
	char m_attachedEffects[32];
};

static_assert(sizeof(Plant) == 608);
static_assert(offsetof(Plant, m_initialLaunchInterval) == 136);
static_assert(offsetof(Plant, m_plantFlags) == 160);
static_assert(offsetof(Plant, m_PlantHealth) == 164);
static_assert(offsetof(Plant, m_overrideMinimumMaxHealth) == 168);
static_assert(offsetof(Plant, m_column) == 188);
static_assert(offsetof(Plant, m_mirror) == 194);
static_assert(offsetof(Plant, m_damageStates) == 256);
static_assert(offsetof(Plant, m_currentPlantActionIdx) == 264);
static_assert(offsetof(Plant, m_type) == 272);
static_assert(offsetof(Plant, m_animRig) == 280);
static_assert(offsetof(Plant, m_isInPlantFoodState) == 296);
static_assert(offsetof(Plant, m_isDuplicate) == 298);
static_assert(offsetof(Plant, m_level) == 300);
static_assert(offsetof(Plant, m_multiPlantGridLayer) == 320);
static_assert(offsetof(Plant, m_relocating) == 328);
static_assert(offsetof(Plant, m_relocateFromX) == 332);
static_assert(offsetof(Plant, m_relocationOffsetFromDestination) == 352);
static_assert(offsetof(Plant, m_relocateStartTime) == 360);
static_assert(offsetof(Plant, m_createdTime) == 120);
static_assert(offsetof(Plant, m_initialLaunchInterval) == 136);
static_assert(offsetof(Plant, m_plantFlags) == 160);
static_assert(offsetof(Plant, m_column) == 188);
static_assert(offsetof(Plant, m_propertySheet) == 312);
static_assert(offsetof(Plant, m_attachedBoardEntities) == 544);

class PlantsSortedByLayer
{
public:
	std::vector<Sexy::RtWeakPtr<Plant>> m_plants;
};

static_assert(sizeof(PlantsSortedByLayer) == 24);
static_assert(offsetof(PlantsSortedByLayer, m_plants) == 0);

class PlantGroup : public BoardEntity
{
public:
	static Sexy::RtClass* StaticGetType() {
		typedef Sexy::RtClass* (*getType)(); 
		getType pGetType = (getType)getActualOffset(0x133566C);
		return pGetType();
	};
	char pad[4];
	int m_gridX;
	int m_gridY;
	bool m_isOnBoard;
	bool m_groupPlantfoodRender;
	PlantsSortedByLayer m_plants;
};

static_assert(sizeof(PlantGroup) == 160);
static_assert(offsetof(PlantGroup, m_gridX) == 124);
static_assert(offsetof(PlantGroup, m_gridY) == 128);
static_assert(offsetof(PlantGroup, m_plants) == 136);

enum PlantMasteryFocusType
{
	health = 0,
	piercing = 1
};

class PlantType : public ObjectTypeDescriptor
{
public:
	SexyString PlantFramework;
	SexyString AnimRigClass;
	SexyString PopAnim;
	Sexy::RtWeakPtr<PlantPropertySheet> Properties;
	bool Enabled = true;
	bool Premium;
	std::vector<SexyString> Families;
	PlantMasteryFocusType MasteryType;
	SexyString HomeWorld;
	SexyString AlmanacBackdropName;
	float SeedChooserVerticalOffset;
	float SeedChooserBigVerticalOffset;
	float SeedChooserBigHorizontalOffset;
	float SeedChooserBigScale;
	char m_pad[4];
	float BundleVerticalOffset;
	bool HideInPlantViewers;
	bool DenyPlantfoodCooldownReset;
	bool CannotBeImitate;
	float SeasonsVerticalOffset;
	std::vector<SexyString> PlantResourceGroups;
	std::vector<SexyString> PlantPreviewResourceGroups;
	int IntegerID;

	static Reflection::CRefManualSymbolBuilder::BuildSymbolsFunc oPlantTypeBuildSymbols;
	static Reflection::CRefManualSymbolBuilder::ConstructFunc oPlantTypeConstruct;

	static void* construct(PlantType* self)
	{
		oPlantTypeConstruct(self);

		self->IntegerID = 0;
		return self;
	}

	static void buildSymbols(Reflection::CRefManualSymbolBuilder* builder, Reflection::RClass* rclass)
	{
		oPlantTypeBuildSymbols(builder, rclass);
		RT_CLASS_REGISTER_STANDARD_PROPERTY(PlantType, IntegerID);
	}
};

//static_assert(sizeof(PlantType) == 280);
static_assert(offsetof(PlantType, PlantFramework) == 32);
static_assert(offsetof(PlantType, AnimRigClass) == 56);
static_assert(offsetof(PlantType, Properties) == 104);
static_assert(offsetof(PlantType, Enabled) == 112);
static_assert(offsetof(PlantType, Families) == 120);
static_assert(offsetof(PlantType, BundleVerticalOffset) == 220);
static_assert(offsetof(PlantType, PlantResourceGroups) == 232);

