#include "Zombie_BullProps.h"
#include "Zombie_BullVeteran.h"
#include "PvZ2/Zombie.h"
#include "ZombieConditions.h"
#include <atomic>
#include <mutex>
#include <assert.h>
#include <pch.h>
#include "AddZombieType.h"
#include "ZombieImp.h"
#include "ZombieBull.h"
#include "ZombieAnimRig_Bull.h"
#include "ZombieHelper.h"
using namespace Sexy;

#pragma region Hide Custom Rider Layer 
void HideCustomRiderLayer(ZombieBull* self, ZombieAnimRig_Bull* animRig) {

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

    size_t customLayersCount = props->RiderLayersToHide.size();
    if (!props->RiderLayersToHide.empty()) {
        for (size_t i = 0; i < customLayersCount; i++) {
            const auto& layerStr = props->RiderLayersToHide[i];
            SexyString layerName(layerStr);
            SetAnimLayerVisible(animRig, layerName, false);
        }
    }
    else {
        for (size_t i = 0; i < HardcodedLayer.size(); i++) {
            const auto& layerStr = HardcodedLayer[i];
            SexyString layerName(layerStr);
            SetAnimLayerVisible(animRig, layerName, false);
        }
    }
}
#pragma endregion

// thanks jay_krow for the codebase to help me recreate the function has more context
#pragma region hk Rider Type To Launch

//Bull and Vet Bull class
void hkZombieBullThrowRider(ZombieBull* self)
{
    SexyString name;
    float distance;
    float launchApex;
    float launchTime;
    SexyVector3 riderOffset;

        auto* props = reinterpret_cast<ZombieBullProps*>(self->m_propertySheet.Get());
        name = props->RiderType;
        distance = props->LaunchDistance;
        launchApex = props->LaunchHeight;
        launchTime = props->LaunchAirTime;
        riderOffset = props->RiderSpawnOffset;

    // we won't need to use RiderType anymore if the zombie spawn from the bull is wild west imp
    if (name.empty()) {
        name = "west_bullrider";
    }


    if (!self->m_hasLaunched && !ZombieHasCondition(self, zombie_condition_shrinking) && !ZombieHasCondition(self, zombie_condition_shrunken))
    {
        auto* rig = reinterpret_cast<ZombieAnimRig_Bull*>(self->m_animRig.Get());
        HideCustomRiderLayer(self, rig);
        ZombieImp* spawnedRider = (ZombieImp*)AddZombie(name, -1, 6, -1);

        spawnedRider->m_getsUpFromLanding = props->UseGetUpAnim;
        bool isHypnotized = ZombieHasCondition(self, zombie_condition_hypnotized);
        float newX = self->m_position.x - riderOffset.x;
        if (isHypnotized)
        {
            newX = self->m_position.x + riderOffset.x;
        }
        float newY = self->m_position.y + riderOffset.y;
        float newZ = self->m_position.z + riderOffset.z;
        SexyVector3 newCoords = SexyVector3(newX, newY, newZ);
        ZombieSetPosition(spawnedRider, &newCoords);
        typedef void (*ZombieThrowVirtual)(Zombie*, int, float, float, float, float, float);
        uintptr_t* vtable = *(uintptr_t**)spawnedRider;
        ZombieThrowVirtual virtualThrow = (ZombieThrowVirtual)(vtable[206]);

        float targetX, targetY, targetZ;

        if (isHypnotized) {

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
        virtualThrow(spawnedRider, 0, targetX, targetY, targetZ, launchTime, launchApex);

        // this one is prevent veteran bull throw further imp
        self->m_hasLaunched = true;
    }
    else {
        LOGI("Already shrinking, shrunken or being thrown");
    }
}
//Cavalry class
void hkZombieCavalryThrowRider(ZombieBull* self)
{
    SexyString name;
    float distance;
    float launchApex;
    float launchTime;
    SexyVector3 riderOffset;

    auto* props = reinterpret_cast<ZombieBullProps*>(self->m_propertySheet.Get());
    name = props->RiderType;
    distance = props->LaunchDistance;
    launchApex = props->LaunchHeight;
    launchTime = props->LaunchAirTime;
    riderOffset = props->RiderSpawnOffset;

    if (name.empty()) {
        name = "dark_cavalry_rider";
    }

    if (!self->m_hasLaunched)
    {
        auto* rig = reinterpret_cast<ZombieAnimRig_Bull*>(self->m_animRig.Get());
        HideCustomRiderLayer(self, rig);
        ZombieImp* spawnedRider = (ZombieImp*)AddZombie(name, -1, 6, -1);
        spawnedRider->m_getsUpFromLanding = props->UseGetUpAnim;
        if (ZombieHasCondition(self, zombie_condition_shrinking) || ZombieHasCondition(self, zombie_condition_shrunken)) {

            ZombieSetCondition(spawnedRider, zombie_condition_shrunken, 0, 3.4028e38f, 0.0f);
        }

        bool isHypnotized = ZombieHasCondition(self, zombie_condition_hypnotized);
        float newX = self->m_position.x - riderOffset.x;
        if (isHypnotized)
        {
            newX = self->m_position.x + riderOffset.x;
        }
        float newY = self->m_position.y + riderOffset.y;
        float newZ = self->m_position.z + riderOffset.z;
        SexyVector3 newCoords = SexyVector3(newX, newY, newZ);
        ZombieSetPosition(spawnedRider, &newCoords);
        typedef void (*ZombieThrowVirtual)(Zombie*, int, float, float, float, float, float);
        uintptr_t* vtable = *(uintptr_t**)spawnedRider;
        ZombieThrowVirtual virtualThrow = (ZombieThrowVirtual)(vtable[206]);

        float targetX, targetY, targetZ;


        if (isHypnotized) {

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
        virtualThrow(spawnedRider, 0, targetX, targetY, targetZ, launchTime, launchApex);
        // this one is prevent veteran bull throw further imp
        self->m_hasLaunched = true;
    }
    else {
        LOGI("Already shrinking, shrunken or being thrown");
    }
}
void hkBullThrowRoute(ZombieBull* self) {
    auto* props = reinterpret_cast<ZombieBullProps*>(self->m_propertySheet.Get());

    bool isCavalry = props->isCavalry;
    if (!isCavalry && !ZombieIsDeadOrDying(self)) {
        return hkZombieBullThrowRider(self);
    }
    else if (isCavalry && !ZombieIsDeadOrDying(self)) {
        return hkZombieCavalryThrowRider(self);
    }
}

void hkZombieBullPlayDeath(ZombieBull* self) {

    typedef void(*zombieFun197)(ZombieBull*);
    ((zombieFun197)getActualOffset(0xC51A40))(self); // does not play the bull's death sound effect
}
void hkZombieBullElectrocute(ZombieBull* self) {
    auto* rig = reinterpret_cast<ZombieAnimRig_Bull*>(self->m_animRig.Get());
    HideCustomRiderLayer(self, rig);
    typedef void (*electrocute)(ZombieBull*);
    ((electrocute)getActualOffset(0xC51FB0))(self);
}
void hkBullFunction71(ZombieBull* self, int conditionID)
{
    auto* props = reinterpret_cast<ZombieBullProps*>(self->m_propertySheet.Get());
    bool isCavalry = props->isCavalry;

    if (conditionID == zombie_condition_shrinking || conditionID == zombie_condition_shrunken)
    {

        if (!isCavalry) {
            auto* rig = reinterpret_cast<ZombieAnimRig_Bull*>(self->m_animRig.Get());
            HideCustomRiderLayer(self, rig);
        }
        return;
    }

    if (conditionID == zombie_condition_gummed)
    {
        if (!self->m_hasLaunched) {
            ZombieEnterState(self, 20, 0);
        }
        return;
    }

    if (conditionID == zombie_condition_hypnotized)
    {
        ZombieFlippedAnim(self, false);
        ZombieEnterState(self, 20, 0);
        return;
    }
}
#pragma endregion


Reflection::CRefManualSymbolBuilder::BuildSymbolsFunc ZombieBullProps::oZombieBullPropsBuildSymbols = nullptr;

void ZombieBullProps::modInit() {
    LOGI("init bull class");
    PVZ2HookFunction(0xAE4FF8, (void*)hkBullThrowRoute, nullptr);
    PVZ2HookFunction(0xAE541C, (void*)HideCustomRiderLayer, nullptr);
    PVZ2HookFunction(0xAE3A64, (void*)hkZombieBullPlayDeath, nullptr);
    PVZ2HookFunction(0xAE53A0, (void*)hkBullFunction71, nullptr);
    PVZ2HookFunction(0xAE5CD4, (void*)hkZombieBullElectrocute, nullptr);
    PVZ2HookFunction(0xDAE1B0, (void*)construct, nullptr);
    LOGI("init bull class complete");
    LOGI("init bull props");
    PVZ2HookFunction(0xDAE2F4, (void*)ZombieBullProps::buildSymbols, (void**)&ZombieBullProps::oZombieBullPropsBuildSymbols);
    LOGI("init bull props complete");
    LOGI("finish init bull");
}