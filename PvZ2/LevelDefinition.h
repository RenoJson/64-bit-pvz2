#pragma once
#include "PropertySheetBase.h"
#include "LevelModule.h"
#include "StageModule.h"
#include "GameFeature.h"

enum class AwardType
{
	none = 0,
	present = 1,
	collectable = 2,
	unlock_plant = 3,
	powerup = 4,
	upgrade = 5,
	mapgadget = 6,
	powerupgadget = 7,
	firstkey = 8,
	note = 9,
	worldtrophy = 10,
	coins = 11,
	gems = 12,
	powerupsuse = 13,
	key = 14,
	game_feature = 15,
	costumegroup_lod = 16,
	costume = 17,
	giftbox = 18,
	worldkey = 19,
	sprout = 20,
	plant_boost = 21,
	unlock_plant_as_packet = 22,
	pinata = 23,
	plantXP = 24,
	rvplantrental = 25,
	tutorialgems = 26,
	tickets = 27,
	mints = 28,
	seasonprogress = 29,
	fuel = 30,
	zps = 34
};

class LevelDefinition : public PropertySheetBase
{
public:
	SexyString Name;
	SexyString Description;
	SexyString MainObjective;
	int16_t StartingSun;
	bool AddBonusStartingSun;
	bool SuppressSunSpawn;
	int LevelNumber;
	int FlickPowerupRightEdgeOffset;
	char m_pad[4];
	Sexy::RtWeakPtr<StageModuleProperties> StageModule;
	Sexy::RtWeakPtr<void> VictoryModule; // OutroModuleProperties
	Sexy::RtWeakPtr<void> LossModule; // OutroModuleProperties
	Sexy::RtWeakPtr<void> LevelMutator; // LevelMutatorModuleProperties
	std::vector<Sexy::RtWeakPtr<LevelModule>> Modules;
	Sexy::RtWeakPtr<void> Loot; // LootConfig
	SexyString NormalPresentTable;
	SexyString ShinyPresentTable;
	bool SuppressAwardScreen;
	AwardType FirstRewardType;
	SexyString FirstRewardParam;
	AwardType ReplayRewardType;
	SexyString ReplayRewardParam;
	bool GameOverDialogShowBrain;
	bool ShowZombieThreatAlerts;
	SexyString FirstIntroNarrative;
	SexyString ReplayIntroNarrative;
	SexyString FirstOutroNarrative;
	SexyString ReplayOutroNarrative;
	SexyString ForceNextLevel;
	bool ForceWarpTunnel;
	bool ForceToWorldMap;
	bool RepeatPlayForceToWorldMap;
	bool IsVasebreaker;
	bool IsDangerroom;
	bool IsLevelOfTheDay;
	bool DisableRestart;
	SexyString CompletionAchievement;
	std::vector<GameFeature> GameFeaturesToUnlock;
	SexyString MusicType;
	SexyString DraperPromo;
	SexyString LevelPowerupSet;
	SexyString LevelJam;
	bool DoMusicChanges;
	std::vector<SexyString> ResourceGroupNames;
	std::vector<SexyString> LevelPrerequisites;
	std::vector<SexyString> GroupsToUnloadForAds;
	std::vector<SexyString> RandomLevelSet;
	bool LoadDefaultMusic;
	bool SuppressDynamicTutorial;
	int ZombieLevel;
	int GridItemLevel;
	int FixedPlantLevel;
	bool SuppressPlantfoodPurchase;
	bool SuppressAccountUpgrades;
	bool DisablePlantBoosts;
	bool DisableSaveGame;
	char m_pad2[4];
	char GeneratorResult[352]; // LevelGenResults
	std::vector<SexyString> PerksDisabledInLevel;
	std::vector<std::vector<SexyString>> PerksDisabledPerDifficulty;
};

static_assert(sizeof(LevelDefinition) == 1112);
static_assert(offsetof(LevelDefinition, Name) == 40);
static_assert(offsetof(LevelDefinition, StartingSun) == 112);
static_assert(offsetof(LevelDefinition, LevelNumber) == 116);
static_assert(offsetof(LevelDefinition, Loot) == 184);
static_assert(offsetof(LevelDefinition, ReplayRewardType) == 272);
static_assert(offsetof(LevelDefinition, ResourceGroupNames) == 592);
static_assert(offsetof(LevelDefinition, LevelPrerequisites) == 616);
static_assert(offsetof(LevelDefinition, GroupsToUnloadForAds) == 640);
static_assert(offsetof(LevelDefinition, RandomLevelSet) == 664);
static_assert(offsetof(LevelDefinition, GeneratorResult) == 712);
static_assert(offsetof(LevelDefinition, PerksDisabledInLevel) == 1064);