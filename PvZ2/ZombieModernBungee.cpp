//#include "ZombieModernBungee.h"
//#include "AddZombieType.h"
//#include "ZombieState.h"
//#include "StateMachineBuilder.h"
//#include "ZombieHelper.h"
//#include "ZombieAnimRig_Basic.h"
//#include "ZombieStateHelper.h"
//#include "ZombieModernBungeeTarget.h"
//
//void* ZombieModernBungee::vftable = nullptr;
//Sexy::RtClass* ZombieModernBungee::s_rtClass = nullptr;
//
//void* ZombieModernBungeeProps::vftable = nullptr;
//Sexy::RtClass* ZombieModernBungeeProps::s_rtClass = nullptr;
//
//void ZombieModernBungee::HuntOnEnter(ZombieModernBungee* zombie)
//{
//		//TODO: Implement choose highest cost plants
//		ZombieModernBungeeTarget* target = (ZombieModernBungeeTarget*)AddZombie("modern_bungee_target", -1, 6, -1);
//		zombie->m_target.FromOther(&target->m_thisPtr);
//}
//
//void ZombieModernBungee::HuntOnLoop(ZombieModernBungee* zombie)
//{
//	auto props = reinterpret_cast<ZombieModernBungeeProps*>(zombie->m_propertySheet.Get());
//	if (zombie->m_elapsedTimeInState >= props->TimeToFallingDown) {
//		zombie->m_preReveal = false;
//		ZombieEnterState(zombie, 17, 0);
//	}
//}
//
//void ZombieModernBungee::HuntOnExit(ZombieModernBungee* zombie)
//{
//	
//}
//
//void ZombieModernBungee::FallOnEnter(ZombieModernBungee* zombie)
//{
//	auto targetPlant = reinterpret_cast<PlantGroup*>(zombie->m_targetedPlant.Get());
//	ZombieUpdatePosition(zombie, &targetPlant->m_position);
//	RegisterEventAfterAnim(zombie, "fall", "onFallingDown");
//}
//
//void ZombieModernBungee::FallOnLoop(ZombieModernBungee* zombie)
//{
//}
//
//void ZombieModernBungee::FallOnExit(ZombieModernBungee* zombie)
//{
//
//}
//
//void ZombieModernBungee::WaitingOnEnter(ZombieModernBungee* zombie)
//{
//	auto dlgtEvent = RegisterDelegateEvent(zombie, "onIdleAnimationCycle");
//	auto rig = reinterpret_cast<ZombieAnimRig_BasicTemplate*>(zombie->m_animRig.Get());
//	PlayAndContinueAnim(rig, "idle", 3, dlgtEvent);
//}
//
//void ZombieModernBungee::WaitingOnLoop(ZombieModernBungee* zombie)
//{
//	auto props = reinterpret_cast<ZombieModernBungeeProps*>(zombie->m_propertySheet.Get());
//	if (zombie->m_elapsedTimeInState >= props->PlantLiftingWaitTime) {
//		ZombieEnterState(zombie, 19, 0);
//	}
//}
//
//void ZombieModernBungee::WaitingOnExit(ZombieModernBungee* zombie)
//{
//}
//
//void ZombieModernBungee::GrabOnEnter(ZombieModernBungee* zombie)
//{
//}
//
//void ZombieModernBungee::GrabOnLoop(ZombieModernBungee* zombie)
//{
//	RegisterEventAfterAnim(zombie, "grab", "onGrabbingDone");
//}
//
//void ZombieModernBungee::GrabOnExit(ZombieModernBungee* zombie)
//{
//}
//
//void ZombieModernBungee::EscapeOnEnter(ZombieModernBungee* zombie)
//{
//	RegisterEventAfterAnim(zombie, "rise", "onEscapedDone");
//}
//
//void ZombieModernBungee::EscapeOnLoop(ZombieModernBungee* zombie)
//{
//
//}
//
//void ZombieModernBungee::EscapeOnExit(ZombieModernBungee* zombie)
//{
//}
//
//void ZombieModernBungee::ModInit() {
//    LOGI("ZombieBungee mod init");
//
//    vftable = CreateChildVFTable(204 + 15, getActualOffset(0x241D430), 204);
//    PatchVFTable(vftable, (void*)ZombieModernBungee::StaticGetType, 0);
//    ZombieModernBungee::StaticGetType();
//    LOGI("ZombieBungee finish init");
//}
//
//void ZombieModernBungeeProps::modInit() {
//    LOGI("ZombieBungeeProps init");
//
//    vftable = CopyVFTable(getActualOffset(0x24328D0), 14);
//
//    PatchVFTable(vftable, (void*)ZombieModernBungeeProps::StaticGetType, 0);
//
//    ZombieModernBungeeProps::StaticGetType();
//
//    LOGI("ZombieBungeeProps finish init");
//}
