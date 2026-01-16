#pragma once
#include "LevelModule.h"

namespace Sexy {
	class Image;
}

enum class MusicState {
	none = 0,
	music_pregame = 1,
	music_gamestart = 2,
	music_hugewave = 3,
	music_hugewave_2 = 4,
	music_finalwave = 5,
	music_losegame = 6,
	music_wingame = 7
};

class StageModule : public LevelModule
{
public:
	bool m_sendAlternateHugeWaveEvent;
	SexyString m_musicTriggerOverride;
	char m_pad[24];
	bool m_shouldStopMusic;
	bool m_shouldStartMusic;
	bool m_startMusicOnLastStandStart;
	MusicState m_musicState;
	bool m_suppressMusicStateChangeEvents;
	int m_lastWaveIntroMusic;
	Sexy::RtWeakPtr<Sexy::Image> m_backImage;
	Sexy::RtWeakPtr<Sexy::Image> m_backImageRight;
	Sexy::RtWeakPtr<Sexy::Image> m_backImageLeft;
	char m_pad2[48];
};

static_assert(sizeof(StageModule) == 168);
static_assert(offsetof(StageModule, m_sendAlternateHugeWaveEvent) == 24);
static_assert(offsetof(StageModule, m_musicTriggerOverride) == 32);
static_assert(offsetof(StageModule, m_shouldStopMusic) == 80);

class StageModuleProperties : public LevelModuleProperties
{
public:
	SexyString BackgroundResourceGroup;
	SexyString BackgroundImagePrefix;
	SexyString StagePrefix;
	SexyString BasicZombieTypeName;
	SexyString FlagZombieTypeName;
	std::vector<SexyString> FlagVeteranZombieTypeNames;
	SexyString Armor1ZombieTypeName;
	SexyString Armor2ZombieTypeName;
	SexyString Armor4ZombieTypeName;
	SexyString MusicSuffix;
	SexyString MusicSuffixMinigame;
	SexyString AmbientAudioSuffix;
	SexyString RailcartDefaultTypeName;
	SexyString GravestoneDefaultTypeName;
	SexyString DirtSpawnEffectName;
	SexyString LevelPowerupSet;
	SexyString ZombieEventAudio;
	std::vector<Sexy::Point> DisabledStreetCells;
	SexyString ProgressFlagAsset;
	SexyString StarChallengeUnlockLevel;
};

static_assert(sizeof(StageModuleProperties) == 608);
static_assert(offsetof(StageModuleProperties, BackgroundResourceGroup) == 128);
static_assert(offsetof(StageModuleProperties, BackgroundImagePrefix) == 152);
static_assert(offsetof(StageModuleProperties, RailcartDefaultTypeName) == 416);
static_assert(offsetof(StageModuleProperties, DisabledStreetCells) == 536);