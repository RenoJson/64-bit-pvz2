#pragma once
#include "RealObject.h"

class DamageInfo;

enum class BoardEntityStatType
{
	damage,
	armor,
	toughness,
	recharge,
	area,
	range,
	speed,
	variable,
	suncost,
	sunprod,
	plantfoodpower,
	armingtime,
	special,
	family,
	masterypiercing,
	masteryboost,
	masteryhealth
};

enum class BoardEntityStatValue
{
	damage0,
	damage1,
	damage2,
	damage3,
	damage4,
	damage5,
	damage6,
	recharge0,
	recharge1,
	recharge2,
	recharge3,
	recharge4,
	armor0,
	armor1,
	armor2,
	armor3,
	single,
	square,
	oneby4,
	threebytwo,
	threebythree,
	lane,
	fullboard,
	touch,
	close,
	threetiles,
	straight,
	lobbed,
	frontback,
	multihit,
	multilane,
	fiveway,
	toughness0,
	toughness1,
	toughness2,
	toughness3,
	toughness4,
	toughness5,
	toughness6,
	toughness7,
	toughness8,
	speed0,
	speed1,
	speed2,
	speed3,
	speed4,
	speed5,
	fourway,
	variable0,
	currant,
	sunprod0,
	masterypiercing,
	masteryboost,
	masteryhealth,
};

class BoardEntityStat
{
public:
	BoardEntityStatType Type;
	BoardEntityStatValue Value;
};

enum class BoardEntityHeight
{
	ground,
	low,
	normal,
	tall
};

enum class BoardEntityTypeFlag
{
	plant,
	zombie,
	griditem,
	other
};

class BoardEntity : public ModularRealObject
{
public:
	int32_t m_level;
	char pad[4];
	Sexy::Rect m_collisionRect; 
	int m_boardEntityFlags;    
	char component[8];
	pvztime_t m_createdTime;
	void GetGridCoords(Sexy::SexyVector2 res)
	{
		res.x = (this->m_position.x - 200.0) / 64.0;
		res.y = (this->m_position.y - 160.0) / 76.0;
	}

	virtual void TakeDamage(DamageInfo* dmgInfo) {};
	virtual void Function36() {};
	virtual void Function37() {};
	virtual void Function38() {};
	virtual void Function39() {};
	virtual void Function40() {};
	virtual void Function41() {};
	virtual void Function42() {};
	virtual void Function43() {};
	virtual void Function44() {};
	virtual void Function45() {};
	virtual void Function46() {};
	virtual void Function47() {};
	virtual SexyString* GetTypeName() {};
	virtual void OnSpawn() {};
};

static_assert(sizeof(BoardEntity) == 120);
static_assert(offsetof(BoardEntity, m_level) == 80);
static_assert(offsetof(BoardEntity, m_collisionRect) == 88);
//static_assert(offsetof(BoardEntity, m_componentRunner) == 112);
//static_assert(offsetof(BoardEntity, m_createdTime) == 120);

enum GroundEffectType
{
	tide,
	tide_with_tail,
	tide_gargantuar,
	rise,
	surf
};

class EntityComponent_GroundEffect : public Sexy::RtObject
{
public:
	char m_pad[4];
	GroundEffectType m_type;
	Sexy::RtWeakPtr<RtObject> m_effect;
};

static_assert(sizeof(EntityComponent_GroundEffect) == 24);
static_assert(offsetof(EntityComponent_GroundEffect, m_type) == 12);
static_assert(offsetof(EntityComponent_GroundEffect, m_effect) == 16);