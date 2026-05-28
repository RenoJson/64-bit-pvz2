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