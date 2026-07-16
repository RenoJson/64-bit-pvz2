#pragma once

#include "Reflection/CRefManualSymbolBuilder.h"
#include <Reflection/ReflectionBuilder.h>
#include "ZombieEvent.h"
#include "Zombie.h"

class StateMachineTableBuilder;
class ZombieState;

#define DECLARE_DELEGATES_SETUP(zombie) \
    static bool zombie##_delegatesSetup = false; 

#define DECLARE_CALLBACK(zombie, eventName) \
	static Sexy::DelegateBase zombie##_##eventName##Delegate;

#define IF_CALLBACK_NOTSETUP(zombie) \
    if (! zombie##_delegatesSetup)

#define DEFINE_CALLBACK(zombie, event) \
    SetupLiteralDelegate(& zombie##_##event##Delegate, (uint) zombie##_##event##Callback);

#define REGISTER_EVENT_CALLBACK(builder, rclass, zombie, event) \
    RegisterEventCallback(builder, rclass, #event, zombie##_##event##Delegate);

#define DEFINE_CALLBACK_FUNC(zombie1, event) \
    void zombie1##_##event##Callback(zombie1 *a1)


void RegisterEventCallback(Reflection::CRefManualSymbolBuilder* builder, void* rClass, const SexyString& eventName, Sexy::DelegateBase& delegate);

ZombieState* ConstructZombieState(ZombieState * state, int stateID, DelegateBase * onEnter, DelegateBase * onLoop, DelegateBase * onExit, const SexyString & stateName);

void RegisterStateByOffsets(StateMachineTableBuilder * stateMachine, int stateID, uintptr_t onEnterOffset, uintptr_t onLoopOffset, uintptr_t onExitOffset, const SexyString & stateName);

StateMachineTableBuilder* CallGetStateMachine(Sexy::RtClass* rClass);

void RegisterEventAfterAnim(Zombie* zombie, const SexyString & animName, const SexyString& eventName);

void RegisterDelegateEvent(Zombie * zombie, const SexyString & animName, const SexyString & eventName);

void RegisterEventOnWalkLoop(Zombie* zombie, const SexyString& eventName);

void RegisterEventOnIdleLoop(Zombie* zombie, const SexyString & animName, const SexyString& eventName);

void RegisterEventOnLoop(Zombie* zombie, const SexyString & animName, const SexyString& eventName);

void SetupLiteralDelegate(Sexy::DelegateBase* delegate, void (*delegateFun)(Zombie*));

void SetupLiteralDelegate(Sexy::DelegateBase* delegate, uintptr_t delegateAddr);
