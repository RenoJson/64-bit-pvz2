#pragma once
#include <PvZ2/Zombie.h>
#include <PvZ2/ZombieAnimRig.h>

enum class ZombieCamelSegmentType
{
	none = 0,
	head = 1,
	body = 2,
	tail = 3
};

enum ZombieState_C {
	ZS_C_CamelIdle = 18,
	ZS_C_Surprised = 17
};


class ZombieCamel : public Zombie
{
public:
	RT_CLASS_GET_CLASS_WRAPPER(0xB156D4);
	ZombieCamelSegmentType m_camelType;
	bool m_isACamel;
	bool m_tooCloseToPrev;
	bool m_tooFarFromNext;
	int m_signDamageIndex;
	bool m_spawnDeathAnim;


	virtual bool GetSwitchToHeadCamelSign() {};
	virtual void AssignSegmentArmor(ZombieCamelSegmentType camelType) {};
	virtual SexyString GetSegmentType() {};
	virtual SexyString GetSegmentZombieType() {};
	virtual SexyString Function208() {};
	virtual void Function209() {};
	virtual void MoveCamelBuffer() {};
	virtual void Function211() {};
	virtual void Function212() {};
	virtual void SurprisedOnExit() {};
	virtual void Function214() {};
	virtual void Function215() {};
	virtual void Function216() {};
};

static_assert(sizeof(ZombieCamel) == 1112); 
static_assert(offsetof(ZombieCamel, m_camelType) == 1096);
static_assert(offsetof(ZombieCamel, m_isACamel) == 1100);
static_assert(offsetof(ZombieCamel, m_tooCloseToPrev) == 1101);
static_assert(offsetof(ZombieCamel, m_tooFarFromNext) == 1102);
static_assert(offsetof(ZombieCamel, m_signDamageIndex) == 1104);
static_assert(offsetof(ZombieCamel, m_spawnDeathAnim) == 1108);


class ZombieCamelProps : public ZombiePropertySheet
{
public:
	int CamelSegmentOffset = 60;
	int CamelSegmentTooCloseBuffer = 2;
	int CamelSegmentTooFarBuffer = 6;
	int WalkCycleOffset = -10;
	int SpawnManyCamelSegmentAtPosition = 780;
	bool PropagateSignDestruction;
	bool OnlyHeadZombieEats;
	float SurprisedTime = 3.0f;
	bool DropArmOnSignDrop = true;
	bool SwitchToHeadCamelSign = true;

	float PostMatchStunChance;
	float PostMatchStunDuration = 1.0f;

	int SegmentCount = 0;
	SexyString ZombieTypeName = "mummy";
	SexyString ChooseYourSeedSegmentTypeName = "camel_segment"; // for fixing the CYS visual
	std::vector<SexyString> FollowerSegmentTypeOrder;
	SexyString HeadArmorType;
	SexyString MiddleArmorType;
	SexyString TailArmorType;


	static Reflection::CRefManualSymbolBuilder::BuildSymbolsFunc oZombieCamelPropsBuildSymbols;
	static Sexy::RtClass* s_rtClass;
	static void modInit();

	static void* Construct()
	{
		auto* props = new ZombieCamelProps(); 

		typedef void* (*ctorWithThisPtr)(void*);
		ctorWithThisPtr baseCtor = (ctorWithThisPtr)getActualOffset(0xC136A4);

		baseCtor(props);

		*reinterpret_cast<uintptr_t*>(props) = getActualOffset(0x2432AA8);
		return props;
	}

		RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombiePropertySheet);
		RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieCamelProps, CamelSegmentOffset);
		RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieCamelProps, CamelSegmentTooCloseBuffer);
		RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieCamelProps, CamelSegmentTooFarBuffer);
		RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieCamelProps, WalkCycleOffset);
		RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieCamelProps, SpawnManyCamelSegmentAtPosition);
		RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieCamelProps, PropagateSignDestruction);
		RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieCamelProps, OnlyHeadZombieEats);
		RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieCamelProps, SurprisedTime);
		RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieCamelProps, DropArmOnSignDrop);
		RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieCamelProps, SwitchToHeadCamelSign); 
		RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieCamelProps, SegmentCount);
		RT_CLASS_REGISTER_STRING_PROPERTY(ZombieCamelProps, ZombieTypeName);
		RT_CLASS_REGISTER_STRING_PROPERTY(ZombieCamelProps, ChooseYourSeedSegmentTypeName);
		RT_CLASS_REGISTER_STRING_VECTOR_PROPERTY(ZombieCamelProps, FollowerSegmentTypeOrder);
		RT_CLASS_REGISTER_STRING_PROPERTY(ZombieCamelProps, HeadArmorType);
		RT_CLASS_REGISTER_STRING_PROPERTY(ZombieCamelProps, MiddleArmorType);
		RT_CLASS_REGISTER_STRING_PROPERTY(ZombieCamelProps, TailArmorType);
		RT_CLASS_BUILD_SYMBOLS_END();
		RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieCamelProps);

	static Sexy::RtClass* StaticGetType() {
		if (s_rtClass)
			return s_rtClass;

		typedef Sexy::RtClass* (*initRtClass)();
		Sexy::RtClass* rtClass = ((initRtClass)getActualOffset(0x163A068))();
		s_rtClass = rtClass;

		rtClassGetClassFunc parentGetType = (rtClassGetClassFunc)getActualOffset(0xDA5B00);
		uintptr_t* parent = (uintptr_t*)parentGetType();
		typedef uintptr_t(*rtClassRegisterClass)(void*, const char*, uintptr_t, ParameterlessConstructorFunc);
		rtClassRegisterClass regrtclass = *(rtClassRegisterClass*)(*(uintptr_t*)rtClass + 0x40);
		uintptr_t registeredClassResult = regrtclass(rtClass, "ZombieCamelProps", (uintptr_t)parent, ZombieCamelProps::Construct);
		ZombieCamelProps::RegisterClass();
		return s_rtClass;
	};
};

enum ZombieState_CamelTouch {
	ZS_CAMELTOUCH_WaitingToRise = 18,
	ZS_CAMELTOUCH_RisingFromGround = 19,
	ZS_CAMELTOUCH_Stunned = 20
};


class ZombieCamelTouch : public ZombieCamel
{
public:

	RT_CLASS_GET_CLASS_WRAPPER(0xB192D4);
	int m_cardType;
	int m_cardState;
	pvztime_t m_riseFromGroundDelay;
	pvztime_t m_shadowFadeInStartTime;
	bool m_isTutorialCamel;
	char pad10[4];
	Sexy::RtWeakPtr<Sexy::RtObject> m_signHolder;
	bool m_hasBeenMatched;
	bool m_isStreetZombie;
};

static_assert(sizeof(ZombieCamelTouch) == 1152); 
static_assert(offsetof(ZombieCamelTouch, m_cardType) == 1112);
static_assert(offsetof(ZombieCamelTouch, m_cardState) == 1116);
static_assert(offsetof(ZombieCamelTouch, m_riseFromGroundDelay) == 1120);
static_assert(offsetof(ZombieCamelTouch, m_shadowFadeInStartTime) == 1124);
static_assert(offsetof(ZombieCamelTouch, m_isTutorialCamel) == 1128);
static_assert(offsetof(ZombieCamelTouch, m_signHolder) == 1136);
static_assert(offsetof(ZombieCamelTouch, m_hasBeenMatched) == 1144);
static_assert(offsetof(ZombieCamelTouch, m_isStreetZombie) == 1145);


class ZombieCamelTouchProps : public ZombieCamelProps
{
public:
	static Reflection::CRefManualSymbolBuilder::BuildSymbolsFunc oZombieCamelTouchPropsBuildSymbols;
	static Sexy::RtClass* s_rtClass;
	static void modInit();

	static void* construct()
	{
		auto* props = new ZombieCamelTouchProps();
		typedef void* (*ctorWithThisPtr)(void*);
		ctorWithThisPtr baseCtor = (ctorWithThisPtr)getActualOffset(0xC136A4);
		baseCtor(props);
		*reinterpret_cast<uintptr_t*>(props) = getActualOffset(0x2432B48);
		return props;
	}

		RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombieCamelProps);
		RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieCamelTouchProps, PostMatchStunChance);
		RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieCamelTouchProps, PostMatchStunDuration);
		RT_CLASS_REGISTER_STANDARD_PROPERTY(ZombieCamelTouchProps, SegmentCount);
		RT_CLASS_REGISTER_STRING_PROPERTY(ZombieCamelTouchProps, ZombieTypeName);
		RT_CLASS_REGISTER_STRING_PROPERTY(ZombieCamelTouchProps, ChooseYourSeedSegmentTypeName);
		RT_CLASS_REGISTER_STRING_VECTOR_PROPERTY(ZombieCamelTouchProps, FollowerSegmentTypeOrder);
		RT_CLASS_BUILD_SYMBOLS_END();
		RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieCamelTouchProps);

	static Sexy::RtClass* StaticGetType() {
		if (s_rtClass) {
			return s_rtClass;
		}
		typedef Sexy::RtClass* (*initRtClass)();
		Sexy::RtClass* rtClass = ((initRtClass)getActualOffset(0x163A068))();
		s_rtClass = rtClass;
		Sexy::RtClass* parent = ZombieCamelProps::StaticGetType();
		typedef uintptr_t(*rtClassRegisterClass)(void*, const char*, Sexy::RtClass*, ParameterlessConstructorFunc);
		rtClassRegisterClass regrtclass = *(rtClassRegisterClass*)(*(uintptr_t*)rtClass + 0x40);
		uintptr_t registeredClassResult = regrtclass(rtClass, "ZombieCamelTouchProps", parent, ZombieCamelTouchProps::construct);
		ZombieCamelTouchProps::RegisterClass();
		return s_rtClass;
	};
};


class ZombieAnimRig_Camel : public ZombieAnimRig
{
public:
	char pad[8];
};

class ZombieAnimRig_CamelAlmanac : public ZombieAnimRig_Camel
{
public:
	ZombieAnimRig_CamelAlmanac* m_camelHead;
	ZombieAnimRig_CamelAlmanac* m_camelHump;

};
