#pragma once
#include "BoardEntity.h"
#include "ZombieState.h"
#include "ZombieDinoBasic.h"

class Creature : public BoardEntity
{
public:
	pvztime_t m_createdTime;
    bool m_walkingToStreet;
    char pad_7D[3]; 
    int m_streetStopX;
    char pad_84[4];
    RtWeakPtr<RtObject> m_type;
    RtWeakPtr<RtObject> m_animRig;

    int m_playingAnim;
    ZombieState m_state;
    double m_elapsedTimeInState;
    float m_stateMachineTimeScale;
    int m_creatureFlags;
    int m_facing;
    float m_walkSpeed;
    float m_translationMultiplier;
    float m_rotation;
    float m_scale;
    int m_renderLayerOffset;
    int m_waveToLeave;
    char pad_17C[4];
    char m_conditionTracker[152];
};

class Dinosaur : public Creature
{
public:
    RT_CLASS_GET_CLASS_WRAPPER(0x111BAC4);
    float m_timeout;
    bool m_isZombossLevel;
};
class DinosaurRaptor : public Dinosaur
{
public:
    RT_CLASS_GET_CLASS_WRAPPER(0x121F0C8);
	int m_kickRemaining;
    bool m_wantsToFlip;
    bool m_ableToKick;
	char pad_1A9[2];
    RtWeakPtr<Zombie> m_targetZombie;
};
class DinosaurStegosaurus : public Dinosaur
{
public:
    RT_CLASS_GET_CLASS_WRAPPER(0x13532F8);
	std::vector<RtWeakPtr<Zombie>> m_zombiesOnTail;
    std::vector<RtWeakPtr<Zombie>> m_zombiesInAttackRange;
    float m_timeToFling;
    int m_numTosses;
    bool m_isHead;
    bool m_hasFlung;
    bool m_prevCharmState;
};
class DinosaurPterodactyl : public Dinosaur
{
public:
    RT_CLASS_GET_CLASS_WRAPPER(0xD54454);
	float m_nextActivationTime;
	int m_numberOfZombiesCarriedAndDropped;
	RtWeakPtr<ZombieDinoBasic> m_grabbedZombie;
};
class DinosaurTyrannosaurus : public Dinosaur
{
public:
    RT_CLASS_GET_CLASS_WRAPPER(0x10DFEB4);
	float m_timeToEndCooldown;
	int m_numZombiesEaten;
	bool m_prevCharmState;
	char pad_1A9[3];
    int m_numTimeRoared;
	float m_timeToRoar;

};
class DinosaurAnkylosaurus : public Dinosaur
{
public:
    RT_CLASS_GET_CLASS_WRAPPER(0xC62ACC);
	float m_timeToFling;
	int m_numZombiesTossed;
    bool m_usedWalkState;
};