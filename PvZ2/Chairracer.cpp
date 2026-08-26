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
#include "ZombieBull.h"
#include "ZombieHelper.h"

Reflection::CRefManualSymbolBuilder::BuildSymbolsFunc ZombieZcorpRacerProps::oZombieZcorpRacerPropsBuildSymbols = nullptr;

#pragma region hk Racer Type To Launch

void hkZombieChairThrowRacer(ZombieZCorpRacer* self)
{

    auto* props = reinterpret_cast<ZombieZcorpRacerProps*>(self->m_propertySheet.Get());

    SexyString racerType = props->RacerType;


    if (racerType.empty()) {
        racerType = "zcorp_racer";

    }

    if (!self->m_hasLaunched) {

        float tileDistance = props->LaunchZombieDistance;

        ZombieImp* spawnedRider = (ZombieImp*)AddZombie(racerType, -1, 6, -1);

        spawnedRider->m_getsUpFromLanding = true;

        if (ZombieHasCondition(self, zombie_condition_shrinking) || ZombieHasCondition(self, zombie_condition_shrunken)) {

            ZombieSetCondition(spawnedRider, zombie_condition_shrunken, 0, 3.4028e38f, 0.0f);
        }

        float launchDistance = tileDistance * 64.0f;

        float currentX = self->m_position.x;
        float currentY = self->m_position.y;
        float currentZ = self->m_position.z;

        SexyVector3 position = SexyVector3(currentX, currentY, currentZ);
        ZombieSetPosition(spawnedRider, &position);

        float targetX = currentX - launchDistance;
        uintptr_t* vtable = *(uintptr_t**)spawnedRider;

        if (ZombieHasCondition(self, zombie_condition_hypnotized)) {

            ZombieSetCondition(spawnedRider, zombie_condition_hypnotized, 0, 3.4028e38f, 0.0f);

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
        self->m_hasLaunched = true;
    }
}

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

bool hkMuteImpSound(Zombie* imp)
{
    return !ZombieIsInState(imp, 16)
        && !ZombieIsInState(imp, 19)
        && !ZombieIsDeadOrDying(imp)
        && !ZombieIsInState(imp, 3)
        && !ZombieHasCondition(imp, zombie_condition_icecubed)
        && !ZombieHasCondition(imp, zombie_condition_present_boxed)
        && !ZombieHasCondition(imp, zombie_condition_stoneblocked)
        && !imp->IsType(ZombieZcorpRacerZombie::StaticGetType())
        && !imp->IsType(ZombiePirateBoomBarrel::StaticGetType());
}

#pragma endregion


void ZombieZcorpRacerProps::modInit() {
    LOGI("init chair class");
    PVZ2HookFunction(0xBEFEE0, (void*)hkZombieChairThrowRacer, nullptr);
    PVZ2HookFunction(0xBEEC10, (void*)ZombieZcorpRacerProps::construct, nullptr);
    PVZ2HookFunction(0xB57128, (void*)hkMuteImpSound, nullptr);
    LOGI("init chair class complete");
    LOGI("init chair props");
    PVZ2HookFunction(0xBEED50, (void*)ZombieZcorpRacerProps::buildSymbols, (void**)&oZombieZcorpRacerPropsBuildSymbols);
    LOGI("init chair props complete");
    LOGI("init chair rig");
    PVZ2HookFunction(0xBF1998, (void*)hkInitZcorpRacerArmList, nullptr);
    PVZ2HookFunction(0xB57768, (void*)hkInitZcorpRacerUpperArmList, (void**)&oInitZcorpRacerUpperArmList);
    LOGI("init chair rig complete");
    LOGI("finish chair init");
}
