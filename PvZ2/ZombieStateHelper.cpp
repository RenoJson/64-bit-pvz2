#include "pch.h"
#include "ZombieStateHelper.h"
#include "memUtils.h"
#include "ZombieState.h"
#include "Sexy/DelegateBase.h"
#include "Zombie.h"
#include "StateMachineBuilder.h"

#define ZOMBIE_STATE_ADDSTATE_ADDR 0xAC7638
#define ZOMBIE_EVENT_CONSTRUCT_ADDR 0x6FDDDC
#define DELEGATE_CONSTRUCT_ADDR 0x68C408
#define DELEGATE_GENERIC_ADDR 0x2377498
#define DELEGATE_ZOMBIE_STATE_ADDR 0x241A528
#define DELEGATE_ADDR 0x2377498
#define EXECUTE_CALLBACK_ADDR 0xC19FE0


typedef uintptr_t (*sub709E68)(Reflection::CRefManualSymbolBuilder*, const SexyString&);
typedef uintptr_t(*sub161BE6C)(Reflection::CRefManualSymbolBuilder*, uintptr_t, void*, void*, uintptr_t, uintptr_t);

typedef void* (*SetStateName)(SexyString* straddr, const SexyString& stateName, size_t length);
typedef void* (*RegisterState)(void* stateMachine, int stateid, ZombieState* state);

typedef int (*playAnimWithCallback)(ZombieAnimRig*, const SexyString&, int, ZombieEvent& event);
typedef int (*playAnimWithoutCallback)(ZombieAnimRig*, const SexyString&, int, ZombieEvent& event);

typedef int (*playLoopWalkAnimWithCallback)(ZombieAnimRig*, ZombieEvent& event);
typedef int (*playLoopIdleAnimWithCallback)(ZombieAnimRig*, ZombieEvent& event);
typedef StateMachineTableBuilder* (*getStateMachine)(StateMachineTableBuilder*, Sexy::RtClass*);

typedef ZombieEvent* (*ConstructEvent)(ZombieEvent*, RtWeakPtr<Zombie>& owner, const SexyString& eventName);
typedef RtReflectionDelegateBase* (*ConstructDelegateEvent)(RtReflectionDelegateBase*, RtWeakPtr<Zombie>& owner, const SexyString& eventName);

void RegisterEventCallback(Reflection::CRefManualSymbolBuilder* builder, void* rClass, const SexyString& eventName, Sexy::DelegateBase& delegate) {
	void* voidPropType = builder->GetPropertyOfType(Reflection::Type_Void, 0);
	uintptr_t unk1 = ((sub709E68)getActualOffset(0x709E68))(builder, eventName);
	uintptr_t unk2 = ((sub161BE6C)getActualOffset(0x161BE6C))(builder, 6, rClass, voidPropType, 1, unk1);
	builder->RegisterCallback(rClass, eventName, &delegate, unk2, 0);
	//LOGI("Callback registered");
}

void setVftable(Sexy::DelegateBase* a1, uint vtableAddr) {
	*(uintptr_t*)a1 = getActualOffset(vtableAddr);
}

void setupDelegate(Sexy::DelegateBase& delegate, uintptr_t owner, uintptr_t funcOffset) {

	delegate.m_callbackFunc = funcOffset;
	delegate.m_vftableDelta = false;
	delegate.m_callee = NULL;
	delegate.m_initialized = 1;
	delegate.m_executeCallbackFunction = (void*)getActualOffset(EXECUTE_CALLBACK_ADDR);
}
ZombieState* ConstructZombieState(ZombieState* state, int stateID,
	DelegateBase* onEnter,
	DelegateBase* onLoop,
	DelegateBase* onExit,
	const SexyString& stateName)
{
	state->m_id = stateID;

	*(void**)&state->m_onEnter = (void*)DELEGATE_ADDR;
	state->m_onEnter.m_callbackFunc = onEnter->m_callbackFunc;
	state->m_onEnter.m_vftableDelta = onEnter->m_vftableDelta;
	state->m_onEnter.m_callee = onEnter->m_callee;
	state->m_onEnter.m_initialized = onEnter->m_initialized;
	state->m_onEnter.m_executeCallbackFunction = onEnter->m_executeCallbackFunction;
	*(void**)&state->m_onEnter = (void*)DELEGATE_ZOMBIE_STATE_ADDR;

	*(void**)&state->m_onLoop = (void*)DELEGATE_ADDR;
	state->m_onLoop.m_callbackFunc = onLoop->m_callbackFunc;
	state->m_onLoop.m_vftableDelta = onLoop->m_vftableDelta;
	state->m_onLoop.m_callee = onLoop->m_callee;
	state->m_onLoop.m_initialized = onLoop->m_initialized;
	state->m_onLoop.m_executeCallbackFunction = onLoop->m_executeCallbackFunction;
	*(void**)&state->m_onLoop = (void*)DELEGATE_GENERIC_ADDR;

	*(void**)&state->m_onExit = (void*)DELEGATE_ADDR;
	state->m_onExit.m_callbackFunc = onExit->m_callbackFunc;
	state->m_onExit.m_vftableDelta = onExit->m_vftableDelta;
	state->m_onExit.m_callee = onExit->m_callee;
	state->m_onExit.m_initialized = onExit->m_initialized;
	state->m_onExit.m_executeCallbackFunction = onExit->m_executeCallbackFunction;
	*(void**)&state->m_onExit = (void*)DELEGATE_ZOMBIE_STATE_ADDR;

	const char* stateNameCopy = stateName.c_str();
	((SetStateName)getActualOffset(0x5AFA28))(&state->m_name, stateNameCopy, stateName.length());


	return state;
}
void RegisterStateByOffsets(StateMachineTableBuilder* stateMachine, int stateID, uintptr_t onEnterOffset, uintptr_t onLoopOffset, uintptr_t onExitOffset, const SexyString& stateName) {

	Sexy::DelegateBase onEnter, onLoop, onExit; 
	
	setVftable(&onEnter, DELEGATE_ZOMBIE_STATE_ADDR);
	setupDelegate(onEnter, NULL, onEnterOffset);

	setVftable(&onExit, DELEGATE_ZOMBIE_STATE_ADDR);
	setupDelegate(onExit, NULL, onExitOffset);

	setVftable(&onLoop, DELEGATE_GENERIC_ADDR);
	setupDelegate(onLoop, NULL, onLoopOffset);

	ZombieState state;
	ConstructZombieState(&state, stateID, &onEnter, &onLoop, &onExit, stateName);
	typedef void (*RegisterStateFunc)(StateMachineTableBuilder*, int, ZombieState*);
	((RegisterStateFunc)getActualOffset(ZOMBIE_STATE_ADDSTATE_ADDR))(stateMachine, stateID, &state);
}

StateMachineTableBuilder* CallGetStateMachine(Sexy::RtClass* rClass) {

	auto instance = StateMachineTableBuilder::GetInstance();

	typedef StateMachineTableBuilder* (*funcGetStateMachine)(StateMachineTableBuilder*, Sexy::RtClass*);
	auto* func = ((funcGetStateMachine)getActualOffset(0xAC745C));

	return func(instance, rClass);
}

void RegisterEventAfterAnim(Zombie* zombie, const SexyString& animName, const SexyString& eventName) {
	auto* animRig = reinterpret_cast<ZombieAnimRig*>(zombie->m_animRig.Get());
	RtWeakPtr<Zombie> zombiePtr;
	zombiePtr.FromOther((RtWeakPtr<Zombie>*) & zombie->m_thisPtr);

	ZombieEvent zombieEvent;
	((ConstructEvent)getActualOffset(ZOMBIE_EVENT_CONSTRUCT_ADDR))(&zombieEvent, zombiePtr, eventName);


	playAnimWithCallback func = ((playAnimWithCallback)getActualOffset(0x8DCEDC));
	
	func(animRig, animName, 0, zombieEvent);
}

RtReflectionDelegateBase RegisterDelegateEvent(Zombie* zombie, const SexyString& eventName) {
	RtWeakPtr<Zombie> zombiePtr;
	zombiePtr.FromOther((RtWeakPtr<Zombie>*) & zombie->m_thisPtr);
	RtReflectionDelegateBase dlgt;
	auto constructFunc = (ConstructDelegateEvent)getActualOffset(DELEGATE_CONSTRUCT_ADDR);
	constructFunc(&dlgt, zombiePtr, eventName);
	return dlgt;
}

void RegisterEventOnWalkLoop(Zombie* zombie, const SexyString& eventName) {
	auto* animRig = reinterpret_cast<ZombieAnimRig*>(zombie->m_animRig.Get());
	RtWeakPtr<Zombie> zombiePtr;
	zombiePtr.FromOther((RtWeakPtr<Zombie>*) & zombie->m_thisPtr);

	ZombieEvent zombieEvent;
	((ConstructEvent)getActualOffset(ZOMBIE_EVENT_CONSTRUCT_ADDR))(&zombieEvent, zombiePtr, eventName);

	playLoopWalkAnimWithCallback func = ((playLoopWalkAnimWithCallback)getActualOffset(0x8DC460));
	func(animRig, zombieEvent);
}

void RegisterEventOnIdleLoop(Zombie* zombie, const SexyString& eventName) {
	auto* animRig = reinterpret_cast<ZombieAnimRig*>(zombie->m_animRig.Get());
	RtWeakPtr<Zombie> zombiePtr;
	zombiePtr.FromOther((RtWeakPtr<Zombie>*) & zombie->m_thisPtr);

	ZombieEvent zombieEvent;
	((ConstructEvent)getActualOffset(ZOMBIE_EVENT_CONSTRUCT_ADDR))(&zombieEvent, zombiePtr, eventName);

	playLoopIdleAnimWithCallback func = ((playLoopIdleAnimWithCallback)getActualOffset(0x8DC27C));
	func(animRig, zombieEvent);
}

void RegisterEventOnLoop(Zombie* zombie, const SexyString& animName, const SexyString& eventName) {
	auto* animRig = reinterpret_cast<ZombieAnimRig*>(zombie->m_animRig.Get());
	RtWeakPtr<Zombie> zombiePtr;
	zombiePtr.FromOther((RtWeakPtr<Zombie>*) &zombie->m_thisPtr);

	ZombieEvent zombieEvent;
	((ConstructEvent)getActualOffset(ZOMBIE_EVENT_CONSTRUCT_ADDR))(&zombieEvent, zombiePtr, eventName); 

	playAnimWithCallback func = ((playAnimWithCallback)getActualOffset(0x8DCEDC));

	func(animRig, animName, 3, zombieEvent);
}

void SetupLiteralDelegate(Sexy::DelegateBase* delegate, void (*delegateFun)(Zombie*)) {
	uintptr_t delegateAddr = (uintptr_t)delegateFun;

	delegate->m_callbackFunc = delegateAddr;
	delegate->m_vftableDelta = false;
	delegate->m_callee = NULL;
	delegate->m_initialized = 1;
	delegate->m_executeCallbackFunction = (void*)getActualOffset(EXECUTE_CALLBACK_ADDR);
}

void SetupLiteralDelegate(Sexy::DelegateBase* delegate, uintptr_t delegateAddr) {

	delegate->m_callbackFunc = delegateAddr;
	delegate->m_vftableDelta = false;
	delegate->m_callee = NULL;
	delegate->m_initialized = 1;
	delegate->m_executeCallbackFunction = (void*)getActualOffset(EXECUTE_CALLBACK_ADDR);
}

