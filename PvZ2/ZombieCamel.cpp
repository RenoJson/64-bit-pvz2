#include "ZombieCamel.h"

Sexy::RtClass* ZombieCamelProps::s_rtClass = __null;
Sexy::RtClass* ZombieCamelTouchProps::s_rtClass = __null;

typedef Sexy::RtClass* (*GetType)();
GetType oGetType = nullptr;

typedef Sexy::RtClass* (*TGetType)();
TGetType oTGetType = nullptr;
typedef void* (*oCamel)(Zombie* thisPtr, int64_t a2, bool a3);
oCamel oCmn = nullptr;

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
typedef void (*Func_ApplyArmor)(ZombieCamel* zombie, SexyString* armorName);
Func_ApplyArmor ApplyArmor = (Func_ApplyArmor)getActualOffset(0xC3EE98);

typedef void (*Func_SetLeader)(ZombieCamel*, int, RtWeakPtr<Zombie>*);
Func_SetLeader SetCamelLeaderFlag = (Func_SetLeader)getActualOffset(0xB165E4);

typedef ZombieCamel* (*Func_SpawnFollowers)(ZombieCamel*, int, int);
Func_SpawnFollowers SpawnFollowers = (Func_SpawnFollowers)getActualOffset(0xB16770);

typedef void (*Func_ZombieOnSpawn)(ZombieCamel*);
Func_ZombieOnSpawn OnSpawn = (Func_ZombieOnSpawn)getActualOffset(0xC3D1F0);

ZombieCamel* ZombieCamelOnSpawn(ZombieCamel* zombie)
{
    OnSpawn(zombie);

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
typedef ZombieCamelTouch* (*Func_SpawnTFollowers)(ZombieCamelTouch*, int, int);
Func_SpawnTFollowers SpawnTFollowers = (Func_SpawnTFollowers)getActualOffset(0xB16770);

typedef void (*Func_ApplyCardType)(ZombieCamelTouch*, int);
Func_ApplyCardType ApplyCardType = (Func_ApplyCardType)getActualOffset(0xB1A4B4);

typedef ZombieCamelTouch* (*Func_InitCardState)(ZombieCamelTouch*);
Func_InitCardState InitCardState = (Func_InitCardState)getActualOffset(0xB1AD14);

void BroadcastZombieSpawnEvent(ZombieCamelTouch* zombie)
{
    void** pMessageRouter = (void**)getActualOffset(0x2513070);

    if (pMessageRouter != nullptr && *pMessageRouter != nullptr)
    {
        void* messageRouter = *pMessageRouter;

        int* pLockDepth = (int*)((uintptr_t)messageRouter + 0x50);
        (*pLockDepth)++;

        uintptr_t vtable = *(uintptr_t*)messageRouter;
        typedef int64_t* (*Func_GetListeners)(void*, void*);

        Func_GetListeners GetListeners = (Func_GetListeners)(*(uintptr_t*)(vtable + 24));

        void* dummy = nullptr;
        int64_t* listenerList = GetListeners(messageRouter, &dummy);

        if (listenerList != nullptr)
        {
            uintptr_t currentItem = listenerList[0];
            uintptr_t endItem = listenerList[1];

            int listenerCount = (endItem - currentItem) / 48;

            int broadcastCount = 0;
            while (currentItem != endItem)
            {
                typedef void (*Func_OnSpawned)(uintptr_t, ZombieCamelTouch*);

                Func_OnSpawned OnSpawned = (Func_OnSpawned)(*(uintptr_t*)(currentItem + 40));

                OnSpawned(currentItem, zombie);
                currentItem += 48;
                broadcastCount++;
            }
        }

        (*pLockDepth)--;

        if (*pLockDepth == 0)
        {
            typedef void (*Func_FlushRouter)(void*);
            Func_FlushRouter FlushRouter = (Func_FlushRouter)getActualOffset(0x96C98C);
            FlushRouter(messageRouter);
        }
    }
}
typedef void (*zombieEnterState)(ZombieCamelTouch*, int, int);
ZombieCamelTouch* ZombieCamelTouchOnSpawn(ZombieCamelTouch* zombie)
{
    BroadcastZombieSpawnEvent(zombie);
        auto* props = reinterpret_cast<ZombieCamelTouchProps*>(zombie->m_propertySheet.Get());
        int followersToSpawn = props->SegmentCount;

        if (followersToSpawn > 0)
        {
            RtWeakPtr<Zombie> zombiePtr;
            zombiePtr.FromOther((RtWeakPtr<Zombie>*) & zombie->m_thisPtr);
            SetCamelLeaderFlag(zombie, 1, &zombiePtr);
            SpawnTFollowers(zombie, followersToSpawn, 0);
        }
    
    ApplyCardType(zombie, static_cast<int>(zombie->m_cardType));
    ((zombieEnterState)getActualOffset(0xC3D428))(zombie, 19, 0);
    return InitCardState(zombie);
}
Reflection::CRefManualSymbolBuilder::BuildSymbolsFunc ZombieCamelProps::oZombieCamelPropsBuildSymbols = nullptr;
Reflection::CRefManualSymbolBuilder::BuildSymbolsFunc ZombieCamelTouchProps::oZombieCamelTouchPropsBuildSymbols = nullptr;

void ZombieCamelProps::modInit() {
    LOGI("init Camel class");
    PVZ2HookFunction(0xC02C70, (void*)GetSegmentName, nullptr);
    PVZ2HookFunction(0xC02CA0, (void*)GetZombieTypeName, nullptr);
    PVZ2HookFunction(0xC02CC8, (void*)GetZombieTypeName, nullptr);
    PVZ2HookFunction(0xB162A8, (void*)ZombieCamelOnSpawn, nullptr);
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
    //PVZ2HookFunction(0xB1BAF0, (void*)ZombieCamelTouchOnSpawn, nullptr);
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