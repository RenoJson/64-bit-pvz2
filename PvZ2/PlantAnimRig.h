#pragma once
#include "PopAnimRig.h"
#include "PropertySheetBase.h"

class PlantAnimRigPropertySheet;

class PlantCustomLayers
{
public:
	SexyString m_baseName;
	char m_pad[24];
};

static_assert(sizeof(PlantCustomLayers) == 48);
static_assert(offsetof(PlantCustomLayers, m_baseName) == 0);

class PlantAnimRig : public PopAnimRig
{
public:
	int m_stateAnimsCompleted;
	char m_pad5[52];
	int m_plantFoodPlayCount;
	int m_currentDamageStateIndex;
	char m_pad6[40];
	Sexy::DelegateBase m_plantfoodCallback;
	Sexy::DelegateBase m_firingCallback;
	ZombieEvent m_onPlantfoodMainStarted;
	ZombieEvent m_onPlantfoodMainLooped;
	ZombieEvent m_onPlantfoodMainEnded;
	std::vector<PlantCustomLayers> m_customizableLayers;
	Sexy::RtWeakPtr<PlantAnimRigPropertySheet> m_props;
	int m_tier;

	virtual void Function32() {};
	virtual void Function33() {};
	virtual void PlayAttackBasedOnTier() {};
	virtual void PlayPlantfoodWithNoCallbacks() {};
	virtual void Function36() {};
	virtual void Function37() {};
	virtual void PlayHelpedOn() {};
	virtual void PlayHelped() {};
	virtual void PlayHelpedOff() {};
	virtual void PlayReload() {};
	virtual void PlayReloadLoop() {};
	virtual void PlayReloadEnd() {};
	virtual void PlayCharging() {};
	virtual void PlayCycling() {};
	virtual void Function46() {};
	virtual void Function47() {};
	virtual void Function48() {};
	virtual void Function49() {};
	virtual void Function50() {};
	virtual void Function51() {};
	virtual void PlayPlantfoodOn() {};
	virtual void PlayPlantfood() {};
	virtual void PlayPlantfoodOff() {};
	virtual void GetAttack() {};
	virtual void GetDamage() {};
	virtual void GetPlantfoodOn() {};
	virtual void GetPlantfood() {};
	virtual void GetPlantfoodOff() {};
	virtual void GetHelpedOn() {};
	virtual void GetHelped() {};
	virtual void GetHelpedOff() {};
	virtual void GetReload() {};
	virtual void GetReloadLoop() {};
	virtual void GetReloadEnd() {};
	virtual void GetCharging() {};
	virtual void GetCycling() {};
};

static_assert(sizeof(PlantAnimRig) == 1064);
static_assert(offsetof(PlantAnimRig, m_stateAnimsCompleted) == 592);
static_assert(offsetof(PlantAnimRig, m_plantFoodPlayCount) == 648);
static_assert(offsetof(PlantAnimRig, m_onPlantfoodMainStarted) == 760);
static_assert(offsetof(PlantAnimRig, m_customizableLayers) == 1024);
static_assert(offsetof(PlantAnimRig, m_props) == 1048);

class PlantAnimRigPropertySheet : public PropertySheetBase
{
public:
	std::vector<SexyString> AttackAnimation;
};

static_assert(sizeof(PlantAnimRigPropertySheet) == 64);
static_assert(offsetof(PlantAnimRigPropertySheet, AttackAnimation) == 40);