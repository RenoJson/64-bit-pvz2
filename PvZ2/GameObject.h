#pragma once
#include <Sexy/RtObject.h>
#include <Sexy/RtWeakPtr.h>

class Renderable
{
public:
	virtual ~Renderable() {};
	virtual void Draw(void* g) {};
	virtual int GetRenderOrder() {};
};

// Base Class for all in-game objects with JSON serialization
// Action frames, prop sheets, board entities etc all derive from it
class GameObject : public Sexy::RtObject
{
public:
	// self pointer I think
	Sexy::RtWeakPtr<RtObject> m_thisPtr;

	// Called when the object is serialized from a JSON
	virtual int RegisterForEvents() {};
	virtual int Function8() {};
	virtual void Function9() {};
	virtual void Function10() {};
	virtual void OnInitialized() {};
	virtual int DestroyObj() {};
};

static_assert(sizeof(GameObject) == 16);
static_assert(offsetof(GameObject, m_thisPtr) == 8);

// GameSubsystem base class
class GameSubsystem : public GameObject
{};