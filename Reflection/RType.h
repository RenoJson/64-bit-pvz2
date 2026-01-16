#pragma once
#include <Sexy/RtObject.h>

namespace Reflection
{
	class RType : Sexy::RtObject
	{
	public:
		virtual void Function7() {};
		virtual void Function8() {};
		virtual void Function9() {};
		virtual void Function10() {};
		virtual void Function11(void* element, void* manipInfo, SexyString& string) {};
		virtual void Function12() {};
	};

	static_assert(sizeof(RType) == 8);
}