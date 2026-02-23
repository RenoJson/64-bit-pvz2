#include "pch.h"
#include "ZombieStateHelper.h"
#include "memUtils.h"
#include "ZombieState.h"
#include "Sexy/DelegateBase.h"
#include "Zombie.h"

#define ZOMBIE_STATE_ADDSTATE_ADDR 0xAC7638
#define ZOMBIE_EVENT_CONSTRUCT_ADDR 0x6FDDDC
#define DELEGATE_GENERIC_ADDR 0x2377498
#define DELEGATE_ZOMBIE_STATE_ADDR 0x241A528
#define EXECUTE_CALLBACK_ADDR 0xC19FE0


typedef uintptr_t (*sub709E68)(Reflection::CRefManualSymbolBuilder*, const SexyString&);
typedef uintptr_t(*sub161BE6C)(Reflection::CRefManualSymbolBuilder*, uintptr_t, void*, void*, uintptr_t, uintptr_t);

typedef void* (*SetStateName)(SexyString* straddr, const SexyString& stateName, size_t length);
typedef void* (*RegisterState)(void* stateMachine, int stateid, ZombieState* state);

typedef void* (*getStateMachine)(void*, Sexy::RtClass*);

typedef ZombieEvent* (*ConstructEvent)(ZombieEvent*, RtWeakPtr<Zombie>& owner, const SexyString& eventName);

void RegisterEventCallback(Reflection::CRefManualSymbolBuilder* builder, void* rClass, const SexyString& eventName, Sexy::DelegateBase& delegate) {
	void* voidPropType = builder->GetPropertyOfType(Reflection::Type_Void, 0);
	uintptr_t unk1 = ((sub709E68)getActualOffset(0x709E68))(builder, eventName);
	uintptr_t unk2 = ((sub161BE6C)getActualOffset(0x161BE6C))(builder, 6, rClass, voidPropType, 1, unk1);
	builder->RegisterCallback(rClass, eventName, &delegate, unk2, 0);
	LOGI("Callback registered");
}

void setVftable(Sexy::DelegateBase* a1, uint vtableAddr) {
	*(uintptr_t*)a1 = getActualOffset(vtableAddr);
}

void setupDelegate(Sexy::DelegateBase& a1, uintptr_t owner, uintptr_t funcOffset) {
	a1.m_callbackFunc = funcOffset;
	a1.m_useOwnerVtable = true;
	a1.m_callbackOwner = owner;
	a1.m_unk = 1;
	a1.m_executeCallbackFunction = getActualOffset(EXECUTE_CALLBACK_ADDR);
}

void RegisterStateByOffsets(void* stateMachine, int stateID, int onEnterOffset, int onLoopOffset, int onExitOffset, const SexyString& stateName) {
	// onLoop uses a normal Sexy::DelegateBase vtable, while the other 2 uses a special DelegateBase<ZombieState> one
	Sexy::DelegateBase onEnter, onLoop, onExit;
	setVftable(&onEnter, DELEGATE_ZOMBIE_STATE_ADDR);
	setupDelegate(onEnter, NULL, onEnterOffset);

	setVftable(&onExit, DELEGATE_ZOMBIE_STATE_ADDR);
	setupDelegate(onExit, NULL, onExitOffset);

	setVftable(&onLoop, DELEGATE_GENERIC_ADDR);
	setupDelegate(onLoop, NULL, onLoopOffset);

	ZombieState state;
	const char* stateNameCopy = stateName.c_str();
	((SetStateName)getActualOffset(0x5AFA28))(&state.m_name, stateNameCopy, stateName.length());
	((RegisterState)getActualOffset(ZOMBIE_STATE_ADDSTATE_ADDR))(stateMachine, stateID, &state);
}

void* GetStateMachine(Sexy::RtClass* rClass) {
	void* unk = *(void**)getActualOffset(0x256A0E0);
	return ((getStateMachine)getActualOffset(0xAC745C))(unk, rClass);
}

void RegisterEventAfterAnim(Zombie* zombie, SexyString* animName, const SexyString& eventName) {
	auto* animRig = reinterpret_cast<ZombieAnimRig*>(zombie->m_animRig.Get());
	RtWeakPtr<Zombie> zombiePtr;
	zombiePtr.FromOther((RtWeakPtr<Zombie>*) & zombie->m_thisPtr);

	ZombieEvent zombieEvent;
	((ConstructEvent)getActualOffset(ZOMBIE_EVENT_CONSTRUCT_ADDR))(&zombieEvent, zombiePtr, eventName);

	animRig->PlayAnimWithCallback(animName, 0, &zombieEvent);
}

void RegisterEventOnWalkLoop(Zombie* zombie, const SexyString& eventName) {
	auto* animRig = reinterpret_cast<ZombieAnimRig*>(zombie->m_animRig.Get());
	RtWeakPtr<Zombie> zombiePtr;
	zombiePtr.FromOther((RtWeakPtr<Zombie>*) & zombie->m_thisPtr);

	ZombieEvent zombieEvent;
	((ConstructEvent)getActualOffset(ZOMBIE_EVENT_CONSTRUCT_ADDR))(&zombieEvent, zombiePtr, eventName);

	// this is hardcoded to only respond to animations with name "walk"
	animRig->LoopWalkWithCallback(&zombieEvent);
}

void RegisterEventOnIdleLoop(Zombie* zombie, SexyString* animName, const SexyString& eventName) {
	auto* animRig = reinterpret_cast<ZombieAnimRig*>(zombie->m_animRig.Get());
	RtWeakPtr<Zombie> zombiePtr;
	zombiePtr.FromOther((RtWeakPtr<Zombie>*) & zombie->m_thisPtr);

	ZombieEvent zombieEvent;
	((ConstructEvent)getActualOffset(ZOMBIE_EVENT_CONSTRUCT_ADDR))(&zombieEvent, zombiePtr, eventName);

	animRig->PlayAnimWithCallback(animName, 3, &zombieEvent);
}

void RegisterEventOnLoop(Zombie* zombie, SexyString* animName, const SexyString& eventName) {
	auto* animRig = reinterpret_cast<ZombieAnimRig*>(zombie->m_animRig.Get());
	RtWeakPtr<Zombie> zombiePtr;
	zombiePtr.FromOther((RtWeakPtr<Zombie>*) &zombie->m_thisPtr);

	ZombieEvent zombieEvent;
	((ConstructEvent)getActualOffset(ZOMBIE_EVENT_CONSTRUCT_ADDR))(&zombieEvent, zombiePtr, eventName);
	animRig->PlayAnimWithCallback(animName, 3, &zombieEvent);
}

void SetupLiteralDelegate(Sexy::DelegateBase* delegate, void (*delegateFun)(Zombie*)) {
	uintptr_t delegateAddr = (uintptr_t)delegateFun;

	delegate->m_callbackFunc = delegateAddr;
	delegate->m_useOwnerVtable = false;
	delegate->m_callbackOwner = NULL;
	delegate->m_unk = 1;
	delegate->m_executeCallbackFunction = getActualOffset(EXECUTE_CALLBACK_ADDR);
}

void SetupLiteralDelegate(Sexy::DelegateBase* delegate, uintptr_t delegateAddr) {
	delegate->m_callbackFunc = delegateAddr;
	delegate->m_useOwnerVtable = false;
	delegate->m_callbackOwner = NULL;
	delegate->m_unk = 1;
	delegate->m_executeCallbackFunction = getActualOffset(EXECUTE_CALLBACK_ADDR);
}

void SetDesiredSpeed(Zombie* zombie, float speed) {
	auto* animRig = reinterpret_cast<ZombieAnimRig*>(zombie->m_animRig.Get());

	typedef void (*sub8DDAA4)(ZombieAnimRig*, float);
	((sub8DDAA4)getActualOffset(0x8DDAA4))(animRig, speed);
}
