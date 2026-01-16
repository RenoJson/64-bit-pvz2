#pragma once

namespace Sexy
{
	class DelegateBase
	{
	public:
		uint m_callbackFunc;
		int m_useOwnerVtable;
		uint m_callbackOwner;
		int m_unk;
		uint m_executeCallbackFunction;

		virtual void Function0() {};
		virtual void Function1() {};
		virtual void Function2() {};
		virtual void Delete() {};
	};
}

inline Sexy::DelegateBase CreateEmptyDelegate(uint vftableAddr)
{
	Sexy::DelegateBase delegate = Sexy::DelegateBase();
	*(uint*)&delegate = getActualOffset(vftableAddr);
	return delegate;
}

inline Sexy::DelegateBase CreateDelegate(
	uint vftableAddr,
	uint callbackFunc,
	bool useOwnerVtable,
	uint callbackOwner,
	int unk,
	uint executeCallbackFunction
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
	uint callbackFunc,
	bool useOwnerVtable,
	uint callbackOwner,
	int unk,
	uint executeCallbackFunction
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