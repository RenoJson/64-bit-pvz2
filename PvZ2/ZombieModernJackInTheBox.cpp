#include "ZombieModernJackInTheBox.h"
#include "ZombieStateHelper.h"
#include "ZombieModernJackInTheBoxProps.h"
#include "ZombieState.h"
#include "StateMachineBuilder.h"
#include "ZombieAnimRig_ModernJackInTheBox.h"
#include "Plant.h"
#include "DamageInfo.h"
#include "TimeMgr.h"
#include "Board.h"
#include "GridItem.h"


void* ZombieModernJackInTheBox::vftable = nullptr;
Sexy::RtClass* ZombieModernJackInTheBox::s_rtClass = nullptr;;
typedef void (*zombieEnterState)(ZombieModernJackInTheBox*, int, int);
typedef void* (*playSoundEvent)(ZombieModernJackInTheBox*, SexyString*, float);
typedef Zombie* (*zombieAllowMovement)(Zombie*, int);
typedef void (*LoopWalk)(ZombieModernJackInTheBox*);
typedef void (*LoopEat)(ZombieModernJackInTheBox*);
typedef Plant* (*getTarg)(ZombieModernJackInTheBox*);
typedef bool (*isDeadOrDying)(ZombieModernJackInTheBox*);
typedef void (*setSpeedScale)(ZombieModernJackInTheBox*, float);
typedef void (*Destroy)(ZombieModernJackInTheBox*);
std::map<GridItemVase*, float> g_VaseRevealTimers;
DECLARE_DELEGATES_SETUP(ZombieModernJackInTheBox)

static Sexy::DelegateBase lostBoxCompletedDelegate;

static Sexy::DelegateBase explodedCompletedDelegate;

bool BoxShouldIgnoreCollision(ZombieModernJackInTheBox* zombie, Projectile* proj)
{
    int myTeam = zombie->m_teamFlags;
    int otherTeam = proj->m_teamFlags;
    if (zombie->m_isExploded == true) {
        return true;
    }
    else {
        if ((otherTeam & 2) != 0 && (myTeam & 1) != 0)
        {
            return false;
        }
        else
        {
            bool isOtherNotPlant = ((otherTeam & 1) == 0);
            bool isMeNotZombie = ((myTeam & 2) == 0);

            return isOtherNotPlant || isMeNotZombie;
        }
    }
}

void BoxOnSpawn(ZombieModernJackInTheBox* zombie)
{
    auto props = reinterpret_cast<ZombieModernJackInTheBoxProps*>(zombie->m_propertySheet.Get());
    auto rig = reinterpret_cast<ZombieAnimRig_ModernJackInTheBox*>(zombie->m_animRig.Get());
    zombie->m_isExploded = false;
    bool hasBoxArmor = false;

    for (auto& weakArmor : zombie->m_armor)
    {
        Armor* armor = weakArmor.Get();
        if (armor != nullptr)
        {
            auto* armorProps = reinterpret_cast<ArmorPropertySheet*>(armor->m_propertySheetPtr.Get());

            if (armorProps != nullptr && armorProps->ArmorType == "JackInTheBox")
            {
                hasBoxArmor = true;
                break;
            }
        }
    }
    if (hasBoxArmor)
    {
        if (rig != nullptr) {
            rig->m_hasBox = true;
        }
        float highChance = props->BoxExplodeTimerProps.HighTimerChance;
        float lowChance = props->BoxExplodeTimerProps.LowTimerChance;
        float totalChance = highChance + lowChance;
        float rollChance = static_cast<float>(rand()) / RAND_MAX * totalChance;
        float chosenDuration = 0.0f;
        if (rollChance <= highChance) {
            float min = props->BoxExplodeTimerProps.HighTimer.Min;
            float max = props->BoxExplodeTimerProps.HighTimer.Max;
            chosenDuration = min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
        }
        else {
            float min = props->BoxExplodeTimerProps.LowTimer.Min;
            float max = props->BoxExplodeTimerProps.LowTimer.Max;

            chosenDuration = min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
        }

        zombie->m_finalExplosionTime = zombie->m_creationTime + chosenDuration;
    }
    else
    {
        if (rig != nullptr) { 
            rig->m_hasBox = false; 
        }
    }

    typedef void (*zombieFun49)(ZombieModernJackInTheBox*);
    ((zombieFun49)getActualOffset(0xC3D1F0))(zombie);
    if (hasBoxArmor)
    {
        ((setSpeedScale)getActualOffset(0xC484C0))(zombie, props->HaveBoxSpeedScale);
    }
}
void BoxOnArmorDestroyed(ZombieModernJackInTheBox* zombie, int a2, SexyString* armorName)
{
    isDeadOrDying isDeadFunc = (isDeadOrDying)getActualOffset(0xC3E204);
    if (*armorName == "JackInTheBox" && !isDeadFunc(zombie)) {
        ((zombieEnterState)getActualOffset(0xC3D428))(zombie, 16, 0);
    }
}
void BoxWalkOnLoop(ZombieModernJackInTheBox* zombie)
{
    isDeadOrDying isDeadFunc = (isDeadOrDying)getActualOffset(0xC3E204);
    if (isDeadFunc(zombie)) {
        SexyString eventName = "Stop_JackInTheBox_MusicBox";
        ((playSoundEvent)getActualOffset(0x10B0608))(zombie, &eventName, 0.0f);
        ((LoopWalk)getActualOffset(0xC506B4))(zombie);
        return;
    }
    auto rig = reinterpret_cast<ZombieAnimRig_ModernJackInTheBox*>(zombie->m_animRig.Get());

    if (rig->m_hasBox == false) {
        ((LoopWalk)getActualOffset(0xC506B4))(zombie);
        return;
    }
    auto* props = reinterpret_cast<ZombieModernJackInTheBoxProps*>(zombie->m_propertySheet.Get());

    if (zombie->m_position.x <= props->MinXPosToExplode) {
        if (TimeMgr::GetInstance()->m_curTime >= zombie->m_finalExplosionTime && ((zombie->m_teamFlags) & 2) != 0) {
            ((zombieEnterState)getActualOffset(0xC3D428))(zombie, 17, 0);
            return;
        }
    }

    ((LoopWalk)getActualOffset(0xC506B4))(zombie);
}
void BoxEatOnLoop(ZombieModernJackInTheBox* zombie)
{
    auto rig = reinterpret_cast<ZombieAnimRig_ModernJackInTheBox*>(zombie->m_animRig.Get());
    auto props = reinterpret_cast<ZombieModernJackInTheBoxProps*>(zombie->m_propertySheet.Get());
    ((LoopEat)getActualOffset(0xC5082C))(zombie);
    if (rig->m_hasBox == true && zombie->m_position.x <= props->MinXPosToExplode) {
        if (TimeMgr::GetInstance()->m_curTime >= zombie->m_finalExplosionTime) {
            ((zombieEnterState)getActualOffset(0xC3D428))(zombie, 17, 0);
            return;
        }
    }
}
void BoxExplosion(ZombieModernJackInTheBox* zombie) {
    auto* props = reinterpret_cast<ZombieModernJackInTheBoxProps*>(zombie->m_propertySheet.Get());
    float damageAmount = props->ExplosionDamage;
    
    int zX = static_cast<int>((zombie->m_position.x - 200.0f) / 64.0f);
    int zY = static_cast<int>((zombie->m_position.y - 160.0f) / 76.0f);

    Rect ExplodeRect;
    ExplodeRect.mX = zX + props->ExplosionRect.mX;
    ExplodeRect.mY = zY + props->ExplosionRect.mY;
    ExplodeRect.mWidth = props->ExplosionRect.mWidth;
    ExplodeRect.mHeight = props->ExplosionRect.mHeight;

    SexyString eventName = "Play_JackInTheBox_BoxBoom";
    ((playSoundEvent)getActualOffset(0x10B0608))(zombie, &eventName, 0.0f);

    std::vector<BoardEntity*> entityList;

    typedef void (*GetEntitiesInRectFunc)(std::vector<BoardEntity*>*, int, Rect*);
    GetEntitiesInRectFunc getEntitiesRect = (GetEntitiesInRectFunc)getActualOffset(0x86F180);
    typedef void* (*Func_VaseBreak)(GridItemVase*);
    Func_VaseBreak breakVase = (Func_VaseBreak)getActualOffset(0xA322B8);
    typedef GridItemVase* (*Func_VaseReveal)(GridItemVase*, bool);
    Func_VaseReveal revealVase = (Func_VaseReveal)getActualOffset(0xA311B0);

    getEntitiesRect(&entityList, 63, &ExplodeRect);

    for (BoardEntity* ptr : entityList) {
        if (ptr == nullptr) continue;

        if (ptr->IsType(PlantGroup::StaticGetType()) && zombie->m_teamFlags == 2)
        {
            DamageInfo dmg;
            dmg.m_attacker = zombie;
            dmg.m_damage = damageAmount;

            void** vtable = *(void***)ptr;
            typedef void (*VirtualTakeDamageFunc)(PlantGroup*, DamageInfo*);
            VirtualTakeDamageFunc takeDmg = (VirtualTakeDamageFunc)vtable[35]; 

            takeDmg((PlantGroup*)ptr, &dmg);
        }
        else if (ptr->IsType(Zombie::StaticGetType()) && zombie->m_teamFlags == 1) {
            DamageInfo dmg;
            dmg.m_attacker = zombie;
            dmg.m_damage = damageAmount;

            void** vtable = *(void***)ptr;
            typedef void (*VirtualTakeDamageFunc)(Zombie*, DamageInfo*);
            VirtualTakeDamageFunc takeDmg = (VirtualTakeDamageFunc)vtable[35];

            takeDmg((Zombie*)ptr, &dmg);
        }
        else if (props->CanBreakVase == true && ptr->IsType(GridItemVase::StaticGetType()))
        {
            auto vase = reinterpret_cast<GridItemVase*>(ptr);
            if ((vase->m_flags & 4) == 0)
            {
                breakVase(vase);
            }
        }
        else if (props->CanRevealVase == true && ptr->IsType(GridItemVase::StaticGetType()))
        {
            auto vase = reinterpret_cast<GridItemVase*>(ptr);
            if ((vase->m_flags & 4) == 0)
            {
                revealVase(vase, true);
                float revealDuration = props->VaseRevealDuration;
                float expireTime = TimeMgr::GetInstance()->m_curTime + revealDuration;

                g_VaseRevealTimers[vase] = expireTime;
            }
        }
    }
}
void BoxActionFrame(ZombieModernJackInTheBox* zombie, SexyString* currentAnim, SexyString* actionName, SexyString* param, float nextFrameTime)
{
    if (*actionName == "explode" && !zombie->m_isExploded)
    {
        zombie->m_isExploded = true;
        zombie->m_damageScale = 0.0f;
        BoxExplosion(zombie);
    }
}
void BoxOnGetCondition(ZombieModernJackInTheBox* zombie, int conditionID)
{
    if (conditionID == zombie_condition_hypnotized)
    {
        ((zombieEnterState)getActualOffset(0xC3D428))(zombie, 17, 0);
    }
    if (conditionID == zombie_condition_stun || conditionID == zombie_condition_warpingIn)
    {
        SexyString eventName = "Stop_JackInTheBox_MusicBox";
        ((playSoundEvent)getActualOffset(0x10B0608))(zombie, &eventName, 0.0f);
	}
}
void ZombieModernJackInTheBox::LostBoxOnEnter(ZombieModernJackInTheBox* zombie)
{
    RegisterEventAfterAnim(zombie, "lost_box", "onLostBoxCompleted");
    SexyString eventName = "Stop_JackInTheBox_MusicBox";
    ((playSoundEvent)getActualOffset(0x10B0608))(zombie, &eventName, 0.0f);
}

void ZombieModernJackInTheBox::LostBoxOnLoop(ZombieModernJackInTheBox* zombie)
{

}

void ZombieModernJackInTheBox::LostBoxOnExit(ZombieModernJackInTheBox* zombie)
{

}

void ZombieModernJackInTheBox::SurpriseOnEnter(ZombieModernJackInTheBox* zombie)
{
    RegisterEventAfterAnim(zombie, "box_surprised", "onSurpriseCompleted");
    SexyString eventName = "Play_JackInTheBox_Surprised";
    ((playSoundEvent)getActualOffset(0x10B0608))(zombie, &eventName, 0.0f);
}

void ZombieModernJackInTheBox::SurpriseOnLoop(ZombieModernJackInTheBox* zombie)
{

}

void ZombieModernJackInTheBox::SurpriseOnExit(ZombieModernJackInTheBox* zombie)
{

}
void LostBoxCompletedCallback(Zombie* zombie) {
    auto rig = reinterpret_cast<ZombieAnimRig_ModernJackInTheBox*>(zombie->m_animRig.Get());
    ZombieModernJackInTheBox* boxZombie = static_cast<ZombieModernJackInTheBox*>(zombie);
    if (boxZombie) {
        rig->m_hasBox = false;
        ((zombieEnterState)getActualOffset(0xC3D428))(boxZombie, 1, 0);
        ((setSpeedScale)getActualOffset(0xC484C0))(boxZombie, 1);
    }
}
void SurpriseCompletedCallback(Zombie* zombie) {
    ZombieModernJackInTheBox* boxZombie = static_cast<ZombieModernJackInTheBox*>(zombie);
    if (boxZombie) {
        boxZombie->m_damageScale = 1.0f;
        for (auto& weakArmor : zombie->m_armor)
        {
            Armor* armor = weakArmor.Get();
            if (armor != nullptr && !armor->m_destroyed)
            {
                armor->m_destroyed = true;
            }
        }
        DamageInfo dmg;
        dmg.m_attacker = boxZombie;
        dmg.m_damage = 100000.0f;
        dmg.m_flags = DamageTypeFlags::damage_instantly_fatal;

        void** vtable = *(void***)boxZombie;
        typedef void (*VirtualTakeDamageFunc)(ZombieModernJackInTheBox*, DamageInfo*);
        VirtualTakeDamageFunc takeDmg = (VirtualTakeDamageFunc)vtable[36]; 
        takeDmg(boxZombie, &dmg);
    }
}

typedef void* (*UpdateVase)(GridItemVase*);
UpdateVase oUpdateVase = nullptr;

void* hkUpdateVase(GridItemVase* vase)
{
    auto it = g_VaseRevealTimers.find(vase);
    if (it != g_VaseRevealTimers.end()) {

        if ((vase->m_flags & 4) != 0) {
            g_VaseRevealTimers.erase(it);
            return oUpdateVase(vase);
        }
        if (TimeMgr::GetInstance()->m_curTime >= it->second) {

            typedef GridItemVase* (*Func_VaseReveal)(GridItemVase*, bool);
            Func_VaseReveal revealVase = (Func_VaseReveal)getActualOffset(0xA311B0);

            revealVase(vase, false);
            g_VaseRevealTimers.erase(it);
        }
    }
    return oUpdateVase(vase);
}

void ZombieModernJackInTheBox::ModInit() {
    LOGI("ZombieBox mod init");
    PVZ2HookFunction(0xA321B8, (void*)hkUpdateVase, (void**)&oUpdateVase);
    vftable = CreateChildVFTable(204 + 9, getActualOffset(0x241D430), 204);
    PatchVFTable(vftable, (void*)ZombieModernJackInTheBox::StaticGetType, 0);

    PatchVFTable(vftable, (void*)BoxShouldIgnoreCollision, 43);
    PatchVFTable(vftable, (void*)BoxOnSpawn, 49);
    PatchVFTable(vftable, (void*)BoxOnGetCondition, 71);
    PatchVFTable(vftable, (void*)BoxOnArmorDestroyed, 115);
    PatchVFTable(vftable, (void*)BoxWalkOnLoop, 124);
    PatchVFTable(vftable, (void*)BoxEatOnLoop, 127);
    PatchVFTable(vftable, (void*)BoxActionFrame, 170);

    PatchVFTable(vftable, (void*)ZombieModernJackInTheBox::LostBoxOnEnter, 204);
    PatchVFTable(vftable, (void*)ZombieModernJackInTheBox::LostBoxOnLoop, 205);
    PatchVFTable(vftable, (void*)ZombieModernJackInTheBox::LostBoxOnExit, 206);

    PatchVFTable(vftable, (void*)ZombieModernJackInTheBox::SurpriseOnEnter, 207);
    PatchVFTable(vftable, (void*)ZombieModernJackInTheBox::SurpriseOnLoop, 208);
    PatchVFTable(vftable, (void*)ZombieModernJackInTheBox::SurpriseOnExit, 209);

    ZombieModernJackInTheBox::StaticGetType();
    LOGI("ZombieBox finish init");
}

void ZombieModernJackInTheBox::buildEventCallbacks(Reflection::CRefManualSymbolBuilder* builder, Reflection::RClass* rtClass)
{
    IF_CALLBACK_NOTSETUP(ZombieModernJackInTheBox) {
        SetupLiteralDelegate(&lostBoxCompletedDelegate, LostBoxCompletedCallback);
        SetupLiteralDelegate(&explodedCompletedDelegate, SurpriseCompletedCallback);
        ZombieModernJackInTheBox_delegatesSetup = true;
        LOGI("SO TRUE");
    }
    RegisterEventCallback(builder, rtClass, "onLostBoxCompleted", lostBoxCompletedDelegate);
    RegisterEventCallback(builder, rtClass, "onSurpriseCompleted", explodedCompletedDelegate);
    LOGI("Reg event complete");
}


void ZombieModernJackInTheBox::buildStates()
{
    StateMachineTableBuilder* stateMachine = CallGetStateMachine(ZombieModernJackInTheBox::StaticGetType());
    RegisterStateByOffsets(stateMachine,
        16,
        (uintptr_t)ZombieModernJackInTheBox::LostBoxOnEnter,
        (uintptr_t)ZombieModernJackInTheBox::LostBoxOnLoop,
        (uintptr_t)ZombieModernJackInTheBox::LostBoxOnExit,
        "ZS_JackInTheBox_LostBox");
    RegisterStateByOffsets(stateMachine,
        17,
        (uintptr_t)ZombieModernJackInTheBox::SurpriseOnEnter,
        (uintptr_t)ZombieModernJackInTheBox::SurpriseOnLoop,
        (uintptr_t)ZombieModernJackInTheBox::SurpriseOnExit,
        "ZS_JackInTheBox_Exploding");
    LOGI("Reg state complete");
}
