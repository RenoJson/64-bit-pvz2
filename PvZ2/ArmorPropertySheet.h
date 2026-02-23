#pragma once
#include "PropertySheetBase.h"
#include "Zombie.h"
using namespace Sexy;
class Zombie;
enum ArmorTypeFlags {
	none = 0,
	metallic = 1,
	damageable = 2,
	droppable = 4,
	passdamage = 8,
	absorboverflow = 16,
	body = 32,
	helm = 64,
	invincible = 128,
	visibleondestroy = 256

};
class ArmorPropertySheet : public PropertySheetBase {
public:
	SexyString ClassName;
	SexyString ArmorType;
	float BaseHealth;
	char pad[4];
	std::vector<SexyString> ArmorLayers;
	std::vector<float> ArmorLayerHealth;
	std::vector<SexyString> ParticleLayerOverride;
	SexyString ImpactSoundEvent;
	SexyString DropSoundEvent;
	std::vector<ArmorTypeFlags> ArmorFlags;
};
static_assert(sizeof(ArmorPropertySheet) == 240);
static_assert(offsetof(ArmorPropertySheet, ClassName) == 40);
static_assert(offsetof(ArmorPropertySheet, ArmorType) == 64);
static_assert(offsetof(ArmorPropertySheet, BaseHealth) == 88);
static_assert(offsetof(ArmorPropertySheet, ArmorLayers) == 96);
static_assert(offsetof(ArmorPropertySheet, ArmorLayerHealth) == 120);
static_assert(offsetof(ArmorPropertySheet, ParticleLayerOverride) == 144);
static_assert(offsetof(ArmorPropertySheet, ImpactSoundEvent) == 168);
static_assert(offsetof(ArmorPropertySheet, DropSoundEvent) == 192);
static_assert(offsetof(ArmorPropertySheet, ArmorFlags) == 216);

class Armor : public GameObject {
public:
	uint8_t m_padTo24[8]; 
	RtWeakPtr<ArmorPropertySheet> m_propertySheetPtr;
	RtWeakPtr<Zombie> m_ownerZombiePtr;
	float m_health;
	float m_maxHealth;
	int m_damageState;
	bool m_destroyed;
	bool m_score;
	uint8_t m_padTo56[2];
	int m_armorFlagsOverride;
};
static_assert(sizeof(Armor) == 64);