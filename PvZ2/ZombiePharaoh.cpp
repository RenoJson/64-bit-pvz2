#include "ZombiePharaoh.h"
#include "DamageInfo.h"


typedef void* (*ZTakeDmg)(Zombie*, DamageInfo*);
ZTakeDmg oZTakeDmg = nullptr;

void* hkTakeDamage(Zombie* thisPtr, DamageInfo* damageInfo)
{
    for (size_t i = 0; i < thisPtr->m_armor.size(); i++)
    {
        Armor* armor = thisPtr->m_armor[i].Get();

        if (armor != nullptr && !armor->m_destroyed && armor->m_health > 0)
        {
            auto* armorProps = reinterpret_cast<ArmorPropertySheet*>(armor->m_propertySheetPtr.Get());

            if (armorProps != nullptr)
            {
                bool hasAbsorbOverflow = false;

                for (size_t j = 0; j < armorProps->ArmorFlags.size(); j++)
                {
                    if (armorProps->ArmorFlags[j] == ArmorTypeFlags::absorboverflow)
                    {
                        hasAbsorbOverflow = true;
                        break;
                    }
                }
                if (hasAbsorbOverflow)
                {
                    if (damageInfo->m_damage >= armor->m_health)
                    {
                        damageInfo->m_flags &= ~DamageTypeFlags::damage_bypass_shield;
                        damageInfo->m_flags &= ~DamageTypeFlags::damage_ash_death;
                        damageInfo->m_flags &= ~DamageTypeFlags::damage_fire;
                        float calculatedDamage = armor->m_health;

                        if (thisPtr->m_damageScale > 0.001f)
                        {
                            calculatedDamage /= thisPtr->m_damageScale;
                        }

                        bool isShrunken = CallFunc<bool, Zombie*, int>(0xC3E44C, thisPtr, zombie_condition_shrinking) 
                            || CallFunc<bool, Zombie*, int>(0xC3E44C, thisPtr, zombie_condition_shrunken);

                        if (isShrunken && thisPtr->m_shrunkenDamageScale > 0.001f)
                        {
                            calculatedDamage /= thisPtr->m_shrunkenDamageScale;
                        }
                        damageInfo->m_damage = calculatedDamage;
                    }
                }
            }
            break; 
        }
    }

    return oZTakeDmg(thisPtr, damageInfo);
}

void ZombiePharaoh::ModInit() {
    LOGI("ZombiePharaoh init");
    PVZ2HookFunction(0xC43B90, (void*)hkTakeDamage, (void**)&oZTakeDmg);
    LOGI("ZombiePharaoh finish init");
}