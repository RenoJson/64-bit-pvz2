#pragma once
#include <Sexy/RtWeakPtr.h>
#include "Zombie.h"
#include "Board.h"

inline void* GetZombieTypeDirectory() {
	return *(void**)getActualOffset(0x256A620);
};
inline void GetZombieType(Sexy::RtWeakPtr<uintptr_t>* a1, void* a2, SexyString* a3) {
	typedef uintptr_t(*getZombieType)(uintptr_t, SexyString*);
	getZombieType getZBType = (getZombieType)getActualOffset(0x5E5924);
	uintptr_t getZomType = getZBType((uintptr_t)a2 + 8, a3);
	if ((uintptr_t)a2 + 16 != getZomType)
	{
		uintptr_t result = getZomType + 56;
		return a1->FromOther((Sexy::RtWeakPtr<uintptr_t>*)result);	
	}
};
inline Zombie* AddZombie(SexyString type, int a3, char a4, int a5) {
	void* zTypeDirectory = GetZombieTypeDirectory();
	Board* board = Board::GetBoard();
	Sexy::RtWeakPtr<uintptr_t> zPtr;
	GetZombieType(&zPtr, zTypeDirectory, &type);
	typedef Zombie* (*addZombieByType)(Board*, Sexy::RtWeakPtr<uintptr_t>*, int, char, int);
	addZombieByType funAddZombieByType = (addZombieByType)getActualOffset(0xAAE9F0);
	return funAddZombieByType(board, &zPtr, a3, a4, a5);
}