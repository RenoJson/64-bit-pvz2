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
#include "PvZ2/ZombieConditions.h"
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
#include <PvZ2/ZombieAnimRig_Gargantuar.h>
#include <PvZ2/ZombieDinoBasic.h>
#include <PvZ2/ZombieAnimRig_Basic.h>
#include <PvZ2/ZombiePirateBoomBarrel.h>
#include <PvZ2/ZombieAnimRig_Imp.h>
#include <PvZ2/ZombieAnimRig_DarkRider.h>
#include <PvZ2/ZombieDarkCavalryRider.h>
#include <PvZ2/DangerRoomLevelDesigner.h>
#include <PvZ2/ZombieModifierModule.h>
#include <PvZ2/ZombieModernBalloon.h>
#include <PvZ2/ZombieAnimRig_ModernBalloon.h>
#include <PvZ2/ZombieJourneyToTheWestBalloonProps.h>
#include <PvZ2/ZombieJourneyToTheWestVendor.h>
#include <PvZ2/ZombieJourneyToTheWestVendorProps.h>
#include <PvZ2/ZombieAnimRig_Vendor.h>
#include <PvZ2/ZombieJourneyToTheWestPiggy.h>
#include <PvZ2/ZombieJourneyToTheWestPiggyProps.h>
#include <PvZ2/ZombieDinoBullyVeteranProps.h>
#include <PvZ2/ZombiePharaoh.h>
#include <PvZ2/ZombieTombRaiser.h>
#include <PvZ2/ZombieTombRaiserProps.h>
#include <PvZ2/ZombieAnimRig_TombRaiser.h>


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

void hkCamelZombieFunc(Zombie* thisPtr, int64_t a2, bool a3)
{
    // Redirect call to some function in CamelMinigameModule
    // This fixes the crash when camels are rising from the ground
    CallFunc<void, Zombie*, int64_t, bool>(0xB1BE04, thisPtr, a2, a3);
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
typedef Zombie* (*effectCondition)(Zombie*, ZombieConditions);
effectCondition oEffCond = nullptr;
Zombie* hkEffectCondition(Zombie* zombie, ZombieConditions cond) {
    typedef Zombie* (*setEffectAnim)(Zombie*, const char*, const char*, const char*, SexyVector3*, uint, bool, bool, uint);
    setEffectAnim setAnim = (setEffectAnim)getActualOffset(0x7BF03C);

    switch (cond)
    {
        case zombie_condition_dazeystunned:
        case zombie_condition_stickybombed:
        case zombie_condition_stun:
        {
            if (zombie->m_attachedEffects.GetObjectIndex("stun") == -1) {
                SexyVector3 transformOffset = { -25.0f, -20.0f, 0.0f };
                setAnim(zombie, "stun", "POPANIM_EFFECTS_ZOMBIE_STUN_EFFECT", "stun_fx", &transformOffset, 1, false, false, 2);
            }
            break;
        }
        case zombie_condition_speeddown1:
        case zombie_condition_speeddown2:
        case zombie_condition_speeddown3:
        case zombie_condition_speeddown4:
        {
            if (zombie->m_attachedEffects.GetObjectIndex("slow") == -1) {
                SexyVector3 transformOffset = { 10.0f, 0.0f, 0.0f };
                setAnim(zombie, "slow", "POPANIM_EFFECTS_ZOMBIE_SLOWDOWN", "anim", &transformOffset, 1, false, false, 2);
            }
            break;
        }
    }
    return oEffCond(zombie, cond);
}
typedef Zombie* (*removeeffectCondition)(Zombie*, ZombieConditions);
removeeffectCondition oRemoveEffCond = nullptr;
Zombie* hkRemoveEffectCondition(Zombie* zombie, ZombieConditions cond) {
    typedef int (*removeEffectAnim)(AttachedEffectManager*, SexyString*);
    removeEffectAnim removeAnim = (removeEffectAnim)getActualOffset(0x662360);

    switch (cond) {
        case zombie_condition_dazeystunned:
        case zombie_condition_stickybombed:
        case zombie_condition_stun:
        {
            std::string stun = "stun";
            removeAnim(&zombie->m_attachedEffects, &stun);
            break;
        }
        case zombie_condition_speeddown1:
        case zombie_condition_speeddown2:
        case zombie_condition_speeddown3:
        case zombie_condition_speeddown4:
        {
            std::string slow = "slow";
            removeAnim(&zombie->m_attachedEffects, &slow);
            break;
        }
    }
    
    return oRemoveEffCond(zombie, cond);
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
    // i should make softcode boss icon as level module xd
    //PVZ2HookFunction(0x540938, (void*)hkBossProgressMeterInit, (void**)&oBossProgressMeterInit);
    PVZ2HookFunction(0xC4987C, (void*)hkEffectCondition, (void**)&oEffCond);
    PVZ2HookFunction(0xC4BC48, (void*)hkRemoveEffectCondition, (void**)&oRemoveEffCond);
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
    ZombieAnimRig_Gargantuar::modInit();
    ZombieAnimRig_FairyTaleGargantuar::modInit();
    ZombieJourneyToTheWestGargantuar::modInit();
    ZombieAnimRig_JourneyToTheWestGargantuar::modInit();
    ZombiePirateBoomBarrel::modInit();
    ZombieAnimRig_PirateBoomBarrel::modInit();
    DangerRoomFallenKnightDesigner::modInit();
    DangerRoomHelpdeskSupportDesigner::modInit();
    ZombieModifierProperties::modInit();
    ZombieAnimRig_ModernBalloon::modInit();
    ZombieJourneyToTheWestBalloon::modInit();   
    ZombieJourneyToTheWestBalloonProps::modInit();
    ZombieJourneyToTheWestVendor::ModInit();
    ZombieJourneyToTheWestVendorProps::modInit();
    ZombieAnimRig_Vendor::modInit();
    ZombieAnimRig_Piggy::modInit();
    ZombieJourneyToTheWestPiggy::ModInit();
    ZombieJourneyToTheWestPiggyProps::modInit();
    ZombieJourneyToTheWestGargantuarProps::modInit();
    //ZombieEgyptPharaoh::ModInit();
    ZombieZCorpEnergyDrinker::modInit();
    ZombieZCorpEnergyDrinkerProps::modInit();
    ZombieAnimRig_EnergyDrinker::modInit();
}