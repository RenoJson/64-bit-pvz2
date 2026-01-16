#pragma once
#include "ObjectTypeDescriptor.h"

enum GameFeature
{
	feature_shovel = 0,
	feature_almanac = 1,
	feature_coins = 2,
	feature_keys = 3,
	feature_worldmap = 4,
	feature_plantfood = 5,
	feature_plantfood_purchase = 6,
	feature_has_played_dangerroom = 7,
	feature_has_failed_dangerroom = 8,
	feature_has_won_dangerroom = 9,
	feature_treasure_yeti = 10,
	upgrade_7_slots = 11,
	upgrade_seedslot2 = 12,
	upgrade_sunshovel_lvl1 = 13,
	upgrade_sunshovel_lvl2 = 14,
	upgrade_sunshovel3 = 15,
	upgrade_pf_refresh = 16,
	upgrade_wallnut_firstaid = 17,
	upgrade_pf_slots_lvl1 = 18,
	upgrade_pf_slots_lvl2 = 19,
	upgrade_starting_sun_lvl1 = 20,
	upgrade_startingsun2 = 21,
	feature_powerup_wizardfinger = 22,
	feature_powerup_flick_zombie = 23,
	feature_powerup_pinch = 24,
	upgrade_manual_mowers_1 = 25,
	feature_level_of_day = 27,
	upgrade_valuable_mowers = 28,
	feature_gems = 29,
	feature_powerup_flamethrower = 30,
	upgrade_powerup_duration_1 = 31,
	upgrade_powerup_duration_2 = 32,
	feature_collectable_bacon = 33,
	feature_powerup_snowball = 34,
	world_trophy_egypt = 35,
	world_trophy_pirate = 36,
	world_trophy_cowboy = 37,
	world_trophy_future = 46,
	world_trophy_dark = 49,
	world_trophy_beach = 52,
	feature_powerup_vasebreaker_move = 53,
	feature_powerup_vasebreaker_reveal = 54,
	feature_powerup_vasebreaker_color = 55,
	feature_powerup_vasebreaker_butter = 56,
	world_trophy_iceage = 61,
	birthday_worldkey_1 = 62,
	elder_player_quest_rollup = 63,
	world_trophy_lostcity = 66,
	feature_powerup_beghouled_shuffle = 67,
	feature_powerup_beghouled_shovel = 68,
	feature_powerup_beghouled_wizardfinger = 69,
	world_trophy_eighties = 72,
	world_trophy_dino = 73,
	world_trophy_modern = 78,
	feature_grant_worldkeys = 79,
	feature_quest_pinata_hunt_slot_1 = 80,
	feature_quest_pinata_hunt_slot_2 = 81,
	league_reached_soil = 85,
	league_reached_wood = 86,
	league_reached_brick = 87,
	league_reached_iron = 88,
	league_acquired_bronze = 89,
	league_acquired_silver = 90,
	league_acquired_gold = 91,
	league_acquired_jade = 92,
	league_acquired_crystal = 93,
	battlez_high_score_1 = 94,
	battlez_high_score_2 = 95,
	battlez_high_score_3 = 96,
	battlez_high_score_4 = 97,
	battlez_high_score_5 = 98,
	battlez_high_score_6 = 99,
	battlez_high_score_7 = 100,
	battlez_avatar_updated = 101,
	ftue_jan_2019_conversion = 102,
	pirate_stargate_start = 105,
	cowboy_stargate_start = 106,
	iceage_stargate_start = 107,
	lostcity_stargate_start = 108,
	future_stargate_start = 109,
	dark_stargate_start = 110,
	eighties_stargate_start = 111,
	dino_stargate_start = 112,
	beach_stargate_start = 113,
	modern_stargate_start = 114,
	has_done_linear_adventure_conversion_mar_2021 = 115,
	is_nonlinear_adventure_flag_pre_mar_2021 = 116,
	calendar_unlock = 117,
	thymed_event_unlock = 120,
	feature_unlock_oct_2021 = 121,
	has_done_linear_adventure_conversion_oct_2021 = 122,
	feature_worldkeys = 256,

	// new features go here
	// they should be individually registered in the RegisterClass method
};

namespace Reflection
{
	class RClass;
	class CRefManualSymbolBuilder;
}

class GameFeatureType : public ObjectTypeDescriptor
{
public:
	SexyString LocalizedName;
	SexyString LocalizedInfo;
	SexyString HomeWorld;
	GameFeature Feature;
	bool IsAPowerup;
	bool VisibleInAlmanac;
	SexyString UpgradeImage;
};

static_assert(sizeof(GameFeatureType) == 136);
static_assert(offsetof(GameFeatureType, LocalizedName) == 32);
static_assert(offsetof(GameFeatureType, HomeWorld) == 80);
static_assert(offsetof(GameFeatureType, Feature) == 104);
static_assert(offsetof(GameFeatureType, UpgradeImage) == 112);