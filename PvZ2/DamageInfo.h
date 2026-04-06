#pragma once
#include "ZombieConditions.h"
#include "PlantConditions.h"
#include "GridItemCondition.h"

enum DamageTypeFlags : int64_t
{
	damage_fatal = 0,
	damage_instantly_fatal = 1ULL << 1,
	damage_bypass_shield = 1ULL << 2,
	damage_hits_shield_and_body = 1ULL << 4,
	damage_hits_only_shield = 1ULL << 5,
	damage_lightning = 1ULL << 6,
	damage_no_flash = 1ULL << 7,
	damage_no_death_spawns = 1ULL << 8,
	damage_fire = 1ULL << 9,
	damage_lobbed = 1ULL << 10,
	damage_ash_death = 1ULL << 11,
	damage_plantfood_effect = 1ULL << 12,
	damage_mower = 1ULL << 13,
	damage_propagated = 1ULL << 14,
	damage_no_sound = 1ULL << 15,
	damage_eating = 1ULL << 16,
	damage_spinning = 1ULL << 17,
	damage_mech = 1ULL << 18,
	damage_crushing = 1ULL << 19,
	damage_friendly_fire = 1ULL << 20,
	damage_magnet = 1ULL << 21,
	damage_catapult = 1ULL << 22,
	damage_squidify = 1ULL << 23,
	damage_hits_submerged = 1ULL << 24,
	damage_frosty_tiny = 1ULL << 25,
	damage_frost_moderate = 1ULL << 26,
	damage_frost_whole_stage = 1ULL << 27,
	damage_frost_freeze = 1ULL << 28,
	damage_thaw_tiny = 1ULL << 29,
	damage_thaw_moderate = 1ULL << 30,
	damage_thaw_whole_stage = 1ULL << 31,
	damage_heals_iceblocks = 1ULL << 32,
	damage_shooter = 1ULL << 33,
	damage_melee = 1ULL << 34,
	damage_suicide = 1ULL << 35,
	damage_smash = 1ULL << 36,
	damage_drown = 1ULL << 37,
	damage_shovel = 1ULL << 38,
	damage_removed_from_board = 1ULL << 39,
	damage_plantify_on_death = 1ULL << 40,
	damage_no_bleed_on_death = 1ULL << 41,
};

class BoardEntity;

class DamageInfo
{
public:

	DamageInfo()
	{
		memset(this, 0, sizeof(DamageInfo));
	}

	~DamageInfo()
	{
		m_zombieConditions.clear();
		m_plantConditions.clear();
		m_gridItemConditions.clear();
		m_plantFamilies.clear();
	}

	BoardEntity* m_attacker;
	float m_damage;
	char pad[4];
	int64_t m_flags;
	std::vector<ZombieConditions> m_zombieConditions;
	std::vector<PlantConditions> m_plantConditions;
	std::vector<GridItemConditions> m_gridItemConditions;
	std::vector<SexyString> m_plantFamilies;
	float unk;

	void Copy(DamageInfo * from)
	{
		this->m_damage = from->m_damage;
		this->m_flags = from->m_flags;
		this->m_attacker = from->m_attacker;

		this->m_zombieConditions = from->m_zombieConditions;
		this->m_plantConditions = from->m_plantConditions;
		this->m_gridItemConditions = from->m_gridItemConditions;
		this->m_plantFamilies = from->m_plantFamilies;

		this->unk = from->unk;
	}
};

static_assert(sizeof(DamageInfo) == 128);
static_assert(offsetof(DamageInfo, m_damage) == 8);
static_assert(offsetof(DamageInfo, m_flags) == 16);
static_assert(offsetof(DamageInfo, m_plantFamilies) == 96);