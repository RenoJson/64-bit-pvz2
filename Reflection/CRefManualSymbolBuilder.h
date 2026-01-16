#pragma once
#include "RClass.h"
#include "ReflectionFieldType.h"
#include <PvZ2/NamedFlagWithId.h>

namespace Sexy {
    class DelegateBase;
}

namespace Reflection
{
    // Class for reflection symbol builder
    class CRefManualSymbolBuilder
    {
    public:
        typedef void(*BuildSymbolsFunc)(Reflection::CRefManualSymbolBuilder*, Reflection::RClass*);

        void GetInnerTemplateTypeName(const SexyString& typeName)
        {
            typedef void(*func)(SexyString&, const SexyString&);
            func pFunc = (func)getActualOffset(0x161B388);
            SexyString outStr;
            pFunc(outStr, typeName);
        }

        void GetMapInnerTemplateTypeName(const SexyString& typeName)
        {
            typedef void(*func)(SexyString&, const SexyString&);
            func pFunc = (func)getActualOffset(0x161B708);
            SexyString outStr;
            pFunc(outStr, typeName);
        }

        virtual void Function0() {};
        virtual void Function1() {};
        virtual void Function2() {};
        virtual void Function3() {};
        virtual void Function4() {};
        virtual void RegisterClassWithProperties(const SexyString& className, BuildSymbolsFunc buildPropsFunc, uint classSize, bool unk) {};
        virtual void RegisterEnum(const SexyString&, std::vector<NamedFlagWithId>*, int) {};
        virtual void* GetPropertyOfType(ReflectionFieldType type, uint size) {};
        virtual void Function8() {};
        virtual void Function9() {};
        virtual void* RegisterPointerProperty(int, void*, void*) {};
        virtual RClassRef* GetClassRef(const SexyString& className) {};
        virtual void SetRClassWithParent(void*, void*, int) {};
        virtual void RegisterProperty(void* rClass, const SexyString& name, uint offset, void* type) {};
        virtual void Function14() {};
        virtual void RegisterCallback(void* rClass, const SexyString& eventName, Sexy::DelegateBase* delegate, int unk, int unk2) {};
    };
}
