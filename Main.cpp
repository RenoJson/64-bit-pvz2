#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <unistd.h>

#include "Main.h"

#include "PvZ2/Board.h"
#include "PvZ2/Effect_PopAnim.h"
#include "Sexy/LawnApp.h"
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
#include <PvZ2/ZombieEightiesBass.h>
#include <PvZ2/ZombieAnimRig_EightiesBass.h>
#include <PvZ2/PlantChomper.h>
#include <PvZ2/ChomperProps.h>
#include <PvZ2/ZombieAnimRig_ModernDolphinRider.h>
#include <PvZ2/ZombieModernDolphinRider.h>
#include <PvZ2/ZombieModernDolphinRiderProps.h>
#include <PvZ2/GridItemMausoleumLawnTile.h>
#include <PvZ2/ZombieMausoleumCavalryProps.h>
#include <PvZ2/ZombieMausoleumCavalry.h>
#include <PvZ2/ZombieMausoleumVendorProps.h>
#include <PvZ2/ZombieMausoleumPiggyProps.h>
#include <PvZ2/ZombieMausoleumPiggy.h>
#include <PvZ2/ZombieMausoleumVendor.h>
#include <PvZ2/ZombieMausoleumBasic.h>
#include <PvZ2/ZombieMausoleumAssasin.h>
#include <PvZ2/ZombieAnimRig_MausoleumAssasin.h>
#include <PvZ2/ZombieMausoleumArcher.h>
#include <PvZ2/ZombieAnimRig_MausoleumArcher.h>
#include <PvZ2/ZombieMausoleumDrunkard.h>
#include <PvZ2/ZombieAnimRig_MausoleumDrunkard.h>
#include <PvZ2/PlantUnchartedHaystack.h>
#include <PvZ2/DamageInfo.h>
#include <PvZ2/ZombieHelper.h>
#include <PvZ2/ZombieAnimRig_MausoleumCursedBase.h>
#include <PvZ2/TombraiserProjectileProps.h>
#include <PvZ2/GridItemSpeakerBass.h>
#include <PvZ2/ZombieTypeTemplate.h>
#include <PvZ2/ZombieModernDancer.h>
#include <PvZ2/ZombieModernBungee.h>
#include <PvZ2/ZombieModernBungeeTarget.h>
#include <PvZ2/ZombieCatapult.h>


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
    auto* props = reinterpret_cast<ZombiePropertySheet*>(zombie->m_propertySheet.Get());
    auto rig = reinterpret_cast<ZombieAnimRig*>(zombie->m_animRig.Get());
    switch (cond)
    {
        case zombie_condition_dazeystunned:
        case zombie_condition_stun:
        {
           if (zombie->m_attachedEffects.GetObjectIndex("stickystun") == -1) {
               if (zombie->m_attachedEffects.GetObjectIndex("stun") == -1) {
                   SexyVector2 headOffset = CallVirtualFunc<SexyVector2>(rig, 43);
                   SexyVector3 transformOffset;
                   if (props->Size == ZombieSize::large) {
                       transformOffset = { headOffset.x -25.0f, headOffset.y -100.0f, 0.0f };
                   }
                   else if (props->Size == ZombieSize::imp) {
                       transformOffset = { headOffset.x -25.0f, headOffset.y -10.0f, 0.0f };
                   }
                   else if (props->Size == ZombieSize::chicken) {
                       transformOffset = { headOffset.x -25.0f, headOffset.y, 0.0f };
                   }
                   else {
                       transformOffset = { headOffset.x -15.0f, headOffset.y -20.0f, 0.0f };
                   }
                   ZombieAttachEffect(zombie, "stun", "POPANIM_EFFECTS_ZOMBIES_STUN_EFFECT", "stun_fx", transformOffset, 1, false, false, 2);
               }
           }
           break;
        }
        case zombie_condition_stickybombed:
        {
            if (zombie->m_attachedEffects.GetObjectIndex("stickystun") == -1) {
                SexyVector2 headOffset = CallVirtualFunc<SexyVector2>(rig, 43);
                SexyVector3 transformOffset;
                if (props->Size == ZombieSize::large) {
                    transformOffset = { headOffset.x - 25.0f, headOffset.y - 100.0f, 0.0f };
                }
                else if (props->Size == ZombieSize::imp) {
                    transformOffset = { headOffset.x - 25.0f, headOffset.y - 10.0f, 0.0f };
                }
                else if (props->Size == ZombieSize::chicken) {
                    transformOffset = { headOffset.x - 25.0f, headOffset.y, 0.0f };
                }
                else {
                    transformOffset = { headOffset.x - 15.0f, headOffset.y - 20.0f, 0.0f };
                }
                ZombieAttachEffect(zombie, "stickystun", "POPANIM_EFFECTS_ZOMBIES_STUN_EFFECT", "stun_fx", transformOffset, 1, false, false, 2);
            }
            break;
        }
        case zombie_condition_zombossstun:
        {
            if (zombie->m_attachedEffects.GetObjectIndex("zombossstun") == -1) {
                SexyVector3 transformOffset = { 60.0f, -200.0f, 0.0f };
                ZombieAttachEffect(zombie, "zombossstun", "POPANIM_EFFECTS_ZOMBOSS_STUN_EFFECT", "stun", transformOffset, 1, false, false, 2);
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
                ZombieAttachEffect(zombie, "slow", "POPANIM_EFFECTS_ZOMBIE_SLOWDOWN", "anim", transformOffset, 1, false, false, 2);
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
                    ZombieAttachEffect(zombie, "plantslow", "POPANIM_EFFECTS_ZOMBIE_SLOWDOWN", "anim", transformOffset, 1, false, false, 2);
                }
            }
            break;
        }
        case zombie_condition_hungered:
        case zombie_condition_speedup1:
        case zombie_condition_speedup2:
        case zombie_condition_speedup3:
        case zombie_condition_speedup4:
        {
            if (zombie->m_attachedEffects.GetObjectIndex("zombiespeedup") == -1) {
                SexyVector3 transformOffset = { 0.0f, -20.0f, 0.0f };
                ZombieAttachEffect(zombie, "zombiespeedup", "POPANIM_EFFECTS_ZOMBIE_SPEEDUP", "zombie_speedup", transformOffset, 1, false, false, 2);
            }
            break;
        }
        case zombie_condition_potionspeed1:
        case zombie_condition_potionspeed2:
        case zombie_condition_potionspeed3:
        case zombie_condition_potiontoughness1:
        case zombie_condition_potiontoughness2:
        case zombie_condition_potiontoughness3:
        case zombie_condition_potionsuper1:
        case zombie_condition_potionsuper2:
        case zombie_condition_potionsuper3:
        {
            if (zombie->m_attachedEffects.GetObjectIndex("potion") == -1) {
                SexyVector3 transformOffset = { 20.0f, -30.0f, 0.0f };
                ZombieAttachEffect(zombie, "potion", "POPANIM_EFFECTS_ZOMBIE_POTION_EFFECT", "idle", transformOffset, -1, false, false, 2);
            }
            break;
        }

        case zombie_condition_terrified:
        {
            if (zombie->m_attachedEffects.GetObjectIndex("terrified") == -1) {
                SexyVector2 headOffset = CallVirtualFunc<SexyVector2>(rig, 43);
                SexyVector3 transformOffset;
                if (props->Size == ZombieSize::large) {
                    transformOffset = { headOffset.x - 25.0f, headOffset.y - 100.0f, 0.0f };
                }
                else if (props->Size == ZombieSize::imp) {
                    transformOffset = { headOffset.x - 25.0f, headOffset.y - 10.0f, 0.0f };
                }
                else if (props->Size == ZombieSize::chicken) {
                    transformOffset = { headOffset.x - 25.0f, headOffset.y, 0.0f };
                }
                else {
                    transformOffset = { headOffset.x - 15.0f, headOffset.y - 20.0f, 0.0f };
                }
                ZombieAttachEffect(zombie, "terrified", "POPANIM_EFFECTS_ZOMBIE_TERRIFIED", "animation", transformOffset, 1, false, false, 2);
            }
            break;
        }
    }
    return oEffCond(zombie, cond);
}
typedef Zombie* (*removeeffectCondition)(Zombie*, ZombieConditions);
removeeffectCondition oRemoveEffCond = nullptr;
Zombie* hkRemoveEffectCondition(Zombie* zombie, ZombieConditions cond) {
    switch (cond) {
        case zombie_condition_dazeystunned:
        case zombie_condition_stun:
        {
            RemoveAttachedEffect(&zombie->m_attachedEffects, "stun");
            break;
        }
        case zombie_condition_stickybombed:
        {
            RemoveAttachedEffect(&zombie->m_attachedEffects, "stickystun");
            break;
        }
        case zombie_condition_zombossstun:
        {
            RemoveAttachedEffect(&zombie->m_attachedEffects, "zombossstun");
            break;
        }
        case zombie_condition_terrified:
        {
            RemoveAttachedEffect(&zombie->m_attachedEffects, "terrified");
            break;
        }
        case zombie_condition_speeddown1:
        case zombie_condition_speeddown2:
        case zombie_condition_speeddown3:
        case zombie_condition_speeddown4:
        {
            RemoveAttachedEffect(&zombie->m_attachedEffects, "slow");
            break;
        }
        case zombie_condition_sapped:
        case zombie_condition_chill:
        case zombie_condition_stackableslow:
        case zombie_condition_stalled:
        {
            RemoveAttachedEffect(&zombie->m_attachedEffects, "plantslow");
            break;
        }
        case zombie_condition_hungered:
        case zombie_condition_speedup1:
        case zombie_condition_speedup2:
        case zombie_condition_speedup3:
        case zombie_condition_speedup4:
        {
            RemoveAttachedEffect(&zombie->m_attachedEffects, "zombiespeedup");
            break;
        }
        case zombie_condition_potionspeed1:
        case zombie_condition_potionspeed2:
        case zombie_condition_potionspeed3:
        case zombie_condition_potiontoughness1:
        case zombie_condition_potiontoughness2:
        case zombie_condition_potiontoughness3:
        case zombie_condition_potionsuper1:
        case zombie_condition_potionsuper2:
        case zombie_condition_potionsuper3:
        {
            RemoveAttachedEffect(&zombie->m_attachedEffects, "potion");
            if (zombie->m_attachedEffects.GetObjectIndex("potion_end") == -1) {
                SexyVector3 transformOffset = { 20.0f, -30.0f, 0.0f };
                ZombieAttachEffect(zombie, "potion_end", "POPANIM_EFFECTS_ZOMBIE_POTION_EFFECT", "over", transformOffset, -1, true, false, 2);
            }
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

#pragma region Format 150
// thanks jay_krow for format 150 code (yummy)
typedef uint(*getGLTextureTotalSize)(IResStreamDriver*, PTXInfo*);
getGLTextureTotalSize oGetGLTextureTotalSize = nullptr;

uint hkGetGLTextureTotalSize(IResStreamDriver* resStreamDriver, PTXInfo* ptxInfo) {
    if (ptxInfo->format == 150) {
        //LOGI("Decode format 150, texture size = %d %d", ptxInfo->width, ptxInfo->height);
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
#pragma region Seed Chooser Camera Fix
enum AspectRatio
{
    Letterbox,
    Widescreen,
    Ultrawide,
};

AspectRatio GetAspectRatio()
{
    float ratio = (float)LawnApp::GetInstance()->mWidth / LawnApp::GetInstance()->mHeight;
    if (ratio <= 1.4f)
    {
        return Letterbox;
    }
    else if (ratio >= 1.41f && ratio <= 1.85f)
    {
        return Widescreen;
    }
    else if (ratio >= 1.86f)
    {
        return Ultrawide;
    }
}
typedef void(*BoardRender)(Board*);
BoardRender oBoardRender = nullptr;
void hkBoardRender(Board* thisPtr)
{
    oBoardRender(thisPtr);
    if (GetAspectRatio() == Ultrawide)
    {
        float adjustedResolution = thisPtr->m_backdropResolutionX - (thisPtr->m_backdropResolutionX * -0.41f);
        thisPtr->m_backdropCameraRenderScale = 1.22f;
        thisPtr->m_backdropResolutionX = adjustedResolution;
        return;
    }
}
#pragma endregion
typedef void (*Fire)(BoardEntity*, int64_t);
Fire oFire = nullptr;
void hkFire(BoardEntity* thisPtr, int64_t flag)
{
    bool isFire = (flag & DamageTypeFlags::damage_fire) != 0;
    bool isRemoved = (flag & DamageTypeFlags::damage_removed_from_board) != 0;
    if (isFire && isRemoved)
    {
        int posX = static_cast<int>((thisPtr->m_position.x - 200.0f) / 64.0f);
        int posY = static_cast<int>((thisPtr->m_position.y - 160.0f) / 76.0f);
        SexyVector3 entityPos;
        entityPos.x = static_cast<float>((posX * 64.0f) + 232.0f);
        entityPos.y = static_cast<float>((posY * 76.0f) + 222.0f);
		entityPos.z = 0.0f;
        Board* boardPtr = Board::GetBoard();
        auto type = Effect_PopAnim::StaticGetType();
        auto popAnim = CallFunc<Effect_PopAnim*, Board*, Sexy::RtClass*>(0xAA1EFC, boardPtr, type);
        popAnim->SetFromResource("POPANIM_EFFECTS_ZOMBIE_FRISTKING_DRUNK_GHOST_EFFECT");
        popAnim->SetFocusFracAndScale(1);

        popAnim->SetPositionAndLayer(entityPos, -1);
        popAnim->PlayAnimation("fire", 0);
        ZombiePlaySoundEvent(thisPtr, "Play_Zomb_Egypt_Explorer_Mvmt_Burn", 0.0f);
        flag &= ~DamageTypeFlags::damage_fire;
    }
    oFire(thisPtr, flag);
}


typedef void* (*ZTakeDmg)(Zombie*, DamageInfo*);
ZTakeDmg oZTakeDmg = nullptr;

void* hkTakeDamageNoCorpse(Zombie* thisPtr, DamageInfo* damageInfo)
{
    for (size_t i = 0; i < thisPtr->m_armor.size(); i++)
    {
        Armor* armor = thisPtr->m_armor[i].Get();

        if (armor != nullptr && !armor->m_destroyed && armor->m_health > 0)
        {
            auto* armorProps = reinterpret_cast<ArmorPropertySheet*>(armor->m_propertySheetPtr.Get());

            if (armorProps != nullptr)
            {
                bool hasAbsorbOverflow = false;

                for (size_t j = 0; j < armorProps->ArmorFlags.size(); j++)
                {
                    if (armorProps->ArmorFlags[j] == ArmorTypeFlags::absorboverflow)
                    {
                        hasAbsorbOverflow = true;
                        break;
                    }
                }

                if (hasAbsorbOverflow)
                {
                    if (damageInfo->m_damage >= armor->m_health)
                    {
                        damageInfo->m_flags &= ~DamageTypeFlags::damage_bypass_shield;
                        damageInfo->m_flags &= ~DamageTypeFlags::damage_hits_shield_and_body;

                        damageInfo->m_flags &= ~DamageTypeFlags::damage_hits_only_shield;

                        float calculatedDamage = armor->m_health;

                        if (thisPtr->m_damageScale > 0.001f)
                        {
                            calculatedDamage /= thisPtr->m_damageScale;
                        }

                        bool isShrunken = CallFunc<bool, Zombie*, int>(0xC3E44C, thisPtr, zombie_condition_shrinking)
                            || CallFunc<bool, Zombie*, int>(0xC3E44C, thisPtr, zombie_condition_shrunken);

                        if (isShrunken && thisPtr->m_shrunkenDamageScale > 0.001f)
                        {
                            calculatedDamage /= thisPtr->m_shrunkenDamageScale;
                        }
                        damageInfo->m_damage = calculatedDamage;
                    }
                }
            }
            break;
        }
    }
    return oZTakeDmg(thisPtr, damageInfo);
}
typedef void (*TeleportatoMineTeleport)(void*, RtWeakPtr<Zombie>*);
TeleportatoMineTeleport oTeleportatoMineTeleport = nullptr;
void hkFixTeleportatoMineTeleport(void* a1, RtWeakPtr<Zombie>* a2) {
    oTeleportatoMineTeleport(a1, a2);
    RtWeakPtr<Zombie> zombiePtr;
    zombiePtr.FromOther(a2);
    auto zombie = reinterpret_cast<Zombie*>(zombiePtr.Get());
    CallFunc<Zombie*>(0xC4BC48, zombie, zombie_condition_stun);
}

typedef ZombieAnimRig* (*CreateAnimRig)(ZombieType*);
CreateAnimRig oCreateAnimRig = nullptr;
//5AB098 is where the rig initialize in almanac
ZombieAnimRig* hkCreateAnimRig(ZombieType* thisPtr)
{
    if (!thisPtr->IsType(ZombieTypeTemplate::StaticGetType())) {
        return oCreateAnimRig(thisPtr);
    }
    else {
        auto templateType = static_cast<ZombieTypeTemplate*>(thisPtr);
       
        auto popAnimResPtr = ResourceManager::GetPopAnimByName(templateType->PopAnim);
        auto popAnimRes = popAnimResPtr.Get();
       
        auto rigClass = CallFunc<Sexy::RtClass*>(0x163A83C, templateType->AnimRigClass.c_str());
       
        auto zombieRig = CallFunc<ZombieAnimRig*>(0x9DA144, popAnimRes, rigClass);
        
        if (zombieRig != nullptr)
        {
            if (zombieRig->IsType(ZombieAnimRig_BasicTemplate::StaticGetType()))
            {
                auto templateRig = static_cast<ZombieAnimRig_BasicTemplate*>(zombieRig);
                auto rigProps = reinterpret_cast<ZombieAnimRigTemplateConfig*>(templateType->AnimRigProps.Get());
                

                    templateRig->m_LowerArmLayers = rigProps->LowerArmLayers;
                    templateRig->m_UpperArmLayers = rigProps->UpperArmLayers;
                    templateRig->m_HeadLayers = rigProps->HeadLayers;
                    templateRig->m_IdleAnimName = rigProps->IdleAnimName;
                    templateRig->m_WalkAnimName = rigProps->WalkAnimName;
                    templateRig->m_EatAnimName = rigProps->EatAnimName;
                    templateRig->m_DieAnimName = rigProps->DieAnimName;
                    templateRig->m_ParticleArmSpriteName = rigProps->ParticleArmSpriteName;
                    templateRig->m_ParticleHeadSpriteName = rigProps->ParticleHeadSpriteName;
                    CallVirtualFunc<void>(templateRig, 21);
                    CallFunc<void>(0x8DE21C, templateRig, &templateType->AnimRigClass);
                    return templateRig;
               
                
            }
            else {
                LOGI("[hkCreateAnimRig] ZombieType %s has AnimRigClass not derive from ZombieAnimRig_BasicTemplate",
                    templateType->TypeName.c_str());
            }
            CallFunc<void>(0x8DE21C, zombieRig, &templateType->AnimRigClass);
        }
        return zombieRig;
    }
}
typedef ZombieAnimRig* (*AlmanacCreateAnimRig)(ZombieType*, bool, bool);
AlmanacCreateAnimRig oAlmanacCreateAnimRig = nullptr;

ZombieAnimRig* hkAlmanacCreateAnimRig(ZombieType* thisPtr, bool a2, bool a3)
{
    ZombieAnimRig* zombieRig = oAlmanacCreateAnimRig(thisPtr, a2, a3);

    if (zombieRig != nullptr && thisPtr->IsType(ZombieTypeTemplate::StaticGetType()))
    {
        auto templateType = static_cast<ZombieTypeTemplate*>(thisPtr);

        if (zombieRig->IsType(ZombieAnimRig_BasicTemplate::StaticGetType()))
        {
            auto templateRig = static_cast<ZombieAnimRig_BasicTemplate*>(zombieRig);
            auto rigProps = reinterpret_cast<ZombieAnimRigTemplateConfig*>(templateType->AnimRigProps.Get());
            auto props = reinterpret_cast<ZombiePropertySheet*>(templateType->Properties.Get());
            if (rigProps != nullptr)
            {
                templateRig->m_LowerArmLayers = rigProps->LowerArmLayers;
                templateRig->m_UpperArmLayers = rigProps->UpperArmLayers;
                templateRig->m_HeadLayers = rigProps->HeadLayers;

                templateRig->m_IdleAnimName = rigProps->IdleAnimName;
                templateRig->m_WalkAnimName = rigProps->WalkAnimName;
                templateRig->m_EatAnimName = rigProps->EatAnimName;
                templateRig->m_DieAnimName = rigProps->DieAnimName;
                templateRig->m_ParticleArmSpriteName = rigProps->ParticleArmSpriteName;
                templateRig->m_ParticleHeadSpriteName = rigProps->ParticleHeadSpriteName;

                CallVirtualFunc<void>(templateRig, 21);
                CallFunc<void>(0x8DE21C, templateRig, &templateType->AnimRigClass);
                CallVirtualFunc<void>(templateRig, 52, &props->ZombieArmorProps);
            }
        }
        else
        {
            LOGI("[hkAlmanacCreateAnimRig] ZombieType %s has AnimRigClass not derive from ZombieAnimRig_BasicTemplate",
                templateType->TypeName.c_str());
        }
    }

    return zombieRig;
}

void hkNewspaperOnArmorDestroyed(Zombie* thisPtr, int armorIndex, const SexyString& armorName)
{
    if (armorName == "Newspaper" && !ZombieIsDeadOrDying(thisPtr) && thisPtr->m_entityState.m_id != 3)
    {
        ZombieEnterState(thisPtr, 16, 0);
	}
}

void hkExcavatorOnArmorDestroyed(Zombie* thisPtr, int armorIndex, const SexyString& armorName)
{
	auto rig = reinterpret_cast<ZombieAnimRig_LostCityExcavator*>(thisPtr->m_animRig.Get());
    rig->m_hasShovel = false;
    if (armorName == "Shovel" && !ZombieIsDeadOrDying(thisPtr) && thisPtr->m_entityState.m_id != 3)
    {
        ZombieEnterState(thisPtr, 17, 0);
    }
}


typedef void (*PharaohCallback)(ZombiePharaoh*);
PharaohCallback oPharaohCallback = nullptr;
void hkPharaohCallback(ZombiePharaoh* thisPtr)
{
    if ((thisPtr->m_zombieFlags & 4) != 0)
    {
        ZombieEnterState(thisPtr, 4, 0);
    }
    else {
        oPharaohCallback(thisPtr);
    }
}

typedef void (*KingActionCommand)(ZombieDarkKing*, SexyString*, SexyString*, SexyString*, float);
KingActionCommand oKingActionCommand = nullptr;

void hkKingActionCommand(ZombieDarkKing* thisPtr, SexyString* currentAnim, SexyString* actionName, SexyString* param, float nextFrameTime)
{
    // apply the effect first then call the original function
    if (*actionName == "knight") {
        auto knightTarget = reinterpret_cast<Zombie*>(thisPtr->m_knightingTarget.Get());
        auto targetRig = reinterpret_cast<ZombieAnimRig*>(knightTarget->m_animRig.Get());
        if (targetRig->IsType(ZombieAnimRig_Basic::StaticGetType())) {
            auto basicRig = static_cast<ZombieAnimRig_Basic*>(targetRig);
            basicRig->m_helmType = HelmType::crown;
        }
    }
    oKingActionCommand(thisPtr, currentAnim, actionName, param, nextFrameTime);
}

void hkHealHelm(Zombie* thisPtr) {
    bool hasArmorUpdated = false; 
    auto rig = reinterpret_cast<ZombieAnimRig*>(thisPtr->m_animRig.Get());
    auto props = reinterpret_cast<ZombiePropertySheet*>(thisPtr->m_propertySheet.Get());
    DamageInfo dmg;
    dmg.m_damage = 0.0f;
    dmg.m_attacker = thisPtr;
    for (auto& armorWeakPtr : thisPtr->m_armor)
    {
        if (armorWeakPtr.IsValid())
        {
            Armor* armor = reinterpret_cast<Armor*>(armorWeakPtr.Get());

            if (armor != nullptr && !armor->m_destroyed)
            {
                armor->m_health = armor->m_maxHealth;
                armor->m_damageState = 0;
                    if (armor->m_propertySheetPtr.IsValid())
                    {
                        auto armorProps = reinterpret_cast<ArmorPropertySheet*>(armor->m_propertySheetPtr.Get());

                        int layerIndex = 0;
                        for (const auto& layerName : armorProps->ArmorLayers)
                        {
                            if (layerIndex == 0) {
                                SetAnimLayerVisible(rig, layerName, true);
                            }
                            else {
                                SetAnimLayerVisible(rig, layerName, false);
                            }
                            layerIndex++;
                        }
                    }
                
                hasArmorUpdated = true;
            }
        }
    }
    if (hasArmorUpdated) {
        CallVirtualFunc<void, void*>(thisPtr, 186, &dmg);
        
    }
}
typedef void (*ZombieHypnotize)(Zombie*, bool, Plant*, float);
ZombieHypnotize oZombieHypnotize = nullptr;
void hkZombieHypnotized(Zombie* zombie, bool healOnHypnotism, Plant* instigator, float healPercent) {
    oZombieHypnotize(zombie, healOnHypnotism, instigator, healPercent);
    hkHealHelm(zombie);
}
#pragma region Build Symbol Funcs

Reflection::CRefManualSymbolBuilder::BuildSymbolsFunc PlantType::oPlantTypeBuildSymbols = nullptr;
Reflection::CRefManualSymbolBuilder::ConstructFunc PlantType::oPlantTypeConstruct = nullptr;
Reflection::CRefManualSymbolBuilder::BuildSymbolsFunc ZombieType::oZombieTypeBuildSymbols = nullptr;
Reflection::CRefManualSymbolBuilder::ConstructFunc ZombieType::oZombieTypeConstruct = nullptr;
Reflection::CRefManualSymbolBuilder::BuildSymbolsFunc ZombiePropertySheet::oZombiePropsBuildSymbols = nullptr;
Reflection::CRefManualSymbolBuilder::ConstructFunc ZombiePropertySheet::oZombiePropsConstruct = nullptr;


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
    PVZ2HookFunction(0xC136A4, (void*)ZombiePropertySheet::construct, (void**)&ZombiePropertySheet::oZombiePropsConstruct);
    PVZ2HookFunction(0xDA69D4, (void*)ZombiePropertySheet::buildSymbols, (void**)&ZombiePropertySheet::oZombiePropsBuildSymbols);

    PVZ2HookFunction(0xC4987C, (void*)hkEffectCondition, (void**)&oEffCond);
    PVZ2HookFunction(0xC4BC48, (void*)hkRemoveEffectCondition, (void**)&oRemoveEffCond);
    PVZ2HookFunction(0x677B40, (void*)hkZombieConditionTrackerUpdate, (void**)&oZombieConditionTrackerUpdate);
    PVZ2HookFunction(0xA9E25C, (void*)hkBoardWaveFunc, (void**)&oBoardWaveFunc);
    PVZ2HookFunction(0xC1D1FC, (void*)hkInitZombiePianoList, (void**)&oInitZombiePianoList);
    PVZ2HookFunction(0xAA0C40, (void*)hkBoardRender, (void**)&oBoardRender);
    PVZ2HookFunction(0x1273244, (void*)hkFire, (void**)&oFire);
    PVZ2HookFunction(0x1001C04, (void*)hkFixTeleportatoMineTeleport, (void**)&oTeleportatoMineTeleport);
    PVZ2HookFunction(0xBBE3A4, (void*)hkNewspaperOnArmorDestroyed, nullptr);
    PVZ2HookFunction(0xBA2388, (void*)hkExcavatorOnArmorDestroyed, nullptr);
    PVZ2HookFunction(0xB1F5CC, (void*)hkPharaohCallback, (void**)&oPharaohCallback);
    PVZ2HookFunction(0xAFAF74, (void*)hkKingActionCommand, (void**)&oKingActionCommand);
    PVZ2HookFunction(0xC41D3C, (void*)hkZombieHypnotized, (void**)&oZombieHypnotize);
    //PVZ2HookFunction(0xC43B90, (void*)hkTakeDamageNoCorpse, (void**)&oZTakeDmg);
    PVZ2HookFunction(0x1069AC4, (void*)hkCreateAnimRig, (void**)&oCreateAnimRig);
    PVZ2HookFunction(0x5AB098, (void*)hkAlmanacCreateAnimRig, (void**)&oAlmanacCreateAnimRig);
    PVZ2HookFunction(0xC47450, (void*)hkHealHelm, nullptr);
    PVZ2HookFunction(0x168D580, (void*)hkLoadAndDecode, (void**)&oLoadAndDecode);
    PVZ2HookFunction(0x176D6CC, (void*)hkGetGLTextureTotalSize, (void**)&oGetGLTextureTotalSize);

    ProjectileActions::modInit(); 
    ZombieTypeTemplate::modInit();
    ZombieModernSuperfanImpProps::modInit();// free stuff
    ZombieBullProps::modInit();// free stuff
    ZombieBullVeteranProps::modInit();// free stuff
    ZombieZcorpRacerProps::modInit();// free stuff
    ZombieAnimRig_ModernAllStar::modInit();// free stuff
    ZombieAnimRig_EightiesPunk::modInit();// free stuff
    ZombieAnimRig_Consultant::modInit();// free stuff
    ZombieAnimRig_Gargantuar::modInit();// free stuff
    ZombiePirateBoomBarrel::modInit();// free stuff
    ZombieAnimRig_PirateBoomBarrel::modInit();// free stuff
    DangerRoomFallenKnightDesigner::modInit();// free stuff
    DangerRoomHelpdeskSupportDesigner::modInit();// free stuff
    ZombieModifierProperties::modInit();// free stuff
    ZombieAnimRig_ModernBalloon::modInit();// free stuff
    ZombieJourneyToTheWestBalloon::modInit();   // free stuff
    ZombieJourneyToTheWestBalloonProps::modInit();// free stuff
    ZombieAnimRigTemplateConfig::modInit();// free stuff
    ZombieDarkCavalry::modInit();// free stuff
    ZombieDarkCavalryProps::modInit();// free stuff
    ZombieBasicTemplate::modInit();// free stuff
    ZombieBasicProps::modInit();// free stuff
    ZombieAnimRig_BasicTemplate::modInit();// free stuff
    ZombieModernPoleVaulter::ModInit();// free stuff
    ZombieAnimRig_ModernPoleVaulter::modInit();// free stuff
    ZombieModernPoleVaulterProps::modInit();// free stuff
    ZombieModernBerserkerProps::modInit(); //free stuff
    ZombieModernBerserker::ModInit(); //free stuff
    ZombieLostCityTorchGargantuar::modInit();// free stuff
    ZombieLostCityGargantuarProps::modInit();// free stuff
    ZombieMysticFormation::modInit();
    ZombieMysticFormationProps::modInit();
    ZombieAnimRig_MysticFormation::modInit();
    PatchRedStingerPF();// free stuff
    
    
    ZombieModernScreenDoor::ModInit();
    ZombieAnimRig_ModernScreenDoor::modInit();
    ZombieAnimRig_ModernScreenDoorAlmanac::modInit();
    ZombieModernScreenDoorProps::modInit();
    ZombieEightiesBassProps::modInit();
    ZombieEightiesBass::ModInit();
    ZombieAnimRig_EightiesBass::modInit();
    ZombieModernDolphinRider::ModInit();
    ZombieModernDolphinRiderProps::modInit();
    ZombieAnimRig_ModernDolphinRider::modInit();
    GridItemSpeakerBass::modInit();
    GridItemSpeakerBassProps::modInit();
    ZombiePharaoh::ModInit();
    ZombieCamelProps::modInit();
    ZombieCamelTouchProps::modInit();
    ZombieFairyTaleImp::modInit();
    ZombieFairyTaleImpProps::modInit();
    ZombieAnimRig_JourneyToTheWestAllStar::modInit();
    ZombieAnimRig_JourneyToTheWestTrident::modInit();
    ZombieAnimRig_FairyTaleGargantuar::modInit();
    ZombieJourneyToTheWestGargantuar::modInit();
    ZombieAnimRig_JourneyToTheWestGargantuar::modInit();
    ZombieJourneyToTheWestVendor::ModInit();
    ZombieJourneyToTheWestVendorProps::modInit();
    ZombieAnimRig_Vendor::modInit();
    ZombieJourneyToTheWestPiggy::ModInit();
    ZombieJourneyToTheWestPiggyProps::modInit();
    ZombieJourneyToTheWestGargantuarProps::modInit();
    TimerExplosionProps::modInit();
    ZombieZCorpEnergyDrinker::modInit();
    ZombieZCorpEnergyDrinkerProps::modInit();
    ZombieAnimRig_EnergyDrinker::modInit();
    FrogProjectile::ModInit();
    ZombieAnimRig_FairyTaleWitch::modInit();
    ZombieFairyTaleWitch::modInit();
    ZombieFairyTaleWitchFrog::ModInit();
    ZombieAnimRig_FairyTaleImp::modInit();
    ZombieFairyTaleGargantuar::modInit();
    ZombieFairyTaleGargantuarProps::modInit();
    ZombieCowboyBasicVeteran::modInit();
    ZombieCowboyVeteranProps::modInit();
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
    PlantDinoGrass::modInit();
	DinoGrassProps::modInit();
    GridItemMausoleumLawnTile::modInit();
    GridItemMausoleumLawnPath::modInit();
    GridItemMausoleumLawnTileProps::modInit();
    GridItemMausoleumLawnPathProps::modInit();
    ZombieMausoleumCavalry::ModInit();
    ZombieMausoleumCavalryProps::modInit();
    ZombieMausoleumVendor::ModInit();
    ZombieMausoleumVendorProps::modInit();
    ZombieMausoleumPiggy::ModInit();
    ZombieMausoleumPiggyProps::modInit();
    ZombieMausoleumSpirit::modInit();
    ZombieMausoleumBasic::modInit();
    ZombieMausoleumBasicProps::modInit();
    ZombieAnimRig_MausoleumBasic::modInit();
    ZombieMausoleumAssasin::modInit();
    ZombieMausoleumAssasinProps::modInit();
    ZombieAnimRig_MausoleumAssasin::modInit();
    ZombieMausoleumDrunkard::modInit();
    ZombieMausoleumDrunkardProps::modInit();
    ZombieAnimRig_MausoleumDrunkard::modInit();
    ZombieMausoleumArcher::modInit();
    ZombieMausoleumArcherProps::modInit();
    ZombieAnimRig_MausoleumArcher::modInit();
    PlantUnchartedHaystack::modInit();
    GridItemHaystack::modInit();
    ZombieMausoleumGargantuar::modInit();
    ZombieMausoleumGargantuarProps::modInit();
    ZombieAnimRig_MausoleumGargantuar::modInit(); 
    TombraiserProjectileProps::modInit();
    ZombieModernDancer::ModInit();
    ZombieModernDancerProps::modInit();
    ZombieModernBungee::ModInit();
    ZombieModernBungeeProps::modInit();
    ZombieModernBungeeTarget::ModInit();
    ZombieCatapult::modInit();
    ZombieCatapultProps::modInit();
    ZombieAnimRig_Catapult::modInit();
}