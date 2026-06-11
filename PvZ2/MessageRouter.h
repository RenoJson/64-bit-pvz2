#pragma once
#include <Sexy/Delegate.h>

typedef std::vector<Sexy::TDelegate<void(Sexy::GC::*)()>> DelegatesVec;

class MessageRouter
{
public:
	virtual void Function0() {};
	virtual void Function1() {};
	virtual void Function2() {};
	virtual DelegatesVec& GetListenersForMessage(void* msgId) {};

	template <typename T, typename F>
	void RegisterCallback(void* msgId, T& obj, F func)
	{
		auto dlgt = Sexy::make_delegate(func, obj);
		CallFunc<void, MessageRouter*, void*, void*>(0x96C4D8, this, msgId, &dlgt);
	}

	void DeregisterCallbacksOwnedByObject(void* obj)
	{
		CallFunc<void, MessageRouter*, void*>(0x96CAD4, this, obj);
	}

	template <typename... Params>
	void ExecuteMessage(void* msgId, Params... args)
	{
		DelegatesVec& listeners = GetListenersForMessage(msgId);

		int unk = m_unkInt + 1;
		m_unkInt = unk;

		for (DelegatesVec::iterator it = listeners.begin(); it != listeners.end(); ++it)
		{
			// WTF
			Sexy::TDelegate<void(Sexy::GC::*)()> delegateWithoutParams = *it;
			void* voidPtr = reinterpret_cast<void*>(&delegateWithoutParams);
			Sexy::TDelegate<void(Sexy::GC::*)(Params...)> dlgt = *reinterpret_cast<Sexy::TDelegate<void(Sexy::GC::*)(Params...)>*>(voidPtr);

			// Unholy
			dlgt.executeCallbackFunc_(&dlgt, args...);
		}

		int result = unk - 1;
		m_unkInt = result;
		if (!result)
		{
			return CallFunc<void, MessageRouter*>(0x96C98C, this);
		}
	}

	static MessageRouter* GetInstance()
	{
		return *(MessageRouter**)getActualOffset(0x2513070);
	}

private:
	int m_unk[9];
	int m_unkInt;
};