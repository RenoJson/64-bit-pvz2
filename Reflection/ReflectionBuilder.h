#pragma once
#include <vector>
#include <map>
#include <string>
#include <cstdint>
#include "CRefManualSymbolBuilder.h"
#include <Sexy/RtClass.h>
#include "RCustomType/TStdVectorManipulator.h"

template <typename T>
using RTMap = std::map<std::string, T>;

template <typename T>
inline Reflection::ReflectionFieldType GetPropertyType()
{
    if (std::is_same<T, bool>::value)
    {
        return Reflection::Type_Bool;
    }
    if (std::is_same<T, int>::value || std::is_same<T, long>::value)
    {
        return Reflection::Type_IntType;
    }
    else if (std::is_same<T, uint>::value || std::is_same<T, ulong>::value)
    {
        return Reflection::Type_UIntType;
    }
    else if (std::is_same<T, float>::value || std::is_same<T, double>::value)
    {
        return Reflection::Type_FloatType;
    }
    else if (std::is_same<T, std::string>::value)
    {
        return Reflection::Type_Char;
    }
}

#define RT_CLASS_GET_SIZE(className, memberName) \
    sizeof(decltype(std::declval<className>().memberName)) \

#define RT_CLASS_GET_TYPE(className, memberName) \
    GetPropertyType<decltype(std::declval<className>().memberName)>()

// Registers a standard type property (int, float, bool...) ONLY use with standard properties. Templated properties have their own macro.
#define RT_CLASS_REGISTER_STANDARD_PROPERTY(className, name) \
    void* prop_##name = (builder)->GetPropertyOfType(RT_CLASS_GET_TYPE(className, name), RT_CLASS_GET_SIZE(className, name)); \
    (builder)->RegisterProperty(rclass, #name, offsetof(className, name), prop_##name); \

#define RT_CLASS_REGISTER_INT_PROPERTY(className, name) \
    void* prop_##name = (builder)->GetPropertyOfType(Reflection::Type_IntType, sizeof(int)); \
    (builder)->RegisterProperty(rclass, #name, offsetof(className, name), prop_##name); \

#define RT_CLASS_REGISTER_STANDARD_VECTOR_PROPERTY(className, name, elementType) \
    void* prop_##name = (builder)->GetPropertyOfType(GetPropertyType<elementType>(), sizeof(elementType)); \
    Reflection::RCustomType::TStdVectorManipulator<elementType>* manip_##name = new Reflection::RCustomType::TStdVectorManipulator<elementType>(); \
    void* regProp_##name = (builder)->RegisterPointerProperty(2, prop_##name, manip_##name); \
    (builder)->RegisterProperty(rclass, #name, offsetof(className, name), regProp_##name); \

#define RT_CLASS_REGISTER_STANDARD_MAP_PROPERTY(className, name, valueType) \
    void* prop_##name = (builder)->GetPropertyOfType(GetPropertyType<valueType>(), sizeof(valueType)); \
    Reflection::RCustomType::TStdVectorManipulator<int>* manip_##name = new Reflection::RCustomType::TStdVectorManipulator<int>(); \
    void* regProp_##name = (builder)->RegisterPointerProperty(6, prop_##name, manip_##name); \
    (builder)->RegisterProperty(rclass, #name, offsetof(className, name), regProp_##name); \

#define RT_CLASS_REGISTER_CLASS_PROPERTY(className, classRefName, name) \
    void* prop_##name = (builder)->GetClassRef(#classRefName); \
    (builder)->RegisterProperty(rclass, #name, offsetof(className, name), prop_##name); \

#define RT_CLASS_REGISTER_CLASS_VECTOR_PROPERTY(className, name, elementType) \
    void* prop_##name = (builder)->GetClassRef(#elementType); \
    Reflection::RCustomType::TStdVectorManipulator<elementType>* manip_##name = new Reflection::RCustomType::TStdVectorManipulator<elementType>(); \
    void* regProp_##name = (builder)->RegisterPointerProperty(2, prop_##name, manip_##name); \
    (builder)->RegisterProperty(rclass, #name, offsetof(className, name), regProp_##name); \

#define RT_CLASS_REGISTER_CLASS_MAP_PROPERTY(className, name, valueType) \
    void* prop_##name = (builder)->GetClassRef(#valueType); \
    Reflection::RCustomType::TStdVectorManipulator<int>* manip_##name = new Reflection::RCustomType::TStdVectorManipulator<int>(); \
    void* regProp_##name = (builder)->RegisterPointerProperty(6, prop_##name, manip_##name); \
    (builder)->RegisterProperty(rclass, #name, offsetof(className, name), regProp_##name); \

#define RT_CLASS_REGISTER_CLASS_RTWEAKPTR_PROPERTY(className, name, valueType) \
    void* prop_##name = (builder)->GetClassRef(#valueType); \
    void* regProp_##name = (builder)->RegisterPointerProperty(3, prop_##name, 0); \
    (builder)->RegisterProperty(rclass, #name, offsetof(className, name), regProp_##name); \

#define RT_CLASS_REGISTER_RTWEAKPTR_VECTOR_PROPERTY(className, name, valueType) \
    void* prop_##name = (builder)->GetClassRef(#valueType); \
    void* rtWeakPtrProp_##name = (builder)->RegisterPointerProperty(3, prop_##name, 0); \
    Reflection::RCustomType::TStdVectorManipulator<RtWeakPtr<valueType>>* manip_##name = new Reflection::RCustomType::TStdVectorManipulator<RtWeakPtr<valueType>>(); \
    void* vecProp_##name = (builder)->RegisterPointerProperty(2, rtWeakPtrProp_##name, manip_##name); \
    (builder)->RegisterProperty(rclass, #name, offsetof(className, name), vecProp_##name); \

// Registers a SexyString class property. ONLY use with SexyString, not any other type of string.
#define RT_CLASS_REGISTER_STRING_PROPERTY(className, name) \
    void* prop_##name = (builder)->GetPropertyOfType(Reflection::Type_Char, sizeof(char)); \
    void* regProp_##name = (builder)->RegisterPointerProperty(0, prop_##name, NULL); \
    (builder)->RegisterProperty(rclass, #name, offsetof(className, name), regProp_##name); \

#define RT_CLASS_REGISTER_STRING_VECTOR_PROPERTY(className, name) \
    void* prop_##name = (builder)->GetPropertyOfType(Reflection::Type_Char, sizeof(char)); \
    Reflection::RCustomType::TStdVectorManipulator<SexyString>* manip_##name = new Reflection::RCustomType::TStdVectorManipulator<SexyString>(); \
    void* strProp_##name = (builder)->RegisterPointerProperty(0, prop_##name, NULL); \
    void* regProp_##name = (builder)->RegisterPointerProperty(2, strProp_##name, manip_##name); \
    (builder)->RegisterProperty(rclass, #name, offsetof(className, name), regProp_##name); \

#define RT_CLASS_REGISTER_CLASS_WITH_PROPERTIES(builder, className) \
    (builder)->RegisterClassWithProperties(#className, className::BuildSymbols, sizeof(className), 0); \

#define DECLARE_STATIC_RT_CLASS_MEMBERS(className) \
    public: \
        static void* vftable; \
        static Sexy::RtClass* s_rtClass; \
        static void ModInit(); 

#define DEFINE_STATIC_RT_CLASS_MEMBERS(className) \
    void* className::vftable = nullptr; \
    Sexy::RtClass* className::s_rtClass = nullptr;

#define RT_CLASS_BUILD_SYMBOLS_BEGIN(parentClassName) \
    static void BuildSymbols(Reflection::CRefManualSymbolBuilder* builder, Reflection::RClass* rclass) \
    { \
        Sexy::RtClass* rtClass = StaticGetType(); \
        rtClass->SetRClass(rclass); \
        rclass->m_rtclass = rtClass; \
        Reflection::RClassRef* ref = builder->GetClassRef(#parentClassName); \
        Reflection::RClass* parent = ref->GetRClass(); \

#define RT_CLASS_BUILD_SYMBOLS_END() \
        builder->SetRClassWithParent(rclass, parent, 0); \
    } \

#define RT_CLASS_BUILD_SYMBOLS_WITHOUT_CLASS_PROPERTIES(parentClassName) \
    static void BuildSymbols(Reflection::CRefManualSymbolBuilder* builder, Reflection::RClass* rclass) \
    { \
        Sexy::RtClass* rtClass = StaticGetType(); \
        rtClass->SetRClass(rclass); \
        rclass->m_rtclass = rtClass; \
        Reflection::RClassRef* ref = builder->GetClassRef(#parentClassName); \
        Reflection::RClass* parent = ref->GetRClass(); \
        builder->SetRClassWithParent(rclass, parent, 0); \
    } \

#define RT_CLASS_REGISTER_CLASS_FUNCTION(className) \
    static void RegisterClass() \
    { \
        void* v0 = CallFunc<void*>(0x1624024); \
        if (v0) \
        { \
            auto* builder = (Reflection::CRefManualSymbolBuilder*)CallFunc<void*, void*>(0x162413C, v0); \
            if (builder) \
            { \
                RT_CLASS_REGISTER_CLASS_WITH_PROPERTIES(builder, className); \
            } \
        } \
    }

#define RT_CLASS_REGISTER_CLASS_AND_STATES_FUNCTION(className) \
    static void RegisterClass() \
    { \
        void* v0 = CallFunc<void*>(0x1624024); \
        if (v0) \
        { \
            auto* builder = (Reflection::CRefManualSymbolBuilder*)CallFunc<void*, void*>(0x162413C, v0); \
            if (builder) \
            { \
                RT_CLASS_REGISTER_CLASS_WITH_PROPERTIES(builder, className); \
            } \
        } \
        className::buildStates(); \
    }

#define RT_CLASS_CONSTRUCT_FUNCTION(className, baseCtorAddr) \
    static void* Construct() \
    { \
        auto* instance = new className(); \
        typedef void* (*ctorWithThisPtr)(void*); \
        ctorWithThisPtr baseCtor = (ctorWithThisPtr)getActualOffset(baseCtorAddr); \
        baseCtor(instance); \
        SetVFTable(instance, (uintptr_t)vftable); \
        return instance; \
    }

#define RT_CLASS_CONSTRUCT_FUNCTION_WITH_TI(className, baseCtorAddr, typeInfoAddr) \
    static void* Construct() \
    { \
        auto* instance = new className(); \
        typedef void* (*ctorWithThisPtr)(void*); \
        ctorWithThisPtr baseCtor = (ctorWithThisPtr)getActualOffset(baseCtorAddr); \
        baseCtor(instance); \
        SetVFTable(instance, (uintptr_t)vftable); \
        ((int*)instance)[4] = getActualOffset(typeInfoAddr); \
        return instance; \
    }

#define RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(className, baseCtorAddr) \
    static void* Construct() \
    { \
        auto* instance = new className(); \
        typedef void* (*ctorWithThisPtr)(void*); \
        ctorWithThisPtr baseCtor = (ctorWithThisPtr)getActualOffset(baseCtorAddr); \
        baseCtor(instance); \

#define RT_CLASS_CONSTRUCT_FUNCTION_END() \
        SetVFTable(instance, (uintptr_t)vftable); \
        return instance; \
    } \

#define RT_CLASS_GET_CLASS_FUNCTION(className, parentGetTypeAddr) \
   static Sexy::RtClass* StaticGetType() \
    { \
        if (s_rtClass) \
            return s_rtClass; \
        \
        typedef Sexy::RtClass* (*initRtClass)(); \
        Sexy::RtClass* rtClass = ((initRtClass)getActualOffset(0x163A068))(); \
        s_rtClass = rtClass; \
        \
        rtClassGetClassFunc parentGetType = (rtClassGetClassFunc)getActualOffset(parentGetTypeAddr); \
        uintptr_t* parent = (uintptr_t*)parentGetType(); \
        \
        typedef uintptr_t (*rtClassRegisterClass)(void*, const char*, uintptr_t, ParameterlessConstructorFunc); \
        rtClassRegisterClass regrtclass = *(rtClassRegisterClass*)(*(uintptr_t*)rtClass + 0x40); \
        regrtclass(rtClass, #className, (uintptr_t)parent, className::Construct); \
        \
        className::RegisterClass(); \
        return s_rtClass; \
    }

#define RT_CLASS_GET_CLASS_WRAPPER(getTypeAddr) \
    static Sexy::RtClass* StaticGetType() \
    { \
        typedef Sexy::RtClass* (*getType)(); \
        getType pGetType = (getType)getActualOffset(getTypeAddr); \
        return pGetType(); \
    } 


#define RT_CLASS_GET_CLASS_NO_PROPERTIES_FUNCTION(className, parentGetTypeAddr) \
    static Sexy::RtClass* StaticGetType() \
    { \
        if (!s_rtClass) \
        { \
            Sexy::RtClass* rtClass = new Sexy::RtClass(); \
            s_rtClass = rtClass; \
            rtClassGetClassFunc parentGetType = (rtClassGetClassFunc)getActualOffset(parentGetTypeAddr); \
            Sexy::RtClass* parent = parentGetType(); \
            rtClass->RegisterClass(#className, parent, className::Construct); \
        } \
        return s_rtClass; \
    }

#define RT_CLASS_DECLARE_ENUM_REGISTER() \
    std::vector<NamedFlagWithId> enumMembers;

#define RT_CLASS_REGISTER_ENUM(enumName) \
    (builder)->RegisterEnum(#enumName, &enumMembers, 0);

#define RT_CLASS_REGISTER_ENUM_VALUE(valueName, valueId) \
    enumMembers.emplace_back(valueName, valueId);