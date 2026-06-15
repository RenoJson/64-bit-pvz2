#pragma once
#include "Zombie.h"
// Zombie Helper Function Start Here

inline float ZombieFacing(Zombie* zombie) {
	if (zombie->m_facing) {
		return 1.0f;
	}
	return -1.0f;
}

inline void ZombieEnterState(Zombie* zombie, int state, int a3) {
	CallFunc<void, Zombie*, int, int>(0xC3D428, zombie, state, a3);
}

inline bool ZombieIsDeadOrDying(Zombie* zombie) {
	return CallFunc<bool, Zombie*>(0xC3E204, zombie);
}

inline Zombie* ZombieAllowMovement(Zombie* zombie, bool isAllowed) {
	return CallFunc<Zombie*, Zombie*, bool>(0xC51F94, zombie, isAllowed);
}

inline Zombie* ZombieFlippedAnim(Zombie* zombie, bool isFlipped) {
	return CallFunc<Zombie*, Zombie*, bool>(0xC41290, zombie, isFlipped);
}

inline void ZombieIsFlying(Zombie* zombie, bool isFlying) {
	CallFunc<void, Zombie*, bool>(0xC4C770, zombie, isFlying);
}

inline Zombie* ZombieUpdatePosition(Zombie* zombie, SexyVector3* newPos) {
	return CallFunc<Zombie*, Zombie*, SexyVector3*>(0x628278, zombie, newPos);
}

inline int ZombieSetPosition(Zombie* zombie, SexyVector3* newPos) {
	return CallFunc<int, Zombie*, SexyVector3*>(0x628058, zombie, newPos);
}

inline void ZombieOnSpawn(Zombie* zombie) {
	CallFunc<void, Zombie*>(0xC3D1F0, zombie);
}

inline void ZombiePlaySoundEvent(Zombie* zombie, const SexyString& eventName, float delayDuration) {
	CallFunc<void*, Zombie*, const SexyString&, float>(0x10B0608, zombie, eventName, delayDuration);
}

inline void ZombieSetCondition(Zombie* zombie, int conditionID, int a3, float duration, float a5) {
	CallFunc<void, Zombie*, int, int, float, float>(0xC40CC0, zombie, conditionID, a3, duration, a5);
}

inline void ZombieSetSpeedScale(Zombie* zombie, float scale) {
	CallFunc<void, Zombie*, float>(0xC484C0, zombie, scale);
}

inline bool ZombieIsInState(Zombie* zombie, int state) {
	return zombie->m_entityState.m_id == state;
}

inline bool ZombieHasCondition(Zombie* zombie, int condition) {
	return CallFunc<bool, Zombie*, int>(0xC3E44C, zombie, condition);
}

inline bool ZombieHasArmor(Zombie* zombie, const SexyString& armorName) {
	return CallFunc<bool, Zombie*, const SexyString&>(0xC3F2E4, zombie, armorName);
}


// Zombie Anim Rig Helper Function Start From Here

inline bool SetAnimLayerVisible(ZombieAnimRig* animRig, const SexyString& layerName, bool visible){
	return CallFunc<bool, ZombieAnimRig*, const SexyString&, bool>(0x9DB8D0, animRig, layerName, visible);
}

inline bool GetAnimRigSpritePosition(ZombieAnimRig* animRig, const SexyString& layerName, SexyVector2* spritePos) {
	return CallFunc<bool, ZombieAnimRig*, const SexyString&, SexyVector2*>(0x9DFBD4, animRig, layerName, spritePos);
}

inline void SetWalkSpeed(ZombieAnimRig* animRig, float speed) {
	return CallFunc<void, ZombieAnimRig*, float>(0x8DDAA4, animRig, speed);
}

// Find target in rect start here
inline void GetEntitiesInRectGrid(std::vector<BoardEntity*>* entities, uint typeID, Rect* area) {
	CallFunc<void, std::vector<BoardEntity*>*, uint, Rect*>(0x86F180, entities, typeID, area);
}

inline void GetEntitiesInRectPixel(std::vector<BoardEntity*>* entities, uint typeID, Rect* area, int minRow, int maxRow) {
	CallFunc<void, std::vector<BoardEntity*>*, uint, Rect*, int, int>(0x86F340, entities, typeID, area, minRow, maxRow);
}