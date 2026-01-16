#pragma once
#include <Sexy/RtObject.h>
#include <Sexy/RtWeakPtr.h>
#include <Sexy/DelegateBase.h>

class ZombieEvent
{
public:
	Sexy::RtWeakPtr<Sexy::RtObject> m_ownerPtr;
	SexyString m_eventName;
	Sexy::DelegateBase m_delegate;
	bool m_isDelegateSetup;
	char m_pad[16];
};

static_assert(sizeof(ZombieEvent) == 88);
static_assert(offsetof(ZombieEvent, m_ownerPtr) == 0);
static_assert(offsetof(ZombieEvent, m_isDelegateSetup) == 64);