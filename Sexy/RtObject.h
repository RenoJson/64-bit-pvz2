#pragma once

#define RTOBJECT_INDEX_GETTYPE 0
#define RTOBJECT_INDEX_DTOR 2
#define RTOBJECT_INDEX_DEALLOC 3
#define RTOBJECT_INDEX_ISTYPE 4
#define RTOBJECT_INDEX_SERIALIZE 6

namespace Sexy
{
	class RtClass;

	// Base class for all Reflection Type objects
	// All deriven classes can use reflection of it's properites
	// Using GameObject is advised though
	class RtObject
	{
	public:
		// Returns the RtClass of this type
		RtClass* GetType() {};

		// i have no fucking idea what this and func5 even does
		// the returned boolean values never change
		virtual bool Function1() {};

		// A destructor that executes some final code right before getting deallocated
		// It's often a default dtor
		virtual void* Destroy() {};

		// this calls the Destroy function and then deletes the allocated instance
		virtual void DestroyAndDealloc() {};

		// Checks if an rtclass is the same type as this object's
		virtual bool IsType(RtClass* type) {};
		virtual bool Function5() {};
		virtual bool Serialize(RtClass* type) {};
	};

	static_assert(sizeof(RtObject) == 8);
}