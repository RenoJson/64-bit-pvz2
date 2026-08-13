#include "ZombieModernBungee.h"
#include "AddZombieType.h"
#include "ZombieState.h"
#include "StateMachineBuilder.h"
#include "ZombieHelper.h"
#include "ZombieAnimRig_Basic.h"
#include "ZombieStateHelper.h"
#include "ZombieModernBungeeTarget.h"

void* ZombieModernBungee::vftable = nullptr;
Sexy::RtClass* ZombieModernBungee::s_rtClass = nullptr;

void* ZombieModernBungeeProps::vftable = nullptr;
Sexy::RtClass* ZombieModernBungeeProps::s_rtClass = nullptr;

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
float BungeeCalcRenderOrder(ZombieModernBungee* zombie){

}
int64_t BungeeThreatAlert() {
    return 0;
}
bool BungeeCanBeTossedByPlant() {
    return false;
}
void BungeeOnSpawn(ZombieModernBungee* zombie) {
    ZombieOnSpawn(zombie);
    SexyVector3 newPos = {6677.0f, zombie->m_position.y, 0};
    ZombieEnterState(zombie, 16, 0);
    ZombieUpdatePosition(zombie, &newPos);
}
void ZombieModernBungee::HuntOnEnter(ZombieModernBungee* zombie)
{
    if (!zombie->m_hasSpawnTarget) {
        auto props = reinterpret_cast<ZombieModernBungeeProps*>(zombie->m_propertySheet.Get());
        std::vector<BoardEntity*> entityList;
        auto board = Board::GetBoard();

        GetEntitiesInRectPixel(&entityList, 63, &board->m_lawnRect, 0, 4);

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

            zombie->m_target.FromOther(&target->m_thisPtr);
            zombie->m_hasSpawnTarget = true;
        }
    }
}
		

void ZombieModernBungee::HuntOnLoop(ZombieModernBungee* zombie)
{
	auto target = reinterpret_cast<ZombieModernBungeeTarget*>(zombie->m_target.Get());
	if (target->m_targeted) {
		ZombieEnterState(zombie, 17, 0);
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
	
}

void ZombieModernBungee::GrabOnExit(ZombieModernBungee* zombie)
{
}

void ZombieModernBungee::EscapeOnEnter(ZombieModernBungee* zombie)
{
	RegisterEventAfterAnim(zombie, "rise", "onEscapedDone");
}

void ZombieModernBungee::EscapeOnLoop(ZombieModernBungee* zombie)
{
    if (zombie->m_attachedPlant.IsValid())
    {
        auto plant = reinterpret_cast<Plant*>(zombie->m_attachedPlant.Get());
        auto rig = reinterpret_cast<ZombieAnimRig_BasicTemplate*>(zombie->m_animRig.Get());
        auto target = reinterpret_cast<ZombieModernBungeeTarget*>(zombie->m_target.Get());
        SexyVector2 handPos;
        GetAnimRigSpritePosition(rig, "hand_01", &handPos);
        plant->m_position.y = handPos.y;
        target->m_position.y = handPos.y;
        CallVirtualFunc<void>(plant, 13, &plant->m_position);
        ZombieUpdatePosition(target, &target->m_position);
    }
}

void ZombieModernBungee::EscapeOnExit(ZombieModernBungee* zombie)
{
    ZombieSetInvincibleStatusFlag(zombie, false);
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

                CallFunc<void>(0x1271688, p0, 2139095039, 0.0f, 0.0f);
            }
        }

        RegisterEventAfterAnim(zombie, "grab02", "onAttachedDone");
    }
}

void onAttachedCallback(Zombie* zombie) {
    auto bungee = static_cast<ZombieModernBungee*>(zombie);
    if (bungee && !ZombieIsDeadOrDying(zombie) && bungee->m_entityState.m_id != 3) {
        ZombieEnterState(bungee, 20, 0);
    }
}

void onEscapedCallback(Zombie* zombie) {
    auto bungee = static_cast<ZombieModernBungee*>(zombie);
    if (bungee && !ZombieIsDeadOrDying(zombie) && bungee->m_entityState.m_id != 3) {
        auto plant = reinterpret_cast<Plant*>(bungee->m_attachedPlant.Get());
        auto target = reinterpret_cast<ZombieModernBungeeTarget*>(bungee->m_target.Get());
        CallFunc<void>(0x1273244, plant, 0x8000000000LL);
        CallFunc<void>(0x8AEB28, bungee);
        CallFunc<void>(0x8AEB28, target);
    }
}



void ZombieModernBungee::ModInit() {
    LOGI("ZombieBungee mod init");

    vftable = CreateChildVFTable(204 + 15, getActualOffset(0x241D430), 204);
    PatchVFTable(vftable, (void*)ZombieModernBungee::StaticGetType, 0);
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
