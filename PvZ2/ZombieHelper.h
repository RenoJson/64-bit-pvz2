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

inline void ZombieOnSpawn(Zombie* zombie) {
	CallFunc<void, Zombie*>(0xC3D1F0, zombie);
}

inline bool ZombieIsInState(Zombie* zombie, int state) {
	return zombie->m_entityState.m_id == state;
}

// Anim Rig Helper Function Start From Here

inline bool SetAnimLayerVisible(ZombieAnimRig* animRig, const SexyString& layerName, bool visible){
	return CallFunc<bool, ZombieAnimRig*, const SexyString&, bool>(0x9DB8D0, animRig, layerName, visible);
}

inline bool GetAnimRigSpritePosition(ZombieAnimRig* animRig, const SexyString& layerName, SexyVector2* spritePos) {
	return CallFunc<bool, ZombieAnimRig*, const SexyString&, SexyVector2*>(0x9DFBD4, animRig, layerName, spritePos);
}

inline void SetWalkSpeed(ZombieAnimRig* animRig, float speed) {
	return CallFunc<void, ZombieAnimRig*, float>(0x8DDAA4, animRig, speed);
}