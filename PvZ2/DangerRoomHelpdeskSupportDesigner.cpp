#include "DangerRoomLevelDesigner.h"
#include "WaveActionProperties.h"
using namespace Sexy;
void* DangerRoomHelpdeskSupportDesigner::vftable = __null;
Sexy::RtClass* DangerRoomHelpdeskSupportDesigner::s_rtClass = __null;;

ParachuteRainZombieSpawnerProps* getParachuteProps(DangerRoomHelpdeskSupportDesigner* props)
{
    RtClass* type = ParachuteRainZombieSpawnerProps::StaticGetType();
    typedef ParachuteRainZombieSpawnerProps* (*DesignerFunc)(RtClass*, uint, int);
    DesignerFunc gFunc = (DesignerFunc)getActualOffset(0x8AE4CC);
    ParachuteRainZombieSpawnerProps* obj = gFunc(type, 0x4F, 1);
    obj->ColumnStart = props->ColumnStart;
    obj->ColumnEnd = props->ColumnEnd;
    obj->WaveStartMessage = props->WaveStartMessage;
    obj->SpiderZombieName = props->SpiderZombieName;
    return obj;
}

void DangerRoomHelpdeskSupportDesigner::modInit() {
    LOGI("DangerRoomHelpdeskSupportDesigner init");

    vftable = CopyVFTable(getActualOffset(0x2376F80), 24);

    PatchVFTable(vftable, (void*)DangerRoomHelpdeskSupportDesigner::StaticGetType, 0);
    PatchVFTable(vftable, (void*)getParachuteProps, 23);


    DangerRoomHelpdeskSupportDesigner::StaticGetType();
    LOGI("DangerRoomHelpdeskSupportDesigner finish init");
}