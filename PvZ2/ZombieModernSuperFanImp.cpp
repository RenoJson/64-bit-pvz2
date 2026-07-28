#include "ZombieImp.h"
#include "Plant.h"
#include "DamageInfo.h"
#include "ZombieHelper.h"

typedef bool (*isDeadOrDying)(ZombieModernSuperfanImp*);
Reflection::CRefManualSymbolBuilder::BuildSymbolsFunc ZombieModernSuperfanImpProps::oZombieModernSuperfanImpPropsBuildSymbols = nullptr;

typedef void(*SuperfanAF)(ZombieModernSuperfanImp*, int64_t, SexyString*, int64_t, SexyString*);
SuperfanAF oSuperfanAF = nullptr;

void hkSuperfanImpAF(ZombieModernSuperfanImp* imp, int64_t unk1, SexyString* actionName, int64_t unk2, SexyString* currentAnim) {
	oSuperfanAF(imp, unk1, actionName, unk2, currentAnim);
	if (ZombieIsDeadOrDying(imp) || ((imp->m_teamFlags) & 2) == 0) {
		return;
	}
	else if (*actionName == "explode") {
        auto* props = reinterpret_cast<ZombieModernSuperfanImpProps*>(imp->m_propertySheet.Get());
        float damageAmount = props->ExplosionDamage;

        int zX = static_cast<int>((imp->m_position.x - 200.0f) / 64.0f);
        int zY = static_cast<int>((imp->m_position.y - 160.0f) / 76.0f);


        Rect ExplodeRect;
        ExplodeRect.mX = zX + props->ExplosionRect.mX;
        ExplodeRect.mY = zY + props->ExplosionRect.mY;
        ExplodeRect.mWidth = props->ExplosionRect.mWidth;
        ExplodeRect.mHeight = props->ExplosionRect.mHeight;

        std::vector<BoardEntity*> entityList;
        GetEntitiesInRectGrid(&entityList, 32, &ExplodeRect);
        ZombieSetInvincibleStatusFlag(imp, true);
        ZombieSetNoCollisionFlag(imp, true);

        for (BoardEntity* ptr : entityList) {
            if (ptr == nullptr) {

                continue;
            }
            if (ptr->IsType(PlantGroup::StaticGetType())) {
                PlantGroup* pGroup = reinterpret_cast<PlantGroup*>(ptr);
                DamageInfo dmg;
                dmg.m_attacker = imp;
                dmg.m_damage = damageAmount;
                dmg.m_flags = DamageTypeFlags::damage_fire;

                void** vtable = *(void***)pGroup;
                typedef void (*ApplyDamageFunc)(PlantGroup*, DamageInfo*);
                ApplyDamageFunc applyDamage = (ApplyDamageFunc)vtable[35];

                applyDamage(pGroup, &dmg);
            }
            else {
                LOGI("-> Bỏ qua thực thể %p (Không phải PlantGroup).", ptr);
            }
        }
	}
}

void ZombieModernSuperfanImpProps::modInit() {
    LOGI("init superfan props");
    PVZ2HookFunction(0xBBF53C, (void*)construct, nullptr);
    PVZ2HookFunction(0xBBF694, (void*)ZombieModernSuperfanImpProps::buildSymbols, (void**)&ZombieModernSuperfanImpProps::oZombieModernSuperfanImpPropsBuildSymbols);
    PVZ2HookFunction(0xBC0960, (void*)hkSuperfanImpAF, (void**)&oSuperfanAF);
    LOGI("finish superfan");
}
