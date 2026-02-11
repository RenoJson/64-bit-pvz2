#include "DangerRoomLevelDesigner.h"
#include "WaveActionProperties.h"
using namespace Sexy;
void* DangerRoomFallenKnightDesigner::vftable = __null;
Sexy::RtClass* DangerRoomFallenKnightDesigner::s_rtClass = __null;;

SpiderRainZombieSpawnerProps* getProps(DangerRoomFallenKnightDesigner* props)
{
    RtClass* type = SpiderRainZombieSpawnerProps::StaticGetType();
    typedef SpiderRainZombieSpawnerProps* (*DesignerFunc)(RtClass*, uint, int);
    DesignerFunc gFunc = (DesignerFunc)getActualOffset(0x8AE4CC);
    SpiderRainZombieSpawnerProps* obj = gFunc(type, 0x4F, 1);
    obj->ColumnStart = props->ColumnStart;
    obj->ColumnEnd = props->ColumnEnd;
    obj->WaveStartMessage = props->WaveStartMessage;
    obj->SpiderZombieName = props->SpiderZombieName;
    return obj;
}

void DangerRoomFallenKnightDesigner::modInit() {
    LOGI("DangerRoomFallenKnightDesigner init");

    vftable = CopyVFTable(getActualOffset(0x2490EB0), 24);

    PatchVFTable(vftable, (void*)DangerRoomFallenKnightDesigner::StaticGetType, 0);
    PatchVFTable(vftable, (void*)getProps, 23);


    DangerRoomFallenKnightDesigner::StaticGetType();
    LOGI("DangerRoomFallenKnightDesigner finish init");
}