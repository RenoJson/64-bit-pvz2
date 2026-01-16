#pragma onceRT_CLASS_REGISTER_STRING_PROPERTY
#include <PvZ2/GridItemAnimation.h>
#include <Reflection/ReflectionBuilder.h>

class GridItemSewer : public GridItemAnimation
{
	DECLARE_STATIC_RT_CLASS_MEMBERS(GridItemSewer);

	SexyString m_lastZombie;
	// used to restrict spawning so nothing happens when its not necessary
	bool m_disabled = true;
	bool m_spawnBackwards = false;
	int m_cycleIndex = 0;

	// this vector gets redefined every 3-5 zombie spawns
	// it will avoid any excessive repetitiveness
	std::vector<SexyString> m_zombiePool;

	RT_CLASS_CONSTRUCT_FUNCTION(GridItemSewer, GRIDITEM_ANIM_CTOR);

	RT_CLASS_BUILD_SYMBOLS_BEGIN(GridItemAnimation);
		RT_CLASS_REGISTER_STRING_PROPERTY(GridItemSewer, m_lastZombie);
		RT_CLASS_REGISTER_STANDARD_PROPERTY(GridItemSewer, m_disabled);
		RT_CLASS_REGISTER_STANDARD_PROPERTY(GridItemSewer, m_spawnBackwards);
		RT_CLASS_REGISTER_STANDARD_PROPERTY(GridItemSewer, m_cycleIndex);
		RT_CLASS_REGISTER_STRING_VECTOR_PROPERTY(GridItemSewer, m_zombiePool);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(GridItemSewer);

	RT_CLASS_GET_CLASS_FUNCTION(GridItemSewer, GRIDITEM_ANIM_GETTYPE);

	static void* Destroy(GridItemSewer* self) {
		return CallFunc<void*, GridItemSewer*>(0x76D350, self);
	}

	static void DestroyAndDealloc(GridItemSewer* self) {
		void* destroyed = Destroy(self);
		operator delete(destroyed);
	}
};

class GridItemSewerProps : public GridItemAnimationProps
{
	DECLARE_STATIC_RT_CLASS_MEMBERS(GridItemSewerProps);

	pvztime_t SpawnStartTime = 5.0f;
	pvztime_t SpawnTimeInterval = 12.0f;

	// contains all allowed zombie types
	// the weight is going to be taken from their props
	ZombieRestrictionSet ZombieTypes;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(GridItemSewerProps, GRIDITEM_ANIMPROPS_CTOR);
		instance->ZombieTypes.List.clear();
		instance->ZombieTypes.List.shrink_to_fit();
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(GridItemAnimationProps);
		RT_CLASS_REGISTER_STANDARD_PROPERTY(GridItemSewerProps, SpawnStartTime);
		RT_CLASS_REGISTER_STANDARD_PROPERTY(GridItemSewerProps, SpawnTimeInterval);
		RT_CLASS_REGISTER_CLASS_PROPERTY(GridItemSewerProps, ZombieRestrictionSet, ZombieTypes);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(GridItemSewerProps);

	RT_CLASS_GET_CLASS_FUNCTION(GridItemSewerProps, GRIDITEM_ANIMPROPS_GETTYPE);

	static void* Destroy(GridItemSewerProps* self) {
		return CallFunc<void*, GridItemSewerProps*>(0x6BD174, self);
	}

	static void DestroyAndDealloc(GridItemSewerProps* self) {
		void* destroyed = Destroy(self);
		operator delete(destroyed);
	}
};