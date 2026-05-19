#pragma once
#include <Sexy/RtWeakPtr.h>
#include "Projectile.h"
#include "Board.h"

inline Projectile* AddProjectile(Sexy::RtWeakPtr<ProjectilePropertySheet>* type, BoardEntity* owner, float mX, float mY, float mZ) {
	Board* board = Board::GetBoard();
	typedef Projectile* (*addProjectileByType)(Board*, Sexy::RtWeakPtr<ProjectilePropertySheet>*, BoardEntity*, float, float, float);
	addProjectileByType funAddProjectileByType = (addProjectileByType)getActualOffset(0xAAE5B4);
	return funAddProjectileByType(board, type, owner, mX, mY, mZ);
}