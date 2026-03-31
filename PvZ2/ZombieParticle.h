#pragma once
#include "BoardEntity.h"

class PopAnimRig;

class ZombieParticle : public BoardEntity
{
public:
	pvztime_t m_creationTime;
	char m_pad6[4];
	Sexy::RtWeakPtr<PopAnimRig> m_zombieRig;
	float m_rotation;
	float m_currentRotation;
	float m_scale;
	float m_shadowScale;
	pvztime_t m_motionBeginTime;
	pvztime_t m_fadeOutBeginTime;
	bool m_settled;
	Sexy::Rect m_newParticleRect;
	Sexy::SexyVector3 m_velocity;
	Sexy::SexyVector3 m_acceleration;
	Sexy::Rect m_spriteRectInZombie;
	SexyString m_particleName;
	int m_motionType;
	int m_attributes;
	char m_pad5[8];
};

static_assert(sizeof(ZombieParticle) == 264);
static_assert(offsetof(ZombieParticle, m_zombieRig) == 128);
static_assert(offsetof(ZombieParticle, m_motionBeginTime) == 152);
static_assert(offsetof(ZombieParticle, m_velocity) == 180);
static_assert(offsetof(ZombieParticle, m_motionType) == 248);