#pragma once
#include <Sexy/RtObject.h>
#include <Sexy/RtWeakPtr.h>
#include <Sexy/DelegateBase.h>

namespace Sexy
{
	class RtReflectionDelegateBase
	{
	public:
		Sexy::RtWeakPtr<Sexy::RtObject> m_ownerPtr;
		SexyString m_methodName;
		Sexy::DelegateBase m_delegate;
		bool m_isDelegateSetup;
	};

	static_assert(sizeof(RtReflectionDelegateBase) == 88);
	static_assert(offsetof(RtReflectionDelegateBase, m_ownerPtr) == 0);
	static_assert(offsetof(RtReflectionDelegateBase, m_methodName) == 8);
}