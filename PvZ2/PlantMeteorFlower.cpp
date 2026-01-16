#include "PvZ2/PlantMeteorFlower.h"
#include "MeteorFlowerProps.h"

void* PlantMeteorFlower::vftable = nullptr;
Sexy::RtClass* PlantMeteorFlower::s_rtClass = nullptr;

#pragma region warming

typedef void* (*getV8)();
getV8 pGetRegistry = (getV8)getActualOffset(OFFSET_RTWEAKPTR_GETREGISTRY);

typedef void (*dtor)(void*);
dtor ptrDtor = (dtor)getActualOffset(0x10C8B38);

typedef void* (*sub10CC404)(void*, int);
sub10CC404 pGetTable = (sub10CC404)getActualOffset(OFFSET_RTWEAKPTR_GETTABLEOFTYPE);

typedef void* (*setStateFunc)(PlantMeteorFlower*, int);
setStateFunc bananaSetState = (setStateFunc)getActualOffset(0xA518F0);

typedef void* (*SetComp)(Plant*, Sexy::RtWeakPtr<PlantPropertySheet>*, SexyString*);
SetComp pSetComp = (SetComp)getActualOffset(0xAC5CD8);

typedef Sexy::RtWeakPtr<ComponentWarmingRadius>* (*func10c8a84)(Sexy::RtWeakPtr<ComponentWarmingRadius>, Sexy::RtWeakPtr<ComponentWarmingRadius>);

func10c8a84 fun10c8a84 = (func10c8a84)getActualOffset(0x10C8A84);

typedef void* (*MoveFunc)(void*, void* );

MoveFunc pMove = (MoveFunc)getActualOffset(OFFSET_RTWEAKPTR_MOVE);

typedef void (*func921938)(Sexy::RtWeakPtr<PowerPropsWarmingRadius>*, Sexy::RtObject*, int, int );

func921938 fun921938 = (func921938)getActualOffset(0x921938);

typedef ComponentWarmingRadiusProps* (*func413f64)(ComponentWarmingRadius*, PowerPropsWarmingRadius*);
func413f64 fun413f64 = (func413f64)getActualOffset(0x413F64);

typedef void (*sub10CC81C_3)(Sexy::RtWeakPtr<PowerPropsWarmingRadius>*, void*, Sexy::RtWeakPtr<PowerPropsWarmingRadius>*);
sub10CC81C_3 pCopyPowerPropsPtr = (sub10CC81C_3)getActualOffset(OFFSET_RTWEAKPTR_COPYWEAKPTR);

typedef int* (*sub10C8780_4)(void*, Sexy::RtWeakPtr<PowerPropsWarmingRadius>*);
sub10C8780_4 pResolvePower = (sub10C8780_4)getActualOffset(OFFSET_RTWEAKPTR_RESOLVE);


void SetState(PlantMeteorFlower* a1, int a2) {
    a2 = 1;
    bananaSetState(a1, a2);
    Sexy::RtWeakPtr<PlantPropertySheet> props =  &a1->m_plant->m_propertySheet;
    Plant* plant = a1->m_plant;
    Sexy::RtWeakPtr<PlantPropertySheet> v43, v44;
    v43.FromOther(&props);
    v44.FromOther(&v43);
    SexyString power = "WarmingRadius";
    LOGI("Stuck in here TT");
    void* setComp = pSetComp(plant, &v44, &power);
    LOGI("Yay");
    Sexy::RtWeakPtr<ComponentWarmingRadius> v45, v46;
    LOGI("Prepare");
    uintptr_t rawBase = (uintptr_t)setComp;
    Sexy::RtWeakPtr<ComponentWarmingRadius>* powerNamePtr = (Sexy::RtWeakPtr<ComponentWarmingRadius>*)(rawBase + 8);
    LOGI("For the");
    v45.FromOther(powerNamePtr);
    fun10c8a84(&v46, &v45);
    pMove(&a1->m_warmingRadius, &v46);
    LOGI("Mighty");
    ptrDtor(&v46);
    ptrDtor(&v45);
    ptrDtor(&v44);
    ptrDtor(&v43);
    LOGI("Millhouse");
    //i'm not sure does v25 and animRig is right one???
    auto* comp = reinterpret_cast<ComponentWarmingRadius*>(a1->m_warmingRadius.Get());
    auto* animRig = reinterpret_cast<Sexy::RtObject*>(a1->m_plant->m_animRig.Get());
    Sexy::RtWeakPtr<PowerPropsWarmingRadius> v40;
    void* v25 = (void*)((uintptr_t)animRig + 116);
    fun921938(&v40, animRig, 2, 0);
    LOGI("Manastorm");
    PowerPropsWarmingRadius* CompRadius = (PowerPropsWarmingRadius*)&comp->m_warming_props;
    PowerPropsWarmingRadius* WarmingRadius = nullptr;
    Sexy::RtWeakPtr<PowerPropsWarmingRadius> propsTempPtr;
    void* registry = pGetRegistry();
    LOGI("None shall pass");
    pCopyPowerPropsPtr(&propsTempPtr, registry, &v40);
    if (!propsTempPtr.IsInvalid())
    {
        int typeId = v40.TypeID();
        void* table = pGetTable(registry, typeId);

        if (table)
        {
            WarmingRadius = (PowerPropsWarmingRadius*)pResolvePower(table, &propsTempPtr);
        }
    }
    LOGI("Nightmare");
    if (WarmingRadius)
    {
        LOGI("All reflourished");
        typedef int (*Warming)(PowerPropsWarmingRadius*, int);
        int* vtable = *(int**)WarmingRadius;
        Warming warm = (Warming)(vtable[4]);
        bool isWarmEnable = (vtable == reinterpret_cast<int*>(getActualOffset(0xFC7704)));
        if (!warm(WarmingRadius, isWarmEnable))
        {
            LOGI("Nothing happen lmao");
        }
        LOGI("checkpoint");
        CompRadius = WarmingRadius;
    }
    LOGI("almost there");
    fun413f64(comp, CompRadius);
    ptrDtor(&v40);
    LOGI("JACKPOT");
}
#pragma endregion

void PlantMeteorFlower::modInit() {
    LOGI("PlantMeteorFlower init");

    vftable = copyVFTable(getActualOffset(0x1CBB418), 121);

    PatchVFTable(vftable, (void*)PlantMeteorFlower::StaticGetType, 0);

    //PatchVFTable(vftable, (void*)SetState, 7);

    PlantMeteorFlower::StaticGetType();

    LOGI("PlantMeteorFlower finish init");
}