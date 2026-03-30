#include "ZombieDarkCavalry.h"
#include "ZombieAnimRig_Bull.h"
#include "AddZombieType.h"
#include "Zombie_BullProps.h"
#include "ZombieCavalryProps.h"
#include "Plant.h"
#include "GridItem.h"


void* ZombieDarkCavalry::vftable = __null;
Sexy::RtClass* ZombieDarkCavalry::s_rtClass = __null;;

typedef void (*zombieEnterState)(ZombieDarkCavalry*, int, int);
typedef Zombie* (*updatePos)(ZombieDarkCavalry*, SexyVector3*);
typedef bool (*checkZombieHasCondition)(Zombie*, int);

typedef void (*setConditionZ)(Zombie*, int, int, float, float);

typedef int (*boardEntitySetPosition)(Zombie*, SexyVector3*);
// override func 49, 206 and 215 so that cavalry alway launch plant when encountered no matter its walk anim are done or not
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
    typedef Plant* (*getTarg)(ZombieDarkCavalry*);
    getTarg getTarget = (getTarg)getActualOffset(0xC41910);
    Plant* target = getTarget(self);
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

        KillTarget(target, self);
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
        // this one is prevent veteran bull throw further imp
        self->m_hasLaunched = true;
    }
    else {
        LOGI("Already shrinking, shrunken or being thrown");
    }
}
void overrideCavalryActionFrame(ZombieDarkCavalry* zombie, int64_t unk1, SexyString* actionName, int64_t unk2, SexyString* currentAnim)
{
	if (*actionName == "launch")
	{
        CavalryThrowRider(zombie);
        LanceSpawn(zombie);
	}
}
SexyString GetCavalryShockEffectName()
{
	return "POPANIM_EFFECTS_ZOMBIE_CAVALRY_SHOCK";
}
void overrideBullFunction206(ZombieDarkCavalry* zombie) {
    typedef void (*zombieFun206)(ZombieDarkCavalry*);
    ((zombieFun206)getActualOffset(0xAE3DE8))(zombie);
    zombie->m_walkCycled = true;
}
void overrideBullFunction215(ZombieDarkCavalry* zombie) {
    typedef void (*zombieFun215)(ZombieDarkCavalry*);
    ((zombieFun215)getActualOffset(0xAE4818))(zombie);
    zombie->m_walkCycled = true;
}

void ZombieDarkCavalry::modInit() {
	LOGI("ZombieDarkCavalry init");

	vftable = CopyVFTable(getActualOffset(0x23DB6C8), 220);

	PatchVFTable(vftable, (void*)ZombieDarkCavalry::StaticGetType, 0);

	PatchVFTable(vftable, (void*)overrideOnSpawn, 49);

	PatchVFTable(vftable, (void*)overrideCavalryActionFrame, 170);

	PatchVFTable(vftable, (void*)GetCavalryShockEffectName, 189);

    PatchVFTable(vftable, (void*)overrideBullFunction206, 206);

    PatchVFTable(vftable, (void*)overrideBullFunction215, 215);

	ZombieDarkCavalry::StaticGetType();

	LOGI("ZombieDarkCavalry finish init");
}