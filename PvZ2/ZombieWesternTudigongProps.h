#pragma once
#include "Zombie.h"
#include <Reflection/ReflectionBuilder.h>

class ZombieWesternTudigongProps : public ZombiePropertySheet
{
public:
	float SmashDamage = 1500.0f;
	float SmashDuration = 2.0f;

	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	static void* Construct() {
		auto* instance = new ZombieWesternTudigongProps(); 
		typedef void* (*ctorWithThisPtr)(void*); 
		ctorWithThisPtr baseCtor = (ctorWithThisPtr)getActualOffset(0xC136A4); 
		baseCtor(instance);;
		SetVFTable(instance, (uintptr_t)vftable); 
		return instance;
	};

	static void BuildSymbols(Reflection::CRefManualSymbolBuilder* builder, Reflection::RClass* rclass) {
		Sexy::RtClass* rtClass = StaticGetType(); 
		rtClass->SetRClass(rclass); 
		rclass->m_rtclass = rtClass; 
		Reflection::RClassRef* ref = builder->GetClassRef("ZombiePropertySheet"); 
		Reflection::RClass* parent = ref->GetRClass();;
		void* prop_SmashDamage = (builder)->GetPropertyOfType(GetPropertyType<decltype(std::declval<ZombieWesternTudigongProps>().SmashDamage)>(), sizeof(decltype(std::declval<ZombieWesternTudigongProps>().SmashDamage))); 
		(builder)->RegisterProperty(rclass, "SmashDamage", __builtin_offsetof(ZombieWesternTudigongProps, SmashDamage), prop_SmashDamage);
		void* prop_SmashDuration = (builder)->GetPropertyOfType(GetPropertyType<decltype(std::declval<ZombieWesternTudigongProps>().SmashDuration)>(), sizeof(decltype(std::declval<ZombieWesternTudigongProps>().SmashDuration))); (builder)->RegisterProperty(rclass, "SmashDuration", __builtin_offsetof(ZombieWesternTudigongProps, SmashDuration), prop_SmashDuration);;
		builder->SetRClassWithParent(rclass, parent, 0);
	};

	static void RegisterClass() {
		void* v0 = CallFunc<void*>(0x1624024); 
		if (v0) {
			auto* builder = (Reflection::CRefManualSymbolBuilder*)CallFunc<void*, void*>(0x162413C, v0); 
			if (builder) {
				(builder)->RegisterClassWithProperties("ZombieWesternTudigongProps", ZombieWesternTudigongProps::BuildSymbols, sizeof(ZombieWesternTudigongProps), 0);;
			}
		}
	};
	static Sexy::RtClass* StaticGetType() {
		if (s_rtClass) 
			return s_rtClass; 
		typedef Sexy::RtClass* (*initRtClass)(); 
		Sexy::RtClass* rtClass = ((initRtClass)getActualOffset(0x163A068))(); 
		s_rtClass = rtClass; 
		rtClassGetClassFunc parentGetType = (rtClassGetClassFunc)getActualOffset(0xDA5B00); 
		uintptr_t parent = (uintptr_t)parentGetType(); 
		typedef uintptr_t(*rtClassRegisterClass)(void*, const char*, uintptr_t, ParameterlessConstructorFunc); 
		rtClassRegisterClass regrtclass = *(rtClassRegisterClass*)(*(uintptr_t*)rtClass + 0x40); 
		regrtclass(rtClass, "ZombieWesternTudigongProps", (uintptr_t)parent, ZombieWesternTudigongProps::Construct); 
		ZombieWesternTudigongProps::RegisterClass(); 
		return s_rtClass;
	};
};