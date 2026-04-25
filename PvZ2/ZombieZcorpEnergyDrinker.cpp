#include "ZombieTombRaiser.h"
#include "ZombieTombRaiserProps.h"
#include "ZombieAnimRig_TombRaiser.h"
#include "ZombieStateHelper.h"
#include "ZombieState.h"
#include "StateMachineBuilder.h"
#include "Board.h"


typedef void (*zombieEnterState)(ZombieZCorpEnergyDrinker*, int, int);

typedef void (*LoopWalk)(ZombieZCorpEnergyDrinker*);
typedef bool (*isDeadOrDying)(ZombieZCorpEnergyDrinker*);
DECLARE_DELEGATES_SETUP(ZombieZCorpEnergyDrinker)

static Sexy::DelegateBase drinkingCompletedDelegate;
using namespace Sexy;

void* ZombieZCorpEnergyDrinker::vftable = __null;
Sexy::RtClass* ZombieZCorpEnergyDrinker::s_rtClass = __null;;

void DrinkerOnSpawn(ZombieZCorpEnergyDrinker* zombie) {
    auto rig = reinterpret_cast<ZombieAnimRig_EnergyDrinker*>(zombie->m_animRig.Get());
    zombie->m_gotCondition = false;
    rig->m_gotCondition = false;
    typedef void (*zombieFun49)(ZombieZCorpEnergyDrinker*);
    ((zombieFun49)getActualOffset(0xC3D1F0))(zombie);
}
void DrinkerWalkOnLoop(ZombieZCorpEnergyDrinker* zombie)
{
    isDeadOrDying isDeadFunc = (isDeadOrDying)getActualOffset(0xC3E204);
    if (isDeadFunc(zombie)) {
        ((LoopWalk)getActualOffset(0xC506B4))(zombie);
        return;
    }
    auto* props = reinterpret_cast<ZombieZCorpEnergyDrinkerProps*>(zombie->m_propertySheet.Get());

    if (zombie->m_position.x <= 744.0f) {
        if (zombie->m_gotCondition == false && ((zombie->m_teamFlags) & 2) != 0) {
            ((zombieEnterState)getActualOffset(0xC3D428))(zombie, 16, 0);
            return;
        }
    }
    ((LoopWalk)getActualOffset(0xC506B4))(zombie);
}
int GetCondIDByName(const char* name)
{
    if (!name) return -1;
    static const std::map<std::string, int> conditionMap = {
        {"none",              zombie_condition_none},
        {"chill",             zombie_condition_chill},
        {"freeze",            zombie_condition_freeze},
        {"stun",              zombie_condition_stun},
        {"butter",            zombie_condition_butter},
        {"bleeding",          zombie_condition_bleeding},
        {"lightning",         zombie_condition_lightning},
        {"rush",              zombie_condition_rush},
        {"speedup1",          zombie_condition_speedup1},
        {"speedup2",          zombie_condition_speedup2},
        {"speedup3",          zombie_condition_speedup3},
        {"speedup4",          zombie_condition_speedup4},
        {"tossed",            zombie_condition_tossed},
        {"blownoff",          zombie_condition_blownoff},
        {"warpingin",         zombie_condition_warpingIn},
        {"warpingout",        zombie_condition_warpingOut},
        {"potionspeed1",      zombie_condition_potionspeed1},
        {"potionspeed2",      zombie_condition_potionspeed2},
        {"potionspeed3",      zombie_condition_potionspeed3},
        {"potiontoughness1",  zombie_condition_potiontoughness1},
        {"potiontoughness2",  zombie_condition_potiontoughness2},
        {"potiontoughness3",  zombie_condition_potiontoughness3},
        {"potionsuper1",      zombie_condition_potionsuper1},
        {"potionsuper2",      zombie_condition_potionsuper2},
        {"potionsuper3",      zombie_condition_potionsuper3},
        {"hypnotized",        zombie_condition_hypnotized},
        {"sunbeaned",         zombie_condition_sunbeaned},
        {"morphedtogargantuar",           zombie_condition_morphedtogargantuar},
        {"knighted",          zombie_condition_knighted},
        {"plantfood",         zombie_condition_hasplantfood},
        {"damageflash",       zombie_condition_damageflash},
        {"zombossstun",       zombie_condition_zombossstun},
        {"haunted",           zombie_condition_haunted},
        {"icecubed",          zombie_condition_icecubed},
        {"sapped",            zombie_condition_sapped},
        {"unsuspendable",     zombie_condition_unsuspendable},
        {"present_boxed",     zombie_condition_present_boxed},
        {"stalled",           zombie_condition_stalled},
        {"speeddown1",        zombie_condition_speeddown1},
        {"speeddown2",        zombie_condition_speeddown2},
        {"speeddown3",        zombie_condition_speeddown3},
        {"speeddown4",        zombie_condition_speeddown4},
        {"invincible",        zombie_condition_invincible},
        {"terrified",         zombie_condition_terrified},
        {"shrinking",         zombie_condition_shrinking},
        {"shrunken",          zombie_condition_shrunken},
        {"poisoned",          zombie_condition_poisoned},
        {"contagiouspoison",  zombie_condition_contagiouspoison},
        {"decaypoison",       zombie_condition_decaypoison},
        {"bloomingheartdebuff",     zombie_condition_bloomingheartdebuff},
        {"hotdateattraction",           zombie_condition_hotdateattraction},
        {"solarflared",       zombie_condition_solarflared},
        {"suiciding",         zombie_condition_suiciding},
        {"stackableslow",     zombie_condition_stackableslow},
        {"suncarrier250",     zombie_condition_suncarrier250},
        {"suncarrier50",      zombie_condition_suncarrier50},
        {"suncarrier100",     zombie_condition_suncarrier100},
        {"dazeystunned",      zombie_condition_dazeystunned},
        {"iceblocked",        zombie_condition_iceblocked},
        {"gummed",            zombie_condition_gummed},
        {"hungered",          zombie_condition_hungered},
        {"stoneblocked",      zombie_condition_stoneblocked},
        {"stickybombed",      zombie_condition_stickybombed},
        {"petrified",         zombie_condition_petrified},
        {"invisibleslow",     zombie_condition_invisibleslow},
        {"concealmintdamagescale",     zombie_condition_concealmintdamagescale},
        {"poweredconcealmintdamagescalepowered",     zombie_condition_poweredconcealmintdamagescalepowered},
        {"corpseexplosion",     zombie_condition_corpseexplosion},
        {"blockolistunned",     zombie_condition_blockolistunned},
        {"bramblebushstunned",     zombie_condition_bramblebushstunned},
        {"bramblebushgrabbed",     zombie_condition_bramblebushgrabbed}
    };

    auto it = conditionMap.find(name);
    if (it != conditionMap.end()) {
        return it->second;
    }
    return -1;
}

void CondZombie(Zombie* self) {
    auto* props = reinterpret_cast<ZombieZCorpEnergyDrinkerProps*>(self->m_propertySheet.Get());
    typedef void (*setConditionZ)(Zombie*, int, int, float, float);
    static setConditionZ setZCondition = (setConditionZ)getActualOffset(0xC40CC0);

    float lifetime = props->ConditionLifeTime;
    if (lifetime <= 0.0f) {
        lifetime = 3.4028e38f;
    }

    auto ApplyRandomCondFromVector = [&](const std::vector<SexyString>& condVector) {
        if (condVector.empty()) return;

        int randomIndex = rand() % condVector.size();
        const SexyString& conditionName = condVector[randomIndex];

        int condID = GetCondIDByName(conditionName.c_str());
        if (condID != -1) {
            if (condID == zombie_condition_shrinking) {
                setZCondition(self, condID, 0, 0.01f, 0.0f);
            }
            else if (condID == zombie_condition_stun || condID == zombie_condition_dazeystunned) {
                setZCondition(self, condID, 0, lifetime, 0.0f);
            }
            else {
                setZCondition(self, condID, 0, lifetime, 0.0f);
            }
            LOGI("[CondZombie] Applied random condition: %s", conditionName.c_str());
        }
        };
    float goodChance = props->ChanceToApplyGoodCondition;
    float badChance = props->ChanceToApplyBadCondition;
    float totalChance = goodChance + badChance;
    float masterRoll = (float)(rand()) / (float)(RAND_MAX)*totalChance;

    if (masterRoll <= props->ChanceToApplyGoodCondition)
    {
        ApplyRandomCondFromVector(props->GoodConditionToApply);
    }
    else
    {
        ApplyRandomCondFromVector(props->BadConditionToApply);
    }
}

void ZombieZCorpEnergyDrinker::DrinkingOnEnter(ZombieZCorpEnergyDrinker* zombie)
{
    RegisterEventAfterAnim(zombie, "drink", "onDrinkingCompleted");
}

void ZombieZCorpEnergyDrinker::DrinkingOnLoop(ZombieZCorpEnergyDrinker* zombie)
{

}

void ZombieZCorpEnergyDrinker::DrinkingOnExit(ZombieZCorpEnergyDrinker* zombie)
{
    if (!zombie->m_gotCondition)
    {
        auto rig = reinterpret_cast<ZombieAnimRig_EnergyDrinker*>(zombie->m_animRig.Get());
        CondZombie(zombie);
        zombie->m_gotCondition = true;
        rig->m_gotCondition = true;
    }
}
void DrinkingCompletedCallback(Zombie* zombie) {
    ZombieZCorpEnergyDrinker* boxZombie = static_cast<ZombieZCorpEnergyDrinker*>(zombie);
    if (boxZombie) {
        ((zombieEnterState)getActualOffset(0xC3D428))(boxZombie, 1, 0);
    }
}
void ZombieZCorpEnergyDrinker::modInit() {
    LOGI("ZombieZCorpEnergyDrinker init");

    vftable = CreateChildVFTable(204 + 6, getActualOffset(0x241D430), 204);

    PatchVFTable(vftable, (void*)ZombieZCorpEnergyDrinker::StaticGetType, 0);

    PatchVFTable(vftable, (void*)DrinkerOnSpawn, 49);
    PatchVFTable(vftable, (void*)DrinkerWalkOnLoop, 124);

    PatchVFTable(vftable, (void*)ZombieZCorpEnergyDrinker::DrinkingOnEnter, 204);
    PatchVFTable(vftable, (void*)ZombieZCorpEnergyDrinker::DrinkingOnLoop, 205);
    PatchVFTable(vftable, (void*)ZombieZCorpEnergyDrinker::DrinkingOnExit, 206);

    ZombieZCorpEnergyDrinker::StaticGetType();

    LOGI("ZombieZCorpEnergyDrinker finish init");
}

void ZombieZCorpEnergyDrinker::buildEventCallbacks(Reflection::CRefManualSymbolBuilder* builder, Reflection::RClass* rtClass)
{
    IF_CALLBACK_NOTSETUP(ZombieZCorpEnergyDrinker) {
        SetupLiteralDelegate(&drinkingCompletedDelegate, DrinkingCompletedCallback);
        ZombieZCorpEnergyDrinker_delegatesSetup = true;
        LOGI("SO TRUE");
    }
    RegisterEventCallback(builder, rtClass, "onDrinkingCompleted", drinkingCompletedDelegate);
    LOGI("Reg event complete");
}


void ZombieZCorpEnergyDrinker::buildStates()
{
    StateMachineTableBuilder* stateMachine = CallGetStateMachine(ZombieZCorpEnergyDrinker::StaticGetType());
    RegisterStateByOffsets(stateMachine,
        16,
        (uintptr_t)ZombieZCorpEnergyDrinker::DrinkingOnEnter,
        (uintptr_t)ZombieZCorpEnergyDrinker::DrinkingOnLoop,
        (uintptr_t)ZombieZCorpEnergyDrinker::DrinkingOnExit,
        "ZS_ZCorpDrinker_Drinking");
    LOGI("Reg state complete");
}