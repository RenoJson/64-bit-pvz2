#pragma once

#define RTOBJECT_INDEX_GETTYPE 0
#define RTOBJECT_INDEX_DTOR 2
#define RTOBJECT_INDEX_DEALLOC 3
#define RTOBJECT_INDEX_ISTYPE 4
#define RTOBJECT_INDEX_SERIALIZE 6

namespace Sexy
{
	class RtClass;

	class RtObject
	{
	public:
		virtual RtClass* GetType() {};
		virtual bool Function1() {};
		virtual void* Destroy() {};
		virtual void DestroyAndDealloc() {};
		virtual bool IsType(RtClass* type) {};
		virtual bool Function5() {};
		virtual bool Serialize(RtClass* type) {};
	};

	static_assert(sizeof(RtObject) == 8);
}