#pragma once
#include <Sexy/RtObject.h>
#include <Sexy/RtWeakPtr.h>
#include "GameObject.h"

#define GRID_TILE_WIDTH 64.0f
#define GRID_TILE_HEIGHT 76.0f
#define GRID_TILE_TOP 160.0f
#define GRID_TILE_LEFT 200.0f

#define BOARD_START_POS 232.0f
#define BOARD_END_POS 776.0f

class PresentRecord
{
public:
	SexyString PresentTableName;
	uint Seed;
	uint TimeAwarded;
	uint TimeOpened;
};

static_assert(sizeof(PresentRecord) == 40);
static_assert(offsetof(PresentRecord, Seed) == 24);
static_assert(offsetof(PresentRecord, TimeOpened) == 32);

class RowPickingItem
{
public:
	int m_row;
	float m_weight;
	float m_lastPicked;
	float m_secondLastPicked;
};

static_assert(sizeof(RowPickingItem) == 16);
static_assert(offsetof(RowPickingItem, m_row) == 0);

class LootStats
{
public:
	int LootTotalCount;
	int SilverCount;
	int GoldCount;
	int DiamondCount;
	int PowerupCount;
	int NoKeyCount;
	int KeyCount;
};

static_assert(sizeof(LootStats) == 28);
static_assert(offsetof(LootStats, LootTotalCount) == 0);
static_assert(offsetof(LootStats, NoKeyCount) == 20);

enum ThymedEventTrack
{
	track1,
	track2,
	track3
};

class BoardLayerEffect : public GameObject
{
public:
	Sexy::Color m_startColor;
	Sexy::Color m_endColor;
	pvztime_t m_startTime;
	pvztime_t m_endTime;
	bool m_useLerp;
	int m_curveType; // CurveType
	bool m_useCinematicTime;
	bool m_useTimeScaleCurve;
	float m_timeScaleStart;
	float m_timeScaleEnd;
	int m_timeScaleCurve;
	char m_pad[48];
};

static_assert(sizeof(BoardLayerEffect) == 128);
static_assert(offsetof(BoardLayerEffect, m_startColor) == 16);
static_assert(offsetof(BoardLayerEffect, m_startTime) == 48);
static_assert(offsetof(BoardLayerEffect, m_curveType) == 60);
static_assert(offsetof(BoardLayerEffect, m_useTimeScaleCurve) == 65);

class Zombie;
class GridItem;
class LevelDefinition;
class LevelModuleManager;
class Collectable;

// Board Widget class containing everything for global-level stage management
class Board : public Sexy::RtObject
{
public:
	char m_pad[208];
	int m_gridSizeX; // column count
	int m_gridSizeY; // row count
	int m_boardState;
	SexyString m_level;
	SexyString m_previousLevel;
	int m_levelNumber;
	int m_levelRandomIndex;
	pvztime_t m_levelStartTime;
	std::vector<SexyString> m_gameplayResources;
	int m_plantfoodCount;
	std::vector<PresentRecord> m_pendingPresentRewards;
	std::vector<Sexy::RtWeakPtr<Sexy::RtObject> > m_boardRegions; // BoardRegion

	// used for reward spawning feature I think
	Sexy::SexyVector3 m_finalDestroyedZombiePosition;

	// it's the current sun count
	// it could be used in mechanics that track the amount of collected sun
	int m_sunCurrency;
	char m_pad2[4];

	// I won't be using C-style arrays
	// fuck you Jay and Tenfoldedly ;3
	std::array<int, 5> m_chooseRowUnk;
	std::array<float, 5> m_timeRowGotLawnMowered;
	std::array<RowPickingItem, 5> m_rowPickingArray;
	std::array<std::array<int, 5>, 9> m_cellType;
	std::array<std::array<bool, 5>, 9> m_cellLocked;

	uint16_t m_plantfoodCountMax;
	LootStats m_lootRollStats;
	int m_activeStarIndex;
	bool m_levelPreviouslyCompleted;
	bool m_sunSpawningSuppressed;
	bool m_plantGrowthAndDecayPaused;
	bool m_plantsCanAttack;
	bool m_manualPlantsActive;
	bool m_pauseGameplayObjects;
	bool m_onlyDrawFade;
	char m_pad3[1];
	bool m_gridItemsEnabled;
	char m_pad4[15];
	Sexy::RtWeakPtr<Sexy::RtObject> m_animationMgr; // AnimationMgr
	std::vector<Sexy::RtWeakPtr<Sexy::RtObject>> m_pushedAnimationMgr; // AnimationMgr
	char m_pad5[72];
	int m_mowerLaunchesLeft;
	char m_pad6[36];
	int m_reasonPlayEnded;
	int m_destinationAfterBoard;
	char m_pad7[8];

	// falling collectable TypeName
	// if changed, can be used to make It's Raining Seeds
	SexyString m_collectableID_SunFromSky;
	Sexy::RtWeakPtr<LevelDefinition> m_levelDefinition;
	LevelModuleManager* m_levelModuleManager;
	char m_pad8[96];
	Sexy::Rect m_lawnRect; // grid size + zoom
	char m_pad9[112];
	std::vector<Zombie*> m_destroyedZombies;
	std::vector<Zombie*> m_addedZombies;
	int m_fastForwardState;
	char m_pad10[8];
	int m_replayCost;
	char m_pad11[4];

	// all fields below are just some achievement bullshit
	int m_bloverBlowAwayZombieCount;
	int m_tangleKelpedCount;
	int m_thawedIceblockCount;
	bool m_jestersSpawnedInLevel;
	bool m_jestersJuggled;
	bool m_wizardsSpawnedInLevel;
	bool m_wizardsSheepedPlant;
	bool m_zombieKilledByNonBanana;
	bool m_zombieKilledByBanana;
	bool m_plantedPhatBeet;
	bool m_plantedNonPhatBeet;
	bool m_fishermanSpawnedInLevel;
	bool m_fishermanHookedAPlant;
	bool m_weaselTouchedASliderTile;
	bool m_weaselSpawnedInLevel;
	int m_troglobitePushMinGridX;
	bool m_troglobitePushedABlock;
	bool m_excavatorSpawnedInLevel;
	bool m_excavatorShoveledAPlant;
	bool m_newspaperBurned;
	int m_goldLeafsPlantedCount;
	int m_tentSpawnCount;
	int m_thymewarpPlantedCount;
	bool m_boomboxSpawnedInLevel;
	bool m_boomboxBoomed;
	bool m_dinoSpawnedInLevel;
	bool m_dinoAffectedAZombie;
	bool m_allStarSpawned;
	bool m_allStarDestroyedPlant;
	int m_tRexKillCount;
	char m_pad12[4];

	// non-achievement fields resume here
	BoardLayerEffect m_layerEffectFastForward;
	char m_pad13[266];
	SexyString m_boardHolidayEventName;
	char m_pad14[20];
	int m_watchedAdForPowerup;
	ThymedEventTrack m_track;
	int m_eventsEndOfLevelFlag;
	char m_pad15[56];

	// Get the Board instance from gLawnApp
	static Board* GetBoard() {
		return CallFunc<Board*>(0xCE4D8C);
	}

	void ChangeState(int newState) {
		CallFunc<void, Board*, int>(0x71B1AC, this, newState); //TODO ADDR
	}

	void GetGridItemsAt(int gridX, int gridY, std::vector<GridItem*>* gridItems) {
		CallFunc<void, Board*, int, int, std::vector<GridItem*>* >(0x720E7C, this, gridX, gridY, gridItems);  //TODO ADDR
	}

	Collectable* SpawnCollectable(const SexyString& collectable, const Sexy::SexyVector2& position) {
		return CallFunc<Collectable*, Board*, SexyString, Sexy::SexyVector2>(0xAA4180, this, collectable, position);
	}
};

static_assert(sizeof(Board) == 1864);
static_assert(offsetof(Board, m_gridSizeX) == 216);
static_assert(offsetof(Board, m_gridSizeY) == 220);
static_assert(offsetof(Board, m_gameplayResources) == 296);
static_assert(offsetof(Board, m_plantfoodCountMax) == 742);
static_assert(offsetof(Board, m_lootRollStats) == 744);
static_assert(offsetof(Board, m_activeStarIndex) == 772);
static_assert(offsetof(Board, m_sunSpawningSuppressed) == 777);
static_assert(offsetof(Board, m_animationMgr) == 800);
static_assert(offsetof(Board, m_mowerLaunchesLeft) == 904);
static_assert(offsetof(Board, m_reasonPlayEnded) == 944);
static_assert(offsetof(Board, m_collectableID_SunFromSky) == 960);
static_assert(offsetof(Board, m_lawnRect) == 1096);
static_assert(offsetof(Board, m_fastForwardState) == 1272);
static_assert(offsetof(Board, m_replayCost) == 1284);
static_assert(offsetof(Board, m_jestersSpawnedInLevel) == 1304);
static_assert(offsetof(Board, m_layerEffectFastForward) == 1352);
static_assert(offsetof(Board, m_boardHolidayEventName) == 1752);
static_assert(offsetof(Board, m_watchedAdForPowerup) == 1796);

inline static uint64_t GetSexyApp() {
	return *(uint64_t*)getActualOffset(0x25A4618);
}

inline static uint64_t GetLawnApp() {
	return *(uint64_t*)getActualOffset(0x1E77D8C);
}