#pragma once

namespace Reflection
{
    // Reflection int type used in BuildSymbols
    // It is being used by the game to tell the field's type
    enum ReflectionFieldType
    {
        Type_Class = 0,
        Type_Elipsis = 1,
        Type_Void = 2,
        Type_Bool = 3,
        Type_Char = 4, // It's also a std::string type
        Type_Wchar_t = 5,
        Type_IntType = 6,
        Type_UIntType = 7,
        Type_FloatType = 8,
        Type_HRESULT = 9,
    };
}