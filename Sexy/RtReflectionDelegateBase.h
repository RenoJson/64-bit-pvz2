#pragma once

namespace Sexy
{
	class RtReflectionDelegateBase
	{
	public:
		int m_objectId;
		SexyString m_methodName;
	};

	static_assert(sizeof(RtReflectionDelegateBase) == 32);
	static_assert(offsetof(RtReflectionDelegateBase, m_objectId) == 0);
	static_assert(offsetof(RtReflectionDelegateBase, m_methodName) == 8);
}