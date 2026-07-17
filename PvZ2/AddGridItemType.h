#pragma once
#include <Sexy/RtWeakPtr.h>
#include "GridItem.h"
#include "Board.h"

inline GridItem* AddGridItem(SexyString type, int gridX, int gridY) {
	Board* board = Board::GetBoard();
	typedef GridItem* (*addGridItemByType)(Board*, SexyString, int, int);
	addGridItemByType funAddGridItemByType = (addGridItemByType)getActualOffset(0xAA230C);
	return funAddGridItemByType(board, type, gridX, gridY);
}

inline bool CanSpawnGridItemAt(int col, int row, bool displacePlant, bool canAddOnMold) {
	Board* board = Board::GetBoard();
	typedef bool (*addGridItemByType)(Board*, int, int, bool, bool);
	addGridItemByType funAddGridItemByType = (addGridItemByType)getActualOffset(0xAA230C);
	return funAddGridItemByType(board, col, row, displacePlant, canAddOnMold);
}