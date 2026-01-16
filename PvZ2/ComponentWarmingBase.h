#pragma once
#include "Sexy/RtWeakPtr.h"
#include <Sexy/RtObject.h>
#include <Sexy/RtReflectionDelegateBase.h>
#include "GridItemCrateTimerProps.h"
#include "Component.h"
#include <vector>
#include <cstdint> 

//I'm not sure about these enum, just found when recreate EffectObject and TargetInfoProps

enum ZombieTestFlag : uint32_t {
    ZOMBIE_NONE = 0,
    ZOMBIE_DYING = 1,
    ZOMBIE_OPPOSING_TEAM = 2,
    ZOMBIE_SAME_TEAM = 4,
    ZOMBIE_BLEEDING_OUT = 8,
    ZOMBIE_IS_TARGETABLE = 16,    
    ZOMBIE_IS_NOT_TARGETABLE = 32,  
    ZOMBIE_PLANT_CAN_TARGET_ZOMBIE = 64,
    ZOMBIE_PLANT_CANNOT_TARGET_ZOMBIE = 128, 
    ZOMBIE_ON_SCREEN = 256,
    ZOMBIE_OFF_SCREEN = 512,
    ZOMBIE_ZOMBIE_CAN_BE_TARGETED_BY_PLANT = 1024,
    ZOMBIE_ZOMBIE_CANNOT_BE_TARGETED_BY_PLANT = 2048, 
    ZOMBIE_COLLIDES_WITH_PLANT = 4096, 
    ZOMBIE_DOES_NOT_COLLIDE_WITH_PLANT = 8192, 
    ZOMBIE_IN_ROW = 16384,
    ZOMBIE_NOT_IN_ROW = 32768,
    ZOMBIE_MARKED_FOR_DEATH = 65536,
    ZOMBIE_STANDARD_IGNORED = 45  
};

enum GridItemTestFlag : uint32_t {
    GRIDITEM_OPPOSING_TEAM = 1,
    GRIDITEM_SAME_TEAM = 2,
    GRIDITEM_ON_SCREEN = 4,
    GRIDITEM_OFF_SCREEN = 8,
    GRIDITEM_IN_ROW = 16, 
    GRIDITEM_NOT_IN_ROW = 32,  
    GRIDITEM_IN_COL = 64,
    GRIDITEM_NOT_IN_COL = 128, 
    GRIDITEM_IS_DAMAGABLE = 256, 
    GRIDITEM_IS_NOT_DAMAGABLE = 512, 
    GRIDITEM_IS_DAMAGABLE_BY_PLANTS = 1024, 
    GRIDITEM_IS_NOT_DAMAGABLE_BY_PLANTS = 2048, 
    GRIDITEM_IS_TARGETABLE_BY_ATTACKER = 4096,
    GRIDITEM_IS_NOT_TARGETABLE_BY_ATTACKER = 8192, 
    GRIDITEM_STANDARD_PLANTS_IGNORE = 2050,
    GRIDITEM_ANY = 16384  
};

enum RenderLayer : int32_t {
    RENDER_LAYER_INVALID = -1,
    RENDER_LAYER_GLITTER_RAINBOW_BACK_OFFSET = -2500,
    RENDER_LAYER_GLITTER_RAINBOW_FRONT_OFFSET = -500,
    RENDER_LAYER_ROW_OFFSET = 16000,
    RENDER_LAYER_STAGE_BACKGROUND = 100000,
    RENDER_LAYER_ABOVE_GROUND = 200000,
    RENDER_LAYER_UI_BOTTOM = 250000,
    RENDER_LAYER_GROUND = 300000,
    RENDER_LAYER_DIRTY_LAWN = 300001,
    RENDER_LAYER_SHADOWS = 300002,
    RENDER_LAYER_GHOSTPEPPER_FOG = 300003,
    RENDER_LAYER_LAWN = 400000,
    RENDER_LAYER_TILES = 400001,
    RENDER_LAYER_PROTECT_THE_PLANT = 400005,
    RENDER_LAYER_LAVA_CRACK = 400007,
    RENDER_LAYER_LILYPAD = 400009,
    RENDER_LAYER_SAP = 400010,
    RENDER_LAYER_SHADOWPORTAL = 400011,
    RENDER_LAYER_RAILCART = 400100,
    RENDER_LAYER_BOMBEGRANATE_SEEDS = 400110,
    RENDER_LAYER_MOONFLOWER_GLOW = 400500,
    RENDER_LAYER_ELECTRIC_FENCE = 400600,
    RENDER_LAYER_GRAVE_STONE = 401000,
    RENDER_LAYER_PLANT = 403000,
    RENDER_LAYER_CREATURE = 406000,
    RENDER_LAYER_LOSTCITY_TENT = 406990,
    RENDER_LAYER_ZOMBIE = 407000,
    RENDER_LAYER_PROJECTILE = 412000,
    RENDER_LAYER_LAWN_MOWER = 413000,
    RENDER_LAYER_PARTICLE = 414000,
    RENDER_LAYER_TOP = 500000,
    RENDER_LAYER_DROPPED_SEEDPACKETS = 501000,
    RENDER_LAYER_END_OF_LEVEL_REWARD = 502000,
    RENDER_LAYER_FOG = 600000, 
    RENDER_LAYER_COIN_BANK = 650000,
    RENDER_LAYER_UI_TOP = 800000,
    RENDER_LAYER_ABOVE_UI = 900000,
    RENDER_LAYER_SCREEN_FADE = 1000000,
    RENDER_LAYER_DARK_BRAZIER_TOP = 300004, //???
    RENDER_LAYER_TIDE_WET_SAND = 300005, //???
    RENDER_LAYER_TIDE_LOWER_LAYER = 300006, //???
    RENDER_LAYER_TIDE_UNDERWATER_SQUARE = 300007, //???
    RENDER_LAYER_TIDE_UPPER_LAYER = 300008, //???
    RENDER_LAYER_STAGE_FOREGROUND = 500001, //???
    RENDER_LAYER_STAGE_FOREGROUND_SHADOW = 151001, //???
    RENDER_LAYER_TIDE_SIGN = 151002, //???
    RENDER_LAYER_POWER_TILE_DOT = 400002, //???
    RENDER_LAYER_PLANT_ABOVE_ZOMBIES = 408000  //???
};

class EffectObjectPropertySheet {
public:
    SexyString PopAnim;
    SexyVector2 PopAnimRenderOffset;
    SexyString SpawnSoundID;
    std::vector<SexyString> SpawnAnimations;
    bool Centered;
    bool CenterInGrid;
    char _pad_align[2];
    float PopAnimScaleX;
    float PopAnimScaleY;
    Point ShakeBoard;
    RenderLayer LayerOverride;
    char _pad_end[4];
};

class EffectObjectType {
public:
    SexyString EffectObjectClass;
    std::vector<SexyString> ResourceGroups;
    Sexy::RtWeakPtr<EffectObjectPropertySheet> Properties;
    char padobj[16];
};
static_assert(sizeof(EffectObjectType) == 0x30);

class TargetInfoProps {
public:
    std::vector<ZombieTestFlag> IgnoreZombieFlag;
    std::vector<GridItemTestFlag> IgnoreGridItemFlag;
    char padtarget[40];
};
static_assert(sizeof(TargetInfoProps) == 0x40);

class ComponentRadiusBurstProps {
public:
    float RadiusInGridSquareWidths;
    float PulseCooldown;
    float PulsePropagationTime;
    int32_t PulseRepeatTimes;
    float PulseRepeatDelay;
    SexyString PopAnimEffectName;
    float PopAnimScaleX;
    float PopAnimScaleY;
    Point PopAnimOffset; 
    float PopAnimEffectStartDelay;
    char _pad1[4];
    TargetInfoProps TargetProps;
    bool RenderAtEffectLayer;
    char _pad2[7];
    Sexy::RtWeakPtr<EffectObjectType> EffectObject;
};

class ComponentWarmingRadiusProps : public ComponentRadiusBurstProps {
public:
    float PulseInitialDelay = 0.50f;
    char _pad1[4];
    DamageInfoProps GridItemDamageProps;
    DamageInfoProps PlantDamageProps;
};



class ComponentRadiusBurst : public ComponentBase {
public:
    ComponentRadiusBurstProps m_props;
    float m_nextPropagateTime;
    float m_nextApplyTime;
    float m_nextAnimationStartTime;
    bool m_isOriginDisplaced;
    char _align[3];
    Point m_displacedOrigin;
    Point m_gridOrigin;
    Sexy::RtReflectionDelegateBase m_completionCallback;
    char pad16[8];
};
static_assert(sizeof(ComponentRadiusBurst) == 0x110);

class ComponentWarmingRadius : public ComponentRadiusBurst {
public:
    ComponentWarmingRadiusProps m_warming_props;
};
static_assert(sizeof(ComponentWarmingRadius) == 0x210);

