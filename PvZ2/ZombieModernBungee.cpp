#include "ZombieModernBungee.h"
#include "AddZombieType.h"
#include "ZombieState.h"
#include "StateMachineBuilder.h"
#include "ZombieHelper.h"
#include "ZombieAnimRig_Basic.h"
#include "ZombieStateHelper.h"
#include "ZombieModernBungeeTarget.h"
#include "Messages.h"
#include "MessageRouter.h"

void* ZombieModernBungee::vftable = nullptr;
void* ZombieModernBungee::vftable1 = nullptr;
Sexy::RtClass* ZombieModernBungee::s_rtClass = nullptr;

void* ZombieModernBungeeProps::vftable = nullptr;
Sexy::RtClass* ZombieModernBungeeProps::s_rtClass = nullptr;

DECLARE_DELEGATES_SETUP(ZombieModernBungee)

static Sexy::DelegateBase fallingCompletedDelegate;

static Sexy::DelegateBase grabCompletedDelegate;

static Sexy::DelegateBase escapeCompletedDelegate;

bool BungeeCanBeTargeted(ZombieModernBungee* zombie, char a2) {
    if (ZombieIsInState(zombie, 16)
        || ZombieIsInState(zombie, 17)
        || ZombieIsInState(zombie, 20)) {
        return false;
    }
    else {
        return CallFunc<bool>(0xC4D594, zombie, a2);
    }
}

int BungeeCalcRenderOrder(void* renderableThis) {

    auto zombie = reinterpret_cast<ZombieModernBungee*>((uintptr_t)renderableThis - 16);

    if (!zombie->m_targetedPlant.IsValid()) {

        return CallFunc<int>(0xC4D804, zombie);
    }
    else {
        auto pGroup = reinterpret_cast<PlantGroup*>(zombie->m_targetedPlant.Get());
        return MakeRenderOrder(402000, pGroup->m_gridY + 1, 4294967291LL);
    }
}

void BungeeOnSpawn(ZombieModernBungee* zombie) {
    ZombieOnSpawn(zombie);
    SexyVector3 newPos = {6677.0f, zombie->m_position.y, 0};
    ZombieEnterState(zombie, 16, 0);
    ZombieUpdatePosition(zombie, &newPos);
}

int64_t BungeeThreatAlert() {
    return 0;
}


void BungeeOnBeforeBlown(ZombieModernBungee* zombie, Plant* plant) {
    ZombieIsFlying(zombie, false);
    ZombieSetUnmovableStatusFlag(zombie, false);
    ZombieEnterState(zombie, 4, 0); 
    if (zombie->m_target.IsValid())
    {
        auto target = reinterpret_cast<ZombieModernBungeeTarget*>(zombie->m_target.Get());
        ZombieEnterState(target, 4, 0);
    }
    CallFunc<void>(0xC47E24, zombie, damage_fatal);
}

bool BungeeCanBeTargetedByPlant(ZombieModernBungee* zombie, RtWeakPtr<PlantType> pType) {
    if (ZombieIsInState(zombie, 16) 
        || ZombieIsInState(zombie, 17) 
        || ZombieIsInState(zombie, 20)) {
        return false;
    }
    if (pType.IsValid()) {
        auto type = reinterpret_cast<PlantType*>(pType.Get());
        if (type != nullptr && type->TypeName == "thymewarp") {
            return false;
        }
    }
    return CallFunc<bool>(0xC5677C, zombie, pType);
}

bool BungeeCanBeTossedByPlant() {
    return false;
}


void BungeeOnInitialize(ZombieModernBungee* zombie) {
    zombie->m_hasSpawnTarget = false;
    ZombieIsFlying(zombie, true);
}


void BungeeOnElectrocute(ZombieModernBungee* zombie) {
    RemoveAttachedEffect(&zombie->m_attachedEffects, "hand");
    RemoveAttachedEffect(&zombie->m_attachedEffects, "riseHand");
    CallFunc<void>(0xC51FB0, zombie);
}

void BungeeOnAsh(ZombieModernBungee* zombie) {
    RemoveAttachedEffect(&zombie->m_attachedEffects, "hand");
    RemoveAttachedEffect(&zombie->m_attachedEffects, "riseHand");
    CallFunc<void>(0xC5274C, zombie);
}

void BungeeOnTakeFatalDamage(ZombieModernBungee* zombie, DamageInfo* dmgInfo) {
    if (zombie->m_target.IsValid())
    {
        auto target = reinterpret_cast<ZombieModernBungeeTarget*>(zombie->m_target.Get());
        ZombieEnterState(target, 4, 0);
    }
    uint64_t SPECIAL_DEATH_MASK = damage_instantly_fatal | damage_lightning | damage_ash_death |
        damage_mower | damage_plantify_on_death | damage_no_bleed_on_death;
    if ((dmgInfo->m_flags & SPECIAL_DEATH_MASK) == 0)
    {
        CallFunc<void>(0xC47E24, zombie, damage_fatal);
    }
}
SexyString GetBungeeElectrocuteAnimName() {
    return "POPANIM_EFFECTS_ZOMBIE_BUNGEE_SHOCK";
}

SexyString GetBungeeAshAnimName() {
    return "POPANIM_EFFECTS_ZOMBIE_BUNGEE_ASH";
}

void BungeeOnDeath(ZombieModernBungee* zombie) {
    RemoveAttachedEffect(&zombie->m_attachedEffects, "hand");
    RemoveAttachedEffect(&zombie->m_attachedEffects, "riseHand");
    CallFunc<void>(0xC51A40, zombie);
}

bool BungeeIsImmuneToShrinking() {
    return true;
}

void ZombieModernBungee::HuntOnEnter(ZombieModernBungee* zombie)
{
    if (!zombie->m_hasSpawnTarget) {
        auto props = reinterpret_cast<ZombieModernBungeeProps*>(zombie->m_propertySheet.Get());
        std::vector<BoardEntity*> entityList;

        Rect boardRect;
        boardRect.mX = 200;
        boardRect.mY = 160;
        boardRect.mWidth = 576;
        boardRect.mHeight = 380;

        GetEntitiesInRectPixel(&entityList, 63, &boardRect, 0, 4);

        std::vector<PlantGroup*> validTargets;

        for (BoardEntity* entity : entityList) {
            if (entity != nullptr && entity->IsType(PlantGroup::StaticGetType())) {
                auto pGroup = static_cast<PlantGroup*>(entity);
                auto& plantVector = pGroup->m_plants.m_plants;

                if (plantVector.empty()) continue;

                auto& layer0WeakPtr = plantVector[0];

                if (layer0WeakPtr.IsValid()) {
                    Plant* p0 = reinterpret_cast<Plant*>(layer0WeakPtr.Get());

                    if (p0 != nullptr) {
                        auto plantType = reinterpret_cast<PlantType*>(p0->m_type.Get());
                        SexyString plantName = plantType->TypeName;
                        std::vector<SexyString>& bungeeExcludeTarget = props->BungeeTargetExcludeList;

                        if (std::find(bungeeExcludeTarget.begin(), bungeeExcludeTarget.end(), plantName) == bungeeExcludeTarget.end()) {
                            validTargets.push_back(pGroup);
                        }
                    }
                }
            }
        }

        if (!validTargets.empty()) {
            int randomIndex = rand() % validTargets.size();
            PlantGroup* selectedGroup = validTargets[randomIndex];

            zombie->m_targetedPlant.FromOther(&selectedGroup->m_thisPtr);

            auto type = reinterpret_cast<ZombieType*>(zombie->m_type.Get());
            SexyString targetTypeName = type->TypeName + "_target";
            ZombieModernBungeeTarget* target = (ZombieModernBungeeTarget*)AddZombie(targetTypeName, -1, 6, -1);

            ZombieSetPosition(target, &selectedGroup->m_position);
            ZombieEnterState(target, 16, 0);
            zombie->m_target.FromOther(&target->m_thisPtr);
            zombie->m_hasSpawnTarget = true;
        }
    }
}
		

void ZombieModernBungee::HuntOnLoop(ZombieModernBungee* zombie)
{
    if (zombie->m_target.IsValid())
    {
        auto target = reinterpret_cast<ZombieModernBungeeTarget*>(zombie->m_target.Get());

        if (target != nullptr && target->m_targeted) {
            ZombieEnterState(zombie, 17, 0);
        }
    }
    else
    {
        ZombieModernBungee::HuntOnEnter(zombie);
    }
}

void ZombieModernBungee::HuntOnExit(ZombieModernBungee* zombie)
{
	
}

void ZombieModernBungee::FallOnEnter(ZombieModernBungee* zombie)
{
    auto target = reinterpret_cast<ZombieModernBungeeTarget*>(zombie->m_target.Get());
	ZombieUpdatePosition(zombie, &target->m_position);
    int randomVariant = (rand() % 3) + 1;
    SexyString soundEvent = "Zombie_Bungee_Scream" + std::to_string(randomVariant);
    ZombiePlaySoundEvent(zombie, soundEvent, 0.0f);
	RegisterEventAfterAnim(zombie, "fall", "onFallingDown");
}

void ZombieModernBungee::FallOnLoop(ZombieModernBungee* zombie)
{
}

void ZombieModernBungee::FallOnExit(ZombieModernBungee* zombie)
{

}

void ZombieModernBungee::WaitingOnEnter(ZombieModernBungee* zombie)
{
	auto dlgtEvent = RegisterDelegateEvent(zombie, "onIdleAnimationCycle");
	auto rig = reinterpret_cast<ZombieAnimRig_BasicTemplate*>(zombie->m_animRig.Get());
	PlayAndContinueAnim(rig, "idle", 3, dlgtEvent);
}

void ZombieModernBungee::WaitingOnLoop(ZombieModernBungee* zombie)
{
    auto props = reinterpret_cast<ZombieModernBungeeProps*>(zombie->m_propertySheet.Get());

    if (zombie->m_elapsedTimeInState >= props->PlantLiftingWaitTime)
    {
        if (!zombie->m_targetedPlant.IsValid())
        {
            std::vector<BoardEntity*> entityList;
            GetEntitiesInGridSquare(entityList, 63, zombie);

            bool foundNewPlant = false; 

            for (BoardEntity* entity : entityList)
            {
                if (entity != nullptr && entity->IsType(PlantGroup::StaticGetType()))
                {
                    auto pGroup = static_cast<PlantGroup*>(entity);
                    auto& plantVector = pGroup->m_plants.m_plants;

                    if (!plantVector.empty() && plantVector[0].IsValid())
                    {
                        Plant* p0 = reinterpret_cast<Plant*>(plantVector[0].Get());
                        auto plantType = reinterpret_cast<PlantType*>(p0->m_type.Get());
                        SexyString plantName = plantType->TypeName;
                        std::vector<SexyString>& bungeeExcludeTarget = props->BungeeTargetExcludeList;

                        if (std::find(bungeeExcludeTarget.begin(), bungeeExcludeTarget.end(), plantName) == bungeeExcludeTarget.end())
                        {
                            zombie->m_targetedPlant.FromOther(&pGroup->m_thisPtr);
                            foundNewPlant = true; 
                            break;
                        }
                    }
                }
            }

            if (foundNewPlant) {
                ZombieEnterState(zombie, 19, 0);
            }
        }
        else
        {
            ZombieEnterState(zombie, 19, 0);
        }
    }
}

void ZombieModernBungee::WaitingOnExit(ZombieModernBungee* zombie)
{
}

void ZombieModernBungee::GrabOnEnter(ZombieModernBungee* zombie)
{
    RegisterEventAfterAnim(zombie, "grab", "onGrabbingDone");
}

void ZombieModernBungee::GrabOnLoop(ZombieModernBungee* zombie)
{
    if (!zombie->m_targetedPlant.IsValid())
    {
        auto props = reinterpret_cast<ZombieModernBungeeProps*>(zombie->m_propertySheet.Get());
        std::vector<BoardEntity*> entityList;
        GetEntitiesInGridSquare(entityList, 63, zombie);

        for (BoardEntity* entity : entityList)
        {
            if (entity != nullptr && entity->IsType(PlantGroup::StaticGetType()))
            {
                auto pGroup = static_cast<PlantGroup*>(entity);
                auto& plantVector = pGroup->m_plants.m_plants;

                if (!plantVector.empty() && plantVector[0].IsValid())
                {
                    Plant* p0 = reinterpret_cast<Plant*>(plantVector[0].Get());
                    auto plantType = reinterpret_cast<PlantType*>(p0->m_type.Get());
                    SexyString plantName = plantType->TypeName;
                    std::vector<SexyString>& bungeeExcludeTarget = props->BungeeTargetExcludeList;

                    if (std::find(bungeeExcludeTarget.begin(), bungeeExcludeTarget.end(), plantName) == bungeeExcludeTarget.end())
                    {
                        zombie->m_targetedPlant.FromOther(&pGroup->m_thisPtr);
                        break;
                    }
                }
            }
        }
    }
}

void ZombieModernBungee::GrabOnExit(ZombieModernBungee* zombie)
{
}

void ZombieModernBungee::EscapeOnEnter(ZombieModernBungee* zombie)
{
    RemoveAttachedEffect(&zombie->m_attachedEffects, "hand");
    if (zombie->m_attachedPlant.IsValid())
    {
        auto plant = reinterpret_cast<Plant*>(zombie->m_attachedPlant.Get());
        SexyVector3 effectPos = { -7.0f, -35.0f, 0.0f };
        int renderOrder = MakeRenderOrder(402000, plant->m_position.y, 1);
        ZombieAttachEffect(zombie, "riseHand", "POPANIM_ZOMBIE_ZOMBIE_BUNGEE", "03", effectPos, renderOrder, true, true, 2);
    }
	RegisterEventAfterAnim(zombie, "rise", "onEscapedDone");
}

void ZombieModernBungee::EscapeOnLoop(ZombieModernBungee* zombie)
{
    if (zombie->m_attachedPlant.IsValid())
    {
        auto plant = reinterpret_cast<Plant*>(zombie->m_attachedPlant.Get());
        auto rig = reinterpret_cast<ZombieAnimRig_BasicTemplate*>(zombie->m_animRig.Get());
        auto pProps = reinterpret_cast<PlantPropertySheet*>(plant->m_propertySheet.Get());

        SexyVector2 headPos;
        GetAnimRigSpritePosition(rig, "tou01", &headPos);

        plant->m_position.y = headPos.y + zombie->m_position.y - (pProps->ArtCenter.y / 2.0f);

        CallVirtualFunc<void>(plant, 13, &plant->m_position);
        if (zombie->m_target.IsValid()) {
            auto target = reinterpret_cast<ZombieModernBungeeTarget*>(zombie->m_target.Get());
            target->m_position.y = plant->m_position.y;
            ZombieUpdatePosition(target, &target->m_position);
        }
    }
}

void ZombieModernBungee::EscapeOnExit(ZombieModernBungee* zombie)
{
    ZombieSetInvincibleStatusFlag(zombie, false);
    ZombieSetNoCollisionFlag(zombie, false);
}

void onFallingCallback(Zombie* zombie) {
    auto bungee = static_cast<ZombieModernBungee*>(zombie);
    if (bungee && !ZombieIsDeadOrDying(zombie) && bungee->m_entityState.m_id != 3) {
        ZombieEnterState(bungee, 18, 0);
    }
}

void onGrabCallback(Zombie* zombie) {
    auto bungee = static_cast<ZombieModernBungee*>(zombie);

    if (bungee && !ZombieIsDeadOrDying(zombie) && bungee->m_entityState.m_id != 3) {
        ZombieSetInvincibleStatusFlag(bungee, true);
        ZombieSetNoCollisionFlag(bungee, true);
        if (!bungee->m_targetedPlant.IsValid()) {
            auto props = reinterpret_cast<ZombieModernBungeeProps*>(bungee->m_propertySheet.Get());
            std::vector<BoardEntity*> entityList;
            GetEntitiesInGridSquare(entityList, 63, bungee);

            for (BoardEntity* entity : entityList) {
                if (entity != nullptr && entity->IsType(PlantGroup::StaticGetType())) {
                    auto pGroup = static_cast<PlantGroup*>(entity);
                    auto& plantVector = pGroup->m_plants.m_plants;

                    if (!plantVector.empty() && plantVector[0].IsValid()) {
                        Plant* p0 = reinterpret_cast<Plant*>(plantVector[0].Get());
                        auto plantType = reinterpret_cast<PlantType*>(p0->m_type.Get());
                        SexyString plantName = plantType->TypeName;
                        std::vector<SexyString>& bungeeExcludeTarget = props->BungeeTargetExcludeList;

                        if (std::find(bungeeExcludeTarget.begin(), bungeeExcludeTarget.end(), plantName) == bungeeExcludeTarget.end()) {
                            bungee->m_targetedPlant.FromOther(&pGroup->m_thisPtr);
                            break;
                        }
                    }
                }
            }
        }

        if (bungee->m_targetedPlant.IsValid()) {
            auto pGroup = reinterpret_cast<PlantGroup*>(bungee->m_targetedPlant.Get());
            auto& plantVector = pGroup->m_plants.m_plants;

            if (!plantVector.empty() && plantVector[0].IsValid()) {
                Plant* p0 = reinterpret_cast<Plant*>(plantVector[0].Get());

                bungee->m_attachedPlant.FromOther(&p0->m_thisPtr);
                auto plant = reinterpret_cast<Plant*>(bungee->m_attachedPlant.Get());
                SexyVector3 effectPos = { -7.0f, -35.0f, 0.0f };
                int renderOrder = MakeRenderOrder(402000, p0->m_position.y, 1);
                ZombieAttachEffect(bungee, "hand", "POPANIM_ZOMBIE_ZOMBIE_BUNGEE", "02", effectPos, renderOrder, false, false, 2);
                CallFunc<void, Plant*, unsigned int, float, float, float>(0x1271688, plant, 14, FLT_MAX, 0.0f, 0.0f);
                CallFunc<void>(0x1276D04, plant);
            }
        }
        ZombieEnterState(bungee, 20, 0);
    }
}


void onEscapedCallback(Zombie* zombie) {
    auto bungee = static_cast<ZombieModernBungee*>(zombie);

    if (bungee && !ZombieIsDeadOrDying(zombie) && bungee->m_entityState.m_id != 3) {

        // alway check attached plant and target plant is valid or we are cooked
        if (bungee->m_attachedPlant.IsValid()) {
            auto plant = reinterpret_cast<Plant*>(bungee->m_attachedPlant.Get());


            if (plant) {
                CallFunc<void>(0x1273244, plant, 0x8000000000LL);
            }
        }

        if (bungee->m_target.IsValid()) {
            auto target = reinterpret_cast<ZombieModernBungeeTarget*>(bungee->m_target.Get());
            if (target != nullptr) {
                CallFunc<void>(0x8AEB28, target);
            }
        }

        CallFunc<void>(0x8AEB28, bungee);
    }
}

void ZombieModernBungee::ModInit() {
    LOGI("ZombieBungee mod init");

    vftable = CreateChildVFTable(204 + 15, getActualOffset(0x241D430), 204);
    vftable1 = CopyVFTable(getActualOffset(0x241DAA0), 4);
    PatchVFTable(vftable, (void*)ZombieModernBungee::StaticGetType, 0);

    PatchVFTable(vftable, (void*)BungeeCanBeTargeted, 21);
    PatchVFTable(vftable1, (void*)BungeeCalcRenderOrder, 3);
    PatchVFTable(vftable, (void*)BungeeOnSpawn, 49);
    PatchVFTable(vftable, (void*)BungeeThreatAlert, 75);
    PatchVFTable(vftable, (void*)BungeeOnBeforeBlown, 79);
    PatchVFTable(vftable, (void*)BungeeCanBeTargetedByPlant, 93);
    PatchVFTable(vftable, (void*)BungeeCanBeTossedByPlant, 97);
    PatchVFTable(vftable, (void*)BungeeOnInitialize, 169);
    PatchVFTable(vftable, (void*)BungeeOnElectrocute, 172);
    PatchVFTable(vftable, (void*)BungeeOnAsh, 173);
    PatchVFTable(vftable, (void*)BungeeOnTakeFatalDamage, 185);
    PatchVFTable(vftable, (void*)GetBungeeElectrocuteAnimName, 189);
    PatchVFTable(vftable, (void*)GetBungeeAshAnimName, 190);
    PatchVFTable(vftable, (void*)BungeeOnDeath, 197);
    PatchVFTable(vftable, (void*)BungeeIsImmuneToShrinking, 199);

    PatchVFTable(vftable, (void*)ZombieModernBungee::HuntOnEnter, 204);
    PatchVFTable(vftable, (void*)ZombieModernBungee::HuntOnLoop, 205);
    PatchVFTable(vftable, (void*)ZombieModernBungee::HuntOnExit, 206);

    PatchVFTable(vftable, (void*)ZombieModernBungee::FallOnEnter, 207);
    PatchVFTable(vftable, (void*)ZombieModernBungee::FallOnLoop, 208);
    PatchVFTable(vftable, (void*)ZombieModernBungee::FallOnExit, 209);

    PatchVFTable(vftable, (void*)ZombieModernBungee::WaitingOnEnter, 210);
    PatchVFTable(vftable, (void*)ZombieModernBungee::WaitingOnLoop, 211);
    PatchVFTable(vftable, (void*)ZombieModernBungee::WaitingOnExit, 212);

    PatchVFTable(vftable, (void*)ZombieModernBungee::GrabOnEnter, 213);
    PatchVFTable(vftable, (void*)ZombieModernBungee::GrabOnLoop, 214);
    PatchVFTable(vftable, (void*)ZombieModernBungee::GrabOnExit, 215);

    PatchVFTable(vftable, (void*)ZombieModernBungee::EscapeOnEnter, 216);
    PatchVFTable(vftable, (void*)ZombieModernBungee::EscapeOnLoop, 217);
    PatchVFTable(vftable, (void*)ZombieModernBungee::EscapeOnExit, 218);

    ZombieModernBungee::StaticGetType();
    LOGI("ZombieBungee finish init");
}

void ZombieModernBungeeProps::modInit() {
    LOGI("ZombieBungeeProps init");

    vftable = CopyVFTable(getActualOffset(0x24328D0), 14);

    PatchVFTable(vftable, (void*)ZombieModernBungeeProps::StaticGetType, 0);

    ZombieModernBungeeProps::StaticGetType();

    LOGI("ZombieBungeeProps finish init");
}

void ZombieModernBungee::buildEventCallbacks(Reflection::CRefManualSymbolBuilder* builder, Reflection::RClass* rtClass)
{
    IF_CALLBACK_NOTSETUP(ZombieModernBungee) {
        SetupLiteralDelegate(&fallingCompletedDelegate, onFallingCallback);
        SetupLiteralDelegate(&grabCompletedDelegate, onGrabCallback);
        SetupLiteralDelegate(&escapeCompletedDelegate, onEscapedCallback);
        ZombieModernBungee_delegatesSetup = true;
        LOGI("SO TRUE");
    }
    RegisterEventCallback(builder, rtClass, "onFallingDown", fallingCompletedDelegate);
    RegisterEventCallback(builder, rtClass, "onGrabbingDone", grabCompletedDelegate);
    RegisterEventCallback(builder, rtClass, "onEscapedDone", escapeCompletedDelegate);
    LOGI("Reg event complete");
}


void ZombieModernBungee::buildStates()
{
    StateMachineTableBuilder* stateMachine = CallGetStateMachine(ZombieModernBungee::StaticGetType());
    RegisterStateByOffsets(stateMachine,
        16,
        (uintptr_t)ZombieModernBungee::HuntOnEnter,
        (uintptr_t)ZombieModernBungee::HuntOnLoop,
        (uintptr_t)ZombieModernBungee::HuntOnExit,
        "ZS_Bungee_Hunting");
    RegisterStateByOffsets(stateMachine,
        17,
        (uintptr_t)ZombieModernBungee::FallOnEnter,
        (uintptr_t)ZombieModernBungee::FallOnLoop,
        (uintptr_t)ZombieModernBungee::FallOnExit,
        "ZS_Bungee_Falling");
    RegisterStateByOffsets(stateMachine,
        18,
        (uintptr_t)ZombieModernBungee::WaitingOnEnter,
        (uintptr_t)ZombieModernBungee::WaitingOnLoop,
        (uintptr_t)ZombieModernBungee::WaitingOnExit,
        "ZS_Bungee_Waiting");
    RegisterStateByOffsets(stateMachine,
        19,
        (uintptr_t)ZombieModernBungee::GrabOnEnter,
        (uintptr_t)ZombieModernBungee::GrabOnLoop,
        (uintptr_t)ZombieModernBungee::GrabOnExit,
        "ZS_Bungee_Grab");
    RegisterStateByOffsets(stateMachine,
        20,
        (uintptr_t)ZombieModernBungee::EscapeOnEnter,
        (uintptr_t)ZombieModernBungee::EscapeOnLoop,
        (uintptr_t)ZombieModernBungee::EscapeOnExit,
        "ZS_Bungee_Escape");
    LOGI("Reg state complete");
}
