#include "ZombieGargantuar.h"
#include "ZombieGargantuarProps.h"
#include "MessageRouter.h"
#include "Messages.h"
#include "DamageInfo.h"
#include "ZombieAnimRig_Gargantuar.h"
#include "ZombieHelper.h"
#include "Plant.h"

void* ZombieLostCityTorchGargantuar::vftable = nullptr;
Sexy::RtClass* ZombieLostCityTorchGargantuar::s_rtClass = nullptr;

void SetFlameLayersVisible(ZombieLostCityTorchGargantuar* self, ZombieAnimRig_Gargantuar* animRig, bool visible) {
	SetAnimLayerVisible(animRig, "torch_end_lit", visible);
	SetAnimLayerVisible(animRig, "torch_fire_frame_01", visible);
	SetAnimLayerVisible(animRig, "torch_fire_frame_02", visible);
	SetAnimLayerVisible(animRig, "torch_fire_frame_03", visible);
	SetAnimLayerVisible(animRig, "torch_fire_frame_04", visible);
	SetAnimLayerVisible(animRig, "torch_fire_fire_frame_01", visible);
}
void LostCityGargOnAffectedByCondition(ZombieLostCityTorchGargantuar* zombie, int condition) {
    if (condition == zombie_condition_freeze || condition == zombie_condition_chill) {
		zombie->m_hasTorch = false;
		auto rig = reinterpret_cast<ZombieAnimRig_Gargantuar*>(zombie->m_animRig.Get());
        SetFlameLayersVisible(zombie, rig, false);
		MessageRouter::GetInstance()->ExecuteMessage((void*)getActualOffset((uintptr_t)MessageId::ExplorerMsg), zombie);
    }
    if (condition == zombie_condition_shrinking) {
        auto rig = reinterpret_cast<ZombieAnimRig_Gargantuar*>(zombie->m_animRig.Get());
		auto props = reinterpret_cast<ZombieLostCityGargantuarProps*>(zombie->m_propertySheet.Get());

        std::vector<SexyString> HardcodedLayer = {};

        bool hasCustomLayers = false;
        if (!props->HealthThresholdToImpAmmoLayers.empty()) {
            hasCustomLayers = true;

            size_t pairCount = props->HealthThresholdToImpAmmoLayers.size();
            for (size_t i = 0; i < pairCount; i++) {
                const auto& layersToHide = props->HealthThresholdToImpAmmoLayers[i].ProjectileLayersToHide;
                size_t layerCount = layersToHide.size();
                for (size_t j = 0; j < layerCount; j++) {
                    SexyString layerName(layersToHide[j]);
					SetAnimLayerVisible(rig, layerName, false);
                }
            }
        }
        if (!hasCustomLayers) {
            for (size_t i = 0; i < HardcodedLayer.size(); i++) {
                SexyString layerName(HardcodedLayer[i]);
                SetAnimLayerVisible(rig, layerName, false);
            }
        }
    }
}
Sexy::Rect LostCityGargantuarGetAttackRect(ZombieLostCityTorchGargantuar* zombie) {
    auto props = reinterpret_cast<ZombieLostCityGargantuarProps*>(zombie->m_propertySheet.Get());
    auto rig = reinterpret_cast<ZombieAnimRig_Gargantuar*>(zombie->m_animRig.Get());
    Rect attackRect;
    float offsetX, offsetY;
    attackRect.mWidth = props->AttackRect.mWidth;
    attackRect.mHeight = props->AttackRect.mHeight;
    offsetX = props->AttackRect.mX;
    offsetY = props->AttackRect.mY;

    float zX = zombie->m_position.x;
    float zY = zombie->m_position.y;
    float zZ = zombie->m_position.z;
    float facing = ZombieFacing(zombie);

    attackRect.mX = static_cast<int>(zX - (attackRect.mWidth / 2.0f) + (facing * offsetX));
    attackRect.mY = static_cast<int>((zY - zZ) - attackRect.mHeight + offsetY);

    if (zombie->m_hasTorch == true) {
        SexyVector2 torchLitPos;
        SexyVector2 outerArmPos;
        auto rig = reinterpret_cast<ZombieAnimRig_Gargantuar*>(zombie->m_animRig.Get());

        GetAnimRigSpritePosition(rig, "torch_end_lit", &torchLitPos);
        GetAnimRigSpritePosition(rig, "Zombie_gargantuar_outerarm_hand", &outerArmPos);

        float currentTorchReachX = outerArmPos.x - torchLitPos.x;
        float maxReachLimit = props->MaxTorchReach;
        float finalReach = std::min(currentTorchReachX, maxReachLimit);

        int oldX = attackRect.mX;
        attackRect.mX = static_cast<int>((facing * finalReach) + static_cast<float>(attackRect.mX));

    };
    return attackRect;
}

void LostCityGargantuarWalkOnLoop(ZombieLostCityTorchGargantuar* zombie) {
    BoardEntity* entity = CallFunc<BoardEntity*, ZombieLostCityTorchGargantuar*>(0xC41910, zombie);
    if (zombie->m_hasTorch == true) {
        if (entity != nullptr) {
            if (entity->IsType(PlantGroup::StaticGetType())) {

                CallFunc<void, BoardEntity*, int64_t, int, bool, ZombieLostCityTorchGargantuar*>(0x1336D2C, entity, damage_fire, 2, false, zombie);
            }
            else {
                ZombieEnterState(zombie, 16, 0);
            }
        }
        else {
            if (CallFunc<bool, ZombieLostCityTorchGargantuar*>(0xB5414C, zombie) == true) {
                auto props = reinterpret_cast<ZombieLostCityGargantuarProps*>(zombie->m_propertySheet.Get());
                if (zombie->m_position.x > props->MinPosXThrowImp) {
                    ZombieEnterState(zombie, 17, 0);
                }
            }
        }
    }
    else {
        CallFunc<void, ZombieLostCityTorchGargantuar*>(0xB4C934, zombie);
    }
}

void LostCityGargantuarOnCreate(ZombieLostCityTorchGargantuar* self) {
    self->m_hasTorch = true;
}
void LostCityGargFunction171(ZombieLostCityTorchGargantuar* self) {
    if(self->m_hasTorch == true) {
		self->m_hasTorch = false;
		auto rig = reinterpret_cast<ZombieAnimRig_Gargantuar*>(self->m_animRig.Get());
		SetFlameLayersVisible(self, rig, false);
	}
}
Zombie* LostCityGargFunction184(ZombieLostCityTorchGargantuar* self, DamageInfo* damageInfo) {
    if (self->m_hasTorch == false
        && damageInfo->m_damage >= 0.0f
        && (damageInfo->m_flags & DamageTypeFlags::damage_fire) != 0) {
		self->m_hasTorch = true;
		auto rig = reinterpret_cast<ZombieAnimRig_Gargantuar*>(self->m_animRig.Get());
		SetFlameLayersVisible(self, rig, true);
		return self;
	}
    return self;
}
void LostCityGargFunction185(ZombieLostCityTorchGargantuar* self) {
    self->m_hasTorch = false;
    auto rig = reinterpret_cast<ZombieAnimRig_Gargantuar*>(self->m_animRig.Get());
    SetFlameLayersVisible(self, rig, false);
}
void ZombieLostCityTorchGargantuar::modInit() {
	LOGI("ZombieLostCityTorchGargantuar mod init");
	vftable = CopyVFTable(getActualOffset(0x2406458), 212);
	PatchVFTable(vftable, (void*)ZombieLostCityTorchGargantuar::StaticGetType, 0);

    PatchVFTable(vftable, (void*)LostCityGargOnAffectedByCondition, 71);
	PatchVFTable(vftable, (void*)LostCityGargantuarGetAttackRect, 88);
	PatchVFTable(vftable, (void*)LostCityGargantuarWalkOnLoop, 124);
	PatchVFTable(vftable, (void*)LostCityGargantuarOnCreate, 169);
	PatchVFTable(vftable, (void*)LostCityGargFunction171, 171);
	PatchVFTable(vftable, (void*)LostCityGargFunction184, 184);
	PatchVFTable(vftable, (void*)LostCityGargFunction185, 185);
	ZombieLostCityTorchGargantuar::StaticGetType();
	LOGI("ZombieLostCityTorchGargantuar finish init");
}

void* ZombieLostCityGargantuarProps::vftable = __null;
Sexy::RtClass* ZombieLostCityGargantuarProps::s_rtClass = __null;


void ZombieLostCityGargantuarProps::modInit() {
	LOGI("ZombieLostCityGargantuarProps init");

	vftable = CopyVFTable(getActualOffset(0x2432A08), 14);

	PatchVFTable(vftable, (void*)ZombieLostCityGargantuarProps::StaticGetType, 0);

	ZombieLostCityGargantuarProps::StaticGetType();

	LOGI("ZombieLostCityGargantuarProps finish init");
}