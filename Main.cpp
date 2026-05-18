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
#include <PvZ2/ZombieAnimRigBasicTemplateProps.h>
#include <PvZ2/ZombieBasicProps.h>
#include <PvZ2/ZombieModernPoleVaulter.h>
#include <PvZ2/ZombieModernPoleVaulterProps.h>
#include <PvZ2/ZombieAnimRig_ModernPoleVaulter.h>
#include <PvZ2/ZombieModernPogo.h>
#include <PvZ2/ZombieModernPogoProps.h>
#include <PvZ2/ZombieAnimRig_ModernPogo.h>
#include <PvZ2/ZombieModernJackInTheBoxProps.h>
#include <PvZ2/ZombieAnimRig_ModernJackInTheBox.h>
#include <PvZ2/ZombieModernJackInTheBox.h>
#include <PvZ2/GridItem.h>
#include <PvZ2/ZombieCamel.h>
#include "PvZ2/ZombieSpawnerProjectile.h"
#include <PvZ2/ZombieFutureJetpack.h>
#include <PvZ2/ZombieModernScreenDoor.h>
#include <PvZ2/ZombieAnimRig_ModernScreenDoor.h>
#include <PvZ2/ZombieModernScreenDoorProps.h>
#include <PvZ2/ZombieModernAllStar.h>
#include <Sexy/PTXInfo.h>
#include <PvZ2/ZombieEightiesBassProps.h>


// TODO: Make every typedef function became a wrapper ig
#pragma region Alias to ID

class ZombieAlmanac
{
public:
    void* vftable;
    std::map<SexyString, uint> m_aliasToId;
};

class PlantNameMapper
{
public:
    void* vftable;
    std::map<SexyString, uint> m_aliasToId;
};

// used for the custom id system
std::vector<PlantType*> g_modPlantTypenames;
std::vector<ZombieType*> g_modZombieTypenames;

#define REGISTER_PLANT_TYPENAME(typename) \
    g_modPlantTypenames.push_back(typename); \

#define REGISTER_ZOMBIE_TYPENAME(typename) \
    g_modZombieTypenames.push_back(typename); \

typedef void* (*plantTypeCtor)(PlantType*);
plantTypeCtor oPlantTypeCtor = nullptr;

void* hkPlantTypeCtor(PlantType* self)
{
    REGISTER_PLANT_TYPENAME(self);
    return oPlantTypeCtor(self);
}

typedef PlantNameMapper* (*PlantNameMapperCtor)(PlantNameMapper*);
PlantNameMapperCtor oPlantNameMapperCtor = nullptr;
void* hkCreatePlantNameMapper(PlantNameMapper* self)
{
    oPlantNameMapperCtor(self);
    for (size_t iter = 0; iter < g_modPlantTypenames.size(); iter++)
    {
        PlantType* type = g_modPlantTypenames[iter];

        self->m_aliasToId[type->TypeName] = type->IntegerID;
    }
    return self;
}

typedef void* (*zombieTypeCtor)(ZombieType*);
zombieTypeCtor oZombieTypeCtor = nullptr;

void* hkZombieTypeCtor(ZombieType* self)
{
    REGISTER_ZOMBIE_TYPENAME(self);
    return oZombieTypeCtor(self);
}

typedef ZombieAlmanac* (*ZombieAlmanacCtor)(ZombieAlmanac*);
ZombieAlmanacCtor oZombieAlmanacCtor = nullptr;

void* hkCreateZombieTypenameMap(ZombieAlmanac* self)
{
    oZombieAlmanacCtor(self);
    for (size_t iter = 0; iter < g_modZombieTypenames.size(); iter++)
    {
        auto* type = g_modZombieTypenames[iter];
        self->m_aliasToId[type->TypeName] = type->IntegerID;
    }
    return self;
}

#pragma endregion
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
                       transformOffset = { -25.0f, -100.0f, 0.0f };
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
                    transformOffset = { -25.0f, -10.0f, 0.0f };
                }
                else if (props->Size == ZombieSize::chicken) {
                    transformOffset = { -25.0f, 0.0f, 0.0f };
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
        case zombie_condition_potionspeed1:
        case zombie_condition_potionspeed2:
        case zombie_condition_potionspeed3:
        {
            if (zombie->m_attachedEffects.GetObjectIndex("zombiespeedup") == -1) {
                SexyVector3 transformOffset = { 0.0f, -20.0f, 0.0f };
                setAnim(zombie, "zombiespeedup", "POPANIM_EFFECTS_ZOMBIE_SPEEDUP", "zombie_speedup", &transformOffset, 1, false, false, 2);
            }
            break;
        }
        case zombie_condition_potiontoughness1:
        case zombie_condition_potiontoughness2:
        case zombie_condition_potiontoughness3:
        {
            if (zombie->m_attachedEffects.GetObjectIndex("dmgreduction") == -1) {
                SexyVector3 transformOffset;
                if (props->Size == ZombieSize::large) {
                    transformOffset = { 0.0f, -50.0f, 0.0f };
                }
                else if (props->Size == ZombieSize::imp) {
                    transformOffset = { 0.0f, -20.0f, 0.0f };
                }
                else if (props->Size == ZombieSize::chicken) {
                    transformOffset = { 0.0f, -10.0f, 0.0f };
                }
                else {
                    transformOffset = { 0.0f, -30.0f, 0.0f };
                }
                setAnim(zombie, "dmgreduction", "POPANIM_EFFECTS_ZOMBIE_DMG_REDUCTION", "02", &transformOffset, 1, false, false, 2);
            }
            break;
        }

        case zombie_condition_potionsuper1:
        case zombie_condition_potionsuper2:
        case zombie_condition_potionsuper3:
        {
            if (zombie->m_attachedEffects.GetObjectIndex("sreduce") == -1 && zombie->m_attachedEffects.GetObjectIndex("sspeedup") == -1) {
                SexyVector3 transformOffset = { 0.0f, -20.0f, 0.0f };
                setAnim(zombie, "sspeedup", "POPANIM_EFFECTS_ZOMBIE_SPEEDUP", "zombie_speedup", &transformOffset, 1, false, false, 2);
                SexyVector3 transformOffset1;
                if (props->Size == ZombieSize::large) {
                    transformOffset1 = { 0.0f, -50.0f, 0.0f };
                }
                else if (props->Size == ZombieSize::imp) {
                    transformOffset1 = { 0.0f, -20.0f, 0.0f };
                }
                else if (props->Size == ZombieSize::chicken) {
                    transformOffset1 = { 0.0f, -10.0f, 0.0f };
                }
                else {
                    transformOffset1 = { 0.0f, -30.0f, 0.0f };
                }
                setAnim(zombie, "sreduce", "POPANIM_EFFECTS_ZOMBIE_DMG_REDUCTION", "02", &transformOffset1, 1, false, false, 2);
            }
            break;
        }
        case zombie_condition_terrified:
        {
            if (zombie->m_attachedEffects.GetObjectIndex("terrified") == -1) {
                SexyVector3 transformOffset;
                if (props->Size == ZombieSize::large) {
                    transformOffset = { -20.0f, -300.0f, 0.0f };
                }
                else if (props->Size == ZombieSize::imp) {
                    transformOffset = { -20.0f, -50.0f, 0.0f };
                }
                else if (props->Size == ZombieSize::chicken) {
                    transformOffset = { 0.0f, -30.0f, 0.0f };
                }
                else {
                    transformOffset = { -20.0f, -150.0f, 0.0f };
                }
                setAnim(zombie, "terrified", "POPANIM_EFFECTS_ZOMBIE_TERRIFIED", "animation", &transformOffset, 1, false, false, 2);
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
        case zombie_condition_terrified:
        {
            std::string zombossstun = "terrified";
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
typedef bool (*initZombiePianoList)(int64_t, int64_t);
initZombiePianoList oInitZombiePianoList = NULL;

std::vector<SexyString>* g_pianoList = nullptr;
bool g_pianoListInitialized = false;

bool hkInitZombiePianoList(int64_t a1, int64_t a2)
{
    bool result = oInitZombiePianoList(a1, a2);

    if (!g_pianoListInitialized)
    {
        uintptr_t ptrAddr = getActualOffset(0x2581BD0);
        g_pianoList = reinterpret_cast<std::vector<SexyString>*>(ptrAddr);

        if (g_pianoList != nullptr)
        {
            g_pianoList->clear();
            g_pianoList->push_back("cowboy");
            g_pianoList->push_back("cowboy_armor1");
            g_pianoList->push_back("cowboy_armor2");
            g_pianoList->push_back("cowboy_armor4");
            g_pianoList->push_back("cowboy_veteran");
            g_pianoList->push_back("cowboy_armor1_veteran");
            g_pianoList->push_back("cowboy_armor2_veteran");
            g_pianoList->push_back("cowboy_armor4_veteran");
        }

        g_pianoListInitialized = true;
    }

    return result;
}
typedef void (*boardWaveFunc)(Board*, int, int, bool);
boardWaveFunc oBoardWaveFunc = nullptr;
void hkBoardWaveFunc(Board* thisPtr, int waveIndex, int waveType, bool isFinalWave)
{
    oBoardWaveFunc(thisPtr, waveIndex, waveType, isFinalWave);
    if (isFinalWave) {
        AudioMgr::GetInstance()->PostEvent("Play_FinalWave");
    }
}
#pragma endregion 
#pragma region Build Symbol Funcs

Reflection::CRefManualSymbolBuilder::BuildSymbolsFunc PlantType::oPlantTypeBuildSymbols = nullptr;
Reflection::CRefManualSymbolBuilder::ConstructFunc PlantType::oPlantTypeConstruct = nullptr;
Reflection::CRefManualSymbolBuilder::BuildSymbolsFunc ZombieType::oZombieTypeBuildSymbols = nullptr;
Reflection::CRefManualSymbolBuilder::ConstructFunc ZombieType::oZombieTypeConstruct = nullptr;


#pragma endregion
#pragma region Format 150
// thanks jay_krow for format 150 code (yummy)
typedef uint(*getGLTextureTotalSize)(IResStreamDriver*, PTXInfo*);
getGLTextureTotalSize oGetGLTextureTotalSize = nullptr;

uint hkGetGLTextureTotalSize(IResStreamDriver* resStreamDriver, PTXInfo* ptxInfo) {
    if (ptxInfo->format == 150) {
        LOGI("Decode format 150, texture size = %d %d", ptxInfo->width, ptxInfo->height);
        return ptxInfo->width * ptxInfo->height;
    }
    return oGetGLTextureTotalSize(resStreamDriver, ptxInfo);
}

typedef uint(*loadAndDecode)(AndroidDecodeTask*);
loadAndDecode oLoadAndDecode = nullptr;

uint hkLoadAndDecode(AndroidDecodeTask* decodeTask) {
    decodeTask->unkInt40 = 1;

    uintptr_t gSexyAppBase = *(uintptr_t*)getActualOffset(0x2599998);
    uintptr_t pResStreamManager = *(uintptr_t*)(gSexyAppBase + 2136);
    auto resStreamDriver = decodeTask->resStreamDriver;

    if (*(bool*)(pResStreamManager + 18)) {
        typedef void (*func)(IResStreamDriver*);
        func pFunc = (func)getActualOffset(0x176EB64);
        pFunc(resStreamDriver);
        return 1LL;
    }

    PTXInfo* ptxInfo = decodeTask->ptxInfo;
    int ptxFormat = ptxInfo->format;
    if (ptxFormat == 150) {
        typedef void (*LoadGLTex)(IResStreamDriver*, PTXInfo*, uint, uint, uint, unsigned char*, uint, uint, uint);
        LoadGLTex pFunc = (LoadGLTex)getActualOffset(0x176EB70);
        auto ptxSize = resStreamDriver->GetGLTextureTotalSize(ptxInfo);

        auto alphaSize = resStreamDriver->GetGLTextureAlphaSize(ptxInfo);
        auto rgbSize = resStreamDriver->GetGLTextureRGBSize(ptxInfo);

        pFunc(
            resStreamDriver,
            ptxInfo,
            0x9278, // 0x9278 is GL_COMPRESSED_RGBA8_ETC2_EAC
            decodeTask->rgbTextureID,
            decodeTask->alphaTextureID,
            decodeTask->pixels,
            decodeTask->unkInt84 * alphaSize,
            rgbSize,
            ptxSize);

        return 1;
    }
    else {
        int a = oLoadAndDecode(decodeTask);
        return a;
        return oLoadAndDecode(decodeTask);
    }
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
    PVZ2HookFunction(0xC6D080, (void*)hkPlantTypeCtor, (void**)&oPlantTypeCtor);
    PVZ2HookFunction(0x11797B4, (void*)hkCreatePlantNameMapper, (void**)&oPlantNameMapperCtor);
    PVZ2HookFunction(0x10680BC, (void*)hkZombieTypeCtor, (void**)&oZombieTypeCtor);
    PVZ2HookFunction(0x14665C4, (void*)hkCreateZombieTypenameMap, (void**)&oZombieAlmanacCtor);

    PVZ2HookFunction(0xC6D080, (void*)PlantType::construct, (void**)&PlantType::oPlantTypeConstruct);
    PVZ2HookFunction(0xC6BF48, (void*)PlantType::buildSymbols, (void**)&PlantType::oPlantTypeBuildSymbols);
    PVZ2HookFunction(0x10680BC, (void*)ZombieType::construct, (void**)&ZombieType::oZombieTypeConstruct);
    PVZ2HookFunction(0x106828C, (void*)ZombieType::buildSymbols, (void**)&ZombieType::oZombieTypeBuildSymbols); 

    PVZ2HookFunction(0xC4987C, (void*)hkEffectCondition, (void**)&oEffCond);
    PVZ2HookFunction(0xC4BC48, (void*)hkRemoveEffectCondition, (void**)&oRemoveEffCond);
    PVZ2HookFunction(0x677B40, (void*)hkZombieConditionTrackerUpdate, (void**)&oZombieConditionTrackerUpdate);
    PVZ2HookFunction(0xA9E25C, (void*)hkBoardWaveFunc, (void**)&oBoardWaveFunc);
    PVZ2HookFunction(0xC1D1FC, (void*)hkInitZombiePianoList, (void**)&oInitZombiePianoList);
    //PVZ2HookFunction(0x168D580, (void*)hkLoadAndDecode, (void**)&oLoadAndDecode);
    //PVZ2HookFunction(0x176D6CC, (void*)hkGetGLTextureTotalSize, (void**)&oGetGLTextureTotalSize);

    ZombieCamelProps::modInit();
    ZombieCamelTouchProps::modInit();
    ZombiePharaoh::ModInit();// free stuff
    ZombieModernSuperfanImpProps::modInit();// free stuff
    ZombieBullProps::modInit();// free stuff
    ZombieBullVeteranProps::modInit();// free stuff
    ZombieZcorpRacerProps::modInit();// free stuff
    ZombieFairyTaleImp::modInit();
    ZombieFairyTaleImpProps::modInit();
    ZombieAnimRig_ModernAllStar::modInit();// free stuff
    ZombieAnimRig_JourneyToTheWestAllStar::modInit();
    ZombieAnimRig_EightiesPunk::modInit();// free stuff
    ZombieAnimRig_Consultant::modInit();// free stuff
    ZombieAnimRig_JourneyToTheWestTrident::modInit();
    ZombieAnimRig_Gargantuar::modInit();// free stuff
    ZombieAnimRig_FairyTaleGargantuar::modInit();
    ZombieJourneyToTheWestGargantuar::modInit();
    ZombieAnimRig_JourneyToTheWestGargantuar::modInit();
    ZombiePirateBoomBarrel::modInit();// free stuff
    ZombieAnimRig_PirateBoomBarrel::modInit();// free stuff
    DangerRoomFallenKnightDesigner::modInit();// free stuff
    DangerRoomHelpdeskSupportDesigner::modInit();// free stuff
    ZombieModifierProperties::modInit();// free stuff
    ZombieAnimRig_ModernBalloon::modInit();// free stuff
    ZombieJourneyToTheWestBalloon::modInit();   // free stuff
    ZombieJourneyToTheWestBalloonProps::modInit();// free stuff
    ZombieJourneyToTheWestVendor::ModInit();
    ZombieJourneyToTheWestVendorProps::modInit();
    ZombieAnimRig_Vendor::modInit();
    ZombieJourneyToTheWestPiggy::ModInit();
    ZombieJourneyToTheWestPiggyProps::modInit();
    ZombieJourneyToTheWestGargantuarProps::modInit();
    ZombieAnimRigTemplateConfig::modInit();// free stuff
    TimerExplosionProps::modInit();
    ZombieZCorpEnergyDrinker::modInit();
    ZombieZCorpEnergyDrinkerProps::modInit();
    ZombieAnimRig_EnergyDrinker::modInit();
    ZombieDarkCavalry::modInit();// free stuff
    ZombieDarkCavalryProps::modInit();// free stuff
    FrogProjectile::ModInit();
    ZombieAnimRig_FairyTaleWitch::modInit();
    ZombieFairyTaleWitch::modInit();
    ZombieFairyTaleWitchFrog::ModInit();
    ZombieAnimRig_FairyTaleImp::modInit();
    ZombieFairyTaleGargantuar::modInit();
    ZombieFairyTaleGargantuarProps::modInit();
    ZombieBasicTemplate::modInit();// free stuff
    ZombieBasicProps::modInit();// free stuff
    ZombieAnimRig_BasicTemplate::modInit();// free stuff
    ZombieCowboyBasicVeteran::modInit();
    ZombieCowboyVeteranProps::modInit();
    ZombieModernPoleVaulter::ModInit();
    ZombieAnimRig_ModernPoleVaulter::modInit();
    ZombieModernPoleVaulterProps::modInit();
    ZombieModernJackInTheBoxProps::modInit();
    ZombieAnimRig_ModernJackInTheBox::modInit();
    ZombieModernJackInTheBox::ModInit();
    ZombieModernMinerProps::modInit();
    ZombieAnimRig_ModernMiner::modInit();
    ZombieModernMiner::modInit();
    ZombieVaseGargantuar::modInit();
    ZombieSpawnerProjectile::ModInit();
    ZombieSpawnerProjectileProps::ModInit();
    ZombieFutureJetpackVeteran::modInit();
    ZombieFutureJetpackVeteranProps::modInit();
    ZombieModernPogo::ModInit();
    ZombieAnimRig_ModernPogo::modInit();
    ZombieModernPogoProps::modInit();
    ZombieModernScreenDoor::ModInit();
    ZombieAnimRig_ModernScreenDoor::modInit();
    ZombieAnimRig_ModernScreenDoorAlmanac::modInit();
    ZombieModernScreenDoorProps::modInit();
    ZombieModernBerserkerProps::modInit();
    ZombieModernBerserker::ModInit();
    ZombieEightiesBassProps::modInit();
    PatchRedStingerPF();
}