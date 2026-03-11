#pragma once

namespace Sexy
{
	class DelegateBase
	{
	public:
		uint64_t m_callbackFunc;
		bool m_useOwnerVtable;
		char pad[7];
		uint64_t m_callbackOwner;
		int m_unk;
		uint64_t m_executeCallbackFunction;

		virtual void Function0() {};
		virtual void Function1() {};
		virtual void Function2() {};
		virtual void Delete() {};
	};
}

inline Sexy::DelegateBase CreateEmptyDelegate(uint64_t vftableAddr)
{
	Sexy::DelegateBase delegate = Sexy::DelegateBase();
	*(uint64_t*)&delegate = getActualOffset(vftableAddr);
	return delegate;
}

inline Sexy::DelegateBase CreateDelegate(
	uint64_t vftableAddr,
	uint64_t callbackFunc,
	bool useOwnerVtable,
	uint64_t callbackOwner,
	int unk,
	uint64_t executeCallbackFunction
) {
	Sexy::DelegateBase res = CreateEmptyDelegate(vftableAddr);
	res.m_callbackFunc = callbackFunc;
	res.m_useOwnerVtable = useOwnerVtable;
	res.m_callbackOwner = callbackOwner;
	res.m_unk = unk;
	res.m_executeCallbackFunction = executeCallbackFunction;
	return res;
}

inline Sexy::DelegateBase CreateDelegate(
	uint64_t callbackFunc,
	bool useOwnerVtable,
	uint64_t callbackOwner,
	int unk,
	uint64_t executeCallbackFunction
) {
	return CreateDelegate(
		0x2377868,
		callbackFunc,
		useOwnerVtable,
		callbackOwner,
		unk,
		executeCallbackFunction
	);
}