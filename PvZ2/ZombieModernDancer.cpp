#include "ZombieModernDancer.h"
#include "AddZombieType.h"
#include "ZombieState.h"
#include "StateMachineBuilder.h"
#include "ZombieHelper.h"
#include "ZombieAnimRig_Basic.h"
#include "ZombieStateHelper.h"

void* ZombieModernDancer::vftable = nullptr;
Sexy::RtClass* ZombieModernDancer::s_rtClass = nullptr;

void* ZombieModernDancerProps::vftable = nullptr;
Sexy::RtClass* ZombieModernDancerProps::s_rtClass = nullptr;

DECLARE_DELEGATES_SETUP(ZombieModernDancer)

static Sexy::DelegateBase danceCompletedDelegate;

static Sexy::DelegateBase summonCompletedDelegate;

static Sexy::DelegateBase waitingCompletedDelegate;


bool IsDancerNeedToStop(ZombieModernDancer* zombie) {
    return ZombieIsInState(zombie, ZS_Eat)
        || ZombieIsInState(zombie, 17)
        || ZombieIsInState(zombie, 18)
        || ZombieIsInState(zombie, 19)
        || ZombieHasCondition(zombie, zombie_condition_freeze)
        || ZombieHasCondition(zombie, zombie_condition_stun)
        || ZombieHasCondition(zombie, zombie_condition_dazeystunned)
        || ZombieHasCondition(zombie, zombie_condition_stickybombed)
        || ZombieHasCondition(zombie, zombie_condition_butter)
        || zombie->IsInGridItem()
        || ZombieHasCondition(zombie, zombie_condition_gummed)
        || (zombie->m_zombieFlags & 0x400) != 0;
}

void DancerOnDestroy(ZombieModernDancer* zombie)
{
    CallFunc<void>(0xC40854, zombie);
    if (zombie->m_isMainDancer) {
        ZombiePlaySoundEvent(zombie, "Play_Disco_Die", 0.0f);
    }
}

void DancerOnSpawn(ZombieModernDancer* zombie) {
    ZombieOnSpawn(zombie);
    if (zombie->m_isMainDancer) {
        ZombieEnterState(zombie, 16, 0);
    }
}
void DancerWalkingOnLoop(ZombieModernDancer* zombie) {
    CallFunc<void>(0xC506B4, zombie);

    auto isZombieStopped = [&](ZombieModernDancer* z) -> bool {
        return z != nullptr && IsDancerNeedToStop(z);
        };

    if (zombie->m_isMainDancer)
    {
        for (int i = 0; i < 4; ++i) {
            if (zombie->m_backupDancer[i].IsValid()) {
                ZombieModernDancer* backup = reinterpret_cast<ZombieModernDancer*>(zombie->m_backupDancer[i].Get());

                if (backup != nullptr && backup->m_teamFlags != zombie->m_teamFlags) {
                    backup->m_mainDancer = RtWeakPtr<Sexy::RtObject>();

                    zombie->m_backupDancer[i] = RtWeakPtr<Sexy::RtObject>();
                }
            }
        }
    }
    else if (zombie->m_mainDancer.IsValid())
    {
        ZombieModernDancer* leader = reinterpret_cast<ZombieModernDancer*>(zombie->m_mainDancer.Get());

        if (leader != nullptr && zombie->m_teamFlags != leader->m_teamFlags) {
            zombie->m_mainDancer = RtWeakPtr<Sexy::RtObject>();
        }
    }

    ZombieModernDancer* groupLeader = nullptr;
    if (zombie->m_isMainDancer) {
        groupLeader = zombie;
    }
    else if (zombie->m_mainDancer.IsValid()) {
        groupLeader = reinterpret_cast<ZombieModernDancer*>(zombie->m_mainDancer.Get());
    }

    bool shouldStop = false;

    if (groupLeader != nullptr)
    {
        if (isZombieStopped(groupLeader)) {
            shouldStop = true;
        }

        if (!shouldStop) {
            for (auto& weakPtr : groupLeader->m_backupDancer) {
                if (weakPtr.IsValid()) {
                    ZombieModernDancer* backupDancer = reinterpret_cast<ZombieModernDancer*>(weakPtr.Get());
                    if (isZombieStopped(backupDancer)) {
                        shouldStop = true;
                        break;
                    }
                }
            }
        }
    }
    else {
        if (isZombieStopped(zombie)) shouldStop = true;
    }

    bool allowMove = !shouldStop;
    ZombieAllowMovement(zombie, allowMove);

    if (groupLeader != nullptr) {
        ZombieAllowMovement(groupLeader, allowMove);
        for (auto& weakPtr : groupLeader->m_backupDancer) {
            if (weakPtr.IsValid()) {
                ZombieModernDancer* backupDancer = reinterpret_cast<ZombieModernDancer*>(weakPtr.Get());
                if (backupDancer != nullptr) ZombieAllowMovement(backupDancer, allowMove);
            }
        }
    }
    auto props = reinterpret_cast<ZombieModernDancerProps*>(zombie->m_propertySheet.Get());
    if (zombie->m_elapsedTimeInState >= props->ActionInterval)
    {
        if (zombie->m_isMainDancer)
        {
            bool needSummon = false;
            float currentY = zombie->m_position.y;
            int zRow = (int)(((currentY - 160.0f) / 76.0f));

            if (zombie->m_backupDancer.size() < 4) {
                needSummon = true;
            }
            else {
                for (int i = 0; i < 4; ++i) {
                    if (!zombie->m_backupDancer[i].IsValid()) {
                        bool isSlotOnBoard = true;
                        if (i == 0 && (zRow - 1 < 0)) isSlotOnBoard = false;
                        if (i == 1 && (zRow + 1 > 4)) isSlotOnBoard = false;

                        if (isSlotOnBoard) {
                            needSummon = true;
                            break;
                        }
                    }
                }
            }

            if (needSummon) {
                ZombieEnterState(zombie, 18, 0);
            }
            else {
                ZombieEnterState(zombie, 17, 0);
            }
        }
        else
        {
            ZombieEnterState(zombie, 17, 0);
        }
    }
}

void DancerEatOnLoop(ZombieModernDancer* zombie)
{
    CallFunc<void>(0xC5082C, zombie);
    auto props = reinterpret_cast<ZombieModernDancerProps*>(zombie->m_propertySheet.Get());
    if (zombie->m_elapsedTimeInState >= props->ActionInterval)
    {
        if (zombie->m_isMainDancer)
        {
            bool needSummon = false;
            float currentY = zombie->m_position.y;
            int zRow = (int)(((currentY - 160.0f) / 76.0f));

            if (zombie->m_backupDancer.size() < 4) {
                needSummon = true;
            }
            else {
                for (int i = 0; i < 4; ++i) {
                    if (!zombie->m_backupDancer[i].IsValid()) {
                        bool isSlotOnBoard = true;
                        if (i == 0 && (zRow - 1 < 0)) isSlotOnBoard = false;
                        if (i == 1 && (zRow + 1 > 4)) isSlotOnBoard = false;

                        if (isSlotOnBoard) {
                            needSummon = true;
                            break;
                        }
                    }
                }
            }

            if (needSummon) {
                ZombieEnterState(zombie, 18, 0);
            }
            else {
                ZombieEnterState(zombie, 17, 0);
            }
        }
        else
        {
            ZombieEnterState(zombie, 17, 0);
        }
    }
}


void DancerOnInitialize(ZombieModernDancer* zombie) {
    auto props = reinterpret_cast<ZombieModernDancerProps*>(zombie->m_propertySheet.Get());
    zombie->m_danceCount = 0;
    zombie->m_isMainDancer = props->IsMainDancer;
    if (zombie->m_isMainDancer) {
        zombie->m_entrySummon = true;
    }
}
void DancerOnHeadDrop(ZombieModernDancer* zombie)
{
    if (zombie->m_isMainDancer) {
        RemoveAttachedEffect(&zombie->m_attachedEffects, "light_on");
    }
}
void ZombieModernDancer::IntroOnEnter(ZombieModernDancer* zombie)
{
	auto rig = reinterpret_cast<ZombieAnimRig_BasicTemplate*>(zombie->m_animRig.Get());
	auto dlgt = RegisterDelegateEvent(zombie, "onWalkAnimationCycle");
	ZombieAllowMovement(zombie, true);
	ZombiePlaySoundEvent(zombie, "Play_Disco_Drop", 0.0f);
    ZombieSetSpeedScale(zombie, 5.0f);
	PlayAndContinueAnim(rig, "entry", 0, dlgt);
}

void ZombieModernDancer::IntroOnLoop(ZombieModernDancer* zombie)
{
	auto entity = CallFunc<BoardEntity*>(0xC41910, zombie);
	if (entity || zombie->m_position.x <= 668.0f && !ZombieIsDeadOrDying(zombie) && zombie->m_entityState.m_id != 3) {
		ZombieEnterState(zombie, 18, 0);
	}
}

void ZombieModernDancer::IntroOnExit(ZombieModernDancer* zombie)
{
    ZombieAllowMovement(zombie, false);
    ZombieSetSpeedScale(zombie, 1.0f);
}

void ZombieModernDancer::DanceOnEnter(ZombieModernDancer* zombie)
{
	auto props = reinterpret_cast<ZombieModernDancerProps*>(zombie->m_propertySheet.Get());
    zombie->m_danceCount = props->DanceCount;
	auto rig = reinterpret_cast<ZombieAnimRig_BasicTemplate*>(zombie->m_animRig.Get());
	auto dlgt = RegisterDelegateEvent(zombie, "onDancingAnimationCycle");
	PlayAndContinueAnim(rig, "wary", 3, dlgt);
}

void ZombieModernDancer::DanceOnLoop(ZombieModernDancer* zombie)
{

}

void ZombieModernDancer::DanceOnExit(ZombieModernDancer* zombie)
{

}

void ZombieModernDancer::SummonOnEnter(ZombieModernDancer* zombie)
{
	SexyString summonAnim = zombie->m_entrySummon ?
		"summon_first_start" :
		"summon_start";
    if (zombie->m_entrySummon) {
        SexyVector3 pos = { 20.0f, -20.0f, 0.0f };
        if (zombie->m_attachedEffects.GetObjectIndex("light_on") == -1) {
            ZombieAttachEffect(zombie,
                "light_on",
                "POPANIM_EFFECTS_ZOMBIE_MODERN_DISCO_EFFECT",
                "idle",
                pos,
                1,
                false,
                false,
                2);
        }
    }
	RegisterEventAfterAnim(zombie, summonAnim, "onSummonStart");
}

void ZombieModernDancer::SummonOnLoop(ZombieModernDancer* zombie)
{

}

void ZombieModernDancer::SummonOnExit(ZombieModernDancer* zombie)
{

}

void ZombieModernDancer::WaitingOnEnter(ZombieModernDancer* zombie)
{
	auto rig = reinterpret_cast<ZombieAnimRig_BasicTemplate*>(zombie->m_animRig.Get());
	auto dlgt = RegisterDelegateEvent(zombie, "onWaitingAnimationCycle");
	PlayAndContinueAnim(rig, "summon_loop", 3, dlgt);
}

void ZombieModernDancer::WaitingOnLoop(ZombieModernDancer* zombie)
{
    auto props = reinterpret_cast<ZombieModernDancerProps*>(zombie->m_propertySheet.Get());

    if (zombie->m_elapsedTimeInState >= 1.0)
    {
        if (zombie->m_entrySummon)
        {
            zombie->m_backupDancer.clear();
            zombie->m_backupDancer.resize(4);
            zombie->m_entrySummon = false;
        }

        float currentX = zombie->m_position.x;
        float currentY = zombie->m_position.y;
        int zRow = (int)(((currentY - 160.0f) / 76.0f));

        if (zombie->m_backupDancer.size() == 4)
        {
            for (int i = 0; i < 4; ++i)
            {
                if (!zombie->m_backupDancer[i].IsValid())
                {
                    int targetRow = zRow;
                    float targetX = currentX;
                    float targetY = currentY;

                    switch (i)
                    {
                    case 0: targetY -= 76.0f; targetRow -= 1; break;
                    case 1: targetY += 76.0f; targetRow += 1; break;
                    case 2: targetX -= 64.0f; break;
                    case 3: targetX += 64.0f; break;
                    }

                    if (targetRow >= 0 && targetRow <= 4)
                    {
                        SexyString selectedTypeName = "";

                        if (!props->DancerSpawnList.empty())
                        {
                            float totalWeight = 0.0f;
                            for (const auto& w : props->DancerSpawnList) {
                                totalWeight += w.Weight;
                            }

                            float randomPoint = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * totalWeight;

                            float currentWeight = 0.0f;
                            for (const auto& w : props->DancerSpawnList) {
                                currentWeight += w.Weight;
                                if (randomPoint <= currentWeight) {
                                    selectedTypeName = w.ZombieTypeName;
                                    break;
                                }
                            }
                        }

                        if (selectedTypeName.empty() && !props->DancerSpawnList.empty()) {
                            selectedTypeName = props->DancerSpawnList[0].ZombieTypeName;
                        }

                        SexyVector3 backupPos = { targetX, targetY, 0 };

                        ZombieModernDancer* backupDancer = (ZombieModernDancer*)AddZombie(selectedTypeName, -1, 6, 1);

                        if (backupDancer != nullptr)
                        {
                            ZombieSetPosition(backupDancer, &backupPos);
                            CallFunc<int64_t>(0xC4CAD4, backupDancer, &backupPos, true);

                            zombie->m_backupDancer[i].FromOther(&backupDancer->m_thisPtr);
                            backupDancer->m_mainDancer.FromOther(&zombie->m_thisPtr);

                            if (ZombieHasCondition(zombie, zombie_condition_shrinking) || ZombieHasCondition(zombie, zombie_condition_shrunken)) {
                                ZombieSetCondition(backupDancer, zombie_condition_shrunken, 0, 3.4028e38f, 0.0f);
                            }

                            if (ZombieHasCondition(zombie, zombie_condition_hypnotized)) {
                                ZombieSetCondition(backupDancer, zombie_condition_hypnotized, 0, 3.4028e38f, 0.0f);

                                int teamflag = zombie->m_teamFlags;
                                typedef void (*func10B013C)(Zombie*, int);
                                auto* setTeamFlag = ((func10B013C)getActualOffset(0x10B013C));
                                setTeamFlag(backupDancer, teamflag);

                                typedef void* (*GetHypnoDataFunc)(Zombie*);
                                GetHypnoDataFunc funGetHypnoData = (GetHypnoDataFunc)getActualOffset(0xC3E6DC);
                                void* hypnoData = funGetHypnoData(zombie);

                                typedef void (*ApplyHypnoDataFunc)(Zombie*, void*);
                                ApplyHypnoDataFunc funApplyHypnoData = (ApplyHypnoDataFunc)getActualOffset(0xC41290);
                                funApplyHypnoData(backupDancer, hypnoData);
                            }
                        }
                    }
                }
            }
        }
    }
}

void ZombieModernDancer::WaitingOnExit(ZombieModernDancer* zombie)
{

}

void onSummonCallback(Zombie* zombie) {
    auto dancer = static_cast<ZombieModernDancer*>(zombie);
    if (dancer && !ZombieIsDeadOrDying(zombie) && dancer->m_entityState.m_id != 3) {
        ZombieEnterState(dancer, 19, 0);
    }
}

void onDancingCallback(Zombie* zombie) {
	auto dancer = static_cast<ZombieModernDancer*>(zombie);
	if (dancer) {
		dancer->m_danceCount -= 1;
		if (dancer->m_danceCount == 0 && !ZombieIsDeadOrDying(zombie) && dancer->m_entityState.m_id != 3) {
			ZombieEnterState(dancer, 1, 0);
		}
	}
}
void onWaitingCallback(Zombie* zombie) {
    auto dancer = static_cast<ZombieModernDancer*>(zombie);
    if (dancer) {
        if (dancer->m_elapsedTimeInState >= 2.0 && !ZombieIsDeadOrDying(dancer) && dancer->m_entityState.m_id != 3) {
            ZombieEnterState(dancer, 1, 0);
        }
    }
}

void* ZombieSpawnWeight::vftable = __null;
Sexy::RtClass* ZombieSpawnWeight::s_rtClass = __null;


void ZombieSpawnWeight::modInit() {
    LOGI("ZombieSpawnWeight init");

    vftable = CopyVFTable(getActualOffset(0x2428660), 14);

    PatchVFTable(vftable, (void*)ZombieSpawnWeight::StaticGetType, 0);

    ZombieSpawnWeight::StaticGetType();

    LOGI("ZombieSpawnWeight finish init");
}

void ZombieModernDancer::ModInit() {
    LOGI("ZombieDancer mod init");

    vftable = CreateChildVFTable(204 + 15, getActualOffset(0x241D430), 204);
    PatchVFTable(vftable, (void*)ZombieModernDancer::StaticGetType, 0);

    PatchVFTable(vftable, (void*)DancerOnDestroy, 12);
    PatchVFTable(vftable, (void*)DancerOnSpawn, 49);
    PatchVFTable(vftable, (void*)DancerWalkingOnLoop, 124);
    PatchVFTable(vftable, (void*)DancerEatOnLoop, 127);
    PatchVFTable(vftable, (void*)DancerOnInitialize, 169);
    PatchVFTable(vftable, (void*)DancerOnHeadDrop, 171);

    PatchVFTable(vftable, (void*)ZombieModernDancer::IntroOnEnter, 204);
    PatchVFTable(vftable, (void*)ZombieModernDancer::IntroOnLoop, 205);
    PatchVFTable(vftable, (void*)ZombieModernDancer::IntroOnExit, 206);

    PatchVFTable(vftable, (void*)ZombieModernDancer::DanceOnEnter, 207);
    PatchVFTable(vftable, (void*)ZombieModernDancer::DanceOnLoop, 208);
    PatchVFTable(vftable, (void*)ZombieModernDancer::DanceOnExit, 209);

    PatchVFTable(vftable, (void*)ZombieModernDancer::SummonOnEnter, 210);
    PatchVFTable(vftable, (void*)ZombieModernDancer::SummonOnLoop, 211);
    PatchVFTable(vftable, (void*)ZombieModernDancer::SummonOnExit, 212);

    PatchVFTable(vftable, (void*)ZombieModernDancer::WaitingOnEnter, 213);
    PatchVFTable(vftable, (void*)ZombieModernDancer::WaitingOnLoop, 214);
    PatchVFTable(vftable, (void*)ZombieModernDancer::WaitingOnExit, 215);
    ZombieModernDancer::StaticGetType();
    LOGI("ZombieDancer finish init");
}

void ZombieModernDancerProps::modInit() {
    LOGI("ZombieDancerProps init");

    vftable = CopyVFTable(getActualOffset(0x24328D0), 14);

    PatchVFTable(vftable, (void*)ZombieModernDancerProps::StaticGetType, 0);

    ZombieModernDancerProps::StaticGetType();

    LOGI("ZombieDancerProps finish init");
}

void ZombieModernDancer::buildEventCallbacks(Reflection::CRefManualSymbolBuilder* builder, Reflection::RClass* rtClass)
{
    IF_CALLBACK_NOTSETUP(ZombieModernDancer) {
        SetupLiteralDelegate(&danceCompletedDelegate, onDancingCallback);
        SetupLiteralDelegate(&summonCompletedDelegate, onSummonCallback);
        SetupLiteralDelegate(&waitingCompletedDelegate, onWaitingCallback);
        ZombieModernDancer_delegatesSetup = true;
        LOGI("SO TRUE");
    }
    RegisterEventCallback(builder, rtClass, "onDancingAnimationCycle", danceCompletedDelegate);
    RegisterEventCallback(builder, rtClass, "onSummonStart", summonCompletedDelegate);
    RegisterEventCallback(builder, rtClass, "onWaitingAnimationCycle", waitingCompletedDelegate);
    LOGI("Reg event complete");
}

void ZombieModernDancer::buildStates()
{
    StateMachineTableBuilder* stateMachine = CallGetStateMachine(ZombieModernDancer::StaticGetType());
    RegisterStateByOffsets(stateMachine,
        16,
        (uintptr_t)ZombieModernDancer::IntroOnEnter,
        (uintptr_t)ZombieModernDancer::IntroOnLoop,
        (uintptr_t)ZombieModernDancer::IntroOnExit,
        "ZS_Dancer_Intro");
    RegisterStateByOffsets(stateMachine,
        17,
        (uintptr_t)ZombieModernDancer::DanceOnEnter,
        (uintptr_t)ZombieModernDancer::DanceOnLoop,
        (uintptr_t)ZombieModernDancer::DanceOnExit,
        "ZS_Dancer_Dancing");
    RegisterStateByOffsets(stateMachine,
        18,
        (uintptr_t)ZombieModernDancer::SummonOnEnter,
        (uintptr_t)ZombieModernDancer::SummonOnLoop,
        (uintptr_t)ZombieModernDancer::SummonOnExit,
        "ZS_Dancer_Summoning");
    RegisterStateByOffsets(stateMachine,
        19,
        (uintptr_t)ZombieModernDancer::WaitingOnEnter,
        (uintptr_t)ZombieModernDancer::WaitingOnLoop,
        (uintptr_t)ZombieModernDancer::WaitingOnExit,
        "ZS_Dancer_Waiting");
    LOGI("Reg state complete");
}