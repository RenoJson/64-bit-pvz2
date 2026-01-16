#pragma once
#include <Sexy/Delegate.h>

typedef std::vector<Sexy::TDelegate<void(Sexy::GC::*)()>> DelegatesVec;

class MessageRouter
{
public:
	virtual void Function0() {};
	virtual void Function1() {};
	virtual void Function2() {};
	virtual DelegatesVec& GetListenersForMessage(int msgId) {};

	template <typename T, typename F>
	void RegisterCallback(int msgId, T& obj, F func)
	{
		auto dlgt = Sexy::make_delegate(func, obj);
		typedef void(*registerFunc)(MessageRouter*, int, void*);
		registerFunc pFunc = (registerFunc)GetActualOffset(0x5FDA54);
		pFunc(this, msgId, &dlgt);
	}

	template <typename T, typename F>
	void RegisterCallback(int msgId, T* obj, F func)
	{
		auto dlgt = Sexy::make_delegate(func, *obj);
		typedef void(*registerFunc)(MessageRouter*, int, void*);
		registerFunc pFunc = (registerFunc)GetActualOffset(0x5FDA54);
		pFunc(this, msgId, &dlgt);
	}

	void DeregisterCallbacksOwnedByObject(void* obj)
	{
		typedef void(*deregisterFunc)(MessageRouter*, void*);
		deregisterFunc pFunc = (deregisterFunc)GetActualOffset(0x5FDE64);
		pFunc(this, obj);
	}

	template <typename... Params>
	int ExecuteMessage(int msgId, Params... args)
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

			// TODO: Revise this to actually use the proper delegate system
			/*
			Sexy::DelegateBase* a = (Sexy::DelegateBase*)&dlgt;

			using foo = void(*)(uint, Params...);
			uint callbackAddr = a->callbackFunc;
			if (a->useOwnerVtable) {
				callbackAddr = *(uint*)a->callbackOwner + a->callbackFunc;
			}
			foo callback = reinterpret_cast<foo>(callbackAddr);

			callback(a->callbackOwner, args...);
			*/
		}

		int result = unk - 1;
		m_unkInt = result;
		if (!result)
		{
			typedef int(*func)(MessageRouter*);
			func pFunc = (func)GetActualOffset(0x5FDDE0);
			return pFunc(this);
		}
		return result;
	}

	static MessageRouter* GetInstance()
	{
		return *(MessageRouter**)GetActualOffset(0x5F1AC0);
	}
private:
	int m_unk[9];
	int m_unkInt;
};