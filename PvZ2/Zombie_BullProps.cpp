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
using namespace Sexy;

#pragma region Hide Custom Rider Layer 
typedef void(*zombieBullHideLayer)(Zombie*, uintptr_t);
zombieBullHideLayer oZombieBullHideLayer = nullptr;
void HideCustomRiderLayer(Zombie* self, uintptr_t animRig) {

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

    typedef int64_t (*setLayerVisibleFunc)(uintptr_t, SexyString*, bool);
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
#pragma endregion

// thanks jay_krow for the codebase to help me recreate the function has more context
#pragma region hk Rider Type To Launch


typedef void(*zombieBullThrowRider)(Zombie*, int);
zombieBullThrowRider oZombieBullThrowRider = nullptr;

// these function is must have to make the bull work properly
using sub_736DE4_t = uintptr_t(*)(Zombie*);
sub_736DE4_t sub_736DE4 = reinterpret_cast<sub_736DE4_t>(getActualOffset(0x6DE754));

typedef bool (*checkZombieHasCondition)(Zombie*, int);
checkZombieHasCondition hasZCondition = (checkZombieHasCondition)getActualOffset(0xC3E44C);

typedef void (*setConditionZ)(Zombie*, int, int, float, float);
setConditionZ setZCondition = (setConditionZ)getActualOffset(0xC40CC0);

typedef int (*boardEntitySetPosition)(Zombie*, SexyVector3*);
boardEntitySetPosition ZfunBoardEntitySetPosition = (boardEntitySetPosition)getActualOffset(0x628058);

//Bull and Vet Bull class
void hkZombieBullThrowRider(Zombie* self, int a2)
{
    auto* props = reinterpret_cast<ZombieBullProps*>(self->m_propertySheet.Get());

    if (!props) {
        return oZombieBullThrowRider(self, a2);
    }


    // using custom symbol depend on what props the bull use
    SexyString name;
    float distance;
    float launchApex;
    float launchTime;

        name = props->RiderType;
        distance = props->LaunchDistance;
        
    // we won't need to use RiderType anymore if the zombie spawn from the bull is wild west imp
    if (name.empty()) {
        name = "west_bullrider";
    }


    bool isShrinking = hasZCondition(self, zombie_condition_shrinking);
    bool isShrunken = hasZCondition(self, zombie_condition_shrunken);

    if (!*(bool*)((uintptr_t)self + 0x448) && !isShrinking && !isShrunken)
    {
        uintptr_t getBullRiderAnimRig = sub_736DE4(self);
        HideCustomRiderLayer(self, getBullRiderAnimRig);
        ZombieImp* spawnedRider = (ZombieImp*)AddZombie(name, -1, 6, -1);

        spawnedRider->m_getsUpFromLanding = true;
        bool isHypnotized = hasZCondition(self, zombie_condition_hypnotized);
        float newX = self->m_position.x - props->RiderSpawnOffset.x;
        if (isHypnotized)
        {
            newX = self->m_position.x + props->RiderSpawnOffset.x;
        }
        float newY = self->m_position.y + props->RiderSpawnOffset.y;
        float newZ = self->m_position.z + props->RiderSpawnOffset.z;
        SexyVector3 newCoords = SexyVector3(newX, newY, newZ);
        ZfunBoardEntitySetPosition(spawnedRider, &newCoords);
        typedef void (*ZombieThrowVirtual)(Zombie*, int, float, float, float, float, float);
        uintptr_t* vtable = *(uintptr_t**)spawnedRider;
        ZombieThrowVirtual virtualThrow = (ZombieThrowVirtual)(vtable[206]);

        float targetX, targetY, targetZ;

        if (isHypnotized) {

            setZCondition(spawnedRider, zombie_condition_hypnotized, 0, 3.4028e38f, 0.0f);

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
        launchApex = props->LaunchHeight;
        launchTime = props->LaunchAirTime;
        virtualThrow(spawnedRider, 0, targetX, targetY, targetZ, launchTime, launchApex);

        // this one is prevent veteran bull throw further imp
        *(bool*)((uintptr_t)self + 0x448) = true;
    }
    else {
        LOGI("Already shrinking, shrunken or being thrown");
    }
}
void hkZombieBullVetThrowRider(Zombie* self, int a2)
{
    auto* props = reinterpret_cast<ZombieBullVeteranProps*>(self->m_propertySheet.Get());

    if (!props) {
        return oZombieBullThrowRider(self, a2);
    }

  
    SexyString name = props->VetRiderType;
    float distance = props->LaunchDistance;


    // we won't need to use RiderType anymore if the zombie spawn from the bull is wild west imp
    if (name.empty()) {
        name = "west_bullrider";
    }


    bool isShrinking = hasZCondition(self, zombie_condition_shrinking);
    bool isShrunken = hasZCondition(self, zombie_condition_shrunken);

    if (!*(bool*)((uintptr_t)self + 0x448) && !isShrinking && !isShrunken)
    {
        uintptr_t getBullRiderAnimRig = sub_736DE4(self);
        HideCustomRiderLayer(self, getBullRiderAnimRig);
        ZombieImp* spawnedRider = (ZombieImp*)AddZombie(name, -1, 6, -1);

        spawnedRider->m_getsUpFromLanding = true;
        bool isHypnotized = hasZCondition(self, zombie_condition_hypnotized);
        float newX = self->m_position.x - props->RiderSpawnOffset.x;
        if (isHypnotized)
        {
            newX = self->m_position.x + props->RiderSpawnOffset.x;
        }
        float newY = self->m_position.y + props->RiderSpawnOffset.y;
        float newZ = self->m_position.z + props->RiderSpawnOffset.z;
        SexyVector3 newCoords = SexyVector3(newX, newY, newZ);
        ZfunBoardEntitySetPosition(spawnedRider, &newCoords);
        typedef void (*ZombieThrowVirtual)(Zombie*, int, float, float, float, float, float);
        uintptr_t* vtable = *(uintptr_t**)spawnedRider;
        ZombieThrowVirtual virtualThrow = (ZombieThrowVirtual)(vtable[206]);

        float targetX, targetY, targetZ;

        

        if (isHypnotized) {

            setZCondition(spawnedRider, zombie_condition_hypnotized, 0, 3.4028e38f, 0.0f);

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

        // Tham số thứ 5 trong hàm ảo là apex/height
        // Tham số thứ 6 trong hàm ảo là thời gian
        float launchApex = props->LaunchHeight;
        float launchTime = props->LaunchAirTime;
        virtualThrow(spawnedRider, 0, targetX, targetY, targetZ, launchTime, launchApex);
        LOGI("Done");
        // this one is prevent veteran bull throw further imp
        *(bool*)((uintptr_t)self + 0x448) = true;
    }
    else {
        LOGI("Already shrinking, shrunken or being thrown");
    }
}
//Cavalry class
void hkZombieCavalryThrowRider(Zombie* self, int a2)
{
    auto* props = reinterpret_cast<ZombieBullProps*>(self->m_propertySheet.Get());


    SexyString name = props->RiderType;
    float distance = props->LaunchDistance;

    

    if (name.empty()) {
        name = "dark_cavalry_rider";
    }

    bool isShrinking = hasZCondition(self, zombie_condition_shrinking);
    bool isShrunken = hasZCondition(self, zombie_condition_shrunken);

    if (!*(bool*)((uintptr_t)self + 0x448))
    {
        uintptr_t getBullRiderAnimRig = sub_736DE4(self);
        HideCustomRiderLayer(self, getBullRiderAnimRig);
        Zombie* spawnedRider = AddZombie(name, -1, 6, -1);

        if (hasZCondition(self, zombie_condition_shrinking) || hasZCondition(self, zombie_condition_shrunken)) {

            setZCondition(spawnedRider, zombie_condition_shrunken, 0, 3.4028e38f, 0.0f);
        }

        bool isHypnotized = hasZCondition(self, zombie_condition_hypnotized);
        float newX = self->m_position.x - props->RiderSpawnOffset.x;
        if (isHypnotized)
        {
            newX = self->m_position.x + props->RiderSpawnOffset.x;
        }
        float newY = self->m_position.y + props->RiderSpawnOffset.y;
        float newZ = self->m_position.z + props->RiderSpawnOffset.z;
        SexyVector3 newCoords = SexyVector3(newX, newY, newZ);
        ZfunBoardEntitySetPosition(spawnedRider, &newCoords);
        typedef void (*ZombieThrowVirtual)(Zombie*, int, float, float, float, float, float);
        uintptr_t* vtable = *(uintptr_t**)spawnedRider;
        ZombieThrowVirtual virtualThrow = (ZombieThrowVirtual)(vtable[206]);

        float targetX, targetY, targetZ;


        if (isHypnotized) {

            setZCondition(spawnedRider, zombie_condition_hypnotized, 0, 3.4028e38f, 0.0f);

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
        // this one is prevent veteran bull throw further imp
        *(bool*)((uintptr_t)self + 0x448) = true;
    }
    else {
        LOGI("Already shrinking, shrunken or being thrown");
    }
}
void hkBullThrowRoute(Zombie* self, int a2) {
    auto* props = reinterpret_cast<ZombieBullProps*>(self->m_propertySheet.Get());
    if (!props) {
        return oZombieBullThrowRider(self, a2);
    }

    uintptr_t* vtable = *reinterpret_cast<uintptr_t**>(self);
    bool isBull = (vtable == reinterpret_cast<uintptr_t*>(getActualOffset(0x23DB6C8)));
    bool isVeteran = (vtable == reinterpret_cast<uintptr_t*>(getActualOffset(0x23DBE28)));
    bool isCavalry = props->isCavalry;
    if (isBull && !isCavalry) {
        return hkZombieBullThrowRider(self, a2);
    }
    else if (isBull && isCavalry) {
        return hkZombieCavalryThrowRider(self, a2);
    }
    else if (isVeteran && !isCavalry) {
        return hkZombieBullVetThrowRider(self, a2);
    }
}



typedef void (*zombieBullPlayDeath)(Zombie*);
zombieBullPlayDeath oZombieBullPlayDeath = nullptr;

void hkZombieBullPlayDeath(Zombie* self) {
    
    typedef void(*zombieFun197)(Zombie*);
    ((zombieFun197)getActualOffset(0xC51A40))(self); // does not play the bull's death sound effect
}
typedef void (*zombieBullElectrocute)(Zombie*);
zombieBullPlayDeath oZombieBullElectrocute = nullptr;
//this function is for bull when electrocuted
//Rewrite this to make sure it don't use the original hiding layer function
void hkZombieBullElectrocute(Zombie* self) {
    uintptr_t getBullRiderAnimRig = sub_736DE4(self);
    HideCustomRiderLayer(self, getBullRiderAnimRig);
    typedef void (*electrocute)(Zombie*);
    ((electrocute)getActualOffset(0xC51FB0))(self);
}
typedef void (*FuncOnCondition)(Zombie*, int);
FuncOnCondition oBullFunction71 = nullptr;
//this function is for bull when hypnotized, shrinking or gummed
//Rewrite this to make sure it don't use the original hiding layer function
void hkBullFunction71(Zombie* self, int conditionID)
{
    auto* props = reinterpret_cast<ZombieBullProps*>(self->m_propertySheet.Get());
    bool isCavalry = props->isCavalry;

    if (conditionID == zombie_condition_shrinking || conditionID == zombie_condition_shrunken)
    {

        if (!isCavalry) {
            uintptr_t getBullRiderAnimRig = sub_736DE4(self);
            HideCustomRiderLayer(self, getBullRiderAnimRig);
        }
        return;
    }

    if (conditionID == zombie_condition_gummed)
    {
        bool hasThrown = *(bool*)((uintptr_t)self + 0x448);

        if (!hasThrown) {
            typedef int (*FuncC3D428)(Zombie*, int, int);
            FuncC3D428 funC3D428 = (FuncC3D428)getActualOffset(0xC3D428);
            funC3D428(self, 20, 0);
        }
        return;
    }

    if (conditionID == zombie_condition_hypnotized)
    {
        typedef void (*FuncC41290)(Zombie*, int);
        FuncC41290 stopSound = (FuncC41290)getActualOffset(0xC41290);
        stopSound(self, 0);

        typedef uintptr_t (*FuncC3D428)(Zombie*, int, int);
        FuncC3D428 funC3D428 = (FuncC3D428)getActualOffset(0xC3D428);
        funC3D428(self, 20, 0);

        return;
    }
}
#pragma endregion


Reflection::CRefManualSymbolBuilder::BuildSymbolsFunc ZombieBullProps::oZombieBullPropsBuildSymbols = nullptr;

void ZombieBullProps::modInit(){
	LOGI("init bull class");
	PVZ2HookFunction(0xAE4FF8, (void*)hkBullThrowRoute, (void**)&oZombieBullThrowRider);
    PVZ2HookFunction(0xAE541C, (void*)HideCustomRiderLayer, (void**)&oZombieBullHideLayer);
    PVZ2HookFunction(0xAE3A64, (void*)hkZombieBullPlayDeath, (void**)&oZombieBullPlayDeath);
    PVZ2HookFunction(0xAE53A0, (void*)hkBullFunction71, (void**)&oBullFunction71);
    PVZ2HookFunction(0xAE5CD4, (void*)hkZombieBullElectrocute, (void**)&oZombieBullElectrocute);
    PVZ2HookFunction(0xDAE1B0, (void*)construct, nullptr);
	LOGI("init bull class complete");
	LOGI("init bull props");
	PVZ2HookFunction(0xDAE2F4, (void*)ZombieBullProps::buildSymbols, (void**)&ZombieBullProps::oZombieBullPropsBuildSymbols);
	LOGI("init bull props complete");
	LOGI("finish init bull");
}