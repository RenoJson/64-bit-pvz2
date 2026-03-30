#include "ZombiePharaoh.h"
#include "DamageInfo.h"

void* ZombieEgyptPharaoh::vftable = __null;
Sexy::RtClass* ZombieEgyptPharaoh::s_rtClass = __null;;


void* PharaohTakeDamage(ZombieEgyptPharaoh* thisPtr, DamageInfo* damageInfo)
{
    for (size_t i = 0; i < thisPtr->m_armor.size(); i++)
    {
        Armor* armor = thisPtr->m_armor[i].Get();
        if (armor != nullptr && !armor->m_destroyed && armor->m_health > 0)
        {
            if (damageInfo->m_damage > armor->m_health) {
               damageInfo->m_damage = armor->m_health - 10;
            }
        }
    }
    typedef void* (*funcC43B90)(ZombieEgyptPharaoh*, DamageInfo*);
    auto* ZTakeDmg = ((funcC43B90)getActualOffset(0xC43B90));
    return ZTakeDmg(thisPtr, damageInfo);
}

void ZombieEgyptPharaoh::ModInit() {
    LOGI("ZombieEgyptPharaoh init");

    vftable = CopyVFTable(getActualOffset(0x23E8A28), 207);

    PatchVFTable(vftable, (void*)ZombieEgyptPharaoh::StaticGetType, 0);

    PatchVFTable(vftable, (void*)PharaohTakeDamage, 35);

    ZombieEgyptPharaoh::StaticGetType();
    LOGI("ZombieEgyptPharaoh finish init");
}