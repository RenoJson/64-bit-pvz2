#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <unistd.h>

#include "Main.h"

#include "PvZ2/Board.h"
#include "PvZ2/AudioMgr.h"
#include "PvZ2/WorldMap.h"
#include "PvZ2/NamedFlagWithId.h"
#include "PvZ2/GameFeature.h"
#include "PvZ2/Zombie_BullProps.h"
#include "PvZ2/Zombie_BullVeteran.h"
#include "PvZ2/Chairracer.h"
#include "PvZ2/ZombieCavalryProps.h"
#include "PvZ2/ZombieDarkCavalry.h"
#include "PvZ2/ZombieWesternTudigongProps.h"

#include "PvZ2/ZombieWesternKongMingLantern.h"
#include "PvZ2/ZombieFairyTaleImp.h"
#include "PvZ2/ZombieFairyTaleImpProps.h"
#include "PvZ2/ZombieFairyTaleGargantuar.h"
#include <PvZ2/ZombieWesternTudigong.h>
#include <PvZ2/ZombieAnimRig_ModernAllStar.h>
#include <PvZ2/ZombieAnimRig_EightiesPunk.h>
#include <PvZ2/ZombieAnimRig_Consultant.h>
#include <PvZ2/ZombieAnimRig_Hunter.h>


// usually using int64_t when hooking on 64 bit architecture, but i'm prefer uintptr_t since it's more flexible and less buggy

class NPCDataSheet
{
public:
    char m_pad[256];
    SexyString IdleWithItemAnim;
};

static_assert(offsetof(NPCDataSheet, IdleWithItemAnim) == 256);

typedef void* (*npcDataSheetCtor)(NPCDataSheet*);
npcDataSheetCtor oNPCDataSheetCtor = nullptr;

void* hkNPCDataSheetCtor(NPCDataSheet* thisPtr)
{
    oNPCDataSheetCtor(thisPtr);
    thisPtr->IdleWithItemAnim = "anim_%s_idle";
    return thisPtr;
}

typedef void (*boardWaveFunc)(Board*, int, int, bool);
boardWaveFunc oBoardWaveFunc = nullptr;

void hkBoardWaveFunc(Board* thisPtr, int waveIndex, int waveType, bool isFinalWave)
{
    oBoardWaveFunc(thisPtr, waveIndex, waveType, isFinalWave);
    if (isFinalWave) {
        AudioMgr::GetInstance()->PlayAudio("Play_FinalWave");
    }
}

#pragma endregion

#pragma region Boss Icon Softcode

typedef int64_t(*bossProgressMeterInit)();
bossProgressMeterInit oBossProgressMeterInit = nullptr;

int64_t hkBossProgressMeterInit()
{
    LOGI("cipa");
    int64_t orig = oBossProgressMeterInit();

    SexyString* zombossIconId = reinterpret_cast<SexyString*>(getActualOffset(0x256B760));
    *zombossIconId = "IMAGE_UI_HUD_INGAME_MINT";

    return orig;
}

#pragma endregion

__attribute__((constructor))
// This is automatically executed when the lib is loaded
// Run your initialization code here
void libChair_main()
{
    LOGI("Initializing %s", LIB_TAG);
    // Function hooks
    //PVZ2HookFunction(0x11F72B0, (void*)hkNPCDataSheetCtor, (void**)&oNPCDataSheetCtor);
    //PVZ2HookFunction(0x540938, (void*)hkBossProgressMeterInit, (void**)&oBossProgressMeterInit);
    ZombieBullProps::modInit();
    ZombieBullVeteranProps::modInit();
    ZombieZcorpRacerProps::modInit();
    ZombieFairyTaleImp::modInit();
    ZombieFairyTaleImpProps::modInit();
    ZombieFairyTaleGargantuar::modInit();
    ZombieAnimRig_ModernAllStar::modInit();
    ZombieAnimRig_JourneyToTheWestAllStar::modInit();
    ZombieAnimRig_EightiesPunk::modInit();
    ZombieAnimRig_Consultant::modInit();
    ZombieAnimRig_JourneyToTheWestTrident::modInit();
}