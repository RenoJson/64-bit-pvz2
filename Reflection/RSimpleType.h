#pragma once
#include "ReflectionFieldType.h"

namespace Reflection
{
	class RSimpleType
	{
	public:
		char m_pad[52];
		uint m_size;
		char m_pad2[4];
		ReflectionFieldType m_type;
	};

	//static_assert(sizeof(RSimpleType) == 168);
	//static_assert(offsetof(RSimpleType, m_size) == 104);
	//static_assert(offsetof(RSimpleType, m_type) == 104);
}