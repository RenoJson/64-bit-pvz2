#pragma once

namespace Sexy
{
	class DelegateBase
	{
	public:
		void** delegateBaseVftable; // delegate base vftable
		uint64_t m_callbackFunc;
		uint64_t m_vftableDelta;
		void* m_callee;
		bool m_initialized;
		char pad[7];
		void* m_executeCallbackFunction;
	};
}

inline Sexy::DelegateBase CreateEmptyDelegate(uint64_t vftableAddr)
{
	Sexy::DelegateBase delegate = Sexy::DelegateBase();
	*(uint64_t*)&delegate = getActualOffset(vftableAddr);
	return delegate;
}

inline Sexy::DelegateBase CreateDelegate(
	uintptr_t vftableAddr,
	uintptr_t callbackFunc,
	uintptr_t vftableDelta,
	void* callee,
	bool initialized,
	void* executeCallbackFunction
) {
	Sexy::DelegateBase res = CreateEmptyDelegate(vftableAddr);
	res.m_callbackFunc = callbackFunc;
	res.m_vftableDelta = vftableDelta;
	res.m_callee = callee;
	res.m_initialized = initialized;
	res.m_executeCallbackFunction = executeCallbackFunction;
	return res;
}

inline Sexy::DelegateBase CreateDelegate(
	uintptr_t callbackFunc,
	uintptr_t vftableDelta,
	void* callee,
	bool initialized,
	void* executeCallbackFunction
) {
	return CreateDelegate(
		0x2377868,
		callbackFunc,
		vftableDelta,
		callee,
		initialized,
		executeCallbackFunction
	);
}