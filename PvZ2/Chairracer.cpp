#include "Chairracer.h"

#include "ZombieConditions.h"
#include <atomic>
#include <mutex>
#include <assert.h>
#include <pch.h>
#include "PvZ2/Zombie.h"

#include "ZcorpRacerZombie.h"
#include "AddZombieType.h"
#include "ZombiePirateBoomBarrel.h"

Reflection::CRefManualSymbolBuilder::BuildSymbolsFunc ZombieZcorpRacerProps::oZombieZcorpRacerPropsBuildSymbols = nullptr;

#pragma region hk Racer Type To Launch

typedef void(*zombieChairThrowRacer)(Zombie*, int);
zombieChairThrowRacer oZombieChairThrowRacer = nullptr;


void hkZombieChairThrowRacer(Zombie * self, int a2)
{

    auto* props = reinterpret_cast<ZombieZcorpRacerProps*>(self->m_propertySheet.Get());

    SexyString racerType = props->RacerType;


    if (racerType.empty()) {
        racerType = "zcorp_racer";

    }

    typedef bool (*checkZombieHasCondition)(Zombie*, int); 
    checkZombieHasCondition hasCondition = (checkZombieHasCondition)getActualOffset(0xC3E44C);

    typedef void (*setConditionZ)(Zombie*, int, int, float, float);
    setConditionZ setCondition = (setConditionZ)getActualOffset(0xC40CC0);

    bool hasThrown = *(bool*)((uintptr_t)self + 0x448);


    if (!hasThrown) {


        typedef uintptr_t(*getChairRacerZombieAnimRig)(Zombie*);
        uintptr_t animRig = ((getChairRacerZombieAnimRig)getActualOffset(0xBEFD4C))(self);

        float tileDistance = props->LaunchZombieDistance;

        
        ZombieImp* spawnedRider = (ZombieImp*)AddZombie(racerType, -1, 6, -1);

        spawnedRider->m_getsUpFromLanding = true;

        if (hasCondition(self, zombie_condition_shrinking) || hasCondition(self, zombie_condition_shrunken)) {

            setCondition(spawnedRider, zombie_condition_shrunken, 0, 3.4028e38f, 0.0f);
        }

        float launchDistance = tileDistance * 64.0f;

        float currentX = self->m_position.x;
        float currentY = self->m_position.y;
        float currentZ = self->m_position.z;

        typedef SexyVector3 (*boardEntitySetPosition)(Zombie*, SexyVector3*);
        boardEntitySetPosition funBoardEntitySetPosition = (boardEntitySetPosition)getActualOffset(0x628058);
        SexyVector3 position = SexyVector3(currentX, currentY, currentZ);
        SexyVector3 updatePos = funBoardEntitySetPosition(spawnedRider, &position);

        float targetX = currentX - launchDistance;
        uintptr_t* vtable = *(uintptr_t**)spawnedRider;

        if (hasCondition(self, zombie_condition_hypnotized)) {

            setCondition(spawnedRider, zombie_condition_hypnotized, 0, 3.4028e38f, 0.0f);

            int valAtOffset36 = *(int*)((uintptr_t)self + 0x24);
            typedef void (*func10B013C)(Zombie*, int);
            auto* setTeamFlag = ((func10B013C)getActualOffset(0x10B013C));
            setTeamFlag(spawnedRider, valAtOffset36);
            LOGI("Clear 1");
            typedef void* (*GetHypnoDataFunc)(Zombie*);
            GetHypnoDataFunc funGetHypnoData = (GetHypnoDataFunc)getActualOffset(0xC3E6DC);

            void* hypnoData = funGetHypnoData(self);
            LOGI("Clear 2");
            typedef void (*ApplyHypnoDataFunc)(Zombie*, void*);
            ApplyHypnoDataFunc funApplyHypnoData = (ApplyHypnoDataFunc)getActualOffset(0xC41290);

            funApplyHypnoData(spawnedRider, hypnoData);
            LOGI("Clear 3");

            targetX = launchDistance + currentX;
            if (targetX > 776.0f) {
                targetX = 776.0f;
            }
        }
        else {
            if (targetX < 232.0f) {
                targetX = 232.0f;
            }
        }

        typedef void (*ZombieThrowVirtual)(Zombie*, int, float, float, float, float, float);


        ZombieThrowVirtual virtualThrow = (ZombieThrowVirtual)(vtable[206]);

        float launchApex =props->LaunchZombieApex;
        float launchTime = props->LaunchZombieTimeToLand;

        virtualThrow(spawnedRider, 0, targetX, currentY, currentZ, launchTime, launchApex);
        // Mark as launched
        *(bool*)((uintptr_t)self + 0x448) = true;
    }
}

typedef void* (*initZcorpRacerArmList)();
initZcorpRacerArmList oInitZcorpRacerArmList = NULL;

void* hkInitZcorpRacerArmList() {

    static std::vector<SexyString> racerLowerArmList = {
       "zombie_arm_outer_lower",
       "zombie_arm_outer_lower01",
       "zombie_hand_outer_01",
       "zombie_hand_outer_02",
       "zombie_hand_outer_03"
    };
    return &racerLowerArmList;
}

typedef void* (*initZcorpRacerUpperArmList)(uintptr_t*);
initZcorpRacerUpperArmList oInitZcorpRacerUpperArmList = NULL;

void* hkInitZcorpRacerUpperArmList(uintptr_t* rig) {

    if (!rig) return  oInitZcorpRacerUpperArmList(rig);

    uintptr_t vtable = *(uintptr_t*)rig;

    bool isZombieRacer = (vtable == getActualOffset(0x2415A08));

    if (isZombieRacer) {

        static std::vector<SexyString> racerList = {
            "zombie_arm_outer_upper",
            "zombie_arm_outer_upper_bone"
        };
        return &racerList;
    }
    else {
        return oInitZcorpRacerUpperArmList(rig);
    }
}

bool hkAddZombieToAudioGroup(Zombie* imp)
{
    typedef bool (*checkZombieHasCondition)(Zombie*, int);
    checkZombieHasCondition hasCondition = (checkZombieHasCondition)getActualOffset(0xC3E44C);
    typedef bool (*isInState)(Zombie*, int);
    isInState IsInState = (isInState)getActualOffset(0xC3E43C);
    typedef bool (*isDeadOrDying)(Zombie*);
    isDeadOrDying isDead = (isDeadOrDying)getActualOffset(0xC3E204);
    uintptr_t* vtable = *reinterpret_cast<uintptr_t**>(imp);
    bool isZombieRacer = (vtable == reinterpret_cast<uintptr_t*>(getActualOffset(0x2415298)));
    bool isPirateBomber = (vtable == ZombiePirateBoomBarrel::vftable);
    return !IsInState(imp, 16)
        && !IsInState(imp, 19)
        && !isDead(imp)
        && !IsInState(imp, -1)
        && !hasCondition(imp, 31) // these three are the imp stuck in GI
        && !hasCondition(imp, 34)
        && !hasCondition(imp, 60)
        && !isZombieRacer
        && !isPirateBomber;
}
#pragma endregion


void ZombieZcorpRacerProps::modInit() {
    LOGI("init chair class");
    PVZ2HookFunction(0xBEFEE0, (void*)hkZombieChairThrowRacer, (void**)&oZombieChairThrowRacer);
    PVZ2HookFunction(0xBEEC10, (void*)ZombieZcorpRacerProps::construct, nullptr);
    PVZ2HookFunction(0xB57128, (void*)hkAddZombieToAudioGroup, nullptr);
    LOGI("init chair class complete");
    LOGI("init chair props");
    PVZ2HookFunction(0xBEED50, (void*)ZombieZcorpRacerProps::buildSymbols, (void**)&oZombieZcorpRacerPropsBuildSymbols);
    LOGI("init chair props complete");
    LOGI("init chair rig");
    PVZ2HookFunction(0xBF1998, (void*)hkInitZcorpRacerArmList, (void**)&oInitZcorpRacerArmList);
    PVZ2HookFunction(0xB57768, (void*)hkInitZcorpRacerUpperArmList, (void**)&oInitZcorpRacerUpperArmList);
    LOGI("init chair rig complete");
    LOGI("finish chair init");
}
