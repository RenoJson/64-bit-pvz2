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
#include "PvZ2/ZombieFairyTaleImp.h"
#include "PvZ2/ZombieFairyTaleImpProps.h"
#include "PvZ2/ZombieFairyTaleGargantuar.h"
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
#include "pch.h"
#include "And64InlineHook.hpp"
#include <PvZ2/FairyTaleWitchProjectile.h>
#include <PvZ2/ZombieAnimRig_FairyTaleWitch.h>
#include <PvZ2/ZombieWithAction.h>
#include <PvZ2/ZombieFairyTaleWitchFrog.h>
#include <PvZ2/ZombieGargantuarProps.h>


// TODO: Make every typedef function became a wrapper ig

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
    auto* props = reinterpret_cast<ZombiePropertySheet*>(zombie->m_propertySheet.Get());
    switch (cond)
    {
        case zombie_condition_dazeystunned:
        case zombie_condition_stun:
        {
           if (zombie->m_attachedEffects.GetObjectIndex("stickystun") == -1) {
               if (zombie->m_attachedEffects.GetObjectIndex("stun") == -1) {
                   SexyVector3 transformOffset;
                   if (props->Size == ZombieSize::large) {
                       transformOffset = { -25.0f, -80.0f, 0.0f };
                   }
                   else if (props->Size == ZombieSize::imp) {
                       transformOffset = { -25.0f, -10.0f, 0.0f };
                   }
                   else if (props->Size == ZombieSize::chicken) {
                       transformOffset = { -25.0f, 0.0f, 0.0f };
                   }
                   else {
                       transformOffset = { -25.0f, -20.0f, 0.0f };
                   }
                   setAnim(zombie, "stun", "POPANIM_EFFECTS_ZOMBIES_STUN_EFFECT", "stun_fx", &transformOffset, 1, false, false, 2);
               }
           }
           break;
        }
        case zombie_condition_stickybombed:
        {
            if (zombie->m_attachedEffects.GetObjectIndex("stickystun") == -1) {
                SexyVector3 transformOffset;
                if (props->Size == ZombieSize::large) {
                    transformOffset = { -25.0f, -100.0f, 0.0f };
                }
                else if (props->Size == ZombieSize::imp) {
                    transformOffset = { -25.0f, 0.0f, 0.0f };
                }
                else if (props->Size == ZombieSize::chicken) {
                    transformOffset = { -25.0f, 10.0f, 0.0f };
                }
                else {
                    transformOffset = { -25.0f, -20.0f, 0.0f };
                }
                setAnim(zombie, "stickystun", "POPANIM_EFFECTS_ZOMBIES_STUN_EFFECT", "stun_fx", &transformOffset, 1, false, false, 2);
            }
            break;
        }
        case zombie_condition_zombossstun:
        {
            if (zombie->m_attachedEffects.GetObjectIndex("zombossstun") == -1) {
                SexyVector3 transformOffset = { 60.0f, -200.0f, 0.0f };
                setAnim(zombie, "zombossstun", "POPANIM_EFFECTS_ZOMBOSS_STUN_EFFECT", "stun", &transformOffset, 1, false, false, 2);
            }
            break;
        }
        case zombie_condition_speeddown1:
        case zombie_condition_speeddown2:
        case zombie_condition_speeddown3:
        case zombie_condition_speeddown4:
        {
            if (zombie->m_attachedEffects.GetObjectIndex("slow") == -1) {
                SexyVector3 transformOffset = { 10.0f, -30.0f, 0.0f };
                setAnim(zombie, "slow", "POPANIM_EFFECTS_ZOMBIE_SLOWDOWN", "anim", &transformOffset, 1, false, false, 2);
            }
            break;
        }
        case zombie_condition_sapped:
        case zombie_condition_chill:
        case zombie_condition_stackableslow:
        case zombie_condition_stalled:
        {
            if (zombie->m_attachedEffects.GetObjectIndex("slow") == -1) {
                if (zombie->m_attachedEffects.GetObjectIndex("plantslow") == -1) {
                    SexyVector3 transformOffset = { 10.0f, -30.0f, 0.0f };
                    setAnim(zombie, "plantslow", "POPANIM_EFFECTS_ZOMBIE_SLOWDOWN", "anim", &transformOffset, 1, false, false, 2);
                }
            }
            break;
        }
        case zombie_condition_hungered:
        case zombie_condition_speedup1:
        case zombie_condition_speedup2:
        case zombie_condition_speedup3:
        case zombie_condition_speedup4:
        case zombie_condition_terrified:
        case zombie_condition_potionspeed1:
        case zombie_condition_potionspeed2:
        case zombie_condition_potionspeed3:
        case zombie_condition_potionsuper1:
        case zombie_condition_potionsuper2:
        case zombie_condition_potionsuper3:
        {
            if (zombie->m_attachedEffects.GetObjectIndex("zombiespeedup") == -1) {
                SexyVector3 transformOffset = { 20.0f, -20.0f, 0.0f };
                setAnim(zombie, "zombiespeedup", "POPANIM_EFFECTS_ZOMBIE_SPEEDUP", "zombie_speedup", &transformOffset, 1, false, false, 2);
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
        case zombie_condition_stun:
        {
            std::string stun = "stun";
            removeAnim(&zombie->m_attachedEffects, &stun);
            break;
        }
        case zombie_condition_stickybombed:
        {
            std::string stun = "stickystun";
            removeAnim(&zombie->m_attachedEffects, &stun);
            break;
        }
        case zombie_condition_zombossstun:
        {
            std::string zombossstun = "zombossstun";
            removeAnim(&zombie->m_attachedEffects, &zombossstun);
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
        case zombie_condition_sapped:
        case zombie_condition_chill:
        case zombie_condition_stackableslow:
        case zombie_condition_stalled:
        {
            std::string plantslow = "plantslow";
            removeAnim(&zombie->m_attachedEffects, &plantslow);
            break;
        }
        case zombie_condition_hungered:
        case zombie_condition_speedup1:
        case zombie_condition_speedup2:
        case zombie_condition_speedup3:
        case zombie_condition_speedup4:
        case zombie_condition_terrified:
        case zombie_condition_potionspeed1:
        case zombie_condition_potionspeed2:
        case zombie_condition_potionspeed3:
        case zombie_condition_potionsuper1:
        case zombie_condition_potionsuper2:
        case zombie_condition_potionsuper3:
        {
            std::string speedup = "zombiespeedup";
            removeAnim(&zombie->m_attachedEffects, &speedup);
            break;
        }
    }
    
    return oRemoveEffCond(zombie, cond);
}
#pragma endregion
#pragma region Sap Condition Shader Restoration

typedef void(*zombieConditionTrackerUpdate)(ZombieConditionTracker*);
zombieConditionTrackerUpdate oZombieConditionTrackerUpdate = nullptr;
Sexy::Color BlendColor(const Sexy::Color& c1, const Sexy::Color& c2)
{
    Sexy::Color color;
    color.mRed = (c1.mRed * c2.mRed) / 255;
    color.mGreen = (c1.mGreen * c2.mGreen) / 255;
    color.mBlue = (c1.mBlue * c2.mBlue) / 255;
    color.mAlpha = (c1.mAlpha * c2.mAlpha) / 255;
    return color;
}
void hkZombieConditionTrackerUpdate(ZombieConditionTracker* thisPtr)
{
    oZombieConditionTrackerUpdate(thisPtr);

    bool allowColorMix = false;

    if (thisPtr->m_conditionFlags[zombie_condition_sapped])
    {
        Sexy::Color sappedColor(255, 134, 44, 255);

        Sexy::Color mixedColor = BlendColor(thisPtr->m_currentColor, sappedColor);
        
        thisPtr->m_currentColor = mixedColor;

        allowColorMix = true;
    }
    thisPtr->m_states[m_colorMixMode] |= allowColorMix;
}
// Removes the sap shader effect from Red Stinger's PF effect
void PatchRedStingerPF()
{
    uint32_t value = 0x528004A1; // changes sapped to slowdown2
    ReplaceBytes(0xE7DB9C, &value, 4);
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
    PVZ2HookFunction(0x677B40, (void*)hkZombieConditionTrackerUpdate, (void**)&oZombieConditionTrackerUpdate);
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
    ZombieDarkCavalry::modInit();
    ZombieDarkCavalryProps::modInit();
    FrogProjectile::ModInit();
    ZombieAnimRig_FairyTaleWitch::modInit();
    ZombieFairyTaleWitch::modInit();
    ZombieFairyTaleWitchFrog::ModInit();
    ZombieAnimRig_FairyTaleImp::modInit();
    ZombieFairyTaleGargantuarProps::modInit();
    PatchRedStingerPF();
}