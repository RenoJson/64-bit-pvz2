#pragma once
#include "GameObject.h"
#include "GameObjectDictionary.h"

namespace Sexy {
	class Graphics;
}
enum class TeamFlags
{
	none = 0,
	plants = 1,
	zombies = 2,
	everything = 3
};
// An object class for rendering visuals
class RealObject : public GameObject, public Renderable
{
public:
	Sexy::SexyVector3 m_position;
	int m_teamFlags;
	int m_realObjectFlags;

	virtual void UpdatePosition(Sexy::SexyVector3& newPos) {};
	virtual void QueueDraw(Sexy::Graphics* g) {};
	virtual void Draw(Sexy::Graphics* g) {};
	virtual bool HasShadow() {};
	virtual void Function17() {};
	virtual bool CanSink() {};
	virtual void UpdateSinkDistance(bool noOverride) {}; // override a deeper sink distance with a more shallow one
	virtual Sexy::Rect GetHitRect() {};
	virtual void Function21() {};
	virtual void Function22() {};
	virtual int GetRenderOrder() {};
	virtual Sexy::FPoint GetGridExtents() {};
	virtual void Function25() {};
	virtual void Function26() {};
	virtual void RenderAnimRigs(Sexy::Graphics* g) {};
	virtual void RenderShadows(Sexy::Graphics* g) {};
	virtual void Update() {}; // runs every frame
	virtual void UpdateGroundClipRect(Sexy::Graphics* graphics) {}; // uses to clip the ground for ground effects
	virtual float GetMaxTideLoweredPercent() {};
	virtual float GetSinkDistance() {};
};

static_assert(sizeof(RealObject) == 48);
static_assert(offsetof(RealObject, m_position) == 24);
static_assert(offsetof(RealObject, m_teamFlags) == 36);
static_assert(offsetof(RealObject, m_realObjectFlags) == 40);

// RealObject that supports color effects
class ModularRealObject : public RealObject
{
public:
	AttachedEffectManager m_attachedEffects;
	virtual void Function33() {};
	virtual void Function34() {};
};

static_assert(sizeof(ModularRealObject) == 80);
static_assert(offsetof(ModularRealObject, m_attachedEffects) == 48);