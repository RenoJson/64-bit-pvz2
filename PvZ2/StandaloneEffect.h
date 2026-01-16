#pragma once
#include "GameObject.h"
#include "ZombieEvent.h"

class RealObject;

class StandaloneEffect : public GameObject
{
public:
	int m_standaloneEffectFlags;
	Sexy::SexyVector2 m_screenSpaceOrigin;
	int m_renderLayerOverride;
	int m_renderLayer;
	char m_pad[4];
	Sexy::RtWeakPtr<RealObject> m_attachmentTarget;
	Sexy::SexyVector2 m_attachmentScreenSpaceOffset;
	int m_attachmentRenderLayerOffset;
	ZombieEvent m_onCompletion;
	bool m_shouldDraw;
	char m_pad2[20];
};

static_assert(sizeof(StandaloneEffect) == 176);
static_assert(offsetof(StandaloneEffect, m_standaloneEffectFlags) == 16);
static_assert(offsetof(StandaloneEffect, m_attachmentTarget) == 40);
static_assert(offsetof(StandaloneEffect, m_attachmentScreenSpaceOffset) == 48);
static_assert(offsetof(StandaloneEffect, m_onCompletion) == 64);
static_assert(offsetof(StandaloneEffect, m_shouldDraw) == 152);