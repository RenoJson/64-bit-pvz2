#include "ZombieDarkCavalry.h"
#include "ZombieAnimRig_Bull.h"
#include "AddZombieType.h"
#include "Zombie_BullProps.h"
#include "ZombieCavalryProps.h"
#include "Plant.h"
#include "GridItem.h"
#include "ZombieStateHelper.h"
#include "ZombieState.h"
#include "StateMachineBuilder.h"
#include "DamageInfo.h"
#include "ZcorpRacerZombie.h"


void* ZombieDarkCavalry::vftable = __null;
Sexy::RtClass* ZombieDarkCavalry::s_rtClass = __null;;

DECLARE_DELEGATES_SETUP(ZombieDarkCavalry)
static Sexy::DelegateBase attackCompletedDelegate;

typedef void* (*playSoundEvent)(ZombieDarkCavalry*, SexyString*, float);
typedef Zombie* (*zombieAllowMovement)(Zombie*, int);
typedef Plant* (*getTarg)(ZombieDarkCavalry*);
typedef bool (*isAnimDone)(ZombieAnimRig*, int);
typedef int (*playAnimWithoutCallback)(ZombieAnimRig*, const SexyString&, int, DelegateBase&);
typedef int (*playAnimWithCallback)(ZombieAnimRig*, const SexyString&, int, ZombieEvent& event);
typedef ZombieEvent* (*ConstructEvent)(ZombieEvent*, RtWeakPtr<Zombie>& owner, const SexyString& eventName);
typedef Zombie* (*zombieFlippedAnim)(Zombie*, int);

typedef void (*zombieEnterState)(ZombieDarkCavalry*, int, int);
typedef Zombie* (*updatePos)(ZombieDarkCavalry*, SexyVector3*);
typedef bool (*checkZombieHasCondition)(Zombie*, int);

typedef void (*setConditionZ)(Zombie*, int, int, float, float);

typedef int (*boardEntitySetPosition)(Zombie*, SexyVector3*);
// override func 49, 206 and 215 so that cavalry alway launch  when encountered a plant no matter its walk anim are done or not
Zombie* overrideOnSpawn(ZombieDarkCavalry* zombie) {
    auto* props = reinterpret_cast<ZombieDarkCavalryProps*>(zombie->m_propertySheet.Get());
	zombie->m_damageState = 0;
	zombie->m_walkCycled = true;
	zombie->m_hasLaunched = false;
	typedef void (*zombieFun49)(ZombieDarkCavalry*);
	((zombieFun49)getActualOffset(0xC3D1F0))(zombie);
    if (props->Stampede == true) {
       ((zombieEnterState)getActualOffset(0xC3D428))(zombie, 19, 0);
    }
    else {
       ((zombieEnterState)getActualOffset(0xC3D428))(zombie, 16, 0);
    }
	Sexy::SexyVector3 newPos = {945.0f, zombie->m_position.y, 0};
	return ((updatePos)getActualOffset(0x628278))(zombie, &newPos);

}
void HideCustomKnightLayer(ZombieDarkCavalry* self, ZombieAnimRig_Bull* animRig) {

    auto* props = reinterpret_cast<ZombieBullProps*>(self->m_propertySheet.Get());

    std::vector<SexyString> HardcodedLayer = {
         "zombie_cowboy_hat_back",
         "zombie_cowboy_hat_front",
         "zombie_imp_arm_inner_lower",
         "zombie_imp_arm_inner_upper",
         "zombie_imp_arm_outer_lower",
         "zombie_imp_arm_outer_upper_01",
         "zombie_imp_arm_outer_upper_02",
         "zombie_imp_arms_outer_upper",
         "zombie_imp_eye",
         "zombie_imp_eye_sm",
         "zombie_imp_hand_inner",
         "zombie_imp_hand_outer",
         "zombie_imp_jaw",
         "zombie_imp_leg_inner_lower",
         "zombie_imp_leg_inner_upper",
         "zombie_imp_leg_outer_lower",
         "zombie_imp_leg_outer_upper",
         "zombie_imp_pupil",
         "zombie_imp_skull",
         "zombie_imp_toe_inner",
         "zombie_imp_toe_outer",
         "zombie_imp_torso",
         "zombie_imp_waist",
         "zombie_imp_leg_outer_upper",
         "_zombie_imp_head_top"
    };

    typedef int64_t(*setLayerVisibleFunc)(ZombieAnimRig_Bull*, SexyString*, bool);
    setLayerVisibleFunc setLayerVisible = (setLayerVisibleFunc)getActualOffset(0x9DB8D0);

    size_t customLayersCount = props->RiderLayersToHide.size();
    if (!props->RiderLayersToHide.empty()) {
        for (size_t i = 0; i < customLayersCount; i++) {
            const auto& layerStr = props->RiderLayersToHide[i];
            SexyString layerName(layerStr);
            setLayerVisible(animRig, &layerName, false);
        }
    }
    else {
        for (size_t i = 0; i < HardcodedLayer.size(); i++) {
            const auto& layerStr = HardcodedLayer[i];
            SexyString layerName(layerStr);
            setLayerVisible(animRig, &layerName, false);
        }
    }
}
void LanceSpawn(ZombieDarkCavalry* self)
{
    typedef PlantGroup* (*getTarg)(ZombieDarkCavalry*);
    getTarg getTarget = (getTarg)getActualOffset(0xC41910);
    PlantGroup* target = getTarget(self);
    float rawPosX;
    float rawPosY;
    int spawnPosX;
    int spawnPosY;
    typedef void (*killTarg)(Plant*, ZombieDarkCavalry*);
    killTarg KillTarget = (killTarg)getActualOffset(0x1337020);

    if (target != nullptr && target->IsType(PlantGroup::StaticGetType())) {
        rawPosX = target->m_position.x;
        rawPosY = target->m_position.y;

        spawnPosX = (int)(((rawPosX - 232.0f) / 64.0f));
        spawnPosY = (int)(((rawPosY - 160.0f) / 76.0f));


        DamageInfo dmg;
        dmg.m_attacker = self;

        void** vtable = *(void***)target;
        typedef void (*VirtualTakeDamageFunc)(PlantGroup*, DamageInfo*);
        VirtualTakeDamageFunc takeDmg = (VirtualTakeDamageFunc)vtable[36];
        takeDmg(target, &dmg);
    }
    else {
        rawPosX = self->m_position.x;
        rawPosY = self->m_position.y;

        spawnPosX = (int)(((rawPosX - 232.0f) / 64.0f) + 0.5f) - 1;
        spawnPosY = (int)(((rawPosY - 160.0f) / 76.0f));
    }

    Board* board = Board::GetBoard();
    auto* props = reinterpret_cast<ZombieDarkCavalryProps*>(self->m_propertySheet.Get());

    if (spawnPosX < 0) spawnPosX = 0;
    if (spawnPosX > 8) spawnPosX = 8;
    if (spawnPosY < 0) spawnPosY = 0;
    if (spawnPosY > 4) spawnPosY = 4;

    typedef GridItemSurfboard* (*funcAA230C)(Board*, SexyString, int, int);
    funcAA230C func_AA230C = (funcAA230C)getActualOffset(0xAA230C);
    GridItemSurfboard* gridItem = func_AA230C(board, "joustsword", spawnPosX, spawnPosY);

    gridItem->m_health = props->LanceHitpoints;
    gridItem->m_healthMax = props->LanceHitpoints;
}
void CavalryThrowRider(ZombieDarkCavalry* self)
{
    auto* props = reinterpret_cast<ZombieDarkCavalryProps*>(self->m_propertySheet.Get());
    SexyString name = props->RiderType;
    float distance = props->LaunchDistance;

    if (name.empty()) {
        name = "dark_cavalry_rider";
    }

    bool isShrinking = ((checkZombieHasCondition)getActualOffset(0xC3E44C))(self, zombie_condition_shrinking);
    bool isShrunken = ((checkZombieHasCondition)getActualOffset(0xC3E44C))(self, zombie_condition_shrunken);

    if (!self->m_hasLaunched)
    {
        auto* rig = reinterpret_cast<ZombieAnimRig_Bull*>(self->m_animRig.Get());
        HideCustomKnightLayer(self, rig);
        Zombie* spawnedRider = AddZombie(name, -1, 6, -1);

        if (((checkZombieHasCondition)getActualOffset(0xC3E44C))(self, zombie_condition_shrinking) || ((checkZombieHasCondition)getActualOffset(0xC3E44C))(self, zombie_condition_shrunken)) {

            ((setConditionZ)getActualOffset(0xC40CC0))(spawnedRider, zombie_condition_shrunken, 0, 3.4028e38f, 0.0f);
        }

        bool isHypnotized = ((checkZombieHasCondition)getActualOffset(0xC3E44C))(self, zombie_condition_hypnotized);
        float newX = self->m_position.x - props->RiderSpawnOffset.x;
        if (isHypnotized)
        {
            newX = self->m_position.x + props->RiderSpawnOffset.x;
        }
        float newY = self->m_position.y + props->RiderSpawnOffset.y;
        float newZ = self->m_position.z + props->RiderSpawnOffset.z;
        SexyVector3 newCoords = SexyVector3(newX, newY, newZ);
        ((boardEntitySetPosition)getActualOffset(0x628058))(spawnedRider, &newCoords);
        typedef void (*ZombieThrowVirtual)(Zombie*, int, float, float, float, float, float);
        uintptr_t* vtable = *(uintptr_t**)spawnedRider;
        ZombieThrowVirtual virtualThrow = (ZombieThrowVirtual)(vtable[206]);

        float targetX, targetY, targetZ;


        if (isHypnotized) {

            ((setConditionZ)getActualOffset(0xC40CC0))(spawnedRider, zombie_condition_hypnotized, 0, 3.4028e38f, 0.0f);

            int teamflag = self->m_teamFlags;
            typedef void (*func10B013C)(Zombie*, int);
            auto* setTeamFlag = ((func10B013C)getActualOffset(0x10B013C));
            setTeamFlag(spawnedRider, teamflag);
            typedef void* (*GetHypnoDataFunc)(Zombie*);
            GetHypnoDataFunc funGetHypnoData = (GetHypnoDataFunc)getActualOffset(0xC3E6DC);

            void* hypnoData = funGetHypnoData(self);
            typedef void (*ApplyHypnoDataFunc)(Zombie*, void*);
            ApplyHypnoDataFunc funApplyHypnoData = (ApplyHypnoDataFunc)getActualOffset(0xC41290);

            funApplyHypnoData(spawnedRider, hypnoData);

            targetX = newX + distance;
            targetY = self->m_position.y;
            targetZ = self->m_position.z;

            if (targetX > 776.0f) {

                targetX = 776.0f;
            }
        }
        else {
            targetX = newX - distance;
            targetY = self->m_position.y;
            targetZ = self->m_position.z;

            if (targetX < 232.0f) {
                targetX = 232.0f;
            }
        }
        float launchApex = props->LaunchHeight;
        float launchTime = props->LaunchAirTime;
        virtualThrow(spawnedRider, 0, targetX, targetY, targetZ, launchTime, launchApex);
        LanceSpawn(self);
        // this one is prevent veteran bull throw further imp
        self->m_hasLaunched = true;
    }
    else {
        LOGI("Already shrinking, shrunken or being thrown");
    }
}
void CavalryAttack(ZombieDarkCavalry* zombie) {
    auto* props = reinterpret_cast<ZombieDarkCavalryProps*>(zombie->m_propertySheet.Get());
    float damageAmount = props->RiderAttackDamage;
        float zombieX = zombie->m_position.x;
        float zombieY = zombie->m_position.y;
        float zombieZ = zombie->m_position.z;
        Rect riderAttackRect;
        riderAttackRect.mWidth = props->RiderAttackRect.mWidth;
        riderAttackRect.mHeight = props->RiderAttackRect.mHeight;
        riderAttackRect.mX = static_cast<int>(zombieX - (riderAttackRect.mWidth / 2.0f) + props->RiderAttackRect.mX);
        riderAttackRect.mY = static_cast<int>((zombieY - zombieZ) - riderAttackRect.mHeight + props->RiderAttackRect.mY);
        int zombieRow = static_cast<int>((zombieY - 160.0f) / 76.0f);
        std::vector<BoardEntity*> entityList;
        typedef void (*GetEntitiesInRectPixelFunc)(std::vector<BoardEntity*>*, int, Rect*, int, int);
        GetEntitiesInRectPixelFunc getEntitiesRectPixel = (GetEntitiesInRectPixelFunc)getActualOffset(0x86F340);
        getEntitiesRectPixel(&entityList, 38, &riderAttackRect, zombieRow, zombieRow);
        for (BoardEntity* ptr : entityList) {
            if (ptr == nullptr) continue;
            if (ptr->IsType(PlantGroup::StaticGetType()))
            {
                DamageInfo dmg;
                dmg.m_attacker = zombie;
                dmg.m_damage = damageAmount;

                void** vtable = *(void***)ptr;
                typedef void (*VirtualTakeDamageFunc)(PlantGroup*, DamageInfo*);
                VirtualTakeDamageFunc takeDmg = (VirtualTakeDamageFunc)vtable[35];

                takeDmg((PlantGroup*)ptr, &dmg);
            }
            else if (ptr->IsType(ZombieGum::StaticGetType()))
            {
                DamageInfo dmg;
                dmg.m_attacker = zombie;
                dmg.m_damage = damageAmount;

                void** vtable = *(void***)ptr;
                typedef void (*VirtualTakeDamageFunc)(ZombieGum*, DamageInfo*);
                VirtualTakeDamageFunc takeDmg = (VirtualTakeDamageFunc)vtable[35];

                takeDmg((ZombieGum*)ptr, &dmg);
            }
        }
    
}
void overrideCavalryActionFrame(ZombieDarkCavalry* zombie, int64_t unk1, SexyString* actionName, int64_t unk2, SexyString* currentAnim)
{
	if (*actionName == "launch")
	{
        CavalryThrowRider(zombie);
	}
    else if (*actionName == "attack")
    {
        CavalryAttack(zombie);
    }
}
SexyString GetCavalryShockEffectName()
{
	return "POPANIM_EFFECTS_ZOMBIE_CAVALRY_SHOCK";
}
SexyVector3 GetGumOffset(ZombieDarkCavalry* zombie) {
    if (!zombie->m_hasLaunched) {
        return { zombie->m_position.x - 70.0f, zombie->m_position.y, 0.0f };
    }
    return { zombie->m_position.x - 50.0f, zombie->m_position.y, 0.0f };
}
void overrideBullFunction206(ZombieDarkCavalry* zombie) {
    auto* props = reinterpret_cast<ZombieDarkCavalryProps*>(zombie->m_propertySheet.Get());
    if (zombie->m_position.x <= props->BullPawLocation) {
        float zombieX = zombie->m_position.x;
        float zombieY = zombie->m_position.y;
        float zombieZ = zombie->m_position.z;
        Rect lanceRect;
        lanceRect.mWidth = props->LanceRect.mWidth;
        lanceRect.mHeight = props->LanceRect.mHeight;
        lanceRect.mX = static_cast<int>(zombieX - (lanceRect.mWidth / 2.0f) + props->LanceRect.mX);
        lanceRect.mY = static_cast<int>((zombieY - zombieZ) - lanceRect.mHeight + props->LanceRect.mY);
        int zombieRow = static_cast<int>((zombieY - 160.0f) / 76.0f);
        std::vector<BoardEntity*> entityList;
        typedef void (*GetEntitiesInRectPixelFunc)(std::vector<BoardEntity*>*, int, Rect*, int, int);
        GetEntitiesInRectPixelFunc getEntitiesRectPixel = (GetEntitiesInRectPixelFunc)getActualOffset(0x86F340); 
        getEntitiesRectPixel(&entityList, 38, &lanceRect, zombieRow, zombieRow);
        bool foundPlant = false; 
        for (BoardEntity* ptr : entityList) {
            if (ptr != nullptr && ptr->IsType(PlantGroup::StaticGetType()))
            {
                foundPlant = true; 
                break;
            }
        }
        typedef void (*zombieEnterState)(ZombieDarkCavalry*, int, int);
        zombieEnterState enterStateFunc = (zombieEnterState)getActualOffset(0xC3D428);

        if (foundPlant) {
            ((zombieEnterState)getActualOffset(0xC3D428))(zombie, 21, 0);
        }
        else {
            ((zombieEnterState)getActualOffset(0xC3D428))(zombie, 17, 0);
        }
    }
    zombie->m_walkCycled = true;
}
void overrideBullFunction215(ZombieDarkCavalry* zombie) {
    typedef void (*zombieFun215)(ZombieDarkCavalry*);
    ((zombieFun215)getActualOffset(0xAE4818))(zombie);
    zombie->m_walkCycled = true;
}

void ZombieDarkCavalry::AttackOnEnter(ZombieDarkCavalry* zombie)
{
    auto animRig = reinterpret_cast<ZombieAnimRig*>(zombie->m_animRig.Get());
    RtWeakPtr<Zombie> zombiePtr;
    zombiePtr.FromOther((RtWeakPtr<Zombie>*) & zombie->m_thisPtr);

    ZombieEvent zombieEvent;
    ((ConstructEvent)getActualOffset(0x6FDDDC))(&zombieEvent, zombiePtr, "onAttackEnd");

    playAnimWithCallback func = ((playAnimWithCallback)getActualOffset(0x8DCEDC));

    zombie->m_watchAnimHandle = func(animRig, "attack", 3, zombieEvent);
}

void ZombieDarkCavalry::AttackOnLoop(ZombieDarkCavalry* zombie)
{
    auto animRig = reinterpret_cast<ZombieAnimRig*>(zombie->m_animRig.Get());
    bool animDone = ((isAnimDone)getActualOffset(0x9DCBE8))(animRig, zombie->m_watchAnimHandle);

    if (animDone)
    {
        auto* props = reinterpret_cast<ZombieDarkCavalryProps*>(zombie->m_propertySheet.Get());
        float zombieX = zombie->m_position.x;
        float zombieY = zombie->m_position.y;
        float zombieZ = zombie->m_position.z;

        Rect lanceRect;
        lanceRect.mWidth = props->LanceRect.mWidth;
        lanceRect.mHeight = props->LanceRect.mHeight;
        lanceRect.mX = static_cast<int>(zombieX - (lanceRect.mWidth / 2.0f) + props->LanceRect.mX);
        lanceRect.mY = static_cast<int>((zombieY - zombieZ) - lanceRect.mHeight + props->LanceRect.mY);
        int zombieRow = static_cast<int>((zombieY - 160.0f) / 76.0f);

        std::vector<BoardEntity*> entityList;
        typedef void (*GetEntitiesInRectPixelFunc)(std::vector<BoardEntity*>*, int, Rect*, int, int);
        GetEntitiesInRectPixelFunc getEntitiesRectPixel = (GetEntitiesInRectPixelFunc)getActualOffset(0x86F340);
        getEntitiesRectPixel(&entityList, 38, &lanceRect, zombieRow, zombieRow);
        bool hasTarget = false;

        for (BoardEntity* ptr : entityList) {
            if (ptr != nullptr && ptr->IsType(PlantGroup::StaticGetType()) || ptr->IsType(ZombieGum::StaticGetType())) {
                hasTarget = true;
                break;
            }
        }

        if (!hasTarget) {
            ((zombieEnterState)getActualOffset(0xC3D428))(zombie, 17, 0);
        }
        else {
            auto animRig = reinterpret_cast<ZombieAnimRig*>(zombie->m_animRig.Get());
            RtWeakPtr<Zombie> zombiePtr;
            zombiePtr.FromOther((RtWeakPtr<Zombie>*) & zombie->m_thisPtr);

            ZombieEvent zombieEvent;
            ((ConstructEvent)getActualOffset(0x6FDDDC))(&zombieEvent, zombiePtr, "onAttackEnd");

            playAnimWithCallback func = ((playAnimWithCallback)getActualOffset(0x8DCEDC));

            zombie->m_watchAnimHandle = func(animRig, "attack", 3, zombieEvent);
        }
    }
}
void CavalryOnGetCondition(ZombieDarkCavalry* zombie, int conditionID)
{
    if (conditionID == zombie_condition_gummed)
    {
        if (!zombie->m_hasLaunched) {
            ((zombieEnterState)getActualOffset(0xC3D428))(zombie, 21, 0);
        }
        return;
    }

    if (conditionID == zombie_condition_hypnotized)
    {
        ((zombieFlippedAnim)getActualOffset(0xC41290))(zombie, 1);
        ((zombieEnterState)getActualOffset(0xC3D428))(zombie, 20, 0);
        return;
    }
}
void ZombieDarkCavalry::AttackOnExit(ZombieDarkCavalry* zombie)
{

}

void AttackCompletedCallback(Zombie* zombie) {}

void ZombieDarkCavalry::modInit() {
	LOGI("ZombieDarkCavalry init");

    vftable = CreateChildVFTable(220 + 9, getActualOffset(0x23DB6C8), 220);

	PatchVFTable(vftable, (void*)ZombieDarkCavalry::StaticGetType, 0);

	PatchVFTable(vftable, (void*)overrideOnSpawn, 49);

    PatchVFTable(vftable, (void*)CavalryOnGetCondition, 71);

	PatchVFTable(vftable, (void*)overrideCavalryActionFrame, 170);

	PatchVFTable(vftable, (void*)GetCavalryShockEffectName, 189);

    PatchVFTable(vftable, (void*)GetGumOffset, 198);

    PatchVFTable(vftable, (void*)overrideBullFunction206, 206);

    PatchVFTable(vftable, (void*)overrideBullFunction215, 215);

    PatchVFTable(vftable, (void*)ZombieDarkCavalry::AttackOnEnter, 220);
    PatchVFTable(vftable, (void*)ZombieDarkCavalry::AttackOnLoop, 221);
    PatchVFTable(vftable, (void*)ZombieDarkCavalry::AttackOnExit, 222);


	ZombieDarkCavalry::StaticGetType();

	LOGI("ZombieDarkCavalry finish init");
}void ZombieDarkCavalry::buildEventCallbacks(Reflection::CRefManualSymbolBuilder* builder, Reflection::RClass* rtClass)
{
    IF_CALLBACK_NOTSETUP(ZombieDarkCavalry) {
        SetupLiteralDelegate(&attackCompletedDelegate, AttackCompletedCallback);
        ZombieDarkCavalry_delegatesSetup = true;
        LOGI("SO TRUE");
    }
    RegisterEventCallback(builder, rtClass, "onAttackEnd", attackCompletedDelegate);
    LOGI("Reg event complete");
}
void ZombieDarkCavalry::buildStates()
{
    StateMachineTableBuilder* stateMachine = CallGetStateMachine(ZombieDarkCavalry::StaticGetType());
    RegisterStateByOffsets(stateMachine,
        21,
        (uintptr_t)ZombieDarkCavalry::AttackOnEnter,
        (uintptr_t)ZombieDarkCavalry::AttackOnLoop,
        (uintptr_t)ZombieDarkCavalry::AttackOnExit,
        "ZS_DarkCavalry_Attack");
    LOGI("Reg state complete");
}
