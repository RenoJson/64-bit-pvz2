#pragma once
#include "GridItem.h"
#include "Projectile.h"

class GridItemSpeakerBass : public GridItemSpeaker {
public:
	float m_nextFireTime;
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit(); 
	static void SpeakerSetState(GridItemSpeakerBass* speaker, int state);
	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(GridItemSpeakerBass, 0xD99F38);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(GridItemSpeaker);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(GridItemSpeakerBass);
	RT_CLASS_GET_CLASS_FUNCTION(GridItemSpeakerBass, 0xD9BE78);
};

class GridItemSpeakerBassProps : public GridItemSpeakerProps {
public:

	Sexy::RtWeakPtr<ProjectilePropertySheet> ShockWaveProjectile;
	SexyVector2 ShockWaveSpawnOffset;
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();
	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(GridItemSpeakerBassProps, 0x9D0DF8);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(GridItemSpeakerProps);
	RT_CLASS_REGISTER_CLASS_RTWEAKPTR_PROPERTY(GridItemSpeakerBassProps, ShockWaveProjectile, ProjectilePropertySheet)
	RT_CLASS_REGISTER_CLASS_PROPERTY(GridItemSpeakerBassProps, SexyVector2, ShockWaveSpawnOffset);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(GridItemSpeakerBassProps);
	RT_CLASS_GET_CLASS_FUNCTION(GridItemSpeakerBassProps, 0xD9904C);
};