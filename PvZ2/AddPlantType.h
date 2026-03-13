#pragma once
#include <Sexy/RtWeakPtr.h>
#include "Plant.h"
#include "Board.h"
inline void* GetDefaultPlantSpawnArgs() {
    return (void*)getActualOffset(0x2513080);
};
class BoardPlanting {
    virtual void function1();
    virtual void function2();
};

inline BoardPlanting* GetBoardPlantingInstance()
{
    typedef BoardPlanting* (*getBoardPlanting)(BoardPlanting*);
    getBoardPlanting GetBoardPlanting = (getBoardPlanting)getActualOffset(0x9B9A50);
    BoardPlanting* boardPtr = *(BoardPlanting**)getActualOffset(0x256AD20);
    if (!boardPtr) {
        boardPtr = new BoardPlanting;   
        GetBoardPlanting(boardPtr);
        return boardPtr;
    }
    return boardPtr;
}
inline Plant* AddPlant(SexyString type, int column, int row) {
    void* a5 = GetDefaultPlantSpawnArgs();
    BoardPlanting* board = GetBoardPlantingInstance();
    typedef Plant* (*addPlantByType)(BoardPlanting*, int, int, SexyString*, void*);
    addPlantByType funAddPlantByType = (addPlantByType)getActualOffset(0x9BB46C);
    return funAddPlantByType(board, column, row, &type, a5);
}