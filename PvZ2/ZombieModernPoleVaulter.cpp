
#include "ZombieModernPoleVaulter.h"
#include "ZombieStateHelper.h"
#include "ZombieModernPoleVaulterProps.h"
#include "ZombieState.h"
#include "StateMachineBuilder.h"
#include "ZombieAnimRig_ModernPoleVaulter.h"

typedef void (*zombieEnterState)(ZombieModernPoleVaulter*, int, int);
typedef Zombie* (*zombieAllowMovement)(Zombie*, int);
DECLARE_DELEGATES_SETUP(ZombieModernPoleVaulter)

static Sexy::DelegateBase jumpingCompletedDelegate;

void ZombieModernPoleVaulter::OnSpawn(ZombieModernPoleVaulter* zombie)
{
	auto rig = reinterpret_cast<ZombieAnimRig_ModernPoleVaulter*>(zombie->m_animRig.Get());
	rig->m_hasPole = true;
	typedef void (*zombieFun49)(ZombieModernPoleVaulter*);
	((zombieFun49)getActualOffset(0xC3D1F0))(zombie);
}

void ZombieModernPoleVaulter::WalkOnLoop(ZombieModernPoleVaulter* zombie)
{

}

void ZombieModernPoleVaulter::JumpOnEnter(ZombieModernPoleVaulter* zombie)
{

}
void ZombieModernPoleVaulter::JumpOnLoop(ZombieModernPoleVaulter* zombie)
{

}
void ZombieModernPoleVaulter::JumpOnExit(ZombieModernPoleVaulter* zombie)
{

}