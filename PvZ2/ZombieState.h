#pragma once
#include <Sexy/DelegateBase.h>

class ZombieState
{
public:
	int m_id;
	Sexy::DelegateBase m_onEnter;
	Sexy::DelegateBase m_onLoop;
	Sexy::DelegateBase m_onExit;
	SexyString m_name;
};