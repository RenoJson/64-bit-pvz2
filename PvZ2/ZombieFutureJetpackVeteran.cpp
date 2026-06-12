#include "ZombieFutureJetpack.h"
#include "ZombieStateHelper.h"
#include "TimeMgr.h"
#include "ZombieHelper.h"

void* ZombieFutureJetpackVeteran::vftable = nullptr;
Sexy::RtClass* ZombieFutureJetpackVeteran::s_rtClass = nullptr;;

void overrideFunction169(ZombieFutureJetpackVeteran* zombie) {
    
    ZombieIsFlying(zombie, false);
    zombie->m_jumpMovement = false;
    auto rig = reinterpret_cast<ZombieAnimRig_FutureJetpack*>(zombie->m_animRig.Get());
    auto props = reinterpret_cast<ZombieFutureJetpackVeteranProps*>(zombie->m_propertySheet.Get());
    rig->m_discoMode = props->DiscoMode;
    static std::vector<SexyString> HardcodedLayer = {
         "disco_zombie_arm_inner_lower",
         "disco_zombie_arm_inner_upper",
         "disco_zombie_arm_outer_lower",
         "disco_zombie_arm_outer_upper",
         "disco_zombie_arm_outer_upper_bone",
         "disco_zombie_foot_inner_heel",
         "disco_zombie_foot_inner_toe",
         "disco_zombie_foot_outer_heel",
         "disco_zombie_foot_outer_toe",
         "disco_zombie_leg_inner_upper",
         "disco_zombie_leg_inner_lower",
         "disco_zombie_leg_outer_lower",
         "disco_zombie_torso",
         "disco_zombie_waist",
         "_glasses",
         "disco_zombie_leg_upper_outer",
         "jetpack_disco_glasses",
         "particle_head_disco"
    };
    for (SexyString& layerName : HardcodedLayer) {
        SetAnimLayerVisible(rig, layerName, props->DiscoMode);
    }
}

void overrideFunction208(ZombieFutureJetpackVeteran* zombie) {
    if (zombie->m_jumpMovement == true)
    {
        typedef BoardEntity* (*GetTarget)(Zombie*);
        typedef int (*GetEntityHeight)(BoardEntity*);
        BoardEntity* entity = ((GetTarget)(*(void***)zombie)[108])(zombie);

        if (entity == nullptr ||
            ((GetEntityHeight)(*(void***)entity)[45])(entity) == 3 ||
            (entity->IsType(Zombie::StaticGetType())))
        {
            zombie->m_jumpMovement = false;
            RegisterEventAfterAnim(zombie, "fly_down", "onFlyDownAnimDone");
        }
        else {
            auto props = reinterpret_cast<ZombieFutureJetpackVeteranProps*>(zombie->m_propertySheet.Get());
            float flyingSpeed = props->Speed * props->SpeedScaleWhenFlyOverPlant;

            ZombieConditionTracker* zTracker = &zombie->m_conditionTracker;
            uint8_t* cond = zTracker->m_states.data();

            if (cond != nullptr && *cond != 0) {
                typedef void (*UpdateConditionsFunc)(ZombieConditionTracker*);
                ((UpdateConditionsFunc)(*(void***)zTracker)[3])(zTracker);
                *cond = 0;
            }

            float speedScale = zTracker->m_speedScale;
            float floatingSpeed = flyingSpeed * speedScale * 64.0f;
            float facing = ZombieFacing(zombie);
            float finalSpeed = facing * floatingSpeed;
            float timeMoving = TimeMgr::GetInstance()->m_unkTime;

            SexyVector3 newPos;
            newPos.x = zombie->m_position.x - (finalSpeed * timeMoving);
            newPos.y = zombie->m_position.y;
            newPos.z = zombie->m_position.z;

           ZombieUpdatePosition(zombie, &newPos);
        }
    }
}
void ZombieFutureJetpackVeteran::modInit() {
	LOGI("ZombieJet mod init");
	vftable = CopyVFTable(getActualOffset(0x23EFF98), 210);
	PatchVFTable(vftable, (void*)ZombieFutureJetpackVeteran::StaticGetType, 0);

	PatchVFTable(vftable, (void*)overrideFunction169, 169);

    PatchVFTable(vftable, (void*)overrideFunction208, 208);

	ZombieFutureJetpackVeteran::StaticGetType();
	LOGI("ZombieJet finish init");
}

void* ZombieFutureJetpackVeteranProps::vftable = __null;
Sexy::RtClass* ZombieFutureJetpackVeteranProps::s_rtClass = __null;


void ZombieFutureJetpackVeteranProps::modInit() {
	LOGI("ZombieFutureJetpackVeteranProps init");

	vftable = CopyVFTable(getActualOffset(0x2433408), 14);

	PatchVFTable(vftable, (void*)ZombieFutureJetpackVeteranProps::StaticGetType, 0);

	ZombieFutureJetpackVeteranProps::StaticGetType();

	LOGI("ZombieFutureJetpackVeteranProps finish init");
}