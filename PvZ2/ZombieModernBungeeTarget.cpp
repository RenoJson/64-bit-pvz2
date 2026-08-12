//#include "ZombieModernBungeeTarget.h"
//#include "ZombieModernBungee.h"
//#include "AddZombieType.h"
//#include "ZombieState.h"
//#include "StateMachineBuilder.h"
//#include "ZombieHelper.h"
//#include "ZombieAnimRig_Basic.h"
//#include "ZombieStateHelper.h"
//
//void* ZombieModernBungeeTarget::vftable = nullptr;
//Sexy::RtClass* ZombieModernBungeeTarget::s_rtClass = nullptr;
//
//
//void ZombieModernBungeeTarget::MarkOnEnter(ZombieModernBungeeTarget* zombie)
//{
//	RegisterEventAfterAnim(zombie, "bz_idle", "onFallingEnd");
//}
//
//void ZombieModernBungeeTarget::MarkOnLoop(ZombieModernBungeeTarget* zombie)
//{
//
//}
//
//void ZombieModernBungeeTarget::MarkOnExit(ZombieModernBungeeTarget* zombie)
//{
//
//}
//
//void ZombieModernBungeeTarget::ModInit() {
//    LOGI("ZombieBungeeTarget mod init");
//
//    vftable = CreateChildVFTable(204 + 15, getActualOffset(0x241D430), 204);
//    PatchVFTable(vftable, (void*)ZombieModernBungeeTarget::StaticGetType, 0);
//    ZombieModernBungeeTarget::StaticGetType();
//    LOGI("ZombieBungeeTarget finish init");
//}
//
