#pragma once
#include "RField.h"

namespace Sexy {
    class RtClass;
}

namespace Reflection
{
    // Reflection Class
    class RClass
    {
    public:
        char m_pad[104];
        uint m_classSize;
        char m_pad2[28];
        Sexy::RtClass* m_rtclass;
        std::vector<RField*> m_fields;
    };

    static_assert(sizeof(RClass) == 168);
    static_assert(offsetof(RClass, m_classSize) == 104);
    static_assert(offsetof(RClass, m_rtclass) == 136);

    class RClassRef
    {
    public:
        virtual void Function0() {}
        virtual void Function1() {}
        virtual void Function2() {}
        virtual void Function3() {}
        virtual void Function4() {}
        virtual void Function5() {}
        virtual void Function6() {}
        virtual void Function7() {}
        virtual void Function8() {}
        virtual void Function9() {}
        virtual void Function10() {}
        virtual void Function11() {}
        virtual void Function12() {}
        virtual RClass* GetRClass() {}
    };

    static_assert(sizeof(RClassRef) == 8);
}