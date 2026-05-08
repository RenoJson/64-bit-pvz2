#include "ZombieModifierModule.h"
#include "Zombie.h"
#include <map>
#include <string>
#include "ZombieParticle.h"


#pragma region hk Condition To Apply

typedef void(*Condition)(ZombieModifierModule*, Zombie*);
Condition oCondition = nullptr;


int GetConditionIDByName(const char* name)
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
        {"bloomingheart",     zombie_condition_bloomingheartdebuff},
        {"hotdate",           zombie_condition_hotdateattraction},
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

Zombie* ConditionModifier(ZombieModifierModule* module, Zombie* zombie) {
    auto* props = reinterpret_cast<ZombieModifierProperties*>(module->m_propertySheet.Get());
    typedef void (*setConditionZ)(Zombie*, int, int, float, float);
    setConditionZ setZCondition = (setConditionZ)getActualOffset(0xC40CC0);
    float lifetime = props->ConditionLifeTime;

    if (lifetime <= 0.0f)
    {
        lifetime = 3.4028e38f;
    }
    if (props->ConditionToApply.size() > 0)
    {
        for (int i = 0; i < props->ConditionToApply.size(); ++i)
        {
            const SexyString& conditionName = props->ConditionToApply[i];
            int condID = GetConditionIDByName(conditionName.c_str());

            if (condID != -1 && condID == zombie_condition_shrinking)
            {
                setZCondition(zombie, condID, 0, 0.01f, 0.0f);
            }
            else if (condID != -1)
            {
               setZCondition(zombie, condID, 0, lifetime, 0.0f);
            }
        }
    }
    return zombie;
}
Zombie* StatModifier(ZombieModifierModule* module, Zombie* zombie) {
    auto props = reinterpret_cast<ZombieModifierProperties*>(module->m_propertySheet.Get());

    zombie->m_dpsScale = props->DPSScale;
    zombie->m_scale = props->ArtScale;
    zombie->m_damageScale = props->DamageScale;

    float finalHpScale = props->HitpointsScale;
    if (finalHpScale < 1.0f) {
        finalHpScale = 1.0f;
    }
    zombie->m_hitpoints *= finalHpScale;
    zombie->m_maxHitpoints *= finalHpScale;

    for (size_t i = 0; i < zombie->m_armor.size(); i++) {
        auto armor = reinterpret_cast<Armor*>(zombie->m_armor[i].Get());
        if (armor != nullptr) {
            armor->m_health *= finalHpScale;
            armor->m_maxHealth *= finalHpScale;
        }
    }

    typedef Zombie* (*setSpeedScale)(Zombie*, float);
    return ((setSpeedScale)getActualOffset(0xC484C0))(zombie, props->SpeedScale);
}
Zombie* FlyModifier(ZombieModifierModule* module, Zombie* zombie) {
    typedef void (*SetFlyingFunc)(Zombie*, bool);
    SetFlyingFunc setFlying = (SetFlyingFunc)getActualOffset(0xC4C770);
    setFlying(zombie, true);
    return zombie;
}
Zombie* hkModifierModule(ZombieModifierModule* module, Zombie* zombie) {
    auto* props = reinterpret_cast<ZombieModifierProperties*>(module->m_propertySheet.Get());

    if (props->ModifierType == "condition") {
        return ConditionModifier(module, zombie);
    }
    else if (props->ModifierType == "stat") {
        return StatModifier(module, zombie);
    }
    else if (props->ModifierType == "aprilfool") {
        return FlyModifier(module, zombie);
    }
}
#pragma endregion

Reflection::CRefManualSymbolBuilder::BuildSymbolsFunc ZombieModifierProperties::oZombieModifierPropertiesBuildSymbols = nullptr;

void ZombieModifierProperties::modInit() {
    LOGI("init mdule");
    PVZ2HookFunction(0x1417FEC, (void*)hkModifierModule, (void**)&oCondition);
    PVZ2HookFunction(0x1417AD0, (void*)construct, nullptr);
    LOGI("init  mdule complete");
    LOGI("init  mdule props");
    PVZ2HookFunction(0x1417C0C, (void*)ZombieModifierProperties::buildSymbols, (void**)&ZombieModifierProperties::oZombieModifierPropertiesBuildSymbols);
    LOGI("init  mdule props complete");
    LOGI("finish init  mdule");
}