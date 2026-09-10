#include "ZombieCamel.h"
#include "ZombieHelper.h"

Sexy::RtClass* ZombieCamelProps::s_rtClass = __null;
Sexy::RtClass* ZombieCamelTouchProps::s_rtClass = __null;

typedef Sexy::RtClass* (*GetType)();
GetType oGetType = nullptr;

typedef Sexy::RtClass* (*TGetType)();
TGetType oTGetType = nullptr;

void hkCamelZombieFunc(Zombie* thisPtr, int64_t a2, bool a3)
{
    CallFunc<void, Zombie*, int64_t, bool>(0xB1BE04, thisPtr, a2, a3);
}

SexyString GetSegmentName(ZombieCamel* zombie, ZombieCamelSegmentType segmentType)
{
    auto props = reinterpret_cast<ZombieCamelProps*>(zombie->m_propertySheet.Get());
    if (props->FollowerSegmentTypeOrder.empty()) {
        return props->ChooseYourSeedSegmentTypeName;
    }
    if (segmentType == ZombieCamelSegmentType::head)
        return props->FollowerSegmentTypeOrder[1];
    else if (segmentType == ZombieCamelSegmentType::body)
        return props->FollowerSegmentTypeOrder[2];
    else if (segmentType == ZombieCamelSegmentType::tail)
        return props->FollowerSegmentTypeOrder[3];
    else
        return props->FollowerSegmentTypeOrder[0];
}
SexyString GetSegmentTouchName(ZombieCamelTouch* zombie, ZombieCamelSegmentType segmentType)
{
    auto props = reinterpret_cast<ZombieCamelTouchProps*>(zombie->m_propertySheet.Get());


    if (props->FollowerSegmentTypeOrder.empty()) {
        return props->ChooseYourSeedSegmentTypeName;
    }
    if (segmentType == ZombieCamelSegmentType::head)
        return props->FollowerSegmentTypeOrder[1];
    else if (segmentType == ZombieCamelSegmentType::body)
        return props->FollowerSegmentTypeOrder[2];
    else if (segmentType == ZombieCamelSegmentType::tail)
        return props->FollowerSegmentTypeOrder[3];
    else
        return props->FollowerSegmentTypeOrder[0];
}

SexyString GetZombieTypeName(ZombieCamel* zombie)
{
    auto props = reinterpret_cast<ZombieCamelProps*>(zombie->m_propertySheet.Get());
    return props->ZombieTypeName;
}
SexyString GetZombieTouchTypeName(ZombieCamelTouch* zombie)
{
    auto props = reinterpret_cast<ZombieCamelTouchProps*>(zombie->m_propertySheet.Get());
    return props->ZombieTypeName;
}
typedef void (*Func_SetLeader)(ZombieCamel*, int, RtWeakPtr<Zombie>*);
Func_SetLeader SetCamelLeaderFlag = (Func_SetLeader)getActualOffset(0xB165E4);

typedef ZombieCamel* (*Func_SpawnFollowers)(ZombieCamel*, int, int);
Func_SpawnFollowers SpawnFollowers = (Func_SpawnFollowers)getActualOffset(0xB16770);

void CamelApplyArmor(ZombieCamel* zombie, ZombieCamelSegmentType segmentType) {
    auto props = reinterpret_cast<ZombieCamelProps*>(zombie->m_propertySheet.Get());
    SexyString armorName;
    if (segmentType == ZombieCamelSegmentType::head) {
        armorName = props->HeadArmorType;
    }
    else if (segmentType == ZombieCamelSegmentType::body) {
        armorName = props->MiddleArmorType;

    }
    else if (segmentType == ZombieCamelSegmentType::tail) {
        armorName = props->TailArmorType;
    }
    else {
        armorName = props->HeadArmorType;
    }
    ZombieApplyArmor(zombie, armorName);
}
ZombieCamel* ZombieCamelOnSpawn(ZombieCamel* zombie)
{
    ZombieOnSpawn(zombie);

    auto* props = reinterpret_cast<ZombieCamelProps*>(zombie->m_propertySheet.Get());
    int followersToSpawn = props->SegmentCount;
    if (followersToSpawn > 0)
    {
        RtWeakPtr<Zombie> zombiePtr;
        zombiePtr.FromOther((RtWeakPtr<Zombie>*) & zombie->m_thisPtr);
        SetCamelLeaderFlag(zombie, 1, &zombiePtr);
        ZombieCamel* result = SpawnFollowers(zombie, followersToSpawn, 0);
        return result;
    }
    return zombie;
}

ZombieCamel* CamelOnPlaceOnStreet(ZombieCamel* thisPtr)
{
    CallFunc<void>(0xC3D644, thisPtr);

    auto* props = reinterpret_cast<ZombieCamelProps*>(thisPtr->m_propertySheet.Get());

    int followersToSpawn = props->SegmentCount;
    if (followersToSpawn > 0) {
        RtWeakPtr<ZombieType> currentType;
        currentType.FromOther(&thisPtr->m_type);

        RtWeakPtr<ZombieType> bodyType = CallFunc<Sexy::RtWeakPtr<ZombieType>>(0xB16C5C, thisPtr, 2);

        if (currentType.Equals(&bodyType))
        {
            return thisPtr;
        }

        RtWeakPtr<ZombieType> tailType = CallFunc<Sexy::RtWeakPtr<ZombieType>>(0xB16C5C, thisPtr, 3);

        if (!currentType.Equals(&tailType))
        {
            float yShiftAmount = followersToSpawn * 25.0f;

            float newY = thisPtr->m_position.y + yShiftAmount;

            if (newY > 500.0f) {
                newY = 500.0f;
            }

            thisPtr->m_position.y = newY;
			ZombieUpdatePosition(thisPtr, &thisPtr->m_position);
            RtWeakPtr<Zombie> zombiePtr;
            zombiePtr.FromOther((RtWeakPtr<Zombie>*) & thisPtr->m_thisPtr);
            SetCamelLeaderFlag(thisPtr, 1, &zombiePtr);

            ZombieCamel* result = SpawnFollowers(thisPtr, followersToSpawn, 1);

            return result;
        }
    }
    return thisPtr;
}

Reflection::CRefManualSymbolBuilder::BuildSymbolsFunc ZombieCamelProps::oZombieCamelPropsBuildSymbols = nullptr;
Reflection::CRefManualSymbolBuilder::BuildSymbolsFunc ZombieCamelTouchProps::oZombieCamelTouchPropsBuildSymbols = nullptr;

void ZombieCamelProps::modInit() {
    LOGI("init Camel class");
    PVZ2HookFunction(0xC02C70, (void*)GetSegmentName, nullptr);
    PVZ2HookFunction(0xC02CA0, (void*)GetZombieTypeName, nullptr);
    PVZ2HookFunction(0xC02CC8, (void*)GetZombieTypeName, nullptr);
    PVZ2HookFunction(0xB162A8, (void*)ZombieCamelOnSpawn, nullptr);
    PVZ2HookFunction(0xB170D0, (void*)CamelApplyArmor, nullptr);
    PVZ2HookFunction(0xB16A9C, (void*)CamelOnPlaceOnStreet, nullptr);
    //Roman
    PVZ2HookFunction(0xC10498, (void*)GetSegmentName, nullptr);
    PVZ2HookFunction(0xC104F8, (void*)GetZombieTypeName, nullptr);
    PVZ2HookFunction(0xC10520, (void*)GetZombieTypeName, nullptr);
    PVZ2HookFunction(0xBDEDF8, (void*)ZombieCamelOnSpawn, nullptr);
    //LNY
    PVZ2HookFunction(0xC02D54, (void*)GetSegmentName, nullptr);
    PVZ2HookFunction(0xC02D8C, (void*)GetZombieTypeName, nullptr);
    PVZ2HookFunction(0xC02DB4, (void*)GetZombieTypeName, nullptr);
    LOGI("init Camel class complete");
    LOGI("init Camel props");
    PVZ2HookFunction(0xDAB6D8, (void*)StaticGetType, (void**)&oGetType);
    PVZ2HookFunction(0xDAB808, (void*)ZombieCamelProps::RegisterClass, nullptr);
    PVZ2HookFunction(0xDAB8D0, (void*)ZombieCamelProps::BuildSymbols, (void**)&ZombieCamelProps::oZombieCamelPropsBuildSymbols);
    ZombieCamelProps::StaticGetType();
    LOGI("init Camel props complete");
    LOGI("finish init Camel");
}
void ZombieCamelTouchProps::modInit() {
    LOGI("init CamelTouch class");
    PVZ2HookFunction(0xB18DC4, (void*)hkCamelZombieFunc, nullptr);
    PVZ2HookFunction(0xB1BDBC, (void*)GetSegmentTouchName, nullptr);
    LOGI("init CamelTouch class complete");
    LOGI("init CamelTouch props");
    PVZ2HookFunction(0xDABED8, (void*)StaticGetType, (void**)&oTGetType);
    PVZ2HookFunction(0xDABF40, (void*)construct, nullptr);
    PVZ2HookFunction(0xDABFCC, (void*)ZombieCamelTouchProps::RegisterClass, nullptr);
    PVZ2HookFunction(0xDAC09C, (void*)ZombieCamelTouchProps::BuildSymbols, (void**)&ZombieCamelTouchProps::oZombieCamelTouchPropsBuildSymbols);
    ZombieCamelTouchProps::StaticGetType();
    LOGI("init CamelTouch props complete");
    LOGI("finish init CamelTouch");
}