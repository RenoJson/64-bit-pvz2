#pragma once
#include "RtObject.h"
#include <Reflection/RClass.h>

typedef void* (*ParameterlessConstructorFunc)();

namespace Sexy
{
	class RtClass : public RtObject
	{
    public:
        const char* m_typeName;
        RtClass* m_parentType;
        void* m_fConstructor;
        char m_pad[32];
        Reflection::RClass* m_rclass;

        RtClass()
        {
            typedef void(*rtClassCtorFunc)(void*);
            rtClassCtorFunc rtClassCtor = (rtClassCtorFunc)getActualOffset(0x163A40C);
            rtClassCtor(this);
        }
		
        void SetRClass(Reflection::RClass* rclass) {
			m_rclass = rclass;
		}

        virtual bool Function7(int64_t unk) {};
        virtual void* RegisterClass(const char* name, RtClass* super, ParameterlessConstructorFunc ctor) {};
	};

    static_assert(sizeof(RtClass) == 72);
    static_assert(offsetof(RtClass, m_typeName) == 8);
    static_assert(offsetof(RtClass, m_rclass) == 64);
}

typedef Sexy::RtClass* (*rtClassCtorFunc)();
typedef Sexy::RtClass* (*rtClassGetClassFunc)();
typedef void(*rtClassSetRClassFunc)(Sexy::RtClass*, Reflection::RClass*);